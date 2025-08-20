#include "uart.h"
#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 16000000UL   // default 16 MHz if not provided by Makefile
#endif

#ifndef BAUD
#define BAUD 9600          // default baud if not overridden
#endif

#define MYUBRR (F_CPU/16/BAUD - 1)

// Forward declaration for stream redirection
static int uart_putchar(char c, FILE *stream);

// Setup FILE stream so printf() works
static FILE uart_stdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

void uart_init(void) {
    // Set baud rate
    UBRR0H = (unsigned char)(MYUBRR >> 8);
    UBRR0L = (unsigned char)(MYUBRR);

    // Enable transmitter (TX only)
    UCSR0B = (1 << TXEN0);

    // Frame format: 8 data bits, 1 stop bit, no parity
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    // Redirect stdout to UART
    stdout = &uart_stdout;
}

static int uart_putchar(char c, FILE *stream) {
    if (c == '\n') {
        // Add carriage return for Serial Monitor compatibility
        uart_putchar('\r', stream);
    }

    // Wait for transmit buffer to be empty
    while (!(UCSR0A & (1 << UDRE0)));

    // Send character
    UDR0 = c;

    return 0;
}
