#pragma once

#include "cseries/cseries.hpp"
#include "game/game_progression.hpp"
#include "game/players.hpp"
#include "networking/online/online_guide_pc.hpp"

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
	uint64 __time0;
	e_campaign_id campaign_id;
	e_map_id map_id;
	int16 campaign_insertion_index;
	s_campaign_armaments campaign_armaments;
	uint8 __pad8A[0x2];
	s_campaign_game_progression campaign_game_progression;
	s_campaign_game_progression hub_progression;
	int32 campaign_difficulty;
	int32 campaign_metagame_scoring;
	uint32 active_primary_skulls;
	uint32 active_secondary_skulls;
	int32 __unknown19C;
};
static_assert(sizeof(s_campaign_progression_profile_data) == 0x1A0);

struct s_player_training_profile_data
{
	int32 training_blob_bitvector_low[2];
	int32 training_blob_bitvector_high[2];
	int32 expansion[4];
};
static_assert(sizeof(s_player_training_profile_data) == 0x20);

enum e_campaign_game_mode
{
	_campaign_game_mode_solo = 0,
	_campaign_game_mode_coop,

	k_campaign_game_mode_count
};

template<int32 max_map_count, int32 max_insertion_point_count>
struct c_player_profile_insertion_point_flags
{
	static int32 const k_max_map_count = max_map_count;
	static int32 const k_max_insertion_point_count = max_insertion_point_count;

	//c_player_profile_insertion_point_flags();
	//void clear();
	//bool is_clear() const;
	//void set(int32, int16, bool);
	//bool test(int32, int16) const;
	//int32 get_flag_index(int32, int32) const;
	//uint16 get_for_map(int32) const;

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
	void set_primary_change_color(int32 color, bool set_by_user);
	void set_secondary_change_color(int32 color, bool set_by_user);
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
	uint32 m_flags;

	int32 m_settings_read_retry_count;
	uint32 m_next_settings_read_retry_time_milliseconds;
	c_enum<e_controller_index, int32, _controller0, k_number_of_controllers> m_controller_index;
	bool m_achievements_report_as_obtained;

	struct
	{
		int32 button_preset;
		int32 joystick_preset;
		int32 look_sensitivity;
	} m_controls;

	uint8 __data1C[0x1D8];

	uint8 __data1F8[0x1];

	bool m_display_hints;

	uint8 __data1FA[0x2];

	struct
	{
		int32 attached_horizontal;
		int32 panning;
		int32 look_sensitivity;
		int32 flying_movement;
		int32 flying_thrust;
	} m_camera;

	struct
	{
		c_enum<e_campaign_difficulty_level, int32, _campaign_difficulty_level_easy, k_campaign_difficulty_levels_count> current_difficulty;
		int16 current_campaign_absolute_index;
		int16 current_map_absolute_index;
		c_static_array<c_flags<e_campaign_difficulty_level, uint8, k_campaign_difficulty_levels_count>, k_campaign_game_mode_count> map_difficulties_completed[32 /* campaign_level_index */];
		c_static_array<uint64, k_campaign_game_mode_count> last_campaign_played_time;
		int32 awarded_primary_skull_bitvector;
		int32 awarded_secondary_skull_bitvector;
		int16 terminals_read_level1_bitvector;
		int16 terminals_read_level2_bitvector;
		c_static_flags<32> map_flags;
		c_player_profile_insertion_point_flags<32, 9> insertion_point_flags;

		struct
		{
			e_map_id solo_map_id;
			real32 solo_unknown;

			e_map_id coop_map_id;
			real32 coop_unknown;
			int32 coop_player_count;
		} survival;

		struct
		{
			e_map_id solo_map_id;
			int16 solo_insertion_point;
			uint8 __pad2B6[0x2];
			real32 solo_unknown;

			e_map_id coop_map_id;
			int16 coop_insertion_point;
			uint8 __pad2C2[0x2];
			real32 coop_unknown;
			int32 coop_player_count;
		} campaign;

		uint8 __data2CC[0x4]; // pad?

		s_campaign_progression_profile_data m_campaign_progression;
	} m_campaign;

	struct
	{
		int16 last_hopper_identifier;
		int32 last_map;
		int32 last_variant_type;
		int32 last_variant_checksum;
		int32 spartan_program_best_milestone;
		int32 spartan_program_best_rank;
	} m_multiplayer;

	struct
	{
		c_enum<e_player_color_index, int32, _player_color_none, k_player_color_index_count> primary_color;
		c_enum<e_player_color_index, int32, _player_color_none, k_player_color_index_count> secondary_color;

		int32 __unknown490;
		int32 __unknown494;
		int32 __unknown498;

		int32 player_model_choice;
		s_emblem_info emblem;
		uint8 model_area_selections[10];
		wchar_t last_known_good_service_tag[5];
		wchar_t desired_service_tag[5];
		bool service_tag_was_randomly_generated;
		bool service_tag_failed_verification;
	} m_appearance;

	struct
	{
		int32 mute_setting;
		int32 output_setting;
		int32 matchmaking_setting;
		int32 voice_mask;
		int32 guide_voice_through_speakers;
		bool guide_voice_muted;
	} m_voice;

	struct
	{
		c_static_array<int32, k_popup_message_title_count> last_shown_popup_message_index;
		int8 last_shown_vidmaster_popup_message_index;
	} m_online;

	// film_progression?
	s_campaign_game_progression __unknownB30;

	bool m_film_auto_save[k_film_auto_save_type_count];
	int32 m_profile_region;
	int32 m_gamer_zone;
	s_player_training_profile_data m_training_data;
	c_string_verify_task m_string_verify_task;
};
static_assert(sizeof(c_player_profile_interface) == 0x13F8);

