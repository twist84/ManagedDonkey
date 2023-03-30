#include "main/global_preferences.hpp"

#include "cseries/console.hpp"
#include "memory/thread_local.hpp"
#include "multithreading/threads.hpp"

s_global_preferences_internals_type* g_global_preferences = reinterpret_cast<s_global_preferences_internals_type*>(0x022C0128);

c_global_preferences_scope_lock::c_global_preferences_scope_lock() :
	m_took_lock(0)
{
	DECLFUNC(0x00509F60, void, __thiscall, bool*)(&m_took_lock);

	//m_took_lock = 0;
	//lock_global_preferences(&m_took_lock);
}

c_global_preferences_scope_lock::~c_global_preferences_scope_lock()
{
	DECLFUNC(0x0050A150, void, __thiscall, bool*)(&m_took_lock);

	//unlock_global_preferences(m_took_lock);
}

s_global_preferences_internals_type* global_preferences_get()
{
	return get_tls()->g_global_preferences;
}

bool __cdecl global_preferences_available()
{
	//return INVOKE(0x0050A420, global_preferences_available);

	return !is_async_thread();
}

void __cdecl global_preferences_clear()
{
	INVOKE(0x0050A430, global_preferences_clear);
}

void __cdecl global_preferences_create_default()
{
	INVOKE(0x0050A520, global_preferences_create_default);
}

void __cdecl global_preferences_dispose()
{
	INVOKE(0x0050A700, global_preferences_dispose);
}

void __cdecl global_preferences_flush()
{
	INVOKE(0x0050A710, global_preferences_flush);
}

bool __cdecl global_preferences_get_antialiasing()
{
	return INVOKE(0x0050A830, global_preferences_get_antialiasing);
}

long __cdecl global_preferences_get_unknown41BD0()
{
	return INVOKE(0x0050A8B0, global_preferences_get_unknown41BD0);
}

real __cdecl global_preferences_get_betrayal_count()
{
	return INVOKE(0x0050A930, global_preferences_get_betrayal_count);
}

long __cdecl global_preferences_get_build_number()
{
	return INVOKE(0x0050A9B0, global_preferences_get_build_number);
}

real __cdecl global_preferences_get_camera_fov()
{
	return INVOKE(0x0050AA30, global_preferences_get_camera_fov);
}

long __cdecl global_preferences_get_controls_method()
{
	return INVOKE(0x0050AAC0, global_preferences_get_controls_method);
}

s_global_preferences_data::s_control_settings::s_bindings* __cdecl global_preferences_get_bindings(s_global_preferences_data::s_control_settings::s_bindings* bindings)
{
	return INVOKE(0x0050AB40, global_preferences_get_bindings, bindings);
}

void __cdecl global_preferences_bindings_create_default(s_global_preferences_data::s_control_settings::s_bindings* bindings)
{
	INVOKE(0x0050AC10, global_preferences_get_bindings, bindings);
}

void __cdecl global_preferences_gameplay_settings_create_default(s_global_preferences_data::s_gameplay_settings* gameplay_settings)
{
	INVOKE(0x0050AC40, global_preferences_gameplay_settings_create_default, gameplay_settings);
}

void __cdecl global_preferences_control_settings_create_default(s_global_preferences_data::s_control_settings* control_settings)
{
	INVOKE(0x0050AC70, global_preferences_control_settings_create_default, control_settings);
}

void __cdecl global_preferences_sound_settings_create_default(s_global_preferences_data::s_sound_settings* sound_settings)
{
	INVOKE(0x0050ACF0, global_preferences_sound_settings_create_default, sound_settings);
}

//.text:0050AD30 ; void __cdecl global_preferences_video_settings_create_default

e_quality_setting __cdecl global_preferences_get_details_quality()
{
	return INVOKE(0x0050AF50, global_preferences_get_details_quality);
}

long __cdecl global_preferences_get_brightness()
{
	return INVOKE(0x0050B050, global_preferences_get_brightness);
}

bool __cdecl global_preferences_get_display_hints()
{
	return INVOKE(0x0050B0D0, global_preferences_get_display_hints);
}

e_quality_setting __cdecl global_preferences_get_effects_quality()
{
	return INVOKE(0x0050B150, global_preferences_get_effects_quality);
}

long __cdecl global_preferences_get_eviction_count()
{
	return INVOKE(0x0050B1D0, global_preferences_get_eviction_count);
}

bool __cdecl global_preferences_get_fullscreen()
{
	return INVOKE(0x0050B250, global_preferences_get_fullscreen);
}

dword __cdecl global_preferences_get_checksum()
{
	return INVOKE(0x0050B2D0, global_preferences_get_checksum);
}

