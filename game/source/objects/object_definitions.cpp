#include "objects/object_definitions.hpp"

void _object_definition::update_reference_names()
{
	model.get_name();
	crate_object.get_name();
	collision_damage.get_name();
	creation_effect.get_name();
	material_effects.get_name();
	armor_sounds.get_name();
	melee_sound.get_name();

	UPDATE_REFERENCE_NAMES(attachments);
	UPDATE_REFERENCE_NAMES(widgets);
	UPDATE_REFERENCE_NAMES(multiplayer_object);
	UPDATE_REFERENCE_NAMES(health_packs);
}

void object_attachment_definition::update_reference_names()
{
}

void object_definition_widget::update_reference_names()
{
}

void s_object_health_pack_definition::update_reference_names()
{
	health_pack_equipment.get_name();
}
