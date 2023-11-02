#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct s_user_interface_widget_reference
{
	c_typed_tag_reference<GUI_SCREEN_WIDGET_DEFINITION_TAG> widget_tag;
};
static_assert(sizeof(s_user_interface_widget_reference) == sizeof(s_tag_reference));

struct s_user_interface_tag_globals
{
	static tag const k_group_tag = USER_INTERFACE_GLOBALS_DEFINITION_TAG;

	// Shared Globals
	// This is a reference to the ui shared globals tag
	c_typed_tag_reference<USER_INTERFACE_SHARED_GLOBALS_DEFINITION_TAG> shared_globals;

	// Multiplayer Variant Settings Interface
	// This blob defines the ui for setting multiplayer game variant parameters
	c_typed_tag_reference<MULTIPLAYER_VARIANT_SETTINGS_INTERFACE_DEFINITION_TAG> mp_variant_settings_ui;

	// Game Hopper Localization Strings
	// This is for the loc game hopper strings
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG> game_hopper_descriptions;

	// Screen Widgets
	// These are the HaloX screen widgets
	c_typed_tag_block<s_user_interface_widget_reference> halox_screen_widgets;

	// Render Textures
	// These are Saber tags
	c_typed_tag_reference<TEXTURE_RENDER_LIST_TAG> texture_render_list;
};
static_assert(sizeof(s_user_interface_tag_globals) == 0x4C);

struct s_campaign_character
{
	char biped_name[32];
	char ai_squad_name[32];
	c_string_id ai_start_pos;
};
static_assert(sizeof(s_campaign_character) == 0x44);

struct s_user_interface_shared_globals
{
	static tag const k_group_tag = USER_INTERFACE_SHARED_GLOBALS_DEFINITION_TAG;

	// UI Rendering Globals
	// miscellaneous rendering globals, more below...
	short inc_text_update_period;
	short inc_text_block_character;
	real near_clip_plane_distance;
	real projection_plane_distance;
	real far_clip_plane_distance;

	// Global Text Strings
	// Global UI Text goes here
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG> unicode_string_list_tag;
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG> unicode_damage_reporting_string_list_tag;
	c_typed_tag_reference<MULTILINGUAL_UNICODE_STRING_LIST_TAG> unicode_input_string_list_tag;

	// Main menu music
	// Looping sound that plays while the main menu is active
	c_typed_tag_reference<SOUND_LOOPING_TAG> main_menu_music;
	long music_fade_time; // milliseconds

	// Default Text and Shadow Colors
	// These are the default values used for text glyphs and text shadows
	real_argb_color text_color;
	real_argb_color shadow_color;

	// Color presets
	// Colors are defined here and used in text widgets throughout the UI.
	// Specify a string_id here and the same one in a text widget.
	// If you don't specify one in a text widget, you'll use the first entry here.
	s_tag_block color_presets;

	// Tint colors
	// These colors are used for tinting widgets by team or player color.
	s_tag_block tint_colors;

	// User interface sounds
	// Default sound effects for various UI actions
	c_typed_tag_reference<USER_INTERFACE_SOUNDS_DEFINITION_TAG> default_sounds;

	// HaloX Alerts
	// Alerts are triggered by specific events that happen in the game.
	// The game can require that an error be resolved before the dialog can be dismissed.
	// Once the error is resolved (or if it doesn't require resolution,) the user can dismiss it.
	c_typed_tag_block<struct s_gui_alert_description> alert_descriptions;

	// HaloX Dialogs
	// Dialogs are brought up by individual screens or UI systems to ask the user a question, typically of the Ok/Cancel sort such as: 'Are you sure you want to leave the game?'.
	// Notes:
	// - first item defaults to 'ok', second item defaults to 'cancel.  third and fourth items don't display if they're not set
	// - if 'b button action' is set to 'button ignored' then the b button doesn't do anything (such as cancelling)
	s_tag_block dialog_descriptions;

	// Global datasources
	// Datasources that are available from anywhere in the UI
	s_tag_block global_datasources;

	// Content Aspect Ratio Scaling
	// These define the scaling that will get applied to UI content and animations when running at the specified display modes
	vector2d authoring_aspect_conversion_scaling[2]; // 16x9, 4x3

