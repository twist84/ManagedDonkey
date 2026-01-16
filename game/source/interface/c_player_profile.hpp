#pragma once

#include "cseries/cseries.hpp"
#include "game/game_progression.hpp"
#include "game/players.hpp"
#include "networking/online/online_guide_pc.hpp"

struct s_player_appearance;

struct s_campaign_progression_profile_data
{
	uns64 __time0;
	e_campaign_id campaign_id;
	e_map_id map_id;
	int16 campaign_insertion_index;
	s_campaign_armaments campaign_armaments;
	byte __pad8A[0x2];
	s_campaign_game_progression campaign_game_progression;
	s_campaign_game_progression hub_progression;
	int32 campaign_difficulty;
	int32 campaign_metagame_scoring;
	uns32 active_primary_skulls;
	uns32 active_secondary_skulls;
	int32 __unknown19C;
};
COMPILE_ASSERT(sizeof(s_campaign_progression_profile_data) == 0x1A0);

struct s_player_training_profile_data
{
	int32 training_blob_bitvector_low[2];
	int32 training_blob_bitvector_high[2];
	int32 expansion[4];
};
COMPILE_ASSERT(sizeof(s_player_training_profile_data) == 0x20);

enum e_campaign_game_mode
{
	_campaign_game_mode_solo = 0,
	_campaign_game_mode_coop,

	k_campaign_game_mode_count
};

template<int32 max_map_count, int32 max_insertion_point_count>
class c_player_profile_insertion_point_flags
{
public:
	static int32 const k_max_map_count = max_map_count;
	static int32 const k_max_insertion_point_count = max_insertion_point_count;

	//c_player_profile_insertion_point_flags();
	//void clear();
	//bool is_clear() const;
	//void set(int32, int16, bool);
	//bool test(int32, int16) const;
	//int32 get_flag_index(int32, int32) const;
	//uns16 get_for_map(int32) const;

	c_static_array<c_static_flags<k_max_map_count>, k_max_insertion_point_count> m_flags;
};
COMPILE_ASSERT(sizeof(c_player_profile_insertion_point_flags<32, 9>) == 0x24);

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

class c_player_profile_interface
{
public:
//private:
	void dirty_or(bool dirty);

	e_campaign_difficulty_level get_campaign_difficulty() const;
	bool get_female_voice_enabled() const;
	int8 get_model_customization_selection(e_player_model_choice player_model_choice, int32 area_index) const;
	void get_player_appearance(s_player_appearance* player_appearance);
	s_emblem_info get_emblem_info() const;
	e_player_model_choice get_player_model_choice() const;

	e_player_color_index get_primary_change_color() const;
	e_player_color_index get_secondary_change_color() const;

