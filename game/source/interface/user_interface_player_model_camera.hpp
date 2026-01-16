#pragma once

#include "cseries/cseries.hpp"

struct s_player_model_camera_state
{
	byte __data0[0xC];
	real_point3d position;
	real32 __unknown18;
	real_vector3d forward;
	real_vector3d up;
	real32 field_of_view;
	real32 __unknown38;
};
COMPILE_ASSERT(sizeof(s_player_model_camera_state) == 0x3C);

class c_user_interface_player_model_camera
{
public:
	s_player_model_camera_state m_current_camera_state;
	s_player_model_camera_state m_desired_camera_state;
	real32 __unknown78;
	int32 __unknown7C;
};
COMPILE_ASSERT(sizeof(c_user_interface_player_model_camera) == 0x80);

extern void __cdecl user_interface_player_model_camera_update();

