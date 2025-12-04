#include "items/equipment.hpp"

#include "ai/ai.hpp"
#include "animations/animation_manager.hpp"
#include "cache/cache_files.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "game/game_time.hpp"
#include "game/multiplayer_definitions.hpp"
#include "items/equipment_definitions.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "motor/actions.hpp"
#include "physics/havok_component.hpp"
#include "simulation/game_interface/simulation_game_action.hpp"
#include "sound/game_sound.hpp"
#include "sound/sound_manager.hpp"

HOOK_DECLARE(0x00B86C20, equipment_activate);
HOOK_DECLARE(0x00B87DA0, equipment_active_fraction);
HOOK_DECLARE(0x00B882F0, equipment_can_be_thrown);
HOOK_DECLARE(0x00B88DA0, equipment_new);

void __cdecl equipment_update0(int32 equipment_index, int32 owner_unit_index)
{
	equipment_update(equipment_index, owner_unit_index);
}
HOOK_DECLARE(0x00B891F0, equipment_update0);

void __cdecl equipment_update1(int32 equipment_index)
{
	equipment_update(equipment_index);
}
HOOK_DECLARE(0x00B89550, equipment_update1);

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
			int32 type_index = 0;
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
							unspatialized_impulse_sound_new(equipment_definition->equipment.activate_sound.index, 1.0f);
						}
						if (equipment_definition->equipment.activate_effect.index != NONE && owner_unit_index != NONE)
						{
							s_damage_reporting_info damage_reporting_info{};
							effect_new_from_object(
								equipment_definition->equipment.activate_effect.index,
								NULL,
								damage_reporting_info,
								owner_unit_index,
								1.0f,
								1.0f,
								NULL,
								NULL,
								_effect_deterministic);
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
	//return (real32)DECLFUNC(0x00B87DA0, real64, __cdecl, int32)(equipment_index);

	equipment_datum const* equipment = EQUIPMENT_GET(equipment_index);
	const struct equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const struct equipment_definition, equipment->definition_index);

	real32 fraction = 0.0f;
	if (equipment->equipment.last_use_time != NONE && equipment_definition->equipment.duration > 0.001f)
	{
		int32 duration_ticks = game_seconds_to_ticks_round(equipment_definition->equipment.duration);
		if (duration_ticks < 1)
		{
			duration_ticks = 1;
		}
		fraction = 1.0f - ((real32)(game_time_get() - equipment->equipment.last_use_time) / (real32)duration_ticks);
		fraction = real_pin(fraction, 0.0f, 1.0f);
	}
	return fraction;
}

string_id __cdecl equipment_animation_get_desired_idle(int32 equipment_index)
{
	//return INVOKE(0x00B87E60, equipment_animation_get_desired_idle, equipment_index);

	real32 active_fraction = equipment_active_fraction(equipment_index);
	return active_fraction == 0.0f ? STRING_ID(global, idle) : STRING_ID(global, emitting);
}

bool __cdecl equipment_animation_is_interruptable(int32 equipment_index, int32 animation_state, int32 desired_state)
{
	//return INVOKE(0x00B87EA0, equipment_animation_is_interruptable, equipment_index, animation_state, desired_state);

	bool interruptable = animation_state == STRING_ID(global, idle) || animation_state == STRING_ID(global, emitting);
	return interruptable;
}

//.text:00B87EC0 ; bool __cdecl equipment_begin_activation_animation(int32)

bool __cdecl equipment_begin_animation_state(int32 equipment_index, string_id state_name, int32 goal_flags, uns32 playback_flags)
{
	return INVOKE(0x00B87FD0, equipment_begin_animation_state, equipment_index, state_name, goal_flags, playback_flags);
}

