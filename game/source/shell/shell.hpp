#pragma once

#define BOT_CLIENT(true_false) if (game_is_bot_client() == true_false)

extern bool __cdecl shell_application_is_paused();
extern void __cdecl shell_application_pause(bool pause);
extern e_shell_application_type __cdecl shell_application_type();
extern bool __cdecl shell_build_number_is_compatible(int32 build_number);
extern bool __cdecl shell_build_string_is_compatible(const char* build_string);
extern void __cdecl shell_dispose();
extern const char* __cdecl shell_get_target();
extern const char* __cdecl shell_get_target_variant();
extern void __cdecl shell_halt_on_pure_virtual_call();
extern void __cdecl shell_halt_with_message(const char* message);
extern bool __cdecl shell_initialize(bool windowed);
extern bool __cdecl sub_42E5C0();
extern bool __cdecl sub_42E5D0();
extern bool __cdecl sub_42E5E0();
extern bool __cdecl game_is_bot_client();
extern bool __cdecl game_is_dedicated_server();
extern bool __cdecl game_is_client();
extern bool __cdecl game_is_guerilla();
extern bool __cdecl game_is_halo3();
extern bool __cdecl game_is_sapien();
extern bool __cdecl game_is_tool();
extern e_shell_tool_type __cdecl shell_tool_type();

extern const char* quality_setting_get_name(e_quality_setting quality_setting);
extern e_quality_setting quality_setting_from_string(const char* str);
extern const char* subtitle_setting_get_name(e_subtitle_setting subtitle_setting);
extern e_subtitle_setting subtitle_setting_from_string(const char* str);
extern const char* campaign_difficulty_level_get_name(e_campaign_difficulty_level difficulty);
extern e_campaign_difficulty_level campaign_difficulty_level_from_string(const char* str);
extern const char* network_session_mode_get_name(int32 session_mode);
extern const char* ui_game_mode_get_name(int32 ui_game_mode);
extern const char* gui_network_session_advertisement_mode_get_name(int32 advertisement_mode);
extern const char* game_variant_parameter_get_name(int32 parameter);

extern const char* const k_network_session_mode_names[k_network_session_mode_count];
extern const char* const k_ui_game_mode_names[k_gui_game_setup_mode_count];
extern const char* const k_gui_network_session_advertisement_mode_names[k_network_session_advertisement_mode_count];
extern const char* const k_game_variant_parameter_names[k_number_of_editable_game_variant_parameters];
extern const char* const global_campaign_difficulty_level_names[k_campaign_difficulty_levels_count];
extern const char* const global_campaign_team_names[k_campaign_team_count];
extern const char* const global_multiplayer_team_names[k_multiplayer_team_count];
extern const char* const global_controller_index_names[k_number_of_controllers];
extern const char* const global_button_preset_names[k_button_presets_count];
extern const char* const global_joystick_preset_names[k_joystick_presets_count];
extern const char* const global_player_model_choice_names[k_player_model_choice_count];
extern const char* const global_voice_output_setting_names[k_number_of_voice_output_settings];
extern const char* const global_voice_mask_names[k_number_of_voice_masks];
extern const char* const global_subtitle_setting_names[k_number_of_subtitle_settings];
extern const char* const global_actor_type_names[k_number_of_actor_types];
extern const char* const global_model_state_names[k_number_of_model_states];
extern const char* const global_character_physics_override_names[k_total_character_physics_overrides];
extern const char* const global_primary_skull_names[k_campaign_skull_primary_count];
extern const char* const global_secondary_skull_names[k_campaign_skull_secondary_count];

