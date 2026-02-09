#include "items/equipment_definitions.hpp"

#include "cache/cache_files.hpp"
#include "memory/module.hpp"

//HOOK_DECLARE(0x00BA0260, equipment_definition_get_type);
//HOOK_DECLARE(0x00BA0360, equipment_definition_has_type);
//HOOK_DECLARE(0x00BA0390, equipment_get_adrenaline_definition);
//HOOK_DECLARE(0x00BA03B0, equipment_get_ammo_pack_definition);
//HOOK_DECLARE(0x00BA03D0, equipment_get_armor_lock_definition);
//HOOK_DECLARE(0x00BA03F0, equipment_get_bomb_run_definition);
//HOOK_DECLARE(0x00BA0410, equipment_get_concussive_blast_definition);
//HOOK_DECLARE(0x00BA0430, equipment_get_forced_reload_definition);
//HOOK_DECLARE(0x00BA0450, equipment_get_health_pack_definition);
//HOOK_DECLARE(0x00BA0470, equipment_get_hologram_definition);
//HOOK_DECLARE(0x00BA0490, equipment_get_invincibility_mode_definition);
//HOOK_DECLARE(0x00BA04B0, equipment_get_invisibility_mode_definition);
//HOOK_DECLARE(0x00BA04D0, equipment_get_lightning_strike_definition);
//HOOK_DECLARE(0x00BA04F0, equipment_get_mag_pulse_definition);
//HOOK_DECLARE(0x00BA0510, equipment_get_motion_tracker_noise_definition);
//HOOK_DECLARE(0x00BA0530, equipment_get_multiplayer_powerup_definition);
//HOOK_DECLARE(0x00BA0550, equipment_get_proximity_mine_definition);
//HOOK_DECLARE(0x00BA0570, equipment_get_reactive_armor_definition);
//HOOK_DECLARE(0x00BA0590, equipment_get_scrambler_definition);
//HOOK_DECLARE(0x00BA05B0, equipment_get_showme_definition);
//HOOK_DECLARE(0x00BA05D0, equipment_get_spawner_definition);
//HOOK_DECLARE(0x00BA05F0, equipment_get_super_shield_definition);
//HOOK_DECLARE(0x00BA0610, equipment_get_tank_mode_definition);
//HOOK_DECLARE(0x00BA0630, equipment_get_treeoflife_definition);
//HOOK_DECLARE(0x00BA0650, equipment_get_vision_definition);
//HOOK_DECLARE(0x00BA0650, equipment_get_weapon_jammer_definition);

e_equipment_type equipment_definition_get_type(int32 equipment_definition_index, int32 type_index)
{
	//return INVOKE(0x00BA0260, equipment_definition_get_type, equipment_definition_index, type_index);

	const e_equipment_type k_type_order[]
	{
		// specific equipment can be disabled with a comment below
	
		_equipment_type_super_shield,
		_equipment_type_multiplayer_powerup,
		_equipment_type_spawner,
		_equipment_type_proximity_mine,
		_equipment_type_motion_tracker_noise,
		_equipment_type_showme,
		_equipment_type_invisibility_mode,
		_equipment_type_invincibility,
		_equipment_type_treeoflife,
	
		// ODST
		//_equipment_type_health_pack,
		
		// Halo Online
		//_equipment_type_forced_reload,
		//_equipment_type_concussive_blast,
		//_equipment_type_tank_mode,
		//_equipment_type_mag_pulse,
		//_equipment_type_hologram,
		//_equipment_type_reactive_armor,
		//_equipment_type_bomb_run,
		//_equipment_type_armor_lock,
		//_equipment_type_adrenaline,
		//_equipment_type_lightning_strike,
		//_equipment_type_scrambler,
		//_equipment_type_weapon_jammer,
		//_equipment_type_ammo_pack,
		//_equipment_type_vision
	};
	COMPILE_ASSERT(NUMBEROF(k_type_order) == k_equipment_type_count);
	
	e_equipment_type result_equipment_type = _equipment_type_none;
	const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, equipment_definition_index);
	
	int32 type_count = 0;
	for (int32 type_blocks_index = 0; type_blocks_index < NUMBEROF(k_type_order); type_blocks_index++)
	{
		if (equipment_definition->equipment.tag_blocks[k_type_order[type_blocks_index]].count > 0)
		{
			if (type_count == type_index)
			{
				result_equipment_type = k_type_order[type_blocks_index];
				break;
			}
	
			type_count++;
		}
	}
	
	return result_equipment_type;
}

