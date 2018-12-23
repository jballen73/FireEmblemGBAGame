#include "logic.h"
#include "graphics.h"
#include "gba.h"
#include "images/sprites.h"
#include <stdlib.h>
void initializeAppState(AppState* appState) {
    // TA-TODO: Initialize everything that's part of this AppState struct here.
    // Suppose the struct contains random values, make sure everything gets
    // the value it should have when the app begins.
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 10; j++) {
            appState->map[i][j] = -1;
        }
    }

    TileSelector *newTileSelector = malloc(sizeof(TileSelector));
    newTileSelector->xpos = 0;
    newTileSelector->ypos = 0;

    Menu *newMenu = malloc(sizeof(Menu));
    newMenu->attack = 0;
    newMenu->item = 0;
    newMenu->wait = 0;
    newMenu->numItems = 0;

    Move *newMove = malloc(sizeof(Move));
    newMove->unitID = -1;
    newMove->startX = 0;
    newMove->startY = 0;
    newMove->dx = 0;
    newMove->dy = 0;
    newMove->totalDist = 0;

    Unit *blueUnit1 = malloc(sizeof(Unit));
    blueUnit1->dead = 0;
    blueUnit1->id = 0;
    blueUnit1->team = BLUETEAM;
    blueUnit1->xpos = 14;
    blueUnit1->ypos = 9;
    blueUnit1->hasMoved = 0;
    blueUnit1->baseAttr0 = SPRITES_PALETTE_TYPE | BLUECHARACTERSPRITE_SPRITE_SHAPE;
    blueUnit1->baseAttr1 = BLUECHARACTERSPRITE_SPRITE_SIZE;
    blueUnit1->maxHP = 15;
    blueUnit1->curHP = 15;
    blueUnit1->atk = 5;
    blueUnit1->def = 4;
    blueUnit1->skl = 4;
    blueUnit1->spd = 6;
    blueUnit1->move = 4;
    appState->map[blueUnit1->xpos][blueUnit1->ypos] = blueUnit1->id;

    appState->tSelector = newTileSelector;
    appState->menu = newMenu;
    appState->currentMove = newMove;
    appState->menuSelectorPosition = 0;
    appState->toMenu = 0;
    appState->toMap = 0;
    appState->toMove = 0;
    appState->selected = -1;
    appState->unitList[0] = blueUnit1;

}

// TA-TODO: Add any process functions for sub-elements of your app here.
// For example, for a snake game, you could have a processSnake function
// or a createRandomFood function or a processFoods function.
//
// e.g.:
// static Snake processSnake(Snake* currentSnake);
// static void generateRandomFoods(AppState* currentAppState, AppState* nextAppState);

static void undoMove(AppState *state) {
    int baseX = state->currentMove->startX;
    int baseY = state->currentMove->startY;
    state->map[baseX + state->currentMove->dx][baseY + state->currentMove->dy] = -1;
    state->map[baseX][baseY] = state->currentMove->unitID;
    state->unitList[state->currentMove->unitID]->hasMoved = 0;
    state->unitList[state->currentMove->unitID]->xpos = baseX;
    state->unitList[state->currentMove->unitID]->ypos = baseY;
    state->tSelector->xpos = baseX;
    state->tSelector->ypos = baseY;
}
static MenuOption parseMenuOption(AppState *state) {
    switch(state->menuSelectorPosition) {
        case 0:
            if (state->menu->attack) {
                return ATTACK;
            } else if (state->menu->item) {
                return ITEM;
            } else {
                return WAIT;
            }
        case 1:
            if (state->menu->attack) {
                if (state->menu->item) {
                    return ITEM;
                } else {
                    return WAIT;
                }
            } else {
                return WAIT;
            }
        case 2:
            return WAIT;
    }
    return WAIT;
}



