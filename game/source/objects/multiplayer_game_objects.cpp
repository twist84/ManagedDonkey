#include "multiplayer_game_objects.hpp"

void s_multiplayer_object_properties_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(spawned_object);
	UPDATE_REFERENCE_NAME(boundary_standard_shader);
	UPDATE_REFERENCE_NAME(boundary_opaque_shader);
	UPDATE_REFERENCE_NAME(sphere_standard_shader);
	UPDATE_REFERENCE_NAME(sphere_opaque_shader);
	UPDATE_REFERENCE_NAME(cylinder_standard_shader);
	UPDATE_REFERENCE_NAME(cylinder_opaque_shader);
	UPDATE_REFERENCE_NAME(box_standard_shader);
	UPDATE_REFERENCE_NAME(box_opaque_shader);
}

