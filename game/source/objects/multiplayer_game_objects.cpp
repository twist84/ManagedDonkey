#include "multiplayer_game_objects.hpp"

void s_multiplayer_object_properties_definition::update_reference_names()
{
	spawned_object.get_name();
	boundary_standard_shader.get_name();
	boundary_opaque_shader.get_name();
	sphere_standard_shader.get_name();
	sphere_opaque_shader.get_name();
	cylinder_standard_shader.get_name();
	cylinder_opaque_shader.get_name();
	box_standard_shader.get_name();
	box_opaque_shader.get_name();
}

