#pragma once

#include "cseries/cseries.hpp"

enum e_global_transition_function
{
	_global_transition_function_linear = 0,
	_global_transition_function_early,
	_global_transition_function_very_early,
	_global_transition_function_late,
	_global_transition_function_very_late,
	_global_transition_function_cosine,
	_global_transition_function_one,
	_global_transition_function_zero,

	k_global_transition_function_count
};

struct s_camera_globals
{
	// CAMERA UNIVERSALS

	real32 camera_field_of_view_scale; // degrees
	real32 camera_yaw_scale;
	real32 camera_pitch_scale;
	real32 camera_forward_scale;
	real32 camera_side_scale;
	real32 camera_up_scale;


	// DEAD CAMERA

	// time it takes for the camera to move from the initial distance to the final distance
	real32 dead_camera_transition_time; // seconds

	// time it takes for the camera to move to its final position during a falling death
	real32 dead_camera_falling_death_transition_time; // seconds

	// on the first frame after death, this is how far out of the body the camera will be
	real32 dead_camera_initial_distance; // wu

	// how far from the body the camera will settle
	real32 dead_camera_final_distance; // wu

	// how far above the body the camera focuses on
	real32 dead_camera_z_offset; // wu

	// the highest angle the camera can raise to(prevents it from flipping over the vertical axis)
	real32 dead_camera_maximum_elevation; // radians

	// delay in tracking the killer
	real32 dead_camera_movement_delay; // seconds

	// how long the death camera lasts before switching to orbiting camera
	real32 dead_camera_time_in_seconds; // seconds

	// minimum velocity to switch to fell to death behavior(when biped is not actually falling to death)
	real32 dead_camera_minimum_falling_velocity;


	// FLYING CAMERA

	// the scaling factor for the left stick when the left trigger is fully depressed
	real32 flying_camera_maximum_boost_speed;

	// seconds. while pegging boost, time to reach maximum speed
	real32 flying_camera_time_to_maximum_boost;
	
	int16 flying_camera_boost_transition_function;
	int16 pad;

	// field of view when zoomed
	real32 flying_camera_zoomed_field_of_view; // degrees

	// scaling factor for look speed when zoomed
	real32 flying_camera_zoomed_look_speed_scale;

	// radius of sphere for collision
	real32 flying_camera_bounding_sphere_radius; // wu

	// how quickly the camera responds to the user's input
	real32 flying_cam_movement_delay; // seconds

	// how long it takes to zoom in or out
	real32 flying_camera_zoom_transition_time; // seconds

	real32 flying_camera_vertical_time_to_max_speed;

	int16 flying_camera_vertical_transition_function;
	int16 pad1;

	// how long it takes in survival mode before switching to flying camera
	real32 survival_switch_time; // seconds


	// ORBITING CAMERA

	real32 orbiting_camera_minimum_distance; // wu
	real32 orbiting_camera_maximum_distance; // wu

	// how quickly the camera responds to the user's input
	real32 orbiting_camera_movement_delay; // seconds

	// how far above the object's root node to position the camera's focus point
	real32 orbiting_camera_z_offset; // wu

	// lowest angle the camera can be moved to
	real32 orbiting_camera_minimum_elevation; // radians

	// highest angle the camera can be moved to
	real32 orbiting_camera_maximum_elevation; // radians


	// SAVED FILMS

	// how fast the film plays when the trigger is fully depressed
	real32 saved_film_director_max_playback_speed;

	// how long it takes for the screen to fade out when rewinding
	real32 saved_film_rewind_fade_out_time; // seconds

	// see above
	real32 saved_film_rewind_fade_in_time; // seconds


	// IN GAME

	// how long it takes the camera to move from first to third person when entering a vehicle
	real32 game_director_enter_vehicle_transition_time; // seconds

	// see above
	real32 game_director_exit_vehicle_transition_time; // seconds
};
COMPILE_ASSERT(sizeof(s_camera_globals) == 0x94);

extern s_camera_globals& g_camera_globals;
extern real32& g_director_camera_speed_scale;
extern bool g_editor_director_mouse_wheel_speed_enabled;

