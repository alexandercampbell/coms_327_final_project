
#include "item.hpp"

struct ItemPrototype {
	int min_level;
	int max_level;
	Item item;
};

static ItemPrototype mk_weapon(int min_level, int max_level, string name, char symb,
		string desc, Dice damage, float hp_mul=1.f, float damage_mul=1.f) {

	ItemPrototype i = {0};

	i.min_level = min_level;
	i.max_level = max_level;

	i.item.type = ItemType::weapon;
	i.item.name = name;
	i.item.desc = desc;
	i.item.symb = symb;
	i.item.damage = damage;
	i.item.hp_mul = hp_mul;
	i.item.damage_mul = damage_mul;

	return i;
}

static ItemPrototype mk_ring(int min_level, int max_level, string name,
		string desc, RingAbility ability, float hp_mul=1.f, float damage_mul=1.f) {

	ItemPrototype i = {0};

	i.min_level = min_level;
	i.max_level = max_level;

	i.item.type = ItemType::ring;
	i.item.name = name;
	i.item.desc = desc;
	i.item.symb = 'o';
	i.item.damage_mul = damage_mul;
	i.item.hp_mul = hp_mul;
	i.item.ability = ability;

	return i;
}

const static vector<ItemPrototype> available_items = {
	mk_weapon(0, 2, "a hunter's knife", '|',
		"Rusty hunter's knife. Sharp, but a little too small for serious combat.",
		mk_dice(3, 1, 4)),

	mk_weapon(1, 3, "an Orcish sword", '|',
		"An orcish sword. Crooked and tinged green.",
		mk_dice(5, 2, 3)),

	mk_weapon(1, 3, "an Elvish handaxe", '|',
		"This light axe is surprisingly elegant.",
		mk_dice(8, 1, 3)),

	mk_ring(0, 0, "a grass Ring",
		"This woven grass Ring pulses green, yellow, and oddly, blue.",
		RingAbility::teleport_to_town),

	mk_ring(3, 10, "a sacrificial Ring",
		"This dark red Ring is oozing a puce liquid.",
		RingAbility::sacrifice),
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

