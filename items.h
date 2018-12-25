#ifndef ITEMS_SEEN
#define ITEMS_SEEN

typedef enum {
    WEAPON,
    CONSUMABLE,
} ItemType;
typedef struct {
    ItemType type;
    int strength;
    int accuracy;
    int uses;
} Item;

Item *newIronSword(void);
Item *newSteelSword(void);
Item *newVulnerary(void);
#endif