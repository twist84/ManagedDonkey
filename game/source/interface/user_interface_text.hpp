#pragma once

#include "cseries/cseries.hpp"
#include "interface/c_gui_widget.hpp"
#include "text/unicode.hpp"

class c_font_cache_base;

#define UNICODE_PRIVATE_FONT_ICON_STRING(name) wchar_t name[2]{}; utf32_to_string(_unicode_private_font_icon_##name, name);

enum e_utf32 : uns32
{
	_unicode_utf32_identifier_character = 0xFEFFFFFF,
	_unicode_utf32_byte_swapped_character = 0xFFFFFFFE,

	_unicode_control_character_start_of_heading = 0x1,
	_unicode_control_character_start_of_text = 0x2,
	_unicode_control_character_end_of_text = 0x3,
	_unicode_control_character_end_of_transmission = 0x4,
	_unicode_control_character_enquiry = 0x5,
	_unicode_control_character_acknowledge = 0x6,
	_unicode_control_character_bell = 0x7,
	_unicode_control_character_backspace = 0x8,
	_unicode_control_character_line_tabulation = 0xB,
	_unicode_control_character_form_feed = 0xC,
	_unicode_control_character_shift_out = 0xE,
	_unicode_control_character_shift_in = 0xF,
	_unicode_control_character_data_link_escape = 0x10,
	_unicode_control_character_device_control_one = 0x11,
	_unicode_control_character_device_control_two = 0x12,
	_unicode_control_character_device_control_three = 0x13,
	_unicode_control_character_device_control_four = 0x14,
	_unicode_control_character_negative_acknowledge = 0x15,
	_unicode_control_character_synchronous_idle = 0x16,
	_unicode_control_character_end_of_transmission_block = 0x17,
	_unicode_control_character_cancel = 0x18,
	_unicode_control_character_end_of_medium = 0x19,
	_unicode_control_character_substitute = 0x1A,
	_unicode_control_character_escape = 0x1B,
	_unicode_control_character_information_seperator_four = 0x1C,
	_unicode_control_character_information_seperator_three = 0x1D,
	_unicode_control_character_information_seperator_two = 0x1E,
	_unicode_control_character_information_seperator_one = 0x1F,

	_unicode_end_of_line = 0xD,
	_unicode_end_of_column = 0x9,
	_unicode_end_of_string = 0x0,
	_unicode_newline_ignored = 0xA,

	_unicode_character_latin_small_letter_e_with_acute = 0xE9,
	_unicode_character_latin_capital_letter_e_with_acute = 0xC9,
	_unicode_character_latin_small_letter_o_with_acute = 0xF3,
	_unicode_character_latin_capital_letter_o_with_acute = 0xD3,
	_unicode_character_not_found = 0x25A1,
	_unicode_character_black_star = 0x2605,
	_unicode_character_white_star = 0x2606,
	_unicode_character_ballot_box = 0x2610,
	_unicode_character_ballot_box_with_check = 0x2611,
	_unicode_character_white_frowning_face = 0x2639,
	_unicode_character_white_smiling_face = 0x263A,
	_unicode_character_black_smiling_face = 0x263B,
	_unicode_character_heavy_check_mark = 0x2714,
	_unicode_character_ballot_x = 0x2717,
	_unicode_character_white_flag = 0x2690,
	_unicode_character_black_flag = 0x2691,

	k_first_unicode_private_use_character = 0xE000,

	_unicode_character_private_use_winking_face = 0xE000,
	_unicode_character_private_use_open_mouth = 0xE001,
	_unicode_character_private_use_neutral_face = 0xE002,
	_unicode_character_private_use_taunt_face = 0xE004,
	_unicode_character_private_use_content_available = 0xE007,
	_unicode_character_private_use_lock_icon = 0xE008,

