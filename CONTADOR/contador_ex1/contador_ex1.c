#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "inc/ssd1306.h" // Verifique se o caminho está correto

// --- Definições de Pinos ---
#define I2C_BUS_SDA_PIN     14 // Pino GPIO para I2C Data
#define I2C_BUS_SCL_PIN     15 // Pino GPIO para I2C Clock
#define PRIMARY_BUTTON_PIN  5  // Pino GPIO para Botão A (Resetar/Iniciar)
#define SECONDARY_BUTTON_PIN 6 // Pino GPIO para Botão B (Incrementar durante contagem)

// --- Variáveis de Estado Compartilhadas (volatile para ISRs) ---
volatile int  seconds_left          = 0;    // Valor atual do contador (0-9)
volatile int  secondary_press_count = 0;    // Contagem de cliques no botão B durante contagem ativa
volatile bool is_timer_running      = false;  // Flag: contador ativo?
volatile bool needs_display_update  = false;  // Flag: atualizar display?

// --- Variáveis do Display ---
struct render_area display_region; // Área de renderização na tela
uint8_t oled_buffer[ssd1306_buffer_length]; // Buffer do display SSD1306

// --- Handle do Timer ---
static repeating_timer_t interval_timer_handle; // Handle para o timer periódico

// --- Callback do Timer Periódico (executado a cada 1s) ---
// Decrementa o contador se estiver ativo.
bool periodic_timer_task(repeating_timer_t *rt) {
    if (is_timer_running && seconds_left > 0) {
        seconds_left--;             // Decrementa tempo restante
        needs_display_update = true; // Sinaliza atualização da tela

        // Contagem terminou?
        if (seconds_left == 0) {
            is_timer_running = false; // Para a lógica do timer

            // Desabilita IRQ do botão secundário fora do modo ativo
            gpio_set_irq_enabled(SECONDARY_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, false);
        }
    }
    return true; // Continua o timer
}

// --- ISR para Eventos GPIO (Botões) ---
// Trata eventos de borda de descida (pressionar botão).
void button_event_handler(uint gpio_pin, uint32_t event_mask) {
    // Ignora outros eventos que não sejam borda de descida
    if ((event_mask & GPIO_IRQ_EDGE_FALL) == 0) return;

    // --- Lógica Botão Primário (Iniciar/Resetar) ---
    if (gpio_pin == PRIMARY_BUTTON_PIN) {
        seconds_left          = 9;    // Reinicia contador para 9
        secondary_press_count = 0;    // Zera contador de cliques B
        is_timer_running      = true;   // Ativa lógica do timer
        needs_display_update  = true;  // Sinaliza atualização

        // Reabilita IRQ do botão secundário (pode ter sido desabilitada)
        gpio_set_irq_enabled(SECONDARY_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true);
    }
    // --- Lógica Botão Secundário (Incrementar durante contagem) ---
    else if (gpio_pin == SECONDARY_BUTTON_PIN) {
        // Só conta cliques se o timer estiver ativo
        if (is_timer_running) {
            secondary_press_count++;    // Incrementa contador
            needs_display_update = true; // Sinaliza atualização
        }
    }
}

// --- Ponto de Entrada Principal ---
int main(void) {
    // Inicializa stdio (para printf)
    stdio_init_all();
    sleep_ms(1000); // Pequeno delay para console serial conectar
    printf("Sistema inicializando...\n");

    // --- Inicialização I2C para Display SSD1306 ---
    i2c_init(i2c1, ssd1306_i2c_clock * 1000); // Usa i2c1, define frequência
    gpio_set_function(I2C_BUS_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_BUS_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_BUS_SDA_PIN); // Habilita pull-ups internos do I2C
    gpio_pull_up(I2C_BUS_SCL_PIN);
    printf("I2C inicializado.\n");

    // --- Inicialização Display SSD1306 ---
    ssd1306_init(); // Inicializa controlador do display
    printf("SSD1306 inicializado.\n");

    // Define tela cheia como área de renderização
    display_region.start_column = 0;
    display_region.end_column   = ssd1306_width - 1;
    display_region.start_page   = 0;
    display_region.end_page     = ssd1306_n_pages - 1;
    calculate_render_area_buffer_length(&display_region); // Calcula tamanho necessário (usamos buffer completo)

    // Limpa buffer e display
    memset(oled_buffer, 0, ssd1306_buffer_length);
    render_on_display(oled_buffer, &display_region);
    printf("Display limpo.\n");

    // --- Inicialização GPIO dos Botões ---
    // Botão Primário (A)
    gpio_init(PRIMARY_BUTTON_PIN);
    gpio_set_dir(PRIMARY_BUTTON_PIN, GPIO_IN); // Define como entrada
    gpio_pull_up(PRIMARY_BUTTON_PIN);         // Habilita pull-up interno

    // Botão Secundário (B)
    gpio_init(SECONDARY_BUTTON_PIN);
    gpio_set_dir(SECONDARY_BUTTON_PIN, GPIO_IN); // Define como entrada
    gpio_pull_up(SECONDARY_BUTTON_PIN);         // Habilita pull-up interno
    printf("Botões inicializados.\n");

    // --- Configuração de Interrupções ---
    // Registra callback compartilhado para borda de descida do botão primário
    gpio_set_irq_enabled_with_callback(PRIMARY_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_event_handler);

    // Habilita IRQ de borda de descida para botão secundário (mesmo callback)
    // Será habilitado/desabilitado pela lógica do timer/botão A.
    gpio_set_irq_enabled(SECONDARY_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true);
    printf("Interrupções configuradas.\n");

    // --- Inicialização Timer Periódico ---
    if (!add_repeating_timer_ms(1000, periodic_timer_task, NULL, &interval_timer_handle)) {
       printf("Erro: Falha ao adicionar timer periódico!\n");
       while(true); // Trava em caso de erro
    }
    printf("Timer periódico iniciado.\n");

    // --- Loop Principal da Aplicação ---
    char display_line_buffer[32]; // Buffer para formatar linhas para o display
    printf("Entrando no loop principal.\n");
    while (true) {
        // Verifica se precisa atualizar o display (flag setada por ISRs/timer)
        if (needs_display_update) {
            needs_display_update = false; // Limpa a flag

            // Limpa buffer antes de desenhar
            memset(oled_buffer, 0, ssd1306_buffer_length);

            // Formata e desenha linha 1 (Tempo)
            sprintf(display_line_buffer, "Tempo: %d", seconds_left);
            ssd1306_draw_string(oled_buffer, 5, 0, display_line_buffer); // x=5, y=0

            // Formata e desenha linha 2 (Cliques B)
            sprintf(display_line_buffer, "Acoes B: %d", secondary_press_count);
            ssd1306_draw_string(oled_buffer, 5, 8, display_line_buffer); // x=5, y=8

            // Envia buffer atualizado para o display OLED
            render_on_display(oled_buffer, &display_region);
        }


        sleep_ms(100);
    }

    // Código inalcançável
    return 0;
}
