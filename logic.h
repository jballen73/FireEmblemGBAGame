#ifndef LOGIC_SEEN
#define LOGIC_SEEN

#include "gba.h"

#define GROUND_NONE (0)
#define GROUND_REGULAR (1)
#define GROUND_KILL (1<<4)
#define COLLISION_BACK (1<<2)
#define COLLISION_ADVANCE (1<<3)
#define COLLISION_SAVE (1<<5)

#define GROUND_KILL_VALUE (0x107d)
#define WALL_BACK (0x7680)
#define WALL_ADVANCE (0x26c4)
#define SAVE_BLOCK (0x5134)

#define NUM_ROOMS (19)
#define STARTING_ROOM (0)


typedef struct {
    int xpos;
    int ypos;

} TileSelector;
typedef struct {
    int attack;
    int item;
    int wait;
    int numItems;

} Menu;
typedef struct {
    // Store whether or not the game is over in this member:
    int gameOver;

    TileSelector *tSelector;
    Menu *menu;
    int menuSelectorPosition;
    int toMenu;
    int toMap;
} AppState;


// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);
// This function will be used to process app frames.
AppState processAppStateMap(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);
AppState processAppStateMenu(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);
// If you have anything else you need accessible from outside the logic.c
// file, you can add them here. You likely won't.
#endif
