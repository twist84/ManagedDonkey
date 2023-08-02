#include "game/materials_definitions.hpp"

void s_global_material_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(breakable_surface);
	UPDATE_REFERENCE_NAME(material_effects);

	UPDATE_STRUCT_REFERENCE_NAMES(sweeteners);
}

void s_global_material_sweeteners_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(sound_sweetener_small);
	UPDATE_REFERENCE_NAME(sound_sweetener_medium);
	UPDATE_REFERENCE_NAME(sound_sweetener_large);
	UPDATE_REFERENCE_NAME(sound_sweetener_rolling);
	UPDATE_REFERENCE_NAME(sound_sweetener_grinding);
	UPDATE_REFERENCE_NAME(sound_sweetener_melee_small);
	UPDATE_REFERENCE_NAME(sound_sweetener_melee);
	UPDATE_REFERENCE_NAME(sound_sweetener_melee_large);
	UPDATE_REFERENCE_NAME(effect_sweetener_small);
	UPDATE_REFERENCE_NAME(effect_sweetener_medium);
	UPDATE_REFERENCE_NAME(effect_sweetener_large);
	UPDATE_REFERENCE_NAME(effect_sweetener_rolling);
	UPDATE_REFERENCE_NAME(effect_sweetener_grinding);
	UPDATE_REFERENCE_NAME(effect_sweetener_melee);
	UPDATE_REFERENCE_NAME(water_ripple_small);
	UPDATE_REFERENCE_NAME(water_ripple_medium);
	UPDATE_REFERENCE_NAME(water_ripple_large);
}