//.text:00B88040 ; void __cdecl equipment_calculate_noise_maker_blip(int32, uns32*, int32, real_point2d*)
//.text:00B88160 ; int32 __cdecl equipment_calculate_noise_maker_blip_count(int32)
//.text:00B881A0 ; void __cdecl equipment_calculate_spawn_location(int32, real32, real32, real32, int16, real_point3d*, real_vector3d*)

bool __cdecl equipment_can_be_thrown(int32 equipment_index)
{
	//return INVOKE(0x00B882F0, equipment_can_be_thrown, equipment_index);
	
	bool can_be_thrown = false;

	equipment_datum* equipment = EQUIPMENT_GET(equipment_index);

	int32 type_index = 0;
	for (e_equipment_type equipment_type = equipment_definition_get_type(equipment->definition_index, 0);
		equipment_type != _equipment_type_none;
		equipment_type = equipment_definition_get_type(equipment->definition_index, type_index))
	{
		type_index++;

		if (equipment_type == _equipment_type_spawner)
		{
			can_be_thrown = true;
			break;
		}
	}

	return can_be_thrown;
}

//.text:00B88360 ; bool __cdecl equipment_compute_function_value(int32, int32, int32, real32*, bool*, bool*)
//.text:00B88470 ; void __cdecl equipment_delete_recursive(int32, int32)

void __cdecl equipment_definition_handle_pickup(int32 player_index, int32 equipment_definition_index)
{
	INVOKE(0x00B887B0, equipment_definition_handle_pickup, player_index, equipment_definition_index);
	
	//const player_datum* player = DATUM_GET(player_data, const player_datum, player_index);
	//if (player->unit_index != NONE)
	//{
	//	const struct equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const struct equipment_definition, equipment_definition_index);
	//	if (equipment_definition->equipment.pickup_sound.index != NONE)
	//	{
	//		s_sound_location sound_location{};
	//		unit_get_body_position(player->unit_index, &sound_location.position);
	//		sound_location_set_forward(&sound_location, global_up3d);
	//		sound_location.translational_velocity = *global_zero_vector3d;
	//		object_get_location(player->unit_index, &sound_location.game_location);
	//		object_unattached_impulse_sound_new(player->unit_index, equipment_definition->equipment.pickup_sound.index, &sound_location, 1.0f);
	//	}
	//}
}

//.text:00B88860 ; bool __cdecl equipment_desires_3rd_person_camera(int32)
//.text:00B888E0 ; void __cdecl sub_B888E0(int32)
//.text:00B889F0 ; real32 __cdecl sub_B889F0(int32)
//.text:00B88B10 ; real32 __cdecl equipment_get_invincible_fraction(int32)
//.text:00B88B70 ; bool __cdecl sub_B88B70(int32, real32*)
//.text:00B88C00 ; int32 __cdecl sub_B88C00(int32, int32)
//.text:00B88C40 ; void __cdecl equipment_handle_pickup(int32, int32)
//.text:00B88C80 ; equipment_has_remaining_use_duration?
//.text:00B88CF0 ; bool __cdecl equipment_is_active_noise_maker(int32)
//.text:00B88D90 ; bool __cdecl equipment_is_object_in_showme_range(int32)

bool __cdecl equipment_new(int32 equipment_index, object_placement_data* data, bool* out_of_memory)
{
	//return INVOKE(0x00B88DA0, equipment_new, equipment_index, data, out_of_memory);

	equipment_datum* equipment = EQUIPMENT_GET(equipment_index);
	const struct equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const struct equipment_definition, equipment->definition_index);
	const int32 now = game_time_get();

	equipment->equipment.game_time_at_creation = now;
	equipment->equipment.last_use_time = equipment_definition->equipment.charges == 0 ? now : NONE;
	equipment->equipment.creator_damage_owner = *global_damage_owner_unknown;
	equipment->equipment.looping_effect_index = NONE;

	// Halo Online
	{
		equipment->equipment.__unknown10 = NONE;
		equipment->equipment.__unknown24 = NONE;
		equipment->equipment.__unknown28 = NONE;
		for (int32 index = 0; index < NUMBEROF(equipment->equipment.__unknown2C); index++)
		{
			equipment->equipment.__unknown2C[index] = NONE;
		}
	}

	{
		int32 desired_state = equipment_animation_get_desired_idle(equipment_index);
		constexpr int32 goal_flags = FLAG(1) | FLAG(7); // FLAG(_allow_default_state) | FLAG(_ignore_weapon);
		equipment_begin_animation_state(equipment_index, desired_state, goal_flags, k_animation_looping_playback_default_flags);
	}

	return true;
}

