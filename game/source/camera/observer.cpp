#include "camera/observer.hpp"

#include "cache/pc_geometry_cache.hpp"
#include "cache/pc_texture_cache.hpp"
#include "main/console.hpp"
#include "main/main_render.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "physics/collision_constants.hpp"
#include "render/render_cameras.hpp"
#include "render/render_debug.hpp"

REFERENCE_DECLARE(0x018BB17C, real, g_observer_wave_height);
REFERENCE_DECLARE(0x018BB180, real, g_camera_speed);

HOOK_DECLARE(0x00612710, observer_game_tick);
HOOK_DECLARE(0x00613A60, observer_update);
HOOK_DECLARE(0x006128B0, observer_initialize);

bool g_debug_observer_render = false;

bool observer_meter_display = false;
c_status_line observer_meters[4]{};

void __cdecl observer_adopt_global_update_list()
{
	INVOKE(0x00610940, observer_adopt_global_update_list);
}

void __cdecl observer_apply_camera_effect(e_output_user_index output_user_index)
{
	INVOKE(0x00610A20, observer_apply_camera_effect, output_user_index);
}

void __cdecl observer_apply_rotational_displacement(vector3d const* in_vector, vector3d* out_vector1, vector3d* out_vector2)
{
	INVOKE(0x00610CF0, observer_apply_rotational_displacement, in_vector, out_vector1, out_vector2);
}

void __cdecl observer_block_for_one_frame()
{
	INVOKE(0x00610E50, observer_adopt_global_update_list);
}

void __cdecl observer_build_result_from_point_and_vectors(s_observer_result* out_result, real_point3d const* position, vector3d const* forward, vector3d const* up)
{
	INVOKE(0x00610E70, observer_build_result_from_point_and_vectors, out_result, position, forward, up);
}

//.text:00610F00 ; void __cdecl observer_check_penetration(e_output_user_index, real_point3d*, vector3d*, vector3d const*, real_point3d const*, real, long, long, real*, real, real)

void __cdecl observer_clear(s_observer* observer)
{
	INVOKE(0x00611420, observer_clear, observer);
}

void __cdecl observer_clear_all()
{
	INVOKE(0x00611570, observer_clear_all);
}

void __cdecl observer_clear_global_update_list()
{
	INVOKE(0x006115F0, observer_clear_global_update_list);
}

s_collision_test_flags __cdecl observer_collision_flags_get(e_output_user_index output_user_index)
{
	return INVOKE(0x00611620, observer_collision_flags_get, output_user_index);
}

//.text:00611690 ; void __cdecl observer_collision_get_initial_configuration(e_output_user_index output_user_index, real_point3d const*, vector3d const*, real, real*, vector3d*, real_point3d*, real)
//.text:00611AA0 ; bool __cdecl observer_collision_in_safe_pill(e_output_user_index, real_point3d*, vector3d*, real)

bool __cdecl sub_611B90(e_output_user_index output_user_index, real_point3d* point_a, real_point3d* point_b, vector3d* vector, real scale, long first_ignore_object_index, long second_ignore_object_index, real collision_scale)
{
	return INVOKE(0x00611B90, sub_611B90, output_user_index, point_a, point_b, vector, scale, first_ignore_object_index, second_ignore_object_index, collision_scale);
}

bool __cdecl sub_611C30(e_output_user_index output_user_index, real_point3d* point_a, real_point3d* point_b, long first_ignore_object_index, long second_ignore_object_index, real collision_scale)
{
	return INVOKE(0x00611C30, sub_611C30, output_user_index, point_a, point_b, first_ignore_object_index, second_ignore_object_index, collision_scale);
}

void __cdecl observer_command_clear(s_observer_command* command)
{
	INVOKE(0x00611C90, observer_command_clear, command);

	//csmemset(command, 0, sizeof(s_observer_command));
	//command->forward = *global_forward3d;
	//command->up = *global_up3d;
	//command->field_of_view = observer_suggested_field_of_view();
}

