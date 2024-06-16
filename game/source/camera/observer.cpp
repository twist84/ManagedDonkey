#include "camera/observer.hpp"

#include "cache/pc_geometry_cache.hpp"
#include "cache/pc_texture_cache.hpp"
#include "main/main_render.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "render/render_cameras.hpp"
#include "render/render_debug.hpp"

HOOK_DECLARE(0x00612710, observer_game_tick);

bool g_debug_observer_render = false;

//.text:00610940 ; void __cdecl observer_adopt_global_update_list()
//.text:00610A20 ; void __cdecl observer_apply_camera_effect(e_output_user_index)
//.text:00610CF0 ; 
//.text:00610E50 ; void __cdecl observer_block_for_one_frame()
//.text:00610E70 ; void __cdecl observer_build_result_from_point_and_vectors(s_observer_result*, real_point3d const*, vector3d const*, vector3d const*)
//.text:00610F00 ; void __cdecl observer_check_penetration(e_output_user_index, real_point3d*, vector3d*, vector3d const*, real_point3d const*, real, long, long, real*, real, real)
//.text:00611420 ; void __cdecl observer_clear(s_observer*)
//.text:00611570 ; void __cdecl observer_clear_all()
//.text:006115F0 ; void __cdecl observer_clear_global_update_list()
//.text:00611620 ; struct s_collision_test_flags __cdecl observer_collision_flags_get(e_output_user_index)
//.text:00611690 ; void __cdecl observer_collision_get_initial_configuration(e_output_user_index, real_point3d const*, vector3d const*, real, real*, vector3d*, real_point3d*, real)
//.text:00611AA0 ; bool __cdecl observer_collision_in_safe_pill(e_output_user_index, real_point3d*, vector3d*, real)
//.text:00611B90 ; 
//.text:00611C30 ; 
//.text:00611C90 ; void __cdecl observer_command_clear(s_observer_command*)
//.text:00611D00 ; void __cdecl observer_command_get_collision_ignore_objects(e_output_user_index, s_observer_command const*, long*, long*)
//.text:00611DA0 ; 
//.text:00611E10 ; void __cdecl observer_compute_result(e_output_user_index, s_focus_and_distance*)
//.text:00612460 ; void __cdecl observer_compute_view_offset_matrix(e_output_user_index, real_matrix4x3*)
//.text:00612520 ; void __cdecl observer_dispose()
//.text:00612530 ; void __cdecl observer_dispose_from_old_map()
//.text:00612540 ; void __cdecl observer_dispose_from_old_structure_bsp(dword)
//.text:00612550 ; void __cdecl observer_find_displacement(real const* const, real const* const, s_observer_derivative*)
//.text:006125E0 ; void __cdecl observer_fix_focus_position_to_safe_position(e_output_user_index, real_point3d*, long, long, real_point3d const*)

void __cdecl observer_game_tick()
{
	//INVOKE(0x00612710, observer_game_tick);

	TLS_DATA_GET_VALUE_REFERENCE(g_observer_globals);

	if (g_observer_globals->block_for_one_frame_block_type1)
	{
		if (!game_is_multiplayer())
		{
			main_render_start_blocking_frame();
			geometry_cache_block_for_one_frame(_geometry_block_type_unknown1);
			texture_cache_block_for_one_frame(_texture_cache_block_type_unknown1);
		}

		g_observer_globals->block_for_one_frame_block_type1 = false;
	}

	if (g_observer_globals->block_for_one_frame_block_type0)
	{
		if (!game_is_multiplayer())
		{
			main_render_start_blocking_frame();
			geometry_cache_block_for_one_frame(_geometry_block_type_unknown0);
			texture_cache_block_for_one_frame(_texture_cache_block_type_unknown0);
		}

		g_observer_globals->block_for_one_frame_block_type0 = false;
	}
}

s_observer* observer_get(e_output_user_index output_user_index)
{
	//return INVOKE(0x00612790, observer_get, output_user_index);

	ASSERT(VALID_INDEX(output_user_index, k_number_of_output_users));

	TLS_DATA_GET_VALUE_REFERENCE(g_observer_globals);

	return &g_observer_globals->observers[output_user_index];
}

