#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/watchdog.h"
#include "pico/bootrom.h"

// Definições dos pinos dos LEDs RGB e do Buzzer
#define RED_PIN 13
#define GREEN_PIN 11
#define BLUE_PIN 12
#define BUZZER_PIN 10

// Função para gerar som "beep" com o buzzer
void beep(uint pin, int duration, int frequency) {
    int period = 1000000 / frequency; // Período em microsegundos
    int half_period = period / 2;    // Meio período para alternar

    for (int i = 0; i < (duration * 1000) / period; i++) {
        gpio_put(pin, true);        // Liga o buzzer
        sleep_us(half_period);      // Espera meio período
        gpio_put(pin, false);       // Desliga o buzzer
        sleep_us(half_period);      // Espera meio período
    }
}

// Função para validar a entrada do usuário
int isValidCommand(char input) {
    char validCommands[] = {'r', 'g', 'b', 'o', 'a', 'p', 'q'};
    int numCommands = sizeof(validCommands) / sizeof(validCommands[0]);

    for (int i = 0; i < numCommands; i++) {
        if (input == validCommands[i]) {
            return 1; // Entrada válida
        }
    }
    return 0; // Entrada inválida
}

int main() {
    // Inicializa as interfaces padrão de I/O
    stdio_init_all();

    // Inicializa os pinos dos LEDs como saída
    gpio_init(RED_PIN);
    gpio_set_dir(RED_PIN, GPIO_OUT);
    gpio_init(GREEN_PIN);
    gpio_set_dir(GREEN_PIN, GPIO_OUT);
    gpio_init(BLUE_PIN);
    gpio_set_dir(BLUE_PIN, GPIO_OUT);

    // Inicializa o pino do Buzzer como saída
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);

    char userInput;

    // Loop principal
    while (1) {
        // Solicita entrada do usuário
        printf("Lista de Comandos:\n a = branco \n r = vermelho \n g = verde \n b = azul \n o = desligar LEDs \n p = beep \n q = reboot para gravação\n");
        userInput = getchar();
        getchar(); // Consome o caractere '\n' residual

        // Valida a entrada do usuário
        if (!isValidCommand(userInput)) {
            printf("Comando inválido! Por favor, tente novamente.\n");
            continue; // Volta para o início do loop
        }

        // Processa o comando recebido
        switch (userInput) {
            case 'r':
                // Liga o LED vermelho
                gpio_put(RED_PIN, 1);
                gpio_put(GREEN_PIN, 0);
                gpio_put(BLUE_PIN, 0);
                printf("LED vermelho ligado!\n");
                break;
            case 'g':
                // Liga o LED verde
                gpio_put(RED_PIN, 0);
                gpio_put(GREEN_PIN, 1);
                gpio_put(BLUE_PIN, 0);
                printf("LED verde ligado!\n");
                break;
            case 'b':
                // Liga o LED azul
                gpio_put(RED_PIN, 0);
                gpio_put(GREEN_PIN, 0);
                gpio_put(BLUE_PIN, 1);
                printf("LED azul ligado!\n");
                break;
            case 'o':
                // Desliga todos os LEDs
                gpio_put(RED_PIN, 0);
                gpio_put(GREEN_PIN, 0);
                gpio_put(BLUE_PIN, 0);
                printf("Todos os LEDs desligados!\n");
                break;
            case 'a':
                // Liga todos os LEDs
                gpio_put(RED_PIN, 1);
                gpio_put(GREEN_PIN, 1);
                gpio_put(BLUE_PIN, 1);
                printf("Todos os LEDs ligados!\n");
                break;

            case 'p':
                // Desliga todos os LEDs
                gpio_put(RED_PIN, 0);
                gpio_put(GREEN_PIN, 0);
                gpio_put(BLUE_PIN, 0);
                // Aciona o buzzer por 2 segundos
                beep(BUZZER_PIN, 2000, 660);
                printf("Buzzer acionado por 2 segundos!\n");
                break;

            case 'q':
                // Entra no modo de boot para gravação
                printf("Reiniciando no modo de bootloader USB...\n");
                sleep_ms(1000); // Pausa para permitir leitura da mensagem
                reset_usb_boot(0, 0);
                break;

            default:
                printf("Digite uma entrada válida!\n");
                break;
        }
    }
}