// This function processes your current app state and returns the new (i.e. next)
// state of your application.
AppState processAppStateMap(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    AppState nextAppState = *currentAppState;
    nextAppState.toMap = 0;
    if (KEY_JUST_PRESSED(BUTTON_A, keysPressedNow, keysPressedBefore)) {
        int curSpot = currentAppState->map[currentAppState->tSelector->xpos][currentAppState->tSelector->ypos];
        if (curSpot != -1) {
            if ((!currentAppState->unitList[curSpot]->hasMoved) && currentAppState->unitList[curSpot]->team == BLUETEAM) {
                nextAppState.selected = curSpot;
                nextAppState.toMove = 1;
                nextAppState.currentMove->unitID = curSpot;
                nextAppState.currentMove->startX = currentAppState->tSelector->xpos;
                nextAppState.currentMove->startY = currentAppState->tSelector->ypos;
                nextAppState.currentMove->dx = 0;
                nextAppState.currentMove->dy = 0;
                nextAppState.currentMove->totalDist = 0;
                nextAppState.map[currentAppState->tSelector->xpos][currentAppState->tSelector->ypos] = -1;
                return nextAppState;
            }
        }
    }

    if (KEY_JUST_PRESSED(BUTTON_LEFT, keysPressedNow, keysPressedBefore)) {
        if (currentAppState->tSelector->xpos > 0) {
            nextAppState.tSelector->xpos--;
        }
    }
    if (KEY_JUST_PRESSED(BUTTON_DOWN, keysPressedNow, keysPressedBefore)) {
        if (currentAppState->tSelector->ypos < 9) {
            nextAppState.tSelector->ypos++;
        }
    }
    if (KEY_JUST_PRESSED(BUTTON_RIGHT, keysPressedNow, keysPressedBefore)) {
        if (currentAppState->tSelector->xpos < 14) {
            nextAppState.tSelector->xpos++;
        }
    }
    if (KEY_JUST_PRESSED(BUTTON_UP, keysPressedNow, keysPressedBefore)) {
        if (currentAppState->tSelector->ypos > 0) {
            nextAppState.tSelector->ypos--;
        }
    }
    
    return nextAppState;
}

AppState processAppStateMenu(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    AppState nextAppState = *currentAppState;
    nextAppState.toMenu = 0;
    if (KEY_JUST_PRESSED(BUTTON_B, keysPressedNow, keysPressedBefore)) {
        nextAppState.toMove = 1;
        return nextAppState;
    }
    if (KEY_JUST_PRESSED(BUTTON_A, keysPressedNow, keysPressedBefore)) {
        MenuOption selectedOption = parseMenuOption(currentAppState);
        switch (selectedOption) {
            case WAIT:
                nextAppState.toMap = 1;
                nextAppState.map[currentAppState->currentMove->startX + currentAppState->currentMove->dx]
                [currentAppState->currentMove->startY + currentAppState->currentMove->dy] = currentAppState->currentMove->unitID;
                nextAppState.unitList[currentAppState->currentMove->unitID]->hasMoved = 1;
                return nextAppState;
            case ATTACK:
                return nextAppState;
            case ITEM:
                return nextAppState;
        }
    }
    if (KEY_JUST_PRESSED(BUTTON_DOWN, keysPressedNow, keysPressedBefore)) {
        if (currentAppState->menuSelectorPosition + 1 < currentAppState->menu->numItems) {
            nextAppState.menuSelectorPosition++;
        }
    }
    if (KEY_JUST_PRESSED(BUTTON_UP, keysPressedNow, keysPressedBefore)) {
        if (currentAppState->menuSelectorPosition > 0) {
            nextAppState.menuSelectorPosition--;
        }
    }
    
    return nextAppState;
}

