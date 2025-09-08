#include "interface/c_player_profile.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"

REFERENCE_DECLARE(0x0191D4FC, bool, g_all_level_unlocked) = true;

//HOOK_DECLARE_CLASS_MEMBER(0x00AA0320, c_player_profile_interface, dirty_or);
//HOOK_DECLARE_CLASS_MEMBER(0x00AA36B0, c_player_profile_interface, set_primary_change_color);
//HOOK_DECLARE_CLASS_MEMBER(0x00AA3860, c_player_profile_interface, set_secondary_change_color);

//.text:00A9FEF0 ; 
//.text:00A9FF00 ; 
//.text:00A9FF40 ; 
//.text:00A9FF70 ; 
//.text:00A9FF90 ; public: c_player_profile_interface::c_player_profile_interface()
//.text:00AA00F0 ; 
//.text:00AA0100 ; 
//.text:00AA0110 ; void __cdecl c_player_profile_dispose()
//.text:00AA0120 ; void __cdecl c_player_profile_initialize()
//.text:00AA0130 ; 
//.text:00AA0140 ; 
//.text:00AA0160 ; 
//.text:00AA0180 ; 
//.text:00AA0210 ; 
//.text:00AA0240 ; 
//.text:00AA0270 ; 
//.text:00AA02A0 ; public: bool c_player_profile_interface::get_map_completed_at_difficulty_level(e_campaign_game_mode, int32) const

void __thiscall c_player_profile_interface::dirty_or(bool dirty)
{
	INVOKE_CLASS_MEMBER(0x00AA0320, c_player_profile_interface, dirty_or, dirty);

	//if (TEST_BIT(m_flags, 5) || dirty)
	//{
	//	if (VALID_INDEX(m_controller_index, k_number_of_controllers))
	//	{
	//		c_controller_interface* controller = controller_get(m_controller_index);
	//		if (controller->is_signed_in_to_machine())
	//			SET_BIT(m_flags, 5, true);
	//
	//		if (controller->in_use())
	//			SET_BIT(m_flags, 7, true);
	//	}
	//}
}

//.text:00AA0380 ; private: static void c_player_profile_interface::generate_random_service_tag(wchar_t* const)
//.text:00AA03E0 ; public: bool c_player_profile_interface::get_async_io_busy() const
//.text:00AA03F0 ; public: e_button_presets c_player_profile_interface::get_button_preset() const
//.text:00AA0400 ; public: e_camera_attached_horizontal c_player_profile_interface::get_camera_attached_horizontal() const
//.text:00AA0410 ; public: e_camera_flying_movement c_player_profile_interface::get_camera_flying_movement() const
//.text:00AA0420 ; public: e_camera_flying_thrust c_player_profile_interface::get_camera_flying_thrust() const
//.text:00AA0430 ; public: e_camera_look_sensitivity c_player_profile_interface::get_camera_look_sensitivity() const
//.text:00AA0440 ; public: e_camera_panning c_player_profile_interface::get_camera_panning() const
//.text:00AA0450 ; public: int32 c_player_profile_interface::get_campaign_awarded_primary_skull_bitvector(int32*) const
//.text:00AA0470 ; public: int32 c_player_profile_interface::get_campaign_awarded_secondary_skull_bitvector(int32*) const

e_campaign_difficulty_level c_player_profile_interface::get_campaign_difficulty() const
{
	//return INVOKE_CLASS_MEMBER(0x00AA0490, c_player_profile_interface, get_campaign_difficulty);

	return m_campaign.current_difficulty;
}

//.text:00AA04A0 ; 
//.text:00AA0500 ; calculate_campaign_completion
//.text:00AA06F0 ; public: int32 c_player_profile_interface::get_campaign_terminal_read_level1_bitvector() const
//.text:00AA0700 ; public: int32 c_player_profile_interface::get_campaign_terminal_read_level2_bitvector() const
//.text:00AA0710 ; public: bool c_player_profile_interface::get_controller_auto_center_look() const
//.text:00AA0720 ; public: bool c_player_profile_interface::get_controller_crouch_lock_enabled() const
//.text:00AA0730 ; public: bool c_player_profile_interface::get_controller_flight_stick_aircraft_controls() const
//.text:00AA0740 ; public: e_controller_index c_player_profile_interface::get_controller_index() const
//.text:00AA0750 ; public: bool c_player_profile_interface::get_controller_look_inverted() const
//.text:00AA0760 ; 
//.text:00AA0780 ; public: const wchar_t* c_player_profile_interface::get_desired_service_tag() const
//.text:00AA0790 ; public: e_display_brightness c_player_profile_interface::get_display_brightness() const
//.text:00AA07A0 ; public: bool c_player_profile_interface::get_display_hints_enabled() const

