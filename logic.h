#ifndef LOGIC_SEEN
#define LOGIC_SEEN

#include "gba.h"



#define NUM_UNITS (4)
typedef enum {
    REDTEAM,
    BLUETEAM,
} Team;
typedef enum {
    ATTACK,
    ITEM,
    WAIT,
} MenuOption;



typedef struct {
    int dead;
    int id;
    Team team;
    int xpos;
    int ypos;
    int hasMoved;
    int baseAttr0;
    int baseAttr1;

    int maxHP;
    int curHP;
    int atk;
    int def;
    int skl;
    int spd;
    int move;
} Unit;
typedef struct {
    int xpos;
    int ypos;
    int show;
} TileSelector;
typedef struct {
    int attack;
    int item;
    int wait;
    int numItems;

} Menu;
typedef struct {
    int unitID;
    int startX;
    int startY;
    int dx;
    int dy;
    int totalDist;

} Move;
typedef struct {
    Unit *attacker;
    Unit *defender;
    int toAttack;
    int toMap;
    int counter;
} CombatState;
typedef struct {
    // Store whether or not the game is over in this member:
    int gameOver;

    TileSelector *tSelector;
    Menu *menu;
    Unit *unitList[NUM_UNITS];
    Move *currentMove;
    int map[15][10];
    int menuSelectorPosition;
    int selected;
    int targeted;
    int toMenu;
    int toMap;
    int toMove;
    int toAttackTargeting;
    int toEnemy;
    int toEnemyMove;
} AppState;


// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);
// This function will be used to process app frames.
AppState processAppStateMap(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);
AppState processAppStateMenu(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);
AppState processAppStateMove(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);
AppState processAppStateEnemy(AppState *currentAppState);
AppState processAppStateEnemyMove(AppState *currentAppState);
AppState processAppStateAttackTargeting(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow, CombatState *combatState);
CombatState processCombatState(CombatState *combatState);
// If you have anything else you need accessible from outside the logic.c
// file, you can add them here. You likely won't.
#endif
