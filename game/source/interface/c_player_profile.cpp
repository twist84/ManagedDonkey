#include "interface/c_player_profile.hpp"

#include "memory/module.hpp"

REFERENCE_DECLARE(0x0191D4FC, bool, g_all_level_unlocked);

//HOOK_DECLARE_CLASS_MEMBER(0x00AA0320, c_player_profile_interface, dirty_or);
//HOOK_DECLARE_CLASS_MEMBER(0x00AA36B0, c_player_profile_interface, set_primary_change_color);
//HOOK_DECLARE_CLASS_MEMBER(0x00AA3860, c_player_profile_interface, set_secondary_change_color);

t_value_type<bool> const all_level_unlocked = { .value = false };
DATA_PATCH_DECLARE(0x0191D4FC, all_level_unlocked, all_level_unlocked.bytes);

//.text:00A9FEF0 ; 
//.text:00A9FF00 ; 
//.text:00A9FF40 ; 
//.text:00A9FF70 ; 
//.text:00A9FF90 ; public: __cdecl c_player_profile_interface::c_player_profile_interface()
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
//.text:00AA02A0 ; public: bool __cdecl c_player_profile_interface::get_map_completed_at_difficulty_level(e_campaign_game_mode, long) const

void __thiscall c_player_profile_interface::dirty_or(bool a1)
{
	INVOKE_CLASS_MEMBER(0x00AA0320, c_player_profile_interface, dirty_or, a1);

	//if (TEST_BIT(m_flags, 5) || a1)
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

//.text:00AA0380 ; private: static void __cdecl c_player_profile_interface::generate_random_service_tag(wchar_t* const)
//.text:00AA03E0 ; 
//.text:00AA03F0 ; public: e_button_presets __cdecl c_player_profile_interface::get_button_preset() const
//.text:00AA0400 ; public: e_camera_attached_horizontal __cdecl c_player_profile_interface::get_camera_attached_horizontal() const
//.text:00AA0410 ; public: e_camera_flying_movement __cdecl c_player_profile_interface::get_camera_flying_movement() const
//.text:00AA0420 ; public: e_camera_flying_thrust __cdecl c_player_profile_interface::get_camera_flying_thrust() const
//.text:00AA0430 ; public: e_camera_look_sensitivity __cdecl c_player_profile_interface::get_camera_look_sensitivity() const
//.text:00AA0440 ; public: e_camera_panning __cdecl c_player_profile_interface::get_camera_panning() const
//.text:00AA0450 ; public: long __cdecl c_player_profile_interface::get_campaign_awarded_primary_skull_bitvector(long*) const
//.text:00AA0470 ; public: long __cdecl c_player_profile_interface::get_campaign_awarded_secondary_skull_bitvector(long*) const
//.text:00AA0490 ; public: e_campaign_difficulty_level __cdecl c_player_profile_interface::get_campaign_difficulty() const
//.text:00AA04A0 ; 
//.text:00AA0500 ; 
//.text:00AA06F0 ; public: long __cdecl c_player_profile_interface::get_campaign_terminal_read_level1_bitvector() const
//.text:00AA0700 ; public: long __cdecl c_player_profile_interface::get_campaign_terminal_read_level2_bitvector() const
//.text:00AA0710 ; public: bool __cdecl c_player_profile_interface::get_controller_auto_center_look() const
//.text:00AA0720 ; public: bool __cdecl c_player_profile_interface::get_controller_crouch_lock_enabled() const
//.text:00AA0730 ; public: bool __cdecl c_player_profile_interface::get_controller_flight_stick_aircraft_controls() const
//.text:00AA0740 ; public: e_controller_index __cdecl c_player_profile_interface::get_controller_index() const
//.text:00AA0750 ; public: bool __cdecl c_player_profile_interface::get_controller_look_inverted() const
//.text:00AA0760 ; 
//.text:00AA0780 ; public: wchar_t const* __cdecl c_player_profile_interface::get_desired_service_tag() const
//.text:00AA0790 ; public: e_display_brightness __cdecl c_player_profile_interface::get_display_brightness() const
//.text:00AA07A0 ; public: bool __cdecl c_player_profile_interface::et_display_hints_enabled() const
//.text:00AA07B0 ; public: bool __cdecl c_player_profile_interface::get_female_voice_enabled() const
//.text:00AA07C0 ; public: bool __cdecl c_player_profile_interface::get_film_auto_save(e_film_gauto_save_type) const
//.text:00AA07E0 ; 
//.text:00AA0800 ; public: bool __cdecl c_player_profile_interface::get_guide_voice_muted() const
//.text:00AA0810 ; public: e_guide_voice_through_speakers __cdecl c_player_profile_interface::get_guide_voice_through_speakers() const
//.text:00AA0820 ; public: e_joystick_presets __cdecl c_player_profile_interface::get_joystick_preset() const
//.text:00AA0830 ; public: static e_language __cdecl c_player_profile_interface::get_language()
//.text:00AA0840 ; public: short __cdecl c_player_profile_interface::get_last_campaign_map_played_absolute_index() const
//.text:00AA0850 ; public: short __cdecl c_player_profile_interface::get_last_campaign_played_absolute_index() const
//.text:00AA0860 ; public: qword __cdecl c_player_profile_interface::get_last_campaign_played_time(e_campaign_game_mode) const
//.text:00AA0880 ; public: e_map_id __cdecl c_player_profile_interface::get_last_custom_multiplayer_map_played() const
//.text:00AA0890 ; public: word __cdecl c_player_profile_interface::get_last_network_game_hopper_played() const
//.text:00AA08A0 ; public: long __cdecl c_player_profile_interface::get_last_shown_popup_message_index(e_popup_message_title) const
//.text:00AA08C0 ; public: bool __cdecl c_player_profile_interface::get_last_variant_played(e_game_engine_type*, long*) const
//.text:00AA08F0 ; public: e_look_sensitivity __cdecl c_player_profile_interface::get_look_sensitivity() const
//.text:00AA0900 ; public: bool __cdecl c_player_profile_interface::get_map_completed_at_difficulty_level(e_campaign_game_mode, long, e_campaign_difficulty_level) const
//.text:00AA0960 ; public: char __cdecl c_player_profile_interface::get_model_customization_selection(e_player_model_choice, long) const
//.text:00AA0A00 ; public: char __cdecl c_player_profile_interface::get_model_customization_selection(e_player_model_choice, long) const
//.text:00AA0A30 ; public: void __cdecl c_player_profile_interface::get_player_appearance(s_player_appearance*)
//.text:00AA0AB0 ; public: s_emblem_info __cdecl c_player_profile_interface::get_emblem_info() const
//.text:00AA0AC0 ; public: e_player_model_choice __cdecl c_player_profile_interface::get_player_model_choice() const

e_player_color_index c_player_profile_interface::get_primary_change_color() const
{
	return INVOKE_CLASS_MEMBER(0x00AA0AE0, c_player_profile_interface, get_primary_change_color);

	return m_appearance.primary_color;
}

//.text:00AA0AF0 ; 
//.text:00AA0B00 ; 
//.text:00AA0B10 ; public: static e_region __cdecl c_player_profile_interface::get_region()

e_player_color_index c_player_profile_interface::get_secondary_change_color() const
{
	return INVOKE_CLASS_MEMBER(0x00AA0B20, c_player_profile_interface, get_secondary_change_color);

	//return m_appearance.secondary_color;
}

//.text:00AA0B30 ; public: bool __cdecl c_player_profile_interface::get_service_tag_failed_verification() const
//.text:00AA0B40 ; public: wchar_t const* __cdecl c_player_profile_interface::get_service_tag_validated() const
//.text:00AA0B50 ; public: bool __cdecl c_player_profile_interface::get_service_tag_was_randomly_generated() const
//.text:00AA0B60 ; 
//.text:00AA0B70 ; 
//.text:00AA0B80 ; 
//.text:00AA0B90 ; 
//.text:00AA0BA0 ; 
//.text:00AA0BB0 ; public: e_subtitle_setting __cdecl c_player_profile_interface::get_subtitle_setting() const
//.text:00AA0BC0 ; 
//.text:00AA0C40 ; 
//.text:00AA0C50 ; private: long __cdecl c_player_profile_interface::get_total_achievement_points() const
//.text:00AA0C90 ; private: long __cdecl c_player_profile_interface::get_total_skull_count() const
//.text:00AA0CE0 ; public: void __cdecl c_player_profile_interface::get_training_data(s_player_training_profile_data*) const
//.text:00AA0D20 ; public: bool __cdecl c_player_profile_interface::get_unlockable_unlocked(long) const
//.text:00AA0FF0 ; 
//.text:00AA1000 ; 
//.text:00AA1010 ; public: e_voice_mask __cdecl c_player_profile_interface::get_voice_mask() const
//.text:00AA1020 ; public: e_voice_matchmaking_setting __cdecl c_player_profile_interface::get_voice_matchmaking_setting() const
//.text:00AA1030 ; public: e_voice_mute_setting __cdecl c_player_profile_interface::get_voice_mute_setting() const
//.text:00AA1040 ; 
//.text:00AA1050 ; 
//.text:00AA1060 ; private: void __cdecl c_player_profile_interface::initialize_to_default_settings()
//.text:00AA18D0 ; 
//.text:00AA1930 ; 
//.text:00AA1990 ; public: static bool __cdecl c_player_profile_interface::is_service_tag_validly_formatted(wchar_t const*)
//.text:00AA1A60 ; 
//.text:00AA1AD0 ; 
//.text:00AA1B60 ; 
//.text:00AA1B70 ; 
//.text:00AA1B90 ; 
//.text:00AA1BB0 ; 
//.text:00AA1C70 ; public: void __cdecl c_player_profile_interface::player_campaign_progress_finished_map(short, bool)
//.text:00AA1C90 ; public: void __cdecl c_player_profile_interface::player_campaign_progress_set_insertion_point(short, short, bool, bool)
//.text:00AA1D50 ; public: void __cdecl c_player_profile_interface::player_campaign_progress_set_map(short, bool, bool)
//.text:00AA1E10 ; 
//.text:00AA1E60 ; public: bool __cdecl c_player_profile_interface::player_campaign_progress_test_map(short) const
//.text:00AA1EB0 ; 
//.text:00AA1F50 ; 
//.text:00AA1F80 ; 
//.text:00AA1F90 ; 
//.text:00AA1FD0 ; 
//.text:00AA2070 ; public: void __cdecl c_player_profile_interface::propagate_settings()
//.text:00AA23B0 ; 
//.text:00AA23C0 ; public: void __cdecl c_player_profile_insertion_point_flags<64, 12>::set(long, short, bool)
//.text:00AA2400 ; 
//.text:00AA2430 ; 
//.text:00AA2450 ; 
//.text:00AA2460 ; 
//.text:00AA2470 ; public: void __cdecl c_player_profile_interface::set_button_preset(e_button_presets, bool)
//.text:00AA24B0 ; public: void __cdecl c_player_profile_interface::set_camera_attached_horizontal(e_camera_attached_horizontal, bool)
//.text:00AA2500 ; public: void __cdecl c_player_profile_interface::set_camera_flying_movement(e_camera_flying_movement, bool)
//.text:00AA2550 ; public: void __cdecl c_player_profile_interface::set_camera_flying_thrust(e_camera_flying_thrust, bool)
//.text:00AA25A0 ; public: void __cdecl c_player_profile_interface::set_camera_look_sensitivity(e_camera_look_sensitivity, bool)
//.text:00AA25F0 ; public: void __cdecl c_player_profile_interface::set_camera_panning(e_camera_panning, bool)
//.text:00AA2640 ; public: void __cdecl c_player_profile_interface::set_campaign_awarded_primary_skull_bitvector(long, bool)
//.text:00AA26D0 ; public: void __cdecl c_player_profile_interface::set_campaign_awarded_secondary_skull_bitvector(long, bool)
//.text:00AA2760 ; public: void __cdecl c_player_profile_interface::set_campaign_difficulty(e_campaign_difficulty_level, bool)
//.text:00AA27B0 ; 
//.text:00AA28A0 ; public: void __cdecl c_player_profile_interface::set_campaign_terminal_read_level1_bitvector(short, bool)
//.text:00AA2930 ; public: void __cdecl c_player_profile_interface::set_campaign_terminal_read_level2_bitvector(short, bool)
//.text:00AA29C0 ; public: void __cdecl c_player_profile_interface::set_controller_auto_center_look(bool, bool)
//.text:00AA2A50 ; public: void __cdecl c_player_profile_interface::set_controller_crouch_lock_enabled(bool, bool)
//.text:00AA2AF0 ; public: void __cdecl c_player_profile_interface::set_controller_flight_stick_aircraft_controls(bool, bool)
//.text:00AA2B80 ; public: void __cdecl c_player_profile_interface::set_controller_look_inverted(bool, bool)
//.text:00AA2C00 ; 
//.text:00AA2C70 ; 
//.text:00AA2D60 ; 
//.text:00AA2E30 ; public: void __cdecl c_player_profile_interface::set_desired_service_tag(wchar_t const*, bool)
//.text:00AA2EE0 ; public: void __cdecl c_player_profile_interface::set_display_brightness(e_display_brightness, bool)
//.text:00AA2F00 ; public: void __cdecl c_player_profile_interface::set_display_hints_enabled(bool)
//.text:00AA2F80 ; public: void __cdecl c_player_profile_interface::set_female_voice_enabled(bool, bool)
//.text:00AA3010 ; public: void __cdecl c_player_profile_interface::set_film_auto_save(e_film_auto_save_type, bool, bool)
//.text:00AA3060 ; 
//.text:00AA3070 ; public: void __cdecl c_player_profile_interface::set_guide_voice_through_speakers(e_guide_voice_through_speakers)
//.text:00AA3090 ; public: void __cdecl c_player_profile_interface::set_joystick_preset(e_joystick_presets, bool)
//.text:00AA30D0 ; public: void __cdecl c_player_profile_interface::set_last_campaign_map_played_absolute_index(short, bool)
//.text:00AA3160 ; public: void __cdecl c_player_profile_interface::set_last_campaign_played_absolute_index(short, bool)
//.text:00AA31F0 ; public: void __cdecl c_player_profile_interface::set_last_campaign_played_time(e_campaign_game_mode, qword, bool)
//.text:00AA3230 ; public: void __cdecl c_player_profile_interface::set_last_custom_multiplayer_map_played(e_map_id, bool)
//.text:00AA32B0 ; public: void __cdecl c_player_profile_interface::set_last_network_game_hopper_played(word, bool)
//.text:00AA3340 ; public: void __cdecl c_player_profile_interface::set_last_shown_popup_message_index(e_popup_message_title, long, bool)
//.text:00AA33C0 ; public: void __cdecl c_player_profile_interface::set_last_variant_played(e_game_engine_type, long, bool)
//.text:00AA3410 ; public: void __cdecl c_player_profile_interface::set_look_sensitivity(e_look_sensitivity, bool)
//.text:00AA3450 ; public: void __cdecl c_player_profile_interface::set_map_completed_at_difficulty_level(e_campaign_game_mode, long, e_campaign_difficulty_level, bool, bool)
//.text:00AA34D0 ; public: void __cdecl c_player_profile_interface::set_model_customization_selection(e_player_model_choice, long, char, bool)
//.text:00AA3560 ; public: void __cdecl c_player_profile_interface::set_player_appearance(s_player_appearance const*, bool)
//.text:00AA35C0 ; 
//.text:00AA3620 ; public: void __cdecl c_player_profile_interface::set_player_model_choice(e_player_model_choice, bool)

//void c_player_profile_interface::set_primary_change_color(e_player_color_index primary_change_color, bool a2)
void __thiscall c_player_profile_interface::set_primary_change_color(long primary_change_color, bool a2)
{
	INVOKE_CLASS_MEMBER(0x00AA36B0, c_player_profile_interface, set_primary_change_color, primary_change_color, a2);

	//if (VALID_INDEX(primary_change_color, k_player_color_index_count))
	//{
	//	dirty_or(a2 && primary_change_color != get_primary_change_color());
	//	m_primary_change_color = primary_change_color;
	//}
	//else
	//{
	//	GENERATE_EVENT(_event_error, "ui:profile: invalid primary color index (#%ld)", primary_change_color);
	//}
}

//.text:00AA3740 ; 
//.text:00AA37D0 ; 

//void c_player_profile_interface::set_secondary_change_color(e_player_color_index secondary_change_color, bool a2)
void __thiscall c_player_profile_interface::set_secondary_change_color(long secondary_change_color, bool a2)
{
	INVOKE_CLASS_MEMBER(0x00AA3860, c_player_profile_interface, set_secondary_change_color, secondary_change_color, a2);

	//if (VALID_INDEX(secondary_change_color, k_player_color_index_count))
	//{
	//	dirty_or(a2 && secondary_change_color != get_primary_change_color());
	//	m_secondary_change_color = secondary_change_color;
	//}
	//else
	//{
	//	GENERATE_EVENT(_event_error, "ui:profile: invalid primary color index (#%ld)", secondary_change_color);
	//}
}

//.text:00AA38F0 ; public: void __cdecl c_player_profile_interface::set_service_tag_failed_verification(bool, bool)
//.text:00AA3980 ; public: void __cdecl c_player_profile_interface::set_service_tag_validated(wchar_t const*, bool)
//.text:00AA3A30 ; public: void __cdecl c_player_profile_interface::set_service_tag_was_randomly_generated(bool, bool)
//.text:00AA3AC0 ; 
//.text:00AA3B30 ; public: void __cdecl c_player_profile_interface::set_spartan_program_best_milestone(e_experience_milestone, bool)
//.text:00AA3B80 ; public: void __cdecl c_player_profile_interface::set_spartan_program_best_rank(long, bool)
//.text:00AA3C10 ; public: void __cdecl c_player_profile_interface::set_subtitle_setting(e_subtitle_setting, bool)
//.text:00AA3C30 ; 
//.text:00AA3D40 ; 
//.text:00AA3DD0 ; public: void __cdecl c_player_profile_interface::set_training_data(s_player_training_profile_data const*, bool)
//.text:00AA3EA0 ; 
//.text:00AA3F30 ; public: void __cdecl c_player_profile_interface::set_voice_mask(e_voice_mask, bool)
//.text:00AA3F80 ; public: void __cdecl c_player_profile_interface::set_voice_matchmaking_setting(e_voice_matchmaking_setting, bool)
//.text:00AA3FD0 ; public: void __cdecl c_player_profile_interface::set_voice_mute_setting(e_voice_mute_setting, bool)
//.text:00AA4020 ; public: e_voice_output_setting __cdecl c_player_profile_interface::get_voice_output_setting() const
//.text:00AA4070 ; public: void __cdecl c_player_profile_interface::signed_out()
//.text:00AA4090 ; public: void __cdecl c_player_profile_interface::synchronization_with_storage_complete()
//.text:00AA40A0 ; 
//.text:00AA40D0 ; 
//.text:00AA4100 ; public: void __cdecl c_player_profile_interface::update(e_controller_index)
//.text:00AA41A0 ; private: void __cdecl c_player_profile_interface::update_service_tag()
//.text:00AA4320 ; 
//.text:00AA4330 ; 
//.text:00AA4340 ; 

