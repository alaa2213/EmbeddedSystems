#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

// UART Configuration
#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// Buffer for receiving data
#define BUFFER_SIZE 256

void uart_init_custom() {
    // Initialize UART
    uart_init(UART_ID, BAUD_RATE);
    
    // Set the TX and RX pins
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    
    // Enable UART FIFO
    uart_set_fifo_enabled(UART_ID, true);
}

void send_ocr_request() {
    const char *command = "START_OCR\n";
    uart_puts(UART_ID, command);
    printf("Sent: START_OCR\n");
}

bool receive_ocr_response(char *buffer, size_t buffer_size) {
    uint32_t timeout = 10000; // 10 seconds timeout in ms
    uint32_t start_time = to_ms_since_boot(get_absolute_time());
    int index = 0;
    
    while ((to_ms_since_boot(get_absolute_time()) - start_time) < timeout) {
        if (uart_is_readable(UART_ID)) {
            char c = uart_getc(UART_ID);
            
            if (c == '\n') {
                buffer[index] = '\0';
                return true;
            }
            
            if (index < buffer_size - 1) {
                buffer[index++] = c;
            }
        }
        sleep_ms(10);
    }
    
    printf("Timeout waiting for OCR response\n");
    return false;
}

int main() {
    stdio_init_all();
    uart_init_custom();
    
    printf("Milestone1 - OCR Test\n");
    sleep_ms(2000); // Wait for Pi to be ready
    
    while (true) {
        printf("\nPress any key to trigger OCR scan...\n");
        getchar(); // Wait for user input
        
        // Send OCR request to Raspberry Pi
        send_ocr_request();
        
        // Wait for response
        char response[BUFFER_SIZE];
        if (receive_ocr_response(response, BUFFER_SIZE)) {
            printf("Received: %s\n", response);
            
            // Parse the response
            if (strncmp(response, "CLUE:", 5) == 0) {
                char *clue = response + 5; // Skip "CLUE:"
                printf("✓ OCR Success! Clue: %s\n", clue);
                // TODO: Process the clue here
            } 
            else if (strncmp(response, "ERROR:", 6) == 0) {
                printf("✗ OCR Failed: %s\n", response + 6);
            }
        }
        
        sleep_ms(1000);
    }
}
