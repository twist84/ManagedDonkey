#include "camera/camera_globals.hpp"

REFERENCE_DECLARE(0x018ECE00, s_camera_globals, g_camera_globals) =
{
	.camera_field_of_view_scale = 80.0f,
	.camera_yaw_scale = 10.0f,
	.camera_pitch_scale = 10.0f,
	.camera_forward_scale = 1.0f,
	.camera_side_scale = 1.0f,
	.camera_up_scale = 1.0f,

	.dead_camera_transition_time = 0.0f,
	.dead_camera_falling_death_transition_time = 0.0f,
	.dead_camera_initial_distance = 0.0f,
	.dead_camera_final_distance = 0.0f,
	.dead_camera_z_offset = 0.0f,
	.dead_camera_maximum_elevation = 0.0f,
	.dead_camera_movement_delay = 0.0f,
	.dead_camera_time_in_seconds = 0.0f,
	.dead_camera_minimum_falling_velocity = 0.0f,

	.flying_camera_maximum_boost_speed = 0.0f,
	.flying_camera_time_to_maximum_boost = 0.0f,
	.flying_camera_boost_transition_function = 0,
	.pad = 0,
	.flying_camera_zoomed_field_of_view = 0.0f,
	.flying_camera_zoomed_look_speed_scale = 0.0f,
	.flying_camera_bounding_sphere_radius = 0.0f,
	.flying_cam_movement_delay = 0.3f,
	.flying_camera_zoom_transition_time = 0.0f,
	.flying_camera_vertical_time_to_max_speed = 0.0f,
	.flying_camera_vertical_transition_function = 0,
	.pad1 = 0,

	.survival_switch_time = 0.0f,

	.orbiting_camera_minimum_distance = 0.0f,
	.orbiting_camera_maximum_distance = 0.0f,
	.orbiting_camera_movement_delay = 0.0f,
	.orbiting_camera_z_offset = 0.0f,
	.orbiting_camera_minimum_elevation = 0.0f,
	.orbiting_camera_maximum_elevation = 0.0f,

	.saved_film_director_max_playback_speed = 0.0f,
	.saved_film_rewind_fade_out_time = 0.0f,
	.saved_film_rewind_fade_in_time = 0.0f,

	.game_director_enter_vehicle_transition_time = 0.0f,
	.game_director_exit_vehicle_transition_time = 0.0f
};
REFERENCE_DECLARE(0x018ECE94, real32, g_director_camera_speed_scale) = 1.0f;

bool g_editor_director_mouse_wheel_speed_enabled = true;

