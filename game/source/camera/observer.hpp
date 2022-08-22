#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct s_observer_command
{
	dword_flags flags;
	real_point3d position;
	real_vector3d focus_offset;
	real_point2d look_shift;
	real focus_distance;
	real field_of_view;
	real_vector3d forward;
	real_vector3d up;
	real_vector3d velocities;
	real_matrix4x3 focus_space;
	dword __unknown84;
	real_point3d center;
	real timer;
	char __data98[40];
	real __unknownC0[6];
	char __dataD8[20];
};
static_assert(sizeof(s_observer_command) == 0xEC);

struct s_observer_result
{
	real_point3d focus_point;
	s_location location;
	float __unknown10[3];
	float __unknown1C[3];
	real_vector3d forward;
	real_vector3d up;
	real horizontal_field_of_view;
	char __data44[36];
	real vertical_field_of_view;
	real __unknown60;
};

struct s_observer
{
	dword header_signature;
	s_observer_command* pending_command;
	s_observer_command command;
	bool updated_for_frame;
	bool __unknownF5;
	bool __unknownF6;
	bool __unknownF7;
	float __unknownF8;
	char __dataFC[16];
	s_observer_result result;
	real_point3d positions_focus_position;
	real_vector3d positions_focus_offset;
	real_point2d positions_look_shift;
	real positions_focus_distance;
	real horizontal_field_of_view;
	real_vector3d positions_forward;
	real_vector3d positions_up;
	real_matrix4x3 focus_space;
	real_vector3d velocities_v;
	real_vector3d velocities_forward;
	char __data208[16];
	real_vector3d velocities_r;
	real_vector3d accelerations_a;
	real_vector3d accelerations_r;
	char __data244[16];
	real_vector3d accelerations_forward;
	char __data258[364];
	unsigned long trailer_signature;
};
static_assert(sizeof(s_observer) == 0x3C8);

struct s_observer_globals
{
	real __unknown0;
	s_observer observers[4];
	char __dataF24[2];
	bool block_for_one_frame;
	char __dataF27[1];
};
static_assert(sizeof(s_observer_globals) == 0xF28);

extern s_observer* observer_get(long user_index);