#include "main/global_preferences.hpp"

s_global_preferences_internals_type* g_global_preferences = reinterpret_cast<s_global_preferences_internals_type*>(0x022C0128);

void __cdecl global_preferences_set_antialiasing(bool antialiasing)
{
	INVOKE(0x0050CCE0, global_preferences_set_antialiasing, antialiasing);
}

void __cdecl global_preferences_set_betrayal_count(float betrayal_count)
{
	INVOKE(0x0050CE00, global_preferences_set_betrayal_count, betrayal_count);
}

void __cdecl global_preferences_set_build_number(long build_number)
{
	INVOKE(0x0050CE90, global_preferences_set_build_number, build_number);
}

void __cdecl global_preferences_set_camera_fov(float camera_fov)
{
	INVOKE(0x0050CF20, global_preferences_set_camera_fov, camera_fov);
}

void __cdecl global_preferences_set_details_quality(e_quality_setting details_quality)
{
	INVOKE(0x0050D190, global_preferences_set_details_quality, details_quality);
}

void __cdecl global_preferences_set_brightness(long brightness)
{
	INVOKE(0x0050D220, global_preferences_set_brightness, brightness);
}

void __cdecl global_preferences_set_contrast(long contrast)
{
	INVOKE(0x0050D2B0, global_preferences_set_contrast, contrast);
}

void __cdecl global_preferences_set_effects_quality(e_quality_setting effects_quality)
{
	INVOKE(0x0050D3D0, global_preferences_set_effects_quality, effects_quality);
}

void __cdecl global_preferences_set_eviction_count(long eviction_count)
{
	INVOKE(0x0050D460, global_preferences_set_eviction_count, eviction_count);
}

void __cdecl global_preferences_set_fullscreen(bool fullscreen)
{
	INVOKE(0x0050D4F0, global_preferences_set_fullscreen, fullscreen);
}

void __cdecl global_preferences_set_watermark_hidden(bool watermark_hidden)
{
	INVOKE(0x0050D610, global_preferences_set_watermark_hidden, watermark_hidden);
}

void __cdecl global_preferences_set_hud_shake(bool hud_shake)
{
	INVOKE(0x0050D6A0, global_preferences_set_hud_shake, hud_shake);
}

void __cdecl global_preferences_set_controls_settings_keyboard_preset(long controls_settings_keyboard_preset)
{
	INVOKE(0x0050D730, global_preferences_set_controls_settings_keyboard_preset, controls_settings_keyboard_preset);
}

void __cdecl global_preferences_set_last_font_language(e_language last_font_language)
{
	INVOKE(0x0050D7C0, global_preferences_set_last_font_language, last_font_language);
}

void __cdecl global_preferences_set_last_fonts_modification_date(s_file_last_modification_date const* last_fonts_modification_date)
{
	INVOKE(0x0050D850, global_preferences_set_last_fonts_modification_date, last_fonts_modification_date);
}

void __cdecl global_preferences_set_last_game_setup(s_gui_game_setup_storage const* last_game_setup)
{
	INVOKE(0x0050D8E0, global_preferences_set_last_game_setup, last_game_setup);
}

void __cdecl global_preferences_set_last_language(e_language last_language)
{
	INVOKE(0x0050D980, global_preferences_set_last_language, last_language);
}

void __cdecl global_preferences_set_last_main_menu_item(long last_main_menu_item)
{
	INVOKE(0x0050DA10, global_preferences_set_last_main_menu_item, last_main_menu_item);
}

void __cdecl global_preferences_set_lighting_quality(e_quality_setting lighting_quality)
{
	INVOKE(0x0050DAA0, global_preferences_set_lighting_quality, lighting_quality);
}

void __cdecl global_preferences_set_master_volume(long master_volume)
{
	INVOKE(0x0050DB30, global_preferences_set_master_volume, master_volume);
}

void __cdecl global_preferences_set_motion_blur(bool motion_blur)
{
	INVOKE(0x0050DBC0, global_preferences_set_motion_blur, motion_blur);
}

void __cdecl global_preferences_set_mouse_acceleration(long mouse_acceleration)
{
	INVOKE(0x0050DC50, global_preferences_set_mouse_acceleration, mouse_acceleration);
}

void __cdecl global_preferences_set_mouse_filter(bool mouse_filter)
{
	INVOKE(0x0050DCE0, global_preferences_set_mouse_filter, mouse_filter);
}

void __cdecl global_preferences_set_mouse_invert(bool mouse_invert)
{
	INVOKE(0x0050DD70, global_preferences_set_mouse_invert, mouse_invert);
}

