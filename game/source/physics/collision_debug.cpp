#include "physics/collision_debug.hpp"

#include "camera/director.hpp"
#include "cseries/cseries.hpp"
#include "game/player_mapping.hpp"
#include "physics/collision_constants.hpp"
#include "physics/collision_features.hpp"
#include "physics/collision_references.hpp"
#include "physics/collision_usage.hpp"
#include "physics/collisions.hpp"
#include "render/render_debug.hpp"
#include "render/views/render_view.hpp"

#include <math.h>

bool collision_debug_test_terrain_shader = true;
bool collision_debug_water_proxy = true;
bool collision_debug_flag_structure = true;
bool collision_debug_flag_water = true;
bool collision_debug_flag_instanced_geometry = true;
bool collision_debug_flag_objects = true;
bool collision_debug_flag_front_facing_surfaces = true;
bool collision_debug_flag_ignore_invisible_surfaces = true;
real collision_debug_length = 100.0f;
long collision_debug_ignore_object_index = NONE;
bool collision_debug_geometry_sampling = false;
bool collision_debug_flag_objects_terminals = false;
bool g_collision_debug_status_lines_render = false;
bool collision_debug = false;
bool collision_debug_spray = false;
bool collision_debug_features = false;
bool collision_debug_phantom_bsp = false;
bool collision_debug_lightmaps = false;
bool collision_debug_flags = true;
bool collision_debug_flag_objects_bipeds = false;
bool collision_debug_flag_objects_giants = false;
bool collision_debug_flag_objects_effect_scenery = false;
bool collision_debug_flag_objects_vehicles = false;
bool collision_debug_flag_objects_weapons = false;
bool collision_debug_flag_objects_equipment = false;
bool collision_debug_flag_objects_projectiles = false;
bool collision_debug_flag_objects_scenery = false;
bool collision_debug_flag_objects_machines = false;
bool collision_debug_flag_objects_controls = false;
bool collision_debug_flag_objects_sound_scenery = false;
bool collision_debug_flag_objects_crates = false;
bool collision_debug_flag_objects_creatures = false;
bool collision_debug_flag_ignore_child_objects = false;
bool collision_debug_flag_ignore_nonpathfindable_objects = false;
bool collision_debug_flag_ignore_cinematic_objects = false;
bool collision_debug_flag_ignore_dead_bipeds = false;
bool collision_debug_flag_ignore_passthrough_bipeds = false;
bool collision_debug_flag_back_facing_surfaces = false;
bool collision_debug_flag_ignore_two_sided_surfaces = false;
bool collision_debug_flag_ignore_breakable_surfaces = false;
bool collision_debug_flag_allow_early_out = false;
bool collision_debug_flag_try_to_keep_location_valid = false;
bool collision_debug_repeat = false;
real collision_debug_width = 0.0f;
real_point3d collision_debug_point{};
real collision_debug_height = 0.0f;
vector3d collision_debug_vector{};
//bool collision_debug_? = false;
//s_status_line g_collision_debug_status_lines[10]{};
//s_status_line g_collision_debug_lightmap_status_line{};
//s_status_line g_collision_debug_lightmap_status_line_red_sh{};
//s_status_line g_collision_debug_lightmap_status_line_green_sh{};
//s_status_line g_collision_debug_lightmap_status_line_blue_sh{};
//c_debug_lightmap_drawer global_lightmap_drawer{}:

