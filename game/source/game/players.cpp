#include "game/players.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game_engine_notifications.hpp"
#include "game/game_globals.hpp"
#include "game/multiplayer_definitions.hpp"
#include "input/input_abstraction.hpp"
#include "interface/interface_constants.hpp"
#include "items/equipment.hpp"
#include "items/equipment_definitions.hpp"
#include "items/weapons.hpp"
#include "math/color_math.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "motor/actions.hpp"
#include "scenario/scenario.hpp"
#include "scenario/scenario_pvs.hpp"
#include "simulation/game_interface/simulation_game_action.hpp"
#include "structures/structure_bsp_definitions.hpp"
#include "text/draw_string.hpp"
#include "units/vehicles.hpp"

HOOK_DECLARE(0x00536020, player_get_armor_loadout);
HOOK_DECLARE(0x00536680, player_get_weapon_loadout);
HOOK_DECLARE(0x005394A0, player_examine_nearby_item);
HOOK_DECLARE(0x00539B20, player_find_action_context);
HOOK_DECLARE(0x0053C8A0, player_set_facing);
HOOK_DECLARE(0x0053F220, player_suppress_action);
//HOOK_DECLARE(0x0053FB80, player_teleport_internal_postprocess);
//HOOK_DECLARE(0x0053FCC0, player_teleport_on_bsp_switch);
HOOK_DECLARE(0x00541310, players_compute_combined_pvs);

bool debug_player_network_aiming = false;
bool debug_objects_biped_melee_in_range = false;
bool debug_render_players = false;
string_id g_player_desired_mode_override = NONE;
int32 g_character_physics_override = _character_physics_default;

//.text:00434720 ; public: s_player_configuration::s_player_configuration()
s_player_configuration::s_player_configuration()
{
	client = {};
	host = {};
}

//.text:00434760 ; public: s_player_configuration_from_client::s_player_configuration_from_client()
s_player_configuration_from_client::s_player_configuration_from_client()
{
	csmemset(this, 0, sizeof(*this));
}

//.text:00434780 ; public: s_player_configuration_from_host::s_player_configuration_from_host()
s_player_configuration_from_host::s_player_configuration_from_host()
{
	csmemset(this, 0, sizeof(*this));
	team_index = _multiplayer_team_none;
	assigned_team_index = _multiplayer_team_none;
}

s_player_identifier::s_player_identifier()
{
	csmemset(identifier, 0, sizeof(identifier));
}

s_player_identifier::s_player_identifier(uns64 data) :
	s_player_identifier()
{
	csmemcpy(identifier, &data, sizeof(identifier));
}

s_player_identifier::s_player_identifier(uns32 _ipv4_address, uns16 _port, uns16 _flags) :
	s_player_identifier()
{
	REFERENCE_DECLARE(identifier + 0, uns32, ipv4_address);
	REFERENCE_DECLARE(identifier + 4, uns16, port);
	REFERENCE_DECLARE(identifier + 6, uns16, flags);
	ipv4_address = _ipv4_address;
	port = _port;
	flags = _flags;
}

s_player_identifier::s_player_identifier(const transport_address* address) :
	s_player_identifier()
{
	REFERENCE_DECLARE(identifier + 0, uns32, ipv4_address);
	REFERENCE_DECLARE(identifier + 4, uns16, port);
	REFERENCE_DECLARE(identifier + 6, uns16, flags);
	ipv4_address = address->ipv4_address;
	port = address->port;
	flags = address->address_length;
}

void c_player_in_game_iterator::begin()
{
	m_iterator.begin(player_data);
}

bool c_player_in_game_iterator::next()
{
	for (m_iterator.m_datum = (player_datum*)data_iterator_next(&m_iterator.iterator);
		m_iterator.m_datum && TEST_BIT(m_iterator.m_datum->flags, _player_left_game_bit);
		m_iterator.m_datum = (player_datum*)data_iterator_next(&m_iterator.iterator))
	{
	}

	return m_iterator.m_datum != NULL;
}

player_datum* c_player_in_game_iterator::get_datum()
{
	return m_iterator.m_datum;
}

int32 c_player_in_game_iterator::get_index() const
{
	return m_iterator.iterator.index;
}

int16 c_player_in_game_iterator::get_absolute_index() const
{
	return m_iterator.get_absolute_index();
}

void c_player_with_unit_iterator::begin()
{
	m_iterator.begin(player_data);
}

bool c_player_with_unit_iterator::next()
{
	for (m_iterator.m_datum = (player_datum*)data_iterator_next(&m_iterator.iterator);
		m_iterator.m_datum && m_iterator.m_datum->unit_index == NONE;
		m_iterator.m_datum = (player_datum*)data_iterator_next(&m_iterator.iterator))
	{
	}

	return m_iterator.m_datum != NULL;
}

player_datum* c_player_with_unit_iterator::get_datum()
{
	return m_iterator.m_datum;
}

int32 c_player_with_unit_iterator::get_index() const
{
	return m_iterator.iterator.index;
}

int16 c_player_with_unit_iterator::get_absolute_index() const
{
	return m_iterator.get_absolute_index();
}

void player_override_desired_mode(int32 desired_mode)
{
	g_player_desired_mode_override = NONE;

	switch (desired_mode)
	{
	case STRING_ID(global, berserk):
	case STRING_ID(global, flaming):
	case STRING_ID(global, sleep):
	case STRING_ID(global, patrol):
	case STRING_ID(global, combat):
	case STRING_ID(global, armored):
	case STRING_ID(global, protected):
	case STRING_ID(global, crouch):
	case STRING_ID(global, flee):
	case STRING_ID(global, stunned):
	case STRING_ID(global, climb):
		g_player_desired_mode_override = desired_mode;
		break;
	}
}

void players_debug_render()
{
	c_font_cache_mt_safe font_cache;
	char string[2048]{};

	if (debug_player_network_aiming)
	{
		//c_data_iterator<player_datum> player_iterator;
		//player_iterator.begin(*player_data);
		//while (player_iterator.next())
		//{
		//	player_datum* player = player_iterator.get_datum();
		//	if (player->unit_index != NONE)
		//	{
		//		// $IMPLEMENT
		//	}
		//}
	}

	if (debug_objects && debug_objects_biped_melee_in_range)
	{
		//c_data_iterator<player_datum> player_iterator;
		//player_iterator.begin(*player_data);
		//while (player_iterator.next())
		//{
		//	player_datum* player = player_iterator.get_datum();
		//	if (player->unit_index != NONE
		//		//&& player->machine_user_index
		//		&& TEST_MASK(_object_mask_biped, FLAG(object_get_type(player->unit_index))))
		//	{
		//		biped_datum* biped = BIPED_GET(player->unit_index);
		// 
		//		// $TODO find the bit index
		//		if (TEST_BIT(biped->biped.flags, ??))
		//		{
		//			real_point3d position{};
		//			real_vector3d aiming_vector{};
		//			biped_get_sight_position(player->unit_index, 0, false, NULL, NULL, NULL, NULL, &position);
		//			unit_get_aiming_vector(player->unit_index, &aiming_vector);
		//
		//			real_point3d point{};
		//			point_from_line3d(&point, &aiming_vector, 0.3f, &point);
		//			render_debug_sphere(true, &point, 0.15f, global_real_argb_red);
		//		}
		//	}
		//}
	}

	if (debug_render_players)
	{

	}

	if (g_player_desired_mode_override != NONE)
	{
		c_rasterizer_draw_string draw_string;

		rectangle2d bounds{};
		interface_get_current_display_settings(NULL, NULL, NULL, &bounds);
		bounds.y0 = bounds.y1 - 40;

		const char* string_const = string_id_get_string_const(g_player_desired_mode_override);
		csnzprintf(string, NUMBEROF(string), "Player Forced Into Mode: %s|n", string_const);

		draw_string.set_bounds(&bounds);
		draw_string.draw(&font_cache, string);
	}

	if (g_character_physics_override)
	{
		//c_rasterizer_draw_string draw_string;
		//
		//rectangle2d bounds{};
		//interface_get_current_display_settings(NULL, NULL, NULL, &bounds);
		//bounds.y0 = bounds.y1 - 20;
		//
		//const char* character_physics_override = g_character_physics_override < k_total_character_physics_overrides ? global_character_physics_override_names[g_character_physics_override] : "OUT OF RANGE!";
		//csnzprintf(string, NUMBEROF(string), "Character Physics Override: %s|n", character_physics_override);
		//
		//draw_string.set_bounds(&bounds);
		//draw_string.draw(&font_cache, string);
	}
}