void __cdecl observer_command_get_collision_ignore_objects(e_output_user_index output_user_index, s_observer_command const* command, long* out_first_ignore_object_index, long* out_second_ignore_object_index)
{
	INVOKE(0x00611D00, observer_command_get_collision_ignore_objects, output_user_index, command, out_first_ignore_object_index, out_second_ignore_object_index);

	//s_observer* observer = observer_get(output_user_index);
	//
	//*out_first_ignore_object_index = NONE;
	//*out_second_ignore_object_index = NONE;
	//
	//if (TEST_BIT(command->flags, 6))
	//{
	//	*out_first_ignore_object_index = observer->__unknown100;
	//	*out_second_ignore_object_index = observer->__unknown104;
	//}
	//
	//if (TEST_BIT(command->flags, 5))
	//{
	//	observer->__unknown100 = NONE;
	//	observer->__unknown104 = NONE;
	//
	//	if (command->number_of_parents_objects > 0)
	//		*out_first_ignore_object_index = observer->__unknown100 = command->parent_objects[0];
	//
	//	if (command->number_of_parents_objects > 1)
	//		*out_first_ignore_object_index = observer->__unknown104 = command->parent_objects[1];
	//}
}

bool __cdecl observer_command_has_finished(e_output_user_index output_user_index)
{
	return INVOKE(0x00611DA0, observer_command_has_finished, output_user_index);
}

void __cdecl observer_compute_result(e_output_user_index output_user_index, s_focus_and_distance* focus_and_distance)
{
	INVOKE(0x00611E10, observer_compute_result, output_user_index, focus_and_distance);

	if (observer_meter_display && output_user_index == player_mapping_first_active_output_user())
	{
		s_observer* observer = observer_get(output_user_index);

		status_line_printf(&observer_meters[0], "positions: p (%.2f, %.2f, %.2f) f (%.2f, %.2f, %.2f) u (%.2f, %.2f, %.2f), fo (%.2f, %.2f, %.2f)",
			observer->focus_position.x,
			observer->focus_position.y,
			observer->focus_position.z,
			observer->forward.i,
			observer->forward.j,
			observer->forward.k,
			observer->up.i,
			observer->up.j,
			observer->up.k,
			observer->focus_offset.i,
			observer->focus_offset.j,
			observer->focus_offset.k);

		status_line_printf(&observer_meters[1], "velocities: v (%.2f, %.2f, %.2f) r (%.2f, %.2f, %.2f), fo (%.2f, %.2f, %.2f)",
			observer->velocities.n[0],
			observer->velocities.n[1],
			observer->velocities.n[2],
			observer->velocities.n[10],
			observer->velocities.n[11],
			observer->velocities.n[12],
			observer->velocities.n[3],
			observer->velocities.n[4],
			observer->velocities.n[5]);

		status_line_printf(&observer_meters[2], "accelerations: a (%.2f, %.2f, %.2f) r (%.2f, %.2f, %.2f), fo (%.2f, %.2f, %.2f)",
			observer->accelerations.n[0],
			observer->accelerations.n[1],
			observer->accelerations.n[2],
			observer->accelerations.n[10],
			observer->accelerations.n[11],
			observer->accelerations.n[12],
			observer->accelerations.n[3],
			observer->accelerations.n[4],
			observer->accelerations.n[5]);

		status_line_printf(&observer_meters[3], "displacements: p (%.2f, %.2f, %.2f) r (%.2f, %.2f, %.2f), fo (%.2f, %.2f, %.2f)",
			observer->displacements.n[0],
			observer->displacements.n[1],
			observer->displacements.n[2],
			observer->displacements.n[10],
			observer->displacements.n[11],
			observer->displacements.n[12],
			observer->displacements.n[3],
			observer->displacements.n[4],
			observer->displacements.n[5]);

	}
}

void __cdecl observer_compute_view_offset_matrix(e_output_user_index output_user_index, real_matrix4x3* view_offset_matrix)
{
	INVOKE(0x00612460, observer_compute_view_offset_matrix, output_user_index, view_offset_matrix);
}

void __cdecl observer_dispose()
{
	INVOKE(0x00612520, observer_dispose);
}

void __cdecl observer_dispose_from_old_map()
{
	INVOKE(0x00612530, observer_dispose_from_old_map);
}

void __cdecl observer_dispose_from_old_structure_bsp(dword a1)
{
	INVOKE(0x00612540, observer_dispose_from_old_structure_bsp, a1);
}

//.text:00612550 ; void __cdecl observer_find_displacement(real const* const, real const* const, s_observer_derivative* derivative)
//.text:006125E0 ; void __cdecl observer_fix_focus_position_to_safe_position(e_output_user_index, real_point3d*, long, long, real_point3d const*)