	_unicode_private_font_icon_a_button = 0xE100,
	_unicode_private_font_icon_b_button = 0xE101,
	_unicode_private_font_icon_x_button = 0xE102,
	_unicode_private_font_icon_y_button = 0xE103,
	_unicode_private_font_icon_left_bumper = 0xE104,
	_unicode_private_font_icon_right_bumper = 0xE105,
	_unicode_private_font_icon_left_trigger = 0xE106,
	_unicode_private_font_icon_right_trigger = 0xE107,
	_unicode_private_font_icon_dpad_up = 0xE108,
	_unicode_private_font_icon_dpad_down = 0xE109,
	_unicode_private_font_icon_dpad_left = 0xE10A,
	_unicode_private_font_icon_dpad_right = 0xE10B,
	_unicode_private_font_icon_start_button = 0xE10C,
	_unicode_private_font_icon_back_button = 0xE10D,
	_unicode_private_font_icon_left_thumb = 0xE10E,
	_unicode_private_font_icon_right_thumb = 0xE10F,
	_unicode_private_font_icon_left_stick = 0xE110,
	_unicode_private_font_icon_right_stick = 0xE111,

	_unicode_private_font_icon_first_weapon = 0xE112,
	_unicode_private_font_icon_weapon_assault_rifle = 0xE112,
	_unicode_private_font_icon_weapon_ball = 0xE113,
	_unicode_private_font_icon_weapon_battle_rifle = 0xE114,
	_unicode_private_font_icon_weapon_bomb = 0xE115,
	_unicode_private_font_icon_weapon_brute_shot = 0xE116,
	_unicode_private_font_icon_weapon_carbine = 0xE117,
	_unicode_private_font_icon_weapon_claymore_grenade = 0xE118,
	_unicode_private_font_icon_weapon_energy_sword = 0xE119,
	_unicode_private_font_icon_weapon_excavator = 0xE11A,
	_unicode_private_font_icon_weapon_firebomb_grenade = 0xE11B,
	_unicode_private_font_icon_weapon_flag = 0xE11C,
	_unicode_private_font_icon_weapon_flamethrower = 0xE11D,
	_unicode_private_font_icon_weapon_frag_grenade = 0xE11E,
	_unicode_private_font_icon_weapon_fuel_rod_gun = 0xE11F,
	_unicode_private_font_icon_weapon_gravity_hammer = 0xE120,
	_unicode_private_font_icon_weapon_guardian_eye_beam = 0xE121,
	_unicode_private_font_icon_weapon_machinegun_turret = 0xE122,
	_unicode_private_font_icon_weapon_magnum = 0xE123,
	_unicode_private_font_icon_weapon_missile_launcher = 0xE124,
	_unicode_private_font_icon_weapon_needler = 0xE125,
	_unicode_private_font_icon_weapon_particle_beam_rifle = 0xE126,
	_unicode_private_font_icon_weapon_plasma_grenade = 0xE127,
	_unicode_private_font_icon_weapon_plasma_mortar_turret = 0xE128,
	_unicode_private_font_icon_weapon_plasma_pistol = 0xE129,
	_unicode_private_font_icon_weapon_plasma_rifle = 0xE12A,
	_unicode_private_font_icon_weapon_plasma_turret = 0xE12B,
	_unicode_private_font_icon_weapon_pump_laser = 0xE12C,
	_unicode_private_font_icon_weapon_rocket_launcher = 0xE12D,
	_unicode_private_font_icon_weapon_sentinel_beam = 0xE12E,
	_unicode_private_font_icon_weapon_shotgun = 0xE12F,
	_unicode_private_font_icon_weapon_smg = 0xE130,
	_unicode_private_font_icon_weapon_sniper_rifle = 0xE131,
	_unicode_private_font_icon_weapon_spike_rifle = 0xE132,
	_unicode_private_font_icon_last_weapon = 0xE132,

	_unicode_private_font_icon_animating_left_thumbstick0 = 0xE133,
	_unicode_private_font_icon_animating_left_thumbstick1 = 0xE134,
	_unicode_private_font_icon_animating_left_thumbstick2 = 0xE135,
	_unicode_private_font_icon_animating_right_thumbstick0 = 0xE136,
	_unicode_private_font_icon_animating_right_thumbstick1 = 0xE137,
	_unicode_private_font_icon_animating_right_thumbstick2 = 0xE138,

