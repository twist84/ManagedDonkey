#pragma once

#include "cseries/cseries.hpp"
#include "game/players.hpp"
#include "networking/online/online_guide_pc.hpp"
#include "shell/shell.hpp"

// e_button_presets
// e_joystick_presets
// e_look_sensitivity
// e_camera_attached_horizontal
// e_camera_panning
// e_camera_look_sensitivity
// e_camera_flying_movement
// e_camera_flying_thrust
// e_campaign_difficulty_level
// e_player_model_choice

const long k_popup_message_title_count = 1;

struct s_player_training_profile_data
{
	byte __data0[0x20];
};
static_assert(sizeof(s_player_training_profile_data) == 0x20);

struct c_player_profile_interface
{
	dword_flags m_flags;

	byte __data4[8];

	c_enum<e_controller_index, long, _controller_index0, k_number_of_controllers> m_controller_index;

	byte __data10[0x4];

	long m_button_presets;
	long m_joystick_presets;
	long m_look_sensitivity;

	byte __data1C[0x1DC];

	long m_camera_attached_horizontal;
	long m_camera_panning;
	long m_camera_look_sensitivity;
	long m_camera_flying_movement;
	long m_camera_flying_thrust;

	c_enum<e_campaign_difficulty_level, long, _campaign_difficulty_level_easy, k_number_of_campaign_difficulty_levels> m_campaign_difficulty;

	byte __data214[0xBC];

	byte campaign_progression[0x1A0];

	byte __data470[0x2C];

	// these fields are expanded from s_player_appearance
	long player_model_choice;
	s_emblem_info emblem_info;
	byte __dataAF0[0xA];

	wchar_t service_tag[5];
	wchar_t desired_service_tag[5];
	bool service_tag_was_randomly_generated;
	bool service_tag_failed_verification;

	byte __dataB10[0x18];

	c_static_array<long, k_popup_message_title_count> popup_message_indices;  // default: 0
	c_static_array<char, k_popup_message_title_count> popup_message_indices2; // default: 0
	byte __padB2E[3];

	byte __dataB30[0x80];

	bool film_auto_save[3];
	byte __unknownBB3;

	long profile_region;
	long gamer_zone;
	s_player_training_profile_data training_data;
	c_string_verify_task m_string_verify_task;
};
static_assert(sizeof(c_player_profile_interface) == 0x13F8);