//.text:00536AC0 ; 
//.text:00536B80 ; 
//.text:00536EA0 ; public: __cdecl player_datum::player_datum()
//.text:00536FD0 ; 
//.text:00537210 ; 
//.text:00537430 ; 
//.text:005375B0 ; int32 __cdecl find_best_starting_location_index(int32, bool, bool)
//.text:00537860 ; void __cdecl map_editor_process_player_control(int32, player_action*)
//.text:005379E0 ; void __cdecl player_action_clear(player_action*)

void __cdecl player_action_context_clear(s_player_action_context* action_context)
{
	INVOKE(0x00537A50, player_action_context_clear, action_context);
}

//.text:00537A80 ; bool __cdecl player_action_context_valid(const s_player_action_context*)
//.text:00537AB0 ; bool __cdecl player_action_valid(const player_action*)
//.text:00537C90 ; 
//.text:00537D10 ; 
//.text:00537DB0 ; void __cdecl player_active_camo_screen_effect(int32)
//.text:00537EA0 ; bool __cdecl player_active_camouflage_on()
//.text:00537F20 ; bool __cdecl sub_537F20(player_datum*, int32)
//.text:00537F90 ; void __cdecl player_appearance_initialize(s_player_appearance*)
//.text:00537FB0 ; bool __cdecl player_appearance_valid(const s_player_appearance*)
//.text:00537FE0 ; void __cdecl player_approve_pickup_weapon(int32, int32, unit_weapon_pickup_result*)
//.text:005381F0 ; void __cdecl player_build_aiming_vector_from_facing(int32, real32, real32, real_vector3d*)
//.text:005384C0 ; void __cdecl player_build_facing_vector_from_throttle(int32, const real_vector3d*, const real_vector2d*, real_vector3d*, real_vector3d*)
//.text:00538510 ; bool __cdecl player_can_assassinate_object(int32, int32)
//.text:005385F0 ; bool __cdecl player_can_fancy_assassinate_object(int32, int32)
//.text:005386D0 ; bool __cdecl sub_5386D0(player_datum*)
//.text:00538730 ; void __cdecl player_clear_assassination_state(int32)
//.text:005387A0 ; void __cdecl player_configuration_initialize(s_player_configuration*)

bool __cdecl player_consider_biped_interaction(int32 player_index, int32 biped_index, s_player_action_context* result)
{
	return INVOKE(0x005387F0, player_consider_biped_interaction, player_index, biped_index, result);
}

bool __cdecl player_consider_device_interaction(int32 player_index, int32 device_index, s_player_action_context* result)
{
	return INVOKE(0x005388D0, player_consider_device_interaction, player_index, device_index, result);
}

bool __cdecl player_consider_equipment_interaction(int32 player_index, int32 equipment_index, s_player_action_context* result)
{
	player_datum* player = DATUM_GET(player_data, player_datum, player_index);
	unit_datum* unit = UNIT_GET(player->unit_index);
	equipment_datum* equipment = EQUIPMENT_GET(equipment_index);

	if (equipment->item.inventory_state || equipment->item.ignore_object_index == player->unit_index)
		return false;

	if (!point_in_sphere(&equipment->object.bounding_sphere_center, &unit->object.bounding_sphere_center, unit->object.bounding_sphere_radius + 0.4f))
		return false;

	if (unit_get_current_equipment(player->unit_index, 0) == NONE || !unit_can_pickup_equipment(player->unit_index, equipment_index))
		return false;

	int32 active_primary_weapon = unit_get_active_primary_weapon(player->unit_index, NULL);
	if (active_primary_weapon != NONE && weapon_is_support_weapon(active_primary_weapon))
		return false;

	s_player_interaction old_interaction = result->interaction;
	s_player_interaction interaction{};
	interaction.type = 2;// _player_interaction_pick_up_equipment;
	interaction.object_index = equipment_index;

	bool interaction_considered = player_evaluate_interaction(player_index, &interaction, &result->interaction);
	if (interaction_considered && !unit_can_access_object(player->unit_index, equipment_index))
	{
		result->interaction = old_interaction;
		interaction_considered = false;
	}

	return interaction_considered;
}

bool __cdecl player_consider_unit_interaction(int32 player_index, int32 unit_index, s_player_action_context* result)
{
	return INVOKE(0x005389E0, player_consider_unit_interaction, player_index, unit_index, result);
}

bool __cdecl player_consider_vehicle_interaction(int32 player_index, int32 vehicle_index, s_player_action_context* result)
{
	return INVOKE(0x00538B10, player_consider_vehicle_interaction, player_index, vehicle_index, result);
}

bool __cdecl player_consider_weapon_interaction(int32 player_index, int32 weapon_index, s_player_action_context* result)
{
	return INVOKE(0x00538D00, player_consider_weapon_interaction, player_index, weapon_index, result);
}

void __cdecl player_copy_object_appearance(int32 player_index, int32 object_index)
{
	INVOKE(0x00538ED0, player_copy_object_appearance, player_index, object_index);
}

//.text:00538FD0 ; void __cdecl sub_538FD0(int32, int32)

void __cdecl player_delete(int32 player_index)
{
	INVOKE(0x00539050, player_delete, player_index);

	//player_set_unit_index(player_index, NONE);
	//player_mapping_set_input_user(player_index, NONE);
	//player_mapping_set_input_controller(player_index, k_no_controller);
	//player_mapping_detach_output_users(player_index);
	//players_rebuild_user_mapping(false);
	//datum_delete(player_data, player_index);
	//game_engine_player_deleted(player_index);
	//players_handle_deleted_player_internal(player_index);
}

//.text:005390B0 ; void __cdecl player_died(int32)
//.text:005391D0 ; void __cdecl player_died_update_for_multiplayer(int32)
//.text:00539210 ; 
//.text:00539220 ; 

bool __cdecl player_evaluate_interaction(int32 player_index, const s_player_interaction* interaction, s_player_interaction* current_interaction)
{
	return INVOKE(0x00539240, player_evaluate_interaction, player_index, interaction, current_interaction);
}

//.text:005392F0 ; real32 __cdecl player_evaluate_interaction_compute_weight(int32, int32)

