/*
 * Exported with nin10kit v1.7
 * Invocation command was nin10kit --mode=sprites --bpp=4 --for_bitmap --transparent=FFFFFF sprites AttackMenu.png BlueCharacterSprite.png ItemMenu.png MenuSelector.png RedCharacterSprite.png TileSelector.png WaitMenu.png YourTurn.png EnemyTurn.png 
 * Time-stamp: Sunday 12/23/2018, 15:53:29
 * 
 * Image Information
 * -----------------
 * AttackMenu.png 32@8
 * BlueCharacterSprite.png 16@16
 * ItemMenu.png 32@8
 * MenuSelector.png 32@8
 * RedCharacterSprite.png 16@16
 * TileSelector.png 16@16
 * WaitMenu.png 32@8
 * YourTurn.png 32@32
 * EnemyTurn.png 32@32
 * Transparent color: (255, 255, 255)
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef SPRITES_H
#define SPRITES_H

#define SPRITES_PALETTE_TYPE (0 << 13)
#define SPRITES_DIMENSION_TYPE (1 << 6)

extern const unsigned short sprites_palette[256];
#define SPRITES_PALETTE_SIZE 512
#define SPRITES_PALETTE_LENGTH 256

extern const unsigned short sprites[960];
#define SPRITES_SIZE 1920
#define SPRITES_LENGTH 960

#define YOURTURN_PALETTE_ID (0 << 12)
#define YOURTURN_SPRITE_SHAPE (0 << 14)
#define YOURTURN_SPRITE_SIZE (2 << 14)
#define YOURTURN_ID 512

#define ENEMYTURN_PALETTE_ID (1 << 12)
#define ENEMYTURN_SPRITE_SHAPE (0 << 14)
#define ENEMYTURN_SPRITE_SIZE (2 << 14)
#define ENEMYTURN_ID 528

#define BLUECHARACTERSPRITE_PALETTE_ID (2 << 12)
#define BLUECHARACTERSPRITE_SPRITE_SHAPE (0 << 14)
#define BLUECHARACTERSPRITE_SPRITE_SIZE (1 << 14)
#define BLUECHARACTERSPRITE_ID 544

#define REDCHARACTERSPRITE_PALETTE_ID (3 << 12)
#define REDCHARACTERSPRITE_SPRITE_SHAPE (0 << 14)
#define REDCHARACTERSPRITE_SPRITE_SIZE (1 << 14)
#define REDCHARACTERSPRITE_ID 548

#define ATTACKMENU_PALETTE_ID (3 << 12)
#define ATTACKMENU_SPRITE_SHAPE (1 << 14)
#define ATTACKMENU_SPRITE_SIZE (1 << 14)
#define ATTACKMENU_ID 552

#define ITEMMENU_PALETTE_ID (3 << 12)
#define ITEMMENU_SPRITE_SHAPE (1 << 14)
#define ITEMMENU_SPRITE_SIZE (1 << 14)
#define ITEMMENU_ID 556

#define WAITMENU_PALETTE_ID (3 << 12)
#define WAITMENU_SPRITE_SHAPE (1 << 14)
#define WAITMENU_SPRITE_SIZE (1 << 14)
#define WAITMENU_ID 560

#define MENUSELECTOR_PALETTE_ID (3 << 12)
#define MENUSELECTOR_SPRITE_SHAPE (1 << 14)
#define MENUSELECTOR_SPRITE_SIZE (1 << 14)
#define MENUSELECTOR_ID 564

#define TILESELECTOR_PALETTE_ID (3 << 12)
#define TILESELECTOR_SPRITE_SHAPE (0 << 14)
#define TILESELECTOR_SPRITE_SIZE (1 << 14)
#define TILESELECTOR_ID 568

#endif