//.text:00B88F70 ; bool __cdecl sub_B88F70(int32, int32*)
//.text:00B89010 ; bool __cdecl equipment_override_damage_material_type(int32, c_global_material_type*)
//.text:00B89090 ; bool __cdecl sub_B89090(int32, int32*)
//.text:00B89130 ; void __cdecl equipment_place(int32, s_scenario_equipment*)

int32 __cdecl equipment_remaining_charges(int32 equipment_index)
{
	//return INVOKE(0x00B89190, equipment_remaining_charges, equipment_index);

	const equipment_datum* equipment = EQUIPMENT_GET(equipment_index);
	const struct equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const struct equipment_definition, equipment->definition_index);

	return equipment_definition->equipment.charges != NONE
		? equipment_definition->equipment.charges - equipment->equipment.charges_used
		: NONE;
}

int32 __cdecl equipment_get_age(int32 equipment_index)
{
	equipment_datum* equipment = EQUIPMENT_GET(equipment_index);
	int32 age = NONE;
	if (equipment->equipment.last_use_time != NONE)
	{
		age = game_time_get() - equipment->equipment.last_use_time;
	}
	return age;
}

void __cdecl equipment_update(int32 equipment_index, int32 owner_unit_index)
{
	//INVOKE(0x00B891F0, equipment_update, equipment_index, owner_unit_index);

	equipment_datum* equipment = EQUIPMENT_GET(equipment_index);
	if (!equipment->object.damage_flags.test(_object_dead_bit))
	{
		int32 type_index = 0;
		for (e_equipment_type equipment_type = equipment_definition_get_type(equipment->definition_index, 0);
			equipment_type != _equipment_type_none;
			equipment_type = equipment_definition_get_type(equipment->definition_index, type_index))
		{
			type_index++;

			switch (equipment_type)
			{
			case _equipment_type_super_shield:
			case _equipment_type_multiplayer_powerup:
			case _equipment_type_spawner:
			case _equipment_type_proximity_mine:
			case _equipment_type_motion_tracker_noise:
			case _equipment_type_showme:
			case _equipment_type_invisibility_mode:
			case _equipment_type_treeoflife:
				continue;
			case _equipment_type_invincibility:
			{
				const struct equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const struct equipment_definition, equipment->definition_index);
				const s_equipment_type_invincibility* invincibility = equipment_get_invincibility_mode_definition(equipment->definition_index);
				if (equipment->equipment.last_use_time != NONE)
				{
					int32 age = equipment_get_age(equipment_index);
					int32 recharge_time = game_seconds_to_ticks_round(invincibility->shield_recharge_time) - 1;
					int32 duration_ticks = game_seconds_to_ticks_round(equipment_definition->equipment.duration);
					if (!age)
					{
						if (invincibility->activation_effect.index != NONE)
						{
							s_damage_reporting_info damage_reporting_info{};
							effect_new_from_object(invincibility->activation_effect.index, NULL, damage_reporting_info, owner_unit_index, 0.0f, 0.0f, NULL, NULL, _effect_deterministic);
						}
					}

					if (age + 1 == duration_ticks)
					{
						//// $TODO add back the `ending_effect` tag reference within `s_equipment_type_invincibility`
						//if (invincibility->ending_effect.index != NONE)
						//{
						//	s_damage_reporting_info damage_reporting_info{};
						//	effect_new_from_object(invincibility->ending_effect.index, NULL, damage_reporting_info, owner_unit_index, 0.0, 0.0, NULL, NULL, _effect_deterministic);
						//}

						// sub_B888E0
						if (invincibility->ongoing_effect.index != NONE)
						{
							s_damage_reporting_info damage_reporting_info{};
							effect_new_from_object(
								invincibility->ongoing_effect.index,
								NULL,
								damage_reporting_info,
								owner_unit_index,
								0.0f,
								0.0f,
								NULL,
								NULL,
								_effect_deterministic);
						}
					}

					if (invincibility->ongoing_effect.index != NONE)
					{
						if (equipment->equipment.looping_effect_index == NONE)
						{
							s_damage_reporting_info damage_reporting_info{};
							equipment->equipment.looping_effect_index = effect_new_from_object(
								invincibility->ongoing_effect.index,
								NULL,
								damage_reporting_info,
								owner_unit_index,
								0.0f,
								0.0f,
								NULL,
								NULL,
								_effect_not_deterministic);
						}
						else
						{
							effect_ping(
								equipment->equipment.looping_effect_index,
								invincibility->ongoing_effect.index,
								0.0f,
								0.0f,
								false);
						}
					}

					if (IN_RANGE_INCLUSIVE(age, 0, recharge_time) && owner_unit_index != NONE)
					{
						unit_datum* owner_unit = UNIT_GET(owner_unit_index);
						if (recharge_time <= 0)
						{
							owner_unit->object.shield_vitality = 1.0f;
						}
						else
						{
							real32 fraction = (1.0f / recharge_time) + owner_unit->object.shield_vitality;
							if (fraction > 1.0f)
							{
								fraction = 1.0f;
							}
							owner_unit->object.shield_vitality = fraction;
						}
					}
				}
			}
			break;
			default:
				UNREACHABLE();
				break;
			}
		}
	}
}

