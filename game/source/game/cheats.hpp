#pragma once

#include "cseries/cseries.hpp"
#include "input/input.hpp"

struct cheat_globals
{
	//bool deathless_player;
	//bool valhalla;
	//bool jetpack;
	bool infinite_ammo;
	//bool bump_possession;
	//bool super_jump;
	//bool reflexive_damage_effects;
	//bool medusa;
	//bool omnipotent;
	bool controller;
	bool bottomless_clip;
	//bool chevy;
	//bool porcupine;
	//byte active_camouflage_player_mapping[4];
	//bool infinite_equipment_energy;

	c_static_array<c_static_string<200>, k_controller_button_count> lines;
};

static cheat_globals cheat
{
	//.deathless_player = true,
	//.valhalla = true,
	//.jetpack = true,
	.infinite_ammo = true,
	//.bump_possession = true,
	//.super_jump = true,
	//.reflexive_damage_effects = true,
	//.medusa = true,
	//.omnipotent = true,
	.controller = true,
	.bottomless_clip = true
	//.chevy = true,
	//.porcupine = true,
	//.infinite_equipment_energy = true
};

struct s_game_input_state;

extern void patch_cheats();
extern void __cdecl cheats_initialize();
extern void __cdecl cheats_dispose();
extern void __cdecl cheats_initialize_for_new_map();
extern void __cdecl cheats_dispose_from_old_map();
extern void __cdecl cheats_load();
extern bool __cdecl cheats_process_gamepad(long controller_index, s_game_input_state const* input_state);
extern void __cdecl cheat_teleport_to_camera();