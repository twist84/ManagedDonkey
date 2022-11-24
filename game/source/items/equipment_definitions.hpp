#pragma once

enum e_equipment_type
{
	_equipment_type_super_shield = 0,
	_equipment_type_multiplayer_powerup,
	_equipment_type_spawner,
	_equipment_type_proximity_mine,
	_equipment_type_motion_tracker_noise,
	_equipment_type_showme,
	_equipment_type_invisibility,
	_equipment_type_invincibility,
	_equipment_type_tree_of_life,
	_equipment_type_health_pack,
	_equipment_type_forced_reload,
	_equipment_type_concussive_blast,
	_equipment_type_tank_mode,
	_equipment_type_mag_pulse,
	_equipment_type_hologram,
	_equipment_type_reactive_armor,
	_equipment_type_bomb_run,
	_equipment_type_armor_lock,
	_equipment_type_adrenaline,
	_equipment_type_lightning_strike,
	_equipment_type_scrambler,
	_equipment_type_weapon_jammer,
	_equipment_type_ammo_pack,
	_equipment_type_vision,

	k_equipment_type_count
};

extern e_equipment_type equipment_definition_get_type(long definition_index, long equipment_type_index);
extern bool equipment_definition_has_type(long definition_index, e_equipment_type equipment_type);