void collision_debug_render()
{
	// #TODO: implement me

	if (global_collision_log_enable)
	{
		ASSERT(VALID_INDEX(global_current_collision_user_depth, MAXIMUM_COLLISION_USER_STACK_DEPTH));
		global_current_collision_users[global_current_collision_user_depth++] = 25; // debug
	}

	if (!collision_debug &&
		collision_debug == collision_debug_spray &&
		collision_debug == collision_debug_features &&
		collision_debug == collision_debug_phantom_bsp &&
		collision_debug == collision_debug_lightmaps &&
		collision_debug == collision_debug_geometry_sampling)
	{
		if (global_collision_log_enable)
		{
			ASSERT(global_current_collision_user_depth > 1);
			global_current_collision_user_depth--;
		}

		return;
	}

	s_collision_test_flags collision_test_flags{};
	if (collision_debug_flags)
	{
		collision_test_flags = _collision_test_empty_flags;
	}
	else
	{
		collision_test_flags.collision_flags.set(_collision_test_structure_bit, collision_debug_flag_structure);
		collision_test_flags.collision_flags.set(_collision_test_water_bit, collision_debug_flag_water);
		collision_test_flags.collision_flags.set(_collision_test_instanced_geometry_bit, collision_debug_flag_instanced_geometry);

		collision_test_flags.object_flags.set(_collision_test_objects_bit, collision_debug_flag_objects);
		collision_test_flags.object_flags.set(_collision_test_objects_biped_bit, collision_debug_flag_objects_bipeds);
		collision_test_flags.object_flags.set(_collision_test_objects_giant_bit, collision_debug_flag_objects_giants);
		collision_test_flags.object_flags.set(_collision_test_objects_vehicle_bit, collision_debug_flag_objects_vehicles);
		collision_test_flags.object_flags.set(_collision_test_objects_weapon_bit, collision_debug_flag_objects_weapons);
		collision_test_flags.object_flags.set(_collision_test_objects_equipment_bit, collision_debug_flag_objects_equipment);
		collision_test_flags.object_flags.set(_collision_test_objects_terminal_bit, collision_debug_flag_objects_terminals);
		collision_test_flags.object_flags.set(_collision_test_objects_projectile_bit, collision_debug_flag_objects_projectiles);
		collision_test_flags.object_flags.set(_collision_test_objects_scenery_bit, collision_debug_flag_objects_scenery);
		collision_test_flags.object_flags.set(_collision_test_objects_machine_bit, collision_debug_flag_objects_machines);
		collision_test_flags.object_flags.set(_collision_test_objects_control_bit, collision_debug_flag_objects_controls);
		collision_test_flags.object_flags.set(_collision_test_objects_sound_scenery_bit, collision_debug_flag_objects_sound_scenery);
		collision_test_flags.object_flags.set(_collision_test_objects_crate_bit, collision_debug_flag_objects_crates);
		collision_test_flags.object_flags.set(_collision_test_objects_creature_bit, collision_debug_flag_objects_creatures);
		collision_test_flags.object_flags.set(_collision_test_objects_effect_scenery_bit, collision_debug_flag_objects_effect_scenery);

		collision_test_flags.collision_flags.set(_collision_test_ignore_child_objects_bit, collision_debug_flag_ignore_child_objects);
		collision_test_flags.collision_flags.set(_collision_test_ignore_nonpathfindable_objects_bit, collision_debug_flag_ignore_nonpathfindable_objects);
		collision_test_flags.collision_flags.set(_collision_test_ignore_cinematic_objects_bit, collision_debug_flag_ignore_cinematic_objects);
		collision_test_flags.collision_flags.set(_collision_test_ignore_dead_bipeds_bit, collision_debug_flag_ignore_dead_bipeds);
		collision_test_flags.collision_flags.set(_collision_test_front_facing_surfaces_bit, collision_debug_flag_front_facing_surfaces);
		collision_test_flags.collision_flags.set(_collision_test_back_facing_surfaces_bit, collision_debug_flag_back_facing_surfaces);
		collision_test_flags.collision_flags.set(_collision_test_ignore_two_sided_surfaces_bit, collision_debug_flag_ignore_two_sided_surfaces);
		collision_test_flags.collision_flags.set(_collision_test_ignore_invisible_surfaces_bit, collision_debug_flag_ignore_invisible_surfaces);
		collision_test_flags.collision_flags.set(_collision_test_ignore_breakable_surfaces_bit, collision_debug_flag_ignore_breakable_surfaces);
		collision_test_flags.collision_flags.set(_collision_test_allow_early_out_bit, collision_debug_flag_allow_early_out);
		collision_test_flags.collision_flags.set(_collision_test_try_to_keep_location_valid_bit, collision_debug_flag_try_to_keep_location_valid);

		if (!collision_test_flags.collision_flags.test(_collision_test_structure_bit) &&
			!collision_test_flags.collision_flags.test(_collision_test_water_bit) &&
			!collision_test_flags.collision_flags.test(_collision_test_instanced_geometry_bit) &&
			!collision_test_flags.object_flags.test(_collision_test_objects_bit))
		{
			collision_test_flags.collision_flags.set(_collision_test_structure_bit, true);
		}
	}

	real_point3d debug_point0 = collision_debug_point;

	if (collision_debug_repeat)
	{
		// #TODO: implement me
		// collision_debug_vector
	}
	else
	{
		collision_debug_ignore_object_index = NONE;

		e_output_user_index user_index = c_player_view::get_current()->get_player_view_output_user_index();
		if (user_index != NONE && !director_get_perspective(user_index))
			collision_debug_ignore_object_index = player_mapping_get_unit_by_output_user(user_index);

		real_matrix4x3 camera{};
		c_player_view::get_player_render_camera_orientation(&camera);
		collision_debug_point = camera.center;
		collision_debug_vector = camera.matrix.forward;
	}
	
	if (collision_debug)
	{
		// #TODO: implement me

		if (collision_debug_length <= 0.0f)
		{
			//if (collision_debug_width > 0.0f)
			//{
			//	if (collision_test_sphere(collision_test_flags, &debug_point0, collision_debug_width))
			//		render_debug_sphere(true, &debug_point0, collision_debug_width, global_real_argb_red);
			//	else
			//		render_debug_sphere(true, &debug_point0, collision_debug_width, global_real_argb_green);
			//}
			//else
			//{
			//	real_argb_color const* color = global_real_argb_green;
			//
			//	e_collision_result_type collision_result_type{};
			//	if (collision_test_point(collision_test_flags, &debug_point0, collision_debug_ignore_object_index, NONE, &collision_result_type))
			//	{
			//		char const* const collision_result_type_names[k_collision_result_type_count]
			//		{
			//			"none",
			//			"structure",
			//			"water",
			//			"instanced_geometry",
			//			"object"
			//		};
			//
			//		ASSERT(VALID_INDEX(collision_result_type, k_collision_result_type_count));
			//
			//		g_collision_debug_status_lines_render = true;
			//		g_collision_debug_status_lines[0].string.print("inside: %s", collision_result_type_names[collision_result_type]);
			//
			//		color = global_real_argb_red;
			//	}
			//	render_debug_point(true, &debug_point0, 0.1f, color);
			//}
		}
		else if (collision_debug_width <= 0.0f)
		{
			//collision_result collision;
			//
			//// c_stop_watch logic
			//bool collision_test_vector_result = collision_test_vector(collision_test_flags, collision_debug_test_terrain_shader, &debug_point0, &debug_vector0, collision_debug_ignore_object_index, NONE, &collision);
			//// c_stop_watch logic
			//
			//if (collision_test_vector_result)
			//{
			//	c_static_string<128> string("UNKNOWN");
			//	e_collision_result_type collision_result_type = collision.type;
			//	switch (collision_result_type)
			//	{
			//	case _collision_result_structure:
			//	{
			//		// #TODO: implement me
			//	}
			//	break;
			//	case _collision_result_water:
			//	{
			//		// #TODO: implement me
			//	}
			//	break;
			//	case _collision_result_instanced_geometry:
			//	{
			//		// #TODO: implement me
			//	}
			//	break;
			//	case _collision_result_object:
			//	{
			//		// #TODO: implement me
			//	}
			//	break;
			//	default:
			//	{
			//		// #TODO: implement me
			//	}
			//	break;
			//	}
			//
			//	// #TODO: implement me
			//
			//	render_debug_vector(true, &debug_point0, &debug_vector0, collision.__real4, global_real_argb_red);
			//	render_debug_point(true, &collision.position, 0.125f, global_real_argb_red);
			//	render_debug_vector(true, &collision.position, collision.?, 0.25f, global_real_argb_red);
			//}
			//else
			//{
			//	render_debug_line(true, &debug_point0, &collision.position, global_real_argb_green);
			//	render_debug_point(true, &collision.position, 0.125f, global_real_argb_green);
			//}
		}
		else
		{
			// #TODO: implement me
		}
	}
	
	if (collision_debug_spray)
	{
		// #TODO: implement me

		//real_matrix4x3 camera{};
		//c_player_view::get_player_render_camera_orientation(&camera);
		//
		//if (collision_debug_length > 0.0f)
		//{
		//	if (!collision_debug_repeat)
		//	{
		//		// #TODO: implement me
		//	}
		//}
		//else
		//{
		//	// #TODO: implement me
		//
		//	//real_point3d point_1{};
		//	//
		//	//real x = camera.center.x + (camera.matrix.forward.i * 2.0f);
		//	//real y = camera.center.y + (camera.matrix.forward.j * 2.0f);
		//	//real z = camera.center.z + (camera.matrix.forward.k * 2.0f);
		//	//
		//	//real z1 = z - 0.125f;
		//	//real z2 = z + 0.125f;
		//	//
		//	//point_1.z = z1;
		//	//
		//	//if (z2 >= z1)
		//	//{
		//	//	real y1 = y - 0.125f;
		//	//	real y2 = y + 0.125f;
		//	//
		//	//	do
		//	//	{
		//	//		point_1.y = y1;
		//	//		if (y2 >= y1)
		//	//		{
		//	//			real x2 = x + 0.125f;
		//	//			do
		//	//			{
		//	//				point_1.x = x - 0.125f;
		//	//				if (x2 >= (x - 0.125f))
		//	//				{
		//	//					do
		//	//					{
		//	//						real_argb_color const* color = global_real_argb_green;
		//	//						if (collision_test_point(collision_test_flags, &point_1, NONE, NONE))
		//	//							color = global_real_argb_red;
		//	//
		//	//						render_debug_point(true, &point_1, 0.0625f, color);
		//	//
		//	//						point_1.x += 0.0625;
		//	//
		//	//					} while (x2 >= point_1.x);
		//	//
		//	//					y1 = point_1.y;
		//	//
		//	//				}
		//	//
		//	//				y1 = y1 + 0.0625f;
		//	//				point_1.y = y1;
		//	//
		//	//			} while (y2 >= y1);
		//	//
		//	//			z1 = point_1.z;
		//	//		}
		//	//
		//	//		z1 += 0.0625f;
		//	//		point_1.z = z1;
		//	//
		//	//	} while (z2 >= z1);
		//	//}
		//}
	}

	if (collision_debug_features)
	{
		real_argb_color const* features_color = global_real_argb_green;
		real height_absolute = fabsf(collision_debug_height);
		real width_absolute = fabsf(collision_debug_width);

		real_point3d debug_point = collision_debug_point;
		real height = 0.0f;
		if (collision_debug_length > 0.0f)
		{
			debug_point.x += (collision_debug_vector.i * 0.5f);
			debug_point.y += (collision_debug_vector.j * 0.5f);
			debug_point.z += (collision_debug_vector.k * 0.5f) + (height_absolute * 0.5f);
			height = (collision_debug_length * 0.5f) + (height_absolute * 0.5f);
		}
		else
		{
			debug_point.x += collision_debug_vector.i;
			debug_point.y += collision_debug_vector.j;
			debug_point.z += collision_debug_vector.k + (height_absolute * 0.5f);
			height = height_absolute * 0.5f;
		}

		real radius = height + width_absolute;

		collision_feature_list features{};
		if (collision_get_features_in_sphere(collision_test_flags, &debug_point, radius, height_absolute, width_absolute, collision_debug_ignore_object_index, NONE, &features))
		{
			render_debug_collision_features(&features);

			collision_plane plane{};
			if (collision_features_test_point(&features, &debug_point, &plane))
				features_color = global_real_argb_red;
		}

		render_debug_point(true, &debug_point, width_absolute, features_color);
		if (height_absolute > 0.0f)
		{
			vector3d up = *global_up3d;
			up.i *= height_absolute;
			up.j *= height_absolute;
			up.k *= height_absolute;
			render_debug_pill(true, &debug_point, &up, width_absolute, features_color);
		}
		else
		{
			render_debug_sphere(true, &debug_point, width_absolute, features_color);
		}

		if (fabs(radius - width_absolute) > _real_epsilon)
			render_debug_sphere(true, &debug_point, radius, global_real_argb_blue);
	}

	if (collision_debug_lightmaps)
	{
		// #TODO: implement me
	}
	
	if (collision_debug_geometry_sampling)
	{
		// #TODO: implement me
	}
	
	if (collision_debug_phantom_bsp)
	{
		// #TODO: implement me
	}

	if (global_collision_log_enable)
	{
		ASSERT(global_current_collision_user_depth > 1);
		global_current_collision_user_depth--;
	}
}