bool __cdecl equipment_update(int32 equipment_index)
{
	//return DECLFUNC(0x00B89550, bool, __cdecl, int32)(equipment_index);
	//return INVOKE(0x00B89550, equipment_update, equipment_index);

	equipment_datum* equipment = EQUIPMENT_GET(equipment_index);
	const struct equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const struct equipment_definition, equipment->definition_index);

	bool updated = equipment_update_phantoms(equipment_index);
	if (!equipment->object.damage_flags.test(_object_dead_bit))
	{
		updated = equipment_active_fraction(equipment_index) > 0.0f || updated;
		updated = equipment_update_animation(equipment_index) || updated;

#if 0 // $TODO reach style equipment
		for (e_equipment_type equipment_type_index = _equipment_type_super_shield; equipment_type_index < k_equipment_type_count; equipment_type_index++)
		{
			if (equipment_definition_has_type(equipment->definition_index, equipment_type_index))
			{
				updated = g_equipment_types[equipment_type_index]->update(equipment_index) || updated;
			}
		}
#else
		int32 type_index = 0;
		for (e_equipment_type equipment_type = equipment_definition_get_type(equipment->definition_index, 0);
			equipment_type != _equipment_type_none;
			equipment_type = equipment_definition_get_type(equipment->definition_index, type_index))
		{
			type_index++;
		
			switch (equipment_type)
			{
			case _equipment_type_super_shield:
			case _equipment_type_multiplayer_powerup:
			case _equipment_type_spawner:
			case _equipment_type_showme:
			case _equipment_type_invisibility_mode:
			case _equipment_type_invincibility:
				continue;
			case _equipment_type_motion_tracker_noise:
			{
				const s_equipment_type_motion_tracker_noise* motion_tracker_noise = equipment_get_motion_tracker_noise_definition(equipment->definition_index);
				real_point3d equipment_origin{};
				object_get_center_of_mass(equipment_index, &equipment_origin);
				chud_add_noisemaker_zone(
					equipment_index,
					&equipment_origin,
					motion_tracker_noise->noise_radius,
					motion_tracker_noise->flash_radius,
					motion_tracker_noise->noise_count);
				ai_handle_noisemaker(equipment_index);
			}
			break;
			case _equipment_type_proximity_mine:
			{
				bool waiting_to_self_destruct = false;
				if (equipment->equipment.last_use_time != NONE && !game_is_predicted())
				{
					const s_equipment_type_proximity_mine* proximity_mine = equipment_get_proximity_mine_definition(equipment->definition_index);
					const real32 age = game_ticks_to_seconds((real32)(game_time_get() - equipment->equipment.last_use_time));
					ASSERT(age >= 0.0f);
		
					if (proximity_mine->self_destruct_time != 0.0f && age > proximity_mine->self_destruct_time)
					{
						waiting_to_self_destruct = true;
		
						object_set_at_rest(equipment_index, false);
						if (equipment->object.havok_component_index != NONE)
						{
							c_havok_component* havok_component = DATUM_GET(g_havok_component_data, c_havok_component, equipment->object.havok_component_index);
							havok_component->wake_all_bodies_in_phantoms();
						}
						if (equipment_definition->equipment.deactivate_sound.index != NONE)
						{
							object_impulse_sound_new(
								equipment_index,
								equipment_definition->equipment.deactivate_sound.index,
								NONE,
								global_origin3d,
								global_forward3d,
								1.0f);
						}
						s_damage_owner damage_owner = equipment->equipment.creator_damage_owner;
						s_damage_reporting_info damage_reporting_info = {.type = _damage_reporting_type_generic_explosion };
						object_deplete_body(equipment_index, &damage_owner, damage_reporting_info);
					}
					else if (age > proximity_mine->arm_time)
					{
						bool object_moving_in_proximity = false;
						if (equipment->object.havok_component_index != NONE)
						{
							c_havok_component* havok_component = DATUM_GET(g_havok_component_data, c_havok_component, equipment->object.havok_component_index);
							int32 phantom_body_count = havok_component->get_bodies_in_phantoms_count();
							for (int32 phantom_body_index = 0; phantom_body_index < phantom_body_count; phantom_body_index++)
							{
								int32 body_object_index;
								int32 body_physics_model_material_index;
								int32 body_rigid_body_index;
								havok_component->get_bodies_in_phantom_info(
									phantom_body_index,
									&body_object_index,
									&body_physics_model_material_index,
									&body_rigid_body_index);
								if (body_object_index != NONE && body_object_index != equipment_index)
								{
									c_havok_component* body_havok_component = DATUM_GET(g_havok_component_data, c_havok_component, OBJECT_GET(body_object_index)->object.havok_component_index);
		
									real_vector3d linear_velocity{};
									object_get_localized_velocities(body_havok_component->get_object_index(), &linear_velocity, NULL, NULL, NULL);
									if (magnitude_squared3d(&linear_velocity) >= proximity_mine->minimum_velocity_to_trigger * proximity_mine->minimum_velocity_to_trigger)
									{
										object_moving_in_proximity = true;
										break;
									}
								}
							}
						}
		
						if (object_moving_in_proximity)
						{
							equipment->equipment.proximity_triggered_counter = FLOOR(equipment->equipment.proximity_triggered_counter + 1, 254);
							if (game_ticks_to_seconds((real32)equipment->equipment.proximity_triggered_counter) > proximity_mine->trigger_time)
							{
								s_damage_reporting_info damage_reporting_info = { .type = _damage_reporting_type_prox_mine };
								if (proximity_mine->explosion_effect.index != NONE)
								{
									real_point3d center_of_mass{};
									object_get_center_of_mass(equipment_index, &center_of_mass);
									effect_new_from_point_vector(
										proximity_mine->explosion_effect.index,
										&center_of_mass,
										global_up3d,
										global_up3d,
										_match_all_markers,
										_effect_deterministic,
										NULL,
										&equipment->object.location);
								}
								if (proximity_mine->explosion_damage_effect.index != NONE)
								{
									s_damage_data damage_data{};
									damage_data_new(&damage_data, proximity_mine->explosion_damage_effect.index);
									damage_data.location = equipment->object.location;
									object_get_center_of_mass(equipment_index, &damage_data.origin);
									damage_data.epicenter = damage_data.origin;
									damage_data.direction = equipment->object.forward;
									damage_data.area_of_effect_forward = damage_data.direction;
									damage_data.damage_reporting_info = damage_reporting_info;
									damage_data.damage_owner = equipment->equipment.creator_damage_owner;
									area_of_effect_cause_damage(&damage_data, NONE, 5);
								}
								object_deplete_body(equipment_index, global_damage_owner_unknown, damage_reporting_info);
							}
						}
						else
						{
							equipment->equipment.proximity_triggered_counter = 0;
						}
					}
				}
				updated = waiting_to_self_destruct || updated;
			}
			break;
			case _equipment_type_treeoflife:
			{
				if (!game_is_predicted() && equipment->object.havok_component_index != NONE)
				{
					const c_havok_component* havok_component = DATUM_GET(g_havok_component_data, c_havok_component, equipment->object.havok_component_index);
					int32 phantom_body_count = havok_component->get_bodies_in_phantoms_count();
					for (int32 phantom_body_index = 0; phantom_body_index < phantom_body_count; phantom_body_index++)
					{
						int32 body_object_index;
						int32 body_physics_model_material_index;
						int32 body_rigid_body_index;
						havok_component->get_bodies_in_phantom_info(
							phantom_body_index,
							&body_object_index,
							&body_physics_model_material_index,
							&body_rigid_body_index);
						if (body_object_index != NONE && body_object_index != equipment_index)
						{
							unit_datum* unit = UNIT_GET(body_object_index);
							unit->object.shield_stun_ticks = 0;
							unit->object.body_stun_ticks = 0;
						}
					}
				}
			}
			break;
			default:
				UNREACHABLE();
				break;
			}
		}
#endif
	}

	return updated;
}

