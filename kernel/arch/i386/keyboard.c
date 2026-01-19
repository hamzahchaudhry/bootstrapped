#include <stdint.h>

#include <kernel/keyboard.h>
#include <kernel/pic.h>

#include "io.h"

static const uint16_t DATA_PORT = 0x60;

/* simple ring buffer for IRQ-driven input */
#define KBD_BUF_SIZE 64
static volatile uint8_t kbd_buf[KBD_BUF_SIZE];
static volatile uint8_t kbd_head;
static volatile uint8_t kbd_tail;

/* set-1 scancode -> ASCII for unmodified keys. zero for non-printables. */
static const unsigned char scancode_ascii[128]
  = {[0x02] = '1', [0x03] = '2',  [0x04] = '3',  [0x05] = '4',  [0x06] = '5',
     [0x07] = '6', [0x08] = '7',  [0x09] = '8',  [0x0A] = '9',  [0x0B] = '0',
     [0x0C] = '-', [0x0D] = '=',  [0x0E] = '\b', [0x0F] = '\t', [0x10] = 'q',
     [0x11] = 'w', [0x12] = 'e',  [0x13] = 'r',  [0x14] = 't',  [0x15] = 'y',
     [0x16] = 'u', [0x17] = 'i',  [0x18] = 'o',  [0x19] = 'p',  [0x1A] = '[',
     [0x1B] = ']', [0x1C] = '\n', [0x1E] = 'a',  [0x1F] = 's',  [0x20] = 'd',
     [0x21] = 'f', [0x22] = 'g',  [0x23] = 'h',  [0x24] = 'j',  [0x25] = 'k',
     [0x26] = 'l', [0x27] = ';',  [0x28] = '\'', [0x29] = '`',  [0x2B] = '\\',
     [0x2C] = 'z', [0x2D] = 'x',  [0x2E] = 'c',  [0x2F] = 'v',  [0x30] = 'b',
     [0x31] = 'n', [0x32] = 'm',  [0x33] = ',',  [0x34] = '.',  [0x35] = '/',
     [0x39] = ' '};

static void kbd_put(uint8_t c)
{
  uint8_t next = (uint8_t)((kbd_head + 1u) % KBD_BUF_SIZE);
  if (next == kbd_tail) return; /* drop on overflow */
  kbd_buf[kbd_head] = c;
  kbd_head = next;
}

static unsigned char kbd_get(void)
{
  if (kbd_head == kbd_tail) return 0;
  unsigned char c = kbd_buf[kbd_tail];
  kbd_tail = (uint8_t)((kbd_tail + 1u) % KBD_BUF_SIZE);
  return c;
}

void keyboard_irq_handler(void)
{
  uint8_t scancode = inb(DATA_PORT);

  /* ignore break codes (release) and extended prefixes for now. */
  if ((scancode & 0x80) == 0 && scancode < sizeof(scancode_ascii))
  {
    unsigned char c = scancode_ascii[scancode];
    if (c) kbd_put((uint8_t)c);
  }

  PIC_sendEOI(1);
}

unsigned char keyboard_getchar(void)
{
  return kbd_get();
}
