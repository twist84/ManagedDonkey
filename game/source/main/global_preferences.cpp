#include "main/global_preferences.hpp"

#include "interface/user_interface.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "multithreading/threads.hpp"
#include "rasterizer/rasterizer.hpp"
#include "shell/shell.hpp"

REFERENCE_DECLARE(0x022C0128, s_global_preferences_internals_type, g_global_preferences_internal);

HOOK_DECLARE(0x0050AA30, global_preferences_get_camera_fov);

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
	return g_global_preferences;
}

void __cdecl global_preferences_dirty(bool dirty)
{
	global_preferences_get()->dirty = dirty;
}

bool __cdecl global_preferences_are_dirty()
{
	return global_preferences_get()->dirty;
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
	//INVOKE(0x0050A700, global_preferences_dispose);
	 
	global_preferences_update();
}

void __cdecl global_preferences_flush()
{
	INVOKE(0x0050A710, global_preferences_flush);
}

bool __cdecl global_preferences_get_antialiasing()
{
	//return INVOKE(0x0050A830, global_preferences_get_antialiasing);

	if (!global_preferences_available())
		return false;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.video_settings.antialiasing;
}

int32 __cdecl global_preferences_get_aspect_ratio()
{
	//return INVOKE(0x0050A8B0, global_preferences_get_aspect_ratio);

	if (!global_preferences_available())
		return 0;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.aspect_ratio;
}

real32 __cdecl global_preferences_get_betrayal_count()
{
	//return INVOKE(0x0050A930, global_preferences_get_betrayal_count);

	if (!global_preferences_available())
		return 0.0f;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.betrayal_count;
}

int32 __cdecl global_preferences_get_build_number()
{
	//return INVOKE(0x0050A9B0, global_preferences_get_build_number);

	if (!global_preferences_available())
		return -1;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.build_number;
}

real32 __cdecl global_preferences_get_camera_fov()
{
	//return INVOKE(0x0050AA30, global_preferences_get_camera_fov);

	if (!global_preferences_available())
		return *reinterpret_cast<real32*>(0x0189D42C); // 90.0f

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.gameplay_settings.camera_fov;
}

int32 __cdecl global_preferences_get_controls_method()
{
	//return INVOKE(0x0050AAC0, global_preferences_get_controls_method);

	if (!global_preferences_available())
		return 0;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.controls_settings.controls_method;
}

s_keyboard_input_preferences* __cdecl global_preferences_get_keyboard_preferences(s_keyboard_input_preferences* preferences)
{
	return INVOKE(0x0050AB40, global_preferences_get_keyboard_preferences, preferences);
}

void __cdecl global_preferences_keyboard_preferences_create_default(s_keyboard_input_preferences* preferences)
{
	INVOKE(0x0050AC10, global_preferences_keyboard_preferences_create_default, preferences);
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
	//return INVOKE(0x0050AF50, global_preferences_get_details_quality);

	if (!global_preferences_available())
		return _quality_setting_medium;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.video_settings.details_quality;
}

int32 __cdecl global_preferences_get_display_brightness()
{
	//return INVOKE(0x0050B050, global_preferences_get_display_brightness);

	if (!global_preferences_available())
		return 50;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.brightness;
}

int32 __cdecl global_preferences_get_display_contrast()
{
	if (!global_preferences_available())
		return 50;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.contrast;
}

bool __cdecl global_preferences_get_display_hints()
{
	//return INVOKE(0x0050B0D0, global_preferences_get_display_hints);

	if (!global_preferences_available())
		return true;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.gameplay_settings.display_hints;
}

e_quality_setting __cdecl global_preferences_get_effects_quality()
{
	//return INVOKE(0x0050B150, global_preferences_get_effects_quality);

	if (!global_preferences_available())
		return _quality_setting_medium;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.video_settings.effects_quality;
}

int32 __cdecl global_preferences_get_eviction_count()
{
	//return INVOKE(0x0050B1D0, global_preferences_get_eviction_count);

	if (!global_preferences_available())
		return 0;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.eviction_count;
}

bool __cdecl global_preferences_get_fullscreen()
{
	//return INVOKE(0x0050B250, global_preferences_get_fullscreen);

	if (!global_preferences_available())
		return false;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.fullscreen;
}

uns32 __cdecl global_preferences_get_hardware_crc()
{
	//return INVOKE(0x0050B2D0, global_preferences_get_hardware_crc);

	if (!global_preferences_available())
		return 0;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.hardware_crc;
}

bool __cdecl global_preferences_get_hud_shake()
{
	//return INVOKE(0x0050B350, global_preferences_get_hud_shake);

	if (!global_preferences_available())
		return true;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.gameplay_settings.hud_shake;
}

int32 __cdecl global_preferences_get_keyboard_preset()
{
	//return INVOKE(0x0050B3D0, global_preferences_get_keyboard_preset);

	if (!global_preferences_available())
		return 0;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.controls_settings.keyboard_preset;
}

e_language __cdecl global_preferences_get_last_font_language()
{
	//return INVOKE(0x0050B450, global_preferences_get_last_font_language);

	if (!global_preferences_available())
		return _language_invalid;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.last_font_language;
}

void __cdecl global_preferences_get_last_fonts_modification_date(s_file_last_modification_date* last_fonts_modification_date)
{
	//INVOKE(0x0050B4D0, global_preferences_get_last_fonts_modification_date, last_fonts_modification_date);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	*last_fonts_modification_date = global_preferences_get()->current.data.last_fonts_modification_date;
}

s_gui_game_setup_storage* __cdecl global_preferences_get_last_game_setup()
{
	//return INVOKE(0x0050B560, global_preferences_get_last_game_setup);

	if (!global_preferences_available())
		return nullptr;

	c_global_preferences_scope_lock scope_lock;
	return &global_preferences_get()->current.data.last_game_setup;
}