void __cdecl observer_game_tick()
{
	//INVOKE(0x00612710, observer_game_tick);

	s_observer_globals* observer_globals = observer_globals_get();

	if (observer_globals->block_for_one_frame_block_type1)
	{
		if (!game_is_multiplayer())
		{
			main_render_start_blocking_frame();
			geometry_cache_block_for_one_frame(_geometry_block_type_unknown1);
			texture_cache_block_for_one_frame(_texture_cache_block_type_unknown1);
		}

		observer_globals->block_for_one_frame_block_type1 = false;
	}

	if (observer_globals->block_for_one_frame_block_type0)
	{
		if (!game_is_multiplayer())
		{
			main_render_start_blocking_frame();
			geometry_cache_block_for_one_frame(_geometry_block_type_unknown0);
			texture_cache_block_for_one_frame(_texture_cache_block_type_unknown0);
		}

		observer_globals->block_for_one_frame_block_type0 = false;
	}
}

s_observer* observer_get(e_output_user_index output_user_index)
{
	//return INVOKE(0x00612790, observer_get, output_user_index);

	ASSERT(VALID_INDEX(output_user_index, k_number_of_output_users));

	return &observer_globals_get()->observers[output_user_index];
}

s_observer_result const* observer_get_camera(e_output_user_index output_user_index)
{
	//return INVOKE(0x006127C0, observer_get_camera, output_user_index);

	s_observer_result* result = &observer_get(output_user_index)->result;
	ASSERT(result != NULL);

	return result;
}

real __cdecl observer_get_max_wave_height()
{
	return INVOKE(0x006127F0, observer_get_max_wave_height);

	//return g_observer_wave_height;
}

real __cdecl observer_get_near_plane_farthest_distance(real horizontal_fov, real vertical_fov)
{
	return INVOKE(0x00612800, observer_get_near_plane_farthest_distance, horizontal_fov, vertical_fov);
}

s_observer_globals* observer_globals_get()
{
	//return INVOKE(0x00612890, observer_globals_get);

	TLS_DATA_GET_VALUE_REFERENCE(g_observer_globals);
	ASSERT(g_observer_globals);

	return g_observer_globals;
}

void __cdecl observer_initialize()
{
	HOOK_INVOKE(, observer_initialize);

	status_lines_initialize(observer_meters, &observer_meter_display, NUMBEROF(observer_meters));
}

void __cdecl observer_initialize_after_load_saved_game(long flags)
{
	INVOKE(0x00612970, observer_initialize_after_load_saved_game, flags);
}

void __cdecl observer_initialize_before_saved_game(long flags)
{
	INVOKE(0x006129A0, observer_initialize_before_saved_game, flags);
}

void __cdecl observer_initialize_for_new_map()
{
	INVOKE(0x00612A50, observer_initialize_for_new_map);

	//observer_clear_all();
	//
	//TLS_DATA_GET_VALUE_REFERENCE(observer_gamestate_globals);
	//observer_gamestate_globals->__unknown0 = 0;
	//observer_gamestate_globals->__unknown4 = NONE;
	//observer_gamestate_globals->__unknown6 = NONE;
	//observer_gamestate_globals->__unknown8 = NONE;
	//observer_gamestate_globals->__unknownA = NONE;
}

void __cdecl observer_initialize_for_new_structure_bsp(dword a1)
{
	INVOKE(0x00612AA0, observer_initialize_for_new_structure_bsp, a1);
}

void __cdecl observer_initialize_for_saved_game(long flags)
{
	INVOKE(0x00612B30, observer_initialize_for_saved_game, flags);
	
	//observer_clear_all();
}

void __cdecl observer_obsolete_position(e_output_user_index output_user_index)
{
	INVOKE(0x00612B40, observer_obsolete_position, output_user_index);

	//observer_clear(observer_get(output_user_index));
}

void __cdecl observer_pass_time(e_output_user_index output_user_index)
{
	INVOKE(0x00612B70, observer_pass_time, output_user_index);
}

//.text:00612C10 ; void __cdecl observer_penetration_check_volume(real_point3d*, vector3d const*, real_point3d const*, real, real)

