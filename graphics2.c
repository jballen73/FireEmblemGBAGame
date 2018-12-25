#include "graphics2.h"
#include "gba.h"
#include "images/sprites2.h"
#include "images/BattleBackground.h"
#include <stdio.h>
volatile OamEntry shadow2[128];
volatile OamEntry* blueSprite = &shadow2[0];
volatile OamEntry* redSprite = &shadow2[1];
void graphics2Init(void) {
    DMA[3].src = sprites2_palette;
    DMA[3].dst = SPRITEPAL;
    DMA[3].cnt = SPRITES2_PALETTE_LENGTH | DMA_ON;

    DMA[3].src = sprites2;
    DMA[3].dst = &CHARBLOCKBASE[5];
    DMA[3].cnt = SPRITES2_LENGTH | DMA_ON;


    for(int i = 0; i < 128; i++) {
        shadow2[i].attr0 = ATTR0_HIDE;
    }
    blueSprite->attr2 = BLUELARGESPRITE_PALETTE_ID | BLUELARGESPRITE_ID;
    redSprite->attr2 = REDLARGESPRITE_PALETTE_ID | REDLARGESPRITE_ID;
}
static void drawSprites(void) {
    DMA[3].src = shadow2;
    DMA[3].dst = OAMMEM;
    DMA[3].cnt = 128*4 | DMA_ON;
}
void drawCombatBackground(void) {
    drawFullScreenImageDMA(BattleBackground);
}
static void drawUnits(void) {
    redSprite->attr0 = 57 | SPRITES2_PALETTE_TYPE | REDLARGESPRITE_SPRITE_SHAPE;
    redSprite->attr1 = 28 | REDLARGESPRITE_SPRITE_SIZE;
    blueSprite->attr0 = 54 | SPRITES2_PALETTE_TYPE | BLUELARGESPRITE_SPRITE_SHAPE;
    blueSprite->attr1 = 168 | BLUELARGESPRITE_SPRITE_SIZE;
}
static void drawHPBars(Unit *redUnit, Unit *blueUnit) {
    drawRectDMA(5, 112, (redUnit->maxHP + 1) * 2, 9, BLACK);
    drawRectDMA(6, 113, redUnit->curHP * 2, 7, RED);
    drawRectDMA(173 + (62 - ((blueUnit->maxHP + 1) * 2)), 112, (blueUnit->maxHP + 1) * 2, 9, BLACK);
    drawRectDMA(174 + (62 - ((blueUnit->maxHP + 1) * 2)), 113, blueUnit->curHP * 2, 7, BLUE);
}
static void printHPNums(Unit *redUnit, Unit *blueUnit) {
    char redHP[2];
    char blueHP[2];
    //98, 112
    sprintf(redHP, "%d", redUnit->curHP);
    sprintf(blueHP, "%d", blueUnit->curHP);
    drawString(98, 112, redHP, BLACK);
    drawString(132, 112, blueHP, BLACK);
}
void drawCombatState(CombatState *state) {
    drawUnits();
    drawSprites();
    if (state->attacker->team == BLUETEAM) {
        drawHPBars(state->defender, state->attacker);
        printHPNums(state->defender, state->attacker);
    } else {
        drawHPBars(state->attacker, state->defender);
        printHPNums(state->attacker, state->defender);
    }
}
