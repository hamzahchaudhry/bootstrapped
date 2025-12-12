#include <stdint.h>

#define UART0_BASE 0x10000000u  // whatever your SoC uses
#define UART0_TX   (*(volatile uint32_t *)(UART0_BASE + 0x0))

static void uart_putc(char c) {
    // possibly poll a status register here if needed
    UART0_TX = (uint32_t)c;
}

void puts(const char *s) {
    while (*s) {
        if (*s == '\n')
            uart_putc('\r');
        uart_putc(*s++);
    }
}

void kernel_main(void) {
    puts("Hello from rv32i kernel!\n");
    for (;;) { /* spin */ }
}