s_observer_result const* observer_get_camera(e_output_user_index output_user_index)
{
	//return INVOKE(0x006127C0, observer_get_camera, output_user_index);

	s_observer_result* result = &observer_get(output_user_index)->result;
	ASSERT(result != NULL);

	return result;
}

//.text:006127F0 ; real __cdecl observer_get_max_wave_height()
//.text:00612800 ; real __cdecl observer_get_near_plane_farthest_distance(real, real)
//.text:006128B0 ; void __cdecl observer_initialize()
//.text:00612970 ; void __cdecl observer_initialize_after_load_saved_game(long)
//.text:006129A0 ; void __cdecl observer_initialize_before_saved_game(long)
//.text:00612A50 ; void __cdecl observer_initialize_for_new_map()
//.text:00612AA0 ; void __cdecl observer_initialize_for_new_structure_bsp(dword)
//.text:00612B30 ; void __cdecl observer_initialize_for_saved_game(long)
//.text:00612B40 ; void __cdecl observer_obsolete_position(e_output_user_index)
//.text:00612B70 ; void __cdecl observer_pass_time()
//.text:00612C10 ; void __cdecl observer_penetration_check_volume(real_point3d*, vector3d const*, real_point3d const*, real, real)
//.text:00612D60 ; void __cdecl observer_perform_collision(e_output_user_index, s_focus_and_distance const*, real)
//.text:00613040 ; void __cdecl observer_post_global_update_list()
//.text:00613130 ; void __cdecl observer_result_clear(s_observer_result*)
//.text:006131E0 ; void __cdecl observer_result_compute_parameters(s_observer_result*)
//.text:00613460 ; void __cdecl observer_result_set_position(e_output_user_index, real_point3d const*, real, vector3d const*)
//.text:00613630 ; bool __cdecl observer_result_valid(e_output_user_index)
//.text:00613660 ; void __cdecl observer_rotational_displacement(vector3d const*, vector3d const*, vector3d const*, vector3d const*, vector3d*)
//.text:006136A0 ; void __cdecl observer_set_camera(e_output_user_index, s_observer_command*)
//.text:00613730 ; real __cdecl observer_suggested_field_of_view()
//.text:00613780 ; real __cdecl observer_suggested_field_of_view_change_time()
//.text:00613790 ; void __cdecl observer_test_water(real_point3d const*, vector3d*, real_point3d*, real, real)

s_observer_result const* __cdecl observer_try_and_get_camera(e_output_user_index output_user_index)
{
	return INVOKE(0x00613960, observer_try_and_get_camera, output_user_index);
}

//.text:006139C0 ; s_observer_result* __cdecl observer_try_and_get_writable_camera(e_output_user_index)
//.text:00613A10 ; s_observer_result const* __cdecl observer_try_to_get_valid_camera_invalid_location_okay(e_output_user_index)

void __cdecl observer_update(real world_seconds_elapsed)
{
	INVOKE(0x00613A60, observer_update, world_seconds_elapsed);
}

//.text:00613B30 ; void __cdecl observer_update_accelerations(e_output_user_index)
//.text:00613D40 ; void __cdecl observer_update_command(e_output_user_index)
//.text:00613FA0 ; void __cdecl observer_update_displacements(e_output_user_index)
//.text:00613FE0 ; void __cdecl observer_update_polynomial(e_output_user_index)
//.text:00614300 ; void __cdecl observer_update_positions(e_output_user_index)
//.text:00614A70 ; void __cdecl observer_update_velocities(e_output_user_index)
//.text:00614CB0 ; bool __cdecl observer_valid_camera_command(s_observer_command*)

void __cdecl observer_validate_camera_command(s_observer_command* result)
{
	INVOKE(0x00614D60, observer_validate_camera_command, result);
}

//.text:00615050 ; bool __cdecl valid_field_of_view(real)
//.text:00615090 ; bool __cdecl valid_focus_distance(real)
//.text:006150D0 ; bool __cdecl valid_timer(real)
//.text:00615110 ; bool __cdecl valid_world_real(real)
//.text:00615150 ; bool __cdecl valid_world_real_point3d(real_point3d*)


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

