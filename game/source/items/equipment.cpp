#include "items/equipment.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game_time.hpp"
#include "items/equipment_definitions.hpp"
#include "memory/module.hpp"
#include "motor/actions.hpp"
#include "simulation/game_interface/simulation_game_action.hpp"

HOOK_DECLARE(0x00B86C20, equipment_activate);

//.text:00B86440 ; void __cdecl sub_B86440(equipment_datum*, int32, void*(__cdecl*)(void*), void*(__cdecl*)(void*))
//.text:00B865A0 ; void __cdecl sub_B865A0(equipment_datum*, int32, void*(__cdecl*)(void*), void*(__cdecl*)(void*))
//.text:00B86700 ; void __cdecl sub_B86700(equipment_datum*, int32)
//.text:00B86BC0 ; void __cdecl sub_B86BC0(int32)

bool __cdecl equipment_activate(int32 equipment_index, int32 owner_unit_index, bool network_predicted)
{
	//INVOKE(0x00B86C20, equipment_activate, equipment_index, owner_unit_index, network_predicted);

	bool result = false;
	
	equipment_datum* equipment = EQUIPMENT_GET(equipment_index);
	const struct equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const struct equipment_definition, equipment->definition_index);
	
	bool immediate = true;
	if (equipment_active_fraction(equipment_index) == 0.0f)
	{
		const bool has_sufficient_charges = equipment_definition->equipment.charges == NONE || equipment_definition->equipment.charges != equipment->equipment.charges_used;
		if (has_sufficient_charges)
		{
			long type_index = 0;
			for (e_equipment_type equipment_type = equipment_definition_get_type(equipment->definition_index, 0);
				equipment_type != _equipment_type_none;
				equipment_type = equipment_definition_get_type(equipment->definition_index, type_index))
			{
				type_index++;
				switch (equipment_type)
				{
				case _equipment_type_super_shield:
				{
					if (owner_unit_index != NONE)
					{
						unit_datum* unit = UNIT_GET(owner_unit_index);
						const s_equipment_type_super_shield* super_shield = equipment_get_super_shield_definition(equipment->definition_index);
						unit->object.shield_vitality = super_shield->shield_ceiling_multiplier;
					}
				}
				break;
				case _equipment_type_multiplayer_powerup:
				{
					event(_event_verbose, "mp powerup activated");
				}
				break;
				case _equipment_type_spawner:
				{
					const s_equipment_type_spawner* spawner = equipment_get_spawner_definition(equipment->definition_index);
					if (owner_unit_index != NONE && spawner->spawned_object_definition.index != NONE)
					{
						action_request request{};
						request.type = _action_equipment;
						request.equipment.spawner_object_definition_index = equipment->definition_index;
						request.equipment.network_aiming_origin = *global_origin3d;
						request.equipment.network_aiming_vector = *global_zero_vector3d;
						result = action_submit(owner_unit_index, &request);
						immediate = !result;
					}
				}
				break;
				case _equipment_type_proximity_mine:
				//case _equipment_type_showme: // is this not just here to crash the game?
				case _equipment_type_motion_tracker_noise:
				case _equipment_type_invincibility:
				case _equipment_type_treeoflife:
				{
					result = true;
				}
				break;
				case _equipment_type_invisibility_mode:
				{
					const s_equipment_type_invisibility_mode* invisibility_mode = equipment_get_invisibility_mode_definition(equipment->definition_index);
					if (owner_unit_index != NONE)
					{
						const unit_datum* owner_unit = UNIT_GET(owner_unit_index);
						if (owner_unit->unit.player_index != NONE)
						{
							real32 transition_time = invisibility_mode->transition_time <= 0.0f ? 4.0f : invisibility_mode->transition_time;
							//player_activate_invisibility(owner_unit->unit.player_index, invisibility_mode->invisible_time, transition_time);
							unit_active_camouflage_enable(owner_unit->unit.player_index, transition_time, game_time_get() + game_seconds_to_ticks_round(invisibility_mode->invisible_time));
							result = true;
						}
					}
				}
				break;
				default:
					UNREACHABLE();
					break;
				}
	
				if (result)
				{
					equipment->equipment.last_use_time = game_time_get();
					simulation_action_object_update(equipment_index, _simulation_item_equipment_activated_bit);
					if (immediate)
					{
						if (equipment_definition->equipment.activate_sound.index)
						{
							//unspatialized_impulse_sound_new(equipment_definition->equipment.activate_sound.index, 1.0f);
						}
						if (equipment_definition->equipment.activate_effect.index != NONE && owner_unit_index != NONE)
						{
							s_damage_reporting_info damage_reporting_info{};
							effect_new_from_object(equipment_definition->equipment.activate_effect.index, NULL, damage_reporting_info, owner_unit_index, 1.0f, 1.0f, NULL, NULL, _effect_deterministic);
						}
						if (equipment_definition->equipment.charges)
						{
							equipment->equipment.charges_used++;
						}
					}
				}
			}
		}
		object_activate(equipment_index);
	}
	return result;
}

