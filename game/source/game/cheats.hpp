#pragma once

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
	//bool controller;
	bool bottomless_clip;
	//bool chevy;
	//bool porcupine;
	//byte active_camouflage_player_mapping[4];
	//bool infinite_equipment_energy;
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
	//.controller = true,
	.bottomless_clip = true
	//.chevy = true,
	//.porcupine = true,
	//.infinite_equipment_energy = true
};

extern void cheat_teleport_to_camera();