void __cdecl global_preferences_set_mouse_sensitivity_horizontal(long mouse_sensitivity_horizontal)
{
	INVOKE(0x0050DE00, global_preferences_set_mouse_sensitivity_horizontal, mouse_sensitivity_horizontal);
}

void __cdecl global_preferences_set_mouse_sensitivity_vehicle_horizontal(long mouse_sensitivity_vehicle_horizontal)
{
	INVOKE(0x0050DE90, global_preferences_set_mouse_sensitivity_vehicle_horizontal, mouse_sensitivity_vehicle_horizontal);
}

void __cdecl global_preferences_set_mouse_sensitivity_vehicle_vertical(long mouse_sensitivity_vehicle_vertical)
{
	INVOKE(0x0050DF20, global_preferences_set_mouse_sensitivity_vehicle_vertical, mouse_sensitivity_vehicle_vertical);
}

void __cdecl global_preferences_set_mouse_sensitivity_vertical(long mouse_sensitivity_vertical)
{
	INVOKE(0x0050DFB0, global_preferences_set_mouse_sensitivity_vertical, mouse_sensitivity_vertical);
}

void __cdecl global_preferences_set_music_volume(long music_volume)
{
	INVOKE(0x0050E040, global_preferences_set_music_volume, music_volume);
}

void __cdecl global_preferences_set_postprocessing_quality(e_quality_setting postprocessing_quality)
{
	INVOKE(0x0050E0D0, global_preferences_set_postprocessing_quality, postprocessing_quality);
}

void __cdecl global_preferences_set_content_item_unique_name_number_seed(e_content_item_type type, long seed)
{
	INVOKE(0x0050E160, global_preferences_set_content_item_unique_name_number_seed, type, seed);
}

void __cdecl global_preferences_set_screen_resolution(long width, long height)
{
	INVOKE(0x0050E1C0, global_preferences_set_screen_resolution, width, height);
}

void __cdecl global_preferences_set_sfx_volume(long sfx_volume)
{
	INVOKE(0x0050E260, global_preferences_set_sfx_volume, sfx_volume);
}

void __cdecl global_preferences_set_shadow_quality(e_quality_setting shadow_quality)
{
	INVOKE(0x0050E2F0, global_preferences_set_shadow_quality, shadow_quality);
}

void __cdecl global_preferences_set_subtitle_setting(e_subtitle_setting subtitle_setting)
{
	INVOKE(0x0050E380, global_preferences_set_subtitle_setting, subtitle_setting);
}

void __cdecl global_preferences_set_team_color(long team_color)
{
	INVOKE(0x0050E410, global_preferences_set_team_color, team_color);
}

void __cdecl global_preferences_set_texture_filtering_quality(e_quality_setting texture_filtering_quality)
{
	INVOKE(0x0050E4A0, global_preferences_set_texture_filtering_quality, texture_filtering_quality);
}

void __cdecl global_preferences_set_texture_resolution_quality(e_quality_setting texture_resolution_quality)
{
	INVOKE(0x0050E530, global_preferences_set_texture_resolution_quality, texture_resolution_quality);
}

void __cdecl global_preferences_set_toggle_crouch(bool toggle_crouch)
{
	INVOKE(0x0050E5C0, global_preferences_set_toggle_crouch, toggle_crouch);
}

void __cdecl global_preferences_set_adapter(long adapter)
{
	INVOKE(0x0050E650, global_preferences_set_adapter, adapter);
}

void __cdecl global_preferences_set_voice_chat_control(long voice_chat_control)
{
	INVOKE(0x0050E770, global_preferences_set_voice_chat_control, voice_chat_control);
}

void __cdecl global_preferences_set_voice_chat_volume(long voice_chat_volume)
{
	INVOKE(0x0050E800, global_preferences_set_voice_chat_volume, voice_chat_volume);
}

void __cdecl global_preferences_set_voice_volume(long voice_volume)
{
	INVOKE(0x0050E890, global_preferences_set_voice_volume, voice_volume);
}

void __cdecl global_preferences_set_vsync(bool vsync)
{
	INVOKE(0x0050E920, global_preferences_set_vsync, vsync);
}

void __cdecl global_preferences_set_xbox_live_private_privacy_setting(e_gui_network_session_advertisement_mode xbox_live_private_privacy_setting)
{
	INVOKE(0x0050E9B0, global_preferences_set_xbox_live_private_privacy_setting, xbox_live_private_privacy_setting);
}
