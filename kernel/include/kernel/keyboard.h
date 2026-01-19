#ifndef _KERNEL_KEYBOARD_H
#define _KERNEL_KEYBOARD_H

unsigned char keyboard_poll(void);
unsigned char keyboard_getchar(void);
void keyboard_irq_handler(void);

#endif
