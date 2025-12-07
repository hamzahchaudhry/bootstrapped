#include <stddef.h>
#include <stdint.h>

/* check if compiler thinks you are targeting wrong operating system */
#if defined(__linux__)
#error "You are not using a cross compiler and will run into trouble"
#endif

/* only for 32-bit i386 targets */
#if !defined(__i386__)
#error "This kernel needs to be compiled with an i386-elf compiler"
#endif

/*
 * hardware text mode colour constants.
 * in VGA text mode, each character on screen has a 4-bit foreground colour and
 * a 4-bit background colour.
 */
enum vga_colour
{
  VGA_COLOUR_BLACK = 0,
  VGA_COLOUR_BLUE = 1,
  VGA_COLOUR_GREEN = 2,
  VGA_COLOUR_CYAN = 3,
  VGA_COLOUR_RED = 4,
  VGA_COLOUR_MAGENTA = 5,
  VGA_COLOUR_BROWN = 6,
  VGA_COLOUR_LIGHT_GREY = 7,
  VGA_COLOUR_DARK_GREY = 8,
  VGA_COLOUR_LIGHT_BLUE = 9,
  VGA_COLOUR_LIGHT_GREEN = 10,
  VGA_COLOUR_LIGHT_CYAN = 11,
  VGA_COLOUR_LIGHT_RED = 12,
  VGA_COLOUR_LIGHT_MAGENTA = 13,
  VGA_COLOUR_LIGHT_BROWN = 14,
  VGA_COLOUR_WHITE = 15,
};

/*
 * note: static menas function is only visible in this file (internal linkage)
 * and inline suggests to compiler to replace calls with function body,
 * avoiding a function call overhead
 */

/*
 * VGA text mode encodes attribute byte as:
 * lower 4 bits = foreground colour,
 * upper 4 bits = background colour
 */
static inline uint8_t vga_entry_colour(enum vga_colour fg, enum vga_colour bg)
{
  return fg | bg << 4;
}

/*
 * VGA text mode encodes each character as:
 * lower 8 bits = ASCII,
 * upper 8 bits = colour attribute byte (vga_entry_colour)
 */
static inline uint16_t vga_entry(unsigned char uc, uint8_t colour)
{
  return (uint16_t)uc | (uint16_t)colour << 8;
}

/*
 * since there's no libc for us to use at this time, define our own
 * strlen function.
 */
size_t strlen(const char *str)
{
  size_t len = 0;
  while(str[len]) len++;
  return len;
}

/*
 * define standard 80x25 text mode.
 * also define physical memory address of VGA text mode framebuffer.
 * screen is represented as contiguous row-major array of VGA_WIDTH *
 * VGA_HEIGHT cells, with each cell being 2 bytes.
 * so, terminal_buffer[y * VGA_WIDTH + x] is cell (x, y).
 */
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_colour;
uint16_t *terminal_buffer = (uint16_t *)VGA_MEMORY;

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
 * put a character at the current cursor and advance. implements simple word
 * wrapping and vertical wrap around, but no scrolling.
 */
void terminal_putchar(char c)
{
  if(c == '\n')
    {
      terminal_newline();
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

/* kernel entry point, called from _start in boot.s. */
void kernel_main(void)
{
  /* initialize terminal interface */
  terminal_initialize();

  for(int i = 0; i < 40; i++) { terminal_writestring("hi\n"); }
}
