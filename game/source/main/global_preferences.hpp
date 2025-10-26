#pragma once

#include "cseries/cseries.hpp"
#include "cseries/language.hpp"
#include "game/game_options.hpp"
#include "input/input_abstraction.hpp"
#include "memory/secure_signature.hpp"
#include "multithreading/synchronized_value.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "saved_games/scenario_map_variant.hpp"
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
	struct s_game_variant_settings
	{
	public:
		bool is_valid() const
		{
			return game_engine_variant_is_valid(&variant);
		}

		const c_game_variant* get_variant() const
		{
			return &variant;
		}

		void set_variant(const c_game_variant& other)
		{
			if (!variant.is_equal_to(&other))
			{
				variant.copy_from_unsafe(&other);
			}
		}

	protected:
		c_game_variant variant;

		struct
		{
			bool valid;
			e_gui_selected_item_location location;
			s_player_identifier owner;
			wchar_t file_path[256];
		} only_local;
	};
	static_assert(sizeof(s_game_variant_settings) == 0x474);

	struct s_map_variant_settings
	{
	public:
		bool is_valid() const
		{
			return variant.is_valid();
		}

		const c_map_variant* get_variant() const
		{
			return &variant;
		}

		void set_variant(c_map_variant& source)
		{
			//if (!variant.is_equal_to(&source))
			{
				source.save_to(&variant);
			}
		}

	protected:
		c_map_variant variant;

		struct
		{
			bool valid;
			e_gui_selected_item_location location;
			s_player_identifier owner;
			wchar_t file_path[256];
		} only_local;
	};
	static_assert(sizeof(s_map_variant_settings) == 0xE2A0);

	struct s_campaign_settings
	{
	public:
		bool is_valid() const
		{
			return valid;
		}

	//protected:
		bool valid;
		e_campaign_id campaign_id;
		e_map_id map_id;

		// ODST
		int16 insertion_point;

		e_campaign_difficulty_level difficulty;
		e_campaign_metagame_scoring metagame_scoring_policy;
		int32 active_primary_skulls;
		int32 active_secondary_skulls;

		// ODST
		s_campaign_armaments campaign_armaments;
		s_campaign_game_progression campaign_progression;
		s_hub_progression hub_progression;
	};
	static_assert(sizeof(s_campaign_settings) == 0x198);

	typedef s_campaign_settings s_survival_settings;

	struct s_matchmaking_settings
	{
		bool valid;
		uns16 hopper_id;
		int32 pad;
	};
	static_assert(sizeof(s_matchmaking_settings) == 0x8);

	struct s_multiplayer_settings
	{
	public:
		bool is_valid() const
		{
			return valid && game_variant_settings.is_valid() && map_variant_settings.is_valid();
		}

	//protected:
		bool valid;
		s_game_variant_settings game_variant_settings;
		s_map_variant_settings map_variant_settings;
	};
	static_assert(sizeof(s_multiplayer_settings) == 0xE718);

	struct s_mapeditor_settings
	{
	public:
		bool is_valid() const
		{
			return valid && map_variant_settings.is_valid();
		}

	//protected:
		bool valid;
		bool dirtied_in_game;
		int32 pad;
		s_map_variant_settings map_variant_settings;
	};
	static_assert(sizeof(s_mapeditor_settings) == 0xE2A8);

	struct s_theater_settings
	{
	public:
		bool is_valid() const
		{
			return valid;
		}

	//protected:
		bool valid;
		e_gui_selected_item_location location;
		s_player_identifier owner;
		s_saved_film_description film_description;
		game_options game_options;
		int32 length_in_ticks;
		int32 start_tick;
	};
	static_assert(sizeof(s_theater_settings) == 0x24E80);

