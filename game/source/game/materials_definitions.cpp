#include "game/materials_definitions.hpp"

void s_global_material_definition::update_reference_names()
{
	breakable_surface.get_name();
	material_effects.get_name();

	sweeteners.update_reference_names();
}

void s_global_material_sweeteners_definition::update_reference_names()
{
	sound_sweetener_small.get_name();
	sound_sweetener_medium.get_name();
	sound_sweetener_large.get_name();
	sound_sweetener_rolling.get_name();
	sound_sweetener_grinding.get_name();
	sound_sweetener_melee_small.get_name();
	sound_sweetener_melee.get_name();
	sound_sweetener_melee_large.get_name();
	effect_sweetener_small.get_name();
	effect_sweetener_medium.get_name();
	effect_sweetener_large.get_name();
	effect_sweetener_rolling.get_name();
	effect_sweetener_grinding.get_name();
	effect_sweetener_melee.get_name();
	water_ripple_small.get_name();
	water_ripple_medium.get_name();
	water_ripple_large.get_name();
}

