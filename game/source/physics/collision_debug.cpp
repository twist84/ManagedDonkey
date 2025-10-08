#include "physics/collision_debug.hpp"

#include "camera/director.hpp"
#include "game/materials.hpp"
#include "game/materials_definitions.hpp"
#include "game/player_mapping.hpp"
#include "interface/interface.hpp"
#include "main/console.hpp"
#include "physics/collision_bsp.hpp"
#include "physics/collision_constants.hpp"
#include "physics/collision_features.hpp"
#include "physics/collision_models.hpp"
#include "physics/collision_references.hpp"
#include "physics/collision_usage.hpp"
#include "physics/collisions.hpp"
#include "profiler/profiler_stopwatch.hpp"
#include "render/render_debug.hpp"
#include "render/render_structure.hpp"
#include "render/views/render_view.hpp"
#include "structures/structure_bsp_definitions.hpp"
#include "text/draw_string.hpp"
#include "memory/module.hpp"

#include <math.h>

bool collision_debug_test_terrain_shader = true;
bool collision_debug_water_proxy = true;
bool collision_debug_flag_structure = true;
bool collision_debug_flag_water = true;
bool collision_debug_flag_instanced_geometry = true;
bool collision_debug_flag_objects = true;
bool collision_debug_flag_front_facing_surfaces = true;
bool collision_debug_flag_ignore_invisible_surfaces = true;
real32 collision_debug_length = 100.0f;
int32 collision_debug_ignore_object_index = NONE;
bool collision_debug_geometry_sampling = false;
bool collision_debug_flag_objects_terminals = false;
bool g_collision_debug_status_lines_render = false;
bool collision_debug = false;
bool collision_debug_spray = false;
bool collision_debug_features = false;
bool collision_debug_phantom_bsp = false;
bool collision_debug_lightmaps = false;
bool collision_debug_flags = false;
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
real32 collision_debug_width = 0.0f;
real_point3d collision_debug_point{};
real32 collision_debug_height = 0.0f;
real_vector3d collision_debug_vector{};
//bool collision_debug_? = false;
s_status_line g_collision_debug_status_lines[10]{};
s_status_line g_collision_debug_lightmap_status_line{};
s_status_line g_collision_debug_lightmap_status_line_red_sh{};
s_status_line g_collision_debug_lightmap_status_line_green_sh{};
s_status_line g_collision_debug_lightmap_status_line_blue_sh{};
//c_debug_lightmap_drawer global_lightmap_drawer{}:

//HOOK_DECLARE(0x005302B0, collision_debug_initialize);
//HOOK_DECLARE(0x005302C0, collision_debug_dispose);

void __cdecl collision_debug_initialize()
{
	status_lines_initialize(g_collision_debug_status_lines, &g_collision_debug_status_lines_render, NUMBEROF(g_collision_debug_status_lines));
	status_lines_set_flags(g_collision_debug_status_lines, _status_line_left_justify_bit, true, NUMBEROF(g_collision_debug_status_lines));

	//status_lines_initialize(&g_collision_debug_lightmap_status_line, &collision_debug_lightmaps, 1);
	//status_lines_set_flags(&g_collision_debug_lightmap_status_line, _status_line_left_justify_bit, true, 1);
	//status_lines_initialize(&g_collision_debug_lightmap_status_line_red_sh, &collision_debug_lightmaps, 1);
	//status_lines_set_flags(&g_collision_debug_lightmap_status_line_red_sh, _status_line_left_justify_bit, true, 1);
	//status_lines_initialize(&g_collision_debug_lightmap_status_line_green_sh, &collision_debug_lightmaps, 1);
	//status_lines_set_flags(&g_collision_debug_lightmap_status_line_green_sh, _status_line_left_justify_bit, true, 1);
	//status_lines_initialize(&g_collision_debug_lightmap_status_line_blue_sh, &collision_debug_lightmaps, 1);
	//status_lines_set_flags(&g_collision_debug_lightmap_status_line_blue_sh, _status_line_left_justify_bit, true, 1);
}

void __cdecl collision_debug_dispose()
{
}

