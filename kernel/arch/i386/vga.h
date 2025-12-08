#ifndef ARCH_i386_VGA_H
#define ARCH_i386_VGA_H

#include <stdint.h>

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

#endif
