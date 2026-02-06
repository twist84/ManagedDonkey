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

REFERENCE_DECLARE(0x018BB17C, real32, g_observer_wave_height) = 0.03f;
REFERENCE_DECLARE(0x018BB180, real32, g_camera_speed) = 1.0f;

HOOK_DECLARE(0x00612710, observer_game_tick);
HOOK_DECLARE(0x00613A60, observer_update);
//HOOK_DECLARE(0x006128B0, observer_initialize);

bool g_debug_observer_render = false;

bool observer_meter_display = false;
s_status_line observer_meters[4]{};

void __cdecl observer_adopt_global_update_list()
{
	INVOKE(0x00610940, observer_adopt_global_update_list);
}

void __cdecl observer_apply_camera_effect(int32 user_index)
{
	INVOKE(0x00610A20, observer_apply_camera_effect, user_index);
}

void __cdecl observer_apply_rotational_displacement(const real_vector3d* in_vector, real_vector3d* out_vector1, real_vector3d* out_vector2)
{
	INVOKE(0x00610CF0, observer_apply_rotational_displacement, in_vector, out_vector1, out_vector2);
}

void __cdecl observer_block_for_one_frame()
{
	INVOKE(0x00610E50, observer_adopt_global_update_list);
}

void __cdecl observer_build_result_from_point_and_vectors(s_observer_result* out_result, const real_point3d* position, const real_vector3d* forward, const real_vector3d* up)
{
	INVOKE(0x00610E70, observer_build_result_from_point_and_vectors, out_result, position, forward, up);
}

//.text:00610F00 ; void __cdecl observer_check_penetration(int32, real_point3d*, real_vector3d*, const real_vector3d*, const real_point3d*, real32, int32, int32, real32*, real32, real32)

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

s_collision_test_flags __cdecl observer_collision_flags_get(int32 user_index)
{
	return INVOKE(0x00611620, observer_collision_flags_get, user_index);
}

//.text:00611690 ; void __cdecl observer_collision_get_initial_configuration(int32 user_index, const real_point3d*, const real_vector3d*, real32, real32*, real_vector3d*, real_point3d*, real32)
//.text:00611AA0 ; bool __cdecl observer_collision_in_safe_pill(int32, real_point3d*, real_vector3d*, real32)

bool __cdecl observer_collision_test_differential(int32 user_index, real_point3d* focus_position, real_point3d* camera_point, real_vector3d* differential_basis, real32 differential, int32 primary_ignore_object_index, int32 secondary_ignore_object_index, real32* t, bool ignore_media)
{
	return INVOKE(0x00611B90, observer_collision_test_differential, user_index, focus_position, camera_point, differential_basis, differential, primary_ignore_object_index, secondary_ignore_object_index, t, ignore_media);

	//real_point3d point{};
	//scale_vector3d(vector, scale, (real_vector3d*)&point);
	//
	//return observer_collision_test_differential(user_index, focus_position, &point, primary_ignore_object_index, secondary_ignore_object_index, t);
}

bool __cdecl observer_collision_test_with_t(int32 user_index, real_point3d* p0, real_point3d* p1, int32 primary_ignore_object_index, int32 secondary_ignore_object_index, real32* t, bool ignore_media)
{
	return INVOKE(0x00611C30, observer_collision_test_with_t, user_index, p0, p1, primary_ignore_object_index, secondary_ignore_object_index, t, ignore_media);

	//collision_result collision{};
	//if (!collision_test_line(observer_collision_flags_get(user_index), p0, p1, primary_ignore_object_index, secondary_ignore_object_index, &collision))
	//{
	//	return false;
	//}
	//
	//*t = collision.t;
	//return true;
}

void __cdecl observer_command_clear(s_observer_command* command)
{
	INVOKE(0x00611C90, observer_command_clear, command);

	//csmemset(command, 0, sizeof(s_observer_command));
	//command->forward = *global_forward3d;
	//command->up = *global_up3d;
	//command->field_of_view = observer_suggested_field_of_view();
}