//bool __cdecl global_preferences_get_last_game_setup_map(e_gui_game_mode, e_campaign_id*, e_map_id*)
bool __cdecl global_preferences_get_last_game_setup_map(e_gui_game_mode game_mode, int32* campaign_id, int32* map_id)
{
	//return INVOKE(0x0050B5E0, global_preferences_get_last_game_setup_map, game_mode, campaign_id, map_id);

	if (!global_preferences_available())
		return false;

	c_global_preferences_scope_lock scope_lock;
	switch (game_mode)
	{
	case _ui_game_mode_campaign:
		if (global_preferences_get()->current.data.last_game_setup.get_campaign()->is_valid())
		{
			*campaign_id = global_preferences_get()->current.data.last_game_setup.get_campaign()->campaign_id;
			*map_id = global_preferences_get()->current.data.last_game_setup.get_campaign()->map_id;
			return true;
		}
		break;
	case _ui_game_mode_matchmaking:
		break;
	case _ui_game_mode_multiplayer:
		if (global_preferences_get()->current.data.last_game_setup.get_multiplayer()->is_valid())
		{
			*campaign_id = NONE;
			*map_id = global_preferences_get()->current.data.last_game_setup.get_multiplayer()->map_variant_settings.get_variant()->get_map_id();
			return true;
		}
		break;
	case _ui_game_mode_map_editor:
		if (global_preferences_get()->current.data.last_game_setup.get_mapeditor()->is_valid())
		{
			*campaign_id = NONE;
			*map_id = global_preferences_get()->current.data.last_game_setup.get_mapeditor()->map_variant_settings.get_variant()->get_map_id();
			return true;
		}
		break;
	case _ui_game_mode_theater:
		if (global_preferences_get()->current.data.last_game_setup.get_theater()->is_valid())
		{
			*campaign_id = global_preferences_get()->current.data.last_game_setup.get_theater()->film_description.campaign_id;
			*map_id = global_preferences_get()->current.data.last_game_setup.get_theater()->film_description.map_id;
			return true;
		}
		break;
	case _ui_game_mode_survival:
		if (global_preferences_get()->current.data.last_game_setup.get_survival()->is_valid())
		{
			*campaign_id = global_preferences_get()->current.data.last_game_setup.get_survival()->campaign_id;
			*map_id = global_preferences_get()->current.data.last_game_setup.get_survival()->map_id;
			return true;
		}
		break;
	}

	return false;
}

e_language __cdecl global_preferences_get_last_language()
{
	//return INVOKE(0x0050B790, global_preferences_get_last_language);

	if (!global_preferences_available())
		return _language_invalid;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.last_language;
}

e_gui_game_mode __cdecl global_preferences_get_last_main_menu_item()
{
	//return INVOKE(0x0050B810, global_preferences_get_last_main_menu_item);

	if (!global_preferences_available())
		return _ui_game_mode_campaign;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.last_main_menu_item;
}

e_quality_setting __cdecl global_preferences_get_lighting_quality()
{
	//return INVOKE(0x0050B890, global_preferences_get_lighting_quality);

	if (!global_preferences_available())
		return _quality_setting_medium;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.video_settings.lighting_quality;
}

int32 __cdecl global_preferences_get_master_volume()
{
	//return INVOKE(0x0050B910, global_preferences_get_master_volume);

	if (!global_preferences_available())
		return 100;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.sound_settings.master_volume;
}

bool __cdecl global_preferences_get_motion_blur()
{
	//return INVOKE(0x0050B990, global_preferences_get_motion_blur);

	if (!global_preferences_available())
		return false;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.video_settings.motion_blur;
}

int32 __cdecl global_preferences_get_mouse_acceleration()
{
	//return INVOKE(0x0050BA10, global_preferences_get_mouse_acceleration);

	if (!global_preferences_available())
		return 50;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.controls_settings.mouse_acceleration;
}

bool __cdecl global_preferences_get_mouse_filter()
{
	//return INVOKE(0x0050BA90, global_preferences_get_mouse_filter);

	if (!global_preferences_available())
		return true;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.controls_settings.mouse_filter;
}

bool __cdecl global_preferences_get_mouse_inversion()
{
	//return INVOKE(0x0050BB10, global_preferences_get_mouse_inversion);

	if (!global_preferences_available())
		return false;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.controls_settings.mouse_inversion;
}

int32 __cdecl global_preferences_get_mouse_sensitivity_horizontal()
{
	//return INVOKE(0x0050BB90, global_preferences_get_mouse_sensitivity_horizontal);

	if (!global_preferences_available())
		return 50;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.controls_settings.mouse_sensitivity_horizontal;
}

int32 __cdecl global_preferences_get_mouse_sensitivity_vehicle_horizontal()
{
	//return INVOKE(0x0050BC10, global_preferences_get_mouse_sensitivity_vehicle_horizontal);

	if (!global_preferences_available())
		return 50;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.controls_settings.mouse_sensitivity_vehicle_horizontal;
}

int32 __cdecl global_preferences_get_mouse_sensitivity_vehicle_vertical()
{
	//return INVOKE(0x0050BC90, global_preferences_get_mouse_sensitivity_vehicle_vertical);

	if (!global_preferences_available())
		return 50;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.controls_settings.mouse_sensitivity_vehicle_vertical;
}

int32 __cdecl global_preferences_get_mouse_sensitivity_vertical()
{
	//return INVOKE(0x0050BD10, global_preferences_get_mouse_sensitivity_vertical);

	if (!global_preferences_available())
		return 50;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.controls_settings.mouse_sensitivity_vertical;
}

int32 __cdecl global_preferences_get_music_volume()
{
	//return INVOKE(0x0050BD90, global_preferences_get_music_volume);

	if (!global_preferences_available())
		return 100;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.sound_settings.music_volume;
}