void __cdecl observer_perform_collision(e_output_user_index output_user_index, s_focus_and_distance const* focus_and_distance, real world_seconds_elapsed)
{
	INVOKE(0x00612D60, observer_perform_collision, output_user_index, focus_and_distance, world_seconds_elapsed);
}

void __cdecl observer_post_global_update_list()
{
	INVOKE(0x00613040, observer_post_global_update_list);
}

void __cdecl observer_result_clear(s_observer_result* result)
{
	INVOKE(0x00613130, observer_result_clear, result);
}

void __cdecl observer_result_compute_parameters(s_observer_result* result)
{
	INVOKE(0x006131E0, observer_result_compute_parameters, result);
}

void __cdecl observer_result_set_position(e_output_user_index output_user_index, real_point3d const* position, real distance, vector3d const* forward)
{
	INVOKE(0x00613460, observer_result_set_position, output_user_index, position, distance, forward);
}

bool __cdecl observer_result_valid(e_output_user_index output_user_index)
{
	return INVOKE(0x00613630, observer_result_valid, output_user_index);

	//return observer_get(output_user_index)->result_valid;
}

void __cdecl observer_rotational_displacement(vector3d const* forward0, vector3d const* up0, vector3d const* forward1, vector3d const* up1, vector3d* displacement)
{
	INVOKE(0x00613660, observer_rotational_displacement, forward0, up0, forward1, up1, displacement);
}

void __cdecl observer_set_camera(e_output_user_index output_user_index, s_observer_command* command)
{
	INVOKE(0x006136A0, observer_set_camera, output_user_index, command);
}

real __cdecl observer_suggested_field_of_view()
{
	return INVOKE(0x00613730, observer_suggested_field_of_view);

	//if (g_camera_globals.field_of_view != 0.0f)
	//	return g_camera_globals.field_of_view * RAD;
	//
	//return g_camera_fov_radians * RAD;
}

real __cdecl observer_suggested_field_of_view_change_time()
{
	return INVOKE(0x00613780, observer_suggested_field_of_view_change_time);

	//return 0.18f;
}

//.text:00613790 ; void __cdecl observer_test_water(real_point3d const*, vector3d*, real_point3d*, real, real)

s_observer_result const* __cdecl observer_try_and_get_camera(e_output_user_index output_user_index)
{
	return INVOKE(0x00613960, observer_try_and_get_camera, output_user_index);
}

s_observer_result* __cdecl observer_try_and_get_writable_camera(e_output_user_index output_user_index)
{
	return INVOKE(0x006139C0, observer_try_and_get_writable_camera, output_user_index);
}

s_observer_result const* __cdecl observer_try_to_get_valid_camera_invalid_location_okay(e_output_user_index output_user_index)
{
	return INVOKE(0x00613A10, observer_try_to_get_valid_camera_invalid_location_okay, output_user_index);
}

void __cdecl observer_update(real world_seconds_elapsed)
{
	//INVOKE(0x00613A60, observer_update, world_seconds_elapsed);

	TLS_DATA_GET_VALUE_REFERENCE(g_observer_globals);
	
	//collision_log_begin_period(6);

	g_observer_globals->timestep = g_camera_speed * world_seconds_elapsed;
	
	for (e_output_user_index user_index = first_output_user(); user_index != k_output_user_none; user_index = next_output_user(user_index))
	{
		if (player_mapping_output_user_is_active(user_index))
		{
			s_observer* observer = observer_get(user_index);
			ASSERT(observer->header_signature == OBSERVER_SIGNATURE && observer->trailer_signature == OBSERVER_SIGNATURE);
			ASSERT(!observer->updated_for_frame);

			observer->updated_for_frame = true;
	
			observer_update_command(user_index);
	
			if (g_observer_globals->timestep != 0.0f)
				observer_pass_time(user_index);
	
			s_focus_and_distance focus_and_distance{};
			observer_compute_result(user_index, &focus_and_distance);
			observer_apply_camera_effect(user_index);
			observer_perform_collision(user_index, &focus_and_distance, world_seconds_elapsed);

			ASSERT(observer->header_signature == OBSERVER_SIGNATURE && observer->trailer_signature == OBSERVER_SIGNATURE);
		}
	}
	
	if (!g_observer_globals->__unknownF24)
		observer_post_global_update_list();
	g_observer_globals->__unknownF24 = false;

	//collision_log_end_period();
}