AppState processAppStateMove(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    AppState nextAppState = *currentAppState;
    nextAppState.toMove = 0;
    if (KEY_JUST_PRESSED(BUTTON_A, keysPressedNow, keysPressedBefore)) {
        nextAppState.toMenu = 1;
        nextAppState.menu->wait = 1;
        nextAppState.menu->numItems = 1;
        return nextAppState;
    }
    if (KEY_JUST_PRESSED(BUTTON_B, keysPressedNow, keysPressedBefore)) {
        undoMove(&nextAppState);
        nextAppState.toMap = 1;
        return nextAppState;
    }

    if (KEY_JUST_PRESSED(BUTTON_LEFT, keysPressedNow, keysPressedBefore)) {
        if (currentAppState->tSelector->xpos > 0) {
            if (currentAppState->currentMove->dx > 0) {
                if (currentAppState->map[currentAppState->tSelector->xpos - 1][currentAppState->tSelector->ypos] == -1) {
                    nextAppState.currentMove->dx--;
                    nextAppState.currentMove->totalDist--;
                    nextAppState.unitList[currentAppState->currentMove->unitID]->xpos--;
                    nextAppState.tSelector->xpos--;
                }
            } else {
                if (currentAppState->currentMove->totalDist < currentAppState->unitList[currentAppState->currentMove->unitID]->move) {
                    if (currentAppState->map[currentAppState->tSelector->xpos - 1][currentAppState->tSelector->ypos] == -1) {
                        nextAppState.currentMove->dx--;
                        nextAppState.currentMove->totalDist++;
                        nextAppState.unitList[currentAppState->currentMove->unitID]->xpos--;
                        nextAppState.tSelector->xpos--;
                    }
                }
            }
        }
    } else if (KEY_JUST_PRESSED(BUTTON_RIGHT, keysPressedNow, keysPressedBefore)) {
        if (currentAppState->tSelector->xpos < 14) {
            if (currentAppState->currentMove->dx < 0) {
                if (currentAppState->map[currentAppState->tSelector->xpos + 1][currentAppState->tSelector->ypos] == -1) {
                    nextAppState.currentMove->dx++;
                    nextAppState.currentMove->totalDist--;
                    nextAppState.unitList[currentAppState->currentMove->unitID]->xpos++;
                    nextAppState.tSelector->xpos++;
                }
            } else {
                if (currentAppState->currentMove->totalDist < currentAppState->unitList[currentAppState->currentMove->unitID]->move) {
                    if (currentAppState->map[currentAppState->tSelector->xpos + 1][currentAppState->tSelector->ypos] == -1) {
                        nextAppState.currentMove->dx++;
                        nextAppState.currentMove->totalDist++;
                        nextAppState.unitList[currentAppState->currentMove->unitID]->xpos++;
                        nextAppState.tSelector->xpos++;
                    }
                }
            }
        }
    } else if (KEY_JUST_PRESSED(BUTTON_UP, keysPressedNow, keysPressedBefore)) {
        if (currentAppState->tSelector->ypos > 0) {
            if (currentAppState->currentMove->dy > 0) {
                if (currentAppState->map[currentAppState->tSelector->xpos][currentAppState->tSelector->ypos - 1] == -1) {
                    nextAppState.currentMove->dy--;
                    nextAppState.currentMove->totalDist--;
                    nextAppState.unitList[currentAppState->currentMove->unitID]->ypos--;
                    nextAppState.tSelector->ypos--;
                }
            } else {
                if (currentAppState->currentMove->totalDist < currentAppState->unitList[currentAppState->currentMove->unitID]->move) {
                    if (currentAppState->map[currentAppState->tSelector->xpos][currentAppState->tSelector->ypos - 1] == -1) {
                        nextAppState.currentMove->dy--;
                        nextAppState.currentMove->totalDist++;
                        nextAppState.unitList[currentAppState->currentMove->unitID]->ypos--;
                        nextAppState.tSelector->ypos--;
                    }
                }
            }
        }
    } else if (KEY_JUST_PRESSED(BUTTON_DOWN, keysPressedNow, keysPressedBefore)) {
        if (currentAppState->tSelector->ypos < 9) {
            if (currentAppState->currentMove->dy < 0) {
                if (currentAppState->map[currentAppState->tSelector->xpos][currentAppState->tSelector->ypos + 1] == -1) {
                    nextAppState.currentMove->dy++;
                    nextAppState.currentMove->totalDist--;
                    nextAppState.unitList[currentAppState->currentMove->unitID]->ypos++;
                    nextAppState.tSelector->ypos++;
                }
            } else {
                if (currentAppState->currentMove->totalDist < currentAppState->unitList[currentAppState->currentMove->unitID]->move) {
                    if (currentAppState->map[currentAppState->tSelector->xpos][currentAppState->tSelector->ypos + 1] == -1) {
                        nextAppState.currentMove->dy++;
                        nextAppState.currentMove->totalDist++;
                        nextAppState.unitList[currentAppState->currentMove->unitID]->ypos++;
                        nextAppState.tSelector->ypos++;
                    }
                }
            }
        }
    }
    return nextAppState;
}