bool c_player_profile_interface::get_female_voice_enabled() const
{
	return INVOKE_CLASS_MEMBER(0x00AA07B0, c_player_profile_interface, get_female_voice_enabled);
}

//.text:00AA07C0 ; public: bool c_player_profile_interface::get_film_auto_save(e_film_gauto_save_type) const
//.text:00AA07E0 ; 
//.text:00AA0800 ; public: bool c_player_profile_interface::get_guide_voice_muted() const
//.text:00AA0810 ; public: e_guide_voice_through_speakers c_player_profile_interface::get_guide_voice_through_speakers() const
//.text:00AA0820 ; public: e_joystick_presets c_player_profile_interface::get_joystick_preset() const
//.text:00AA0830 ; public: static e_language c_player_profile_interface::get_language()
//.text:00AA0840 ; public: int16 c_player_profile_interface::get_last_campaign_map_played_absolute_index() const
//.text:00AA0850 ; public: int16 c_player_profile_interface::get_last_campaign_played_absolute_index() const
//.text:00AA0860 ; public: uns64 c_player_profile_interface::get_last_campaign_played_time(e_campaign_game_mode) const
//.text:00AA0880 ; public: e_map_id c_player_profile_interface::get_last_custom_multiplayer_map_played() const
//.text:00AA0890 ; public: uns16 c_player_profile_interface::get_last_network_game_hopper_played() const
//.text:00AA08A0 ; public: int32 c_player_profile_interface::get_last_shown_popup_message_index(e_popup_message_title) const
//.text:00AA08C0 ; public: bool c_player_profile_interface::get_last_variant_played(e_game_engine_type*, int32*) const
//.text:00AA08F0 ; public: e_look_sensitivity c_player_profile_interface::get_look_sensitivity() const
//.text:00AA0900 ; public: bool c_player_profile_interface::get_map_completed_at_difficulty_level(e_campaign_game_mode, int32, e_campaign_difficulty_level) const
//.text:00AA0960 ; public: char c_player_profile_interface::get_model_customization_selection(e_player_model_choice, int32) const
//.text:00AA0A00 ; public: char c_player_profile_interface::get_model_customization_selection(e_player_model_choice, int32) const
//.text:00AA0A30 ; public: void c_player_profile_interface::get_player_appearance(s_player_appearance*)

s_emblem_info c_player_profile_interface::get_emblem_info() const
{
	//return INVOKE_CLASS_MEMBER(0x00AA0AB0, c_player_profile_interface, get_emblem_info);

	return m_appearance.emblem;
}

e_player_model_choice c_player_profile_interface::get_player_model_choice() const
{
	return INVOKE_CLASS_MEMBER(0x00AA0AC0, c_player_profile_interface, get_player_model_choice);

	//if (!IN_RANGE_INCLUSIVE(m_appearance.player_model_choice, _player_model_choice_spartan, _player_model_choice_elite))
	//{
	//	return _player_model_choice_first;
	//}
	//
	//return (e_player_model_choice)m_appearance.player_model_choice;
}

e_player_color_index c_player_profile_interface::get_primary_change_color() const
{
	return INVOKE_CLASS_MEMBER(0x00AA0AE0, c_player_profile_interface, get_primary_change_color);

	//return m_appearance.primary_color;
}

//.text:00AA0AF0 ; public: e_player_color_index c_player_profile_interface::get_quaternary_change_color() const
//.text:00AA0B00 ; public: e_player_color_index c_player_profile_interface::get_quinary_change_color() const
//.text:00AA0B10 ; public: static e_region __cdecl c_player_profile_interface::get_region()