bool __cdecl equipment_update_animation(int32 equipment_index)
{
	equipment_datum* equipment = EQUIPMENT_GET(equipment_index);
	const struct equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const struct equipment_definition, equipment->definition_index);

	bool animating = false;
	if (object_has_animation_manager(equipment_index))
	{
		c_animation_manager* animation_manager = (c_animation_manager*)object_header_block_get(equipment_index, &equipment->object.animation);
		const c_animation_channel* animation_channel = animation_manager->get_state_channel();

		animating = animation_manager->update_state_animation(object_animation_callback, equipment_index, 0, NULL, NULL);

		int32 current_state = animation_manager->get_state_name();
		int32 desired_state = equipment_animation_get_desired_idle(equipment_index);

		if (!animating || animation_channel->playback_complete() ||
			current_state != desired_state && equipment_animation_is_interruptable(equipment_index, current_state, desired_state))
		{
			constexpr int32 goal_flags = FLAG(1) | FLAG(7); // FLAG(_allow_default_state) | FLAG(_ignore_weapon);
			bool success = equipment_begin_animation_state(equipment_index, desired_state, goal_flags, k_animation_looping_playback_default_flags);
			if (!success && animation_channel->valid())
			{
				animation_manager->set_state_position_to_last_frame();
			}
			animating = success;
		}

		if (animating)
		{
			object_set_requires_motion(equipment_index);
		}
	}
	return animating;
}