bool equipment_definition_has_type(int32 definition_index, e_equipment_type equipment_type)
{
	//return INVOKE(0x00BA0360, equipment_definition_has_type, definition_index, equipment_type);

	const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	const int32 equipment_type_index = equipment_type;
	ASSERT(VALID_INDEX(equipment_type_index, k_equipment_type_count));
	return equipment_definition->equipment.tag_blocks[equipment_type_index].count != 0;
}

const s_equipment_type_adrenaline* equipment_get_adrenaline_definition(int32 definition_index)
{
	return INVOKE(0x00BA0390, equipment_get_adrenaline_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_adrenaline* result = equipment_definition->equipment.adrenaline.begin();
	//return result;
}

const s_equipment_type_ammo_pack* equipment_get_ammo_pack_definition(int32 definition_index)
{
	return INVOKE(0x00BA03B0, equipment_get_ammo_pack_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_ammo_pack* result = equipment_definition->equipment.ammo_pack.begin();
	//return result;
}

const s_equipment_type_armor_lock* equipment_get_armor_lock_definition(int32 definition_index)
{
	return INVOKE(0x00BA03D0, equipment_get_armor_lock_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_armor_lock* result = equipment_definition->equipment.armor_lock.begin();
	//return result;
}

const s_equipment_type_bomb_run* equipment_get_bomb_run_definition(int32 definition_index)
{
	return INVOKE(0x00BA03F0, equipment_get_bomb_run_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_bomb_run* result = equipment_definition->equipment.bomb_run.begin();
	//return result;
}

const s_equipment_type_concussive_blast* equipment_get_concussive_blast_definition(int32 definition_index)
{
	return INVOKE(0x00BA0410, equipment_get_concussive_blast_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_concussive_blast* result = equipment_definition->equipment.concussive_blast.begin();
	//return result;
}

const s_equipment_type_forced_reload* equipment_get_forced_reload_definition(int32 definition_index)
{
	return INVOKE(0x00BA0430, equipment_get_forced_reload_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_forced_reload* result = equipment_definition->equipment.forced_reload.begin();
	//return result;
}

const s_equipment_type_health_pack* equipment_get_health_pack_definition(int32 definition_index)
{
	return INVOKE(0x00BA0450, equipment_get_health_pack_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_health_pack* result = equipment_definition->equipment.health_pack.begin();
	//return result;
}

const s_equipment_type_hologram* equipment_get_hologram_definition(int32 definition_index)
{
	return INVOKE(0x00BA0470, equipment_get_hologram_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_hologram* result = equipment_definition->equipment.hologram.begin();
	//return result;
}

const s_equipment_type_invincibility* __cdecl equipment_get_invincibility_mode_definition(int32 definition_index)
{
	return INVOKE(0x00BA0490, equipment_get_invincibility_mode_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_invincibility* result = equipment_definition->equipment.invincibility_mode.begin();
	//return result;
}

const s_equipment_type_invisibility_mode* __cdecl equipment_get_invisibility_mode_definition(int32 definition_index)
{
	return INVOKE(0x00BA04B0, equipment_get_invisibility_mode_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_invisibility_mode* result = equipment_definition->equipment.invisibility_mode.begin();
	//return result;
}

const s_equipment_type_lightning_strike* __cdecl equipment_get_lightning_strike_definition(int32 definition_index)
{
	return INVOKE(0x00BA04D0, equipment_get_lightning_strike_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_lightning_strike* result = equipment_definition->equipment.lightning_strike.begin();
	//return result;
}

const s_equipment_type_mag_pulse* __cdecl equipment_get_mag_pulse_definition(int32 definition_index)
{
	return INVOKE(0x00BA04F0, equipment_get_mag_pulse_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_mag_pulse* result = equipment_definition->equipment.mag_pulse.begin();
	//return result;
}

const s_equipment_type_motion_tracker_noise* __cdecl equipment_get_motion_tracker_noise_definition(int32 definition_index)
{
	return INVOKE(0x00BA0510, equipment_get_motion_tracker_noise_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_motion_tracker_noise* result = equipment_definition->equipment.motion_tracker_noise.begin();
	//return result;
}

const s_equipment_type_multiplayer_powerup* __cdecl equipment_get_multiplayer_powerup_definition(int32 definition_index)
{
	return INVOKE(0x00BA0530, equipment_get_multiplayer_powerup_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_multiplayer_powerup* result = equipment_definition->equipment.multiplayer_powerup.begin();
	//return result;
}

const s_equipment_type_proximity_mine* __cdecl equipment_get_proximity_mine_definition(int32 definition_index)
{
	return INVOKE(0x00BA0550, equipment_get_proximity_mine_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_proximity_mine* result = equipment_definition->equipment.proximity_mine.begin();
	//return result;
}

const s_equipment_type_reactive_armor* __cdecl equipment_get_reactive_armor_definition(int32 definition_index)
{
	return INVOKE(0x00BA0570, equipment_get_reactive_armor_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_reactive_armor* result = equipment_definition->equipment.reactive_armor.begin();
	//return result;
}

const s_equipment_type_scrambler* __cdecl equipment_get_scrambler_definition(int32 definition_index)
{
	return INVOKE(0x00BA0590, equipment_get_scrambler_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_scrambler* result = equipment_definition->equipment.scrambler.begin();
	//return result;
}

const s_equipment_type_showme* __cdecl equipment_get_showme_definition(int32 definition_index)
{
	return INVOKE(0x00BA05B0, equipment_get_showme_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_showme* result = equipment_definition->equipment.showme.begin();
	//return result;
}

const s_equipment_type_spawner* __cdecl equipment_get_spawner_definition(int32 definition_index)
{
	return INVOKE(0x00BA05D0, equipment_get_spawner_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_spawner* result = equipment_definition->equipment.spawner.begin();
	//return result;
}

const s_equipment_type_super_shield* __cdecl equipment_get_super_shield_definition(int32 definition_index)
{
	return INVOKE(0x00BA05F0, equipment_get_super_shield_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_super_shield* result = equipment_definition->equipment.super_shield.begin();
	//return result;
}

const s_equipment_type_tank_mode* __cdecl equipment_get_tank_mode_definition(int32 definition_index)
{
	return INVOKE(0x00BA0610, equipment_get_tank_mode_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_tank_mode* result = equipment_definition->equipment.tank_mode.begin();
	//return result;
}

const s_equipment_type_treeoflife* __cdecl equipment_get_treeoflife_definition(int32 definition_index)
{
	return INVOKE(0x00BA0630, equipment_get_treeoflife_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_treeoflife* result = equipment_definition->equipment.tree_of_life.begin();
	//return result;
}

const s_equipment_type_vision* __cdecl equipment_get_vision_definition(int32 definition_index)
{
	return INVOKE(0x00BA0650, equipment_get_vision_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_vision* result = equipment_definition->equipment.vision.begin();
	//return result;
}

const s_equipment_type_weapon_jammer* __cdecl equipment_get_weapon_jammer_definition(int32 definition_index)
{
	return INVOKE(0x00BA0650, equipment_get_weapon_jammer_definition, definition_index);

	//const s_equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const s_equipment_definition, definition_index);
	//const s_equipment_type_weapon_jammer* result = equipment_definition->equipment.weapon_jammer.begin();
	//return result;
}

//.text:00BA0690 ; 
//.text:00BA06A0 ; 
//.text:00BA06B0 ; 
//.text:00BA06C0 ; 
//.text:00BA06D0 ; 

void _equipment_definition::update_reference_names()
{
	UPDATE_REFERENCE_NAME(chud_interface);
	UPDATE_REFERENCE_NAME(pickup_sound);
	UPDATE_REFERENCE_NAME(empty_sound);
	UPDATE_REFERENCE_NAME(activate_sound);
	UPDATE_REFERENCE_NAME(activate_effect);
	UPDATE_REFERENCE_NAME(deactivate_sound);

	UPDATE_BLOCK_REFERENCE_NAMES(super_shield);
	UPDATE_BLOCK_REFERENCE_NAMES(spawner);
	UPDATE_BLOCK_REFERENCE_NAMES(proximity_mine);
	UPDATE_BLOCK_REFERENCE_NAMES(invincibility_mode);
	UPDATE_BLOCK_REFERENCE_NAMES(tree_of_life);
	UPDATE_BLOCK_REFERENCE_NAMES(forced_reload);
	UPDATE_BLOCK_REFERENCE_NAMES(concussive_blast);
	UPDATE_BLOCK_REFERENCE_NAMES(tank_mode);
	UPDATE_BLOCK_REFERENCE_NAMES(hologram);
	UPDATE_BLOCK_REFERENCE_NAMES(reactive_armor);
	UPDATE_BLOCK_REFERENCE_NAMES(bomb_run);
	UPDATE_BLOCK_REFERENCE_NAMES(armor_lock);
	UPDATE_BLOCK_REFERENCE_NAMES(adrenaline);
	UPDATE_BLOCK_REFERENCE_NAMES(lightning_strike);
	UPDATE_BLOCK_REFERENCE_NAMES(scrambler);
	UPDATE_BLOCK_REFERENCE_NAMES(weapon_jammer);
	UPDATE_BLOCK_REFERENCE_NAMES(ammo_pack);
	UPDATE_BLOCK_REFERENCE_NAMES(vision);
}

void s_equipment_type_super_shield::update_reference_names()
{
	UPDATE_REFERENCE_NAME(shield_effect);
	UPDATE_REFERENCE_NAME(overcharge_effect);
	UPDATE_REFERENCE_NAME(overcharge_damage_effect);
}

void s_equipment_type_spawner::update_reference_names()
{
	UPDATE_REFERENCE_NAME(spawned_object_definition);
	UPDATE_REFERENCE_NAME(spawned_effect_definition);
}

void s_equipment_type_proximity_mine::update_reference_names()
{
	UPDATE_REFERENCE_NAME(explosion_effect);
	UPDATE_REFERENCE_NAME(explosion_damage_effect);
}

void s_equipment_type_invincibility::update_reference_names()
{
	UPDATE_REFERENCE_NAME(activation_effect);
	UPDATE_REFERENCE_NAME(ongoing_effect);
}

void s_equipment_type_treeoflife::update_reference_names()
{
	UPDATE_REFERENCE_NAME(regenerating_effect);
}

void s_equipment_type_forced_reload::update_reference_names()
{
	UPDATE_REFERENCE_NAME(effect);
}

void s_equipment_type_concussive_blast::update_reference_names()
{
	UPDATE_REFERENCE_NAME(secondary_activate_effect);
	UPDATE_REFERENCE_NAME(secondary_damage_effect);
}

void s_equipment_type_tank_mode::update_reference_names()
{
	UPDATE_REFERENCE_NAME(active_hud);
}

void s_equipment_type_hologram::update_reference_names()
{
	UPDATE_REFERENCE_NAME(creation_effect);
	UPDATE_REFERENCE_NAME(attached_effect);
	UPDATE_REFERENCE_NAME(destruction_effect);
	UPDATE_REFERENCE_NAME(nav_point_hud);
}

void s_equipment_type_reactive_armor::update_reference_names()
{
	UPDATE_REFERENCE_NAME(activation_effect);
	UPDATE_REFERENCE_NAME(__unknown1C);
	UPDATE_REFERENCE_NAME(melee_impact_effect);
	UPDATE_REFERENCE_NAME(__unknown3C);
}

void s_equipment_type_bomb_run::update_reference_names()
{
	UPDATE_REFERENCE_NAME(projectile);
	UPDATE_REFERENCE_NAME(throw_sound);
}

void s_equipment_type_armor_lock::update_reference_names()
{
	UPDATE_REFERENCE_NAME(collision_damage);
	UPDATE_REFERENCE_NAME(friendly_collision_damage);
}

void s_equipment_type_adrenaline::update_reference_names()
{
	UPDATE_REFERENCE_NAME(activation_effect);
	UPDATE_REFERENCE_NAME(active_effect);
}

void s_equipment_type_lightning_strike::update_reference_names()
{
	UPDATE_REFERENCE_NAME(unknown_effect);
}

void s_equipment_type_scrambler::update_reference_names()
{
	UPDATE_REFERENCE_NAME(__unknown4);
}

void s_equipment_type_weapon_jammer::update_reference_names()
{
	UPDATE_REFERENCE_NAME(__unknown4);
}

void s_equipment_type_ammo_pack_weapon::update_reference_names()
{
	UPDATE_REFERENCE_NAME(weapon);
}

void s_equipment_type_ammo_pack::update_reference_names()
{
	UPDATE_REFERENCE_NAME(ammo_delivery_sound);
	UPDATE_BLOCK_REFERENCE_NAMES(weapons);
}

void s_equipment_type_vision::update_reference_names()
{
	UPDATE_REFERENCE_NAME(screen_effect);
	UPDATE_REFERENCE_NAME(damage_response);
}

void equipment_definition::update_reference_names()
{
	UPDATE_STRUCT_REFERENCE_NAMES(equipment);
}