e_quality_setting __cdecl global_preferences_get_postprocessing_quality()
{
	//return INVOKE(0x0050BE10, global_preferences_get_postprocessing_quality);

	if (!global_preferences_available())
		return _quality_setting_medium;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.video_settings.postprocessing_quality;
}

//int32 __cdecl global_preferences_get_content_item_unique_name_number_seed(e_content_item_type)
int32 __cdecl global_preferences_get_content_item_unique_name_number_seed(int32 content_item_type)
{
	//return INVOKE(0x0050BFA0, global_preferences_get_content_item_unique_name_number_seed, content_item_type);

	if (!global_preferences_available() || !VALID_INDEX(content_item_type, 14))
		return 0;

	c_global_preferences_scope_lock scope_lock;

	return global_preferences_get()->current.data.content_item_unique_name_number_seed[content_item_type];
}

void __cdecl global_preferences_get_screen_resolution(int32* width, int32* height)
{
	//INVOKE(0x0050C030, global_preferences_get_screen_resolution, width, height);

	*width = NONE;
	*height = NONE;

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;

	*width = global_preferences_get()->current.data.video_settings.width;
	*height = global_preferences_get()->current.data.video_settings.height;
}

int32 __cdecl global_preferences_get_sfx_volume()
{
	//return INVOKE(0x0050C0D0, global_preferences_get_sfx_volume);

	if (!global_preferences_available())
		return 100;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.sound_settings.sfx_volume;
}

e_quality_setting __cdecl global_preferences_get_shadow_quality()
{
	//return INVOKE(0x0050C150, global_preferences_get_shadow_quality);

	if (!global_preferences_available())
		return _quality_setting_medium;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.video_settings.shadow_quality;
}

e_subtitle_setting __cdecl global_preferences_get_subtitle_setting()
{
	//return INVOKE(0x0050C1D0, global_preferences_get_subtitle_setting);

	if (!global_preferences_available())
		return _subtitle_setting_automatic;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.subtitle_setting;
}

int32 __cdecl global_preferences_get_team_color()
{
	//return INVOKE(0x0050C250, global_preferences_get_team_color);

	if (!global_preferences_available())
		return 0;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.gameplay_settings.team_color;
}

e_quality_setting __cdecl global_preferences_get_texture_filtering_quality()
{
	//return INVOKE(0x0050C2D0, global_preferences_get_texture_filtering_quality);

	if (!global_preferences_available())
		return _quality_setting_medium;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.video_settings.texture_filtering_quality;
}

e_quality_setting __cdecl global_preferences_get_texture_resolution_quality()
{
	//return INVOKE(0x0050C350, global_preferences_get_texture_resolution_quality);

	if (!global_preferences_available())
		return _quality_setting_medium;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.video_settings.texture_resolution_quality;
}

bool __cdecl global_preferences_get_toggle_crouch()
{
	//return INVOKE(0x0050C3D0, global_preferences_get_toggle_crouch);

	if (!global_preferences_available())
		return false;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.gameplay_settings.toggle_crouch;
}

int32 __cdecl global_preferences_get_adapter()
{
	//return INVOKE(0x0050C450, global_preferences_get_adapter);

	if (!global_preferences_available())
		return -1;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.adapter;
}

int32 __cdecl global_preferences_get_unknown41BDC()
{
	//return INVOKE(0x0050C4D0, global_preferences_get_unknown41BDC);

	if (!global_preferences_available())
		return 4;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.video_settings.__unknown0;
}

int32 __cdecl global_preferences_get_voice_chat_control()
{
	//return INVOKE(0x0050C550, global_preferences_get_voice_chat_control);

	if (!global_preferences_available())
		return 0;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.sound_settings.voice_chat_control;
}

int32 __cdecl global_preferences_get_voice_chat_volume()
{
	//return INVOKE(0x0050C5D0, global_preferences_get_voice_chat_volume);

	if (!global_preferences_available())
		return 100;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.sound_settings.voice_chat_volume;
}

int32 __cdecl global_preferences_get_voice_volume()
{
	//return INVOKE(0x0050C650, global_preferences_get_voice_volume);

	if (!global_preferences_available())
		return 100;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.sound_settings.voice_volume;
}

bool __cdecl global_preferences_get_vsync()
{
	//return INVOKE(0x0050C6D0, global_preferences_get_vsync);

	if (!global_preferences_available())
		return false;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.video_settings.vsync;
}

e_gui_network_session_advertisement_mode __cdecl global_preferences_get_xbox_live_private_privacy_setting()
{
	//return INVOKE(0x0050C750, global_preferences_get_xbox_live_private_privacy_setting);

	if (!global_preferences_available() || get_alpha_is_internal_beta())
		return _network_advertise_xbox_live_public;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.xbox_live_private_privacy_setting;
}

void __cdecl global_preferences_init()
{
	INVOKE(0x0050C830, global_preferences_init);
}

void __cdecl global_preferences_initialize()
{
	//INVOKE(0x0050CAA0, global_preferences_initialize);

	c_global_preferences_scope_lock scope_lock;

	global_preferences_init();
	if (shell_application_type() == _shell_application_editor)
	{
		global_preferences_set_shadow_quality(_quality_setting_high);
		global_preferences_set_texture_resolution_quality(_quality_setting_high);
		global_preferences_set_texture_filtering_quality(_quality_setting_high);
		global_preferences_set_lighting_quality(_quality_setting_high);
		global_preferences_set_effects_quality(_quality_setting_high);
		global_preferences_set_details_quality(_quality_setting_high);
		global_preferences_set_postprocessing_quality(_quality_setting_high);
		global_preferences_set_vsync(false);
		global_preferences_set_motion_blur(true);
		global_preferences_set_antialiasing(true);
		global_preferences_set_display_brightness(50);
		global_preferences_set_display_contrast(50);
		global_preferences_dirty(false);
	}
}

