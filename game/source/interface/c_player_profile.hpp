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
	e_campaign_id campaign_id;
	e_map_id map_id;
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

struct s_player_training_profile_data
{
	long training_blob_bitvector_low[2];
	long training_blob_bitvector_high[2];
	long expansion[4];
};
static_assert(sizeof(s_player_training_profile_data) == 0x20);

enum e_campaign_game_mode
{
	_campaign_game_mode_solo = 0,
	_campaign_game_mode_coop,

	k_campaign_game_mode_count
};

template<long max_map_count, long max_insertion_point_count>
struct c_player_profile_insertion_point_flags
{
	static long const k_max_map_count = max_map_count;
	static long const k_max_insertion_point_count = max_insertion_point_count;

	//c_player_profile_insertion_point_flags();
	//void clear();
	//bool is_clear() const;
	//void set(long, short, bool);
	//bool test(long, short) const;
	//long get_flag_index(long, long) const;
	//word get_for_map(long) const;

	c_static_array<c_static_flags<k_max_map_count>, k_max_insertion_point_count> m_flags;
};
static_assert(sizeof(c_player_profile_insertion_point_flags<32, 9>) == 0x24);

enum e_popup_message_title
{
	_popup_message_title_halo3 = 0,
	//_popup_message_title_halo3_blue,

	k_popup_message_title_count,
};

enum e_film_auto_save_type
{
	_film_auto_save_type_campaign = 0,
	_film_auto_save_type_multiplayer,
	_film_auto_save_type_editor,

	k_film_auto_save_type_count
};

struct c_player_profile_interface
{
//private:
	void dirty_or(bool dirty);

	e_player_color_index get_primary_change_color() const;
	e_player_color_index get_secondary_change_color() const;

//public:
	void set_primary_change_color(long color, bool set_by_user);
	void set_secondary_change_color(long color, bool set_by_user);
	void signed_out();

//protected:
	enum
	{
		_look_inverted_bit = 0,
		_flight_stick_aircraft_controls_bit,
		_auto_center_look_bit,
		_crouch_lock_bit,
		_voice_female_bit,

		k_number_of_player_profile_settings_flags,

		//_first_internal_player_profile_bit = k_number_of_player_profile_settings_flags,
		//_settings_need_written_bit = _first_internal_player_profile_bit,
		//_settings_need_retrieval_bit,
		//_settings_need_propagated_bit,
		//_settings_io_busy_bit,
		//_title_specific_settings_previously_retrieved_bit,
		//_settings_unsafe_to_save_bit,
		//
		//k_number_of_player_profile_flags,
	};
	dword_flags m_flags;

	long m_settings_read_retry_count;
	dword m_next_settings_read_retry_time_milliseconds;
	c_enum<e_controller_index, long, _controller0, k_number_of_controllers> m_controller_index;
	bool m_achievements_report_as_obtained;

	struct
	{
		long button_preset;
		long joystick_preset;
		long look_sensitivity;
	} m_controls;

	byte __data1C[0x1D8];

	byte __data1F8[0x1];

	bool m_display_hints;

	byte __data1FA[0x2];

	struct
	{
		long attached_horizontal;
		long panning;
		long look_sensitivity;
		long flying_movement;
		long flying_thrust;
	} m_camera;

	struct
	{
		c_enum<e_campaign_difficulty_level, long, _campaign_difficulty_level_easy, k_number_of_campaign_difficulty_levels> current_difficulty;
		short current_campaign_absolute_index;
		short current_map_absolute_index;
		c_static_array<c_flags<e_campaign_difficulty_level, byte, k_number_of_campaign_difficulty_levels>, k_campaign_game_mode_count> map_difficulties_completed[32 /* campaign_level_index */];
		c_static_array<qword, k_campaign_game_mode_count> last_campaign_played_time;
		long awarded_primary_skull_bitvector;
		long awarded_secondary_skull_bitvector;
		short terminals_read_level1_bitvector;
		short terminals_read_level2_bitvector;
		c_static_flags<32> map_flags;
		c_player_profile_insertion_point_flags<32, 9> insertion_point_flags;

		struct
		{
			e_map_id solo_map_id;
			real solo_unknown;

			e_map_id coop_map_id;
			real coop_unknown;
			long coop_player_count;
		} survival;

		struct
		{
			e_map_id solo_map_id;
			short solo_insertion_point;
			byte __pad2B6[0x2];
			real solo_unknown;

			e_map_id coop_map_id;
			short coop_insertion_point;
			byte __pad2C2[0x2];
			real coop_unknown;
			long coop_player_count;
		} campaign;

		byte __data2CC[0x4]; // pad?

		s_campaign_progression_profile_data m_campaign_progression;
	} m_campaign;

	struct
	{
		short last_hopper_identifier;
		long last_map;
		long last_variant_type;
		long last_variant_checksum;
		long spartan_program_best_milestone;
		long spartan_program_best_rank;
	} m_multiplayer;

	struct
	{
		c_enum<e_player_color_index, long, _player_color_none, k_player_color_index_count> primary_color;
		c_enum<e_player_color_index, long, _player_color_none, k_player_color_index_count> secondary_color;

		long __unknown490;
		long __unknown494;
		long __unknown498;

		long player_model_choice;
		s_emblem_info emblem;
		byte model_area_selections[10];
		wchar_t last_known_good_service_tag[5];
		wchar_t desired_service_tag[5];
		bool service_tag_was_randomly_generated;
		bool service_tag_failed_verification;
	} m_appearance;

	struct
	{
		long mute_setting;
		long output_setting;
		long matchmaking_setting;
		long voice_mask;
		long guide_voice_through_speakers;
		bool guide_voice_muted;
	} m_voice;

	struct
	{
		c_static_array<long, k_popup_message_title_count> last_shown_popup_message_index;
		char last_shown_vidmaster_popup_message_index;
	} m_online;

	// film_progression?
	s_campaign_game_progression __unknownB30;

	bool m_film_auto_save[k_film_auto_save_type_count];
	long m_profile_region;
	long m_gamer_zone;
	s_player_training_profile_data m_training_data;
	c_string_verify_task m_string_verify_task;
};
static_assert(sizeof(c_player_profile_interface) == 0x13F8);

