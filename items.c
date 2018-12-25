#include "items.h"
#include <stdlib.h>

Item *newIronSword(void) {
    Item *ironSword = malloc(sizeof(Item));
    ironSword->type = WEAPON;
    ironSword->strength = 3;
    ironSword->accuracy = 95;
    ironSword->uses = 20;
    return ironSword;
}
Item *newSteelSword(void) {
    Item *steelSword = malloc(sizeof(Item));
    steelSword->type = WEAPON;
    steelSword->strength = 6;
    steelSword->accuracy = 80;
    steelSword->uses = 15;
    return steelSword;
}
Item *newVulnerary(void) {
    Item *vulnerary = malloc(sizeof(Item));
    vulnerary->type = CONSUMABLE;
    vulnerary->strength = 10;
    vulnerary->accuracy = 0;
    vulnerary->uses = 3;
    return vulnerary;
}