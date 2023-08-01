#include "items/item_definitions.hpp"

void _item_definition::update_reference_names()
{
	collision_sound.get_name();
	detonation_damage_effect.get_name();
	detonating_effect.get_name();
	detonation_effect.get_name();

	UPDATE_REFERENCE_NAMES(predicted_bitmaps);
}

void s_predicted_bitmaps::update_reference_names()
{
	bitmap.get_name();
}

