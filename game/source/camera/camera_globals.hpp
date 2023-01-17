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

	real field_of_view; // degrees
	real yaw_scale;
	real pitch_scale;
	real forward_scale;
	real side_scale;
	real up_scale;


	// DEAD CAMERA

	// time it takes for the camera to move from the initial distance to the final distance
	real transition_time; // seconds

	// time it takes for the camera to move to its final position during a falling death
	real falling_death_transition_time; // seconds

	// on the first frame after death, this is how far out of the body the camera will be
	real initial_distance; // wu

	// how far from the body the camera will settle
	real final_distance; // wu

	// how far above the body the camera focuses on
	real dead_cam_z_offset; // wu

	// the highest angle the camera can raise to(prevents it from flipping over the vertical axis)
	real dead_cam_maximum_elevation; // radians

	// delay in tracking the killer
	real dead_cam_movement_delay; // seconds

	// how long the death camera lasts before switching to orbiting camera
	real time; // seconds

	// minimum velocity to switch to fell to death behavior(when biped is not actually falling to death)
	real dead_camera_minimum_falling_velocity;


	// FLYING CAMERA

	// the scaling factor for the left stick when the left trigger is fully depressed
	real maximum_boost_speed;

	// seconds. while pegging boost, time to reach maximum speed
	real time_to_maximum_boost;

	c_enum<e_global_transition_function, short, k_global_transition_function_count> boost_function;
	byte hoist[2];

	// field of view when zoomed
	real zoomed_field_of_view; // degrees

	// scaling factor for look speed when zoomed
	real zoomed_look_speed;

	// radius of sphere for collision
	real bounding_sphere_radius; // wu

	// how quickly the camera responds to the user's input
	real flying_cam_movement_delay; // seconds

	// how long it takes to zoom in or out
	real zoom_transition_time; // seconds

	real vertical_movement_time_to;
	c_enum<e_global_transition_function, short, k_global_transition_function_count> vertical_movement_function;
	byte moist[2];

	// how long it takes in survival mode before switching to flying camera
	real survival_switch_time; // seconds


	// ORBITING CAMERA

	real minimum_distance; // wu
	real maximum_distance; // wu

	// how quickly the camera responds to the user's input
	real orbit_cam_movement_delay; // seconds

	// how far above the object's root node to position the camera's focus point
	real orbit_cam_z_offset; // wu

	// lowest angle the camera can be moved to
	real orbit_cam_minimum_elevation; // radians

	// highest angle the camera can be moved to
	real orbit_cam_maximum_elevation; // radians


	// SAVED FILMS

	// how fast the film plays when the trigger is fully depressed
	real max_playback_speed;

	// how long it takes for the screen to fade out when rewinding
	real fade_out_time; // seconds

	// see above
	real fade_in_time; // seconds


	// IN GAME

	// how long it takes the camera to move from first to third person when entering a vehicle
	real enter_vehicle_transition_time; // seconds

	// see above
	real exit_vehicle_transition_time; // seconds
};
static_assert(sizeof(s_camera_globals) == 0x94);

extern s_camera_globals& g_camera_globals;
extern real& g_director_camera_speed_scale;