	bool player_campaign_progress_test_insertion_point(int16 map_index, int16 insertion_point_index) const;

//public:
	void set_button_preset(e_button_presets preset, bool set_by_user);
	void set_camera_attached_horizontal(e_camera_attached_horizontal value, bool set_by_user);
	void set_camera_flying_movement(e_camera_flying_movement value, bool set_by_user);
	void set_camera_flying_thrust(e_camera_flying_thrust value, bool set_by_user);
	void set_camera_look_sensitivity(e_camera_look_sensitivity value, bool set_by_user);
	void set_camera_panning(e_camera_panning value, bool set_by_user);
	void set_campaign_awarded_primary_skull_bitvector(int32 primary_skull_bitvector, bool set_by_user);
	void set_campaign_awarded_secondary_skull_bitvector(int32 secondary_skull_bitvector, bool set_by_user);
	void set_campaign_difficulty(e_campaign_difficulty_level difficulty, bool set_by_user);
	void set_campaign_terminal_read_level1_bitvector(int16 level1_bitvector, bool set_by_user);
	void set_campaign_terminal_read_level2_bitvector(int16 level2_bitvector, bool set_by_user);
	void set_controller_auto_center_look(bool auto_center, bool set_by_user);
	void set_controller_crouch_lock_enabled(bool crouch_lock, bool set_by_user);
	void set_controller_flight_stick_aircraft_controls(bool aircraft_controls, bool set_by_user);
	void set_controller_look_inverted(bool invert, bool set_by_user);
	void set_desired_service_tag(const wchar_t* value, bool set_by_user);
	void set_display_brightness(e_display_brightness setting, bool set_by_user);
	void set_display_hints_enabled(bool setting);
	void set_female_voice_enabled(bool female, bool set_by_user);
	void set_film_auto_save(e_film_auto_save_type type, bool value, bool set_by_user);
	void set_guide_voice_muted(bool setting);
	void set_guide_voice_through_speakers(e_guide_voice_through_speakers setting);
	void set_joystick_preset(e_joystick_presets preset, bool set_by_user);
	void set_last_campaign_map_played_absolute_index(int16 absolute_map_index, bool set_by_user);
	void set_last_campaign_played_absolute_index(int16 absolute_campaign_index, bool set_by_user);
	void set_last_campaign_played_time(e_campaign_game_mode campaign_game_mode, uns64 last_campaign_played_time, bool set_by_user);
	void set_last_custom_multiplayer_map_played(e_map_id map, bool set_by_user);
	void set_last_network_game_hopper_played(uns16 hopper_identifier, bool set_by_user);
	void set_last_shown_popup_message_index(e_popup_message_title title_index, int32 message_index, bool set_by_user);
	void set_last_variant_played(e_game_engine_type type, int32 checksum, bool set_by_user);
	void set_look_sensitivity(e_look_sensitivity sensitivity, bool set_by_user);
	void set_map_completed_at_difficulty_level(e_campaign_game_mode campaign_game_mode, int32 absolute_map_index, e_campaign_difficulty_level difficulty, bool complete, bool set_by_user);
	void set_model_customization_selection(e_player_model_choice player_model_choice, int32 area_index, int8 selection_index, bool set_by_user);
	void set_player_appearance(const s_player_appearance* player_appearance, bool set_by_user);
	void set_emblem_info(const s_emblem_info* emblem_info, bool set_by_user);
	void set_player_model_choice(e_player_model_choice player_model_choice, bool set_by_user);
	void __thiscall set_primary_change_color(e_player_color_index color, bool set_by_user);
	void __thiscall set_secondary_change_color(e_player_color_index color, bool set_by_user);
	void set_service_tag_failed_verification(bool value, bool set_by_user);
	void set_service_tag_validated(const wchar_t* value, bool set_by_user);
	void set_service_tag_was_randomly_generated(bool value, bool set_by_user);
	void set_subtitle_setting(e_subtitle_setting setting, bool set_by_user);
	void set_tertiary_change_color(e_player_color_index color, bool set_by_user);
	void set_voice_mask(e_voice_mask mask, bool set_by_user);
	void set_voice_matchmaking_setting(e_voice_matchmaking_setting setting, bool set_by_user);
	void set_voice_mute_setting(e_voice_mute_setting setting, bool set_by_user);
	void set_voice_output_setting(e_voice_output_setting setting, bool set_by_user);
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
	uns32 m_flags;

	int32 m_settings_read_retry_count;
	uns32 m_next_settings_read_retry_time_milliseconds;
	c_enum<e_controller_index, int32, _controller0, k_number_of_controllers> m_controller_index;
	bool m_achievements_report_as_obtained;

	struct
	{
		int32 button_preset;
		int32 joystick_preset;
		int32 look_sensitivity;
	} m_controls;

	byte __data1C[0x1D8];

	byte __data1F8[0x1];

	bool m_display_hints;

	byte __data1FA[0x2];

	struct
	{
		e_camera_attached_horizontal attached_horizontal;
		e_camera_panning panning;
		e_camera_look_sensitivity look_sensitivity;
		e_camera_flying_movement flying_movement;
		e_camera_flying_thrust flying_thrust;
	} m_camera;

	struct
	{
		c_enum<e_campaign_difficulty_level, int32, _campaign_difficulty_level_easy, k_campaign_difficulty_levels_count> current_difficulty;
		int16 current_campaign_absolute_index;
		int16 current_map_absolute_index;
		c_static_array<c_flags<e_campaign_difficulty_level, uns8, k_campaign_difficulty_levels_count>, k_campaign_game_mode_count> map_difficulties_completed[32 /* campaign_level_index */];
		c_static_array<uns64, k_campaign_game_mode_count> last_campaign_played_time;
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
			byte __pad2B6[0x2];
			real32 solo_unknown;

			e_map_id coop_map_id;
			int16 coop_insertion_point;
			byte __pad2C2[0x2];
			real32 coop_unknown;
			int32 coop_player_count;
		} campaign;

		byte __data2CC[0x4]; // pad?

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
		e_player_color_index primary_color;
		e_player_color_index secondary_color;
		e_player_color_index tertiary_color;
		e_player_color_index quaternary_color;
		e_player_color_index quinary_color;
		e_player_model_choice player_model_choice;
		s_emblem_info emblem;
		int8 model_area_selections[k_player_model_choice_count][10];
		byte emblem_pad[0x63E];
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
COMPILE_ASSERT(sizeof(c_player_profile_interface) == 0x13F8);

