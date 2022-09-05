#pragma once
#pragma once

#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"
#include "multithreading/synchronized_value.hpp"
#include "shell/shell.hpp"
#include "tag_files/files_windows.hpp"

struct s_gui_game_setup_storage
{
	struct s_campaign_settings
	{
		byte __data[0x198];
	};
	static_assert(sizeof(s_campaign_settings) == 0x198);

	// `s_survival_settings` is `s_campaign_settings`
	struct s_survival_settings
	{
		byte __data[0x198];
	};
	static_assert(sizeof(s_survival_settings) == sizeof(s_campaign_settings));

	struct s_matchmaking_settings
	{
		byte __data[0x8];
	};
	static_assert(sizeof(s_matchmaking_settings) == 0x8);

	struct s_multiplayer_settings
	{
		byte __data[0xE718];
	};
	static_assert(sizeof(s_multiplayer_settings) == 0xE718);

	struct s_map_editor_settings
	{
		byte __data[0xE2A8];
	};
	static_assert(sizeof(s_map_editor_settings) == 0xE2A8);

	struct s_theater_settings
	{
		byte __data[0x24E80];
	};
	static_assert(sizeof(s_theater_settings) == 0x24E80);

	s_campaign_settings campaign_settings;
	s_survival_settings survival_settings;
	s_matchmaking_settings matchmaking_settings;
	s_multiplayer_settings multiplayer_settings;
	s_map_editor_settings map_editor_settings;
	s_theater_settings theater_settings;
};
static_assert(sizeof(s_gui_game_setup_storage) == 0x41B78);

struct s_global_preferences_data
{
	struct s_video_settings
	{
		long __unknown0;
		long width;
		long height;
		long resolution_width;
		long resolution_height;
		long texture_resolution;
		long shadow_quality;
		long texture_filtering_quality;
		long lighting_quality;
		long effects_quality;
		long details_quality;
		long postprocessing_quality;
		bool motion_blur;
		bool is_vsync;
		bool is_anti_aliasing;
	};
	static_assert(sizeof(s_video_settings) == 0x34);

	struct s_sound_settings
	{
		long master_volume;
		long sfx_volume;
		long music_volume;
		long voice_volume;
		long voice_chat_control;
		long voice_chat_volume;
	};
	static_assert(sizeof(s_sound_settings) == 0x18);

	struct s_gameplay_settings
	{
		bool toggle_crouch;
		bool hud_shake_enabled;
		bool display_hints;
		long team_color_mode;
		real camera_fov;
	};
	static_assert(sizeof(s_gameplay_settings) == 0xC);

	struct s_control_settings
	{
		struct s_bindings
		{
			// e_key_code
			word primary_keys[63];
			word alternative_keys[63];

			// e_mouse_button
			byte primary_mouse_buttons[63];
			byte alternative_mouse_buttons[63];

			short joystick_layout;
		};
		static_assert(sizeof(s_bindings) == 0x17C);

		long controls_method;
		long mouse_sensitivity_vert;
		long mouse_sensitivity_hor;
		long mouse_sensitivity_vehicle_vert;
		long mouse_sensitivity_vehicle_hor;
		long mouse_acceleration;
		bool mouse_filter;
		bool mouse_invertion;
		long keyboard_preset;
		s_bindings bindings;
	};
	static_assert(sizeof(s_control_settings) == 0x19C);

	long version;
	long last_language;
	long last_font_language;
	s_file_last_modification_date last_fonts_modification_date;
	long build_number;
	long betrayal_count;
	long eviction_count;
	long last_main_menu_item;
	long last_title_menu_item;
	s_gui_game_setup_storage last_game_setup;
	long xbox_live_private_privacy;
	long __unknown41BC4;
	long adapter;
	bool is_fullscreen;
	long __unknown41BD0;
	long brightness;
	long contrast;
	s_video_settings video_settings;
	bool hide_watermark;
	long subtitle_setting;
	long content_item_unique_name_number_seed[14];
	s_sound_settings sound_settings;
	s_gameplay_settings gameplay_settings;
	s_control_settings controls_settings;
};
static_assert(sizeof(s_global_preferences_data) == 0x41DF0);

struct s_global_preferences
{
	s_network_http_request_hash secure_signature;
	byte __data14[4];
	s_global_preferences_data data;
	char __data41E08[0x1F8];
};
static_assert(sizeof(s_global_preferences) == 0x42000);

struct s_global_preferences_internals_type
{
	bool initialized;
	bool dirty;
	s_file_handle handle;
	s_global_preferences preferences0;
	s_global_preferences preferences1;
	long __unknown84008;
	c_synchronized_long ready_to_write;
	bool __unknown84010;
	bool __unknown84011;
	bool __unknown84012;
	bool __unknown84013;
	byte __data84018[0x1C];
};
static_assert(sizeof(s_global_preferences_internals_type) == 0x84030);