void __cdecl player_examine_nearby_item(int32 player_index, int32 item_index)
{
	//INVOKE(0x005394A0, player_examine_nearby_item, player_index, item_index);

	const player_datum* player = DATUM_GET(player_data, const player_datum, player_index);
	if (player && player->unit_index != NONE && !object_is_multiplayer_cinematic_object(item_index))
	{
		const unit_datum* unit = UNIT_GET(player->unit_index);
		const item_datum* item = ITEM_GET(item_index);

		bool parent_object_can_have_attached_weapons = false;

		if (item->object.parent_object_index != NONE)
		{
			if (item->object.physics_flags.test(_object_is_early_mover_child_bit)
				|| TEST_BIT(_object_mask_sightblocking, object_get_type(item->object.parent_object_index)))
			{
				parent_object_can_have_attached_weapons = true;
			}
		}

		if (item->object.parent_object_index == NONE || parent_object_can_have_attached_weapons)
		{
			if (item->item.ignore_object_index != player->unit_index)
			{
				const real32 examine_nearby_object_bonus = 0.4f;
				if (point_in_sphere(&item->object.bounding_sphere_center, &unit->object.bounding_sphere_center, unit->object.bounding_sphere_radius + examine_nearby_object_bonus))
				{
					if (!game_is_multiplayer() || unit_can_access_object(player->unit_index, item_index))
					{
						int32 ammo_taker_weapon_index = NONE;

						for (int32 index = 0; index < NUMBEROF(unit->unit.weapon_object_indices); index++)
						{
							int32 primary_weapon_index = 0;
							if (unit->unit.current_weapon_set.weapon_indices[0] >= 0)
							{
								primary_weapon_index = unit->unit.current_weapon_set.weapon_indices[0];
							}

							int32 weapon_index = unit->unit.weapon_object_indices[(index + primary_weapon_index) % NUMBEROF(unit->unit.weapon_object_indices)];
							if (weapon_index != NONE)
							{
								int16 rounds_picked_up = 0;
								if (weapon_handle_potential_inventory_item(
									weapon_index,
									item_index,
									player->unit_index,
									player_index,
									&rounds_picked_up) &&
									rounds_picked_up > 0)
								{
									//data_mine_insert_equipment_event("ammo pickup", layer->unit_index, item->definition_index, rounds_picked_up);

									ammo_taker_weapon_index = weapon_index;

									c_player_output_user_iterator iterator{};
									iterator.begin_player(player_index);
									while (iterator.next())
									{
										const weapon_datum* weapon = WEAPON_GET(weapon_index);
										int32 user_index = iterator.get_user_index();
										chud_picked_up_ammunition(user_index, weapon->definition_index, rounds_picked_up);
									}
								}

								if (ammo_taker_weapon_index != NONE && ammo_taker_weapon_index != weapon_index)
								{
									if (OBJECT_GET(ammo_taker_weapon_index)->definition_index == OBJECT_GET(weapon_index)->definition_index)
									{
										object_wake(weapon_index);
									}
								}
							}
						}

						const equipment_datum* equipment = EQUIPMENT_GET(item_index);
						if (equipment)
						{
							// bool can_pickup_grenades; // where used?

							if (game_get_grenade_type_index_from_item_defintion(equipment->definition_index) == NONE)
							{
								if (game_is_multiplayer() && unit_can_pickup_equipment(player->unit_index, item_index))
								{
									e_equipment_type equipment_type = equipment_definition_get_type(equipment->definition_index, 0);
									int32 current_equipment_index = unit_get_current_equipment(player->unit_index, 0);

									if (equipment_type == _equipment_type_multiplayer_powerup)
									{
										if (game_is_predicted())
										{
											simulation_request_autopickup_powerup(player_index, item_index);
										}
										else
										{
											player_use_multiplayer_powerup(player_index, item_index);
										}
									}
									else if (current_equipment_index == NONE)
									{
										if (game_is_predicted())
										{
											simulation_request_autopickup_equipment(player_index, item_index);
										}
										else
										{
											player_pickup_equipment(player_index, item_index);
										}
									}
								}
							}
							else if (!game_is_multiplayer() || player->multiplayer.player_traits.get_weapons_traits()->get_weapon_pickup_allowed())
							{
								if (game_is_predicted())
								{
									simulation_request_autopickup_grenade(player_index, item_index);
								}
								else if (unit_add_grenade_to_inventory(player->unit_index, item_index))
								{
									//data_mine_insert_equipment_event("grenade pickup", player->unit_index, equipment->definition_index, 1);

									c_player_output_user_iterator iterator{};
									iterator.begin_player(player_index);
									while (iterator.next())
									{
										chud_picked_up_grenade(iterator.get_user_index(), equipment->definition_index);
									}
								}
							}
						}

						unit_weapon_pickup_result pickup_result{};
						if (TEST_BIT(_object_mask_weapon, object_get_type(item_index))
							&& unit_can_pickup_weapon(player->unit_index, item_index, _unit_add_weapon_unknown_method, &pickup_result)
							&& pickup_result.can_pick_up[0]
							&& player_should_auto_pickup_weapon(player_index, item_index))
						{
							if (game_is_predicted())
							{
								simulation_request_pickup_weapon(player->unit_index, item_index, _unit_add_weapon_as_primary_weapon);
							}
							else
							{
								action_request request{};
								request.type = _action_weapon_pickup;
								request.weapon_pickup.weapon_index = item_index;
								request.weapon_pickup.add_weapon_mode = _unit_add_weapon_as_primary_weapon;
								action_submit(player->unit_index, &request);
							}
						}

						game_engine_player_nearby_item(player_index, item_index);
					}
				}
			}
		}
	}
}

//.text:00539900 ; void __cdecl player_examine_nearby_objects(int32)
//.text:00539A30 ; bool __cdecl player_fancy_assassinate_object(int32, int32)

void __cdecl player_find_action_context(int32 player_index, s_player_action_context* out_action_context)
{
	//INVOKE(0x00539B20, player_find_action_context, player_index, out_action_context);

	const player_datum* player = DATUM_GET(player_data, const player_datum, player_index);

	player_action_context_clear(out_action_context);

	if (player->unit_index != NONE)
	{
		unit_datum* unit = UNIT_GET(player->unit_index);
		if (unit->object.parent_object_index == NONE || TEST_BIT(player->flags, _player_unknown_bit14))
		{
			const uns32 accept_type_mask = _object_mask_unit | _object_mask_item | _object_mask_scenery | _object_mask_crate | _object_mask_device;
			static_assert(0x2BBF == accept_type_mask);

			const real32 examine_nearby_object_bonus = 0.4f;
			real32 search_radius = (unit->object.bounding_sphere_radius + examine_nearby_object_bonus) + 0.1f;
			int32 object_indices[64]{};
			int32 object_count = objects_in_sphere(
				0,
				accept_type_mask,
				&unit->object.location,
				&unit->object.bounding_sphere_center,
				search_radius,
				object_indices,
				NUMBEROF(object_indices));

			const uns32 exclude_type_mask = _object_mask_weapon | _object_mask_machine | _object_mask_control;
			static_assert(0x304 == exclude_type_mask);
			for (int32 index = 0; index < object_count; index++)
			{
				const object_datum* object = object_get(object_indices[index]);
				e_object_type object_type = object->object.object_identifier.get_type();

				uns32 local_exclude_type_mask = _object_mask_projectile | _object_mask_sound_scenery | _object_mask_creature;
				static_assert(0x1440 == (_object_mask_projectile | _object_mask_sound_scenery | _object_mask_creature));

				if (!TEST_BIT(local_exclude_type_mask, object_type))
				{
					bool examine_children = TEST_BIT(exclude_type_mask, object_type);
					if (!examine_children || point_in_sphere(&object->object.bounding_sphere_center, &unit->object.bounding_sphere_center, search_radius + object->object.bounding_sphere_radius))
					{
						switch (object_type)
						{
						case _object_type_biped:
							player_consider_biped_interaction(player_index, object_indices[index], out_action_context);
							break;
						case _object_type_vehicle:
							player_consider_vehicle_interaction(player_index, object_indices[index], out_action_context);
							static_assert(0x1442 == (_object_mask_projectile | _object_mask_sound_scenery | _object_mask_creature | _object_mask_vehicle));
							local_exclude_type_mask |= _object_mask_vehicle;
							break;
						case _object_type_weapon:
							player_consider_weapon_interaction(player_index, object_indices[index], out_action_context);
							break;
						case _object_type_equipment:
							player_consider_equipment_interaction(player_index, object_indices[index], out_action_context);
							break;
						case _object_type_arg_device:
						case _object_type_terminal:
						case _object_type_control:
							player_consider_device_interaction(player_index, object_indices[index], out_action_context);
							break;
						default:
							examine_children = true;
							break;
						}

						if (examine_children)
						{
							object_datum* child_object = NULL;

							for (int32 child_object_index = object->object.first_child_object_index;
								child_object_index != NONE && object_count < NUMBEROF(object_indices);
								child_object_index = child_object->object.next_object_index)
							{
								e_object_type child_object_type = child_object->object.object_identifier.get_type();
								bool consider_child = TEST_BIT(local_exclude_type_mask, child_object_type);
								if (!consider_child &&
									(!examine_children || point_in_sphere(&child_object->object.bounding_sphere_center, &unit->object.bounding_sphere_center, search_radius + child_object->object.bounding_sphere_radius)))
								{
									child_object = object_get(child_object_index);
									object_indices[object_count++] = child_object_index;
								}
							}
						}

						if (TEST_MASK(_object_mask_unit, object_type))
						{
							player_consider_unit_interaction(player_index, object_indices[index], out_action_context);
						}
					}
				}
			}
		}
	}
}

//.text:00539E30 ; bool __cdecl player_find_best_spawn_location(int32, real_point3d*, real32*, real32*, bool, bool)
//.text:00539F70 ; void __cdecl player_find_player_character_unit_and_variant_info(int32, int32*, int32*)
//.text:0053A010 ; bool __cdecl player_find_zone_set_switches(int32, int32*)
//.text:0053A1E0 ; bool __cdecl player_flashlight_on()
//.text:0053A290 ; void __cdecl player_force_spawn(int32)
//.text:0053A330 ; void __cdecl player_forge_cleanup_on_death(int32)
//.text:0053A3F0 ; player_get_stamina_deplete_restore_time
//.text:0053A450 ; player_get_vehicle_shield_regen_speed_modifier_value
//.text:0053A5A0 ; player_get_vehicle_shield_regen_delay_modifier_value
//.text:0053A6F0 ; player_get_representation_block
//.text:0053A770 ; enum e_player_character_type __cdecl player_get_character_type(int32)
//.text:0053A7D0 ; player_get_scavenger_aura_modifier_value
//.text:0053A940 ; adrenaline related
//.text:0053A9A0 ; int32 __cdecl player_get_unit_index_even_if_dead(int32)
//.text:0053A9E0 ; bool __cdecl player_handle_interaction_held(int32, const s_player_interaction*)
//.text:0053AD10 ; bool __cdecl player_handle_interaction_press(int32, const s_player_interaction*)
//.text:0053ADE0 ; bool __cdecl player_handle_interaction_held(int32, const s_player_interaction*)
//.text:0053AED0 ; void __cdecl player_handle_weapon_added(int32, int16)
//.text:0053AEF0 ; void __cdecl player_handle_weapon_put_away(int32, int16)
//.text:0053AF10 ; revenge_shield_boost related
//.text:0053AFA0 ; void __cdecl player_health_pack_screen_effect(int32)
//.text:0053B060 ; bool __cdecl player_identifier_compare(const s_player_identifier*, const s_player_identifier*)
//.text:0053B090 ; const char* __cdecl player_identifier_get_string(const s_player_identifier*)
//.text:0053B0E0 ; bool __cdecl player_identifier_is_valid(const s_player_identifier*)
//.text:0053B120 ; void __cdecl player_increment_control_context(int32)

