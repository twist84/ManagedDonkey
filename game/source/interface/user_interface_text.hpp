#pragma once

#include "cseries/cseries.hpp"
#include "text/unicode.hpp"

#define MAGIC_CHARACTER(name) wchar_t name[2]{}; magic_character_to_string(_magic_character_##name, name);

enum e_magic_character : dword
{
	// is this actually part of `e_utf32`
	_magic_character_ampersand = '\u0026',

	_magic_character_a_button = 0xE100,
	_magic_character_b_button = 0xE101,
	_magic_character_x_button = 0xE102,
	_magic_character_y_button = 0xE103,
	_magic_character_left_bumper = 0xE104,
	_magic_character_right_bumper = 0xE105,
	_magic_character_left_trigger = 0xE106,
	_magic_character_right_trigger = 0xE107,
	_magic_character_dpad_up = 0xE108,
	_magic_character_dpad_down = 0xE109,
	_magic_character_dpad_left = 0xE10A,
	_magic_character_dpad_right = 0xE10B,
	_magic_character_start_button = 0xE10C,
	_magic_character_back_button = 0xE10D,
	_magic_character_left_thumb = 0xE10E,
	_magic_character_right_thumb = 0xE10F,
	_magic_character_left_stick = 0xE110,
	_magic_character_right_stick = 0xE111,

	_magic_character_assault_rifle = 0xE112,
	_magic_character_ball = 0xE113,
	_magic_character_battle_rifle = 0xE114,
	_magic_character_bomb = 0xE115,
	_magic_character_brute_shot = 0xE116,
	_magic_character_carbine = 0xE117,
	_magic_character_claymore_grenade = 0xE118,
	_magic_character_energy_sword = 0xE119,
	_magic_character_excavator = 0xE11A,
	_magic_character_firebomb_grenade = 0xE11B,
	_magic_character_flag = 0xE11C,
	_magic_character_flamethrower = 0xE11D,
	_magic_character_frag_grenade = 0xE11E,
	_magic_character_fuel_rod_gun = 0xE11F,
	_magic_character_gravity_hammer = 0xE120,
	_magic_character_guardian_eye_beam = 0xE121,
	_magic_character_machinegun_turret = 0xE122,
	_magic_character_magnum = 0xE123,
	_magic_character_missile_launcher = 0xE124,
	_magic_character_needler = 0xE125,
	_magic_character_particle_beam_rifle = 0xE126,
	_magic_character_plasma_grenade = 0xE127,
	_magic_character_plasma_mortar_turret = 0xE128,
	_magic_character_plasma_pistol = 0xE129,
	_magic_character_plasma_rifle = 0xE12A,
	_magic_character_plasma_turret = 0xE12B,
	_magic_character_pump_laser = 0xE12C,
	_magic_character_rocket_launcher = 0xE12D,
	_magic_character_sentinel_beam = 0xE12E,
	_magic_character_shotgun = 0xE12F,
	_magic_character_smg = 0xE130,
	_magic_character_sniper_rifle = 0xE131,
	_magic_character_spike_rifle = 0xE132,

	_magic_character_mapname = 0xE40B,
	_magic_character_variant = 0xE40D,
	_magic_character_motion_sensor_enabled = 0xE40E,
	_magic_character_teams_enabled = 0xE40F,

	_magic_character_vehicle_set = 0xE410,
	_magic_character_weapon_set = 0xE411,
	_magic_character_player0_profile_name = 0xE412,
	_magic_character_player1_profile_name = 0xE413,
	_magic_character_player2_profile_name = 0xE414,
	_magic_character_player3_profile_name = 0xE415,
	_magic_character_countdown = 0xE416,
	_magic_character_name_of_last_person_to_delay_countdown = 0xE417,
	_magic_character_build_number = 0xE418,
	_magic_character_leader = 0xE419,
	_magic_character_qos_probes_sent = 0xE41A,
	_magic_character_qos_probes_received = 0xE41B,
	_magic_character_qos_msec_minimum = 0xE41C,
	_magic_character_qos_msec_mediam = 0xE41D,
	_magic_character_qos_bandwidth_downstream_bps = 0xE41E,
	_magic_character_qos_bandwidth_upstream_bps = 0xE41F,

	_magic_character_qos_max_machine_count = 0xE420,
	_magic_character_qos_nat_type = 0xE421,
	// 0xE422
	// 0xE423
	// 0xE424
	_magic_character_hud_quantity = 0xE425,
	_magic_character_ge_round_time_left = 0xE426,
	_magic_character_target_player_gamertag = 0xE427,
	_magic_character_game_player_count = 0xE428,
	_magic_character_coop_level_name = 0xE429,
	_magic_character_coop_level_description = 0xE42A,
	_magic_character_coop_difficulty = 0xE42B,
	_magic_character_player0_gamertag = 0xE42C,
	_magic_character_player1_gamertag = 0xE42D,
	_magic_character_player2_gamertag = 0xE42E,
	_magic_character_player3_gamertag = 0xE42F,