real32 __cdecl equipment_active_fraction(int32 equipment_index)
{
	//return INVOKE(0x00B87DA0, equipment_active_fraction, equipment_index);
	return (real32)DECLFUNC(0x00B87DA0, real64, __cdecl, int32)(equipment_index);
}

//.text:00B87E60 ; 
//.text:00B87EA0 ; 
//.text:00B87EC0 ; bool __cdecl equipment_begin_activation_animation(int32)
//.text:00B87FD0 ; bool __cdecl equipment_begin_animation_state(int32, int32, int32, uns32)
//.text:00B88040 ; void __cdecl equipment_calculate_noise_maker_blip(int32, uns32*, int32, real_point2d*)
//.text:00B88160 ; int32 __cdecl equipment_calculate_noise_maker_blip_count(int32)
//.text:00B881A0 ; void __cdecl equipment_calculate_spawn_location(int32, real32, real32, real32, int16, real_point3d*, real_vector3d*)
//.text:00B882F0 ; bool __cdecl equipment_can_be_thrown(int32)
//.text:00B88360 ; bool __cdecl equipment_compute_function_value(int32, int32, int32, real32*, bool*, bool*)
//.text:00B88470 ; void __cdecl equipment_delete_recursive(int32, int32)

void __cdecl equipment_definition_handle_pickup(int32 player_index, int32 equipment_definition_index)
{
	INVOKE(0x00B887B0, equipment_definition_handle_pickup, player_index, equipment_definition_index);
}

//.text:00B88860 ; bool __cdecl equipment_desires_3rd_person_camera(int32)
//.text:00B888E0 ; void __cdecl sub_B888E0(int32)
//.text:00B889F0 ; real64 __cdecl sub_B889F0(int32)
//.text:00B88B10 ; real64 __cdecl equipment_get_invincible_fraction(int32)
//.text:00B88B70 ; bool __cdecl sub_B88B70(int32, real32*)
//.text:00B88C00 ; int32 __cdecl sub_B88C00(int32, int32)
//.text:00B88C40 ; void __cdecl equipment_handle_pickup(int32, int32)
//.text:00B88C80 ; equipment_has_remaining_use_duration?
//.text:00B88CF0 ; bool __cdecl equipment_is_active_noise_maker(int32)
//.text:00B88D90 ; 
//.text:00B88DA0 ; bool __cdecl equipment_new(int32, object_placement_data*, bool*)
//.text:00B88F70 ; bool __cdecl sub_B88F70(int32, int32*)
//.text:00B89010 ; bool __cdecl equipment_override_damage_material_type(int32, c_global_material_type*)
//.text:00B89090 ; bool __cdecl sub_B89090(int32, int32*)
//.text:00B89130 ; void __cdecl equipment_place(int32, s_scenario_equipment*)
//.text:00B89190 ; int32 __cdecl equipment_remaining_charges(int32)
//.text:00B891F0 ; void __cdecl sub_B891F0(int32, int32)
//.text:00B89550 ; bool __cdecl equipment_update(int32)
//.text:00B89E90 ; 
//.text:00B89EA0 ; 
//.text:00B89EB0 ; s_unknown_unit_struct_sizeof_14* __cdecl sub_B89EC0(void*)
//.text:00B89EC0 ; s_unknown_unit_struct_sizeof_14* __cdecl sub_B89EC0(void*)
//.text:00B89ED0 ; void __cdecl sub_B89ED0(int32)
//.text:00B89F30 ; 
//.text:00B89F60 ; 
//.text:00B89F80 ; void __cdecl sub_B89F80(int32)
//.text:00B8A040 ; bool __cdecl sub_B8A040(int32, int32)

