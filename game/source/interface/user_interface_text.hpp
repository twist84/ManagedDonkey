#pragma once

#include "cseries/cseries.hpp"
#include "text/unicode.hpp"

#define UTF32_STRING(name) wchar_t name[2]{}; utf32_to_string(_utf32_##name, name);

enum e_utf32 : dword
{
	_utf32_ampersand = '\u0026',

	_utf32_a_button = 0xE100,
	_utf32_b_button = 0xE101,
	_utf32_x_button = 0xE102,
	_utf32_y_button = 0xE103,
	_utf32_left_bumper = 0xE104,
	_utf32_right_bumper = 0xE105,
	_utf32_left_trigger = 0xE106,
	_utf32_right_trigger = 0xE107,
	_utf32_dpad_up = 0xE108,
	_utf32_dpad_down = 0xE109,
	_utf32_dpad_left = 0xE10A,
	_utf32_dpad_right = 0xE10B,
	_utf32_start_button = 0xE10C,
	_utf32_back_button = 0xE10D,
	_utf32_left_thumb = 0xE10E,
	_utf32_right_thumb = 0xE10F,
	_utf32_left_stick = 0xE110,
	_utf32_right_stick = 0xE111,

	_utf32_assault_rifle = 0xE112,
	_utf32_ball = 0xE113,
	_utf32_battle_rifle = 0xE114,
	_utf32_bomb = 0xE115,
	_utf32_brute_shot = 0xE116,
	_utf32_carbine = 0xE117,
	_utf32_claymore_grenade = 0xE118,
	_utf32_energy_sword = 0xE119,
	_utf32_excavator = 0xE11A,
	_utf32_firebomb_grenade = 0xE11B,
	_utf32_flag = 0xE11C,
	_utf32_flamethrower = 0xE11D,
	_utf32_frag_grenade = 0xE11E,
	_utf32_fuel_rod_gun = 0xE11F,
	_utf32_gravity_hammer = 0xE120,
	_utf32_guardian_eye_beam = 0xE121,
	_utf32_machinegun_turret = 0xE122,
	_utf32_magnum = 0xE123,
	_utf32_missile_launcher = 0xE124,
	_utf32_needler = 0xE125,
	_utf32_particle_beam_rifle = 0xE126,
	_utf32_plasma_grenade = 0xE127,
	_utf32_plasma_mortar_turret = 0xE128,
	_utf32_plasma_pistol = 0xE129,
	_utf32_plasma_rifle = 0xE12A,
	_utf32_plasma_turret = 0xE12B,
	_utf32_pump_laser = 0xE12C,
	_utf32_rocket_launcher = 0xE12D,
	_utf32_sentinel_beam = 0xE12E,
	_utf32_shotgun = 0xE12F,
	_utf32_smg = 0xE130,
	_utf32_sniper_rifle = 0xE131,
	_utf32_spike_rifle = 0xE132,

	_utf32_mapname = 0xE40B,
	_utf32_variant = 0xE40D,
	_utf32_motion_sensor_enabled = 0xE40E,
	_utf32_teams_enabled = 0xE40F,

	_utf32_vehicle_set = 0xE410,
	_utf32_weapon_set = 0xE411,
	_utf32_player0_profile_name = 0xE412,
	_utf32_player1_profile_name = 0xE413,
	_utf32_player2_profile_name = 0xE414,
	_utf32_player3_profile_name = 0xE415,
	_utf32_countdown = 0xE416,
	_utf32_name_of_last_person_to_delay_countdown = 0xE417,
	_utf32_build_number = 0xE418,
	_utf32_leader = 0xE419,
	_utf32_qos_probes_sent = 0xE41A,
	_utf32_qos_probes_received = 0xE41B,
	_utf32_qos_msec_minimum = 0xE41C,
	_utf32_qos_msec_mediam = 0xE41D,
	_utf32_qos_bandwidth_downstream_bps = 0xE41E,
	_utf32_qos_bandwidth_upstream_bps = 0xE41F,

	_utf32_qos_max_machine_count = 0xE420,
	_utf32_qos_nat_type = 0xE421,
	// 0xE422
	// 0xE423
	// 0xE424
	_utf32_hud_quantity = 0xE425,
	_utf32_ge_round_time_left = 0xE426,
	_utf32_target_player_gamertag = 0xE427,
	_utf32_game_player_count = 0xE428,
	_utf32_coop_level_name = 0xE429,
	_utf32_coop_level_description = 0xE42A,
	_utf32_coop_difficulty = 0xE42B,
	_utf32_player0_gamertag = 0xE42C,
	_utf32_player1_gamertag = 0xE42D,
	_utf32_player2_gamertag = 0xE42E,
	_utf32_player3_gamertag = 0xE42F,

	_utf32_hud_scoreboard_player_score = 0xE430,
	_utf32_hud_scoreboard_other_player_score = 0xE431,
	_utf32_hud_scoreboard_variant_name = 0xE432,
	_utf32_hud_scoreboard_time_left = 0xE433,
	// 0xE434
	_utf32_qos_bandwidth_bps = 0xE435,
	// 0xE436
	// 0xE437
	// 0xE438
	_utf32_hopper_name = 0xE439,
	_utf32_hopper_description = 0xE43A,
	// 0xE43B
	// 0xE43C
	_utf32_hour_12 = 0xE43D,
	_utf32_hour_24 = 0xE43E,
	_utf32_minute = 0xE43F,
	_utf32_day = 0xE440,
	_utf32_month = 0xE441,
	_utf32_year = 0xE442,

