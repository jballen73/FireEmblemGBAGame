#include "graphics.h"
#include "gba.h"
#include "images/sprites.h"
#include "logic.h"
// TA-TODO: Include any header files for images generated by nin10kit.
// Example for the provided garbage image:
//#include "images/garbage.h"
static void drawSprites(void);
volatile OamEntry shadow[128];
volatile OamEntry* tileSelector = &shadow[0];
volatile OamEntry* menuAttack = &shadow[4];
volatile OamEntry* menuItem = &shadow[2];
volatile OamEntry* menuWait = &shadow[3];
volatile OamEntry* menuSelector = &shadow[1];
volatile OamEntry* yourTurn = &shadow[5];
volatile OamEntry* enemyTurn = &shadow[6];
volatile OamEntry* ironSwordMenu = &shadow[7];
volatile OamEntry* steelSwordMenu = &shadow[8];
volatile OamEntry* vulneraryMenu = &shadow[9];

volatile OamEntry* blueUnit1 = &shadow[BASE_UNIT_INDEX];
volatile OamEntry* blueUnit2 = &shadow[BASE_UNIT_INDEX + 1];
volatile OamEntry* redUnit1 = &shadow[BASE_UNIT_INDEX + 2];
volatile OamEntry* redUnit2 = &shadow[BASE_UNIT_INDEX + 3];
void hideSprites(void) {
    for(int i = 0; i < 128; i++) {
        shadow[i].attr0 = ATTR0_HIDE;
    }
    drawSprites();
}
void graphicsInit(void) {
    DMA[3].src = sprites_palette;
    DMA[3].dst = SPRITEPAL;
    DMA[3].cnt = SPRITES_PALETTE_LENGTH | DMA_ON;

    DMA[3].src = sprites;
    DMA[3].dst = &CHARBLOCKBASE[5];
    DMA[3].cnt = SPRITES_LENGTH | DMA_ON;


    for(int i = 0; i < 128; i++) {
        shadow[i].attr0 = ATTR0_HIDE;
    }
    

    tileSelector->attr2 = TILESELECTOR_PALETTE_ID | TILESELECTOR_ID;
    menuAttack->attr2 = ATTACKMENU_PALETTE_ID | ATTACKMENU_ID;
    menuItem->attr2 = ITEMMENU_PALETTE_ID | ITEMMENU_ID;
    menuWait->attr2 = WAITMENU_PALETTE_ID | WAITMENU_ID;
    menuSelector->attr2 = MENUSELECTOR_PALETTE_ID | MENUSELECTOR_ID;
    yourTurn->attr2 = YOURTURN_PALETTE_ID | YOURTURN_ID;
    enemyTurn->attr2 = ENEMYTURN_PALETTE_ID | ENEMYTURN_ID;
    ironSwordMenu->attr2 = IRONSWORDMENU_PALETTE_ID | IRONSWORDMENU_ID;
    steelSwordMenu->attr2 = STEELSWORDMENU_PALETTE_ID | STEELSWORDMENU_ID;
    vulneraryMenu->attr2 = VULNERARYMENU_PALETTE_ID | VULNERARYMENU_ID;
    blueUnit1->attr2 = BLUECHARACTERSPRITE_PALETTE_ID | BLUECHARACTERSPRITE_ID;
    blueUnit2->attr2 = BLUECHARACTERSPRITE_PALETTE_ID | BLUECHARACTERSPRITE_ID;
    redUnit1->attr2 = REDCHARACTERSPRITE_PALETTE_ID | REDCHARACTERSPRITE_ID;
    redUnit2->attr2 = REDCHARACTERSPRITE_PALETTE_ID | REDCHARACTERSPRITE_ID;
}
static void drawTileSelector(int xpos, int ypos) {
    tileSelector->attr0 = ypos | SPRITES_PALETTE_TYPE | TILESELECTOR_SPRITE_SHAPE;
    tileSelector->attr1 = xpos | TILESELECTOR_SPRITE_SIZE;
}

