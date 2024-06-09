#pragma once

#include "cseries/cseries.hpp"
#include "game/game_progression.hpp"
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

struct s_campaign_progression_profile_data
{
	qword __time0;
	long campaign_id;
	long map_id;
	short campaign_insertion_index;
	s_campaign_armaments campaign_armaments;
	byte __pad8A[0x2];
	s_campaign_game_progression campaign_game_progression;
	s_campaign_game_progression hub_progression;
	long campaign_difficulty;
	long campaign_metagame_scoring;
	dword_flags active_primary_skulls;
	dword_flags active_secondary_skulls;
	long __unknown19C;
};
static_assert(sizeof(s_campaign_progression_profile_data) == 0x1A0);

const long k_popup_message_title_count = 1;

struct s_player_training_profile_data
{
	byte __data0[0x20];
};
static_assert(sizeof(s_player_training_profile_data) == 0x20);

struct c_player_profile_interface
{
	// 0: controller_look_inverted
	// 1: controller_flight_stick_aircraft_controls
	// 2: controller_auto_center_look
	// 3: controller_crouch_lock_enabled
	// 4: female_voice_enabled
	dword_flags m_flags;

	long __unknown4;
	long __unknown8;

	c_enum<e_controller_index, long, _controller_index0, k_number_of_controllers> m_controller_index;

	byte __data10[0x4];

	struct // sizeof 0x1E4
	{
		long button_presets;   // 0-5
		long joystick_presets; // 0-3
		long look_sensitivity; // 0-9

		byte __data1C[0x1D8];
	};

	byte __data1F8[0x1];

	bool m_display_hints;

	byte __data1FA[0x2];

	long m_camera_attached_horizontal; // 0-1
	long m_camera_panning;             // 0-1
	long m_camera_look_sensitivity;    // 0-3
	long m_camera_flying_movement;     // 0-3
	long m_camera_flying_thrust;       // 0-2

	struct // sizeof 0xC0
	{
		c_enum<e_campaign_difficulty_level, long, _campaign_difficulty_level_easy, k_number_of_campaign_difficulty_levels> m_campaign_difficulty;

		short __unknown214; // last_campaign_insertion_point?

		short last_campaign_map_played_absolute_index;
		byte_flags map_completed_at_difficulty_level[2 /* e_campaign_game_mode */][32 /* campaign_level_index */];
		qword last_campaign_played_time[2];
		dword_flags last_primary_skulls;
		dword_flags last_secondary_skulls;

		byte __data270[0x4];
		c_static_flags<32> __unknown274; // campaign_level_flags
		byte __data278[0x38];

		long solo_map_id;
		short solo_insertion_point;
		byte __pad2B6[0x2];
		real solo_unknown;
		long coop_map_id;
		short coop_insertion_point;
		byte __pad2C2[0x2];
		real coop_unknown;
		long coop_player_count;

		byte __data2CC[0x4];
	};

	s_campaign_progression_profile_data campaign_progression;

	short __unknown470;
	byte __data472[0x2]; // pad?
	long __unknown474;

	// linked
	long __unknown478;
	long __unknown47C; // 0-10

	long __unknown480; // 0-4

	long m_player_rank_and_grade;

	struct // sizeof 0x688
	{
		long __unknown488;
		long __unknown48C;
		long __unknown490;
		long __unknown494;
		long __unknown498;

		// these fields are expanded from s_player_appearance
		long player_model_choice;
		s_emblem_info emblem_info;

		// model_customization_selection
		byte __dataAF0[10]; // what is the count?

		wchar_t service_tag[5];
		wchar_t desired_service_tag[5];
		bool service_tag_was_randomly_generated;
		bool service_tag_failed_verification;

	};

	long __unknownB10; // 0-1
	long __unknownB14; // 0-1
	long __unknownB18; // 0-1
	long __unknownB1C;
	byte __dataB20[0x8];

	c_static_array<long, k_popup_message_title_count> m_popup_message_indices;  // default: 0
	c_static_array<char, k_popup_message_title_count> m_popup_message_indices2; // default: 0
	byte __padB2E[0x3];

	s_campaign_game_progression __unknownB30; // film_campaign_game_progression

	// 0: campaign
	// 1: multiplayer
	// 2: editor
	bool m_film_auto_save[3];
	byte __unknownBB3;

	long m_profile_region;
	long m_gamer_zone;
	s_player_training_profile_data m_training_data;
	c_string_verify_task m_string_verify_task;
};
static_assert(sizeof(c_player_profile_interface) == 0x13F8);

