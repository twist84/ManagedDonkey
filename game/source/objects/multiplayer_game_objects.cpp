#include "objects/multiplayer_game_objects.hpp"

#include "memory/module.hpp"
#include "render/render_debug.hpp"

#include <math.h>

HOOK_DECLARE(0x00BA0FB0, multiplayer_object_render_debug_boundary);

bool mp_debug_goal_object_boundary_geometry = false;

bool __cdecl game_in_editor_for_multiplayer_object_render_debug_boundary() // game_in_editor
{
	return mp_debug_goal_object_boundary_geometry;
}
HOOK_DECLARE_CALL(0x006E501C, game_in_editor_for_multiplayer_object_render_debug_boundary);

bool __cdecl editor_should_render_transparent_geometry_for_multiplayer_object_render_debug_boundary() // editor_should_render_transparent_geometry
{
	return mp_debug_goal_object_boundary_geometry;
}
HOOK_DECLARE_CALL(0x006E5025, editor_should_render_transparent_geometry_for_multiplayer_object_render_debug_boundary);

void __cdecl multiplayer_object_render_debug_boundary(s_multiplayer_object_boundary_geometry_data const* geometry_data, real_argb_color const* color)
{
	// requires `game_in_editor` and `editor_should_render_transparent_geometry` to return true

	ASSERT(geometry_data);
	ASSERT(color);

	if (mp_debug_goal_object_boundary_geometry)
	{
		e_multiplayer_object_boundary_shape boundary_shape = geometry_data->boundary_shape;
		switch (boundary_shape)
		{
		case _multiplayer_object_boundary_shape_sphere:
		{
			render_debug_sphere(false, &geometry_data->boundary_matrix.position, geometry_data->boundary_width, color);
		}
		break;
		case _multiplayer_object_boundary_shape_cylinder:
		{
			vector3d height{};
			real_point3d base{};

			real absolute_boundary_positive_height = fabsf(geometry_data->boundary_positive_height);
			real absolute_boundary_negative_height = fabsf(geometry_data->boundary_negative_height);

			height.k = absolute_boundary_negative_height + absolute_boundary_positive_height;
			base.z = height.k * -0.5f;

			matrix4x3_transform_vector(&geometry_data->boundary_matrix, &height, &height);
			matrix4x3_transform_point(&geometry_data->boundary_matrix, &base, &base);

			render_debug_cylinder(false, &base, &height, geometry_data->boundary_width, color);
		}
		break;
		case _multiplayer_object_boundary_shape_box:
		{
			real_rectangle3d bounds{};

			real absolute_boundary_positive_height = fabsf(geometry_data->boundary_positive_height);
			real absolute_boundary_negative_height = fabsf(geometry_data->boundary_negative_height);
			real absolute_boundary_width = fabsf(geometry_data->boundary_width);
			real absolute_boundary_box_length = fabsf(geometry_data->boundary_box_length);

			bounds.x.lower = absolute_boundary_width / 2;
			bounds.x.upper = absolute_boundary_width / -2;

			bounds.y.lower = absolute_boundary_box_length / 2;
			bounds.y.upper = absolute_boundary_box_length / -2;

			real v7 = absolute_boundary_positive_height - ((absolute_boundary_negative_height + absolute_boundary_positive_height) / 2);
			bounds.z.lower = absolute_boundary_positive_height - v7;
			bounds.z.upper = -absolute_boundary_negative_height - v7;

			render_debug_box_outline_oriented(false, &bounds, &geometry_data->boundary_matrix, color);
		}
		break;
		}
	}
}

void debug_multiplayer_object_boundary_geometry(bool should_debug)
{
	mp_debug_goal_object_boundary_geometry = should_debug;
}

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