e_player_color_index c_player_profile_interface::get_secondary_change_color() const
{
	return INVOKE_CLASS_MEMBER(0x00AA0B20, c_player_profile_interface, get_secondary_change_color);

	//return m_appearance.secondary_color;
}

//.text:00AA0B30 ; public: bool c_player_profile_interface::get_service_tag_failed_verification() const
//.text:00AA0B40 ; public: const wchar_t* c_player_profile_interface::get_service_tag_validated() const
//.text:00AA0B50 ; public: bool c_player_profile_interface::get_service_tag_was_randomly_generated() const
//.text:00AA0B60 ; 
//.text:00AA0B70 ; public: e_experience_milestone c_player_profile_interface::get_spartan_program_best_milestone() const
//.text:00AA0B80 ; public: void c_player_profile_interface::get_spartan_program_best_rank(int32, uns16*, int32*) const
//.text:00AA0B90 ; 
//.text:00AA0BA0 ; 
//.text:00AA0BB0 ; public: e_subtitle_setting c_player_profile_interface::get_subtitle_setting() const
//.text:00AA0BC0 ; 
//.text:00AA0C40 ; public: e_player_color_index c_player_profile_interface::get_tertiary_change_color() const
//.text:00AA0C50 ; private: int32 c_player_profile_interface::get_total_achievement_points() const
//.text:00AA0C90 ; private: int32 c_player_profile_interface::get_total_skull_count() const
//.text:00AA0CE0 ; public: void c_player_profile_interface::get_training_data(s_player_training_profile_data*) const
//.text:00AA0D20 ; public: bool c_player_profile_interface::get_unlockable_unlocked(int32) const
//.text:00AA0FF0 ; 
//.text:00AA1000 ; 
//.text:00AA1010 ; public: e_voice_mask c_player_profile_interface::get_voice_mask() const
//.text:00AA1020 ; public: e_voice_matchmaking_setting c_player_profile_interface::get_voice_matchmaking_setting() const
//.text:00AA1030 ; public: e_voice_mute_setting c_player_profile_interface::get_voice_mute_setting() const
//.text:00AA1040 ; 
//.text:00AA1050 ; 
//.text:00AA1060 ; private: void c_player_profile_interface::initialize_to_default_settings()
//.text:00AA18D0 ; 
//.text:00AA1930 ; 
//.text:00AA1990 ; public: static bool __cdecl c_player_profile_interface::is_service_tag_validly_formatted(const wchar_t*)
//.text:00AA1A60 ; 
//.text:00AA1AD0 ; 
//.text:00AA1B60 ; 
//.text:00AA1B70 ; 
//.text:00AA1B90 ; 
//.text:00AA1BB0 ; 
//.text:00AA1C70 ; public: void c_player_profile_interface::player_campaign_progress_finished_map(int16, bool)
//.text:00AA1C90 ; public: void c_player_profile_interface::player_campaign_progress_set_insertion_point(int16, int16, bool, bool)
//.text:00AA1D50 ; public: void c_player_profile_interface::player_campaign_progress_set_map(int16, bool, bool)

bool c_player_profile_interface::player_campaign_progress_test_insertion_point(int16 map_index, int16 insertion_point_index) const
{
	return INVOKE_CLASS_MEMBER(0x00AA1E10, c_player_profile_interface, player_campaign_progress_test_insertion_point, map_index, insertion_point_index);
}

//.text:00AA1E60 ; public: bool c_player_profile_interface::player_campaign_progress_test_map(int16) const
//.text:00AA1EB0 ; 
//.text:00AA1F50 ; 
//.text:00AA1F80 ; 
//.text:00AA1F90 ; 
//.text:00AA1FD0 ; 
//.text:00AA2070 ; public: void c_player_profile_interface::propagate_settings()
//.text:00AA23B0 ; 
//.text:00AA23C0 ; public: void __cdecl c_player_profile_insertion_point_flags<64, 12>::set(int32, int16, bool)
//.text:00AA2400 ; 
//.text:00AA2430 ; 
//.text:00AA2450 ; 
//.text:00AA2460 ; 

