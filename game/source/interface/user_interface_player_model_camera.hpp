#pragma once

#include "cseries/cseries.hpp"

struct s_player_model_camera_state
{
	byte __data0[0xC];
	real_point3d position;
	real __unknown18;
	vector3d forward;
	vector3d up;
	real field_of_view;
	real __unknown38;
};
static_assert(sizeof(s_player_model_camera_state) == 0x3C);

struct c_user_interface_player_model_camera
{
	s_player_model_camera_state m_current_camera_state;
	s_player_model_camera_state m_desired_camera_state;
	real __unknown78;
	long __unknown7C;
};
static_assert(sizeof(c_user_interface_player_model_camera) == 0x80);

extern void __cdecl user_interface_player_model_camera_update();

