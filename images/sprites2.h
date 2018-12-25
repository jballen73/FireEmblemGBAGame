/*
 * Exported with nin10kit v1.7
 * Invocation command was nin10kit --mode=sprites --bpp=4 --for_bitmap --transparent=FFFFFF sprites2 BlueLargeSprite.png RedLargeSprite.png 
 * Time-stamp: Monday 12/24/2018, 14:29:20
 * 
 * Image Information
 * -----------------
 * BlueLargeSprite.png 64@64
 * RedLargeSprite.png 64@64
 * Transparent color: (255, 255, 255)
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef SPRITES2_H
#define SPRITES2_H

#define SPRITES2_PALETTE_TYPE (0 << 13)
#define SPRITES2_DIMENSION_TYPE (1 << 6)

extern const unsigned short sprites2_palette[256];
#define SPRITES2_PALETTE_SIZE 512
#define SPRITES2_PALETTE_LENGTH 256

extern const unsigned short sprites2[2048];
#define SPRITES2_SIZE 4096
#define SPRITES2_LENGTH 2048

#define BLUELARGESPRITE_PALETTE_ID (0 << 12)
#define BLUELARGESPRITE_SPRITE_SHAPE (0 << 14)
#define BLUELARGESPRITE_SPRITE_SIZE (3 << 14)
#define BLUELARGESPRITE_ID 512

#define REDLARGESPRITE_PALETTE_ID (1 << 12)
#define REDLARGESPRITE_SPRITE_SHAPE (0 << 14)
#define REDLARGESPRITE_SPRITE_SIZE (3 << 14)
#define REDLARGESPRITE_ID 576

#endif