bool __cdecl global_preferences_get_hud_shake()
{
	return INVOKE(0x0050B350, global_preferences_get_hud_shake);
}

long __cdecl global_preferences_get_keyboard_preset()
{
	return INVOKE(0x0050B3D0, global_preferences_get_keyboard_preset);
}

e_language __cdecl global_preferences_get_last_font_language()
{
	return INVOKE(0x0050B450, global_preferences_get_last_font_language);
}

void __cdecl global_preferences_get_last_fonts_modification_date(s_file_last_modification_date* last_fonts_modification_date)
{
	INVOKE(0x0050B4D0, global_preferences_get_last_fonts_modification_date, last_fonts_modification_date);
}

s_gui_game_setup_storage* __cdecl global_preferences_get_last_game_setup()
{
	return INVOKE(0x0050B560, global_preferences_get_last_game_setup);
}

//bool __cdecl global_preferences_get_last_game_setup_map(e_gui_game_mode, e_campaign_id*, e_map_id*)
bool __cdecl global_preferences_get_last_game_setup_map(e_gui_game_mode game_mode, long* campaign_id, long* map_id)
{
	return INVOKE(0x0050B350, global_preferences_get_hud_shake);
}

e_language __cdecl global_preferences_get_last_language()
{
	return INVOKE(0x0050B790, global_preferences_get_last_language);
}

e_gui_game_mode __cdecl global_preferences_get_last_main_menu_item()
{
	return INVOKE(0x0050B810, global_preferences_get_last_main_menu_item);
}

e_quality_setting __cdecl global_preferences_get_lighting_quality()
{
	return INVOKE(0x0050B890, global_preferences_get_lighting_quality);
}

long __cdecl global_preferences_get_master_volume()
{
	return INVOKE(0x0050B910, global_preferences_get_master_volume);
}

bool __cdecl global_preferences_get_motion_blur()
{
	return INVOKE(0x0050B990, global_preferences_get_motion_blur);
}

long __cdecl global_preferences_get_mouse_acceleration()
{
	return INVOKE(0x0050BA10, global_preferences_get_mouse_acceleration);
}

bool __cdecl global_preferences_get_mouse_filter()
{
	return INVOKE(0x0050BA90, global_preferences_get_mouse_filter);
}

bool __cdecl global_preferences_get_mouse_inversion()
{
	return INVOKE(0x0050BB10, global_preferences_get_mouse_inversion);
}

long __cdecl global_preferences_get_mouse_sensitivity_horizontal()
{
	return INVOKE(0x0050BB90, global_preferences_get_mouse_sensitivity_horizontal);
}

long __cdecl global_preferences_get_mouse_sensitivity_vehicle_horizontal()
{
	return INVOKE(0x0050BC10, global_preferences_get_mouse_sensitivity_vehicle_horizontal);
}

long __cdecl global_preferences_get_mouse_sensitivity_vehicle_vertical()
{
	return INVOKE(0x0050BC90, global_preferences_get_mouse_sensitivity_vehicle_vertical);
}

long __cdecl global_preferences_get_mouse_sensitivity_vertical()
{
	return INVOKE(0x0050BD10, global_preferences_get_mouse_sensitivity_vertical);
}

long __cdecl global_preferences_get_music_volume()
{
	return INVOKE(0x0050BD90, global_preferences_get_music_volume);
}

e_quality_setting __cdecl global_preferences_get_postprocessing_quality()
{
	return INVOKE(0x0050BE10, global_preferences_get_postprocessing_quality);
}

//long __cdecl global_preferences_get_content_item_unique_name_number_seed(e_content_item_type)
long __cdecl global_preferences_get_content_item_unique_name_number_seed(long content_item_type)
{
	return INVOKE(0x0050BFA0, global_preferences_get_content_item_unique_name_number_seed, content_item_type);
}

void __cdecl global_preferences_get_screen_resolution(long* width, long* height)
{
	INVOKE(0x0050C030, global_preferences_get_screen_resolution, width, height);
}

long __cdecl global_preferences_get_sfx_volume()
{
	return INVOKE(0x0050C0D0, global_preferences_get_sfx_volume);
}

e_quality_setting __cdecl global_preferences_get_shadow_quality()
{
	return INVOKE(0x0050C150, global_preferences_get_shadow_quality);
}

e_subtitle_setting __cdecl global_preferences_get_subtitle_setting()
{
	return INVOKE(0x0050C1D0, global_preferences_get_subtitle_setting);
}

long __cdecl global_preferences_get_team_color()
{
	return INVOKE(0x0050C250, global_preferences_get_team_color);
}

