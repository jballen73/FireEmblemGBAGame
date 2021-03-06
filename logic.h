#ifndef LOGIC_SEEN
#define LOGIC_SEEN

#include "gba.h"
#include "items.h"


#define NUM_UNITS (7)
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

    Item *items[5];
    int numItems;
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
    int position1;
    int position2;
    int position3;
} ItemMenu;
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
    int atkHit;
    int atkDmg;
    int atkCrit;
    int atkNum;
    int defHit;
    int defDmg;
    int defCrit;
    int defNum;

    int state;
    int redAtk;
    int blueAtk;
    int enemy;
} CombatState;
typedef struct {
    // Store whether or not the game is over in this member:
    int gameOver;

    TileSelector *tSelector;
    Menu *menu;
    ItemMenu *itemMenu;
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
    int toItemMenu;
    int toEnemy;
    int toEnemyMove;
    int toEnemyTargeting;
} AppState;


// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);
void initializeCombatState(CombatState *combatState);
// This function will be used to process app frames.
AppState processAppStateMap(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);
AppState processAppStateMenu(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);
AppState processAppStateMove(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);
AppState processAppStateEnemy(AppState *currentAppState);
AppState processAppStateEnemyMove(AppState *currentAppState);
AppState processAppStateEnemyTargeting(AppState *currentAppState, CombatState *combatState);
AppState processAppStateItemMenu(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);
AppState processAppStateAttackTargeting(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow, CombatState *combatState);
CombatState processCombatState(CombatState *combatState);
CombatState processRedAttack(CombatState *combatState);
CombatState processBlueAttack(CombatState *combatState);
AppState processPostCombat(AppState *currentAppState, CombatState *combatState);

// If you have anything else you need accessible from outside the logic.c
// file, you can add them here. You likely won't.
#endif