public:
	s_campaign_settings* get_campaign()
	{
		return &campaign_settings;
	}

	s_campaign_settings const* get_campaign() const
	{
		return &campaign_settings;
	}

	s_survival_settings* get_survival()
	{
		return &survival_settings;
	}

	s_survival_settings const* get_survival() const
	{
		return &survival_settings;
	}

	s_matchmaking_settings* get_matchmaking()
	{
		return &matchmaking_settings;
	}

	s_matchmaking_settings const* get_matchmaking() const
	{
		return &matchmaking_settings;
	}

	s_multiplayer_settings* get_multiplayer()
	{
		return &multiplayer_settings;
	}

	s_multiplayer_settings const* get_multiplayer() const
	{
		return &multiplayer_settings;
	}

	s_mapeditor_settings* get_mapeditor()
	{
		return &map_editor_settings;
	}

	s_mapeditor_settings const* get_mapeditor() const
	{
		return &map_editor_settings;
	}

	s_theater_settings* get_theater()
	{
		return &theater_settings;
	}

	s_theater_settings const* get_theater() const
	{
		return &theater_settings;
	}

protected:
	s_campaign_settings campaign_settings;
	s_survival_settings survival_settings;
	s_matchmaking_settings matchmaking_settings;
	s_multiplayer_settings multiplayer_settings;
	s_mapeditor_settings map_editor_settings;
	s_theater_settings theater_settings;
};
static_assert(sizeof(s_gui_game_setup_storage) == 0x41B78);

#pragma pack(push, 4)

struct s_gui_single_game_setup_storage
{
	e_gui_game_mode game_mode;

	union
	{
		s_gui_game_setup_storage::s_campaign_settings campaign;
		s_gui_game_setup_storage::s_survival_settings survival;
		s_gui_game_setup_storage::s_matchmaking_settings matchmaking;
		s_gui_game_setup_storage::s_multiplayer_settings multiplayer;
		s_gui_game_setup_storage::s_mapeditor_settings mapeditor;
		s_gui_game_setup_storage::s_theater_settings theater;
	};
};
static_assert(sizeof(s_gui_single_game_setup_storage) == 0x24E84);

#pragma pack(pop)

struct s_global_preferences_data
{
	struct s_video_settings
	{
		int32 __unknown0;
		int32 width;
		int32 height;
		int32 resolution_width;
		int32 resolution_height;
		c_enum<e_quality_setting, int32, _quality_setting_low, k_quality_setting_count> texture_resolution_quality;
		c_enum<e_quality_setting, int32, _quality_setting_low, k_quality_setting_count> shadow_quality;
		c_enum<e_quality_setting, int32, _quality_setting_low, k_quality_setting_count> texture_filtering_quality;
		c_enum<e_quality_setting, int32, _quality_setting_low, k_quality_setting_count> lighting_quality;
		c_enum<e_quality_setting, int32, _quality_setting_low, k_quality_setting_count> effects_quality;
		c_enum<e_quality_setting, int32, _quality_setting_low, k_quality_setting_count> details_quality;
		c_enum<e_quality_setting, int32, _quality_setting_low, k_quality_setting_count> postprocessing_quality;
		bool motion_blur;
		bool vsync;
		bool antialiasing;
	};
	static_assert(sizeof(s_video_settings) == 0x34);

	struct s_sound_settings
	{
		int32 master_volume;
		int32 sfx_volume;
		int32 music_volume;
		int32 voice_volume;
		int32 voice_chat_control;
		int32 voice_chat_volume;
	};
	static_assert(sizeof(s_sound_settings) == 0x18);

	struct s_gameplay_settings
	{
		bool toggle_crouch;
		bool hud_shake;
		bool display_hints;
		int32 team_color;
		real32 camera_fov;
	};
	static_assert(sizeof(s_gameplay_settings) == 0xC);

	struct s_control_settings
	{
		int32 controls_method;
		int32 mouse_sensitivity_vertical;
		int32 mouse_sensitivity_horizontal;
		int32 mouse_sensitivity_vehicle_vertical;
		int32 mouse_sensitivity_vehicle_horizontal;
		int32 mouse_acceleration;
		bool mouse_filter;
		bool mouse_inversion;
		int32 keyboard_preset;
		s_keyboard_input_preferences keyboard_preferences;
	};
	static_assert(sizeof(s_control_settings) == 0x19C);

