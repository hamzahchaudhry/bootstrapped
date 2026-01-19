#include <stdbool.h>
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

static volatile bool shift_down;
static volatile bool caps_lock;

/* set-1 scancode -> ASCII for unmodified keys */
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

/* set-1 scancode -> ASCII for shifted keys */
static const unsigned char shifted_scancode_ascii[128]
  = {[0x02] = '!', [0x03] = '@',  [0x04] = '#',  [0x05] = '$',  [0x06] = '%',
     [0x07] = '^', [0x08] = '&',  [0x09] = '*',  [0x0A] = '(',  [0x0B] = ')',
     [0x0C] = '_', [0x0D] = '+',  [0x0E] = '\b', [0x0F] = '\t', [0x10] = 'Q',
     [0x11] = 'W', [0x12] = 'E',  [0x13] = 'R',  [0x14] = 'T',  [0x15] = 'Y',
     [0x16] = 'U', [0x17] = 'I',  [0x18] = 'O',  [0x19] = 'P',  [0x1A] = '{',
     [0x1B] = '}', [0x1C] = '\n', [0x1E] = 'A',  [0x1F] = 'S',  [0x20] = 'D',
     [0x21] = 'F', [0x22] = 'G',  [0x23] = 'H',  [0x24] = 'J',  [0x25] = 'K',
     [0x26] = 'L', [0x27] = ':',  [0x28] = '"',  [0x29] = '~',  [0x2B] = '|',
     [0x2C] = 'Z', [0x2D] = 'X',  [0x2E] = 'C',  [0x2F] = 'V',  [0x30] = 'B',
     [0x31] = 'N', [0x32] = 'M',  [0x33] = '<',  [0x34] = '>',  [0x35] = '?',
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

  if (scancode == 0x2A || scancode == 0x36) shift_down = true;
  if (scancode == 0xAA || scancode == 0xB6) shift_down = false;
  if (scancode == 0x3A) caps_lock = !caps_lock;

  if ((scancode & 0x80) == 0 && scancode < sizeof(scancode_ascii))
  {
    unsigned char c = shift_down ? shifted_scancode_ascii[scancode] : scancode_ascii[scancode];
    if (c >= 'A' && c <= 'Z')
    {
      if (!(shift_down ^ caps_lock)) c = (unsigned char)(c + 32);
    }
    else if (c >= 'a' && c <= 'z')
    {
      if (shift_down ^ caps_lock) c = (unsigned char)(c - 32);
    }
    if (c) kbd_put((uint8_t)c);
  }

  PIC_sendEOI(1);
}

unsigned char keyboard_getchar(void)
{
  return kbd_get();
}