void c_player_profile_interface::set_button_preset(e_button_presets preset, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA2470, c_player_profile_interface, set_button_preset, preset, set_by_user);
}

void c_player_profile_interface::set_camera_attached_horizontal(e_camera_attached_horizontal value, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA24B0, c_player_profile_interface, set_camera_attached_horizontal, value, set_by_user);
}

void c_player_profile_interface::set_camera_flying_movement(e_camera_flying_movement value, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA2500, c_player_profile_interface, set_camera_flying_movement, value, set_by_user);
}

void c_player_profile_interface::set_camera_flying_thrust(e_camera_flying_thrust value, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA2550, c_player_profile_interface, set_camera_flying_thrust, value, set_by_user);
}

void c_player_profile_interface::set_camera_look_sensitivity(e_camera_look_sensitivity value, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA25A0, c_player_profile_interface, set_camera_look_sensitivity, value, set_by_user);
}

void c_player_profile_interface::set_camera_panning(e_camera_panning value, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA25F0, c_player_profile_interface, set_camera_panning, value, set_by_user);
}

void c_player_profile_interface::set_campaign_awarded_primary_skull_bitvector(int32 primary_skull_bitvector, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA2640, c_player_profile_interface, set_campaign_awarded_primary_skull_bitvector, primary_skull_bitvector, set_by_user);
}

void c_player_profile_interface::set_campaign_awarded_secondary_skull_bitvector(int32 secondary_skull_bitvector, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA26D0, c_player_profile_interface, set_campaign_awarded_secondary_skull_bitvector, secondary_skull_bitvector, set_by_user);
}

void c_player_profile_interface::set_campaign_difficulty(e_campaign_difficulty_level difficulty, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA2760, c_player_profile_interface, set_campaign_difficulty, difficulty, set_by_user);
}

//.text:00AA27B0 ; 

void c_player_profile_interface::set_campaign_terminal_read_level1_bitvector(int16 level1_bitvector, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA28A0, c_player_profile_interface, set_campaign_terminal_read_level1_bitvector, level1_bitvector, set_by_user);
}

void c_player_profile_interface::set_campaign_terminal_read_level2_bitvector(int16 level2_bitvector, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA2930, c_player_profile_interface, set_campaign_terminal_read_level2_bitvector, level2_bitvector, set_by_user);
}

void c_player_profile_interface::set_controller_auto_center_look(bool auto_center, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA29C0, c_player_profile_interface, set_controller_auto_center_look, auto_center, set_by_user);
}

void c_player_profile_interface::set_controller_crouch_lock_enabled(bool crouch_lock, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA2A50, c_player_profile_interface, set_controller_crouch_lock_enabled, crouch_lock, set_by_user);
}

void c_player_profile_interface::set_controller_flight_stick_aircraft_controls(bool aircraft_controls, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA2AF0, c_player_profile_interface, set_controller_flight_stick_aircraft_controls, aircraft_controls, set_by_user);
}

void c_player_profile_interface::set_controller_look_inverted(bool invert, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA2B80, c_player_profile_interface, set_controller_look_inverted, invert, set_by_user);
}

//.text:00AA2C00 ; 
//.text:00AA2C70 ; 
//.text:00AA2D60 ; 

void c_player_profile_interface::set_desired_service_tag(const wchar_t* value, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA2E30, c_player_profile_interface, set_desired_service_tag, value, set_by_user);
}

void c_player_profile_interface::set_display_brightness(e_display_brightness setting, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA2EE0, c_player_profile_interface, set_display_brightness, setting, set_by_user);
}

void c_player_profile_interface::set_display_hints_enabled(bool setting)
{
	INVOKE_CLASS_MEMBER(0x00AA2F00, c_player_profile_interface, set_display_hints_enabled, setting);
}

void c_player_profile_interface::set_female_voice_enabled(bool female, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA2F80, c_player_profile_interface, set_female_voice_enabled, female, set_by_user);
}

void c_player_profile_interface::set_film_auto_save(e_film_auto_save_type type, bool value, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA3010, c_player_profile_interface, set_film_auto_save, type, value, set_by_user);
}

void c_player_profile_interface::set_guide_voice_muted(bool setting)
{
	INVOKE_CLASS_MEMBER(0x00AA3060, c_player_profile_interface, set_guide_voice_muted, setting);
}

