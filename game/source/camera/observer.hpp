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
	byte __data98[0xC];
	real_point3d physics_pill_position;
	real physics_pill_height;
	real physics_pill_radius;
	byte __dataB8[0x8];
	real __unknownC0[6];
	byte __dataD8[0x14];
};
static_assert(sizeof(s_observer_command) == 0xEC);

struct s_observer_result
{
	real_point3d focus_point;
	s_location location;
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
	real_point3d positions_focus_position;
	vector3d positions_focus_offset;
	real_point2d positions_look_shift;
	real positions_focus_distance;
	real horizontal_field_of_view;
	vector3d positions_forward;
	vector3d positions_up;
	real_matrix4x3 focus_space;
	vector3d velocities_v;
	vector3d velocities_forward;
	byte __data208[0x10];
	vector3d velocities_r;
	vector3d accelerations_a;
	vector3d accelerations_r;
	byte __data244[0x10];
	vector3d accelerations_forward;
	byte __data258[0x16C];
	tag trailer_signature;
};
static_assert(sizeof(s_observer) == 0x3C8);

struct s_observer_globals
{
	real timestep;
	s_observer observers[4];
	byte __dataF24[0x2];
	bool block_for_one_frame;
	byte __dataF27[0x1];
};
static_assert(sizeof(s_observer_globals) == 0xF28);

extern bool g_debug_observer_render;

enum e_output_user_index;

extern s_observer* observer_get(long user_index);
extern s_observer_result const* observer_get_camera(e_output_user_index output_user_index);
extern long __cdecl choose_appropriate_director(e_output_user_index output_user_index);
extern void __cdecl observer_game_tick();
extern s_observer_result const* __cdecl observer_try_and_get_camera(e_output_user_index output_user_index);
extern void __cdecl observer_update(real world_seconds_elapsed);
extern void __cdecl observer_validate_camera_command(s_observer_command* result);
extern void __cdecl debug_render_observer();

