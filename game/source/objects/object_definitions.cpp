#include "objects/object_definitions.hpp"

void _object_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(model);
	UPDATE_REFERENCE_NAME(crate_object);
	UPDATE_REFERENCE_NAME(collision_damage);
	UPDATE_REFERENCE_NAME(creation_effect);
	UPDATE_REFERENCE_NAME(material_effects);
	UPDATE_REFERENCE_NAME(armor_sounds);
	UPDATE_REFERENCE_NAME(melee_sound);

	UPDATE_BLOCK_REFERENCE_NAMES(attachments);
	UPDATE_BLOCK_REFERENCE_NAMES(widgets);
	UPDATE_BLOCK_REFERENCE_NAMES(multiplayer_object);
	UPDATE_BLOCK_REFERENCE_NAMES(health_packs);
}

void object_definition::update_reference_names()
{
	UPDATE_STRUCT_REFERENCE_NAMES(object);
}

void object_attachment_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(type);
}

void object_definition_widget::update_reference_names()
{
	UPDATE_REFERENCE_NAME(type);
}

void s_object_health_pack_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(health_pack_equipment);
}