	_magic_character_hud_scoreboard_player_score = 0xE430,
	_magic_character_hud_scoreboard_other_player_score = 0xE431,
	_magic_character_hud_scoreboard_variant_name = 0xE432,
	_magic_character_hud_scoreboard_time_left = 0xE433,
	// 0xE434
	_magic_character_qos_bandwidth_bps = 0xE435,
	// 0xE436
	// 0xE437
	// 0xE438
	_magic_character_hopper_name = 0xE439,
	_magic_character_hopper_description = 0xE43A,
	// 0xE43B
	// 0xE43C
	_magic_character_hour_12 = 0xE43D,
	_magic_character_hour_24 = 0xE43E,
	_magic_character_minute = 0xE43F,
	_magic_character_day = 0xE440,
	_magic_character_month = 0xE441,
	_magic_character_year = 0xE442,

	_magic_character_button_jump = 0xE443,
	_magic_character_button_switch_grenade = 0xE444,
	_magic_character_button_action_reload = 0xE445,
	_magic_character_button_switch_weapon = 0xE446,
	_magic_character_button_melee_attack = 0xE447,
	_magic_character_button_flashlight = 0xE448,
	_magic_character_button_throw_grenade = 0xE449,
	_magic_character_button_fire = 0xE44A,
	_magic_character_button_start = 0xE44B,
	_magic_character_button_back = 0xE44C,
	_magic_character_button_crouch = 0xE44D,
	_magic_character_button_scope_zoom = 0xE44E,
	_magic_character_button_lean_left = 0xE44F,

	_magic_character_button_lean_right = 0xE450, // sprint
	_magic_character_button_accept = 0xE451,
	_magic_character_button_cancel = 0xE452,
	_magic_character_stick_move = 0xE453,
	_magic_character_stick_look = 0xE454,
	_magic_character_hud_betraying_player = 0xE455,
	_magic_character_animating_left_thumbstick = 0xE456,
	_magic_character_animating_right_thumbstick = 0xE457,
	_magic_character_matchmaking_countdown = 0xE458,
	// 0xE459
	_magic_character_map_load_percent = 0xE45A,
	_magic_character_button_dual_fire = 0xE45B,
	_magic_character_button_action_generic = 0xE45C,
	_magic_character_button_action_weapon_primary = 0xE45D,
	_magic_character_button_action_weapon_secondary = 0xE45E,
	_magic_character_button_action_use_primary = 0xE45F,

	_magic_character_button_action_use_secondary = 0xE460,
	_magic_character_button_iteract_action = 0xE461,
	_magic_character_button_push_to_talk = 0xE462,
	_magic_character_button_use_equipment = 0xE463,
	_magic_character_bonus_round_target_score = 0xE464
};

struct c_font_cache_base;
struct c_user_interface_text
{
public:
	virtual ~c_user_interface_text();
	virtual void set_string(wchar_t const* _string, bool parse_xml, long);
	virtual wchar_t const* get_string();
	virtual void update(long);

	void set_flags(dword_flags flags)
	{
		m_flags = flags;
	}

	void set_unknown8(long unknown8)
	{
		__unknown8 = unknown8;
	}

	void set_font(long font)
	{
		m_font = font;
	}

	void set_argb_color(argb_color color)
	{
		m_argb_color = color;
	}

	void set_drop_shadow_style(long drop_shadow_style)
	{
		m_drop_shadow_style = drop_shadow_style;
	}

	void set_style(long style)
	{
		m_style = style;
	}

	void set_justification(long justification)
	{
		m_justification = justification;
	}

	void set_rotation_origin(real x, real y)
	{
		m_rotation_origin.x = x;
		m_rotation_origin.y = y;
	}

	void set_rotation(real rotation)
	{
		m_rotation = rotation;
	}

	void set_scroll_amount(real i, real j)
	{
		m_scroll_amount.i = i;
		m_scroll_amount.j = j;
	}

	void set_scale(real scale)
	{
		m_scale = scale;
	}

protected:
	// 1:  render_uppercase
	// 2:  align_vertically
	// 4:  wrap_horizontally
	// 8:  has_overflowed
	// 16: ?
	// 32: get_string_was_set
	dword_flags m_flags;

	long __unknown8;
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

extern void wchar_string_sanitize_for_game(wchar_t* string, long maximum_character_count);

enum e_controller_index;
enum e_utf32;

extern void __cdecl parse_build_number_string(e_controller_index controller_index, e_utf32 utf32, c_static_wchar_string<1024>* out_string);
extern void magic_character_to_string(e_magic_character magic_character, wchar_t(&magic_string)[2]);