	int32 version;
	c_enum<e_language, int32, _language_invalid, k_language_count> last_language;
	c_enum<e_language, int32, _language_invalid, k_language_count> last_font_language;
	s_file_last_modification_date last_fonts_modification_date;
	int32 build_number;
	real32 betrayal_count; // betrayal_penalty?
	int32 eviction_count;
	c_enum<e_gui_game_mode, int32, _ui_game_mode_none, k_gui_game_setup_mode_count> last_main_menu_item;
	int32 last_title_menu_item;
	s_gui_game_setup_storage last_game_setup;
	e_gui_network_session_advertisement_mode xbox_live_private_privacy_setting;
	uns32 hardware_crc;
	int32 adapter;
	bool fullscreen;
	int32 aspect_ratio;
	int32 brightness;
	int32 contrast;
	s_video_settings video_settings;
	bool hide_watermark;
	c_enum<e_subtitle_setting, int32, _subtitle_setting_automatic, k_number_of_subtitle_settings> subtitle_setting;
	int32 content_item_unique_name_number_seed[14];
	s_sound_settings sound_settings;
	s_gameplay_settings gameplay_settings;
	s_control_settings controls_settings;
};
static_assert(sizeof(s_global_preferences_data) == 0x41DF0);

struct s_global_preferences
{
	s_network_http_request_hash secure_signature;
	byte __data14[0x4];
	s_global_preferences_data data;
	byte __data41E08[0x1F8];
};
static_assert(sizeof(s_global_preferences) == 0x42000);

struct s_global_preferences_internals_type
{
	bool initialized;
	bool dirty;
	s_file_handle file;
	s_global_preferences current;
	s_global_preferences writing;
	int32 task;
	c_synchronized_long ready_to_write;
	bool need_utility_drive_flush;
};
static_assert(sizeof(s_global_preferences_internals_type) == 0x84018);

extern s_global_preferences_internals_type& g_global_preferences_internal;