void c_player_profile_interface::set_guide_voice_through_speakers(e_guide_voice_through_speakers setting)
{
	INVOKE_CLASS_MEMBER(0x00AA3070, c_player_profile_interface, set_guide_voice_through_speakers, setting);
}

void c_player_profile_interface::set_joystick_preset(e_joystick_presets preset, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA3090, c_player_profile_interface, set_joystick_preset, preset, set_by_user);
}

void c_player_profile_interface::set_last_campaign_map_played_absolute_index(int16 absolute_map_index, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA30D0, c_player_profile_interface, set_last_campaign_map_played_absolute_index, absolute_map_index, set_by_user);
}

void c_player_profile_interface::set_last_campaign_played_absolute_index(int16 absolute_campaign_index, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA3160, c_player_profile_interface, set_last_campaign_played_absolute_index, absolute_campaign_index, set_by_user);
}

void c_player_profile_interface::set_last_campaign_played_time(e_campaign_game_mode campaign_game_mode, uns64 last_campaign_played_time, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA31F0, c_player_profile_interface, set_last_campaign_played_time, campaign_game_mode, last_campaign_played_time, set_by_user);
}

void c_player_profile_interface::set_last_custom_multiplayer_map_played(e_map_id map, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA3230, c_player_profile_interface, set_last_custom_multiplayer_map_played, map, set_by_user);
}

void c_player_profile_interface::set_last_network_game_hopper_played(uns16 hopper_identifier, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA32B0, c_player_profile_interface, set_last_network_game_hopper_played, hopper_identifier, set_by_user);
}

void c_player_profile_interface::set_last_shown_popup_message_index(e_popup_message_title title_index, int32 message_index, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA3340, c_player_profile_interface, set_last_shown_popup_message_index, title_index, message_index, set_by_user);
}

void c_player_profile_interface::set_last_variant_played(e_game_engine_type type, int32 checksum, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA33C0, c_player_profile_interface, set_last_variant_played, type, checksum, set_by_user);
}

void c_player_profile_interface::set_look_sensitivity(e_look_sensitivity sensitivity, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA3410, c_player_profile_interface, set_look_sensitivity, sensitivity, set_by_user);
}

void c_player_profile_interface::set_map_completed_at_difficulty_level(e_campaign_game_mode campaign_game_mode, int32 absolute_map_index, e_campaign_difficulty_level difficulty, bool complete, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA3450, c_player_profile_interface, set_map_completed_at_difficulty_level, campaign_game_mode, absolute_map_index, difficulty, complete, set_by_user);
}

void c_player_profile_interface::set_model_customization_selection(e_player_model_choice player_model_choice, int32 area_index, int8 selection_index, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA34D0, c_player_profile_interface, set_model_customization_selection, player_model_choice, area_index, selection_index, set_by_user);
}

void c_player_profile_interface::set_player_appearance(const s_player_appearance* player_appearance, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA3560, c_player_profile_interface, set_player_appearance, player_appearance, set_by_user);
}

//.text:00AA35C0 ; public: void c_player_profile_interface::set_emblem_info(int32, const s_emblem_info*)
void c_player_profile_interface::set_emblem_info(const s_emblem_info* emblem_info, bool set_by_user)
{
	ASSERT(emblem_info != NULL);
	ASSERT(emblem_info->pad == 0);

	if (emblem_info->foreground_emblem_index >= 80
		|| emblem_info->background_emblem_index >= 48
		|| emblem_info->primary_color_index >= k_player_color_index_count
		|| emblem_info->secondary_color_index >= k_player_color_index_count
		|| emblem_info->background_color_index >= k_player_color_index_count)
	{
		event(_event_error, "ui:profile: invalid emblem");
	}
	else
	{
		dirty_or(set_by_user && csmemcmp(emblem_info, &m_appearance.emblem, sizeof(s_emblem_info)) != 0);
		m_appearance.emblem = *emblem_info;
	}
}

void c_player_profile_interface::set_player_model_choice(e_player_model_choice player_model_choice, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA3620, c_player_profile_interface, set_player_model_choice, player_model_choice, set_by_user);
}

