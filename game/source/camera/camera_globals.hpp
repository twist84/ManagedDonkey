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

	real32 field_of_view; // degrees
	real32 yaw_scale;
	real32 pitch_scale;
	real32 forward_scale;
	real32 side_scale;
	real32 up_scale;


	// DEAD CAMERA

	// time it takes for the camera to move from the initial distance to the final distance
	real32 transition_time; // seconds

	// time it takes for the camera to move to its final position during a falling death
	real32 falling_death_transition_time; // seconds

	// on the first frame after death, this is how far out of the body the camera will be
	real32 initial_distance; // wu

	// how far from the body the camera will settle
	real32 final_distance; // wu

	// how far above the body the camera focuses on
	real32 dead_cam_z_offset; // wu

	// the highest angle the camera can raise to(prevents it from flipping over the vertical axis)
	real32 dead_cam_maximum_elevation; // radians

	// delay in tracking the killer
	real32 dead_cam_movement_delay; // seconds

	// how long the death camera lasts before switching to orbiting camera
	real32 time; // seconds

	// minimum velocity to switch to fell to death behavior(when biped is not actually falling to death)
	real32 dead_camera_minimum_falling_velocity;


	// FLYING CAMERA

	// the scaling factor for the left stick when the left trigger is fully depressed
	real32 maximum_boost_speed;

	// seconds. while pegging boost, time to reach maximum speed
	real32 time_to_maximum_boost;

	c_enum<e_global_transition_function, int16, _global_transition_function_linear, k_global_transition_function_count> boost_function;
	uns8 hoist[2];

	// field of view when zoomed
	real32 zoomed_field_of_view; // degrees

	// scaling factor for look speed when zoomed
	real32 zoomed_look_speed;

	// radius of sphere for collision
	real32 bounding_sphere_radius; // wu

	// how quickly the camera responds to the user's input
	real32 flying_cam_movement_delay; // seconds

	// how long it takes to zoom in or out
	real32 zoom_transition_time; // seconds

	real32 vertical_movement_time_to;
	c_enum<e_global_transition_function, int16, _global_transition_function_linear, k_global_transition_function_count> vertical_movement_function;
	uns8 moist[2];

	// how long it takes in survival mode before switching to flying camera
	real32 survival_switch_time; // seconds


	// ORBITING CAMERA

	real32 minimum_distance; // wu
	real32 maximum_distance; // wu

	// how quickly the camera responds to the user's input
	real32 orbit_cam_movement_delay; // seconds

	// how far above the object's root node to position the camera's focus point
	real32 orbit_cam_z_offset; // wu

	// lowest angle the camera can be moved to
	real32 orbit_cam_minimum_elevation; // radians

	// highest angle the camera can be moved to
	real32 orbit_cam_maximum_elevation; // radians


	// SAVED FILMS

	// how fast the film plays when the trigger is fully depressed
	real32 max_playback_speed;

	// how long it takes for the screen to fade out when rewinding
	real32 fade_out_time; // seconds

	// see above
	real32 fade_in_time; // seconds


	// IN GAME

	// how long it takes the camera to move from first to third person when entering a vehicle
	real32 enter_vehicle_transition_time; // seconds

	// see above
	real32 exit_vehicle_transition_time; // seconds
};
static_assert(sizeof(s_camera_globals) == 0x94);

extern s_camera_globals& g_camera_globals;
extern real32& g_director_camera_speed_scale;
extern bool g_editor_director_mouse_wheel_speed_enabled;

