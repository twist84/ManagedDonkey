#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct chud_widget_datum : s_datum_header
{
	byte __data[0x16];
};
static_assert(sizeof(chud_widget_datum) == 0x18);

struct chud_player_hud_elements
{
	bool __unknown0;
	bool __unknown1;
	bool show_crosshair;
	bool show_shield;
	bool show_grenades;
	bool show_messages;
	bool show_motion_sensor;
	bool show_spike_grenades;
	bool show_fire_grenades;
	bool show_compass;
	bool show_stamina;
	bool show_energy_meters;
	bool show_consumables;
};
static_assert(sizeof(chud_player_hud_elements) == 0xD);

struct c_chud_persistent_global_data
{
	byte __data[0x14D];
	chud_player_hud_elements player_hud;
	byte __unknown181[0x316];
};
static_assert(sizeof(c_chud_persistent_global_data) == 0x470);

struct c_chud_persistent_user_data
{
	byte __unknown0[0x14D];
	chud_player_hud_elements player_hud[4];
	byte __unknown181[0x273];
	byte bonus_round_show_timer;
	byte bonus_round_start_timer;
	byte __unknown3F6[2];
	long bonus_round_set_timer;
	long bonus_round_set_target_score;
	byte __unknown3FC[0xF640];
};
static_assert(sizeof(c_chud_persistent_user_data) == 0xFA40);

struct s_chud_navpoint
{
	long object_index;
	long __unknown4;
	long __unknown8;
	long __unknownC;
	dword_flags flags;
	real __unknown14;
	wchar_t str[6];
	real_point3d position;
};
static_assert(sizeof(s_chud_navpoint) == 0x30);

extern void __cdecl chud_submit_navpoint(long, s_chud_navpoint* navpoint);

