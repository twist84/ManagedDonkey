#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct s_observer_command
{
	dword_flags flags;
	real_point3d position;
	vector3d focus_offset;
	real_point2d crosshair_location;
	real focus_distance;
	real field_of_view;
	vector3d forward;
	vector3d up;
	vector3d velocities;
	real_matrix4x3 focus_space;

	dword __unknown84;

	real_point3d center;
	real timer;

	long parent_objects[2];
	long number_of_parents_objects;

	real_point3d physics_pill_position;
	real physics_pill_height;
	real physics_pill_radius;

	// 0: c_dead_camera::update
	// 1: c_following_camera::update
	// 3: c_dead_camera::update
	// 4: c_first_person_camera::update, c_flying_camera::update
	byte __unknownB8[6];
	//byte __dataBE[0x2];
	real __unknownC0[6];

	byte __dataD8[0x14];
};
static_assert(sizeof(s_observer_command) == 0xEC);
static_assert(0x84 == offsetof(s_observer_command, __unknown84));
static_assert(0xB8 == offsetof(s_observer_command, __unknownB8));
static_assert(0xC0 == offsetof(s_observer_command, __unknownC0));
static_assert(0xD8 == offsetof(s_observer_command, __dataD8));

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

struct s_observer
{
	tag header_signature;
	s_observer_command* pending_command;
	s_observer_command command;
	bool updated_for_frame;
	bool __unknownF5;
	bool __unknownF6;
	bool __unknownF7;
	real __unknownF8;
	byte __dataFC[0x10];
	s_observer_result result;
	real_point3d focus_position;
	vector3d focus_offset;
	real_point2d look_shift;
	real focus_point;
	real horizontal_field_of_view;
	vector3d forward;
	vector3d up;
	real_matrix4x3 focus_space;
	real velocities[13];
	real accelerations[13];
	real __unknown260[13];
	real __unknown294[13];
	real __unknown2C8[13];
	real __unknown2FC[13];
	real __unknown330[13];
	real __unknown364[13];
	real displacements[13];
	tag trailer_signature;
};
static_assert(sizeof(s_observer) == 0x3C8);

struct s_observer_globals
{
	real timestep;
	s_observer observers[4];
	byte __dataF24[0x1];
	bool block_for_one_frame_block_type1;
	bool block_for_one_frame_block_type0;
	byte __dataF27[0x1];
};
static_assert(sizeof(s_observer_globals) == 0xF28);

extern bool g_debug_observer_render;

enum e_output_user_index;

extern void __cdecl observer_game_tick();
extern s_observer* __cdecl observer_get(e_output_user_index output_user_index);
extern s_observer_result const* observer_get_camera(e_output_user_index output_user_index);
extern s_observer_result const* __cdecl observer_try_and_get_camera(e_output_user_index output_user_index);
extern void __cdecl observer_update(real world_seconds_elapsed);
extern void __cdecl observer_validate_camera_command(s_observer_command* result);
extern void __cdecl debug_render_observer();

