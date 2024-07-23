#pragma once

#include "cseries/cseries.hpp"

struct s_first_person_orientations
{
	byte __data[0x12C00];
};
static_assert(sizeof(s_first_person_orientations) == 0x12C00);

struct first_person_weapon
{
	byte __data[0x14000];
};
static_assert(sizeof(first_person_weapon) == 0x14000);

enum e_output_user_index;

extern void __cdecl first_person_weapon_perspective_changed(e_output_user_index output_user_index);
extern void __cdecl first_person_weapons_update();
extern void __cdecl first_person_weapons_update_camera_estimates();

