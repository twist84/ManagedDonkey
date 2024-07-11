#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

#define OBSERVER_SIGNATURE 'rad!'

struct s_focus_and_distance
{
	real_point3d focus;
	real distance;
};
static_assert(sizeof(s_focus_and_distance) == 0x10);

struct s_observer_command
{
	dword_flags flags;
	union
	{
		struct
		{
			real_point3d focus_position;
			vector3d focus_offset;
			real_point2d crosshair_location;
			real focus_distance;
			real field_of_view;
			vector3d forward;
			vector3d up;
		};

		real parameters[16];
	};

	vector3d focus_velocity;
	real_matrix4x3 focus_space;

	dword __unknown84;

	real_point3d center;
	real timer;

	long parent_objects[2];
	long number_of_parents_objects;

	real_point3d physics_pill_position;
	real physics_pill_height;
	real physics_pill_radius;

	union
	{
		struct
		{
			byte position_flags;
			byte focus_offset_flags;
			byte look_shift_flags;
			byte distance_flags;
			byte field_of_view_flags;
			byte orientation_flags;
		};
		byte parameter_flags[6];
	};

	union
	{
		struct
		{
			real position_timer;
			real focus_offset_timer;
			real look_shift_timer;
			real distance_timer;
			real field_of_view_timer;
			real orientation_timer;
		};
		real parameter_timers[6];
	};

	byte __dataD8[0x14];
};
static_assert(sizeof(s_observer_command) == 0xEC);
static_assert(0x84 == OFFSETOF(s_observer_command, __unknown84));
static_assert(0xB8 == OFFSETOF(s_observer_command, parameter_flags));
static_assert(0xC0 == OFFSETOF(s_observer_command, parameter_timers));
static_assert(0xD8 == OFFSETOF(s_observer_command, __dataD8));

struct s_observer_result
{
	real_point3d focus_point;
	s_location location;
	byte __dataE[2];
	vector3d __vector10;
	vector3d __vector1C;
	vector3d forward;
	vector3d up;
	real horizontal_field_of_view;
	byte __data44[0x20];
	real __unknown64;
	real vertical_field_of_view;
	real __unknown6C;
};
static_assert(sizeof(s_observer_result) == 0x70);

struct observer_derivative
{
	real n[13];
};

struct s_observer
{
	tag header_signature;
	s_observer_command* pending_command;
	s_observer_command last_command;
	bool updated_for_frame;
	bool __unknownF5;
	bool __unknownF6;
	bool __unknownF7;
	real __unknownF8;
	dword __unknownFC;
	long __unknown100;
	long __unknown104;
	bool result_valid;
	byte __unknown109;
	byte __unknown10A;
	byte __unknown10B;
	s_observer_result result;

	union
	{
		struct
		{
			real_point3d focus_position;
			vector3d focus_offset;
			real_point2d look_shift;
			real focus_distance;
			real field_of_view;
			vector3d forward;
			vector3d up;
		};
		real positions[16];
	};

	real_matrix4x3 focus_space;

	observer_derivative velocities;
	observer_derivative accelerations;
	real a[13];
	real b[13];
	real c[13];
	real d[13];
	real e[13];
	real f[13];
	observer_derivative displacements;

	tag trailer_signature;
};
static_assert(sizeof(s_observer) == 0x3C8);

struct s_observer_globals
{
	real timestep;
	s_observer observers[4];
	bool __unknownF24;
	bool block_for_one_frame_block_type1;
	bool block_for_one_frame_block_type0;
	byte __dataF27[0x1];
};
static_assert(sizeof(s_observer_globals) == 0xF28);

extern real& g_observer_wave_height;
extern real& g_camera_speed;

extern bool g_debug_observer_render;

enum e_output_user_index;
struct s_collision_test_flags;

