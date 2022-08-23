#include "main/global_preferences.hpp"

s_global_preferences_internals_type* g_global_preferences = reinterpret_cast<s_global_preferences_internals_type*>(0x022C0128);

void __cdecl global_preferences_set_antialiasing(bool antialiasing)
{
	reinterpret_cast<decltype(global_preferences_set_antialiasing)*>(0x0050CCE0)(antialiasing);
}

void __cdecl global_preferences_set_betrayal_count(float betrayal_count)
{
	reinterpret_cast<decltype(global_preferences_set_betrayal_count)*>(0x0050CE00)(betrayal_count);
}

void __cdecl global_preferences_set_build_number(long build_number)
{
	reinterpret_cast<decltype(global_preferences_set_build_number)*>(0x0050CE90)(build_number);
}

void __cdecl global_preferences_set_camera_fov(float camera_fov)
{
	reinterpret_cast<decltype(global_preferences_set_camera_fov)*>(0x0050CF20)(camera_fov);
}

void __cdecl global_preferences_set_details_quality(e_quality_setting details_quality)
{
	reinterpret_cast<decltype(global_preferences_set_details_quality)*>(0x0050D190)(details_quality);
}

void __cdecl global_preferences_set_brightness(long brightness)
{
	reinterpret_cast<decltype(global_preferences_set_brightness)*>(0x0050D220)(brightness);
}

void __cdecl global_preferences_set_contrast(long contrast)
{
	reinterpret_cast<decltype(global_preferences_set_contrast)*>(0x0050D2B0)(contrast);
}

void __cdecl global_preferences_set_effects_quality(e_quality_setting effects_quality)
{
	reinterpret_cast<decltype(global_preferences_set_effects_quality)*>(0x0050D3D0)(effects_quality);
}

void __cdecl global_preferences_set_eviction_count(long eviction_count)
{
	reinterpret_cast<decltype(global_preferences_set_eviction_count)*>(0x0050D460)(eviction_count);
}

void __cdecl global_preferences_set_fullscreen(bool fullscreen)
{
	reinterpret_cast<decltype(global_preferences_set_fullscreen)*>(0x0050D4F0)(fullscreen);
}

void __cdecl global_preferences_set_watermark_hidden(bool watermark_hidden)
{
	reinterpret_cast<decltype(global_preferences_set_watermark_hidden)*>(0x0050D610)(watermark_hidden);
}

void __cdecl global_preferences_set_hud_shake(bool hud_shake)
{
	reinterpret_cast<decltype(global_preferences_set_hud_shake)*>(0x0050D6A0)(hud_shake);
}

void __cdecl global_preferences_set_controls_settings_keyboard_preset(long controls_settings_keyboard_preset)
{
	reinterpret_cast<decltype(global_preferences_set_controls_settings_keyboard_preset)*>(0x0050D730)(controls_settings_keyboard_preset);
}

void __cdecl global_preferences_set_last_font_language(e_language last_font_language)
{
	reinterpret_cast<decltype(global_preferences_set_last_font_language)*>(0x0050D7C0)(last_font_language);
}

void __cdecl global_preferences_set_last_fonts_modification_date(s_file_last_modification_date const* last_fonts_modification_date)
{
	reinterpret_cast<decltype(global_preferences_set_last_fonts_modification_date)*>(0x0050D850)(last_fonts_modification_date);
}

void __cdecl global_preferences_set_last_game_setup(s_gui_game_setup_storage const* last_game_setup)
{
	reinterpret_cast<decltype(global_preferences_set_last_game_setup)*>(0x0050D8E0)(last_game_setup);
}

void __cdecl global_preferences_set_last_language(e_language last_language)
{
	reinterpret_cast<decltype(global_preferences_set_last_language)*>(0x0050D980)(last_language);
}

void __cdecl global_preferences_set_last_main_menu_item(long last_main_menu_item)
{
	reinterpret_cast<decltype(global_preferences_set_last_main_menu_item)*>(0x0050DA10)(last_main_menu_item);
}

void __cdecl global_preferences_set_lighting_quality(e_quality_setting lighting_quality)
{
	reinterpret_cast<decltype(global_preferences_set_lighting_quality)*>(0x0050DAA0)(lighting_quality);
}

void __cdecl global_preferences_set_master_volume(long master_volume)
{
	reinterpret_cast<decltype(global_preferences_set_master_volume)*>(0x0050DB30)(master_volume);
}

void __cdecl global_preferences_set_motion_blur(bool motion_blur)
{
	reinterpret_cast<decltype(global_preferences_set_motion_blur)*>(0x0050DBC0)(motion_blur);
}

void __cdecl global_preferences_set_mouse_acceleration(long mouse_acceleration)
{
	reinterpret_cast<decltype(global_preferences_set_mouse_acceleration)*>(0x0050DC50)(mouse_acceleration);
}

void __cdecl global_preferences_set_mouse_filter(bool mouse_filter)
{
	reinterpret_cast<decltype(global_preferences_set_mouse_filter)*>(0x0050DCE0)(mouse_filter);
}

