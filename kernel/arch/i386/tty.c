#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"

/*
 * define standard 80x25 text mode.
 * also define physical memory address of VGA text mode framebuffer.
 * screen is represented as contiguous row-major array of VGA_WIDTH *
 * VGA_HEIGHT cells, with each cell being 2 bytes.
 * so, terminal_buffer[y * VGA_WIDTH + x] is cell (x, y).
 */
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t *const VGA_MEMORY = (uint16_t *)0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_colour;
static uint16_t *terminal_buffer;

/*
 * helper function to clear a row.
 */
static void terminal_clearrow(size_t row)
{
  for(size_t x = 0; x < VGA_WIDTH; x++)
    {
      const size_t index = row * VGA_WIDTH + x;
      terminal_buffer[index] = vga_entry(' ', terminal_colour);
    }
}

/*
 * clears screen and sets default colour.
 */
void terminal_initialize(void)
{
  terminal_row = 0;
  terminal_column = 0;
  terminal_colour = vga_entry_colour(VGA_COLOUR_DARK_GREY, VGA_COLOUR_BLACK);
  terminal_buffer = VGA_MEMORY;

  /* clear screen */
  for(size_t y = 0; y < VGA_HEIGHT; y++) { terminal_clearrow(y); }
}

void terminal_setcolour(uint8_t colour) { terminal_colour = colour; }

/*
 * put a character at a specific position.
 */
void terminal_putentryat(char c, uint8_t colour, size_t x, size_t y)
{
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(c, colour);
}

/*
 * helper function for scrolling.
 */
static void terminal_scroll(void)
{
  /* move all lines up by 1, discarding uppermost line */
  for(size_t y = 0; y < VGA_HEIGHT - 1; y++)
    {
      for(size_t x = 0; x < VGA_WIDTH; x++)
        {
          const size_t dst = y * VGA_WIDTH + x;
          const size_t src = (y + 1) * VGA_WIDTH + x;
          terminal_buffer[dst] = terminal_buffer[src];
        }
    }

  /* clear last line */
  terminal_clearrow(VGA_HEIGHT - 1);
}

/*
 * helper function for new line.
 */
static void terminal_newline(void)
{
  terminal_column = 0;

  if(terminal_row < VGA_HEIGHT - 1) { terminal_row++; }
  else
    {
      terminal_scroll();
    }
}

/*
 * put a character at the current cursor and advance.
 */
void terminal_putchar(char c)
{
  if(c == '\n')
    {
      terminal_newline();
      return;
    }
  if(c == '\b')
    {
      if(terminal_column > 0) terminal_column--;
      terminal_putentryat(' ', terminal_colour, terminal_column, terminal_row);
      return;
    }
  if(c == '\t')
    {
      terminal_column += 4;
      terminal_putentryat(' ', terminal_colour, terminal_column, terminal_row);
      return;
    }

  terminal_putentryat(c, terminal_colour, terminal_column, terminal_row);

  if(++terminal_column == VGA_WIDTH) terminal_newline();
}

/*
 * write a raw buffer to the terminal.
 */
void terminal_write(const char *data, size_t size)
{
  for(size_t i = 0; i < size; i++) terminal_putchar(data[i]);
}

/*
 * write a null-terminated string.
 */
void terminal_writestring(const char *data)
{
  terminal_write(data, strlen(data));
}