extern s_global_preferences_internals_type* __cdecl global_preferences_get();
extern void __cdecl global_preferences_dirty(bool dirty);
extern bool __cdecl global_preferences_are_dirty();
extern bool __cdecl global_preferences_available();
extern void __cdecl global_preferences_clear();
extern void __cdecl global_preferences_create_default();
extern void __cdecl global_preferences_dispose();
extern void __cdecl global_preferences_flush();
extern bool __cdecl global_preferences_get_antialiasing();
extern int32 __cdecl global_preferences_get_aspect_ratio();
extern real32 __cdecl global_preferences_get_betrayal_count();
extern int32 __cdecl global_preferences_get_build_number();
extern real32 __cdecl global_preferences_get_camera_fov();
extern int32 __cdecl global_preferences_get_controls_method();
extern s_keyboard_input_preferences* __cdecl global_preferences_get_keyboard_preferences(s_keyboard_input_preferences* preferences);
extern void __cdecl global_preferences_bindings_create_default(s_keyboard_input_preferences* preferences);
extern void __cdecl global_preferences_gameplay_settings_create_default(s_global_preferences_data::s_gameplay_settings* gameplay_settings);
extern void __cdecl global_preferences_control_settings_create_default(s_global_preferences_data::s_control_settings* control_settings);
extern void __cdecl global_preferences_sound_settings_create_default(s_global_preferences_data::s_sound_settings* sound_settings);
extern e_quality_setting __cdecl global_preferences_get_details_quality();
extern int32 __cdecl global_preferences_get_display_brightness();
extern int32 __cdecl global_preferences_get_display_contrast();
extern bool __cdecl global_preferences_get_display_hints();
extern e_quality_setting __cdecl global_preferences_get_effects_quality();
extern int32 __cdecl global_preferences_get_eviction_count();
extern bool __cdecl global_preferences_get_fullscreen();
extern uns32 __cdecl global_preferences_get_hardware_crc();
extern bool __cdecl global_preferences_get_hud_shake();
extern int32 __cdecl global_preferences_get_keyboard_preset();
extern e_language __cdecl global_preferences_get_last_font_language();
extern void __cdecl global_preferences_get_last_fonts_modification_date(s_file_last_modification_date* last_fonts_modification_date);
extern s_gui_game_setup_storage* __cdecl global_preferences_get_last_game_setup();
extern bool __cdecl global_preferences_get_last_game_setup_map(e_gui_game_mode game_mode, int32* campaign_id, int32* map_id);
extern e_language __cdecl global_preferences_get_last_language();
extern e_gui_game_mode __cdecl global_preferences_get_last_main_menu_item();
extern e_quality_setting __cdecl global_preferences_get_lighting_quality();
extern int32 __cdecl global_preferences_get_master_volume();
extern bool __cdecl global_preferences_get_motion_blur();
extern int32 __cdecl global_preferences_get_mouse_acceleration();
extern bool __cdecl global_preferences_get_mouse_filter();
extern bool __cdecl global_preferences_get_mouse_inversion();
extern int32 __cdecl global_preferences_get_mouse_sensitivity_horizontal();
extern int32 __cdecl global_preferences_get_mouse_sensitivity_vehicle_horizontal();
extern int32 __cdecl global_preferences_get_mouse_sensitivity_vehicle_vertical();
extern int32 __cdecl global_preferences_get_mouse_sensitivity_vertical();
extern int32 __cdecl global_preferences_get_music_volume();
extern e_quality_setting __cdecl global_preferences_get_postprocessing_quality();
extern int32 __cdecl global_preferences_get_content_item_unique_name_number_seed(int32 content_item_type);
extern void __cdecl global_preferences_get_screen_resolution(int32* width, int32* height);
extern int32 __cdecl global_preferences_get_sfx_volume();
extern e_quality_setting __cdecl global_preferences_get_shadow_quality();
extern e_subtitle_setting __cdecl global_preferences_get_subtitle_setting();
extern int32 __cdecl global_preferences_get_team_color();
extern e_quality_setting __cdecl global_preferences_get_texture_filtering_quality();
extern e_quality_setting __cdecl global_preferences_get_texture_resolution_quality();
extern bool __cdecl global_preferences_get_toggle_crouch();
extern int32 __cdecl global_preferences_get_adapter();
extern int32 __cdecl global_preferences_get_unknown41BDC();
extern int32 __cdecl global_preferences_get_voice_chat_control();
extern int32 __cdecl global_preferences_get_voice_chat_volume();
extern int32 __cdecl global_preferences_get_voice_volume();
extern bool __cdecl global_preferences_get_vsync();
extern e_gui_network_session_advertisement_mode __cdecl global_preferences_get_xbox_live_private_privacy_setting();
extern void __cdecl global_preferences_init();
extern void __cdecl global_preferences_initialize();
extern void __cdecl global_preferences_invalidate_maps();
extern void __cdecl global_preferences_make_safe(s_global_preferences* preferences);
extern void __cdecl global_preferences_set_antialiasing(bool antialiasing);
extern void __cdecl global_preferences_set_aspect_ratio(bool aspect_ratio);
extern void __cdecl global_preferences_set_betrayal_count(real32 betrayal_count);
extern void __cdecl global_preferences_set_build_number(int32 build_number);
extern void __cdecl global_preferences_set_camera_fov(real32 camera_fov);
extern void __cdecl global_preferences_set_controls_method(int32 controls_method);
extern void __cdecl global_preferences_set_keyboard_preferences(s_keyboard_input_preferences* keyboard_preferences);
extern void __cdecl global_preferences_set_details_quality(e_quality_setting details_quality);
extern void __cdecl global_preferences_set_display_brightness(int32 brightness);
extern void __cdecl global_preferences_set_display_contrast(int32 contrast);
extern void __cdecl global_preferences_set_display_hints(bool display_hints);
extern void __cdecl global_preferences_set_effects_quality(e_quality_setting effects_quality);
extern void __cdecl global_preferences_set_eviction_count(int32 eviction_count);
extern void __cdecl global_preferences_set_fullscreen(bool fullscreen);
extern void __cdecl global_preferences_set_hardware_crc(uns32 crc);
extern void __cdecl global_preferences_set_hide_watermark(bool hide_watermark);
extern void __cdecl global_preferences_set_hud_shake(bool hud_shake);
extern void __cdecl global_preferences_set_keyboard_preset(int32 keyboard_preset);
extern void __cdecl global_preferences_set_last_font_language(e_language last_font_language);
extern void __cdecl global_preferences_set_last_fonts_modification_date(const s_file_last_modification_date* last_fonts_modification_date);
extern void __cdecl global_preferences_set_last_game_setup(const s_gui_game_setup_storage* last_game_setup);
extern void __cdecl global_preferences_set_last_language(e_language last_language);
extern void __cdecl global_preferences_set_last_main_menu_item(int32 last_main_menu_item);
extern void __cdecl global_preferences_set_lighting_quality(e_quality_setting lighting_quality);
extern void __cdecl global_preferences_set_master_volume(int32 master_volume);
extern void __cdecl global_preferences_set_motion_blur(bool motion_blur);
extern void __cdecl global_preferences_set_mouse_acceleration(int32 mouse_acceleration);
extern void __cdecl global_preferences_set_mouse_filter(bool mouse_filter);
extern void __cdecl global_preferences_set_mouse_inversion(bool mouse_invert);
extern void __cdecl global_preferences_set_mouse_sensitivity_horizontal(int32 mouse_sensitivity_horizontal);
extern void __cdecl global_preferences_set_mouse_sensitivity_vehicle_horizontal(int32 mouse_sensitivity_vehicle_horizontal);
extern void __cdecl global_preferences_set_mouse_sensitivity_vehicle_vertical(int32 mouse_sensitivity_vehicle_vertical);
extern void __cdecl global_preferences_set_mouse_sensitivity_vertical(int32 mouse_sensitivity_vertical);
extern void __cdecl global_preferences_set_music_volume(int32 music_volume);
extern void __cdecl global_preferences_set_postprocessing_quality(e_quality_setting postprocessing_quality);
extern void __cdecl global_preferences_set_content_item_unique_name_number_seed(int32 content_item_type, int32 seed);
extern void __cdecl global_preferences_set_screen_resolution(int32 width, int32 height);
extern void __cdecl global_preferences_set_sfx_volume(int32 sfx_volume);
extern void __cdecl global_preferences_set_shadow_quality(e_quality_setting shadow_quality);
extern void __cdecl global_preferences_set_subtitle_setting(e_subtitle_setting subtitle_setting);
extern void __cdecl global_preferences_set_team_color(int32 team_color);
extern void __cdecl global_preferences_set_texture_filtering_quality(e_quality_setting texture_filtering_quality);
extern void __cdecl global_preferences_set_texture_resolution_quality(e_quality_setting texture_resolution_quality);
extern void __cdecl global_preferences_set_toggle_crouch(bool toggle_crouch);
extern void __cdecl global_preferences_set_adapter(int32 adapter);
extern void __cdecl global_preferences_set_unknown41BDC(int32 unknown41BDC);
extern void __cdecl global_preferences_set_voice_chat_control(int32 voice_chat_control);
extern void __cdecl global_preferences_set_voice_chat_volume(int32 voice_chat_volume);
extern void __cdecl global_preferences_set_voice_volume(int32 voice_volume);
extern void __cdecl global_preferences_set_vsync(bool vsync);
extern void __cdecl global_preferences_set_xbox_live_private_privacy_setting(e_gui_network_session_advertisement_mode xbox_live_private_privacy_setting);
extern bool __cdecl global_preferences_get_hide_watermark();
extern void __cdecl global_preferences_update();
extern void __cdecl global_preferences_update_last_game_setup(s_gui_single_game_setup_storage* last_game_setup);
extern void __cdecl global_preferences_write();

extern void global_preferences_set_screen_resolution_hs(int32 width, int32 height);
extern void global_preferences_set_fullscreen_hs(bool fullscreen);
extern void global_preferences_set_master_volume_hs(int32 master_volume);
extern void global_preferences_set_music_volume_hs(int32 music_volume);
extern void global_preferences_set_sfx_volume_hs(int32 sfx_volume);

