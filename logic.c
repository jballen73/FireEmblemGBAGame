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
    newTileSelector->show = 1;

    Menu *newMenu = malloc(sizeof(Menu));
    newMenu->attack = 0;
    newMenu->item = 0;
    newMenu->wait = 0;
    newMenu->numItems = 0;

    ItemMenu *newItemMenu = malloc(sizeof(ItemMenu));
    newItemMenu->position1 = 0;
    newItemMenu->position2 = 0;
    newItemMenu->position3 = 0;

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
    blueUnit1->items[0] = newIronSword();
    blueUnit1->numItems = 1;
    appState->map[blueUnit1->xpos][blueUnit1->ypos] = blueUnit1->id;

    Unit *blueUnit2 = malloc(sizeof(Unit));
    blueUnit2->dead = 0;
    blueUnit2->id = 1;
    blueUnit2->team = BLUETEAM;
    blueUnit2->xpos = 13;
    blueUnit2->ypos = 7;
    blueUnit2->hasMoved = 0;
    blueUnit2->baseAttr0 = SPRITES_PALETTE_TYPE | BLUECHARACTERSPRITE_SPRITE_SHAPE;
    blueUnit2->baseAttr1 = BLUECHARACTERSPRITE_SPRITE_SIZE;
    blueUnit2->maxHP = 12;
    blueUnit2->curHP = 12;
    blueUnit2->atk = 6;
    blueUnit2->def = 3;
    blueUnit2->skl = 3;
    blueUnit2->spd = 7;
    blueUnit2->move = 4;
    blueUnit2->items[0] = newSteelSword();
    blueUnit2->items[1] = newVulnerary();
    blueUnit2->items[2] = newIronSword();
    blueUnit2->numItems = 3;
    appState->map[blueUnit2->xpos][blueUnit2->ypos] = blueUnit2->id;

    Unit *redUnit1 = malloc(sizeof(Unit));
    redUnit1->dead = 0;
    redUnit1->id = 2;
    redUnit1->team = REDTEAM;
    redUnit1->xpos = 2;
    redUnit1->ypos = 1;
    redUnit1->hasMoved = 0;
    redUnit1->baseAttr0 = SPRITES_PALETTE_TYPE | REDCHARACTERSPRITE_SPRITE_SHAPE;
    redUnit1->baseAttr1 = REDCHARACTERSPRITE_SPRITE_SIZE;
    redUnit1->maxHP = 10;
    redUnit1->curHP = 10;
    redUnit1->atk = 4;
    redUnit1->def = 2;
    redUnit1->skl = 3;
    redUnit1->spd = 3;
    redUnit1->move = 3;
    redUnit1->items[0] = newIronSword();
    redUnit1->numItems = 1;
    appState->map[redUnit1->xpos][redUnit1->ypos] = redUnit1->id;

    Unit *redUnit2 = malloc(sizeof(Unit));
    redUnit2->dead = 0;
    redUnit2->id = 3;
    redUnit2->team = REDTEAM;
    redUnit2->xpos = 3;
    redUnit2->ypos = 3;
    redUnit2->hasMoved = 0;
    redUnit2->baseAttr0 = SPRITES_PALETTE_TYPE | REDCHARACTERSPRITE_SPRITE_SHAPE;
    redUnit2->baseAttr1 = REDCHARACTERSPRITE_SPRITE_SIZE;
    redUnit2->maxHP = 11;
    redUnit2->curHP = 11;
    redUnit2->atk = 3;
    redUnit2->def = 4;
    redUnit2->skl = 2;
    redUnit2->spd = 2;
    redUnit2->move = 2;
    redUnit2->items[0] = newIronSword();
    redUnit2->numItems = 1;
    appState->map[redUnit2->xpos][redUnit2->ypos] = redUnit2->id;

    appState->tSelector = newTileSelector;
    appState->menu = newMenu;
    appState->itemMenu = newItemMenu;
    appState->currentMove = newMove;
    appState->menuSelectorPosition = 0;
    appState->toMenu = 0;
    appState->toMap = 0;
    appState->toMove = 0;
    appState->toAttackTargeting = 0;
    appState->toItemMenu = 0;
    appState->toEnemy = 0;
    appState->toEnemyMove = 0;
    appState->toEnemyTargeting = 0;
    appState->selected = -1;
    appState->targeted = -1;
    appState->unitList[0] = blueUnit1;
    appState->unitList[1] = blueUnit2;
    appState->unitList[2] = redUnit1;
    appState->unitList[3] = redUnit2;
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
static int leftEnemy(AppState *state) {
    int unitXPos = state->unitList[state->selected]->xpos;
    int unitYPos = state->unitList[state->selected]->ypos;
    Team unitTeam = state->unitList[state->selected]->team;
    if (unitXPos > 0 && state->map[unitXPos - 1][unitYPos] != -1 
    && state->unitList[state->map[unitXPos - 1][unitYPos]]->team != unitTeam) {
        return state->map[unitXPos - 1][unitYPos];
    }
    return -1;
}
static int rightEnemy(AppState *state) {
    int unitXPos = state->unitList[state->selected]->xpos;
    int unitYPos = state->unitList[state->selected]->ypos;
    Team unitTeam = state->unitList[state->selected]->team;
    if (unitXPos < 14 && state->map[unitXPos + 1][unitYPos] != -1 
    && state->unitList[state->map[unitXPos + 1][unitYPos]]->team != unitTeam) {
        return state->map[unitXPos + 1][unitYPos];
    }
    return -1;
}
static int upEnemy(AppState *state) {
    int unitXPos = state->unitList[state->selected]->xpos;
    int unitYPos = state->unitList[state->selected]->ypos;
    Team unitTeam = state->unitList[state->selected]->team;
    if (unitYPos > 0 && state->map[unitXPos][unitYPos - 1] != -1 
    && state->unitList[state->map[unitXPos][unitYPos - 1]]->team != unitTeam) {
        return state->map[unitXPos][unitYPos - 1];
    }
    return -1;
}
static int downEnemy(AppState *state) {
    int unitXPos = state->unitList[state->selected]->xpos;
    int unitYPos = state->unitList[state->selected]->ypos;
    Team unitTeam = state->unitList[state->selected]->team;
    if (unitYPos < 9 && state->map[unitXPos][unitYPos + 1] != -1 
    && state->unitList[state->map[unitXPos][unitYPos + 1]]->team != unitTeam) {
        return state->map[unitXPos][unitYPos + 1];
    }
    return -1;
}
static int isAdjacentEnemy(AppState *state) {
    return (leftEnemy(state) + rightEnemy(state) + upEnemy(state) + downEnemy(state) == -4) ? 0 : 1;
}
static int getPositionNum(Item *item) {
    switch(item->strength) {
        case 3:
            return 1;
        case 6:
            return 2;
        case 10:
            return 3;
        default:
            return 0;
    }
}
static void buildItemMenu(AppState *state) {
    Unit *currentUnit = state->unitList[state->selected];
    state->itemMenu->position1 = getPositionNum(currentUnit->items[0]);
    if (currentUnit->numItems == 1) {
        state->itemMenu->position2 = 0;
        state->itemMenu->position3 = 0;
        return;
    }
    state->itemMenu->position2 = getPositionNum(currentUnit->items[1]);
    if (currentUnit->numItems == 2) {
        state->itemMenu->position3 = 0;
        return;
    }
    state->itemMenu->position3 = getPositionNum(currentUnit->items[2]);
}
static int getItemMenuSelection(AppState *state) {
    switch(state->menuSelectorPosition) {
        case 0:
            return state->itemMenu->position1;
        case 1:
            return state->itemMenu->position2;
        case 2:
            return state->itemMenu->position3;
        default:
            return 0;
    }
}
static void swapItemToFront(Unit *unit, int position) {
    Item *originalEquip = unit->items[0];
    unit->items[0] = unit->items[position];
    unit->items[position] = originalEquip;
}
static int min(int x, int y) {
    return (x < y) ? x : y;
}
static int max(int x, int y) {
    return (x > y) ? x : y;
}
static void deleteItem(Unit *unit, int itemSlot) {
    if (itemSlot == 4 || itemSlot + 1 == unit->numItems) {
        unit->items[itemSlot] = NULL;
        unit->numItems--;
        return;
    }
    if (itemSlot == 0) {
        for (int i = 1; i < unit->numItems; i++) {
            if (unit->items[i]->type == WEAPON) {
                swapItemToFront(unit, i);
                deleteItem(unit, i);
                return;
            }
        }
    }
    unit->items[itemSlot] = unit->items[itemSlot + 1];
    deleteItem(unit, itemSlot + 1);
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
    if (KEY_JUST_PRESSED(BUTTON_R, keysPressedNow, keysPressedBefore)) {
        for (int i = 0; i < NUM_UNITS; i++) {
            if (currentAppState->unitList[i]->team == REDTEAM && !currentAppState->unitList[i]->dead) {
                nextAppState.unitList[i]->hasMoved = 0;
            }
        }
        nextAppState.toEnemy = 1;
        return nextAppState;
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
                nextAppState.toAttackTargeting = 1;
                if (upEnemy(currentAppState) != -1) {
                    nextAppState.tSelector->ypos--;
                } else if (leftEnemy(currentAppState) != -1) {
                    nextAppState.tSelector->xpos--;
                } else if (rightEnemy(currentAppState) != -1) {
                    nextAppState.tSelector->xpos++;
                } else if (downEnemy(currentAppState) != -1) {
                    nextAppState.tSelector->ypos++;
                }
                return nextAppState;
            case ITEM:
                buildItemMenu(&nextAppState);
                nextAppState.toItemMenu = 1;
                nextAppState.menuSelectorPosition = 0;
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
        nextAppState.menu->attack = 0;
        nextAppState.menu->item = 0;
        nextAppState.menuSelectorPosition = 0;
        int numItems = 0;
        nextAppState.toMenu = 1;
        nextAppState.menu->wait = 1;
        numItems++;
        if (isAdjacentEnemy(currentAppState) && currentAppState->unitList[currentAppState->selected]->numItems &&
            currentAppState->unitList[currentAppState->selected]->items[0]->type == WEAPON) {
            nextAppState.menu->attack = 1;
            numItems++;
        }
        if (currentAppState->unitList[currentAppState->selected]->numItems) {
            nextAppState.menu->item = 1;
            numItems++;
        }
        nextAppState.menu->numItems = numItems;
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
AppState processAppStateEnemyTargeting(AppState *currentAppState, CombatState *combatState) {
    AppState nextAppState = *currentAppState;
    nextAppState.toEnemyTargeting = 0;
    if (!isAdjacentEnemy(currentAppState)) {
        nextAppState.toEnemy = 1;
        return nextAppState;
    }
    combatState->attacker = currentAppState->unitList[currentAppState->selected];
    int leftHP = (leftEnemy(currentAppState) != -1) ? currentAppState->unitList[leftEnemy(currentAppState)]->curHP : 999;
    int rightHP = (rightEnemy(currentAppState) != -1) ? currentAppState->unitList[rightEnemy(currentAppState)]->curHP : 999;
    int upHP = (upEnemy(currentAppState) != -1) ? currentAppState->unitList[upEnemy(currentAppState)]->curHP : 999;
    int downHP = (downEnemy(currentAppState) != -1) ? currentAppState->unitList[downEnemy(currentAppState)]->curHP : 999;
    if (leftHP <= rightHP && leftHP <= upHP && leftHP <= downHP) {
        combatState->defender = currentAppState->unitList[leftEnemy(currentAppState)];
    } else if (downHP <= upHP && downHP <= rightHP) {
        combatState->defender = currentAppState->unitList[downEnemy(currentAppState)];
    } else if (rightHP <= upHP) {
        combatState->defender = currentAppState->unitList[rightEnemy(currentAppState)];
    } else {
        combatState->defender = currentAppState->unitList[upEnemy(currentAppState)];
    }
    combatState->toAttack = 1;
    combatState->toMap = 0;
    combatState->enemy = 1;
    return nextAppState;
}
AppState processAppStateAttackTargeting(AppState *currentAppState, u32 keysPressedBefore, 
    u32 keysPressedNow, CombatState *combatState) {
    AppState nextAppState = *currentAppState;
    nextAppState.toAttackTargeting = 0;
    int unitXPos = currentAppState->unitList[currentAppState->selected]->xpos;
    int unitYPos = currentAppState->unitList[currentAppState->selected]->ypos;
    int leftId = leftEnemy(currentAppState);
    int rightId = rightEnemy(currentAppState);
    int upId = upEnemy(currentAppState);
    int downId = downEnemy(currentAppState);
    if (KEY_JUST_PRESSED(BUTTON_B, keysPressedNow, keysPressedBefore)) {
        nextAppState.toMenu = 1;
        nextAppState.tSelector->xpos = unitXPos;
        nextAppState.tSelector->ypos = unitYPos;
        return nextAppState;
    }
    if (KEY_JUST_PRESSED(BUTTON_A, keysPressedNow, keysPressedBefore)) {
        combatState->toAttack = 1;
        combatState->toMap = 0;
        combatState->enemy = 0;
        combatState->attacker = currentAppState->unitList[currentAppState->selected];
        combatState->defender = currentAppState->unitList[currentAppState->map[currentAppState->tSelector->xpos][currentAppState->tSelector->ypos]];
        nextAppState.tSelector->xpos = unitXPos;
        nextAppState.tSelector->ypos = unitYPos;
        nextAppState.unitList[currentAppState->selected]->hasMoved = 1;
        nextAppState.map[currentAppState->currentMove->startX + currentAppState->currentMove->dx]
                [currentAppState->currentMove->startY + currentAppState->currentMove->dy] = currentAppState->currentMove->unitID;
        return nextAppState;
    }
    if (KEY_JUST_PRESSED(BUTTON_LEFT, keysPressedNow, keysPressedBefore)) {
        if (leftId != -1) {
            nextAppState.tSelector->xpos = unitXPos - 1;
            nextAppState.tSelector->ypos = unitYPos;
        }
    } else if (KEY_JUST_PRESSED(BUTTON_RIGHT, keysPressedNow, keysPressedBefore)) {
        if (rightId != -1) {
            nextAppState.tSelector->xpos = unitXPos + 1;
            nextAppState.tSelector->ypos = unitYPos;
        }
    } else if (KEY_JUST_PRESSED(BUTTON_UP, keysPressedNow, keysPressedBefore)) {
        if (upId != -1) {
            nextAppState.tSelector->xpos = unitXPos;
            nextAppState.tSelector->ypos = unitYPos - 1;
        }
    } else if (KEY_JUST_PRESSED(BUTTON_DOWN, keysPressedNow, keysPressedBefore)) {
        if (downId != -1) {
            nextAppState.tSelector->xpos = unitXPos;
            nextAppState.tSelector->ypos = unitYPos + 1;
        }
    }
    return nextAppState;
}
AppState processAppStateItemMenu(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    AppState nextAppState = *currentAppState;
    nextAppState.toItemMenu = 0;
    if (KEY_JUST_PRESSED(BUTTON_B, keysPressedNow, keysPressedBefore)) {
        nextAppState.toMenu = 1;
        nextAppState.menuSelectorPosition = 0;
        return nextAppState;
    }
    if (KEY_JUST_PRESSED(BUTTON_A, keysPressedNow, keysPressedBefore)) {
        int selection = getItemMenuSelection(currentAppState);
        Unit *curUnit = nextAppState.unitList[currentAppState->selected];
        switch (selection) {
            case 1:
                swapItemToFront(curUnit, currentAppState->menuSelectorPosition);
                nextAppState.toMenu = 1;
                nextAppState.menuSelectorPosition = 0;
                return nextAppState;
            case 2:
                swapItemToFront(curUnit, currentAppState->menuSelectorPosition);
                nextAppState.toMenu = 1;
                nextAppState.menuSelectorPosition = 0;
                return nextAppState;
            case 3:
                curUnit->curHP = min(curUnit->curHP + 10, curUnit->maxHP);
                curUnit->items[currentAppState->menuSelectorPosition]->uses--;
                if (!curUnit->items[currentAppState->menuSelectorPosition]->uses) {
                    deleteItem(curUnit, currentAppState->menuSelectorPosition);
                }
                nextAppState.map[currentAppState->currentMove->startX + currentAppState->currentMove->dx]
                [currentAppState->currentMove->startY + currentAppState->currentMove->dy] = currentAppState->currentMove->unitID;
                curUnit->hasMoved = 1;
                nextAppState.toMap = 1;
                return nextAppState;
            default:
                nextAppState.toMenu = 1;
                nextAppState.menuSelectorPosition = 0;
                return nextAppState;
        }
    }
    if (KEY_JUST_PRESSED(BUTTON_DOWN, keysPressedNow, keysPressedBefore)) {
        if (currentAppState->menuSelectorPosition + 1 < currentAppState->unitList[currentAppState->selected]->numItems) {
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
AppState processAppStateEnemy(AppState *currentAppState) {
    AppState nextAppState = *currentAppState;
    nextAppState.toEnemy = 0;
    for (int i = 0; i < NUM_UNITS; i++) {
        if (currentAppState->unitList[i]->team == REDTEAM && !currentAppState->unitList[i]->dead
         && !currentAppState->unitList[i]->hasMoved) {
             nextAppState.selected = i;
             break;
        }
        if (i + 1 == NUM_UNITS) {
            for (int i = 0; i < NUM_UNITS; i++) {
                if (currentAppState->unitList[i]->team == BLUETEAM && !currentAppState->unitList[i]->dead) {
                    nextAppState.unitList[i]->hasMoved = 0;
                }
            }
            nextAppState.toMap = 1;
            return nextAppState;
        }
    }
    int dist = 30;
    for (int i = 0; i < NUM_UNITS; i++) {
        if (currentAppState->unitList[i]->team == BLUETEAM && !currentAppState->unitList[i]->dead) {
            int newDistx = currentAppState->unitList[i]->xpos - currentAppState->unitList[nextAppState.selected]->xpos;
            int newDisty = currentAppState->unitList[i]->ypos - currentAppState->unitList[nextAppState.selected]->ypos;
            if (abs(newDistx) + abs(newDisty) < dist) {
                dist = abs(newDistx) + abs(newDisty);
                nextAppState.targeted = i;
            }
        }
    }
    nextAppState.toEnemyMove = 1;
    nextAppState.currentMove->totalDist = 0;
    return nextAppState;
}
AppState processAppStateEnemyMove(AppState *currentAppState) {
    AppState nextAppState = *currentAppState;
    nextAppState.toEnemyMove = 0;
    if (currentAppState->currentMove->totalDist == currentAppState->unitList[currentAppState->selected]->move) {
        nextAppState.unitList[currentAppState->selected]->hasMoved = 1;
        nextAppState.toEnemyTargeting = 1;
        return nextAppState;
    }
    int beginX = currentAppState->unitList[currentAppState->selected]->xpos;
    int beginY = currentAppState->unitList[currentAppState->selected]->ypos;
    int xDist = currentAppState->unitList[currentAppState->targeted]->xpos - beginX;
    int yDist = currentAppState->unitList[currentAppState->targeted]->ypos - beginY;
    if (abs(xDist) + abs(yDist) == 1) {
        nextAppState.unitList[currentAppState->selected]->hasMoved = 1;
        nextAppState.toEnemyTargeting = 1;
        return nextAppState;
    }
    if (abs(xDist) > abs(yDist)) {
        if (currentAppState->map[beginX + ((xDist > 0) ? 1 : -1)][beginY] == -1) {
            nextAppState.map[beginX][beginY] = -1;
            nextAppState.map[beginX + ((xDist > 0) ? 1 : -1)][beginY] = currentAppState->selected;
            nextAppState.unitList[currentAppState->selected]->xpos += ((xDist > 0) ? 1 : -1);
            nextAppState.currentMove->totalDist++;
            return nextAppState;
        } else if (currentAppState->map[beginX][beginY + ((yDist > 0) ? 1 : -1)] == -1) {
            nextAppState.map[beginX][beginY] = -1;
            nextAppState.map[beginX][beginY + ((yDist > 0) ? 1 : -1)] = currentAppState->selected;
            nextAppState.currentMove->totalDist++;
            nextAppState.unitList[currentAppState->selected]->ypos += ((yDist > 0) ? 1 : -1);
            return nextAppState;
        }
    } else {
        if (currentAppState->map[beginX][beginY + ((yDist > 0) ? 1 : -1)] == -1) {
            nextAppState.map[beginX][beginY] = -1;
            nextAppState.map[beginX][beginY + ((yDist > 0) ? 1 : -1)] = currentAppState->selected;
            nextAppState.currentMove->totalDist++;
            nextAppState.unitList[currentAppState->selected]->ypos += ((yDist > 0) ? 1 : -1);
            return nextAppState;
        } else if (currentAppState->map[beginX + ((xDist > 0) ? 1 : -1)][beginY] == -1) {
            nextAppState.map[beginX][beginY] = -1;
            nextAppState.map[beginX + ((xDist > 0) ? 1 : -1)][beginY] = currentAppState->selected;
            nextAppState.currentMove->totalDist++;
            nextAppState.unitList[currentAppState->selected]->xpos += ((xDist > 0) ? 1 : -1);
            return nextAppState;
        }
    }
    nextAppState.unitList[currentAppState->selected]->hasMoved = 1;
    nextAppState.toEnemyTargeting = 1;
    return nextAppState;
}

void initializeCombatState(CombatState *combatState) {
    if (combatState->attacker->numItems && combatState->attacker->items[0]->type == WEAPON) {
        combatState->atkDmg = max(combatState->attacker->atk + combatState->attacker->items[0]->strength - combatState->defender->def, 0);
        combatState->atkHit = combatState->attacker->items[0]->accuracy + (2 * combatState->attacker->skl);
        combatState->atkHit = min(100, max(combatState->atkHit, 0));
        combatState->atkCrit = combatState->attacker->skl / 2;
        if (combatState->attacker->spd - combatState->defender->spd >= 3) {
            combatState->atkNum = 2;
        } else {
            combatState->atkNum = 1;
        }
    } else {
        combatState->atkNum = -1;
        combatState->atkHit = -1;
        combatState->atkCrit = -1;
        combatState->atkNum = 0;
    }
    if (combatState->defender->numItems && combatState->defender->items[0]->type == WEAPON) {
        combatState->defDmg = max(combatState->defender->atk + combatState->defender->items[0]->strength - combatState->attacker->def, 0);
        combatState->defHit = combatState->defender->items[0]->accuracy + (2 * combatState->defender->skl);
        combatState->defHit = min(100, max(combatState->defHit, 0));
        combatState->defCrit = combatState->defender->skl / 2;
        if (combatState->defender->spd - combatState->attacker->spd >= 3) {
            combatState->defNum = 2;
        } else {
            combatState->defNum = 1;
        }
    } else {
        combatState->defNum = -1;
        combatState->defHit = -1;
        combatState->defCrit = -1;
        combatState->defNum = 0;
    }
    combatState->state = 1;
    combatState->redAtk = 0;
    combatState->blueAtk = 0;
}
CombatState processRedAttack(CombatState *combatState) {
    CombatState newCombatState = *combatState;
    newCombatState.redAtk = 0;
    if (combatState->attacker->team == REDTEAM) {
        int result = randint(1, 101);
        if (combatState->atkHit >= result) {
            result = randint(1, 101);
            if (combatState->atkCrit >= result) {
                newCombatState.defender->curHP -= (3 * combatState->atkDmg);
            } else {
                newCombatState.defender->curHP -= combatState->atkDmg;
            }
            newCombatState.attacker->items[0]->uses--;
            if (newCombatState.attacker->items[0]->uses <= 0) {
                deleteItem(newCombatState.attacker, 0);
                newCombatState.atkNum = 0;
            }
            if (newCombatState.defender->curHP <= 0) {
                newCombatState.defender->curHP = 0;
                newCombatState.defender->dead = 1;
            }
        }
    } else {
        int result = randint(1, 101);
        if (combatState->defHit >= result) {
            result = randint(1, 101);
            if (combatState->defCrit >= result) {
                newCombatState.attacker->curHP -= (3 * combatState->defDmg);
            } else {
                newCombatState.attacker->curHP -= combatState->defDmg;
            }
            newCombatState.defender->items[0]->uses--;
            if (newCombatState.defender->items[0]->uses <= 0) {
                deleteItem(newCombatState.defender, 0);
                newCombatState.defNum = 0;
            }
            if (newCombatState.attacker->curHP <= 0) {
                newCombatState.attacker->curHP = 0;
                newCombatState.attacker->dead = 1;
            }
        }
    }
    return newCombatState;
}
CombatState processBlueAttack(CombatState *combatState) {
    CombatState newCombatState = *combatState;
    newCombatState.blueAtk = 0;
    if (combatState->attacker->team == BLUETEAM) {
        int result = randint(1, 101);
        if (combatState->atkHit >= result) {
            result = randint(1, 101);
            if (combatState->atkCrit >= result) {
                newCombatState.defender->curHP -= (3 * combatState->atkDmg);
            } else {
                newCombatState.defender->curHP -= combatState->atkDmg;
            }
            newCombatState.attacker->items[0]->uses--;
            if (newCombatState.attacker->items[0]->uses <= 0) {
                deleteItem(newCombatState.attacker, 0);
                newCombatState.atkNum = 0;
            }
            if (newCombatState.defender->curHP <= 0) {
                newCombatState.defender->dead = 1;
                newCombatState.defender->curHP = 0;
            }
        }
    } else {
        int result = randint(1, 101);
        if (combatState->defHit >= result) {
            result = randint(1, 101);
            if (combatState->defCrit >= result) {
                newCombatState.attacker->curHP -= (3 * combatState->defDmg);
            } else {
                newCombatState.attacker->curHP -= combatState->defDmg;
            }
            newCombatState.defender->items[0]->uses--;
            if (newCombatState.defender->items[0]->uses <= 0) {
                deleteItem(newCombatState.defender, 0);
                newCombatState.defNum = 0;
            }
            if (newCombatState.attacker->curHP <= 0) {
                newCombatState.attacker->curHP = 0;
                newCombatState.attacker->dead = 1;
            }
        }
    }
    return newCombatState;
}
CombatState processCombatState(CombatState *combatState) {
    CombatState newCombatState = *combatState;
    newCombatState.toAttack = 0;
    if (combatState->state > 2 || combatState->attacker->dead || combatState->defender->dead) {
        newCombatState.toMap = 1;
        return newCombatState;
    }
    switch(combatState->state % 2) {
        case 1:
            if (combatState->atkNum) {
                newCombatState.atkNum--;
                if (combatState->attacker->team == REDTEAM) {
                    newCombatState.redAtk = 1;
                } else {
                    newCombatState.blueAtk = 1;
                }
            }
            break;
        case 0:
            if (combatState->defNum) {
                newCombatState.defNum--;
                if (combatState->defender->team == REDTEAM) {
                    newCombatState.redAtk = 1;
                } else {
                    newCombatState.blueAtk = 1;
                }
            }
            break;
    }
    newCombatState.state++;
    return newCombatState;
}
AppState processPostCombat(AppState *currentAppState, CombatState *combatState) {
    AppState newAppState = *currentAppState;
    if (combatState->attacker->dead) {
        newAppState.map[combatState->attacker->xpos][combatState->attacker->ypos] = -1;
    }
    if (combatState->defender->dead) {
        newAppState.map[combatState->defender->xpos][combatState->defender->ypos] = -1;
    }
    if (combatState->enemy) {
        newAppState.toEnemy = 1;
    } else {
        newAppState.toMap = 1;
    }
    return newAppState;
}