void collision_debug_render()
{
	if (global_collision_log_enable)
	{
		ASSERT(VALID_INDEX(global_current_collision_user_depth, MAXIMUM_COLLISION_USER_STACK_DEPTH));
		global_current_collision_users[global_current_collision_user_depth++] = 25; // debug
	}

	g_collision_debug_status_lines_render = false;

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
		collision_test_flags.collision_flags.set(_collision_test_media_bit, collision_debug_flag_water);
		collision_test_flags.collision_flags.set(_collision_test_instanced_geometry_bit, collision_debug_flag_instanced_geometry);

		collision_test_flags.object_flags.set(_collision_test_objects_bit, collision_debug_flag_objects);
		collision_test_flags.object_flags.set(_collision_test_objects_bipeds_bit, collision_debug_flag_objects_bipeds);
		collision_test_flags.object_flags.set(_collision_test_objects_giants_bit, collision_debug_flag_objects_giants);
		collision_test_flags.object_flags.set(_collision_test_objects_vehicles_bit, collision_debug_flag_objects_vehicles);
		collision_test_flags.object_flags.set(_collision_test_objects_weapons_bit, collision_debug_flag_objects_weapons);
		collision_test_flags.object_flags.set(_collision_test_objects_equipment_bit, collision_debug_flag_objects_equipment);
		collision_test_flags.object_flags.set(_collision_test_objects_terminals_bit, collision_debug_flag_objects_terminals);
		collision_test_flags.object_flags.set(_collision_test_objects_projectiles_bit, collision_debug_flag_objects_projectiles);
		collision_test_flags.object_flags.set(_collision_test_objects_scenery_bit, collision_debug_flag_objects_scenery);
		collision_test_flags.object_flags.set(_collision_test_objects_machines_bit, collision_debug_flag_objects_machines);
		collision_test_flags.object_flags.set(_collision_test_objects_controls_bit, collision_debug_flag_objects_controls);
		collision_test_flags.object_flags.set(_collision_test_objects_sound_scenery_bit, collision_debug_flag_objects_sound_scenery);
		collision_test_flags.object_flags.set(_collision_test_objects_crates_bit, collision_debug_flag_objects_crates);
		collision_test_flags.object_flags.set(_collision_test_objects_creatures_bit, collision_debug_flag_objects_creatures);
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
			!collision_test_flags.collision_flags.test(_collision_test_media_bit) &&
			!collision_test_flags.collision_flags.test(_collision_test_instanced_geometry_bit) &&
			!collision_test_flags.object_flags.test(_collision_test_objects_bit))
		{
			collision_test_flags.collision_flags.set(_collision_test_structure_bit, true);
		}
	}

	if (!collision_debug_repeat)
	{
		collision_debug_ignore_object_index = NONE;

		int32 user_index = c_player_view::get_current()->get_player_view_user_index();
		if (user_index != NONE && !director_get_perspective(user_index))
			collision_debug_ignore_object_index = player_mapping_get_unit_by_output_user(user_index);

		real_matrix4x3 camera{};
		c_player_view::get_player_render_camera_orientation(&camera);

		collision_debug_point = camera.position;
		collision_debug_vector = camera.forward;
	}

	real_point3d debug_point = collision_debug_point;
	real_vector3d debug_vector = collision_debug_vector;

	real_vector3d debug_vector_scaled{};
	scale_vector3d(&debug_vector, fabsf(collision_debug_length), &debug_vector_scaled);

	if (collision_debug)
	{
		if (collision_debug_length <= 0.0f)
		{
			add_vectors3d((real_vector3d*)&debug_point, &debug_vector_scaled, (real_vector3d*)&debug_point);

			if (collision_debug_width > 0.0f)
			{
				if (collision_test_sphere(collision_test_flags, &debug_point, collision_debug_width, collision_debug_ignore_object_index, NONE))
					render_debug_sphere(true, &debug_point, collision_debug_width, global_real_argb_red);
				else
					render_debug_sphere(true, &debug_point, collision_debug_width, global_real_argb_green);
			}
			else
			{
				const real_argb_color* color = global_real_argb_green;
			
				e_collision_result_type collision_result_type{};
				if (collision_test_point(collision_test_flags, &debug_point, collision_debug_ignore_object_index, NONE, &collision_result_type))
				{
					const char* const collision_result_type_names[k_collision_result_type_count]
					{
						"none",
						"structure",
						"water",
						"instanced_geometry",
						"object"
					};
			
					ASSERT(VALID_INDEX(collision_result_type, k_collision_result_type_count));
			
					g_collision_debug_status_lines_render = true;
					g_collision_debug_status_lines[0].text.print("inside: %s", collision_result_type_names[collision_result_type]);
			
					color = global_real_argb_red;
				}
				render_debug_point(true, &debug_point, 0.1f, color);
			}
		}
		else if (collision_debug_width <= 0.0f)
		{
			int32 seam_material_index = NONE;
			collision_result collision;
			
			c_stop_watch stop_watch{};
			stop_watch.reset();
			stop_watch.stop();
			stop_watch.start();
			bool collision_test_vector_result = collision_test_vector(collision_test_flags, collision_debug_test_terrain_shader, &debug_point, &debug_vector_scaled, collision_debug_ignore_object_index, NONE, NONE, &collision);
			g_collision_debug_status_lines[9].text.print("time %.6f", 1000.0f * c_stop_watch::cycles_to_seconds(stop_watch.stop()));
			
			if (collision_test_vector_result)
			{
				c_static_string<128> shader_name("UNKNOWN");
				const real_matrix4x3* matrix = NULL;
		
				e_collision_result_type collision_result_type = collision.type;
				switch (collision_result_type)
				{
				case _collision_result_structure:
				{
					c_collision_surface_reference surface_reference(collision.collision_bsp_reference, collision.surface_index);
					int32 material_index = surface_reference.get_material_index();
					if (material_index != NONE)
					{
						structure_bsp* structure = global_structure_bsp_get(collision.structure_bsp_index);
						structure_collision_material& collision_material = structure->collision_materials[material_index];
						if (collision_material.seam_mapping_index != NONE)
							seam_material_index = collision_material.seam_mapping_index;
		
						int32 render_method_index = collision_material.render_method.index;
						if (render_method_index != NONE)
							shader_name.set(tag_get_name(render_method_index));
					}
				}
				break;
				case _collision_result_instanced_geometry:
				{
					structure_bsp* structure = global_structure_bsp_get(collision.structure_bsp_index);
					structure_instanced_geometry_instance& instance = structure->instanced_geometry_instances[collision.instanced_geometry_instance_index];
					matrix = &instance.world_transform;
		
					c_collision_surface_reference surface_reference(collision.collision_bsp_reference, collision.surface_index);
					int32 material_index = surface_reference.get_material_index();
					if (material_index != NONE)
					{
						structure_bsp* structure = global_structure_bsp_get(collision.structure_bsp_index);
						structure_collision_material& collision_material = structure->collision_materials[material_index];
		
						int32 render_method_index = collision_material.render_method.index;
						if (render_method_index != NONE)
							shader_name.set(tag_get_name(render_method_index));
					}
				}
				break;
				case _collision_result_object:
				{
					collision_model_instance instance{};
					if (collision_model_instance_new(&instance, collision.object_index))
					{
						int32 bsp_node_index = collision_model_get_bsp_node_index(&instance, collision.bsp_reference);
						matrix = &instance.matrices[bsp_node_index];
		
						c_collision_surface_reference surface_reference(collision.collision_bsp_reference, collision.surface_index);
						int32 material_index = surface_reference.get_material_index();
						if (material_index != NONE)
						{
							collision_model_material& material = instance.collision_model->materials[material_index];
							shader_name.set(material.name.get_string());
						}
					}
				}
				break;
				}
			
				render_debug_vector(true, &debug_point, &debug_vector_scaled, collision.t, global_real_argb_red);
				render_debug_point(true, &collision.point, 0.125f, global_real_argb_red);
				render_debug_vector(true, &collision.point, &collision.plane.n, 0.25f, global_real_argb_red);
			
				if (collision.collision_bsp_reference.valid())
				{
					render_debug_collision_surface(collision.collision_bsp_reference, collision.surface_index, matrix, global_real_argb_red);
				}
			
				if (collision_result_type == _collision_result_structure && TEST_BIT(collision.flags, _collision_surface_conveyor_bit))
				{
					structure_bsp* structure = global_structure_bsp_get(collision.structure_bsp_index);
					structure_collision_material& collision_material = structure->collision_materials[collision.material_index];
					structure_conveyor_surface& conveyor_surface = structure->conveyor_surfaces[collision_material.conveyor_surface_index];
					render_debug_vector(true, &collision.point, &conveyor_surface.u, 1.0f, global_real_argb_green);
					render_debug_vector(true, &collision.point, &conveyor_surface.v, 1.0f, global_real_argb_blue);
				}
			
				g_collision_debug_status_lines_render = true;
			
				switch (collision_result_type)
				{
				case _collision_result_structure:
				{
					g_collision_debug_status_lines[0].text.print("structure leaf #%d", collision.leaf_index);
				}
				break;
				case _collision_result_water:
				{
					g_collision_debug_status_lines[0].text.print("water plane");
				}
				break;
				case _collision_result_instanced_geometry:
				{
					structure_bsp* structure = global_structure_bsp_get(collision.structure_bsp_index);
					structure_instanced_geometry_instance& instance = structure->instanced_geometry_instances[collision.instanced_geometry_instance_index];
					g_collision_debug_status_lines[0].text.print("instanced geometry #%d ('%s')", collision.instanced_geometry_instance_index, instance.name.get_string());
				}
				break;
				case _collision_result_object:
				{
					g_collision_debug_status_lines[0].text.print("object #%d, node #%d, region #%d, bsp 0x%x", collision.object_index, collision.node_index, collision.region_index, collision.bsp_reference);
				}
				break;
				default:
				{
					g_collision_debug_status_lines[0].text.print("unknown (wtf?)");
				}
				break;
				}
			
				c_global_material_type material_type = collision.global_material_type;
				if (collision_debug_water_proxy)
				{
					//material_type = global_material_resolve_underwater_material_proxy(collision.global_material_type, &collision.point);
				}
			
				s_global_material_definition* material = global_get_material(material_type);
				c_static_string<128> seam_material_string("unknown");
			
				if (material)
					seam_material_string.set(material->name.get_string());
			
				if (seam_material_index != NONE)
					seam_material_string.print("seam material %d", seam_material_index);
			
				const char* string1 = "";
			
				g_collision_debug_status_lines[1].text.print("%s%s (shader: %s)",
					seam_material_string.get_string(),
					material_type != collision.global_material_type ? "(underwater proxy)" : "",
					shader_name.get_string());
			
				if (collision.collision_bsp_reference.valid())
				{
					c_collision_surface_reference surface_reference(collision.collision_bsp_reference, collision.surface_index);
			
					g_collision_debug_status_lines[2].text.print("plane #%d%s",
						surface_reference.get_plane_index(),
						surface_reference.is_plane_negated() ? " negated" : "");

					g_collision_debug_status_lines[3].text.print("surface #%d%s%s%s%s%s",
						collision.surface_index,
						TEST_BIT(collision.flags, _collision_surface_two_sided_bit) ? " two-sided" : "",
						TEST_BIT(collision.flags, _collision_surface_invisible_bit) ? " invisible" : "",
						TEST_BIT(collision.flags, _collision_surface_climbable_bit) ? " climbable" : "",
						TEST_BIT(collision.flags, _collision_surface_breakable_bit) ? " breakable" : "",
						TEST_BIT(collision.flags, _collision_surface_slip_bit) ? " slip" : "");
			
					if (collision.breakable_surface_set_index == 0xFF)
					{
						g_collision_debug_status_lines[8].text.clear();
					}
					else
					{
						g_collision_debug_status_lines[8].text.print("breakable set index %d,%d",
							collision.breakable_surface_set_index,
							collision.breakable_surface_index);
					}
				}
				else
				{
					g_collision_debug_status_lines[2].text.clear();
					g_collision_debug_status_lines[3].text.clear();
				}

				g_collision_debug_status_lines[4].text.print("distance %f",
					collision.t* magnitude3d(&debug_vector_scaled));

				g_collision_debug_status_lines[5].text.print("position %f %f %f",
					collision.point.x,
					collision.point.y,
					collision.point.z);

				g_collision_debug_status_lines[6].text.print("normal %f %f %f",
					collision.plane.n.i,
					collision.plane.n.j,
					collision.plane.n.k);

				real32 x = fminf(1.0f, collision.plane.n.k);
				ASSERT(x >= -1.0f - k_real_epsilon && x <= +1.0f + k_real_epsilon);

				real32 x_angle = fmaxf(x, -1.0);
				if (x_angle >= 1.0f)
					x_angle = 1.0f;

				real32 slope = acosf(x_angle);
				g_collision_debug_status_lines[7].text.print("slope %.0f", slope * RAD);

			}
			else
			{
				render_debug_line(true, &debug_point, &collision.point, global_real_argb_green);
				render_debug_point(true, &collision.point, 0.125f, global_real_argb_green);
			}
		}
		else
		{
			real_point3d old_position = collision_debug_point;
			real_vector3d old_velocity = debug_vector_scaled;

			int16 count = 0;
			real_point3d new_position{};
			real_vector3d new_velocity{};
			collision_plane collisions[16]{};
			if (collision_debug_height > 0.0f)
			{
				count = collision_move_pill(
					collision_test_flags,
					&old_position,
					&old_velocity,
					collision_debug_height,
					collision_debug_width,
					collision_debug_ignore_object_index,
					NONE,
					&new_position,
					&new_velocity,
					14,
					collisions);
			}
			else
			{
				count = collision_move_sphere(
					collision_test_flags,
					&old_position,
					&old_velocity,
					collision_debug_width,
					collision_debug_ignore_object_index,
					NONE,
					&new_position,
					&new_velocity,
					14,
					collisions);
			}

			render_debug_vector(true, &old_position, &old_velocity, 1.0f, global_real_argb_blue);
			ASSERT(count <= 14);

			collisions[0].point = old_position;
			collisions[0].plane = {};
			ASSERT(VALID_INDEX(count, NUMBEROF(collisions)));

			collisions[count + 1].point = new_position;
			collisions[count + 1].plane = {};

			int16 v112 = count + 2;
			int16 v113 = 0;
			if (v112 > 0)
			{
				collision_plane* collision = collisions;
				do
				{
					render_debug_point(true, &collision->point, 0.0625f, global_real_argb_red);
					if (v113 > 0)
						render_debug_line(true, &collisions[v113 - 1].point, &collision->point, global_real_argb_red);
					render_debug_vector(true, &collision->point, &collision->plane.n, 0.125f, global_real_argb_red);
					v113++;
					collision++;

				} while (v113 < v112);
			}
			render_debug_vector(true, &new_position, &new_velocity, 1.0f, global_real_argb_green);
		}
	}

	if (collision_debug_spray)
	{
		// $IMPLEMENT

		//real_matrix4x3 camera{};
		//c_player_view::get_player_render_camera_orientation(&camera);
		//
		//if (collision_debug_length > 0.0f)
		//{
		//	if (!collision_debug_repeat)
		//	{
		//		// $IMPLEMENT
		//	}
		//}
		//else
		//{
		//	// $IMPLEMENT
		//
		//	real_point3d point{};
		//	
		//	real32 x = camera.center.x + (camera.matrix.forward.i * 2.0f);
		//	real32 y = camera.center.y + (camera.matrix.forward.j * 2.0f);
		//	real32 z = camera.center.z + (camera.matrix.forward.k * 2.0f);
		//	
		//	real32 z1 = z - 0.125f;
		//	real32 z2 = z + 0.125f;
		//	
		//	for (point.z = z1; z2 >= z1; z1 += 0.0625f, point.z = z1)
		//	{
		//		real32 y1 = y - 0.125f;
		//		real32 y2 = y + 0.125f;
		//	
		//		for (point.y = y1; y2 >= y1; y1 += 0.0625f, point.y = y1)
		//		{
		//			real32 x1 = x - 0.125f;
		//			real32 x2 = x + 0.125f;
		//	
		//			for (point.x = x1; x2 >= x1; x1 += 0.0625f, point.x += x1)
		//			{
		//				const real_argb_color* color = global_real_argb_green;
		//				if (collision_test_point(collision_test_flags, &point, NONE, NONE))
		//					color = global_real_argb_red;
		//	
		//				render_debug_point(true, &point, 0.0625f, color);
		//			}
		//		}
		//	}
		//}
	}

	if (collision_debug_features)
	{
		const real_argb_color* features_color = global_real_argb_green;
		real32 height_absolute = fabsf(collision_debug_height);
		real32 width_absolute = fabsf(collision_debug_width);

		real_point3d debug_point = collision_debug_point;
		real32 height = 0.0f;
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

		real32 radius = height + width_absolute;

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
			real_vector3d up = *global_up3d;
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
		// $IMPLEMENT
	}

	if (collision_debug_geometry_sampling)
	{
		// $IMPLEMENT
	}

	if (collision_debug_phantom_bsp)
	{
		// $IMPLEMENT
	}

	if (global_collision_log_enable)
	{
		ASSERT(global_current_collision_user_depth > 1);
		global_current_collision_user_depth--;
	}
}