static void drawSprites(void) {
    DMA[3].src = shadow;
    DMA[3].dst = OAMMEM;
    DMA[3].cnt = 128*4 | DMA_ON;
}
static void drawMenu(Menu* menu, int selectorPosition) {
    int basex = 104;
    int basey = 56;
    menuSelector->attr0 = (basey + (8*selectorPosition)) | SPRITES_PALETTE_TYPE | MENUSELECTOR_SPRITE_SHAPE;
    menuSelector->attr1 = basex | MENUSELECTOR_SPRITE_SIZE;
    if (menu->attack) {
        menuAttack->attr0 = basey | SPRITES_PALETTE_TYPE | ATTACKMENU_SPRITE_SHAPE;
        menuAttack->attr1 = basex | ATTACKMENU_SPRITE_SIZE;
        basey += 8;
    }
    if (menu->item) {
        menuItem->attr0 = basey | SPRITES_PALETTE_TYPE | ITEMMENU_SPRITE_SHAPE;
        menuItem->attr1 = basex | ITEMMENU_SPRITE_SIZE;
        basey += 8;
    }
    if (menu->wait) {
        menuWait->attr0 = basey | SPRITES_PALETTE_TYPE | WAITMENU_SPRITE_SHAPE;
        menuWait->attr1 = basex | WAITMENU_SPRITE_SIZE;
        basey += 8;
    }
}
static void setItemMenuElementPosition(int positionValue, int xpos, int ypos) {
    switch(positionValue) {
        case 1:
            ironSwordMenu->attr0 = ypos | SPRITES_PALETTE_TYPE | IRONSWORDMENU_SPRITE_SHAPE;
            ironSwordMenu->attr1 = xpos | IRONSWORDMENU_SPRITE_SIZE;
            break;
        case 2:
            steelSwordMenu->attr0 = ypos | SPRITES_PALETTE_TYPE | STEELSWORDMENU_SPRITE_SHAPE;
            steelSwordMenu->attr1 = xpos | STEELSWORDMENU_SPRITE_SIZE;
            break;
        case 3:
            vulneraryMenu->attr0 = ypos | SPRITES_PALETTE_TYPE | VULNERARYMENU_SPRITE_SHAPE;
            vulneraryMenu->attr1 = xpos | VULNERARYMENU_SPRITE_SIZE;
            break;
    }
}
static void drawItemMenu(ItemMenu *itemMenu, int selectorPosition) {
    ironSwordMenu->attr0 = ATTR0_HIDE;
    steelSwordMenu->attr0 = ATTR0_HIDE;
    vulneraryMenu->attr0 = ATTR0_HIDE;
    menuSelector->attr0 = (56 + (8*selectorPosition)) | SPRITES_PALETTE_TYPE | MENUSELECTOR_SPRITE_SHAPE;
    menuSelector->attr1 = 104 | MENUSELECTOR_SPRITE_SIZE;
    setItemMenuElementPosition(itemMenu->position1, 104, 56);
    setItemMenuElementPosition(itemMenu->position2, 104, 64);
    setItemMenuElementPosition(itemMenu->position3, 104, 72);
}
static void drawUnits(AppState *state) {
    for (int i = 0; i < NUM_UNITS; i++) {
        Unit *curUnit = state->unitList[i];
        if (!curUnit->dead) {
            shadow[curUnit->id + BASE_UNIT_INDEX].attr0 = curUnit->baseAttr0 | (curUnit->ypos * 16);
            shadow[curUnit->id + BASE_UNIT_INDEX].attr1 = curUnit->baseAttr1 | (curUnit->xpos * 16);
        } else {
            shadow[curUnit->id + BASE_UNIT_INDEX].attr0 = ATTR0_HIDE;
        }
    }
}

void showYourTurn(void) {
    yourTurn->attr0 = 64 | SPRITES_PALETTE_TYPE | YOURTURN_SPRITE_SHAPE;
    yourTurn->attr1 = 104 | YOURTURN_SPRITE_SIZE;
    drawSprites();
}
void hideYourTurn(void) {
    yourTurn->attr0 = ATTR0_HIDE;
    drawSprites();
}
void showEnemyTurn(void) {
    enemyTurn->attr0 = 64 | SPRITES_PALETTE_TYPE | ENEMYTURN_SPRITE_SHAPE;
    enemyTurn->attr1 = 104 | ENEMYTURN_SPRITE_SIZE;
    drawSprites();
}
void hideEnemyTurn(void) {
    enemyTurn->attr0 = ATTR0_HIDE;
    drawSprites();
}
static void hideMenu(void) {
    menuAttack->attr0 = ATTR0_HIDE;
    menuItem->attr0 = ATTR0_HIDE;
    menuWait->attr0 = ATTR0_HIDE;
    menuSelector->attr0 = ATTR0_HIDE;
}
static void hideItemMenu(void) {
    ironSwordMenu->attr0 = ATTR0_HIDE;
    steelSwordMenu->attr0 = ATTR0_HIDE;
    vulneraryMenu->attr0 = ATTR0_HIDE;
}
static void hideTileSelector(void) {
    tileSelector->attr0 = ATTR0_HIDE;
}
// This function will be used to draw everything about the app
// including the background and whatnot.
void fullDrawAppState(AppState *state) {
    // TA-TODO: IMPLEMENT.
    fillScreenDMA(WHITE);
    drawAppStateMap(state);
}

// This function will be used to undraw (i.e. erase) things that might
// move in a frame. E.g. in a Snake game, erase the Snake, the food & the score.
void undrawAppState(AppState *state) {
    // TA-TODO: IMPLEMENT.
    UNUSED(state);
}

// This function will be used to draw things that might have moved in a frame.
// For example, in a Snake game, draw the snake, the food, the score.
void drawAppStateMap(AppState *state) {
    hideMenu();
    hideItemMenu();
    if (state->tSelector->show) {
        drawTileSelector(state->tSelector->xpos * 16, state->tSelector->ypos * 16);
    } else {
        hideTileSelector();
    }
    drawUnits(state);
    drawSprites();
}
void drawAppStateMenu(AppState *state) {
    tileSelector->attr0 = ATTR0_HIDE;
    hideItemMenu();
    drawMenu(state->menu, state->menuSelectorPosition);
    drawSprites();

}
void drawAppStateItemMenu(AppState *state) {
    hideMenu();
    tileSelector->attr0 = ATTR0_HIDE;
    drawItemMenu(state->itemMenu, state->menuSelectorPosition);
    drawSprites();
}