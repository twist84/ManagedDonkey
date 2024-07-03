#include "items/item_definitions.hpp"

void _item_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(collision_sound);
	UPDATE_REFERENCE_NAME(detonation_damage_effect);
	UPDATE_REFERENCE_NAME(detonating_effect);
	UPDATE_REFERENCE_NAME(detonation_effect);

	UPDATE_BLOCK_REFERENCE_NAMES(predicted_bitmaps);
}

void s_predicted_bitmaps::update_reference_names()
{
	UPDATE_REFERENCE_NAME(bitmap);
}

void item_definition::update_reference_names()
{
	UPDATE_STRUCT_REFERENCE_NAMES(item);
}

