#pragma once
#pragma once

#include "cseries/cseries.hpp"
#include "cseries/language.hpp"
#include "game/game_options.hpp"
#include "memory/secure_signature.hpp"
#include "multithreading/synchronized_value.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "saved_games/scenario_map_variant.hpp"
#include "shell/shell.hpp"
#include "tag_files/files_windows.hpp"

class c_global_preferences_scope_lock
{
public:
	c_global_preferences_scope_lock();
	~c_global_preferences_scope_lock();

protected:
	bool m_took_lock;
};

struct s_gui_game_setup_storage
{
	struct s_campaign_settings
	{
		bool valid;

		long campaign_id;
		long map_id;
		short insertion_point;
		c_enum<e_campaign_difficulty_level, long, _campaign_difficulty_level_easy, k_campaign_difficulty_levels_count> difficulty_level;
		long metagame_scoring_option;
		dword active_primary_skulls;
		dword active_secondary_skulls;
		s_campaign_armaments campaign_armaments;
		s_campaign_game_progression campaign_progression;
		s_hub_progression hub_progression;
	};
	static_assert(sizeof(s_campaign_settings) == 0x198);

	// `s_survival_settings` is `s_campaign_settings`
	struct s_survival_settings
	{
		bool valid;

		long campaign_id;
		long map_id;
		short insertion_point;
		c_enum<e_campaign_difficulty_level, long, _campaign_difficulty_level_easy, k_campaign_difficulty_levels_count> difficulty_level;
		long metagame_scoring_option;
		dword active_primary_skulls;
		dword active_secondary_skulls;
		s_campaign_armaments campaign_armaments;
		s_campaign_game_progression campaign_progression;
		s_hub_progression hub_progression;
	};
	static_assert(sizeof(s_survival_settings) == sizeof(s_campaign_settings));

	struct s_matchmaking_settings
	{
		bool valid;

		word hopper_identifier;
		long : 32;
	};
	static_assert(sizeof(s_matchmaking_settings) == 0x8);

	struct s_multiplayer_settings
	{
		struct s_game_variant_settings
		{
			c_game_variant variant;

			bool valid;
			long : 32;

			s_player_identifier player_identifier;
			c_static_wchar_string<256> path;
		};
		static_assert(sizeof(s_game_variant_settings) == 0x474);

		struct s_map_variant_settings
		{
			c_map_variant variant;

			bool valid;
			long : 32;

			s_player_identifier player_identifier;
			c_static_wchar_string<256> path;
		};
		static_assert(sizeof(s_map_variant_settings) == 0xE2A0);

		bool valid;
		s_game_variant_settings game_variant_settings;
		s_map_variant_settings map_variant_settings;
	};
	static_assert(sizeof(s_multiplayer_settings) == 0xE718);

	struct s_map_editor_settings
	{
		bool valid;
		long : 32;

		s_multiplayer_settings::s_map_variant_settings map_variant_settings;
	};
	static_assert(sizeof(s_map_editor_settings) == 0xE2A8);

	struct s_theater_settings
	{
		bool valid;
		long : 32;
		s_player_identifier player_identifier;

		s_saved_film_description film;
		game_options options;

		byte __data24E78[0x8];
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
		c_enum<e_quality_setting, long, _quality_setting_low, k_quality_setting_count> texture_resolution_quality;
		c_enum<e_quality_setting, long, _quality_setting_low, k_quality_setting_count> shadow_quality;
		c_enum<e_quality_setting, long, _quality_setting_low, k_quality_setting_count> texture_filtering_quality;
		c_enum<e_quality_setting, long, _quality_setting_low, k_quality_setting_count> lighting_quality;
		c_enum<e_quality_setting, long, _quality_setting_low, k_quality_setting_count> effects_quality;
		c_enum<e_quality_setting, long, _quality_setting_low, k_quality_setting_count> details_quality;
		c_enum<e_quality_setting, long, _quality_setting_low, k_quality_setting_count> postprocessing_quality;
		bool motion_blur;
		bool vsync;
		bool antialiasing;
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
		bool hud_shake;
		bool display_hints;
		long team_color;
		real camera_fov;
	};
	static_assert(sizeof(s_gameplay_settings) == 0xC);

	struct s_control_settings
	{
		struct s_bindings
		{
			// e_key_code
			c_static_array<short, 63> keys_primary;
			c_static_array<short, 63> keys_secondary;

			// e_mouse_button
			c_static_array<char, 63> mouse_buttons_primary;
			c_static_array<char, 63> mouse_buttons_secondary;

			short joystick_layout;
		};
		static_assert(sizeof(s_bindings) == 0x17C);

		long controls_method;
		long mouse_sensitivity_vertical;
		long mouse_sensitivity_horizontal;
		long mouse_sensitivity_vehicle_vertical;
		long mouse_sensitivity_vehicle_horizontal;
		long mouse_acceleration;
		bool mouse_filter;
		bool mouse_inversion;
		long keyboard_preset;
		s_bindings bindings;
	};
	static_assert(sizeof(s_control_settings) == 0x19C);

