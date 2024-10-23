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

struct first_person_weapon_attachment
{
	byte __data0[0x30];
};
static_assert(sizeof(first_person_weapon_attachment) == 0x30);

struct first_person_weapon_data
{
	dword_flags flags;
	first_person_weapon_attachment attachment;

	byte __data34[0xA0];

	string_id current_state_string;

	byte __dataD4[0xEA];

	word frames_remaining;

	byte __data1C4[0x25D0];
	string_id __string_id2794;

	string_id pending_state_string;

	byte __data[0x4];
};
static_assert(sizeof(first_person_weapon_data) == 0x27A0);

struct first_person_weapon
{
	dword_flags flags;
	long unit_index;

	first_person_weapon_attachment attachment;
	first_person_weapon_data weapon_slots[k_first_person_max_weapons];

	c_interpolator_control __unknown4F78;
	byte __data4F84[0x3C];
	real_matrix4x3 __identity4FC0;
	dword __unknown4FF4;
	byte __data4FF8[0x8];
};
static_assert(sizeof(first_person_weapon) == 0x5000);

extern bool& debug_animation_fp_sprint_disable;

enum e_output_user_index;

extern first_person_weapon_data* __cdecl first_person_weapon_get_weapon_data(first_person_weapon* fp_weapon, long weapon_slot);
extern void __cdecl first_person_weapon_perspective_changed(e_output_user_index output_user_index);
extern void __cdecl first_person_weapons_update();
extern void __cdecl first_person_weapons_update_camera_estimates();

extern first_person_weapon* first_person_weapon_get(e_output_user_index output_user_index);
extern long first_person_weapon_get_current_state_string(long unit_index, long weapon_slot);
extern long first_person_weapon_get_pending_state_string(long unit_index, long weapon_slot);