extern void __cdecl observer_adopt_global_update_list();
extern void __cdecl observer_apply_camera_effect(e_output_user_index user_index);
extern void __cdecl observer_apply_rotational_displacement(vector3d const* in_vector, vector3d* out_vector1, vector3d* out_vector2);
extern void __cdecl observer_block_for_one_frame();
extern void __cdecl observer_build_result_from_point_and_vectors(s_observer_result* out_result, real_point3d const* position, vector3d const* forward, vector3d const* up);
extern void __cdecl observer_clear(s_observer* observer);
extern void __cdecl observer_clear_all();
extern void __cdecl observer_clear_global_update_list();
extern s_collision_test_flags __cdecl observer_collision_flags_get(e_output_user_index output_user_index);
extern bool __cdecl sub_611B90(e_output_user_index output_user_index, real_point3d* point_a, real_point3d* point_b, vector3d* vector, real scale, long first_ignore_object_index, long second_ignore_object_index, real collision_scale);
extern bool __cdecl sub_611C30(e_output_user_index output_user_index, real_point3d* point_a, real_point3d* point_b, long first_ignore_object_index, long second_ignore_object_index, real collision_scale);
extern void __cdecl observer_command_clear(s_observer_command* command);
extern void __cdecl observer_command_get_collision_ignore_objects(e_output_user_index output_user_index, s_observer_command const* command, long* out_first_ignore_object_index, long* out_second_ignore_object_index);
extern bool __cdecl observer_command_has_finished(e_output_user_index output_user_index);
extern void __cdecl observer_compute_result(e_output_user_index user_index, s_focus_and_distance* focus_and_distance);
extern void __cdecl observer_compute_view_offset_matrix(e_output_user_index output_user_index, real_matrix4x3* view_offset_matrix);
extern void __cdecl observer_dispose();
extern void __cdecl observer_dispose_from_old_map();
extern void __cdecl observer_dispose_from_old_structure_bsp(dword a1);
extern void __cdecl observer_game_tick();
extern s_observer* __cdecl observer_get(e_output_user_index output_user_index);
extern s_observer_result const* observer_get_camera(e_output_user_index output_user_index);
extern real __cdecl observer_get_max_wave_height();
extern real __cdecl observer_get_near_plane_farthest_distance(real horizontal_fov, real vertical_fov);
extern s_observer_globals* observer_globals_get();
extern void __cdecl observer_initialize();
extern void __cdecl observer_initialize_after_load_saved_game(long flags);
extern void __cdecl observer_initialize_before_saved_game(long flags);
extern void __cdecl observer_initialize_for_new_map();
extern void __cdecl observer_initialize_for_new_structure_bsp(dword a1);
extern void __cdecl observer_initialize_for_saved_game(long flags);
extern void __cdecl observer_obsolete_position(e_output_user_index output_user_index);
extern void __cdecl observer_pass_time(e_output_user_index user_index);
extern void __cdecl observer_perform_collision(e_output_user_index user_index, s_focus_and_distance const* focus_and_distance, real world_seconds_elapsed);
extern void __cdecl observer_post_global_update_list();
extern void __cdecl observer_result_clear(s_observer_result* result);
extern void __cdecl observer_result_compute_parameters(s_observer_result* result);
extern void __cdecl observer_result_set_position(e_output_user_index output_user_index, real_point3d const* position, real distance, vector3d const* forward);
extern bool __cdecl observer_result_valid(e_output_user_index output_user_index);
extern void __cdecl observer_rotational_displacement(vector3d const* forward0, vector3d const* up0, vector3d const* forward1, vector3d const* up1, vector3d* displacement);
extern void __cdecl observer_set_camera(e_output_user_index output_user_index, s_observer_command* command);
extern real __cdecl observer_suggested_field_of_view();
extern real __cdecl observer_suggested_field_of_view_change_time();
extern s_observer_result const* __cdecl observer_try_and_get_camera(e_output_user_index output_user_index);
extern void __cdecl observer_update(real world_seconds_elapsed);
extern void __cdecl observer_update_accelerations(e_output_user_index user_index);
extern void __cdecl observer_update_command(e_output_user_index user_index);
extern void __cdecl observer_update_displacements(e_output_user_index user_index);
extern void __cdecl observer_update_polynomial(e_output_user_index user_index);
extern void __cdecl observer_update_positions(e_output_user_index user_index);
extern void __cdecl observer_update_velocities(e_output_user_index user_index);
extern bool __cdecl observer_valid_camera_command(s_observer_command* command);
extern void __cdecl observer_validate_camera_command(s_observer_command* command);
extern bool __cdecl valid_field_of_view(real field_of_view);
extern bool __cdecl valid_focus_distance(real focus_distance);
extern bool __cdecl valid_timer(real timer);
extern bool __cdecl valid_world_real(real world_real);
extern bool __cdecl valid_world_real_point3d(real_point3d* world_real_point3d);
extern void __cdecl debug_render_observer();

