#pragma once

#include "animations/animation_interpolation.hpp"
#include "animations/animation_manager.hpp"
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
	long unit_index;
	long __unknown4;
	long weapon_index;
	long __unknownC;
	long weapon_render_model_definition_index;
	long __unknown14;
	long hands_render_model_definition_index;
	long __unknown1C;
	long __unknown20;
	long __unknown24;
	long weapon_node_count;
	long hands_node_count;
};
static_assert(sizeof(first_person_weapon_attachment) == 0x30);

struct first_person_weapon_data
{
	// 0: _weapon_attached_bit
	// 1: _weapon_visibility_bit

	dword_flags flags;
	first_person_weapon_attachment attachment;
	c_animation_manager animation_manager;

	byte __dataF4[0xC0];

	real __unknown1B4;
	real __unknown1B8;
	short __unknown1BC;
	short __unknown1BE;
	short __unknown1C0;
	word frames_remaining;

	long weapon_node_remapping_table_count;
	long hands_node_remapping_table_count;
	long weapon_node_remapping_table[150];
	long hands_node_remapping_table[150];

	long __unknown67C[150];

	short __unknown8D4;
	short __unknown8D8;
	long node_orientations_count;
	long __unknown4DC;

	real_matrix4x3 node_matrices[150];
	long camera_control_node;
	real_matrix4x3 camera_offset_matrix;

	long __unknown2790;
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
	byte __data4F84[0xC];
	real __unknown4F90;
	real __unknown4F94;
	real __unknown4F98;
	real __unknown4F9C;
	real __unknown4FA0;
	real __unknown4FA4;
	real __unknown4FA8;
	real __unknown4FAC;
	euler_angles2d __angles4FB0;
	euler_angles2d __angles4FB8;
	real_matrix4x3 camera_offset_matrix_estimate;
	dword __unknown4FF4;
	byte __data4FF8[0x8];
};
static_assert(sizeof(first_person_weapon) == 0x5000);

extern bool& debug_animation_fp_sprint_disable;

enum e_output_user_index;

extern first_person_weapon* __cdecl first_person_weapon_get(e_output_user_index output_user_index);
extern first_person_weapon_data* __cdecl first_person_weapon_get_weapon_data(first_person_weapon* fp_weapon, long weapon_slot);
extern void __cdecl first_person_weapon_perspective_changed(e_output_user_index output_user_index);
extern void __cdecl first_person_weapons_update();
extern void __cdecl first_person_weapons_update_camera_estimates();

extern long first_person_weapon_get_current_state_string(long unit_index, long weapon_slot);
extern long first_person_weapon_get_pending_state_string(long unit_index, long weapon_slot);