	k_last_unicode_private_font_icon = 0xE2FF,

	k_first_unicode_private_sequence_character = 0xE300,

	_unicode_private_use_set_font = 0xE300,
	_unicode_private_use_set_color = 0xE301,

	k_last_unicode_private_sequence_character = 0xE3FF,

	k_first_non_icon_private_use_character = 0xE400,

	_unicode_private_use_style_plain = 0xE400,
	_unicode_private_use_style_italic = 0xE401,
	_unicode_private_use_style_bold = 0xE402,
	_unicode_private_use_style_condense = 0xE403,
	_unicode_private_use_style_underline = 0xE404,
	_unicode_private_use_justification_left = 0xE405,
	_unicode_private_use_justification_right = 0xE406,
	_unicode_private_use_justification_center = 0xE407,
	_unicode_private_use_restore_font = 0xE408,
	_unicode_private_use_restore_color = 0xE409,
	_unicode_private_use_restore_justification = 0xE40A,
	_unicode_private_use_mapname = 0xE40B,
	_unicode_private_use_gametype = 0xE40C,
	_unicode_private_use_variant = 0xE40D,
	_unicode_private_use_motion_sensor_enabled = 0xE40E,
	_unicode_private_use_teams_enabled = 0xE40F,
	_unicode_private_use_vehicle_set = 0xE410,
	_unicode_private_use_weapon_set = 0xE411,
	_unicode_private_use_player0_profile_name = 0xE412,
	_unicode_private_use_player1_profile_name = 0xE413,
	_unicode_private_use_player2_profile_name = 0xE414,
	_unicode_private_use_player3_profile_name = 0xE415,
	_unicode_private_use_countdown = 0xE416,
	_unicode_private_use_name_of_last_person_to_delay_countdown = 0xE417,
	_unicode_private_use_build_number = 0xE418,
	_unicode_private_use_leader = 0xE419,
	_unicode_private_use_live_service_qos_probes_sent = 0xE41A,
	_unicode_private_use_live_service_qos_probes_received = 0xE41B,
	_unicode_private_use_live_service_qos_ping_minimum = 0xE41C,
	_unicode_private_use_live_service_qos_ping_median = 0xE41D,
	_unicode_private_use_live_service_qos_bandwidth_downstream = 0xE41E,
	_unicode_private_use_live_service_qos_bandwidth_upstream = 0xE41F,
	_unicode_private_use_live_max_machines = 0xE420,
	_unicode_private_use_live_nat_type = 0xE421,
	_unicode_private_use_active_multiplayer_protocol = 0xE422,
	_unicode_private_use_live_ui_driver_gamertag = 0xE423,
	_unicode_private_use_live_ui_driver_clan_name = 0xE424,
	_unicode_private_use_hud_quantity = 0xE425,
	_unicode_private_use_game_engine_round_time_left = 0xE426,
	_unicode_private_use_target_player_gamertag = 0xE427,
	_unicode_private_use_network_game_player_count = 0xE428,
	_unicode_private_use_coop_level_name = 0xE429,
	_unicode_private_use_coop_level_description = 0xE42A,
	_unicode_private_use_coop_difficulty = 0xE42B,
	_unicode_private_use_player0_gamertag = 0xE42C,
	_unicode_private_use_player1_gamertag = 0xE42D,
	_unicode_private_use_player2_gamertag = 0xE42E,
	_unicode_private_use_player3_gamertag = 0xE42F,
	_unicode_private_use_hud_scoreboard_player_score = 0xE430,
	_unicode_private_use_hud_scoreboard_other_player_score = 0xE431,
	_unicode_private_use_hud_scoreboard_variant_name = 0xE432,
	_unicode_private_use_hud_scoreboard_time_left = 0xE433,
	_unicode_private_use_edit_player_profile_name = 0xE434,
	_unicode_private_use_live_estimated_bandwidth = 0xE435,
	_unicode_private_use_target_player_clan_name = 0xE436,
	_unicode_private_use_target_player_game_title_name = 0xE437,
	_unicode_private_use_playlist = 0xE438,
	_unicode_private_use_hopper_name = 0xE439,
	_unicode_private_use_hopper_description = 0xE43A,
	_unicode_private_use_target_player_clan_level = 0xE43B,
	_unicode_private_use_live_ui_driver_clan_level = 0xE43C,
	_unicode_private_use_hour_12 = 0xE43D,
	_unicode_private_use_hour_24 = 0xE43E,
	_unicode_private_use_minute = 0xE43F,
	_unicode_private_use_day = 0xE440,
	_unicode_private_use_month = 0xE441,
	_unicode_private_use_year = 0xE442,
	_unicode_private_use_button_jump = 0xE443,
	_unicode_private_use_button_switch_grenade = 0xE444,
	_unicode_private_use_button_action_reload = 0xE445,
	_unicode_private_use_button_switch_weapon = 0xE446,
	_unicode_private_use_button_melee_attack = 0xE447,
	_unicode_private_use_button_flashlight = 0xE448,
	_unicode_private_use_button_throw_grenade = 0xE449,
	_unicode_private_use_button_fire = 0xE44A,
	_unicode_private_use_button_start = 0xE44B,
	_unicode_private_use_button_back = 0xE44C,
	_unicode_private_use_button_crouch = 0xE44D,
	_unicode_private_use_button_scope_zoom = 0xE44E,
	_unicode_private_use_button_lean_left = 0xE44F,
	_unicode_private_use_button_lean_right = 0xE450, // sprint
	_unicode_private_use_button_accept = 0xE451,
	_unicode_private_use_button_cancel = 0xE452,
	_unicode_private_use_stick_move = 0xE453,
	_unicode_private_use_stick_look = 0xE454,
	_unicode_private_use_hud_betraying_player = 0xE455,
	_unicode_private_use_animating_left_thumbstick = 0xE456,
	_unicode_private_use_animating_right_thumbstick = 0xE457,
	_unicode_private_use_matchmaking_countdown = 0xE458,
	_unicode_private_use_new_content_available = 0xE459,
	_unicode_private_use_map_load_percent = 0xE45A,
	_unicode_private_use_button_fire_dual_weapon = 0xE45B,
	_unicode_private_use_button_action_generic = 0xE45C,
	_unicode_private_use_button_action_weapon_primary = 0xE45D,
	_unicode_private_use_button_action_weapon_secondary = 0xE45E,
	_unicode_private_button_action_use_primary = 0xE45F,
	_unicode_private_button_action_use_secondary = 0xE460,
	_unicode_private_button_iteract_action = 0xE461,
	_unicode_private_use_button_push_to_talk = 0xE462,
	_unicode_private_use_button_use_equipment = 0xE463,
	_unicode_private_bonus_round_target_score = 0xE464,