void __cdecl observer_update_accelerations(e_output_user_index output_user_index)
{
	INVOKE(0x00613B30, observer_update_accelerations, output_user_index);
}

void __cdecl observer_update_command(e_output_user_index output_user_index)
{
	INVOKE(0x00613D40, observer_update_command, output_user_index);
}

void __cdecl observer_update_displacements(e_output_user_index output_user_index)
{
	INVOKE(0x00613FA0, observer_update_displacements, output_user_index);
}

void __cdecl observer_update_polynomial(e_output_user_index output_user_index)
{
	INVOKE(0x00613FE0, observer_update_polynomial, output_user_index);
}

void __cdecl observer_update_positions(e_output_user_index output_user_index)
{
	INVOKE(0x00614300, observer_update_positions, output_user_index);
}

void __cdecl observer_update_velocities(e_output_user_index output_user_index)
{
	INVOKE(0x00614A70, observer_update_velocities, output_user_index);
}

bool __cdecl observer_valid_camera_command(s_observer_command* command)
{
	return INVOKE(0x00614CB0, observer_valid_camera_command, command);

	//if (command && (!TEST_BIT(command->flags, 0) ||
	//	valid_real_vector3d_axes2(&command->forward, &command->up) &&
	//	valid_world_real_point3d(&command->position) &&
	//	valid_world_real_point3d((real_point3d*)&command->focus_offset) &&
	//	valid_real_vector3d(&command->velocities) &&
	//	valid_focus_distance(command->focus_distance) &&
	//	valid_field_of_view(command->field_of_view) &&
	//	valid_timer(command->timer)));
}

void __cdecl observer_validate_camera_command(s_observer_command* command)
{
	INVOKE(0x00614D60, observer_validate_camera_command, command);
}

bool __cdecl valid_field_of_view(real field_of_view)
{
	return INVOKE(0x00615050, valid_field_of_view, field_of_view);
}

bool __cdecl valid_focus_distance(real focus_distance)
{
	return INVOKE(0x00615090, valid_focus_distance, focus_distance);
}

bool __cdecl valid_timer(real timer)
{
	return INVOKE(0x006150D0, valid_timer, timer);
}

bool __cdecl valid_world_real(real world_real)
{
	return INVOKE(0x00615110, valid_world_real, world_real);
}

bool __cdecl valid_world_real_point3d(real_point3d* world_real_point3d)
{
	return INVOKE(0x00615150, valid_world_real_point3d, world_real_point3d);
}

void __cdecl debug_render_observer()
{
	if (!g_debug_observer_render)
		return;

	e_output_user_index user_index = player_mapping_first_active_output_user();
	if (user_index == k_output_user_none)
		return;

	s_observer* observer = observer_get(user_index);

	render_camera camera{};
	render_camera_build(&camera, &observer->result);
	render_view_compute_fullscreen_bounds(&camera);

	render_projection projection{};
	render_camera_build_projection(&camera, nullptr, &projection, 0.0f);

	real_point2d screen_points[4]{};
	screen_points[0].x = static_cast<real>(camera.render_pixel_bounds.x0);
	screen_points[1].x = static_cast<real>(camera.render_pixel_bounds.x0);
	screen_points[2].x = static_cast<real>(camera.render_pixel_bounds.x1);
	screen_points[3].x = static_cast<real>(camera.render_pixel_bounds.x1);
	screen_points[0].y = static_cast<real>(camera.render_pixel_bounds.y0);
	screen_points[1].y = static_cast<real>(camera.render_pixel_bounds.y1);
	screen_points[2].y = static_cast<real>(camera.render_pixel_bounds.y1);
	screen_points[3].y = static_cast<real>(camera.render_pixel_bounds.y0);

	real_point3d point{};
	real_point3d points[4]{};
	for (long i = 0; i < NUMBEROF(points); i++)
	{
		vector3d vector{};
		render_camera_screen_to_world(&camera, &projection, &screen_points[i], &point, &vector);
		point_from_line3d(&point, &vector, 1.0f, &points[i]);
		render_debug_vector(false, &point, &vector, 1.0f, global_real_argb_red);
	}
	render_debug_polygon_edges(points, NUMBEROF(points), global_real_argb_red);

	if (observer->__unknownF6)
		matrix4x3_transform_point(&observer->focus_space, &point, &point);
}

