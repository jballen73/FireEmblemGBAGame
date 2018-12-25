#include "gba.h"
#include "logic.h"
#include "graphics.h"
#include "graphics2.h"
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
    APP_ENEMY,
    APP_ENEMY_MOVE,
    APP_ATTACK_TARGETING,
    APP_ITEM_MENU,
    APP_COMBAT_INIT,
    APP_COMBAT,
    APP_COMBAT_END,
} GBAState;

int main(void) {
    // TA-TODO: Manipulate REG_DISPCNT here to set Mode 3.
    REG_DISPCNT = MODE3 | BG2_ENABLE | OBJ_ENABLE | SPRITES_DIMENSION_TYPE;

    GBAState state = START;

    // We store the "previous" and "current" states.
    AppState currentAppState, nextAppState;
    CombatState combatState;
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
                currentAppState.tSelector->show = 1;
                state = APP_MENU;
            }
            if (currentAppState.toMove) {
                state = APP_MOVE;
            }
            if (currentAppState.toEnemy) {
                currentAppState.tSelector->show = 0;
                waitForVBlank();
                drawAppStateMap(&currentAppState);
                for (int i = 0; i < 60; i++) {
                    waitForVBlank();
                    showEnemyTurn();
                }
                hideEnemyTurn();
                state = APP_ENEMY;
            }
            break;
        case APP_MENU:
            waitForVBlank();
            nextAppState = processAppStateMenu(&currentAppState, previousButtons, currentButtons);
            drawAppStateMenu(&nextAppState);
            
            currentAppState = nextAppState;
            if (currentAppState.toMap) {
                currentAppState.tSelector->show = 1;
                state = APP_MAP;
            }
            if (currentAppState.toMove) {
                currentAppState.tSelector->show = 1;
                state = APP_MOVE;
            }  
            if (currentAppState.toAttackTargeting) {
                currentAppState.tSelector->show = 1;
                state = APP_ATTACK_TARGETING;
            }
            if (currentAppState.toItemMenu) {
                state = APP_ITEM_MENU;
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
                currentAppState.tSelector->show = 1;
                state = APP_MENU;
            }
            break;
        case APP_ITEM_MENU:
            waitForVBlank();
            nextAppState = processAppStateItemMenu(&currentAppState, previousButtons, currentButtons);
            drawAppStateItemMenu(&nextAppState);
            currentAppState = nextAppState;
            if (currentAppState.toMenu) {
                state = APP_MENU;
            }
            if (currentAppState.toMap) {
                state = APP_MAP;
            }
            break;
        case APP_ENEMY:
            waitForVBlank();
            nextAppState = processAppStateEnemy(&currentAppState);
            drawAppStateMap(&nextAppState);
            currentAppState = nextAppState;
            if (currentAppState.toEnemyMove) {
                state = APP_ENEMY_MOVE;
            }
            if (currentAppState.toMap) {
                currentAppState.tSelector->show = 1;
                waitForVBlank();
                showYourTurn();
                for (int i = 0; i < 60; i++) {
                    waitForVBlank();
                    showYourTurn();
                }
                hideYourTurn();
                state = APP_MAP;
            }
            break;
        case APP_ENEMY_MOVE:
            waitForVBlank();
            if (vBlankCounter % 15 == 0) {
                nextAppState = processAppStateEnemyMove(&currentAppState);
                drawAppStateMap(&nextAppState);
                currentAppState = nextAppState;
                if (currentAppState.toEnemy) {
                    state = APP_ENEMY;
                }
            }
            break;
        case APP_ATTACK_TARGETING:
            waitForVBlank();
            nextAppState = processAppStateAttackTargeting(&currentAppState, previousButtons, currentButtons, &combatState);
            drawAppStateMap(&nextAppState);
            currentAppState = nextAppState;
            if (currentAppState.toMenu) {
                state = APP_MENU;
            }
            if (combatState.toAttack) {
                state = APP_COMBAT_INIT;
            }
            break;
        case APP_COMBAT_INIT:
            graphics2Init();
            waitForVBlank();
            drawCombatBackground();
            drawCombatState(&combatState);
            state = APP_COMBAT;
            break;
        case APP_COMBAT:
            waitForVBlank();
            combatState = processCombatState(&combatState);
            drawCombatState(&combatState);
            if (combatState.toMap) {
                state = APP_COMBAT_END;
            }
            break;
        case APP_COMBAT_END:
            combatState.toMap = 0;
            graphicsInit();
            fullDrawAppState(&currentAppState);
            state = APP_MAP;
            break;
        }

        // Store the current state of the buttons
        previousButtons = currentButtons;
    }

    return 0;
}
