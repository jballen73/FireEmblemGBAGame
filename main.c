#include "gba.h"
#include "logic.h"
#include "graphics.h"
// TA-TODO: Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
#include "images/sprites.h"

#include <stdio.h>
#include <stdlib.h>
// AppState enum definition
typedef enum {
    // TA-TODO: Add any additional states you need for your app.
    START,  
    APP_INIT,
    APP_MAP,
    APP_MENU,
    APP_MOVE,
} GBAState;

int main(void) {
    // TA-TODO: Manipulate REG_DISPCNT here to set Mode 3.
    REG_DISPCNT = MODE3 | BG2_ENABLE | OBJ_ENABLE | SPRITES_DIMENSION_TYPE;

    GBAState state = START;

    // We store the "previous" and "current" states.
    AppState currentAppState, nextAppState;

    // We store the current and previous values of the button input.
    u32 previousButtons = BUTTONS;
    u32 currentButtons = BUTTONS;
    while(1) {
        // Load the current state of the buttons
        currentButtons = BUTTONS;
        if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
            state = START;
        }
        // TA-TODO: Manipulate the state machine below as needed.
        switch(state) {
        case START:
            // Wait for vblank
            waitForVBlank();
            graphicsInit();
            
            state = APP_INIT;
            break;
        case APP_INIT:
            waitForVBlank();
            fillScreenDMA(WHITE);
            initializeAppState(&currentAppState);
            state = APP_MAP;
            break;
        case APP_MAP:

            waitForVBlank();
            nextAppState = processAppStateMap(&currentAppState, previousButtons, currentButtons);
            drawAppStateMap(&nextAppState);
            
            currentAppState = nextAppState;
            if (currentAppState.toMenu) {
                state = APP_MENU;
            }
            if (currentAppState.toMove) {
                state = APP_MOVE;
            }
            break;
        case APP_MENU:
            waitForVBlank();
            nextAppState = processAppStateMenu(&currentAppState, previousButtons, currentButtons);
            drawAppStateMenu(&nextAppState);
            
            currentAppState = nextAppState;
            if (currentAppState.toMap) {
                state = APP_MAP;
            }
            if (currentAppState.toMove) {
                state = APP_MOVE;
            }  
            break;
        case APP_MOVE:
            waitForVBlank();
            nextAppState = processAppStateMove(&currentAppState, previousButtons, currentButtons);
            drawAppStateMap(&nextAppState);

            currentAppState = nextAppState;
            if (currentAppState.toMap) {
                state = APP_MAP;
            }
            if (currentAppState.toMenu) {
                state = APP_MENU;
            }
            break;
        }

        // Store the current state of the buttons
        previousButtons = currentButtons;
    }

    return 0;
}