int32 __cdecl player_index_from_absolute_player_index(int32 player_index)
{
	return INVOKE(0x0053B1A0, player_index_from_absolute_player_index, player_index);
}

int32 __cdecl player_index_from_unit_index(int32 unit_index)
{
	return INVOKE(0x0053B1D0, player_index_from_unit_index, unit_index);
}

//.text:0053B200 ; public: __cdecl s_player_information::s_player_information()
//.text:0053B260 ; void __cdecl player_input_enable(bool)
//.text:0053B290 ; bool __cdecl player_input_enabled()
//.text:0053B2B0 ; void __cdecl player_input_mostly_inhibit(bool)
//.text:0053B2D0 ; void __cdecl player_interaction_clear(s_player_interaction*)

bool __cdecl player_interaction_exists(int32 player_index, uns32 object_mask, const s_player_interaction* interaction)
{
	return INVOKE(0x0053B2F0, player_interaction_exists, player_index, object_mask, interaction);
}

//.text:0053B340 ; bool __cdecl player_interaction_valid(const s_player_interaction*)
//.text:0053B370 ; bool __cdecl player_is_allowed_to_attempt_assassination(int32, int32)
//.text:0053B480 ; bool __cdecl player_is_immune_to_headshot(int32, bool)

bool __cdecl player_is_local(int32 player_index)
{
	return INVOKE(0x0053B4E0, player_is_local, player_index);
}

bool __cdecl player_is_reading_terminal()
{
	return INVOKE(0x0053B570, player_is_reading_terminal);
}

//.text:0053B590 ; bool __cdecl player_is_sprinting(int32, real32*)
//.text:0053B670 ; void __cdecl player_leave_game_internal(int32)
//.text:0053B7D0 ; void __cdecl player_left_game(int32)
//.text:0053B7E0 ; first_player_set_armor
//.text:0053B840 ; void __cdecl player_mostly_input_inhibit(int32, player_action*)

int32 __cdecl player_new(int32 player_array_index, const game_player_options* options, bool joined_in_progress)
{
	return INVOKE(0x0053B880, player_new, player_array_index, options, joined_in_progress);

	//int32 player_absolute_index = datum_new_at_absolute_index(*player_data, player_array_index);
	//if (player_absolute_index != NONE)
	//{
	//	player_reset(player_absolute_index, true, joined_in_progress, options);
	//	if (!options->player_left_game)
	//	{
	//		players_globals->players_in_game_count++;
	//
	//		simulation_player_joined_game(player_absolute_index);
	//		ai_player_state_create(player_absolute_index);
	//		game_engine_player_added(player_absolute_index);
	//	}
	//}
	//return player_absolute_index;
}

void __cdecl player_pickup_equipment(int32 player_index, int32 equipment_index)
{
	const player_datum* player = DATUM_GET(player_data, const player_datum, player_index);
	const equipment_datum* equipment = EQUIPMENT_GET(equipment_index);

	action_request request{};
	request.type = _action_equipment_pickup;
	request.equipment_pickup.equipment_index = equipment_index;
	action_submit(player->unit_index, &request);
	//data_mine_insert_equipment_event("powerup pickup", player->unit_index, equipment->definition_index, 1);
	equipment_definition_handle_pickup(player_index, equipment->definition_index);

	c_player_output_user_iterator iterator{};
	iterator.begin_player(player_index);
	while (iterator.next())
	{
		chud_messaging_picked_up_powerup(iterator.get_user_index(), equipment->definition_index);
	}
	simulation_action_pickup_powerup(player->unit_index, equipment->definition_index);
}

//.text:0053B8F0 ; void __cdecl player_notify_of_tracking_or_locking(int32, int32, int16)
//.text:0053BA10 ; void __cdecl player_notify_vehicle_ejection_finished(int32)
//.text:0053BA90 ; void __cdecl player_over_shield_screen_effect(int32)

void __cdecl player_positions_dispose()
{
	INVOKE(0x0053BB90, player_positions_dispose);
}

void __cdecl player_positions_dispose_from_old_map()
{
	INVOKE(0x0053BBA0, player_positions_dispose_from_old_map);
}

void __cdecl player_positions_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask)
{
	INVOKE(0x0053BBB0, player_positions_dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);
}

void __cdecl player_positions_initialize()
{
	INVOKE(0x0053BBC0, player_positions_initialize);
}

void __cdecl player_positions_initialize_for_new_map()
{
	INVOKE(0x0053BBD0, player_positions_initialize_for_new_map);
}

void __cdecl player_positions_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask)
{
	INVOKE(0x0053BBE0, player_positions_initialize_for_new_structure_bsp, activating_structure_bsp_mask);
}

//.text:0053BD90 ; void __cdecl player_prepare_action(int32, player_action*)

real_rgb_color __cdecl player_profile_get_rgb_color(int32 color_index)
{
	if (!global_scenario_try_and_get())
		return *global_real_rgb_white;

	s_game_globals* game_globals = scenario_get_game_globals();
	if (!VALID_INDEX(color_index, game_globals->profile_colors.count))
	{
		event(_event_message, "no player color defined in game globals tag for player color index #%ld",
			color_index);
		return *global_real_rgb_white;
	}

	// profile_colors is supposed to be `real_rgb_color`

	real_rgb_color real_color{};
	return *pixel32_to_real_rgb_color(game_globals->profile_colors[color_index], &real_color);
}

//.text:0053BFF0 ; void __cdecl player_reading_terminal_set(bool)
//.text:0053C020 ; void __cdecl player_rejoined_game(int32, const game_player_options*, bool)
//.text:0053C070 ; void __cdecl player_reset(int32, bool, bool, const game_player_options*)
//.text:0053C570 ; 
//.text:0053C630 ; void __cdecl player_set_configuration(int32,const s_player_configuration*)
//.text:0053C860 ; 

void __cdecl player_set_facing(int32 player_index, const real_vector3d* facing)
{
	//INVOKE(0x0053C8A0, player_set_facing, player_index, facing);

	const player_datum* player = DATUM_TRY_AND_GET(player_data, player_datum, player_index);
		if (game_is_authoritative())
	{
		if (player->unit_index != NONE)
		{
			unit_datum* unit = UNIT_GET(player->unit_index);
			unit->unit.desired_facing_vector = *facing;
			unit->unit.desired_aiming_vector = *facing;
			unit->unit.desired_looking_vector = *facing;

			simulation_action_object_update(player->unit_index, _simulation_unit_update_desired_aiming_vector_bit);
		}

	}

	int32 input_user_index = player_mapping_get_input_user(player_index);
	if (input_user_index != NONE)
	{
		player_control_set_facing(input_user_index, facing);
	}
}

//.text:0053C980 ; 
//.text:0053CA10 ; 

HOOK_DECLARE(0x0053CA80, player_set_unit_index);

