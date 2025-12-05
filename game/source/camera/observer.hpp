#pragma once

#include "cseries/location.hpp"
#include "math/real_math.hpp"

#define OBSERVER_SIGNATURE 'rad!'

struct s_focus_and_distance
{
	real_point3d focus;
	real32 distance;
};
static_assert(sizeof(s_focus_and_distance) == 0x10);

struct s_observer_depth_of_field
{
	enum
	{
		_active_bit = 0,
		k_flags_count
	};

	int32 flags;
	real32 near_focal_plane_distance;
	real32 far_focal_plane_distance;
	real32 focal_depth;
	real32 blur_amount;
};
static_assert(sizeof(s_observer_depth_of_field) == 0x14);

struct s_observer_command
{
	uns32 flags;

	union
	{
		struct
		{
			real_point3d focus_position;
			real_vector3d focus_offset;
			real_point2d crosshair_location;
			real32 focus_distance;
			real32 field_of_view;
			real_vector3d forward;
			real_vector3d up;
		};

		real32 parameters[16];
	};

	real_vector3d focus_velocity;
	real_matrix4x3 focus_space;

	int32 relative_space_identifier;

	real_point3d safe_position;
	real32 timer;

	int32 collision_ignore_objects[2];
	int32 collision_ignore_objects_count;

	real_point3d pill_base;
	real32 pill_height;
	real32 pill_width;

	union
	{
		struct
		{
			uns8 position_flags;
			uns8 focus_offset_flags;
			uns8 view_offset_flags;
			uns8 distance_flags;
			uns8 field_of_view_flags;
			uns8 orientation_flags;
		};

		uns8 parameter_flags[6];
	};

	union
	{
		struct
		{
			real32 position_timer;
			real32 focus_offset_timer;
			real32 view_offset_timer;
			real32 distance_timer;
			real32 field_of_view_timer;
			real32 orientation_timer;
		};

		real32 parameter_timers[6];
	};

	s_observer_depth_of_field depth_of_field;
};
static_assert(sizeof(s_observer_command) == 0xEC);

struct s_observer_result
{
	real_point3d position;
	s_location location;
	real_vector3d velocity;
	real_vector3d rotation;
	real_vector3d forward;
	real_vector3d up;
	real32 horizontal_field_of_view;
	s_observer_depth_of_field depth_of_field;
	real32 aspect_ratio;
	real_vector2d view_offset;
	real32 magic_crosshair_offset;
	real32 vertical_field_of_view;
	real32 field_of_view_scale;
};
static_assert(sizeof(s_observer_result) == 0x70);

struct s_observer_derivative
{
	union
	{
		struct
		{
			real_vector3d focus_position;
			real_vector3d focus_offset;
			real_vector2d view_offset;
			real32 focus_distance;
			real32 field_of_view;
			real_vector3d rotation;
		};

		real32 n[13];
	};
};

struct s_observer
{
	int32 header_signature;
	s_observer_command* pending_command;
	s_observer_command last_command;
	bool updated_for_frame;
	bool first_command;
	bool is_relative;
	bool variable_up;
	real32 geometry_anticipation_buffer_scale;
	real32 geometry_anticipation_buffer_scale_velocity;
	int32 collision_ignore_object_a;
	int32 collision_ignore_object_b;
	bool result_valid;
	s_observer_result result;

	union
	{
		struct
		{
			real_point3d focus_position;
			real_vector3d focus_offset;
			real_vector2d view_offset;
			real32 focus_distance;
			real32 horizontal_field_of_view;
			real_vector3d forward;
			real_vector3d up;
		};

		real32 positions[16];
	};

	real_matrix4x3 focus_space;

	s_observer_derivative velocities;
	s_observer_derivative accelerations;
	real32 a[13];
	real32 b[13];
	real32 c[13];
	real32 d[13];
	real32 e[13];
	real32 f[13];
	s_observer_derivative displacements;

	int32 trailer_signature;
};
static_assert(sizeof(s_observer) == 0x3C8);

struct s_observer_globals
{
	real32 dtime;
	s_observer observers[4];
	bool first_call;
	bool bsp_lightmap_block_requested_for_next_tick;
	bool full_block_requested_for_next_tick;
};
static_assert(sizeof(s_observer_globals) == 0xF28);

extern real32& g_observer_wave_height;
extern real32& g_camera_speed;

extern bool g_debug_observer_render;

struct s_collision_test_flags;

