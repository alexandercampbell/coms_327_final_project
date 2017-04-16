
#include "item.hpp"

struct ItemPrototype {
	int min_level;
	int max_level;
	Item item;
};

static ItemPrototype mk_weapon(int min_level, int max_level, string name, char symb,
		string desc, Dice damage, float hp_mul) {

	ItemPrototype i = {0};

	i.min_level = min_level;
	i.max_level = max_level;

	i.item.name = name;
	i.item.desc = desc;
	i.item.symb = symb;
	i.item.damage = damage;
	i.item.damage_mul = 1.f;
	i.item.hp_mul = hp_mul;

	return i;
}

const static vector<ItemPrototype> available_items = {
	mk_weapon(0, 3, "Kitchen Knife", '-',
		"Standard steak knife. Wickedly sharp. Unfortunately, ill-suited to combat.",
		mk_dice(3, 1, 4), 0),
};

Item *item_generate(int depth) {
	for (int attempts = 0; attempts < 150; attempts++) {
		int index = RAND_BETWEEN(0, available_items.size());
		const ItemPrototype *i = &available_items[index];
		if (depth >= i->min_level && depth <= i->max_level) {
			return (Item*) &i->item;
		}
	}

	return NULL;
}