void __cdecl player_set_unit_index(int32 player_index, int32 unit_index)
{
	//INVOKE(0x0053CA80, player_set_unit_index, player_index, unit_index);
	HOOK_INVOKE(, player_set_unit_index, player_index, unit_index);

	player_datum* player = DATUM_GET(player_data, player_datum, player_index);
	if (player->unit_index != NONE)
	{
		unit_datum* unit = UNIT_GET(player->unit_index);
		printf("");
	}

#if 0
	player_datum* player = DATUM_GET(player_data, player_datum, player_index);
	if (player->unit_index != unit_index)
	{
		if (player->unit_index != NONE)
		{
			unit_datum* old_unit = UNIT_GET(player->unit_index);

			player_mapping_set_player_unit(player_index, NONE);
			old_unit->unit.player_index = NONE;
			old_unit->unit.last_player_index = player_index;

			old_unit->unit.consumable_energy_level = 1;
			old_unit->unit.consumable_energy_restored_game_time = NONE;

			simulation_action_object_update(player->unit_index, _simulation_unit_update_control_bit);
			unit_set_actively_controlled(player->unit_index, false);

			player->unit_index = NONE;
		}

		player->latched_action_flags = 0;
		player->momentum_timer = 0;
		player->momemtum_unknown2CE6 = 0;
		player->momentum_decay_timer = 0;
		player->momentum_falloff_timer = 0;
		player->momemtum_suppressed = false;
		player->sprinting = false;
		player->crouching = false;
		player->shooting_left = false;
		player->shooting_right = false;
		player->__unknown5E = false;
		player->__unknown5F = false;
		player->__unknown60 = 0;
		player->magnification_level = NONE;
		player->latched_control_flags = 0;
		player->__unknown62 = 0;

		set_real_point3d(&player->last_soft_ceiling_update_position,
			2.0f * 32768.0f,
			2.0f * 32768.0f,
			2.0f * 32768.0f);

		if (unit_index != NONE)
		{
			unit_datum* new_unit = UNIT_GET(unit_index);
			ASSERT(new_unit->unit.player_index == NONE);

			new_unit->unit.player_index = player_index;
			new_unit->unit.last_player_index = NONE;

			simulation_action_object_update(player->unit_index, _simulation_unit_update_control_bit);
			unit_set_actively_controlled(player->unit_index, true);

			if (player->dead_unit_index != NONE && player->dead_unit_index != unit_index)
			{
				object_adjust_garbage_timer(player->dead_unit_index, 0);
			}
			player->dead_unit_index = NONE;

			player_mapping_set_player_unit(player_index, unit_index);

			//unit_dialogue_setup(unit_index, player->configuration.client.appearance.flags.test(_female_voice_bit));

			if (current_game_engine())
			{
				new_unit->object.shield_vitality = (real32)player->multiplayer.player_traits.get_shield_vitality_traits()->get_shield_multiplier();
			}

			if (game_is_multiplayer())
			{
				const uns8 k_supression_ticks = 2;
				player->melee_suppression_timer = k_supression_ticks;
				player->grenade_suppression_timer = k_supression_ticks;
			}
		}
	}
#endif
}

bool __cdecl player_should_auto_pickup_weapon(int32 player_index, int32 weapon_index)
{
	return INVOKE(0x0053CDC0, player_should_auto_pickup_weapon, player_index, weapon_index);
}

bool __cdecl player_spawn(int32 player_index, const real_point3d* position, const real32* facing)
{
	return INVOKE(0x0053CE60, player_spawn, player_index, position, facing);
}

//.text:0053D500 ; void __cdecl player_sprint_inhibit(bool)
//.text:0053D520 ; void __cdecl player_submit_actions(int32, int32, player_action*)
//.text:0053EC70 ; bool __cdecl player_submit_assassination(int32, int32, int32, const real_point3d*, const real_vector3d*, const real_vector3d*)
//.text:0053F010 ; void __cdecl player_submit_control(int32, int32, const player_action*)

//void __cdecl player_suppress_action(int32, enum e_player_suppress_action_type_enum)
void __cdecl player_suppress_action(int32 player_index, int32 player_suppress_action_type)
{
	//INVOKE(0x0053F220, player_suppress_action, player_index, player_suppress_action_type);

	player_datum* player = DATUM_TRY_AND_GET(player_data, player_datum, player_index);

	int32 input_user = player_mapping_get_input_user(player_index);
	switch (player_suppress_action_type)
	{
	case 0:
	{
		if (!TEST_BIT(player->flags, _player_action_suppress_primary_bit))
		{
			if (global_game_globals->input_globals.index != NONE)
			{
				s_input_globals_definition* input_globals = global_game_globals->input_globals.cast_to<s_input_globals_definition>();

				player->flags |= FLAG(_player_action_suppress_primary_bit);
				player->__unknown2CD4 = game_seconds_to_ticks_round(input_globals->__unknown30 / 1000.0f);
				break;
			}

			player->__unknown2CD4 = 0;
		}

		player->flags |= FLAG(_player_action_suppress_primary_bit);
	}
	break;
	case 1:
	{
		player->flags |= FLAG(_player_action_suppress_secondary_bit);
		if (input_user != NONE)
			player_control_suppress_rotate_weapons(input_user);
	}
	break;
	case 2:
	{
		player->flags |= FLAG(_player_action_suppress_tertiary_bit);
	}
	break;
	}
}

//.text:0053F300 ; void __cdecl player_suppress_control(int32, int32)
//.text:0053F390 ; void __cdecl player_swap(int32, int32)

static bool __cdecl player_teleport(int32 player_index, int32 source_unit_index, bool always_use_raytest, const real_point3d* position)
{
	//return INVOKE(0x0053F570, player_teleport, player_index, source_unit_index, always_use_raytest, position);

	const player_datum* player = DATUM_GET(player_data, const player_datum, player_index);
	int32 unit_index = player->unit_index;
	const unit_datum* unit = UNIT_GET(unit_index);
	if (unit->object.parent_object_index != NONE)
	{
		action_submit(unit_index, _action_vehicle_exit_immediate);
	}
	real_point3d offset_position{};
	point_from_line3d(position, global_up3d, g_havok_constants.havok_collision_tolerance, &offset_position);

	bool success = player_teleport_internal(player_index, source_unit_index, &offset_position, always_use_raytest, false);
	return success;
}

bool __cdecl player_teleport(int32 player_index, int32 source_unit_index, const real_point3d* position)
{
	//return INVOKE(0x0053F550, player_teleport, player_index, source_unit_index, position);

	return player_teleport(player_index, source_unit_index, false, position);
}

bool __cdecl player_teleport_internal(int32 player_index, int32 source_unit_index, const real_point3d* position, bool always_use_raytest, bool play_teleport_effect)
{
	return INVOKE(0x0053F630, player_teleport_internal, player_index, source_unit_index, position, always_use_raytest, play_teleport_effect);
}

void __cdecl player_teleport_internal_postprocess(int32 player_index, int32 source_unit_index, bool play_teleport_effect)
{
#if 1
	INVOKE(0x0053FB80, player_teleport_internal_postprocess, player_index, source_unit_index, play_teleport_effect);
#else
	player_datum* player = DATUM_GET(player_data, player_datum, player_index);
	object_set_velocities(object_get_ultimate_parent(player->unit_index), global_zero_vector3d, global_zero_vector3d);

	{
		c_player_output_user_iterator iterator;
		iterator.begin_player(player_index);
		while (iterator.next())
		{
			chud_motion_sensor_invalidate(iterator.get_user_index());
		}
	}

	if (source_unit_index != NONE)
	{
		real_vector3d forward = UNIT_GET(source_unit_index)->object.forward;
		player_set_facing(player_index, &forward);

		real_vector3d source_unit_local_linear_velocity{};
		real_vector3d source_unit_local_angular_velocity{};
		if (TEST_BIT(_object_mask_biped, object_get_type(source_unit_index))
			&& TEST_BIT(_object_mask_biped, object_get_type(player->unit_index))
			&& object_get_early_mover_local_space_velocity(source_unit_index,
				&source_unit_local_linear_velocity,
				&source_unit_local_angular_velocity,
				false,
				false))
		{
			object_in_early_mover_join(source_unit_index, player->unit_index);
			object_set_velocities(player->unit_index, &source_unit_local_linear_velocity, NULL);
		}
	}

	if (play_teleport_effect)
	{
		player->flags |= FLAG(_player_play_coop_spawn_effect_bit);
	}
#endif
}