	k_last_unicode_private_use_character = 0xF8FF,
};

struct utf32
{
	e_utf32 character;
};

struct s_user_interface_text_render_data :
	s_gui_widget_render_data
{
	e_font_id font;
	e_text_style style;
	e_text_justification justification;
	e_text_drop_shadow_style drop_shadow_style;
	uns32 color;
	uns32 shadow_color;
	bool wrap_horizontally;
	bool align_vertically;
	int16 tab_stop_count;
	int16 tab_stops[16];
	real_point2d rotation_origin;
	real32 rotation_angle_radians;
	real32 glyph_scale;
	real_rectangle2d bounds_rect;
	real_rectangle2d clip_rect;
};
//COMPILE_ASSERT(sizeof(s_user_interface_text_render_data) == 0x98);
COMPILE_ASSERT(sizeof(s_user_interface_text_render_data) == sizeof(s_gui_widget_render_data) + 0x6C);

struct s_gui_text_widget_small_render_data :
	s_user_interface_text_render_data
{
	wchar_t text[48];
};
COMPILE_ASSERT(sizeof(s_gui_text_widget_small_render_data) == sizeof(s_user_interface_text_render_data) + (sizeof(wchar_t) * 48));

struct s_gui_text_widget_large_render_data :
	s_user_interface_text_render_data
{
	wchar_t text[256];
};
COMPILE_ASSERT(sizeof(s_gui_text_widget_large_render_data) == sizeof(s_user_interface_text_render_data) + (sizeof(wchar_t) * 256));

