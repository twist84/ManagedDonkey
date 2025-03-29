#include "game/materials_definitions.hpp"

void s_global_material_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(breakable_surface);
	UPDATE_REFERENCE_NAME(material_effects);

	UPDATE_STRUCT_REFERENCE_NAMES(sweeteners);
}

void s_global_material_sweeteners_definition::update_reference_names()
{
	UPDATE_ARRAY_REFERENCE_NAMES(sweetener_tag_references);
}

