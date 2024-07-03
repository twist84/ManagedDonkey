#include "units/biped_definitions.hpp"

void _biped_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(area_damage_effect);
	UPDATE_REFERENCE_NAME(reanimation_character);
	UPDATE_REFERENCE_NAME(reanimation_morph_muffins);
	UPDATE_REFERENCE_NAME(death_spawn_character);
}

void biped_definition::update_reference_names()
{
	UPDATE_STRUCT_REFERENCE_NAMES(biped);
}

