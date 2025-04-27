#pragma once

#include "animations/animation_interpolation.hpp"
#include "animations/animation_manager.hpp"
#include "animations/mixing_board/mixing_board_slider.hpp"
#include "cseries/cseries.hpp"

enum
{
	k_first_person_max_weapons = 2
};

struct s_weapon_orientations
{
	real_orientation node_orientations[150];
	real_orientation original_node_orientations[150];
};
static_assert(sizeof(s_weapon_orientations) == 0x2580);

struct s_first_person_orientations
{
	s_weapon_orientations weapon_orientations[4][2];
};
static_assert(sizeof(s_first_person_orientations) == 0x12C00);

struct first_person_weapon_attachment
{
	long unit_index;
	long player_character_type;
	long weapon_index;
	long weapon_class;
	long weapon_render_model_index;
	long weapon_animation_graph_index;
	long hands_render_model_index;
	long arms_render_model_index;
	long body_render_model_index;
	long legs_render_model_index;
	long weapon_node_count;
	long hands_node_count;
};
static_assert(sizeof(first_person_weapon_attachment) == 0x30);

enum e_first_person_weapon_data_flags
{
	_weapon_attached_bit = 0,
	_weapon_visible_bit,
	_weapon_node_table_valid_bit,
	_arm_node_table_valid_bit,
	_first_person_camera_matrix_ready_bit

	// are there more?
};

struct first_person_weapon_data
{
	// e_first_person_weapon_data_flags
	uint32 flags;
	first_person_weapon_attachment attachment;
	c_animation_manager animation_manager;
	c_animation_channel channel1;
	c_animation_channel channel2;
	c_animation_channel channel3;
	c_animation_id pitch_and_turn_id;
	c_animation_id overlays_id;
	c_animation_id ammunition_id;
	real32 firing_push_back;
	real32 firing_push_back_velocity;
	short ticks_idle;
	short ticks_until_pose;
	short ticks_until_predict;
	short ticks_until_automatic_firing_may_stop;
	long weapon_node_remapping_table_count;
	long hands_node_remapping_table_count;
	long weapon_node_remapping_table[150];
	long hands_node_remapping_table[150];
	long hands_matrix_remapping_table[150];
	short weapon_root_node_index;
	short opposite_hand_node_index;
	long node_orientations_count;
	long node_matrices_count;
	real_matrix4x3 node_matrices[150];
	long camera_control_node;
	real_matrix4x3 camera_control_offset_matrix;
	long current_sound_index;
	string_id current_sound_state;
	string_id pending_state;
	bool pending_reset_sounds;
};
static_assert(sizeof(first_person_weapon_data) == 0x27A0);

struct first_person_weapon
{
	uint32 flags;
	long unit_index;
	first_person_weapon_attachment attachment;
	first_person_weapon_data weapon[k_first_person_max_weapons];
	c_interpolator_control overlay_interpolator;
	c_mixing_board_slider ik_slider;
	real_vector2d position;
	real_vector2d position_velocity;
	real_vector2d turning;
	real_vector2d turning_velocity;
	real_euler_angles2d facing_angles;
	real_euler_angles2d facing_angles_delta;
	real_matrix4x3 estimated_root_matrix;
	long custom_animation_graph_index;
	long custom_animation_name;
	c_animation_id custom_animation_id;
};
static_assert(sizeof(first_person_weapon) == 0x5000);

extern bool& debug_animation_fp_sprint_disable;

extern bool debug_first_person_skeleton;

struct render_first_person_model;

extern long __cdecl first_person_weapon_build_models(long user_index, long object_index, long maximum_model_count, render_first_person_model* first_person_model);
extern first_person_weapon* __cdecl first_person_weapon_get(long user_index);
extern first_person_weapon_data* __cdecl first_person_weapon_get_weapon_data(first_person_weapon* fp_weapon, long weapon_slot);
extern void __cdecl first_person_weapon_perspective_changed(long user_index);
extern void __cdecl first_person_weapons_update();
extern void __cdecl first_person_weapons_update_camera_estimates();

extern long first_person_weapon_get_current_state_string(long unit_index, long weapon_slot);
extern long first_person_weapon_get_pending_state_string(long unit_index, long weapon_slot);

