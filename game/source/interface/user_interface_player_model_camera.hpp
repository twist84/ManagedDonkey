#pragma once

#include "cseries/cseries.hpp"

struct s_player_model_camera_state
{
	uint8 __data0[0xC];
	real_point3d position;
	real32 __unknown18;
	real_vector3d forward;
	real_vector3d up;
	real32 field_of_view;
	real32 __unknown38;
};
static_assert(sizeof(s_player_model_camera_state) == 0x3C);

struct c_user_interface_player_model_camera
{
	s_player_model_camera_state m_current_camera_state;
	s_player_model_camera_state m_desired_camera_state;
	real32 __unknown78;
	int32 __unknown7C;
};
static_assert(sizeof(c_user_interface_player_model_camera) == 0x80);

extern void __cdecl user_interface_player_model_camera_update();