void __cdecl player_teleport_on_bsp_switch(int32 player_index, int32 source_unit_index, const real_point3d* position, const real_vector3d* facing, bool can_bring_vehicle)
{
#if 1
	INVOKE(0x0053FCC0, player_teleport_on_bsp_switch, player_index, source_unit_index, position, facing, can_bring_vehicle);
#else
	player_datum* player = DATUM_GET(player_data, player_datum, player_index);
	if (player->unit_index != NONE)
	{
		const unit_datum* unit = UNIT_GET(player->unit_index);
		bool success = false;
		if (unit->object.parent_object_index != NONE)
		{
			if (object_get_ultimate_parent(unit->object.parent_object_index) != object_get_ultimate_parent(source_unit_index))
			{
				const int32 ultimate_parent_index = object_get_ultimate_parent(player->unit_index);
				if (can_bring_vehicle
					&& TEST_BIT(_object_mask_vehicle, object_get_type(ultimate_parent_index))
					&& vehicle_can_be_teleported(ultimate_parent_index))
				{
					object_set_position(ultimate_parent_index, NULL, facing, global_up3d, NULL);
					success = player_teleport_internal(player_index, source_unit_index, position, false, true);
				}

				if (!success)
				{
					action_submit(player->unit_index, _action_vehicle_exit_immediate);
				}
			}
		}

		if (!success)
		{
			object_set_position(player->unit_index, NULL, facing, global_up3d, NULL);
			unit_set_aiming_vectors(player->unit_index, facing, facing);
			success = player_teleport_internal(player_index, source_unit_index, position, true, true);
		}

		if (!success)
		{
			if (player->unit_index != NONE)
			{
				int32 source_unit_ultimate_parent_index = object_get_ultimate_parent(source_unit_index);
				int32 unit_ultimate_parent_index = object_get_ultimate_parent(player->unit_index);

				s_location source_unit_location{};
				object_get_location(source_unit_ultimate_parent_index, &source_unit_location);

				real_point3d source_unit_center_of_mass{};
				object_get_center_of_mass(source_unit_ultimate_parent_index, &source_unit_center_of_mass);

				real_point3d unit_center_of_mass{};
				object_get_center_of_mass(unit_ultimate_parent_index, &unit_center_of_mass);

				real_point3d unit_origin{};
				object_get_origin(unit_ultimate_parent_index, &unit_origin);

				real_vector3d origin_offset{};
				vector_from_points3d(&unit_center_of_mass, &unit_origin, &origin_offset);

				real_point3d new_origin{};
				point_from_line3d(&source_unit_center_of_mass, &origin_offset, 1.0f, &new_origin);

				object_set_position(unit_ultimate_parent_index, &new_origin, NULL, NULL, &source_unit_location);
				player_teleport_internal_postprocess(player_index, source_unit_index, true);

				event(_event_warning, "failed to used standard teleporation techniques, teleporting directly to the center of mass of the source unit.");
			}
			else
			{
				player->respawn_in_progress = true;
				player->respawn_forced = true;
				player->single_player_respawn_timer = 0;

				event(_event_warning, "critical teleporter malfunction!!!  player landed on a romulan bird of prey and is no more :-(");
			}
		}
	}
#endif
}
//.text:0053FED0 ; int16 __cdecl player_terminals_accessed_bitvector_get()
//.text:0053FEF0 ; void __cdecl player_terminals_accessed_bitvector_set(int16)
//.text:0053FF20 ; int16 __cdecl player_terminals_read_bitvector_get()
//.text:0053FF40 ; void __cdecl player_terminals_read_bitvector_set(int16)
//.text:0053FF70 ; int32 __cdecl player_try_and_get_player_index_from_absolute_player_index(int32)

bool __cdecl player_try_to_drop_weapon(int32 player_index, bool primary_weapon)
{
	return INVOKE(0x0053FFC0, player_try_to_drop_weapon, player_index, primary_weapon);
}

//.text:00540070 ; bool __cdecl player_try_to_put_away_weapon(int32, bool)
//.text:00540150 ; bool __cdecl player_try_to_swap_weapons(int32, bool, const s_player_interaction*)
//.text:00540320 ; void __cdecl player_unit_calculate_location_pesemistic(int32, s_location*)

int32 __cdecl player_unit_get_representation_index(int32 unit_index)
{
	return INVOKE(0x00540350, player_unit_get_representation_index, unit_index);
}

// not the correct name
int32 __cdecl player_unit_get_control_index(int32 unit_index)
{
	return INVOKE(0x00540440, player_unit_get_control_index, unit_index);
}

//.text:00540490 ; bool __cdecl player_unit_should_teleport_to_unit(int32, int32, int32)
//.text:00540510 ; 

void __cdecl player_update_invisibility(int32 player_index)
{
	INVOKE(0x005405A0, player_update_invisibility, player_index);
}

//.text:00540650 ; void __cdecl player_update_reactive_armor(int32)
//.text:00540730 ; void __cdecl player_update_tank_mode(int32)

void __cdecl player_use_multiplayer_powerup(int32 player_index, int32 equipment_index)
{
	INVOKE(0x005408E0, player_use_multiplayer_powerup, player_index, equipment_index);
}

//.text:00540A70 ; 
//.text:00540A80 ; void __cdecl player_validate_configuration(int32, s_player_configuration*)
//.text:00540AE0 ; bool __cdecl player_waiting_to_respawn_compare(int32, int32, const void*)
//.text:00540B30 ; void __cdecl player_weapon_pickup_inhibit(bool)
//.text:00540B50 ; int32 __cdecl players_active_zone_set_switch_trigger_get(void)

bool __cdecl players_all_are_dead()
{
	return INVOKE(0x00540B80, players_all_are_dead);
}

bool __cdecl players_any_are_dead()
{
	return INVOKE(0x00540BA0, players_any_are_dead);
}

bool __cdecl players_any_are_in_the_air(int32* out_unit_index)
{
	return INVOKE(0x00540BC0, players_any_are_in_the_air, out_unit_index);
}

bool __cdecl players_any_are_near_death(int32* out_unit_index)
{
	return INVOKE(0x00540D10, players_any_are_near_death, out_unit_index);
}

//.text:00540DC0 ; bool __cdecl players_arm_unit_from_campaign_armaments_data(const s_campaign_armaments_player*, int32)
//.text:00540F30 ; void __cdecl players_build_persistent_player_options(game_options*)
//.text:00541060 ; int32 __cdecl players_build_weapon_from_campaign_armaments_data(const s_campaign_armaments_weapon*, int32, bool*)
//.text:00541250 ; void __cdecl players_calculate_safe_position(int32, real_point3d*, real_vector3d*)

void __cdecl players_compute_combined_pvs(s_game_cluster_bit_vectors* combined_pvs, bool local_only, t_cluster_activation_reason* activation_reason)
{
	//INVOKE(0x00541310, players_compute_combined_pvs, combined_pvs, local_only, activation_reason);

	int32 cluster_reference_count = 0;
	s_cluster_reference cluster_references[16]{};

	c_data_iterator<player_datum> player_iterator;
	player_iterator.begin(player_data);
	while (player_iterator.next())
	{
		if (local_only)
		{
			for (int32 player_index = player_mapping_get_first_output_user(player_iterator.get_index());
				player_index != NONE;
				player_index = player_mapping_get_next_output_user(player_iterator.get_index(), player_index))
			{
				const s_observer_result* camera = observer_get_camera(player_index);
				cluster_references[cluster_reference_count++] = camera->location.cluster_reference;
			}
		}
		else
		{
			player_datum* player = player_iterator.get_datum();
			cluster_references[cluster_reference_count++] = player->cluster_reference;
		}
	}

	csmemset(combined_pvs, 0, sizeof(s_game_cluster_bit_vectors));

	for (int32 cluster_reference_index = 0; cluster_reference_index < cluster_reference_count; cluster_reference_index++)
	{
		s_cluster_reference& cluster_reference = cluster_references[cluster_reference_index];
		ASSERT(!cluster_reference_valid(&cluster_reference) || scenario_cluster_reference_valid(&cluster_reference));

		if (!cluster_reference_valid(&cluster_reference))
		{
			continue;
		}

		s_game_cluster_bit_vectors player_active_pvs{};
		structure_bsp_compute_cluster_active_pvs(cluster_reference, &player_active_pvs);

		if (cluster_reference_valid(&cluster_reference))
		{
			s_scenario_pvs_row player_row{};
			scenario_zone_set_pvs_get_row(global_scenario_index, &player_row, scenario_zone_set_index_get(), cluster_reference, false);

			for (int32 structure_bsp_index = global_structure_bsp_first_active_index_get();
				structure_bsp_index != NONE;
				structure_bsp_index = global_structure_bsp_next_active_index_get(structure_bsp_index))
			{
				if (activation_reason)
				{
					struct structure_bsp* structure_bsp = global_structure_bsp_get(structure_bsp_index);
					for (int32 cluster_index = 0; cluster_index < structure_bsp->clusters.count; cluster_index++)
					{
						s_cluster_reference test_cluster_reference{};
						cluster_reference_set(&test_cluster_reference, structure_bsp_index, cluster_index);

						if (!game_clusters_test(combined_pvs, test_cluster_reference)
							&& scenario_zone_set_pvs_row_test(global_scenario_index, &player_row, test_cluster_reference))
						{
							if (game_clusters_test(&player_active_pvs, test_cluster_reference))
							{
								activation_reason->element(structure_bsp_index).element(cluster_index) = _cluster_activation_reason_player_pvs;
							}
							else
							{
								activation_reason->element(structure_bsp_index).element(cluster_index) = _cluster_activation_reason_player_pvs_inactive;
							}
						}
					}
				}
			}
		}

		game_clusters_or(combined_pvs, &player_active_pvs, combined_pvs);
	}
}

//.text:00541400 ; void __cdecl players_coop_desire_respawn(int32)
//.text:00541480 ; void __cdecl players_coop_update_respawn(int32)
//.text:00541920 ; void __cdecl players_death_status_calculate(bool*, bool*)

void __cdecl players_detach_from_map()
{
	INVOKE(0x005419A0, players_detach_from_map);
}

void __cdecl players_dispose()
{
	INVOKE(0x00541AF0, players_dispose);
}

void __cdecl players_dispose_from_old_map()
{
	INVOKE(0x00541B30, players_dispose_from_old_map);
}