void __cdecl global_preferences_set_mouse_invert(bool mouse_invert)
{
	reinterpret_cast<decltype(global_preferences_set_mouse_invert)*>(0x0050DD70)(mouse_invert);
}

void __cdecl global_preferences_set_mouse_sensitivity_horizontal(long mouse_sensitivity_horizontal)
{
	reinterpret_cast<decltype(global_preferences_set_mouse_sensitivity_horizontal)*>(0x0050DE00)(mouse_sensitivity_horizontal);
}

void __cdecl global_preferences_set_mouse_sensitivity_vehicle_horizontal(long mouse_sensitivity_vehicle_horizontal)
{
	reinterpret_cast<decltype(global_preferences_set_mouse_sensitivity_vehicle_horizontal)*>(0x0050DE90)(mouse_sensitivity_vehicle_horizontal);
}

void __cdecl global_preferences_set_mouse_sensitivity_vehicle_vertical(long mouse_sensitivity_vehicle_vertical)
{
	reinterpret_cast<decltype(global_preferences_set_mouse_sensitivity_vehicle_vertical)*>(0x0050DF20)(mouse_sensitivity_vehicle_vertical);
}

void __cdecl global_preferences_set_mouse_sensitivity_vertical(long mouse_sensitivity_vertical)
{
	reinterpret_cast<decltype(global_preferences_set_mouse_sensitivity_vertical)*>(0x0050DFB0)(mouse_sensitivity_vertical);
}

void __cdecl global_preferences_set_music_volume(long music_volume)
{
	reinterpret_cast<decltype(global_preferences_set_music_volume)*>(0x0050E040)(music_volume);
}

void __cdecl global_preferences_set_postprocessing_quality(e_quality_setting postprocessing_quality)
{
	reinterpret_cast<decltype(global_preferences_set_postprocessing_quality)*>(0x0050E0D0)(postprocessing_quality);
}

void __cdecl global_preferences_set_content_item_unique_name_number_seed(e_content_item_type type, long seed)
{
	reinterpret_cast<decltype(global_preferences_set_content_item_unique_name_number_seed)*>(0x0050E160)(type, seed);
}

void __cdecl global_preferences_set_screen_resolution(long width, long height)
{
	reinterpret_cast<decltype(global_preferences_set_screen_resolution)*>(0x0050E1C0)(width, height);
}

void __cdecl global_preferences_set_sfx_volume(long sfx_volume)
{
	reinterpret_cast<decltype(global_preferences_set_sfx_volume)*>(0x0050E260)(sfx_volume);
}

void __cdecl global_preferences_set_shadow_quality(e_quality_setting shadow_quality)
{
	reinterpret_cast<decltype(global_preferences_set_shadow_quality)*>(0x0050E2F0)(shadow_quality);
}

void __cdecl global_preferences_set_subtitle_setting(e_subtitle_setting subtitle_setting)
{
	reinterpret_cast<decltype(global_preferences_set_subtitle_setting)*>(0x0050E380)(subtitle_setting);
}

void __cdecl global_preferences_set_team_color(long team_color)
{
	reinterpret_cast<decltype(global_preferences_set_team_color)*>(0x0050E410)(team_color);
}

void __cdecl global_preferences_set_texture_filtering_quality(e_quality_setting texture_filtering_quality)
{
	reinterpret_cast<decltype(global_preferences_set_texture_filtering_quality)*>(0x0050E4A0)(texture_filtering_quality);
}

void __cdecl global_preferences_set_texture_resolution_quality(e_quality_setting texture_resolution_quality)
{
	reinterpret_cast<decltype(global_preferences_set_texture_resolution_quality)*>(0x0050E530)(texture_resolution_quality);
}

void __cdecl global_preferences_set_toggle_crouch(bool toggle_crouch)
{
	reinterpret_cast<decltype(global_preferences_set_toggle_crouch)*>(0x0050E5C0)(toggle_crouch);
}

void __cdecl global_preferences_set_adapter(long adapter)
{
	reinterpret_cast<decltype(global_preferences_set_adapter)*>(0x0050E650)(adapter);
}

void __cdecl global_preferences_set_voice_chat_control(long voice_chat_control)
{
	reinterpret_cast<decltype(global_preferences_set_voice_chat_control)*>(0x0050E770)(voice_chat_control);
}

void __cdecl global_preferences_set_voice_chat_volume(long voice_chat_volume)
{
	reinterpret_cast<decltype(global_preferences_set_voice_chat_volume)*>(0x0050E800)(voice_chat_volume);
}

void __cdecl global_preferences_set_voice_volume(long voice_volume)
{
	reinterpret_cast<decltype(global_preferences_set_voice_volume)*>(0x0050E890)(voice_volume);
}

void __cdecl global_preferences_set_vsync(bool vsync)
{
	reinterpret_cast<decltype(global_preferences_set_vsync)*>(0x0050E920)(vsync);
}

void __cdecl global_preferences_set_xbox_live_private_privacy_setting(e_gui_network_session_advertisement_mode xbox_live_private_privacy_setting)
{
	reinterpret_cast<decltype(global_preferences_set_xbox_live_private_privacy_setting)*>(0x0050E9B0)(xbox_live_private_privacy_setting);
}
