
#include "item.hpp"

struct ItemPrototype {
	int min_level;
	int max_level;
	Item item;
};

static ItemPrototype mk_weapon(ItemType type, string name, string desc, int min_level,
		int max_level, Dice damage, float damage_mul, float hp_mul) {

	assert(IS_WEAPON(type));
	ItemPrototype i;

	i.min_level = min_level;
	i.max_level = max_level;

	i.item.name = name;
	i.item.desc = desc;
	i.item.type = type;
	i.item.damage = damage;
	i.item.damage_mul = damage_mul;
	i.item.hp_mul = hp_mul;

	return i;
}

const static vector<ItemPrototype> available_items = {
	mk_weapon(ItemType::knife, "Kitchen Knife",
		"Your standard steak knife. Wickedly sharp. Unfortunately, ill-suited to combat.",
		0, 3, mk_dice(3, 1, 4), 0, 0),
};

Item item_generate(int depth) {
	while (true) {
		int index = RAND_BETWEEN(0, available_items.size());
		const ItemPrototype *i = &available_items[index];
		if (depth >= i->min_level && depth <= i->max_level) {
			return i->item;
		}
	}
}

