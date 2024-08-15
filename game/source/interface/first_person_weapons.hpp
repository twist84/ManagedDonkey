#pragma once

#include "animations/animation_interpolation.hpp"
#include "cseries/cseries.hpp"

enum
{
	k_first_person_max_weapons = 2
};

struct s_first_person_orientations
{
	byte __data[0x12C00];
};
static_assert(sizeof(s_first_person_orientations) == 0x12C00);

struct first_person_weapon_data
{
	dword_flags flags;
	byte __data4[0x279C];
};
static_assert(sizeof(first_person_weapon_data) == 0x27A0);

struct first_person_weapon
{
	dword_flags flags;
	long unit_index;

	byte __data8[0x30];

	first_person_weapon_data weapon_data[k_first_person_max_weapons];

	c_interpolator_control __unknown4F78;
	byte __data4F84[0x3C];
	real_matrix4x3 __identity4FC0;
	byte __data4FF4[0xC];
};
static_assert(sizeof(first_person_weapon) == 0x5000);

enum e_output_user_index;

extern void __cdecl first_person_weapon_perspective_changed(e_output_user_index output_user_index);
extern void __cdecl first_person_weapons_update();
extern void __cdecl first_person_weapons_update_camera_estimates();