	_utf32_button_jump = 0xE443,
	_utf32_button_switch_grenade = 0xE444,
	_utf32_button_action_reload = 0xE445,
	_utf32_button_switch_weapon = 0xE446,
	_utf32_button_melee_attack = 0xE447,
	_utf32_button_flashlight = 0xE448,
	_utf32_button_throw_grenade = 0xE449,
	_utf32_button_fire = 0xE44A,
	_utf32_button_start = 0xE44B,
	_utf32_button_back = 0xE44C,
	_utf32_button_crouch = 0xE44D,
	_utf32_button_scope_zoom = 0xE44E,
	_utf32_button_lean_left = 0xE44F,

	_utf32_button_lean_right = 0xE450, // sprint
	_utf32_button_accept = 0xE451,
	_utf32_button_cancel = 0xE452,
	_utf32_stick_move = 0xE453,
	_utf32_stick_look = 0xE454,
	_utf32_hud_betraying_player = 0xE455,
	_utf32_animating_left_thumbstick = 0xE456,
	_utf32_animating_right_thumbstick = 0xE457,
	_utf32_matchmaking_countdown = 0xE458,
	// 0xE459
	_utf32_map_load_percent = 0xE45A,
	_utf32_button_dual_fire = 0xE45B,
	_utf32_button_action_generic = 0xE45C,
	_utf32_button_action_weapon_primary = 0xE45D,
	_utf32_button_action_weapon_secondary = 0xE45E,
	_utf32_button_action_use_primary = 0xE45F,

	_utf32_button_action_use_secondary = 0xE460,
	_utf32_button_iteract_action = 0xE461,
	_utf32_button_push_to_talk = 0xE462,
	_utf32_button_use_equipment = 0xE463,
	_utf32_bonus_round_target_score = 0xE464
};

enum e_controller_index;
struct c_font_cache_base;
struct c_user_interface_text
{
public:
	virtual ~c_user_interface_text();
	virtual void set_string(wchar_t const* _string, bool parse_xml, long);
	virtual wchar_t const* get_string();
	virtual void update(long);

	void set_argb_color(real_argb_color* color);
	void set_font(long font);
	void set_justification(long justification);
	void set_style(long style);
	void set_flags(dword_flags flags);
	void set_controller_index(e_controller_index controller_index);
	void set_drop_shadow_style(long drop_shadow_style);
	void set_rotation_origin(real x, real y);
	void set_rotation(real rotation);
	void set_scroll_amount(real i, real j);
	void set_scale(real scale);

protected:
	// FLAG(0),  0x1: render_uppercase
	// FLAG(1),  0x2: align_vertically
	// FLAG(2),  0x4: wrap_horizontally
	// FLAG(3),  0x8: has_overflowed
	// FLAG(4), 0x10: ?
	// FLAG(5), 0x20: get_string_was_set
	dword_flags m_flags;

	e_controller_index m_controller_index;
	long m_font;
	argb_color m_argb_color;
	long m_drop_shadow_style;
	long m_style;
	long m_justification;
	short m_tab_stops[16];
	long m_tab_stop_count;
	real_point2d m_rotation_origin;
	real m_rotation;
	vector2d m_scroll_amount;
	real m_scale;
};
static_assert(sizeof(c_user_interface_text) == 0x5C);

struct s_parse_text_entry
{
	wchar_t* name;
	dword magic_character;
	void(__cdecl* parse_proc)(wchar_t*, e_controller_index controller_index, dword magic_character, c_static_wchar_string<1024>* buffer);
};

extern s_parse_text_entry(&g_parse_text_table)[131];

extern void wchar_string_sanitize_for_game(wchar_t* string, long maximum_character_count);
extern void utf32_to_string(e_utf32 utf32, wchar_t(&out_string)[2]);
extern void __cdecl parse_build_number_string(e_controller_index controller_index, e_utf32 utf32, c_static_wchar_string<1024>* out_string);
extern bool __cdecl parse_lobby_privacy(void* this_ptr, wchar_t* buffer, long buffer_length);
extern bool __cdecl parse_lobby_coop_max_players(void* this_ptr, wchar_t* buffer, long buffer_length);
extern bool __cdecl parse_lobby_countdown_remaining(void* this_ptr, wchar_t* buffer, long buffer_length);
extern bool __cdecl parse_lobby_current_players(void* this_ptr, wchar_t* buffer, long buffer_length);
extern bool __cdecl parse_lobby_delaying_player(void* this_ptr, wchar_t* buffer, long buffer_length);
extern bool __cdecl parse_lobby_film_max_players(void* this_ptr, wchar_t* buffer, long buffer_length);
extern bool __cdecl parse_lobby_film_party_leader_requirement(void* this_ptr, wchar_t* buffer, long buffer_length);
extern bool __cdecl parse_lobby_header(void* this_ptr, wchar_t* buffer, long buffer_length);
extern bool __cdecl parse_lobby_max_players(void* this_ptr, wchar_t* buffer, long buffer_length);
extern bool __cdecl parse_lobby_network(void* this_ptr, wchar_t* buffer, long buffer_length);
extern bool __cdecl parse_lobby_party_leader(void* this_ptr, wchar_t* buffer, long buffer_length);
extern bool __cdecl parse_lobby_percent_loaded(void* this_ptr, wchar_t* buffer, long buffer_length);
extern bool __cdecl parse_lobby_start_button_name(void* this_ptr, wchar_t* buffer, long buffer_length);
extern bool __cdecl parse_lobby_title(void* this_ptr, wchar_t* buffer, long buffer_length);