	// Blur Constants
	// These control the blur kernel used when blurring overlayed UI
	real horizontal_blur_factor;
	real vertical_blur_factor;

	// Player Appearance Bipeds
	// One element per player model choice.
	// Each specifies the bipeds needed for player appearance (customization and roster).
	s_tag_block player_appearance_bipeds;

	// Hollywood Squares Flags
	// These are the names of the cinematic cutscene flags used to place player appearance models in the Hollywood Squares (for display in the roster).
	c_string_id player0_flag;
	c_string_id player1_flag;
	c_string_id player2_flag;
	c_string_id player3_flag;

	// Hollywood Squares Camera Alignment Biped
	// These fields specify the biped that stands behind the Hollywood Squares, and on which the camera will be focused to capture the Hollywood Squares.
	// If you want to use a scenario-placed biped (non-AI), enter the biped name in the 'biped name' field.
	// If you want to use an AI-placed biped, enter the squad name string and start position string_id.
	s_campaign_character camera_alignment_biped;

	// Playable Campaign Characters
	// These fields specify the bipeds that show in the campaign lobby roster for the first player.
	// If you want to use a scenario-placed biped (non-AI), enter the biped name in the 'biped name' field.
	// If you want to use an AI-placed biped, enter the squad name string and start position string_id.
	s_campaign_character rookie_biped;
	s_campaign_character mickey_biped;
	s_campaign_character romeo_biped;
	s_campaign_character dutch_biped;
	s_campaign_character buck_biped;

	// Extra Campaign Characters
	// These fields specify the bipeds that show in the campaign lobby roster for players 2-4.
	// If you want to use a scenario-placed biped (non-AI), enter the biped name in the 'biped name' field.
	// If you want to use an AI-placed biped, enter the squad name string and start position string_id.
	s_campaign_character odst02_biped;
	s_campaign_character odst03_biped;
	s_campaign_character odst04_biped;

	// UI Navigation speed
	// This controls tabbing around the UI.
	// The tab delay is how fast you move around normally.
	// Tab fast wait is how long the UI waits till using the tab fast delay instead of the tab delay.
	long navigation_tab_delay_msec;
	long navigation_tab_fast_wait_msec;
	long navigation_tab_fast_delay_msec;

	// Attract Mode Settings
	// These control behavior of the attract mode movies
	long delay; // seconds

	// PDA Settings
	// Random globals for the PDA
	s_tag_data beacon_bitmap_zoom_scale;
	real camera_velocity; // pixels per second
	real camera_auto_move_delay;
	real player_offscreen_marker_tolerance; // pixels

	// area around you where the PDA doesn't auto-orient you
	real player_orientation_dead_zone; // pixels

	real ai_clump_culling_distance; // wu
	real_argb_color fourth_wall_played_color;
	real_argb_color fourth_wall_unplayed_color;
	c_string_id pda_poi_waypoint_prefix;
	c_string_id pda_poi_waypoint_suffix;
	long pda_back_button_latch_ticks;
	real pda_fourth_wall_text_fade_time;
	s_tag_block pda_fourth_wall_audio_sounds;
};
static_assert(sizeof(s_user_interface_shared_globals) == 0x3CC);

extern real g_ui_time_scale;
extern real g_ui_time_step;

extern bool __cdecl get_alpha_custom_games_disabled();
extern bool __cdecl get_alpha_eula_accepted();
extern bool __cdecl get_alpha_is_external_beta();
extern bool __cdecl get_alpha_is_internal_beta();
extern bool __cdecl get_alpha_is_locked_down();
extern long __cdecl get_alpha_locked_down_state();
extern bool __cdecl get_is_alpha_version();
extern void __cdecl user_interface_scoreboard_update();
extern s_user_interface_shared_globals const* user_interface_shared_tag_globals_try_and_get();
extern bool __cdecl user_interface_should_show_console_scoreboard(long* user_interface_show_busy_state);
extern s_user_interface_tag_globals const* __cdecl user_interface_tag_globals_try_and_get();
extern void __cdecl user_interface_update(real ui_time);

