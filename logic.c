#include "logic.h"
#include "graphics.h"
#include "gba.h"
#include <stdlib.h>
void initializeAppState(AppState* appState) {
    // TA-TODO: Initialize everything that's part of this AppState struct here.
    // Suppose the struct contains random values, make sure everything gets
    // the value it should have when the app begins.
    TileSelector *newTileSelector = malloc(sizeof(TileSelector));
    newTileSelector->xpos = 0;
    newTileSelector->ypos = 0;

    Menu *newMenu = malloc(sizeof(Menu));
    newMenu->attack = 0;
    newMenu->item = 0;
    newMenu->wait = 0;
    newMenu->numItems = 0;

    appState->tSelector = newTileSelector;
    appState->menu = newMenu;
    appState->menuSelectorPosition = 0;
}

// TA-TODO: Add any process functions for sub-elements of your app here.
// For example, for a snake game, you could have a processSnake function
// or a createRandomFood function or a processFoods function.
//
// e.g.:
// static Snake processSnake(Snake* currentSnake);
// static void generateRandomFoods(AppState* currentAppState, AppState* nextAppState);




// This function processes your current app state and returns the new (i.e. next)
// state of your application.
AppState processAppStateMap(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
    AppState nextAppState = *currentAppState;
    nextAppState.toMap = 0;
    if (KEY_JUST_PRESSED(BUTTON_A, keysPressedNow, keysPressedBefore)) {
        nextAppState.toMenu = 1;
        nextAppState.menu->attack = 1;
        nextAppState.menu->item = 1;
        nextAppState.menu->wait = 1;
        nextAppState.menu->numItems = 3;
        nextAppState.menuSelectorPosition = 0;
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
        nextAppState.toMap = 1;
        return nextAppState;
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