bool __cdecl equipment_update_phantoms(int32 equipment_index)
{
	equipment_datum* equipment = EQUIPMENT_GET(equipment_index);
	const struct equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const struct equipment_definition, equipment->definition_index);

	bool phantom_volumes_active = false;
	if (equipment->object.physics_flags.test(_object_has_havok_shape_phantoms_bit) && equipment->object.havok_component_index != NONE)
	{
		c_havok_component* havok_component = DATUM_GET(g_havok_component_data, c_havok_component, equipment->object.havok_component_index);
		int32 activation_time = game_seconds_to_ticks_round(equipment_definition->equipment.phantom_volume_activation_time);
		const int32 age = equipment_get_age(equipment_index);

		if (activation_time && equipment->equipment.last_use_time != NONE && age == activation_time)
		{
			object_set_at_rest(equipment_index, false);
		}

		if (equipment->equipment.last_use_time == NONE || age >= activation_time)
		{
			havok_component->update_phantoms(true);
			phantom_volumes_active = true;
		}
	}
	return phantom_volumes_active;
}

//.text:00B89E90 ; 
//.text:00B89EA0 ; 
//.text:00B89EB0 ; s_unknown_unit_struct_sizeof_14* __cdecl sub_B89EC0(void*)
//.text:00B89EC0 ; s_unknown_unit_struct_sizeof_14* __cdecl sub_B89EC0(void*)
//.text:00B89ED0 ; void __cdecl sub_B89ED0(int32)
//.text:00B89F30 ; 
//.text:00B89F60 ; 
//.text:00B89F80 ; void __cdecl sub_B89F80(int32)
//.text:00B8A040 ; bool __cdecl sub_B8A040(int32, int32)