extern void __cdecl observer_adopt_global_update_list();
extern void __cdecl observer_apply_camera_effect(int32 user_index);
extern void __cdecl observer_apply_rotational_displacement(const real_vector3d* in_vector, real_vector3d* out_vector1, real_vector3d* out_vector2);
extern void __cdecl observer_block_for_one_frame();
extern void __cdecl observer_build_result_from_point_and_vectors(s_observer_result* out_result, const real_point3d* position, const real_vector3d* forward, const real_vector3d* up);
extern void __cdecl observer_clear(s_observer* observer);
extern void __cdecl observer_clear_all();
extern void __cdecl observer_clear_global_update_list();
extern s_collision_test_flags __cdecl observer_collision_flags_get(int32 user_index);
extern bool __cdecl observer_collision_test_differential(int32 user_index, real_point3d* focus_position, real_point3d* camera_point, real_vector3d* differential_basis, real32 differential, int32 primary_ignore_object_index, int32 secondary_ignore_object_index, real32* t, bool ignore_media);
extern bool __cdecl observer_collision_test_with_t(int32 user_index, real_point3d* p0, real_point3d* p1, int32 primary_ignore_object_index, int32 secondary_ignore_object_index, real32* t, bool ignore_media);
extern void __cdecl observer_command_clear(s_observer_command* command);
extern void __cdecl observer_command_get_collision_ignore_objects(int32 user_index, const s_observer_command* command, int32* out_first_ignore_object_index, int32* out_second_ignore_object_index);
extern bool __cdecl observer_command_has_finished(int32 user_index);
extern void __cdecl observer_compute_result(int32 user_index, s_focus_and_distance* focus_and_distance);
extern void __cdecl observer_compute_view_offset_matrix(int32 user_index, real_matrix4x3* view_offset_matrix);
extern void __cdecl observer_dispose();
extern void __cdecl observer_dispose_from_old_map();
extern void __cdecl observer_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask);
extern void __cdecl observer_game_tick();
extern s_observer* __cdecl observer_get(int32 user_index);
extern const s_observer_result* observer_get_camera(int32 user_index);
extern real32 __cdecl observer_get_max_wave_height();
extern real32 __cdecl observer_get_near_plane_farthest_distance(real32 horizontal_fov, real32 vertical_fov);
extern s_observer_globals* observer_globals_get();
extern void __cdecl observer_initialize();
extern void __cdecl observer_initialize_after_load_saved_game(int32 flags);
extern void __cdecl observer_initialize_before_saved_game(int32 flags);
extern void __cdecl observer_initialize_for_new_map();
extern void __cdecl observer_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);
extern void __cdecl observer_initialize_for_saved_game(int32 flags);
extern void __cdecl observer_obsolete_position(int32 user_index);
extern void __cdecl observer_pass_time(int32 user_index);
extern void __cdecl observer_perform_collision(int32 user_index, const s_focus_and_distance* focus_and_distance, real32 world_seconds_elapsed);
extern void __cdecl observer_post_global_update_list();
extern void __cdecl observer_result_clear(s_observer_result* result);
extern void __cdecl observer_result_compute_parameters(s_observer_result* result);
extern void __cdecl observer_result_set_position(int32 user_index, const real_point3d* position, real32 distance, const real_vector3d* forward);
extern bool __cdecl observer_result_valid(int32 user_index);
extern void __cdecl observer_rotational_displacement(const real_vector3d* forward0, const real_vector3d* up0, const real_vector3d* forward1, const real_vector3d* up1, real_vector3d* displacement);
extern void __cdecl observer_set_camera(int32 user_index, s_observer_command* command);
extern real32 __cdecl observer_suggested_field_of_view();
extern real32 __cdecl observer_suggested_field_of_view_change_time();
extern const s_observer_result* __cdecl observer_try_and_get_camera(int32 user_index);
extern void __cdecl observer_update(real32 world_seconds_elapsed);
extern void __cdecl observer_update_accelerations(int32 user_index);
extern void __cdecl observer_update_command(int32 user_index);
extern void __cdecl observer_update_displacements(int32 user_index);
extern void __cdecl observer_update_polynomial(int32 user_index);
extern void __cdecl observer_update_positions(int32 user_index);
extern void __cdecl observer_update_velocities(int32 user_index);
extern bool __cdecl observer_valid_camera_command(s_observer_command* command);
extern void __cdecl observer_validate_camera_command(s_observer_command* command);
extern bool __cdecl valid_field_of_view(real32 field_of_view);
extern bool __cdecl valid_focus_distance(real32 focus_distance);
extern bool __cdecl valid_timer(real32 timer);
extern bool __cdecl valid_world_real(real32 world_real);
extern bool __cdecl valid_world_real_point3d(real_point3d* world_real_point);
extern void __cdecl debug_render_observer();

