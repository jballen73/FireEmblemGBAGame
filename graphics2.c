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
static void drawPartialBackground(void) {
    drawImageDMA(0, 0, 240, 126, BattleBackground);
}
void doRedAttack(void) {
    for (int i = 0; i <= 20; i++) {
        waitForVBlank();
        redSprite->attr1 = (28 + i) | REDLARGESPRITE_SPRITE_SIZE;
        drawSprites();
        waitForVBlank();
    }
    for (int i = 20; i >= 0; i--) {
        waitForVBlank();
        redSprite->attr1 = (28 + i) | REDLARGESPRITE_SPRITE_SIZE;
        drawSprites();
        waitForVBlank();
    }
}
void doBlueAttack(void) {
    for (int i = 0; i <= 20; i++) {
        waitForVBlank();
        blueSprite->attr1 = (168 - i) | BLUELARGESPRITE_SPRITE_SIZE;
        drawSprites();
        waitForVBlank();
    }
    for (int i = 20; i >= 0; i--) {
        waitForVBlank();
        blueSprite->attr1 = (168 - i) | BLUELARGESPRITE_SPRITE_SIZE;
        drawSprites();
        waitForVBlank();
    }
}
static void drawUnits(void) {
    redSprite->attr0 = 57 | SPRITES2_PALETTE_TYPE | REDLARGESPRITE_SPRITE_SHAPE;
    redSprite->attr1 = 28 | REDLARGESPRITE_SPRITE_SIZE;
    blueSprite->attr0 = 54 | SPRITES2_PALETTE_TYPE | BLUELARGESPRITE_SPRITE_SHAPE;
    blueSprite->attr1 = 168 | BLUELARGESPRITE_SPRITE_SIZE;
}
static void drawHPBars(Unit *redUnit, Unit *blueUnit) {
    drawRectDMA(5, 112, (redUnit->maxHP + 1) * 2, 9, BLACK);
    if (redUnit->curHP) {
        drawRectDMA(6, 113, redUnit->curHP * 2, 7, RED);
    }
    drawRectDMA(173 + (62 - ((blueUnit->maxHP + 1) * 2)), 112, (blueUnit->maxHP + 1) * 2, 9, BLACK);
    if (blueUnit->curHP) {
        drawRectDMA(174 + (62 - ((blueUnit->maxHP + 1) * 2)), 113, blueUnit->curHP * 2, 7, BLUE);
    }
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
static void printRedNums(int dmg, int hit, int crit) {
    char redDmg[3];
    char redHit[3];
    char redCrit[3];
    if (dmg > -1) {
        sprintf(redDmg, "%d", dmg);
        sprintf(redHit, "%d", hit);
        sprintf(redCrit, "%d", crit);
    } else {
        sprintf(redDmg, "--");
        sprintf(redHit, "--");
        sprintf(redCrit, "--");
    }
    drawString(50, 129, redDmg, BLACK);
    drawString(50, 139, redHit, BLACK);
    drawString(50, 150, redCrit, BLACK);
}
static void printBlueNums(int dmg, int hit, int crit) {
    char blueDmg[3];
    char blueHit[3];
    char blueCrit[3];
    if (dmg > -1) {
        sprintf(blueDmg, "%d", dmg);
        sprintf(blueHit, "%d", hit);
        sprintf(blueCrit, "%d", crit);
    } else {
        sprintf(blueDmg, "--");
        sprintf(blueHit, "--");
        sprintf(blueCrit, "--");
    }
    drawString(218, 129, blueDmg, BLACK);
    drawString(218, 139, blueHit, BLACK);
    drawString(218, 150, blueCrit, BLACK);
}
void fullDrawCombatState(CombatState *state) {
    if (state->attacker->team == BLUETEAM) {
        printBlueNums(state->atkDmg, state->atkHit, state->atkCrit);
        printRedNums(state->defDmg, state->defHit, state->defCrit);
    } else {
        printRedNums(state->atkDmg, state->atkHit, state->atkCrit);
        printBlueNums(state->defDmg, state->defHit, state->defCrit);
    }
    drawCombatState(state);
}
void drawCombatState(CombatState *state) {
    drawPartialBackground();
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