	long version;
	c_enum<e_language, long, _language_invalid, k_language_count> last_language;
	c_enum<e_language, long, _language_invalid, k_language_count> last_font_language;
	s_file_last_modification_date last_fonts_modification_date;
	long build_number;
	real betrayal_count; // betrayal_penalty?
	long eviction_count;
	c_enum<e_gui_game_mode, long, _ui_game_mode_none, k_ui_game_mode_count> last_main_menu_item;
	long last_title_menu_item;
	s_gui_game_setup_storage last_game_setup;
	c_enum<e_gui_network_session_advertisement_mode, long, _gui_network_session_advertisement_mode_invalid, k_gui_network_session_advertisement_mode_count> xbox_live_private_privacy_setting;
	long checksum;
	long adapter;
	bool fullscreen;
	long __unknown41BD0;
	long brightness;
	long contrast;
	s_video_settings video_settings;
	bool hide_watermark;
	c_enum<e_subtitle_setting, long, _subtitle_setting_automatic, k_subtitle_setting_count> subtitle_setting;
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
	bool initialized; // locked?
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

extern s_global_preferences_internals_type* __cdecl global_preferences_get();
extern void __cdecl global_preferences_dirty(bool dirty);
extern bool __cdecl global_preferences_are_dirty();
extern bool __cdecl global_preferences_available();
extern void __cdecl global_preferences_clear();
extern void __cdecl global_preferences_create_default();
extern void __cdecl global_preferences_dispose();
extern void __cdecl global_preferences_flush();
extern bool __cdecl global_preferences_get_antialiasing();
extern long __cdecl global_preferences_get_unknown41BD0();
extern real __cdecl global_preferences_get_betrayal_count();
extern long __cdecl global_preferences_get_build_number();
extern real __cdecl global_preferences_get_camera_fov();
extern long __cdecl global_preferences_get_controls_method();
extern s_global_preferences_data::s_control_settings::s_bindings* __cdecl global_preferences_get_bindings(s_global_preferences_data::s_control_settings::s_bindings* bindings);
extern void __cdecl global_preferences_bindings_create_default(s_global_preferences_data::s_control_settings::s_bindings* bindings);
extern void __cdecl global_preferences_gameplay_settings_create_default(s_global_preferences_data::s_gameplay_settings* gameplay_settings);
extern void __cdecl global_preferences_control_settings_create_default(s_global_preferences_data::s_control_settings* control_settings);
extern void __cdecl global_preferences_sound_settings_create_default(s_global_preferences_data::s_sound_settings* sound_settings);
extern e_quality_setting __cdecl global_preferences_get_details_quality();
extern long __cdecl global_preferences_get_brightness();
extern bool __cdecl global_preferences_get_display_hints();
extern e_quality_setting __cdecl global_preferences_get_effects_quality();
extern long __cdecl global_preferences_get_eviction_count();
extern bool __cdecl global_preferences_get_fullscreen();
extern dword __cdecl global_preferences_get_checksum();
extern bool __cdecl global_preferences_get_hud_shake();
extern long __cdecl global_preferences_get_keyboard_preset();
extern e_language __cdecl global_preferences_get_last_font_language();
extern void __cdecl global_preferences_get_last_fonts_modification_date(s_file_last_modification_date* last_fonts_modification_date);
extern s_gui_game_setup_storage* __cdecl global_preferences_get_last_game_setup();
extern bool __cdecl global_preferences_get_last_game_setup_map(e_gui_game_mode game_mode, long* campaign_id, long* map_id);
extern e_language __cdecl global_preferences_get_last_language();
extern e_gui_game_mode __cdecl global_preferences_get_last_main_menu_item();
extern e_quality_setting __cdecl global_preferences_get_lighting_quality();
extern long __cdecl global_preferences_get_master_volume();
extern bool __cdecl global_preferences_get_motion_blur();
extern long __cdecl global_preferences_get_mouse_acceleration();
extern bool __cdecl global_preferences_get_mouse_filter();
extern bool __cdecl global_preferences_get_mouse_inversion();
extern long __cdecl global_preferences_get_mouse_sensitivity_horizontal();
extern long __cdecl global_preferences_get_mouse_sensitivity_vehicle_horizontal();
extern long __cdecl global_preferences_get_mouse_sensitivity_vehicle_vertical();
extern long __cdecl global_preferences_get_mouse_sensitivity_vertical();
extern long __cdecl global_preferences_get_music_volume();
extern e_quality_setting __cdecl global_preferences_get_postprocessing_quality();
extern long __cdecl global_preferences_get_content_item_unique_name_number_seed(long content_item_type);
extern void __cdecl global_preferences_get_screen_resolution(long* width, long* height);
extern long __cdecl global_preferences_get_sfx_volume();
extern e_quality_setting __cdecl global_preferences_get_shadow_quality();
extern e_subtitle_setting __cdecl global_preferences_get_subtitle_setting();
extern long __cdecl global_preferences_get_team_color();
extern e_quality_setting __cdecl global_preferences_get_texture_filtering_quality();
extern e_quality_setting __cdecl global_preferences_get_texture_resolution_quality();
extern bool __cdecl global_preferences_get_toggle_crouch();
extern long __cdecl global_preferences_get_adapter();
extern long __cdecl global_preferences_get_unknown41BDC();
extern long __cdecl global_preferences_get_voice_chat_control();
extern long __cdecl global_preferences_get_voice_chat_volume();
extern long __cdecl global_preferences_get_voice_volume();
extern bool __cdecl global_preferences_get_vsync();
extern e_gui_network_session_advertisement_mode __cdecl global_preferences_get_xbox_live_private_privacy_setting();
extern void __cdecl global_preferences_init();
extern void __cdecl global_preferences_initialize();
extern void __cdecl global_preferences_invalidate_maps();
extern void __cdecl global_preferences_make_safe(s_global_preferences* preferences);
extern void __cdecl global_preferences_set_antialiasing(bool antialiasing);
extern void __cdecl global_preferences_set_unknown41BD0(bool unknown41BD0);
extern void __cdecl global_preferences_set_betrayal_count(real betrayal_count);
extern void __cdecl global_preferences_set_build_number(long build_number);
extern void __cdecl global_preferences_set_camera_fov(real camera_fov);
extern void __cdecl global_preferences_set_controls_method(long controls_method);
extern void __cdecl global_preferences_set_details_quality(e_quality_setting details_quality);
extern void __cdecl global_preferences_set_brightness(long brightness);
extern void __cdecl global_preferences_set_contrast(long contrast);
extern void __cdecl global_preferences_set_display_hints(bool display_hints);
extern void __cdecl global_preferences_set_effects_quality(e_quality_setting effects_quality);
extern void __cdecl global_preferences_set_eviction_count(long eviction_count);
extern void __cdecl global_preferences_set_fullscreen(bool fullscreen);
extern void __cdecl global_preferences_set_hide_watermark(bool hide_watermark);
extern void __cdecl global_preferences_set_hud_shake(bool hud_shake);
extern void __cdecl global_preferences_set_keyboard_preset(long controls_settings_keyboard_preset);
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
extern void __cdecl global_preferences_set_mouse_inversion(bool mouse_invert);
extern void __cdecl global_preferences_set_mouse_sensitivity_horizontal(long mouse_sensitivity_horizontal);
extern void __cdecl global_preferences_set_mouse_sensitivity_vehicle_horizontal(long mouse_sensitivity_vehicle_horizontal);
extern void __cdecl global_preferences_set_mouse_sensitivity_vehicle_vertical(long mouse_sensitivity_vehicle_vertical);
extern void __cdecl global_preferences_set_mouse_sensitivity_vertical(long mouse_sensitivity_vertical);
extern void __cdecl global_preferences_set_music_volume(long music_volume);
extern void __cdecl global_preferences_set_postprocessing_quality(e_quality_setting postprocessing_quality);
extern void __cdecl global_preferences_set_content_item_unique_name_number_seed(long content_item_type, long seed);
extern void __cdecl global_preferences_set_screen_resolution(long width, long height);
extern void __cdecl global_preferences_set_sfx_volume(long sfx_volume);
extern void __cdecl global_preferences_set_shadow_quality(e_quality_setting shadow_quality);
extern void __cdecl global_preferences_set_subtitle_setting(e_subtitle_setting subtitle_setting);
extern void __cdecl global_preferences_set_team_color(long team_color);
extern void __cdecl global_preferences_set_texture_filtering_quality(e_quality_setting texture_filtering_quality);
extern void __cdecl global_preferences_set_texture_resolution_quality(e_quality_setting texture_resolution_quality);
extern void __cdecl global_preferences_set_toggle_crouch(bool toggle_crouch);
extern void __cdecl global_preferences_set_adapter(long adapter);
extern void __cdecl global_preferences_set_unknown41BDC(long unknown41BDC);
extern void __cdecl global_preferences_set_voice_chat_control(long voice_chat_control);
extern void __cdecl global_preferences_set_voice_chat_volume(long voice_chat_volume);
extern void __cdecl global_preferences_set_voice_volume(long voice_volume);
extern void __cdecl global_preferences_set_vsync(bool vsync);
extern void __cdecl global_preferences_set_xbox_live_private_privacy_setting(e_gui_network_session_advertisement_mode xbox_live_private_privacy_setting);
extern bool __cdecl global_preferences_get_hide_watermark();
extern void __cdecl global_preferences_update();
extern void __cdecl global_preferences_write();

