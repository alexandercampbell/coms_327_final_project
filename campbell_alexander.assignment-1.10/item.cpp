
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
	/**
	 * Weapons
	 */

	mk_weapon(0, 2, "a hunter's knife", 'k',
		"Rusty hunter's knife. Sharp, but a little too small for serious combat.",
		mk_dice(3, 1, 4)),

	mk_weapon(1, 3, "an Orcish sword", 's',
		"An orcish sword. Crooked and tinged green.",
		mk_dice(5, 2, 3)),

	mk_weapon(1, 3, "an Elvish handaxe", 'p',
		"This light axe is surprisingly elegant.",
		mk_dice(8, 1, 4)),

	mk_weapon(0, 2, "an old man's cane", 'c',
		"Heavier and sturdier than it looks.",
		mk_dice(6, 0, 0)),

	mk_weapon(2, 3, "a cutthroat razor", 'r',
		"Razor. Can be folded open or closed.",
		mk_dice(3, 2, 4)),

	mk_weapon(2, 4, "an enormous scythe", '7',
		"Huge and crushingly heavy even to lift from the ground.",
		mk_dice(0, 2, 16)),

	mk_weapon(3, 5, "a dwarvish warhammer", 'W',
		"Beautifully forged out of silver.",
		mk_dice(6, 6, 3)),

	mk_weapon(5, 5, "Lucifer's Blade", 'L',
		"The blade is patterned with shifting stains. Warm to the touch.",
		mk_dice(20, 6, 6)),

	/**
	 * Rings
	 */

	mk_ring(0, 3, "a grass town Ring",
		"This woven grass Ring pulses green, yellow, and oddly, blue.",
		RingAbility::teleport_to_town),

	mk_ring(3, 5, "a sacrificial Ring",
		"This dark red Ring is oozing a puce liquid.",
		RingAbility::sacrifice),

	mk_ring(0, 5, "a vitality Ring",
		"This ring is crystal clear with flecks of blue.",
		RingAbility::boost_hp),

	mk_ring(0, 5, "a Ring of revelations",
		"This ring is a dark shade of green, with papery texture.",
		RingAbility::reveal),
};

Item *item_generate(int depth) {
	for (int attempts = 0; attempts < 150; attempts++) {
		int index = RAND_BETWEEN(0, available_items.size() - 1);
		const ItemPrototype *i = &available_items[index];
		if (depth >= i->min_level && depth <= i->max_level) {
			return (Item*) &i->item;
		}
	}

	return NULL;
}