e_quality_setting __cdecl global_preferences_get_texture_filtering_quality()
{
	return INVOKE(0x0050C2D0, global_preferences_get_texture_filtering_quality);
}

e_quality_setting __cdecl global_preferences_get_texture_resolution_quality()
{
	return INVOKE(0x0050C350, global_preferences_get_texture_resolution_quality);
}

bool __cdecl global_preferences_get_toggle_crouch()
{
	return INVOKE(0x0050C3D0, global_preferences_get_toggle_crouch);
}

long __cdecl global_preferences_get_adapter()
{
	return INVOKE(0x0050C450, global_preferences_get_adapter);
}

long __cdecl global_preferences_get_unknown41BDC()
{
	return INVOKE(0x0050C4D0, global_preferences_get_unknown41BDC);
}

long __cdecl global_preferences_get_voice_chat_control()
{
	return INVOKE(0x0050C550, global_preferences_get_voice_chat_control);
}

long __cdecl global_preferences_get_voice_chat_volume()
{
	return INVOKE(0x0050C5D0, global_preferences_get_voice_chat_volume);
}

long __cdecl global_preferences_get_voice_volume()
{
	return INVOKE(0x0050C650, global_preferences_get_voice_volume);
}

bool __cdecl global_preferences_get_vsync()
{
	return INVOKE(0x0050C6D0, global_preferences_get_vsync);
}

e_gui_network_session_advertisement_mode __cdecl global_preferences_get_xbox_live_private_privacy_setting()
{
	return INVOKE(0x0050C750, global_preferences_get_xbox_live_private_privacy_setting);
}

void __cdecl global_preferences_init()
{
	INVOKE(0x0050C830, global_preferences_init);
}

void __cdecl global_preferences_initialize()
{
	INVOKE(0x0050CAA0, global_preferences_initialize);
}

void __cdecl global_preferences_invalidate_maps()
{
	INVOKE(0x0050CBB0, global_preferences_invalidate_maps);
}

void __cdecl global_preferences_make_safe(s_global_preferences* preferences)
{
	INVOKE(0x0050CC30, global_preferences_make_safe, preferences);
}

void __cdecl global_preferences_set_antialiasing(bool antialiasing)
{
	INVOKE(0x0050CCE0, global_preferences_set_antialiasing, antialiasing);
}

void __cdecl global_preferences_set_unknown41BD0(bool unknown41BD0)
{
	INVOKE(0x0050CD70, global_preferences_set_unknown41BD0, unknown41BD0);
}

void __cdecl global_preferences_set_betrayal_count(real betrayal_count)
{
	INVOKE(0x0050CE00, global_preferences_set_betrayal_count, betrayal_count);
}

void __cdecl global_preferences_set_build_number(long build_number)
{
	INVOKE(0x0050CE90, global_preferences_set_build_number, build_number);
}

void __cdecl global_preferences_set_camera_fov(real camera_fov)
{
	INVOKE(0x0050CF20, global_preferences_set_camera_fov, camera_fov);
}

void __cdecl global_preferences_set_controls_method(long controls_method)
{
	INVOKE(0x0050CFB0, global_preferences_set_controls_method, controls_method);
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

void __cdecl global_preferences_set_display_hints(bool display_hints)
{
	INVOKE(0x0050D340, global_preferences_set_display_hints, display_hints);
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

void __cdecl global_preferences_set_checksum(dword checksum)
{
	INVOKE(0x0050D580, global_preferences_set_checksum, checksum);
}

void __cdecl global_preferences_set_hide_watermark(bool hide_watermark)
{
	INVOKE(0x0050D610, global_preferences_set_hide_watermark, hide_watermark);
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

void __cdecl global_preferences_set_mouse_inversion(bool mouse_inversion)
{
	INVOKE(0x0050DD70, global_preferences_set_mouse_inversion, mouse_inversion);
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

void __cdecl global_preferences_set_unknown41BDC(long unknown41BDC)
{
	INVOKE(0x0050E6E0, global_preferences_set_unknown41BDC, unknown41BDC);
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

bool __cdecl global_preferences_get_hide_watermark()
{
	return INVOKE(0x0050EA40, global_preferences_get_hide_watermark);
}

void __cdecl global_preferences_update()
{
	INVOKE(0x0050EAC0, global_preferences_update);
}

//.text:0050EBA0 ; void __cdecl c_gui_pregame_setup_manager::restore_game_setup(c_gui_single_game_setup_storage* storage)

void __cdecl global_preferences_write()
{
	INVOKE(0x0050ECE0, global_preferences_write);
}