extern s_global_preferences_internals_type* g_global_preferences;

enum e_quality_setting
{
	_quality_setting_low = 0,
	_quality_setting_medium,
	_quality_setting_high,

	k_quality_setting_count
};

enum e_subtitle_setting
{

};

enum e_gui_network_session_advertisement_mode
{

};

extern void __cdecl global_preferences_set_antialiasing(bool antialiasing);
extern void __cdecl global_preferences_set_betrayal_count(float betrayal_count);
extern void __cdecl global_preferences_set_build_number(long build_number);
extern void __cdecl global_preferences_set_camera_fov(float camera_fov);
extern void __cdecl global_preferences_set_details_quality(e_quality_setting details_quality);
extern void __cdecl global_preferences_set_brightness(long brightness);
extern void __cdecl global_preferences_set_contrast(long contrast);
extern void __cdecl global_preferences_set_effects_quality(e_quality_setting effects_quality);
extern void __cdecl global_preferences_set_eviction_count(long eviction_count);
extern void __cdecl global_preferences_set_fullscreen(bool fullscreen);
extern void __cdecl global_preferences_set_watermark_hidden(bool watermark_hidden);
extern void __cdecl global_preferences_set_hud_shake(bool hud_shake);
extern void __cdecl global_preferences_set_controls_settings_keyboard_preset(long controls_settings_keyboard_preset);
extern void __cdecl global_preferences_set_last_font_language(e_language last_font_language);
extern void __cdecl global_preferences_set_last_fonts_modification_date(s_file_last_modification_date const* last_fonts_modification_date);
extern void __cdecl global_preferences_set_last_game_setup(s_gui_game_setup_storage const* last_game_setup);
extern void __cdecl global_preferences_set_last_language(e_language last_language);
extern void __cdecl global_preferences_set_last_main_menu_item(long last_main_menu_item);
extern void __cdecl global_preferences_set_lighting_quality(e_quality_setting lighting_quality);
extern void __cdecl global_preferences_set_master_volume(long master_volume);
extern void __cdecl global_preferences_set_motion_blur(bool motion_blur);
extern void __cdecl global_preferences_set_mouse_acceleration(long mouse_acceleration);
extern void __cdecl global_preferences_set_mouse_filter(bool mouse_filter);
extern void __cdecl global_preferences_set_mouse_invert(bool mouse_invert);
extern void __cdecl global_preferences_set_mouse_sensitivity_horizontal(long mouse_sensitivity_horizontal);
extern void __cdecl global_preferences_set_mouse_sensitivity_vehicle_horizontal(long mouse_sensitivity_vehicle_horizontal);
extern void __cdecl global_preferences_set_mouse_sensitivity_vehicle_vertical(long mouse_sensitivity_vehicle_vertical);
extern void __cdecl global_preferences_set_mouse_sensitivity_vertical(long mouse_sensitivity_vertical);
extern void __cdecl global_preferences_set_music_volume(long music_volume);
extern void __cdecl global_preferences_set_postprocessing_quality(e_quality_setting postprocessing_quality);
extern void __cdecl global_preferences_set_content_item_unique_name_number_seed(e_content_item_type type, long seed);
extern void __cdecl global_preferences_set_screen_resolution(long width, long height);
extern void __cdecl global_preferences_set_sfx_volume(long sfx_volume);
extern void __cdecl global_preferences_set_shadow_quality(e_quality_setting shadow_quality);
extern void __cdecl global_preferences_set_subtitle_setting(e_subtitle_setting subtitle_setting);
extern void __cdecl global_preferences_set_team_color(long team_color);
extern void __cdecl global_preferences_set_texture_filtering_quality(e_quality_setting texture_filtering_quality);
extern void __cdecl global_preferences_set_texture_resolution_quality(e_quality_setting texture_resolution_quality);
extern void __cdecl global_preferences_set_toggle_crouch(bool toggle_crouch);
extern void __cdecl global_preferences_set_adapter(long adapter);
extern void __cdecl global_preferences_set_voice_chat_control(long voice_chat_control);
extern void __cdecl global_preferences_set_voice_chat_volume(long voice_chat_volume);
extern void __cdecl global_preferences_set_voice_volume(long voice_volume);
extern void __cdecl global_preferences_set_vsync(bool vsync);
extern void __cdecl global_preferences_set_xbox_live_private_privacy_setting(e_gui_network_session_advertisement_mode xbox_live_private_privacy_setting);