void __cdecl global_preferences_invalidate_maps()
{
	//INVOKE(0x0050CBB0, global_preferences_invalidate_maps);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.build_number = 0;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_make_safe(s_global_preferences* preferences)
{
	//INVOKE(0x0050CC30, global_preferences_make_safe, preferences);

	if (!VALID_INDEX(preferences->data.last_language, k_language_count))
		preferences->data.last_language = _language_invalid;

	if (!VALID_INDEX(preferences->data.last_font_language, k_language_count))
		preferences->data.last_font_language = _language_invalid;

	if (!VALID_INDEX(preferences->data.subtitle_setting, k_number_of_subtitle_settings))
		preferences->data.subtitle_setting = _subtitle_setting_automatic;

	if (!VALID_INDEX(preferences->data.aspect_ratio, 3))
		preferences->data.aspect_ratio = 0;

	if (!VALID_INDEX(preferences->data.brightness, 100))
		preferences->data.brightness = 50;

	if (!VALID_INDEX(preferences->data.contrast, 100))
		preferences->data.contrast = 50;

	if (preferences->data.xbox_live_private_privacy_setting
		&& preferences->data.xbox_live_private_privacy_setting != _network_advertise_xbox_live_friends_only
		&& preferences->data.xbox_live_private_privacy_setting != _network_advertise_xbox_live_invite_only)
	{
		preferences->data.xbox_live_private_privacy_setting = _network_advertise_xbox_live_friends_only;
	}
}

void __cdecl global_preferences_set_antialiasing(bool antialiasing)
{
	//INVOKE(0x0050CCE0, global_preferences_set_antialiasing, antialiasing);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.video_settings.antialiasing = antialiasing;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_aspect_ratio(bool aspect_ratio)
{
	//INVOKE(0x0050CD70, global_preferences_set_aspect_ratio, aspect_ratio);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.aspect_ratio = aspect_ratio;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_betrayal_count(real32 betrayal_count)
{
	//INVOKE(0x0050CE00, global_preferences_set_betrayal_count, betrayal_count);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.betrayal_count = betrayal_count;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_build_number(int32 build_number)
{
	//INVOKE(0x0050CE90, global_preferences_set_build_number, build_number);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.build_number = build_number;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_camera_fov(real32 camera_fov)
{
	//INVOKE(0x0050CF20, global_preferences_set_camera_fov, camera_fov);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.gameplay_settings.camera_fov = camera_fov;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_controls_method(int32 controls_method)
{
	//INVOKE(0x0050CFB0, global_preferences_set_controls_method, controls_method);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.controls_settings.controls_method = controls_method;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_keyboard_preferences(s_keyboard_input_preferences* keyboard_preferences)
{
	INVOKE(0x0050D040, global_preferences_set_keyboard_preferences, keyboard_preferences);
}

void __cdecl global_preferences_set_details_quality(e_quality_setting details_quality)
{
	//INVOKE(0x0050D190, global_preferences_set_details_quality, details_quality);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.video_settings.details_quality = details_quality;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_display_brightness(int32 brightness)
{
	//INVOKE(0x0050D220, global_preferences_set_display_brightness, brightness);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.brightness = brightness;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_display_contrast(int32 contrast)
{
	//INVOKE(0x0050D2B0, global_preferences_set_display_contrast, contrast);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.contrast = contrast;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_display_hints(bool display_hints)
{
	//INVOKE(0x0050D340, global_preferences_set_display_hints, display_hints);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.gameplay_settings.display_hints = display_hints;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_effects_quality(e_quality_setting effects_quality)
{
	//INVOKE(0x0050D3D0, global_preferences_set_effects_quality, effects_quality);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.video_settings.effects_quality = effects_quality;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_eviction_count(int32 eviction_count)
{
	//INVOKE(0x0050D460, global_preferences_set_eviction_count, eviction_count);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.eviction_count = eviction_count;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_fullscreen(bool fullscreen)
{
	//INVOKE(0x0050D4F0, global_preferences_set_fullscreen, fullscreen);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.fullscreen = fullscreen;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_hardware_crc(uns32 crc)
{
	//INVOKE(0x0050D580, global_preferences_set_hardware_crc, crc);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.hardware_crc = crc;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_hide_watermark(bool hide_watermark)
{
	//INVOKE(0x0050D610, global_preferences_set_hide_watermark, hide_watermark);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.hide_watermark = hide_watermark;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_hud_shake(bool hud_shake)
{
	//INVOKE(0x0050D6A0, global_preferences_set_hud_shake, hud_shake);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.gameplay_settings.hud_shake = hud_shake;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_keyboard_preset(int32 keyboard_preset)
{
	//INVOKE(0x0050D730, global_preferences_set_keyboard_preset, keyboard_preset);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.controls_settings.keyboard_preset = keyboard_preset;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_last_font_language(e_language last_font_language)
{
	//INVOKE(0x0050D7C0, global_preferences_set_last_font_language, last_font_language);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.last_font_language = last_font_language;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_last_fonts_modification_date(const s_file_last_modification_date* last_fonts_modification_date)
{
	//INVOKE(0x0050D850, global_preferences_set_last_fonts_modification_date, last_fonts_modification_date);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.last_fonts_modification_date = *last_fonts_modification_date;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_last_game_setup(const s_gui_game_setup_storage* last_game_setup)
{
	//INVOKE(0x0050D8E0, global_preferences_set_last_game_setup, last_game_setup);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	csmemcpy(&global_preferences_get()->current.data.last_game_setup, last_game_setup, sizeof(s_gui_game_setup_storage));
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_last_language(e_language last_language)
{
	//INVOKE(0x0050D980, global_preferences_set_last_language, last_language);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.last_language = last_language;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_last_main_menu_item(int32 last_main_menu_item)
{
	//INVOKE(0x0050DA10, global_preferences_set_last_main_menu_item, last_main_menu_item);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.last_main_menu_item = last_main_menu_item;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_lighting_quality(e_quality_setting lighting_quality)
{
	//INVOKE(0x0050DAA0, global_preferences_set_lighting_quality, lighting_quality);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.video_settings.lighting_quality = lighting_quality;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_master_volume(int32 master_volume)
{
	//INVOKE(0x0050DB30, global_preferences_set_master_volume, master_volume);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.sound_settings.master_volume = master_volume;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_motion_blur(bool motion_blur)
{
	//INVOKE(0x0050DBC0, global_preferences_set_motion_blur, motion_blur);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.video_settings.motion_blur = motion_blur;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_mouse_acceleration(int32 mouse_acceleration)
{
	//INVOKE(0x0050DC50, global_preferences_set_mouse_acceleration, mouse_acceleration);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.controls_settings.mouse_acceleration = mouse_acceleration;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_mouse_filter(bool mouse_filter)
{
	//INVOKE(0x0050DCE0, global_preferences_set_mouse_filter, mouse_filter);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.controls_settings.mouse_filter = mouse_filter;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_mouse_inversion(bool mouse_inversion)
{
	//INVOKE(0x0050DD70, global_preferences_set_mouse_inversion, mouse_inversion);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.controls_settings.mouse_inversion = mouse_inversion;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_mouse_sensitivity_horizontal(int32 mouse_sensitivity_horizontal)
{
	//INVOKE(0x0050DE00, global_preferences_set_mouse_sensitivity_horizontal, mouse_sensitivity_horizontal);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.controls_settings.mouse_sensitivity_horizontal = mouse_sensitivity_horizontal;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_mouse_sensitivity_vehicle_horizontal(int32 mouse_sensitivity_vehicle_horizontal)
{
	//INVOKE(0x0050DE90, global_preferences_set_mouse_sensitivity_vehicle_horizontal, mouse_sensitivity_vehicle_horizontal);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.controls_settings.mouse_sensitivity_vehicle_horizontal = mouse_sensitivity_vehicle_horizontal;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_mouse_sensitivity_vehicle_vertical(int32 mouse_sensitivity_vehicle_vertical)
{
	//INVOKE(0x0050DF20, global_preferences_set_mouse_sensitivity_vehicle_vertical, mouse_sensitivity_vehicle_vertical);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.controls_settings.mouse_sensitivity_vehicle_vertical = mouse_sensitivity_vehicle_vertical;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_mouse_sensitivity_vertical(int32 mouse_sensitivity_vertical)
{
	//INVOKE(0x0050DFB0, global_preferences_set_mouse_sensitivity_vertical, mouse_sensitivity_vertical);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.controls_settings.mouse_sensitivity_vertical = mouse_sensitivity_vertical;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_music_volume(int32 music_volume)
{
	//INVOKE(0x0050E040, global_preferences_set_music_volume, music_volume);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.sound_settings.music_volume = music_volume;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_postprocessing_quality(e_quality_setting postprocessing_quality)
{
	//INVOKE(0x0050E0D0, global_preferences_set_postprocessing_quality, postprocessing_quality);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.video_settings.postprocessing_quality = postprocessing_quality;
	global_preferences_dirty(true);
}

//void __cdecl global_preferences_set_content_item_unique_name_number_seed(e_content_item_type content_item_type, int32 seed)
void __cdecl global_preferences_set_content_item_unique_name_number_seed(int32 content_item_type, int32 seed)
{
	//INVOKE(0x0050E160, global_preferences_set_content_item_unique_name_number_seed, content_item_type, seed);

	if (!global_preferences_available() && !VALID_INDEX(content_item_type, 14))
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.content_item_unique_name_number_seed[content_item_type] = seed;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_screen_resolution(int32 width, int32 height)
{
	//INVOKE(0x0050E1C0, global_preferences_set_screen_resolution, width, height);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.video_settings.width = width;
	global_preferences_get()->current.data.video_settings.height = height;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_sfx_volume(int32 sfx_volume)
{
	//INVOKE(0x0050E260, global_preferences_set_sfx_volume, sfx_volume);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.sound_settings.sfx_volume = sfx_volume;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_shadow_quality(e_quality_setting shadow_quality)
{
	//INVOKE(0x0050E2F0, global_preferences_set_shadow_quality, shadow_quality);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.video_settings.shadow_quality = shadow_quality;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_subtitle_setting(e_subtitle_setting subtitle_setting)
{
	//INVOKE(0x0050E380, global_preferences_set_subtitle_setting, subtitle_setting);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.subtitle_setting = subtitle_setting;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_team_color(int32 team_color)
{
	//INVOKE(0x0050E410, global_preferences_set_team_color, team_color);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.gameplay_settings.team_color = team_color;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_texture_filtering_quality(e_quality_setting texture_filtering_quality)
{
	//INVOKE(0x0050E4A0, global_preferences_set_texture_filtering_quality, texture_filtering_quality);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.video_settings.texture_filtering_quality = texture_filtering_quality;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_texture_resolution_quality(e_quality_setting texture_resolution_quality)
{
	//INVOKE(0x0050E530, global_preferences_set_texture_resolution_quality, texture_resolution_quality);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.video_settings.texture_resolution_quality = texture_resolution_quality;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_toggle_crouch(bool toggle_crouch)
{
	//INVOKE(0x0050E5C0, global_preferences_set_toggle_crouch, toggle_crouch);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.gameplay_settings.toggle_crouch = toggle_crouch;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_adapter(int32 adapter)
{
	//INVOKE(0x0050E650, global_preferences_set_adapter, adapter);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.adapter = adapter;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_unknown41BDC(int32 unknown41BDC)
{
	//INVOKE(0x0050E6E0, global_preferences_set_unknown41BDC, unknown41BDC);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.video_settings.__unknown0 = unknown41BDC;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_voice_chat_control(int32 voice_chat_control)
{
	//INVOKE(0x0050E770, global_preferences_set_voice_chat_control, voice_chat_control);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.sound_settings.voice_chat_control = voice_chat_control;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_voice_chat_volume(int32 voice_chat_volume)
{
	//INVOKE(0x0050E800, global_preferences_set_voice_chat_volume, voice_chat_volume);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.sound_settings.voice_chat_volume = voice_chat_volume;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_voice_volume(int32 voice_volume)
{
	//INVOKE(0x0050E890, global_preferences_set_voice_volume, voice_volume);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.sound_settings.voice_volume = voice_volume;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_vsync(bool vsync)
{
	//INVOKE(0x0050E920, global_preferences_set_vsync, vsync);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.video_settings.vsync = vsync;
	global_preferences_dirty(true);
}

void __cdecl global_preferences_set_xbox_live_private_privacy_setting(e_gui_network_session_advertisement_mode xbox_live_private_privacy_setting)
{
	//INVOKE(0x0050E9B0, global_preferences_set_xbox_live_private_privacy_setting, xbox_live_private_privacy_setting);

	if (!global_preferences_available())
		return;

	c_global_preferences_scope_lock scope_lock;
	global_preferences_get()->current.data.xbox_live_private_privacy_setting = xbox_live_private_privacy_setting;
	global_preferences_dirty(true);
}

bool __cdecl global_preferences_get_hide_watermark()
{
	//return INVOKE(0x0050EA40, global_preferences_get_hide_watermark);

	if (!global_preferences_available())
		return false;

	c_global_preferences_scope_lock scope_lock;
	return global_preferences_get()->current.data.hide_watermark;
}

void __cdecl global_preferences_update()
{
	INVOKE(0x0050EAC0, global_preferences_update);

	//c_global_preferences_scope_lock scope_lock;
	//if (global_preferences_are_dirty())
	//	global_preferences_write();
	//
	//if (global_preferences_get()->__unknown84010 && global_preferences_get()->ready_to_write.peek())
	//	global_preferences_get()->__unknown84010 = 0;
}

void __cdecl global_preferences_update_last_game_setup(s_gui_single_game_setup_storage* last_game_setup)
{
	INVOKE(0x0050EBA0, global_preferences_update_last_game_setup, last_game_setup);
}

void __cdecl global_preferences_write()
{
	INVOKE(0x0050ECE0, global_preferences_write);

	//if (global_preferences_get()->__unknown84010 && global_preferences_get()->ready_to_write.peek())
	//{
	//	if (csmemcmp(&global_preferences_get()->current.data, &global_preferences_get()->preferences1.data, sizeof(s_global_preferences_data)) != 0)
	//		global_preferences_dirty(true);
	//	csmemcpy(&global_preferences_get()->current.data, &global_preferences_get()->preferences1.data, sizeof(s_global_preferences_data));
	//
	//	s_network_http_request_hash hash{};
	//	if (file_handle_is_valid(global_preferences_get()->handle) &&
	//		security_calculate_hash(&global_preferences_get()->preferences1.data, sizeof(s_global_preferences_data), true, &hash))
	//	{
	//		global_preferences_get()->preferences1.secure_signature = hash;
	//		global_preferences_get()->__unknown84008 = async_write_position_ex(
	//			global_preferences_get()->handle,
	//			&global_preferences_get()->preferences1,
	//			sizeof(s_global_preferences),
	//			0,
	//			FLAG(_write_position_flush_file_bit),
	//			_async_category_saved_games,
	//			_async_priority_important_non_blocking,
	//			true,
	//			NULL);
	//	}
	//}
}

void __stdcall sound_system_set_master_volume(int32 volume, bool update_preference)
{
	INVOKE(0x0079B7D0, sound_system_set_master_volume, volume, update_preference);
}

void __stdcall sound_system_set_music_volume(int32 volume, bool update_preference)
{
	INVOKE(0x0079B940, sound_system_set_music_volume, volume, update_preference);
}

void __stdcall sound_system_set_sfx_volume(int32 volume, bool update_preference)
{
	INVOKE(0x0079B9E0, sound_system_set_sfx_volume, volume, update_preference);
}

const char* const k_global_preference_names[k_global_preference_count]
{
	"antialiasing",
	//"aspect_ratio",
	//"betrayal_count",
	//"build_number",
	"camera_fov",
	"controls_method",
	"details_quality",
	"brightness",
	"contrast",
	"display_hints",
	"effects_quality",
	//"eviction_count",
	"fullscreen",
	"hide_watermark",
	"hud_shake",
	"keyboard_preset",
	//"last_font_language",
	//"last_fonts_modification_date",
	//"last_game_setup",
	//"last_language",
	//"last_main_menu_item",
	"lighting_quality",
	"master_volume",
	"motion_blur",
	"mouse_acceleration",
	"mouse_filter",
	"mouse_inversion",
	"mouse_sensitivity_horizontal",
	"mouse_sensitivity_vehicle_horizontal",
	"mouse_sensitivity_vehicle_vertical",
	"mouse_sensitivity_vertical",
	"music_volume",
	"postprocessing_quality",
	//"content_item_unique_name_number_seed",
	"screen_resolution",
	"sfx_volume",
	"shadow_quality",
	"subtitle_setting",
	//"team_color",
	"texture_filtering_quality",
	"texture_resolution_quality",
	"toggle_crouch",
	//"adapter",
	//"unknown41BDC",
	//"voice_chat_control",
	//"voice_chat_volume",
	//"voice_volume",
	"vsync",
	//"xbox_live_private_privacy_setting"
};

#define DECLARE_GLOBAL_PREFERENCE(NAME, DESCRIPTION, PARAMETER_COUNT, ...) new s_global_preference({ (#NAME), (DESCRIPTION), (global_preferences_get_##NAME), (global_preferences_set_##NAME), (_global_preference_##NAME), (PARAMETER_COUNT), { __VA_ARGS__ } })

const s_global_preference* k_global_preferences[k_global_preference_count]
{
	DECLARE_GLOBAL_PREFERENCE(antialiasing, "<bool>", 1, _global_preference_type_bool),
	//DECLARE_GLOBAL_PREFERENCE(aspect_ratio, "<long>", 1, _global_preference_type_long),
	//DECLARE_GLOBAL_PREFERENCE(betrayal_count, "<real>", 1, _global_preference_type_real),
	//DECLARE_GLOBAL_PREFERENCE(build_number, "<long>", 1, _global_preference_type_long),
	DECLARE_GLOBAL_PREFERENCE(camera_fov, "<real>", 1, _global_preference_type_real),
	DECLARE_GLOBAL_PREFERENCE(controls_method, "<long>", 1, _global_preference_type_long),
	DECLARE_GLOBAL_PREFERENCE(details_quality, "<quality_setting>", 1, _global_preference_type_quality_setting),
	DECLARE_GLOBAL_PREFERENCE(display_brightness, "<long>", 1, _global_preference_type_long),
	DECLARE_GLOBAL_PREFERENCE(display_contrast, "<long>", 1, _global_preference_type_long),
	DECLARE_GLOBAL_PREFERENCE(display_hints, "<bool>", 1, _global_preference_type_bool),
	DECLARE_GLOBAL_PREFERENCE(effects_quality, "<quality_setting>", 1, _global_preference_type_quality_setting),
	//DECLARE_GLOBAL_PREFERENCE(eviction_count, "<long>", 1, _global_preference_type_long),
	DECLARE_GLOBAL_PREFERENCE(fullscreen, "<bool>", 1, _global_preference_type_bool),
	DECLARE_GLOBAL_PREFERENCE(hide_watermark, "<bool>", 1, _global_preference_type_bool),
	DECLARE_GLOBAL_PREFERENCE(hud_shake, "<bool>", 1, _global_preference_type_bool),
	DECLARE_GLOBAL_PREFERENCE(keyboard_preset, "<long>", 1, _global_preference_type_long),
	//DECLARE_GLOBAL_PREFERENCE(last_font_language, "<language>", 1, _global_preference_type_language),
	//DECLARE_GLOBAL_PREFERENCE(last_fonts_modification_date, "", 1),
	//DECLARE_GLOBAL_PREFERENCE(last_game_setup, "", 1),
	//DECLARE_GLOBAL_PREFERENCE(last_language, "<language>", 1, _global_preference_type_language),
	//DECLARE_GLOBAL_PREFERENCE(last_main_menu_item, "<long>", 1, _global_preference_type_long),
	DECLARE_GLOBAL_PREFERENCE(lighting_quality, "<quality_setting>", 1, _global_preference_type_quality_setting),
	DECLARE_GLOBAL_PREFERENCE(master_volume, "<long>", 1, _global_preference_type_long),
	DECLARE_GLOBAL_PREFERENCE(motion_blur, "<bool>", 1, _global_preference_type_bool),
	DECLARE_GLOBAL_PREFERENCE(mouse_acceleration, "<long>", 1, _global_preference_type_long),
	DECLARE_GLOBAL_PREFERENCE(mouse_filter, "<bool>", 1, _global_preference_type_bool),
	DECLARE_GLOBAL_PREFERENCE(mouse_inversion, "<bool>", 1, _global_preference_type_bool),
	DECLARE_GLOBAL_PREFERENCE(mouse_sensitivity_horizontal, "<long>", 1, _global_preference_type_long),
	DECLARE_GLOBAL_PREFERENCE(mouse_sensitivity_vehicle_horizontal, "<long>", 1, _global_preference_type_long),
	DECLARE_GLOBAL_PREFERENCE(mouse_sensitivity_vehicle_vertical, "<long>", 1, _global_preference_type_long),
	DECLARE_GLOBAL_PREFERENCE(mouse_sensitivity_vertical, "<long>", 1, _global_preference_type_long),
	DECLARE_GLOBAL_PREFERENCE(music_volume, "<long>", 1, _global_preference_type_long),
	DECLARE_GLOBAL_PREFERENCE(postprocessing_quality, "<quality_setting>", 1, _global_preference_type_quality_setting),
	//DECLARE_GLOBAL_PREFERENCE(content_item_unique_name_number_seed, "<long> <long>", 2, _global_preference_type_long, _global_preference_type_long),
	DECLARE_GLOBAL_PREFERENCE(screen_resolution, "<long> <long>", 2, _global_preference_type_long, _global_preference_type_long),
	DECLARE_GLOBAL_PREFERENCE(sfx_volume, "<long>", 1, _global_preference_type_long),
	DECLARE_GLOBAL_PREFERENCE(shadow_quality, "<quality_setting>", 1, _global_preference_type_quality_setting),
	DECLARE_GLOBAL_PREFERENCE(subtitle_setting, "<subtitle_setting>", 1, _global_preference_type_subtitle_setting),
	//DECLARE_GLOBAL_PREFERENCE(team_color, "<long>", 1, _global_preference_type_long),
	DECLARE_GLOBAL_PREFERENCE(texture_filtering_quality, "<quality_setting>", 1, _global_preference_type_quality_setting),
	DECLARE_GLOBAL_PREFERENCE(texture_resolution_quality, "<quality_setting>", 1, _global_preference_type_quality_setting),
	DECLARE_GLOBAL_PREFERENCE(toggle_crouch, "<bool>", 1, _global_preference_type_bool),
	//DECLARE_GLOBAL_PREFERENCE(adapter, "<long>", 1, _global_preference_type_long),
	//DECLARE_GLOBAL_PREFERENCE(unknown41BDC, "<long>", 1, _global_preference_type_long),
	//DECLARE_GLOBAL_PREFERENCE(voice_chat_control, "<long>", 1, _global_preference_type_long),
	//DECLARE_GLOBAL_PREFERENCE(voice_chat_volume, "<long>", 1, _global_preference_type_long),
	//DECLARE_GLOBAL_PREFERENCE(voice_volume, "<long>", 1, _global_preference_type_long),
	DECLARE_GLOBAL_PREFERENCE(vsync, "<bool>", 1, _global_preference_type_bool),
	//DECLARE_GLOBAL_PREFERENCE(xbox_live_private_privacy_setting, "<advertisement_mode>", 1, _global_preference_type_advertisement_mode)
};

union value_converter_t
{
	bool type_bool;
	real32 type_real;
	int32 type_long;
	e_quality_setting type_quality_setting;
	e_language type_language;
	e_subtitle_setting type_subtitle_setting;
	e_gui_network_session_advertisement_mode type_advertisement_mode;
};

template<const int32 k_maximum_parameter_count>
void value_handler(const s_global_preference& global_preference, value_converter_t(&values)[k_maximum_parameter_count], va_list parameters)
{
	ASSERT(VALID_COUNT(k_maximum_parameter_count, k_maximum_global_preference_parameters));

	for (int32 parameter_index = 0; parameter_index < global_preference.parameter_count; parameter_index++)
	{
		ASSERT(VALID_INDEX(parameter_index, k_maximum_global_preference_parameters));

		value_converter_t& value = values[parameter_index];

		switch (global_preference.parameter_types[parameter_index])
		{
		case _global_preference_type_bool:
			value.type_bool = va_arg(parameters, bool);
			break;
		case _global_preference_type_real:
			value.type_real = static_cast<real32>(va_arg(parameters, real64));
			break;
		case _global_preference_type_long:
			value.type_long = va_arg(parameters, int32);
			break;
		case _global_preference_type_quality_setting:
			value.type_quality_setting = va_arg(parameters, e_quality_setting);
			break;
		case _global_preference_type_language:
			value.type_language = va_arg(parameters, e_language);
			break;
		case _global_preference_type_subtitle_setting:
			value.type_subtitle_setting = va_arg(parameters, e_subtitle_setting);
			break;
		case _global_preference_type_advertisement_mode:
			value.type_advertisement_mode = va_arg(parameters, e_gui_network_session_advertisement_mode);
			break;
		}
	}
}

union function_converter_t
{
	void* pointer;

	void(__cdecl* type_bool)(bool);
	void(__cdecl* type_real)(real32);
	void(__cdecl* type_long)(int32);
	void(__cdecl* type_quality_setting)(e_quality_setting);
	void(__cdecl* type_language)(e_language);
	void(__cdecl* type_subtitle_setting)(e_subtitle_setting);
	void(__cdecl* type_advertisement_mode)(e_gui_network_session_advertisement_mode);

	void(__cdecl* type_screen_resolution)(int32, int32);
};

template<const int32 k_maximum_parameter_count>
void function_handler(const s_global_preference& global_preference, value_converter_t(&values)[k_maximum_parameter_count])
{
	function_converter_t function = { .pointer = global_preference.set };
	ASSERT(function.pointer != nullptr);

	if (global_preference.parameter_count == 1)
	{
		switch (global_preference.parameter_types[0])
		{
		case _global_preference_type_bool:
		{
			function.type_bool(values[0].type_bool);
		}
		break;
		case _global_preference_type_real:
		{
			function.type_real(values[0].type_real);
		}
		break;
		case _global_preference_type_long:
		{
			function.type_long(values[0].type_long);
		}
		break;
		case _global_preference_type_quality_setting:
		{
			function.type_quality_setting(values[0].type_quality_setting);
		}
		break;
		case _global_preference_type_language:
		{
			function.type_language(values[0].type_language);
		}
		break;
		case _global_preference_type_subtitle_setting:
		{
			function.type_subtitle_setting(values[0].type_subtitle_setting);
		}
		break;
		case _global_preference_type_advertisement_mode:
		{
			function.type_advertisement_mode(values[0].type_advertisement_mode);
		}
		break;
		}

		switch (global_preference.preference)
		{
		case _global_preference_fullscreen:
		{
			rasterizer_reset_device();
		}
		break;
		case _global_preference_master_volume:
		{
			sound_system_set_master_volume(values[0].type_long, false);
		}
		break;
		case _global_preference_music_volume:
		{
			sound_system_set_music_volume(values[0].type_long, false);
		}
		break;
		case _global_preference_sfx_volume:
		{
			sound_system_set_sfx_volume(values[0].type_long, false);
		}
		break;
		}
	}
	else if (global_preference.parameter_count == 2)
	{
		switch (global_preference.preference)
		{
		//case _global_preference_content_item_unique_name_number_seed:
		//	break
		case _global_preference_screen_resolution:
		{
			function.type_screen_resolution(values[0].type_long, values[1].type_long);
			rasterizer_reset_device();
		}
		break;
		}
	}
}

bool global_preference_set_impl(const char* name, int16 parameter_count, ...)
{
	e_global_preference preference = global_preference_from_string(name);
	if (preference == e_global_preference(-1))
		return false;

	const s_global_preference* global_preference = k_global_preferences[preference];
	ASSERT(VALID_COUNT(global_preference->parameter_count, k_maximum_global_preference_parameters));

	if (parameter_count != global_preference->parameter_count)
		return false;

	va_list parameters;
	va_start(parameters, parameter_count);

	value_converter_t values[k_maximum_global_preference_parameters] = {};
	value_handler(*global_preference, values, parameters);
	function_handler(*global_preference, values);

	va_end(parameters);

	return true;
}

const char* global_preference_get_name(e_global_preference preference)
{
	if (VALID_INDEX(preference, k_global_preference_count))
		return k_global_preference_names[preference];

	return nullptr;
}

e_global_preference global_preference_from_string(const char* str)
{
	e_global_preference preference = e_global_preference(-1);
	for (int32 i = _global_preference_antialiasing; i < k_global_preference_count; i++)
	{
		if (csstricmp(str, k_global_preference_names[i]) != 0)
			continue;

		preference = e_global_preference(i);
	}

	return preference;
}

