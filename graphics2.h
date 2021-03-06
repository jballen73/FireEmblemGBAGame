#ifndef GRAPHICS2_SEEN
#define GRAPHICS2_SEEN

#include "logic.h"

void graphics2Init(void);
void drawCombatBackground(void);
void doRedAttack(void);
void doBlueAttack(void);
void drawCombatState(CombatState *state);
void fullDrawCombatState(CombatState *state);
#endif