void __thiscall c_player_profile_interface::set_primary_change_color(e_player_color_index color, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA36B0, c_player_profile_interface, set_primary_change_color, color, set_by_user);

	//if (VALID_INDEX(color, k_player_color_index_count))
	//{
	//	dirty_or(set_by_user && color != get_primary_change_color());
	//	m_primary_change_color = color;
	//}
	//else
	//{
	//	event(_event_error, "ui:profile: invalid primary color index (#%ld)", color);
	//}
}

//.text:00AA3740 ; public: void c_player_profile_interface::set_quaternary_change_color(e_player_color_index, bool)
//.text:00AA37D0 ; public: void c_player_profile_interface::set_quinary_change_color(e_player_color_index, bool)

void __thiscall c_player_profile_interface::set_secondary_change_color(e_player_color_index color, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA3860, c_player_profile_interface, set_secondary_change_color, color, set_by_user);

	//if (VALID_INDEX(color, k_player_color_index_count))
	//{
	//	dirty_or(set_by_user && color != get_primary_change_color());
	//	m_secondary_change_color = color;
	//}
	//else
	//{
	//	event(_event_error, "ui:profile: invalid primary color index (#%ld)", color);
	//}
}

void c_player_profile_interface::set_service_tag_failed_verification(bool value, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA38F0, c_player_profile_interface, set_service_tag_failed_verification, value, set_by_user);
}

void c_player_profile_interface::set_service_tag_validated(const wchar_t* value, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA3980, c_player_profile_interface, set_service_tag_validated, value, set_by_user);
}

void c_player_profile_interface::set_service_tag_was_randomly_generated(bool value, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA3A30, c_player_profile_interface, set_service_tag_was_randomly_generated, value, set_by_user);
}

//.text:00AA3AC0 ; 
//.text:00AA3B30 ; public: void c_player_profile_interface::set_spartan_program_best_milestone(e_experience_milestone, bool)
//.text:00AA3B80 ; public: void c_player_profile_interface::set_spartan_program_best_rank(int32, bool)

void c_player_profile_interface::set_subtitle_setting(e_subtitle_setting setting, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA3C10, c_player_profile_interface, set_subtitle_setting, setting, set_by_user);
}

//.text:00AA3C30 ; 
//.text:00AA3D40 ; public: void c_player_profile_interface::set_tertiary_change_color(e_player_color_index, bool)
//.text:00AA3DD0 ; public: void c_player_profile_interface::set_training_data(const s_player_training_profile_data*, bool)
//.text:00AA3EA0 ; 

void c_player_profile_interface::set_voice_mask(e_voice_mask mask, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA3F30, c_player_profile_interface, set_voice_mask, mask, set_by_user);
}

void c_player_profile_interface::set_voice_matchmaking_setting(e_voice_matchmaking_setting setting, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA3F80, c_player_profile_interface, set_voice_matchmaking_setting, setting, set_by_user);
}

void c_player_profile_interface::set_voice_mute_setting(e_voice_mute_setting setting, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA3FD0, c_player_profile_interface, set_voice_mute_setting, setting, set_by_user);
}

void c_player_profile_interface::set_voice_output_setting(e_voice_output_setting setting, bool set_by_user)
{
	INVOKE_CLASS_MEMBER(0x00AA4020, c_player_profile_interface, set_voice_output_setting, setting, set_by_user);
}

void c_player_profile_interface::signed_out()
{
	INVOKE_CLASS_MEMBER(0x00AA4070, c_player_profile_interface, signed_out);

	//m_flags = 0;
	//m_settings_read_retry_count = 0;
	//m_next_settings_read_retry_time_milliseconds = 0;
	//initialize_to_default_settings();
}

//.text:00AA4090 ; public: void c_player_profile_interface::synchronization_with_storage_complete()
//.text:00AA40A0 ; 
//.text:00AA40D0 ; 
//.text:00AA4100 ; public: void c_player_profile_interface::update(e_controller_index)
//.text:00AA41A0 ; private: void c_player_profile_interface::update_service_tag()
//.text:00AA4320 ; 
//.text:00AA4330 ; 
//.text:00AA4340 ; 