// ====================================================================================
// |                                                                                  |
// |      Patches and Hooks that don't belong in this file, because I am LAZY!!       |
// |                                                                                  |
// ====================================================================================

// player_submit_actions
// skip over `cooldown_reset_unknown40` check
byte const player_consumables_cooldown_reset_skip_patch_bytes[]
{
	//.text:0053DC0A                 jnz     short loc_53DC6B

	0x90,
	0x90
};
DATA_PATCH_DECLARE(0x0053DC0A, player_consumables_cooldown_reset_skip, player_consumables_cooldown_reset_skip_patch_bytes);

int32 multiplayer_globals_get_equipment_block_index(int32 equipment_definition_index)
{
	int32 result = NONE;
	if (s_multiplayer_universal_globals_definition* universal_data = scenario_multiplayer_globals_try_and_get_universal_data())
	{
		for (int32 equipment_block_index = 1; equipment_block_index < universal_data->equipment.count; equipment_block_index++)
		{
			s_multiplayer_equipment* equipment = &universal_data->equipment[equipment_block_index];
			if (equipment->object.index == equipment_definition_index)
			{
				result = equipment_block_index;
				break;
			}
		}
	}
	return result;
}

s_s3d_player_weapon_configuration_loadout* __cdecl player_get_weapon_loadout_sub_5366A0(player_datum* player)
{
	//return INVOKE(0x005366A0, player_get_weapon_loadout_sub_5366A0, player);

	s_s3d_player_weapon_configuration_loadout* weapon_loadout = &player->configuration.host.weapon.loadouts[player->weapon_loadout_index];
	if (player->unit_index != NONE)
	{
		int32 slot_index = 0;

		int32 current_equipment_index = unit_get_current_equipment(player->unit_index, slot_index);
		if (current_equipment_index != NONE)
		{
			const equipment_datum* current_equipment = EQUIPMENT_GET(current_equipment_index);
			int32 equipment_block_index = multiplayer_globals_get_equipment_block_index(current_equipment->definition_index);
			if (equipment_block_index != NONE)
			{
				weapon_loadout->consumables[slot_index] = (int8)equipment_block_index;
			}
		}
	}
	return weapon_loadout;
}
// called in `c_chud_update_user_data` constructor
HOOK_DECLARE_CALL(0x00A858FE, player_get_weapon_loadout_sub_5366A0);

// =====================================================================================

