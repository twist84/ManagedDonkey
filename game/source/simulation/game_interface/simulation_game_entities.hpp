#pragma once

enum e_simulation_entity_type
{
	_simulation_entity_type_slayer = 0,
	_simulation_entity_type_ctf,
	_simulation_entity_type_assault,
	_simulation_entity_type_oddball,
	_simulation_entity_type_king,
	_simulation_entity_type_territories,
	_simulation_entity_type_juggernaut,
	_simulation_entity_type_sandbox,
	_simulation_entity_type_infection,
	_simulation_entity_type_vip,
	_simulation_entity_type_game_engine_player,
	_simulation_entity_type_game_statborg,
	_simulation_entity_type_breakable_surface_group,
	_simulation_entity_type_map_variant,
	_simulation_entity_type_unit,
	_simulation_entity_type_item,
	_simulation_entity_type_generic,
	_simulation_entity_type_generic_garbage,
	_simulation_entity_type_vehicle,
	_simulation_entity_type_projectile,
	_simulation_entity_type_weapon,
	_simulation_entity_type_device,

	k_simulation_entity_type_count,

	k_simulation_entity_type_none = -1,
	k_simulation_entity_type_maximum_count = 32
};