void __cdecl observer_command_get_collision_ignore_objects(int32 user_index, const s_observer_command* command, int32* out_first_ignore_object_index, int32* out_second_ignore_object_index)
{
	INVOKE(0x00611D00, observer_command_get_collision_ignore_objects, user_index, command, out_first_ignore_object_index, out_second_ignore_object_index);

	//s_observer* observer = observer_get(user_index);
	//
	//*out_first_ignore_object_index = NONE;
	//*out_second_ignore_object_index = NONE;
	//
	//if (TEST_BIT(command->flags, 6))
	//{
	//	*out_first_ignore_object_index = observer->collision_ignore_object_a;
	//	*out_second_ignore_object_index = observer->collision_ignore_object_b;
	//}
	//
	//if (TEST_BIT(command->flags, 5))
	//{
	//	observer->collision_ignore_object_a = NONE;
	//	observer->collision_ignore_object_b = NONE;
	//
	//	if (command->number_of_parents_objects > 0)
	//		*out_first_ignore_object_index = observer->collision_ignore_object_a = command->parent_objects[0];
	//
	//	if (command->number_of_parents_objects > 1)
	//		*out_first_ignore_object_index = observer->collision_ignore_object_b = command->parent_objects[1];
	//}
}

bool __cdecl observer_command_has_finished(int32 user_index)
{
	return INVOKE(0x00611DA0, observer_command_has_finished, user_index);
}