struct s_gui_text_widget_extra_large_render_data :
	s_user_interface_text_render_data
{
	wchar_t text[1024];
};
COMPILE_ASSERT(sizeof(s_gui_text_widget_extra_large_render_data) == sizeof(s_user_interface_text_render_data) + (sizeof(wchar_t) * 1024));

class c_user_interface_text
{
public:
	enum // m_flags
	{
		_interface_text_render_uppercase_bit = 0,
		_align_vertically_bit,
		_wrap_horizontally_bit,
		_has_overflowed_bit,
		_has_special_characters_bit,
		_string_was_set_bit,

		k_number_of_interface_text_flags,
	};

	virtual ~c_user_interface_text();
	virtual void set_string(const wchar_t* new_string, bool parse_xml, int32 screen_index);// = 0;
	virtual const wchar_t* get_string();// = 0;
	virtual void update(int32 user_index);// = 0;

	static void __cdecl render(s_user_interface_text_render_data* render_data, const rectangle2d* window_bounds);

	void set_argb_color(real_argb_color* color);
	void set_font(e_font_id font);
	void set_justification(e_text_justification justification);
	void set_style(e_text_style new_style);
	void set_flags(uns32 flags);
	void set_controller_index(e_controller_index controller_index);
	void set_drop_shadow_style(e_text_drop_shadow_style drop_shadow_style);
	void set_rotation_origin(real32 x, real32 y);
	void set_rotation(real32 rotation);
	void set_scroll_amount(real32 i, real32 j);
	void set_scale(real32 scale);

//protected:
	uns32 m_flags;

	e_controller_index m_controller_index;
	e_font_id m_font;
	argb_color m_argb_color;
	e_text_drop_shadow_style m_drop_shadow_style;
	e_text_style m_text_style;
	e_text_justification m_justification;
	int16 m_tab_stops[16];
	int32 m_tab_stop_count;
	real_point2d m_rotation_origin;
	real32 m_rotation;
	real_vector2d m_scroll_amount;
	real32 m_scale;
};
COMPILE_ASSERT(sizeof(c_user_interface_text) == 0x5C);

struct s_parse_text_entry
{
	wchar_t* name;
	uns32 magic_character;
	void(__cdecl* parse_proc)(wchar_t*, e_controller_index controller_index, uns32 magic_character, c_static_wchar_string<1024>* buffer);
};
COMPILE_ASSERT(sizeof(s_parse_text_entry) == 0xC);

struct s_last_known_session_state
{
	uns16 hopper_identifier;
	c_static_wchar_string<256> mapname;
	c_static_wchar_string<512> variant;
	c_static_wchar_string<256> leader;
	int32 player_count;
};
COMPILE_ASSERT(sizeof(s_last_known_session_state) == 0x808);

extern s_parse_text_entry(&g_parse_text_table)[131];
extern s_last_known_session_state& g_last_known_session_state;
extern bool g_render_text_as_font_index;

extern void wchar_string_sanitize_for_game(wchar_t* string, int32 maximum_character_count);
extern void utf32_to_string(e_utf32 utf32, wchar_t(&out_string)[2]);
extern void __cdecl parse_build_number_string(int32 user_index, e_utf32 character, c_static_wchar_string<1024>* text);
extern bool __cdecl parse_xml_ui_screen_party_privacy(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_lobby_coop_max_players(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_lobby_countdown_remaining(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_lobby_current_players(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_lobby_delaying_player(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_lobby_film_max_players(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_lobby_film_party_leader_requirement(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_lobby_header(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_lobby_max_players(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_lobby_network(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_lobby_party_leader(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_lobby_percent_loaded(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_lobby_start_button_name(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern bool __cdecl parse_xml_lobby_title(void* this_ptr, wchar_t* buffer, int32 buffer_length);
extern void user_interface_text_debug_display_font_index(bool display_font_index_in_place_of_text);