void __cdecl players_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask)
{
	INVOKE(0x00541B70, players_dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);
}

void __cdecl players_finish_creation()
{
	INVOKE(0x00541B80, players_finish_creation);
}

//.text:00541BA0 ; void __cdecl players_force_detach_from_object(int32)

int32 __cdecl players_get_active_and_in_game_count(bool include_joined_in_progress_players)
{
	return INVOKE(0x00541C70, players_get_active_and_in_game_count, include_joined_in_progress_players);
}

//.text:00541CF0 ; int32 __cdecl players_get_alive_count()
//.text:00541D60 ; const s_campaign_armaments_player* __cdecl players_get_campaign_armaments_player_from_player_index(int32)
//.text:00541DF0 ; const uns32* __cdecl players_get_combined_pvs()
//.text:00541E10 ; const uns32* __cdecl players_get_combined_pvs_local()
//.text:00541E30 ; bool __cdecl players_get_local_machine(s_machine_identifier*)
//.text:00541E90 ; int32 __cdecl players_get_local_machine_index()
//.text:00541EB0 ; const s_machine_identifier* __cdecl players_get_machine_identifier(int32)
//.text:00541EE0 ; int32 __cdecl players_get_machine_index(const s_machine_identifier*)
//.text:00541F60 ; void __cdecl players_get_machines(uns32*, s_machine_identifier*)
//.text:00541FA0 ; int16 __cdecl players_get_respawn_failure()
//.text:00541FC0 ; void __cdecl players_get_sorted_lifeless_waiting_to_respawn_list(int32, int32*, int32, int32*)
//.text:00542070 ; int32 __cdecl players_get_total_players_in_game()
//.text:00542090 ; bool __cdecl players_globals_exists()
//.text:005420B0 ; void __cdecl players_handle_deleted_object(int32)

void __cdecl players_handle_deleted_player_internal(int32 player_index)
{
	INVOKE(0x005422A0, players_handle_deleted_player_internal, player_index);
}

void __cdecl players_initialize()
{
	INVOKE(0x005422C0, players_initialize);
}

void __cdecl players_initialize_for_new_map()
{
	INVOKE(0x00542340, players_initialize_for_new_map);
}

void __cdecl players_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask)
{
	INVOKE(0x00542460, players_initialize_for_new_structure_bsp, activating_structure_bsp_mask);
}

//.text:00542470 ; void __cdecl players_initialize_for_saved_game(int32)
//.text:00542520 ; void __cdecl players_joined_in_progress_allow_spawn()
//.text:005425A0 ; bool __cdecl players_joined_in_progress_and_waiting_to_respawn()

void __cdecl players_rebuild_user_mapping(bool force_rebuild)
{
	INVOKE(0x00542620, players_rebuild_user_mapping, force_rebuild);
}

//.text:005427A0 ; void __cdecl players_remap_local_machine(const s_machine_identifier*, const game_player_options*)

void __cdecl players_set_local_machine(const s_machine_identifier* machine_identifier)
{
	INVOKE(0x00542D60, players_set_local_machine, machine_identifier);
}

void __cdecl players_set_machines(uns32 new_machine_valid_mask, const s_machine_identifier* new_machine_identifiers)
{
	INVOKE(0x00542F80, players_set_machines, new_machine_valid_mask, new_machine_identifiers);

	//ASSERT(VALID_BITS(new_machine_valid_mask, k_maximum_machines));
	//ASSERT(new_machine_identifiers);
	//
	//for (int32 machine_index = 0; machine_index < k_maximum_machines; machine_index++)
	//{
	//	if (!TEST_BIT(new_machine_valid_mask, machine_index))
	//		continue;
	//
	//	for (int32 other_machine_index = 0; other_machine_index < k_maximum_machines; other_machine_index++)
	//	{
	//		if (other_machine_index != machine_index && TEST_BIT(new_machine_valid_mask, other_machine_index))
	//			ASSERT(csmemcmp(&new_machine_identifiers[machine_index], &new_machine_identifiers[other_machine_index], sizeof(s_machine_identifier)) != 0);
	//	}
	//}
	//
	//uns32 old_machine_valid_mask = players_globals->machine_valid_mask;
	//
	//s_machine_identifier old_machine_identifiers[k_maximum_machines];
	//int32 new_machine_indices[k_maximum_machines];
	//csmemcpy(old_machine_identifiers, players_globals->machine_identifiers.begin(), sizeof(s_machine_identifier) * k_maximum_machines);
	//csmemset(new_machine_indices, NONE, sizeof(int32) * k_maximum_machines);
	//
	//for (int32 machine_index = 0; machine_index < k_maximum_machines; machine_index++)
	//{
	//	if (!TEST_BIT(old_machine_valid_mask, machine_index))
	//		continue;
	//
	//	for (int32 other_machine_index = 0; other_machine_index < k_maximum_machines; other_machine_index++)
	//	{
	//		if (TEST_BIT(new_machine_valid_mask, other_machine_index) && !csmemcmp(&new_machine_identifiers[other_machine_index], &old_machine_identifiers[machine_index], sizeof(s_machine_identifier)))
	//			new_machine_indices[machine_index] = other_machine_index;
	//	}
	//}
	//
	//c_data_iterator<player_datum> player_iterator;
	//player_iterator.begin(*player_data);
	//while (player_iterator.next())
	//{
	//	player_datum* player = player_iterator.get_datum();
	//	ASSERT(TEST_BIT(player->flags, _player_left_game_bit) == (player->machine_index == NONE));
	//
	//	if (player->machine_index != NONE)
	//	{
	//		int32 new_machine_index = new_machine_indices[player->machine_index];
	//
	//		ASSERT(player->machine_index >= 0 && player->machine_index < k_maximum_machines);
	//		ASSERT(TEST_BIT(old_machine_valid_mask, player->machine_index));
	//		ASSERT(csmemcmp(&player->machine_identifier, &old_machine_identifiers[player->machine_index], sizeof(player->machine_identifier)) == 0);
	//
	//		if (new_machine_index != NONE)
	//		{
	//			ASSERT(new_machine_index >= 0 && new_machine_index < k_maximum_machines);
	//			ASSERT(TEST_BIT(players_globals->machine_valid_mask, new_machine_index));
	//			ASSERT(csmemcmp(&player->machine_identifier, &players_globals->machine_identifiers[new_machine_index], sizeof(player->machine_identifier)) == 0);
	//
	//			player_leave_game_internal(player_iterator.get_index());
	//		}
	//	}
	//	ASSERT(TEST_BIT(player->flags, _player_left_game_bit) == (player->machine_index == NONE));
	//}
	//
	//players_globals->machine_valid_mask = new_machine_valid_mask;
	//csmemcpy(players_globals->machine_identifiers.begin(), new_machine_identifiers, sizeof(s_machine_identifier) * k_maximum_machines);
	//
	//player_iterator.begin(*player_data);
	//while (player_iterator.next())
	//{
	//	player_datum* player = player_iterator.get_datum();
	//	ASSERT(TEST_BIT(player->flags, _player_left_game_bit) == (player->machine_index == NONE));
	//
	//	int32 new_machine_index = new_machine_indices[player->machine_index];
	//	if (player->machine_index != NONE)
	//		player->machine_index = new_machine_index;
	//}
	//
	//if (players_globals->local_machine_exists)
	//{
	//	int32 new_machine_index = NONE;
	//	for (int32 machine_index = 0; machine_index < k_maximum_machines; machine_index++)
	//	{
	//		if (TEST_BIT(new_machine_valid_mask, machine_index) && csmemcmp(&new_machine_identifiers[machine_index], &players_globals->local_machine_identifier, sizeof(s_machine_identifier)) == 0)
	//		{
	//			new_machine_index = machine_index;
	//			break;
	//		}
	//	}
	//
	//	if (new_machine_index != NONE)
	//	{
	//		if (players_globals->local_machine_index == NONE)
	//		{
	//			players_set_local_machine(&players_globals->local_machine_identifier);
	//			players_rebuild_user_mapping(true);
	//		}
	//		else
	//		{
	//			players_globals->local_machine_index = new_machine_index;
	//		}
	//	}
	//}
	//
	//players_verify();
}

//.text:005431B0 ; void __cdecl players_set_safe_to_respawn_for_player(bool)
//.text:005431E0 ; void __cdecl players_store_campaign_armamements_weapon(int32, s_campaign_armaments_weapon*)
//.text:005432D0 ; void __cdecl players_store_campaign_armaments_on_game_won(s_campaign_armaments*)
//.text:00543560 ; void __cdecl players_update_activation()

