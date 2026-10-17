extern void uart_put_char(char c);
extern char uart_get_char(void);

void kmain(void) {
    uart_put_char('A');
    while (1) {
      char c = uart_get_char();
      uart_put_char(c);
    }
}