void __cdecl observer_compute_result(int32 user_index, s_focus_and_distance* focus_and_distance)
{
	INVOKE(0x00611E10, observer_compute_result, user_index, focus_and_distance);

	if (observer_meter_display && user_index == player_mapping_first_active_output_user())
	{
		s_observer* observer = observer_get(user_index);

		observer_meters[0].text.print("positions: p (%.2f, %.2f, %.2f) f (%.2f, %.2f, %.2f) u (%.2f, %.2f, %.2f), fo (%.2f, %.2f, %.2f)",
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

		observer_meters[1].text.print("velocities: v (%.2f, %.2f, %.2f) r (%.2f, %.2f, %.2f), fo (%.2f, %.2f, %.2f)",
			observer->velocities.n[0],
			observer->velocities.n[1],
			observer->velocities.n[2],
			observer->velocities.n[10],
			observer->velocities.n[11],
			observer->velocities.n[12],
			observer->velocities.n[3],
			observer->velocities.n[4],
			observer->velocities.n[5]);

		observer_meters[2].text.print("accelerations: a (%.2f, %.2f, %.2f) r (%.2f, %.2f, %.2f), fo (%.2f, %.2f, %.2f)",
			observer->accelerations.n[0],
			observer->accelerations.n[1],
			observer->accelerations.n[2],
			observer->accelerations.n[10],
			observer->accelerations.n[11],
			observer->accelerations.n[12],
			observer->accelerations.n[3],
			observer->accelerations.n[4],
			observer->accelerations.n[5]);

		observer_meters[3].text.print("displacements: p (%.2f, %.2f, %.2f) r (%.2f, %.2f, %.2f), fo (%.2f, %.2f, %.2f)",
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

void __cdecl observer_compute_view_offset_matrix(int32 user_index, real_matrix4x3* view_offset_matrix)
{
	INVOKE(0x00612460, observer_compute_view_offset_matrix, user_index, view_offset_matrix);
}

void __cdecl observer_dispose()
{
	INVOKE(0x00612520, observer_dispose);
}

void __cdecl observer_dispose_from_old_map()
{
	INVOKE(0x00612530, observer_dispose_from_old_map);
}

void __cdecl observer_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask)
{
	INVOKE(0x00612540, observer_dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);
}

//.text:00612550 ; void __cdecl observer_find_displacement(const real32* const, const real32* const, s_observer_derivative* derivative)
//.text:006125E0 ; void __cdecl observer_fix_focus_position_to_safe_position(int32, real_point3d*, int32, int32, const real_point3d*)

void __cdecl observer_game_tick()
{
	//INVOKE(0x00612710, observer_game_tick);

	s_observer_globals* observer_globals = observer_globals_get();

	if (observer_globals->bsp_lightmap_block_requested_for_next_tick)
	{
		if (!game_is_multiplayer())
		{
			main_render_start_blocking_frame();
			geometry_cache_block_for_one_frame(_geometry_block_type_bsp_geometry);
			texture_cache_block_for_one_frame(_texture_cache_block_type_unknown1);
		}

		observer_globals->bsp_lightmap_block_requested_for_next_tick = false;
	}

	if (observer_globals->full_block_requested_for_next_tick)
	{
		if (!game_is_multiplayer())
		{
			main_render_start_blocking_frame();
			geometry_cache_block_for_one_frame(_geometry_block_type_all);
			texture_cache_block_for_one_frame(_texture_cache_block_type_unknown0);
		}

		observer_globals->full_block_requested_for_next_tick = false;
	}
}

s_observer* observer_get(int32 user_index)
{
	//return INVOKE(0x00612790, observer_get, user_index);

	ASSERT(VALID_INDEX(user_index, k_number_of_users));

	return &observer_globals_get()->observers[user_index];
}

const s_observer_result* observer_get_camera(int32 user_index)
{
	//return INVOKE(0x006127C0, observer_get_camera, user_index);

	s_observer_result* result = &observer_get(user_index)->result;
	ASSERT(result != nullptr);

	return result;
}

real32 __cdecl observer_get_max_wave_height()
{
	return INVOKE(0x006127F0, observer_get_max_wave_height);

	//return g_observer_wave_height;
}

real32 __cdecl observer_get_near_plane_farthest_distance(real32 horizontal_fov, real32 vertical_fov)
{
	return INVOKE(0x00612800, observer_get_near_plane_farthest_distance, horizontal_fov, vertical_fov);
}

s_observer_globals* observer_globals_get()
{
	//return INVOKE(0x00612890, observer_globals_get);

	ASSERT(g_observer_globals);

	return g_observer_globals;
}

void __cdecl observer_initialize()
{
	INVOKE(0x006128B0, observer_initialize);

	status_lines_initialize(observer_meters, &observer_meter_display, NUMBEROF(observer_meters));
}

void __cdecl observer_initialize_after_load_saved_game(int32 flags)
{
	INVOKE(0x00612970, observer_initialize_after_load_saved_game, flags);
}

void __cdecl observer_initialize_before_saved_game(int32 flags)
{
	INVOKE(0x006129A0, observer_initialize_before_saved_game, flags);
}

void __cdecl observer_initialize_for_new_map()
{
	INVOKE(0x00612A50, observer_initialize_for_new_map);

	//observer_clear_all();
	//
	//observer_gamestate_globals->__unknown0 = 0;
	//observer_gamestate_globals->__unknown4 = NONE;
	//observer_gamestate_globals->__unknown6 = NONE;
	//observer_gamestate_globals->__unknown8 = NONE;
	//observer_gamestate_globals->__unknownA = NONE;
}

void __cdecl observer_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask)
{
	INVOKE(0x00612AA0, observer_initialize_for_new_structure_bsp, activating_structure_bsp_mask);
}

void __cdecl observer_initialize_for_saved_game(int32 flags)
{
	INVOKE(0x00612B30, observer_initialize_for_saved_game, flags);

	//observer_clear_all();
}

void __cdecl observer_obsolete_position(int32 user_index)
{
	INVOKE(0x00612B40, observer_obsolete_position, user_index);

	//observer_clear(observer_get(user_index));
}

void __cdecl observer_pass_time(int32 user_index)
{
	INVOKE(0x00612B70, observer_pass_time, user_index);
}

//.text:00612C10 ; void __cdecl observer_penetration_check_volume(real_point3d*, const real_vector3d*, const real_point3d*, real32, real32)

void __cdecl observer_perform_collision(int32 user_index, const s_focus_and_distance* focus_and_distance, real32 world_seconds_elapsed)
{
	INVOKE(0x00612D60, observer_perform_collision, user_index, focus_and_distance, world_seconds_elapsed);
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

void __cdecl observer_result_set_position(int32 user_index, const real_point3d* position, real32 distance, const real_vector3d* forward)
{
	INVOKE(0x00613460, observer_result_set_position, user_index, position, distance, forward);
}

bool __cdecl observer_result_valid(int32 user_index)
{
	return INVOKE(0x00613630, observer_result_valid, user_index);

	//return observer_get(user_index)->result_valid;
}

void __cdecl observer_rotational_displacement(const real_vector3d* forward0, const real_vector3d* up0, const real_vector3d* forward1, const real_vector3d* up1, real_vector3d* displacement)
{
	INVOKE(0x00613660, observer_rotational_displacement, forward0, up0, forward1, up1, displacement);
}

void __cdecl observer_set_camera(int32 user_index, s_observer_command* command)
{
	INVOKE(0x006136A0, observer_set_camera, user_index, command);
}

real32 __cdecl observer_suggested_field_of_view()
{
	return INVOKE(0x00613730, observer_suggested_field_of_view);

	//if (g_camera_globals.field_of_view != 0.0f)
	//	return g_camera_globals.field_of_view * RAD;
	//
	//return g_camera_fov_radians * RAD;
}

real32 __cdecl observer_suggested_field_of_view_change_time()
{
	return INVOKE(0x00613780, observer_suggested_field_of_view_change_time);

	//return 0.18f;
}

//.text:00613790 ; void __cdecl observer_test_water(const real_point3d*, real_vector3d*, real_point3d*, real32, real32)

const s_observer_result* __cdecl observer_try_and_get_camera(int32 user_index)
{
	return INVOKE(0x00613960, observer_try_and_get_camera, user_index);
}

s_observer_result* __cdecl observer_try_and_get_writable_camera(int32 user_index)
{
	return INVOKE(0x006139C0, observer_try_and_get_writable_camera, user_index);
}

const s_observer_result* __cdecl observer_try_to_get_valid_camera_invalid_location_okay(int32 user_index)
{
	return INVOKE(0x00613A10, observer_try_to_get_valid_camera_invalid_location_okay, user_index);
}

void __cdecl observer_update(real32 world_seconds_elapsed)
{
	//INVOKE(0x00613A60, observer_update, world_seconds_elapsed);

	//collision_log_begin_period(6);

	g_observer_globals->dtime = g_camera_speed * world_seconds_elapsed;
	
	for (int32 user_index = first_output_user(); user_index != NONE; user_index = next_output_user(user_index))
	{
		if (player_mapping_output_user_is_active(user_index))
		{
			s_observer* observer = observer_get(user_index);
			ASSERT(observer->header_signature == OBSERVER_SIGNATURE && observer->trailer_signature == OBSERVER_SIGNATURE);
			ASSERT(!observer->updated_for_frame);

			observer->updated_for_frame = true;
	
			observer_update_command(user_index);
	
			if (g_observer_globals->dtime != 0.0f)
				observer_pass_time(user_index);
	
			s_focus_and_distance focus_and_distance{};
			observer_compute_result(user_index, &focus_and_distance);
			observer_apply_camera_effect(user_index);
			observer_perform_collision(user_index, &focus_and_distance, world_seconds_elapsed);

			ASSERT(observer->header_signature == OBSERVER_SIGNATURE && observer->trailer_signature == OBSERVER_SIGNATURE);
		}
	}
	
	if (!g_observer_globals->first_call)
		observer_post_global_update_list();
	g_observer_globals->first_call = false;

	//collision_log_end_period();
}

void __cdecl observer_update_accelerations(int32 user_index)
{
	INVOKE(0x00613B30, observer_update_accelerations, user_index);
}

void __cdecl observer_update_command(int32 user_index)
{
	INVOKE(0x00613D40, observer_update_command, user_index);
}

void __cdecl observer_update_displacements(int32 user_index)
{
	INVOKE(0x00613FA0, observer_update_displacements, user_index);
}

void __cdecl observer_update_polynomial(int32 user_index)
{
	INVOKE(0x00613FE0, observer_update_polynomial, user_index);
}

void __cdecl observer_update_positions(int32 user_index)
{
	INVOKE(0x00614300, observer_update_positions, user_index);
}

void __cdecl observer_update_velocities(int32 user_index)
{
	INVOKE(0x00614A70, observer_update_velocities, user_index);
}

bool __cdecl observer_valid_camera_command(s_observer_command* command)
{
	return INVOKE(0x00614CB0, observer_valid_camera_command, command);

	//if (command && (!TEST_BIT(command->flags, 0) ||
	//	valid_real_vector3d_axes2(&command->forward, &command->up) &&
	//	valid_world_real_point3d(&command->focus_position) &&
	//	valid_world_real_point3d((real_point3d*)&command->focus_offset) &&
	//	valid_real_vector3d(&command->focus_velocity) &&
	//	valid_focus_distance(command->focus_distance) &&
	//	valid_field_of_view(command->field_of_view) &&
	//	valid_timer(command->timer)));
}

void __cdecl observer_validate_camera_command(s_observer_command* command)
{
	INVOKE(0x00614D60, observer_validate_camera_command, command);
}

bool __cdecl valid_field_of_view(real32 field_of_view)
{
	//return INVOKE(0x00615050, valid_field_of_view, field_of_view);

	return valid_real(field_of_view)
		&& IN_RANGE_INCLUSIVE(field_of_view, k_horizontal_field_of_view_min, k_horizontal_field_of_view_max);
}

bool __cdecl valid_focus_distance(real32 focus_distance)
{
	//return INVOKE(0x00615090, valid_focus_distance, focus_distance);

	return valid_real(focus_distance)
		&& IN_RANGE_INCLUSIVE(focus_distance, 0.0f, k_distance_max);
}

bool __cdecl valid_timer(real32 timer)
{
	//return INVOKE(0x006150D0, valid_timer, timer);

	return valid_real(timer)
		&& IN_RANGE_INCLUSIVE(timer, 0.0f, 3600.0f);
}

bool __cdecl valid_world_real(real32 world_real)
{
	//return INVOKE(0x00615110, valid_world_real, world_real);

	return valid_real(world_real)
		&& IN_RANGE_INCLUSIVE(world_real, -50000.0f, 50000.0f);
}

bool __cdecl valid_world_real_point3d(real_point3d* world_real_point)
{
	//return INVOKE(0x00615150, valid_world_real_point3d, world_real_point);

	return valid_world_real(world_real_point->x)
		&& valid_world_real(world_real_point->y)
		&& valid_world_real(world_real_point->z);
}

void __cdecl debug_render_observer()
{
	if (!g_debug_observer_render)
		return;

	int32 user_index = player_mapping_first_active_output_user();
	if (user_index == NONE)
		return;

	s_observer* observer = observer_get(user_index);

	render_camera camera{};
	render_camera_build(&camera, &observer->result);
	render_view_compute_fullscreen_bounds(&camera);

	render_projection projection{};
	render_camera_build_projection(&camera, nullptr, &projection, 0.0f);

	real_point2d screen_points[4]{};
	screen_points[0].x = static_cast<real32>(camera.render_pixel_bounds.x0);
	screen_points[1].x = static_cast<real32>(camera.render_pixel_bounds.x0);
	screen_points[2].x = static_cast<real32>(camera.render_pixel_bounds.x1);
	screen_points[3].x = static_cast<real32>(camera.render_pixel_bounds.x1);
	screen_points[0].y = static_cast<real32>(camera.render_pixel_bounds.y0);
	screen_points[1].y = static_cast<real32>(camera.render_pixel_bounds.y1);
	screen_points[2].y = static_cast<real32>(camera.render_pixel_bounds.y1);
	screen_points[3].y = static_cast<real32>(camera.render_pixel_bounds.y0);

	real_point3d focus_position{};
	real_point3d frustum_points[4]{};
	real_argb_color frustum_color = *global_real_argb_red;
	for (int32 i = 0; i < NUMBEROF(frustum_points); i++)
	{
		real_vector3d frustum_edge{};
		render_camera_screen_to_world(&camera, &projection, &screen_points[i], &focus_position, &frustum_edge);
		point_from_line3d(&focus_position, &frustum_edge, 1.0f, &frustum_points[i]);
		render_debug_vector(false, &focus_position, &frustum_edge, 1.0f, &frustum_color);
	}
	render_debug_polygon_edges(frustum_points, NUMBEROF(frustum_points), &frustum_color);

	if (observer->is_relative)
		matrix4x3_transform_point(&observer->focus_space, &focus_position, &focus_position);
}