void verify_coop_respawn_effect()
{
	c_data_iterator<player_datum> player_iterator;
	player_iterator.begin(player_data);
	while (player_iterator.next())
	{
		player_datum* player = player_iterator.get_datum();
		if (!TEST_BIT(player->flags, _player_play_coop_spawn_effect_bit))
			continue;

		int32 control_index = player_unit_get_control_index(player->unit_index);
		if (VALID_INDEX(control_index, global_game_globals->player_information.count))
			continue;

		SET_BIT(player->flags, _player_play_coop_spawn_effect_bit, false);
	}
}

void __cdecl players_update_after_game(const struct simulation_update* update)
{
	verify_coop_respawn_effect();

	INVOKE(0x00543650, players_update_after_game, update);
}

void __cdecl players_update_before_game(const struct simulation_update* update)
{
	INVOKE(0x00543D20, players_update_before_game, update);
}

//.text:00544370 ; void __cdecl players_update_for_checkpoint()
//.text:00544380 ; void __cdecl players_validate_configuration()

void __cdecl players_verify()
{
	INVOKE(0x00544400, players_verify);
}

//.text:00544410 ; bool __cdecl players_zone_set_switch_trigger_active()
//.text:00544440 ; void __cdecl players_zone_set_switch_trigger_clear()
//.text:00544610 ; 
//.text:00544680 ; 

s_s3d_player_armor_configuration_loadout* __cdecl player_get_armor_loadout(player_datum* player)
{
	s_s3d_player_armor_configuration_loadout& loadout = player->configuration.host.armor.loadouts[player->armor_loadout_index];

	return &loadout;
}

// this function gets called in `player_spawn` -> `game_engine_add_starting_equipment`
// $TODO hook `game_engine_add_starting_equipment` and reimplement the original functionality
s_s3d_player_weapon_configuration_loadout* __cdecl player_get_weapon_loadout(player_datum* player)
{
	s_s3d_player_weapon_configuration_loadout& loadout = player->configuration.host.weapon.loadouts[player->weapon_loadout_index];

	// allow player traits override
	// $TODO pull these from tags
	loadout.primary_weapon_index = 0xFF;
	loadout.secondary_weapon_index = 0xFF;

	return &loadout;
}

// find a better name?
bool customized_area_selection_from_name(s_multiplayer_customized_model_selection& selection, const char* selection_name)
{
	const char* selection_name_str = selection.selection_name.get_string();

	if (selection.third_person_armor_object.index != NONE /*&& selection.first_person_armor_object.index != NONE*/)
	{
		if (selection_name_str && csstricmp(selection_name, selection_name_str) == 0)
			return true;
	}

	return false;
}

// find a better name?
int32 customized_spartan_character_from_name(s_multiplayer_customized_model_character& character, const char* region_or_biped_name, const char* selection_name)
{
	const char* armor_region = character.armor_region.get_string();
	const char* biped_region = character.biped_region.get_string();

	if (armor_region && csstricmp(region_or_biped_name, armor_region) == 0)
	{
		for (int32 selection_absolute_index = 0; selection_absolute_index < character.customized_selection.count; selection_absolute_index++)
		{
			s_multiplayer_customized_model_selection& customized_model_selection = character.customized_selection[selection_absolute_index];
			if (customized_area_selection_from_name(customized_model_selection, selection_name))
				return selection_absolute_index;
		}
	}

	if (biped_region && csstricmp(region_or_biped_name, biped_region) == 0)
	{
		for (int32 selection_absolute_index = 0; selection_absolute_index < character.customized_selection.count; selection_absolute_index++)
		{
			s_multiplayer_customized_model_selection& customized_model_selection = character.customized_selection[selection_absolute_index];
			if (customized_area_selection_from_name(customized_model_selection, selection_name))
				return selection_absolute_index;
		}
	}

	return 0;
}

int32 multiplayer_universal_data_get_absolute_equipment_block_index(const char* name)
{
	s_multiplayer_universal_globals_definition* universal_data = scenario_multiplayer_globals_try_and_get_universal_data();
	if (!universal_data)
		return 0;

	int32 result = 0;
	for (int32 equipment_absolute_index = 0; equipment_absolute_index < universal_data->equipment.count; equipment_absolute_index++)
	{
		s_multiplayer_equipment& equipment = universal_data->equipment[equipment_absolute_index];
		const char* equipment_name = equipment.name.get_string();
		if (equipment_name && csstricmp(name, equipment_name) == 0)
		{
			result = equipment_absolute_index;
			break;
		}
	}

	return result;
}

// find a better name?
int32 multiplayer_universal_data_get_absolute_customized_spartan_character_block_index(const char* region_or_biped_name, const char* selection_name)
{
	if (!g_cache_file_globals.tags_loaded)
		return 0;

	s_multiplayer_universal_globals_definition* universal_data = scenario_multiplayer_globals_try_and_get_universal_data();
	if (!universal_data)
		return 0;

	int32 result = 0;
	for (s_multiplayer_customized_model_character& customized_model_character : universal_data->customized_spartan_characters)
	{
		result = customized_spartan_character_from_name(customized_model_character, region_or_biped_name, selection_name);
		if (result != 0)
			break;
	}

	return result;
}

// find a better name?
int16 multiplayer_universal_data_get_absolute_weapons_selection_block_index(const char* selection_name)
{
	s_multiplayer_universal_globals_definition* universal_data = scenario_multiplayer_globals_try_and_get_universal_data();
	if (!universal_data)
		return int16(0xFFFD);

	if (universal_data->weapon_selections.count)
	{
		for (int16 weapon_selection_index = 0; weapon_selection_index < universal_data->weapon_selections.count; weapon_selection_index++)
		{
			s_multiplayer_weapon_selection& weapon_selection = universal_data->weapon_selections[weapon_selection_index];

			const char* name = weapon_selection.name.get_string();
			if (name && csstricmp(selection_name, name) == 0)
				return weapon_selection_index;
		}
	}

	return int16(0xFFFD);
}

bool scenario_is_solo()
{
	struct scenario* scenario = global_scenario_get();
	return scenario && scenario->type == _scenario_type_solo;
}

int32 get_spartan_representation_index()
{
	static string_id sp_name = string_id_retrieve("spartan");
	static string_id mp_name = string_id_retrieve("mp_spartan");

	string_id name = scenario_is_solo() ? sp_name : mp_name;

	if (s_game_globals* game_globals = scenario_get_game_globals())
	{
		for (int32 i = 0; i < game_globals->player_representation.count; i++)
		{
			if (name == game_globals->player_representation[i].name.get_value())
				return i;
		}
	}

	return 2;
}

int32 get_elite_representation_index()
{
	static string_id sp_name = string_id_retrieve("sp_elite");
	static string_id mp_name = string_id_retrieve("mp_elite");

	string_id name = scenario_is_solo() ? sp_name : mp_name;

	int32 result = 3;
	if (s_game_globals* game_globals = scenario_get_game_globals())
	{
		for (int32 i = 0; i < game_globals->player_representation.count; i++)
		{
			if (name == game_globals->player_representation[i].name.get_value())
			{
				result = i;
				break;
			}
		}
	}

	return result;
}

void test_player_teleport(int32 user_index, int32 source_user_index)
{
#if 0
	int32 player_index = player_mapping_get_player_by_input_user(user_index);
	int32 unit_index = player_mapping_get_unit_by_input_user(user_index);
	int32 source_unit_index = player_mapping_get_unit_by_input_user(source_user_index);
	if (player_index != NONE && unit_index != NONE && source_unit_index != NONE)
	{
		player_teleport_internal(player_index, source_unit_index, &UNIT_GET(source_unit_index)->object.bounding_sphere_center, true, false);
	}
#endif
}

void apply_player_representation_fixup()
{
	static t_value_type<int32> spartan_representation_index = { .value = 2 };
	static t_value_type<int32> elite_representation_index = { .value = 3 };

	static const uns32 spartan_representation_addresses[] = { 0x00537482 + 1, 0x0053761A + 1, 0x00539EB9 + 1, 0x00539FBD + 1, 0x0053A738 + 1, 0x0053A7C6 + 1 };
	static const uns32 elite_representation_addresses[] = { 0x0053748F + 1, 0x00537627 + 1, 0x00539EC6 + 1, 0x00539FCA + 1, 0x0053A745 + 1, 0x0053A7BE + 1 };

	DATA_PATCH_ARRAY_DECLARE2(spartan_representation_addresses, spartan_representation_index, spartan_representation_index.bytes);
	DATA_PATCH_ARRAY_DECLARE2(elite_representation_addresses, elite_representation_index, elite_representation_index.bytes);

	spartan_representation_index.value = get_spartan_representation_index();
	elite_representation_index.value = get_elite_representation_index();

	spartan_representation_index_patch.apply(false);
	elite_representation_index_patch.apply(false);
}

