#include "game/players.hpp"

#include "cache/cache_files.hpp"
#include "game/cheats.hpp"
#include "game/game_engine_notifications.hpp"
#include "game/multiplayer_definitions.hpp"
#include "input/input_abstraction.hpp"
#include "interface/interface_constants.hpp"
#include "items/equipment.hpp"
#include "items/weapons.hpp"
#include "memory/bitstream.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "render/render_debug.hpp"
#include "scenario/scenario.hpp"
#include "simulation/game_interface/simulation_game_action.hpp"
#include "text/draw_string.hpp"
#include "units/bipeds.hpp"

HOOK_DECLARE(0x00536020, player_get_armor_loadout);
HOOK_DECLARE(0x00536680, player_get_weapon_loadout);
HOOK_DECLARE(0x00539B20, player_find_action_context);
HOOK_DECLARE(0x0053F220, player_suppress_action);
HOOK_DECLARE_CLASS_MEMBER(0x00B26390, s_emblem_info, decode);
HOOK_DECLARE_CLASS_MEMBER(0x00B267B0, s_emblem_info, encode);

bool debug_player_network_aiming = false;
bool debug_objects_biped_melee_in_range = false;
bool debug_render_players = false;
string_id g_player_desired_mode_override = NONE;
long g_character_physics_override = _character_physics_default;

//.text:00434720 ; public: s_player_configuration::s_player_configuration()
s_player_configuration::s_player_configuration()
{
	client = {};
	host = {};
}

//.text:00434760 ; public: s_player_configuration_from_client::s_player_configuration_from_client()
s_player_configuration_from_client::s_player_configuration_from_client()
{
	memset(this, 0, sizeof(*this));
}

//.text:00434780 ; public: s_player_configuration_from_host::s_player_configuration_from_host()
s_player_configuration_from_host::s_player_configuration_from_host()
{
	memset(this, 0, sizeof(*this));
	team_index = _multiplayer_team_none;
	assigned_team_index = _multiplayer_team_none;
}

s_player_identifier::s_player_identifier() :
	identifier(0)
{
}

s_player_identifier::s_player_identifier(qword data) :
	identifier(0)
{
	csmemcpy(identifier, &data, sizeof(identifier));
}

s_player_identifier::s_player_identifier(dword _ipv4_address, word _port, word_flags _flags) :
	identifier(0)
{
	REFERENCE_DECLARE(identifier + 0, dword, ipv4_address);
	REFERENCE_DECLARE(identifier + 4, word, port);
	REFERENCE_DECLARE(identifier + 6, word, flags);
	ipv4_address = _ipv4_address;
	port = _port;
	flags = _flags;
}

s_player_identifier::s_player_identifier(transport_address const* address) :
	identifier(0)
{
	REFERENCE_DECLARE(identifier + 0, dword, ipv4_address);
	REFERENCE_DECLARE(identifier + 4, word, port);
	REFERENCE_DECLARE(identifier + 6, word, flags);
	ipv4_address = address->ipv4_address;
	port = address->port;
	flags = address->address_length;
}

void c_player_in_game_iterator::begin()
{
	TLS_DATA_GET_VALUE_REFERENCE(player_data);
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

long c_player_in_game_iterator::get_index() const
{
	return m_iterator.iterator.index;
}

short c_player_in_game_iterator::get_absolute_index() const
{
	return m_iterator.get_absolute_index();
}

void c_player_with_unit_iterator::begin()
{
	TLS_DATA_GET_VALUE_REFERENCE(player_data);
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

long c_player_with_unit_iterator::get_index() const
{
	return m_iterator.iterator.index;
}

short c_player_with_unit_iterator::get_absolute_index() const
{
	return m_iterator.get_absolute_index();
}

void player_override_desired_mode(long desired_mode)
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
	TLS_DATA_GET_VALUE_REFERENCE(player_data);

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
		//		// $TODO: implement this
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
		//		// $TODO: find the bit index
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

		char const* string_const = string_id_get_string_const(g_player_desired_mode_override);
		csnzprintf(string, sizeof(string), "Player Forced Into Mode: %s|n", string_const);

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
		//char const* character_physics_override = g_character_physics_override < k_total_character_physics_overrides ? global_character_physics_override_names[g_character_physics_override] : "OUT OF RANGE!";
		//csnzprintf(string, sizeof(string), "Character Physics Override: %s|n", character_physics_override);
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
//.text:005375B0 ; long __cdecl find_best_starting_location_index(long, bool, bool)
//.text:00537860 ; void __cdecl map_editor_process_player_control(long, player_action*)
//.text:005379E0 ; void __cdecl player_action_clear(player_action*)

void __cdecl player_action_context_clear(s_player_action_context* action_context)
{
	INVOKE(0x00537A50, player_action_context_clear, action_context);
}

//.text:00537A80 ; bool __cdecl player_action_context_valid(s_player_action_context const*)
//.text:00537AB0 ; bool __cdecl player_action_valid(player_action const*)
//.text:00537C90 ; 
//.text:00537D10 ; 
//.text:00537DB0 ; void __cdecl player_active_camo_screen_effect(long)
//.text:00537EA0 ; bool __cdecl player_active_camouflage_on()
//.text:00537F20 ; bool __cdecl sub_537F20(player_datum*, long)
//.text:00537F90 ; void __cdecl player_appearance_initialize(s_player_appearance*)
//.text:00537FB0 ; bool __cdecl player_appearance_valid(s_player_appearance const*)
//.text:00537FE0 ; void __cdecl player_approve_pickup_weapon(long, long, unit_weapon_pickup_result*)
//.text:005381F0 ; void __cdecl player_build_aiming_vector_from_facing(long, real, real, real_vector3d*)
//.text:005384C0 ; void __cdecl player_build_facing_vector_from_throttle(long, real_vector3d const*, real_vector2d const*, real_vector3d*, real_vector3d*)
//.text:00538510 ; bool __cdecl player_can_assassinate_object(long, long)
//.text:005385F0 ; bool __cdecl player_can_fancy_assassinate_object(long, long)
//.text:005386D0 ; bool __cdecl sub_5386D0(player_datum*)
//.text:00538730 ; void __cdecl player_clear_assassination_state(long)
//.text:005387A0 ; void __cdecl player_configuration_initialize(s_player_configuration*)

bool __cdecl player_consider_biped_interaction(long player_index, long biped_index, s_player_action_context* result)
{
	return INVOKE(0x005387F0, player_consider_biped_interaction, player_index, biped_index, result);
}

bool __cdecl player_consider_device_interaction(long player_index, long device_index, s_player_action_context* result)
{
	return INVOKE(0x005388D0, player_consider_device_interaction, player_index, device_index, result);
}

bool __cdecl player_consider_equipment_interaction(long player_index, long equipment_index, s_player_action_context* result)
{
	TLS_DATA_GET_VALUE_REFERENCE(player_data);

	player_datum* player = DATUM_GET(player_data, player_datum, player_index);
	unit_datum* unit = UNIT_GET(player->unit_index);
	equipment_datum* equipment = EQUIPMENT_GET(equipment_index);

	if (equipment->item.inventory_state || equipment->item.ignore_object_index == player->unit_index)
		return false;

	if (!point_in_sphere(&equipment->object.bounding_sphere_center, &unit->object.bounding_sphere_center, unit->object.bounding_sphere_radius + 0.4f))
		return false;

	if (unit_get_current_equipment(player->unit_index, 0) == NONE || !unit_can_pickup_equipment(player->unit_index, equipment_index))
		return false;

	long active_primary_weapon = unit_get_active_primary_weapon(player->unit_index, NULL);
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

bool __cdecl player_consider_unit_interaction(long player_index, long unit_index, s_player_action_context* result)
{
	return INVOKE(0x005389E0, player_consider_unit_interaction, player_index, unit_index, result);
}

bool __cdecl player_consider_vehicle_interaction(long player_index, long vehicle_index, s_player_action_context* result)
{
	return INVOKE(0x00538B10, player_consider_vehicle_interaction, player_index, vehicle_index, result);
}

bool __cdecl player_consider_weapon_interaction(long player_index, long weapon_index, s_player_action_context* result)
{
	return INVOKE(0x00538D00, player_consider_weapon_interaction, player_index, weapon_index, result);
}

//.text:00538ED0 ; void __cdecl player_copy_object_appearance(long, long)
//.text:00538FD0 ; void __cdecl sub_538FD0(long, long)

void __cdecl player_delete(long player_index)
{
	INVOKE(0x00539050, player_delete, player_index);

	//TLS_DATA_GET_VALUE_REFERENCE(player_data);
	//
	//player_set_unit_index(player_index, NONE);
	//player_mapping_set_input_user(player_index, NONE);
	//player_mapping_set_input_controller(player_index, k_no_controller);
	//player_mapping_detach_output_users(player_index);
	//players_rebuild_user_mapping(false);
	//datum_delete(*player_data, player_index);
	//game_engine_player_deleted(player_index);
	//players_handle_deleted_player_internal(player_index);
}

//.text:005390B0 ; void __cdecl player_died(long)
//.text:005391D0 ; void __cdecl player_died_update_for_multiplayer(long)
//.text:00539210 ; 
//.text:00539220 ; 

bool __cdecl player_evaluate_interaction(long player_index, s_player_interaction const* interaction, s_player_interaction* current_interaction)
{
	return INVOKE(0x00539240, player_evaluate_interaction, player_index, interaction, current_interaction);
}

//.text:005392F0 ; real __cdecl player_evaluate_interaction_compute_weight(long, long)
//.text:005394A0 ; void __cdecl player_examine_nearby_item(long, long)
//.text:00539900 ; void __cdecl player_examine_nearby_objects(long)
//.text:00539A30 ; bool __cdecl player_fancy_assassinate_object(long, long)

void __cdecl player_find_action_context(long player_index, s_player_action_context* out_action_context)
{
	//INVOKE(0x00539B20, player_find_action_context, player_index, out_action_context);

	TLS_DATA_GET_VALUE_REFERENCE(player_data);

	player_datum* player = DATUM_GET(player_data, player_datum, player_index);

	player_action_context_clear(out_action_context);

	if (player->unit_index == NONE)
		return;

	unit_datum* unit = UNIT_GET(player->unit_index);
	if (unit->object.parent_object_index != NONE && !TEST_BIT(player->flags, _player_unknown_bit14))
		return;

	s_location location{};
	object_get_location(player->unit_index, &location);

	real search_radius = (unit->object.bounding_sphere_radius + 0.4f) + 0.1f;

	long object_indices[64]{};
	long object_count = objects_in_sphere(
		0,
		0x2BBF,
		&location,
		&unit->object.bounding_sphere_center,
		search_radius,
		object_indices,
		NUMBEROF(object_indices));

	long index = 0;
	for (long index = 0; index < object_count; index++)
	{
		object_datum* object = object_get(object_indices[index]);
		e_object_type object_type = object->object.object_identifier.get_type();

		if (TEST_BIT(0x1440, object_type))
			continue;

		bool search_children = false;

		if (!TEST_BIT(0x304, object_type) || point_in_sphere(&object->object.bounding_sphere_center, &unit->object.bounding_sphere_center, search_radius + object->object.bounding_sphere_radius))
		{
			switch (object_type)
			{
			case _object_type_biped:
				player_consider_biped_interaction(player_index, object_indices[index], out_action_context);
				break;
			case _object_type_vehicle:
				player_consider_vehicle_interaction(player_index, object_indices[index], out_action_context);
				search_children = true;
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
				search_children = true;
				break;
			}

			if (search_children)
			{
				object_datum* child_object = NULL;

				for (long child_object_index = object->object.first_child_object_index;
					child_object_index != NONE && object_count < NUMBEROF(object_indices);
					child_object_index = child_object->object.next_object_index)
				{
					child_object = object_get(child_object_index);
					object_indices[object_count++] = child_object_index;
				}
			}

			if (TEST_MASK(_object_mask_unit, object_type))
				player_consider_unit_interaction(player_index, object_indices[index], out_action_context);
		}
	}
}

//.text:00539E30 ; bool __cdecl player_find_best_spawn_location(long, real_point3d*, real*, real*, bool, bool)
//.text:00539F70 ; void __cdecl player_find_player_character_unit_and_variant_info(long, long*, long*)
//.text:0053A010 ; bool __cdecl player_find_zone_set_switches(long, long*)
//.text:0053A1E0 ; bool __cdecl player_flashlight_on()
//.text:0053A290 ; void __cdecl player_force_spawn(long)
//.text:0053A330 ; void __cdecl player_forge_cleanup_on_death(long)
//.text:0053A3F0 ; player_get_stamina_deplete_restore_time
//.text:0053A450 ; player_get_vehicle_shield_regen_speed_modifier_value
//.text:0053A5A0 ; player_get_vehicle_shield_regen_delay_modifier_value
//.text:0053A6F0 ; player_get_representation_block
//.text:0053A770 ; enum e_player_character_type __cdecl player_get_character_type(long)
//.text:0053A7D0 ; player_get_scavenger_aura_modifier_value
//.text:0053A940 ; adrenaline related
//.text:0053A9A0 ; long __cdecl player_get_unit_index_even_if_dead(long)
//.text:0053A9E0 ; bool __cdecl player_handle_interaction_held(long, s_player_interaction const*)
//.text:0053AD10 ; bool __cdecl player_handle_interaction_press(long, s_player_interaction const*)
//.text:0053ADE0 ; bool __cdecl player_handle_interaction_held(long, s_player_interaction const*)
//.text:0053AED0 ; void __cdecl player_handle_weapon_added(long, short)
//.text:0053AEF0 ; void __cdecl player_handle_weapon_put_away(long, short)
//.text:0053AF10 ; revenge_shield_boost related
//.text:0053AFA0 ; void __cdecl player_health_pack_screen_effect(long)
//.text:0053B060 ; bool __cdecl player_identifier_compare(s_player_identifier const*, s_player_identifier const*)
//.text:0053B090 ; char const* __cdecl player_identifier_get_string(s_player_identifier const*)
//.text:0053B0E0 ; bool __cdecl player_identifier_is_valid(s_player_identifier const*)
//.text:0053B120 ; void __cdecl player_increment_control_context(long)
//.text:0053B1A0 ; long __cdecl player_index_from_absolute_player_index(long)

long __cdecl player_index_from_unit_index(long unit_index)
{
	return INVOKE(0x0053B1D0, player_index_from_unit_index, unit_index);
}

//.text:0053B200 ; public: __cdecl s_player_information::s_player_information()
//.text:0053B260 ; void __cdecl player_input_enable(bool)
//.text:0053B290 ; bool __cdecl player_input_enabled()
//.text:0053B2B0 ; void __cdecl player_input_mostly_inhibit(bool)
//.text:0053B2D0 ; void __cdecl player_interaction_clear(s_player_interaction*)

bool __cdecl player_interaction_exists(long player_index, dword object_mask, s_player_interaction const* interaction)
{
	return INVOKE(0x0053B2F0, player_interaction_exists, player_index, object_mask, interaction);
}

//.text:0053B340 ; bool __cdecl player_interaction_valid(s_player_interaction const*)
//.text:0053B370 ; bool __cdecl player_is_allowed_to_attempt_assassination(long, long)
//.text:0053B480 ; bool __cdecl player_is_immune_to_headshot(long, bool)

bool __cdecl player_is_local(long player_index)
{
	return INVOKE(0x0053B4E0, player_is_local, player_index);
}

bool __cdecl player_is_reading_terminal()
{
	return INVOKE(0x0053B570, player_is_reading_terminal);
}

//.text:0053B590 ; bool __cdecl player_is_sprinting(long, real*)
//.text:0053B670 ; void __cdecl player_leave_game_internal(long)
//.text:0053B7D0 ; void __cdecl player_left_game(long)
//.text:0053B7E0 ; first_player_set_armor
//.text:0053B840 ; void __cdecl player_mostly_input_inhibit(long, player_action*)

long __cdecl player_new(long player_array_index, game_player_options const* options, bool joined_in_progress)
{
	return INVOKE(0x0053B880, player_new, player_array_index, options, joined_in_progress);

	//TLS_DATA_GET_VALUE_REFERENCE(player_data);
	//TLS_DATA_GET_VALUE_REFERENCE(players_globals);
	//
	//long player_absolute_index = datum_new_at_absolute_index(*player_data, player_array_index);
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

//.text:0053B8F0 ; void __cdecl player_notify_of_tracking_or_locking(long, long, short)
//.text:0053BA10 ; void __cdecl player_notify_vehicle_ejection_finished(long)
//.text:0053BA90 ; void __cdecl player_over_shield_screen_effect(long)

void __cdecl player_positions_dispose()
{
	INVOKE(0x0053BB90, player_positions_dispose);
}

void __cdecl player_positions_dispose_from_old_map()
{
	INVOKE(0x0053BBA0, player_positions_dispose_from_old_map);
}

void __cdecl player_positions_dispose_from_old_structure_bsp(dword deactivating_structure_bsp_mask)
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

void __cdecl player_positions_initialize_for_new_structure_bsp(dword activating_structure_bsp_mask)
{
	INVOKE(0x0053BBE0, player_positions_initialize_for_new_structure_bsp, activating_structure_bsp_mask);
}

//.text:0053BD90 ; void __cdecl player_prepare_action(long, player_action*)
//.text:0053BFF0 ; void __cdecl player_reading_terminal_set(bool)
//.text:0053C020 ; void __cdecl player_rejoined_game(long,  game_player_options const*, bool)
//.text:0053C070 ; void __cdecl player_reset(long, bool, bool,  game_player_options const*)
//.text:0053C570 ; 
//.text:0053C630 ; void __cdecl player_set_configuration(long, s_player_configuration const*)
//.text:0053C860 ; 

void __cdecl player_set_facing(long player_index, real_vector3d const* facing)
{
	INVOKE(0x0053C8A0, player_set_facing, player_index, facing);

	//TLS_DATA_GET_VALUE_REFERENCE(player_data);
	//
	//player_datum* player = DATUM_TRY_AND_GET(player_data, player_datum, player_index);
	//if (game_is_authoritative())
	//{
	//	if (player->unit_index != NONE)
	//	{
	//		unit_datum* unit = UNIT_GET(player->unit_index);
	//		unit->unit.facing_vector = *facing;
	//		unit->unit.desired_aiming_vector = *facing;
	//		unit->unit.desired_looking_vector = *facing;
	//
	//		simulation_action_object_update(player->unit_index, _simulation_unit_update_desired_aiming_vector);
	//	}
	//
	//}
	//
	//long input_user = player_mapping_get_input_user(player_index);
	//if (input_user != NONE)
	//	player_control_set_facing(input_user, facing);
}

//.text:0053C980 ; 
//.text:0053CA10 ; 

void __cdecl player_set_unit_index(long player_index, long unit_index)
{
	INVOKE(0x0053CA80, player_set_unit_index, player_index, unit_index);
}

//.text:0053CDC0 ; bool __cdecl player_should_auto_pickup_weapon(long, long)

bool __cdecl player_spawn(long player_index, real_point3d const* position, real const* facing)
{
	return INVOKE(0x0053CE60, player_spawn, player_index, position, facing);
}

//.text:0053D500 ; void __cdecl player_sprint_inhibit(bool)
//.text:0053D520 ; void __cdecl player_submit_actions(long, long, player_action*)
//.text:0053EC70 ; bool __cdecl player_submit_assassination(long, long, long, real_point3d const*, real_vector3d const*, real_vector3d const*)
//.text:0053F010 ; void __cdecl player_submit_control(long, long, player_action const*)

//void __cdecl player_suppress_action(long, enum e_player_suppress_action_type_enum)
void __cdecl player_suppress_action(long player_index, long player_suppress_action_type)
{
	//INVOKE(0x0053F220, player_suppress_action, player_index, player_suppress_action_type);

	TLS_DATA_GET_VALUE_REFERENCE(player_data);
	player_datum* player = DATUM_TRY_AND_GET(player_data, player_datum, player_index);

	long input_user = player_mapping_get_input_user(player_index);
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

//.text:0053F300 ; void __cdecl player_suppress_control(long, long)
//.text:0053F390 ; void __cdecl player_swap(long, long)
//.text:0053F550 ; bool __cdecl player_teleport(long, long, real_point3d const*, bool)

bool __cdecl player_teleport(long player_index, long object_index, real_point3d const* position)
{
	return INVOKE(0x0053F550, player_teleport, player_index, object_index, position);
}

//.text:0053F630 ; bool __cdecl player_teleport_internal(long, long, real_point3d const*, bool, bool)
//.text:0053FB80 ; void __cdecl player_teleport_internal_postprocess(long, long, bool)
//.text:0053FCC0 ; void __cdecl player_teleport_on_bsp_switch(long, long, real_point3d const*, real_vector3d const*, bool)
//.text:0053FED0 ; short __cdecl player_terminals_accessed_bitvector_get()
//.text:0053FEF0 ; void __cdecl player_terminals_accessed_bitvector_set(short)
//.text:0053FF20 ; short __cdecl player_terminals_read_bitvector_get()
//.text:0053FF40 ; void __cdecl player_terminals_read_bitvector_set(short)
//.text:0053FF70 ; long __cdecl player_try_and_get_player_index_from_absolute_player_index(long)

bool __cdecl player_try_to_drop_weapon(long player_index, bool primary_weapon)
{
	return INVOKE(0x0053FFC0, player_try_to_drop_weapon, player_index, primary_weapon);
}

//.text:00540070 ; bool __cdecl player_try_to_put_away_weapon(long, bool)
//.text:00540150 ; bool __cdecl player_try_to_swap_weapons(long, bool, s_player_interaction const*)
//.text:00540320 ; void __cdecl player_unit_calculate_location_pesemistic(long, s_location*)

long __cdecl player_unit_get_representation_index(long unit_index)
{
	return INVOKE(0x00540350, player_unit_get_representation_index, unit_index);
}

// not the correct name
long __cdecl player_unit_get_control_index(long unit_index)
{
	return INVOKE(0x00540440, player_unit_get_control_index, unit_index);
}

//.text:00540490 ; bool __cdecl player_unit_should_teleport_to_unit(long, long, long)
//.text:00540510 ; 

void __cdecl player_update_invisibility(long player_index)
{
	INVOKE(0x005405A0, player_update_invisibility, player_index);
}

//.text:00540650 ; void __cdecl player_update_reactive_armor(long)
//.text:00540730 ; void __cdecl player_update_tank_mode(long)
//.text:005408E0 ; void __cdecl player_use_multiplayer_powerup(long, long)
//.text:00540A70 ; 
//.text:00540A80 ; void __cdecl player_validate_configuration(long, s_player_configuration*)
//.text:00540AE0 ; bool __cdecl player_waiting_to_respawn_compare(long, long, void const *)
//.text:00540B30 ; void __cdecl player_weapon_pickup_inhibit(bool)
//.text:00540B50 ; long __cdecl players_active_zone_set_switch_trigger_get(void)

bool __cdecl players_all_are_dead()
{
	return INVOKE(0x00540B80, players_all_are_dead);
}

bool __cdecl players_any_are_dead()
{
	return INVOKE(0x00540BA0, players_any_are_dead);
}

bool __cdecl players_any_are_in_the_air(long* out_unit_index)
{
	return INVOKE(0x00540BC0, players_any_are_in_the_air, out_unit_index);
}

bool __cdecl players_any_are_near_death(long* out_unit_index)
{
	return INVOKE(0x00540D10, players_any_are_near_death, out_unit_index);
}

//.text:00540DC0 ; bool __cdecl players_arm_unit_from_campaign_armaments_data(s_campaign_armaments_player const*, long)
//.text:00540F30 ; void __cdecl players_build_persistent_player_options(game_options*)
//.text:00541060 ; long __cdecl players_build_weapon_from_campaign_armaments_data(s_campaign_armaments_weapon const*, long, bool*)
//.text:00541250 ; void __cdecl players_calculate_safe_position(long, real_point3d*, real_vector3d*)
//.text:00541310 ; void __cdecl players_compute_combined_pvs(s_game_cluster_bit_vectors*, bool)
//.text:00541400 ; void __cdecl players_coop_desire_respawn(long)
//.text:00541480 ; void __cdecl players_coop_update_respawn(long)
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

void __cdecl players_dispose_from_old_structure_bsp(dword deactivating_structure_bsp_mask)
{
	INVOKE(0x00541B70, players_dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);
}

void __cdecl players_finish_creation()
{
	INVOKE(0x00541B80, players_finish_creation);
}

//.text:00541BA0 ; void __cdecl players_force_detach_from_object(long)

long __cdecl players_get_active_and_in_game_count(bool include_joined_in_progress_players)
{
	return INVOKE(0x00541C70, players_get_active_and_in_game_count, include_joined_in_progress_players);
}

//.text:00541CF0 ; long __cdecl players_get_alive_count()
//.text:00541D60 ; s_campaign_armaments_player const* __cdecl players_get_campaign_armaments_player_from_player_index(long)
//.text:00541DF0 ; dword const* __cdecl players_get_combined_pvs()
//.text:00541E10 ; dword const* __cdecl players_get_combined_pvs_local()
//.text:00541E30 ; bool __cdecl players_get_local_machine(s_machine_identifier*)
//.text:00541E90 ; long __cdecl players_get_local_machine_index()
//.text:00541EB0 ; s_machine_identifier const* __cdecl players_get_machine_identifier(long)
//.text:00541EE0 ; long __cdecl players_get_machine_index(s_machine_identifier const*)
//.text:00541F60 ; void __cdecl players_get_machines(dword*, s_machine_identifier*)
//.text:00541FA0 ; short __cdecl players_get_respawn_failure()
//.text:00541FC0 ; void __cdecl players_get_sorted_lifeless_waiting_to_respawn_list(long, long*, long, long*)
//.text:00542070 ; long __cdecl players_get_total_players_in_game()
//.text:00542090 ; bool __cdecl players_globals_exists()
//.text:005420B0 ; void __cdecl players_handle_deleted_object(long)

void __cdecl players_handle_deleted_player_internal(long player_index)
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

void __cdecl players_initialize_for_new_structure_bsp(dword activating_structure_bsp_mask)
{
	INVOKE(0x00542460, players_initialize_for_new_structure_bsp, activating_structure_bsp_mask);
}

//.text:00542470 ; void __cdecl players_initialize_for_saved_game(long)
//.text:00542520 ; void __cdecl players_joined_in_progress_allow_spawn()
//.text:005425A0 ; bool __cdecl players_joined_in_progress_and_waiting_to_respawn()

void __cdecl players_rebuild_user_mapping(bool force_rebuild)
{
	INVOKE(0x00542620, players_rebuild_user_mapping, force_rebuild);
}

//.text:005427A0 ; void __cdecl players_remap_local_machine(s_machine_identifier const*,  game_player_options const*)

void __cdecl players_set_local_machine(s_machine_identifier const* machine_identifier)
{
	INVOKE(0x00542D60, players_set_local_machine, machine_identifier);
}

void __cdecl players_set_machines(dword new_machine_valid_mask, s_machine_identifier const* new_machine_identifiers)
{
	INVOKE(0x00542F80, players_set_machines, new_machine_valid_mask, new_machine_identifiers);

	//TLS_DATA_GET_VALUE_REFERENCE(player_data);
	//TLS_DATA_GET_VALUE_REFERENCE(players_globals);
	//
	//ASSERT(VALID_BITS(new_machine_valid_mask, k_maximum_machines));
	//ASSERT(new_machine_identifiers);
	//
	//for (long machine_index = 0; machine_index < k_maximum_machines; machine_index++)
	//{
	//	if (!TEST_BIT(new_machine_valid_mask, machine_index))
	//		continue;
	//
	//	for (long other_machine_index = 0; other_machine_index < k_maximum_machines; other_machine_index++)
	//	{
	//		if (other_machine_index != machine_index && TEST_BIT(new_machine_valid_mask, other_machine_index))
	//			ASSERT(csmemcmp(&new_machine_identifiers[machine_index], &new_machine_identifiers[other_machine_index], sizeof(s_machine_identifier)) != 0);
	//	}
	//}
	//
	//dword old_machine_valid_mask = players_globals->machine_valid_mask;
	//
	//s_machine_identifier old_machine_identifiers[k_maximum_machines];
	//long new_machine_indices[k_maximum_machines];
	//csmemcpy(old_machine_identifiers, players_globals->machine_identifiers.begin(), sizeof(s_machine_identifier) * k_maximum_machines);
	//csmemset(new_machine_indices, NONE, sizeof(long) * k_maximum_machines);
	//
	//for (long machine_index = 0; machine_index < k_maximum_machines; machine_index++)
	//{
	//	if (!TEST_BIT(old_machine_valid_mask, machine_index))
	//		continue;
	//
	//	for (long other_machine_index = 0; other_machine_index < k_maximum_machines; other_machine_index++)
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
	//		long new_machine_index = new_machine_indices[player->machine_index];
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
	//	long new_machine_index = new_machine_indices[player->machine_index];
	//	if (player->machine_index != NONE)
	//		player->machine_index = new_machine_index;
	//}
	//
	//if (players_globals->local_machine_exists)
	//{
	//	long new_machine_index = NONE;
	//	for (long machine_index = 0; machine_index < k_maximum_machines; machine_index++)
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
//.text:005431E0 ; void __cdecl players_store_campaign_armamements_weapon(long, s_campaign_armaments_weapon*)
//.text:005432D0 ; void __cdecl players_store_campaign_armaments_on_game_won(s_campaign_armaments*)
//.text:00543560 ; void __cdecl players_update_activation()

void verify_coop_respawn_effect()
{
	TLS_DATA_GET_VALUE_REFERENCE(player_data);
	c_data_iterator<player_datum> player_iterator;
	player_iterator.begin(player_data);
	while (player_iterator.next())
	{
		player_datum* player = player_iterator.get_datum();
		if (!TEST_BIT(player->flags, _player_play_coop_spawn_effect_bit))
			continue;

		long control_index = player_unit_get_control_index(player->unit_index);
		if (VALID_INDEX(control_index, global_game_globals->player_information.count))
			continue;

		SET_BIT(player->flags, _player_play_coop_spawn_effect_bit, false);
	}
}

void __cdecl players_update_after_game(struct simulation_update const* update)
{
	verify_coop_respawn_effect();
	
	INVOKE(0x00543650, players_update_after_game, update);
}

void __cdecl players_update_before_game(struct simulation_update const* update)
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

void s_emblem_info::decode(c_bitstream* packet)
{
	foreground_emblem_index = (char)packet->read_integer("foreground-emblem", 6);
	background_emblem_index = (char)packet->read_integer("background-emblem", 6);
	emblem_info_flags.set_unsafe((byte)packet->read_integer("emblem-flags", 3));
	primary_color_index.set_raw_value((char)packet->read_integer("emblem-primary-color", 6));
	secondary_color_index.set_raw_value((char)packet->read_integer("emblem-secondary-color", 6));
	background_color_index.set_raw_value((char)packet->read_integer("emblem-background-color", 6));
}

void s_emblem_info::encode(c_bitstream* packet)
{
	packet->write_integer("foreground-emblem", foreground_emblem_index, 6);
	packet->write_integer("background-emblem", background_emblem_index, 6);
	packet->write_integer("emblem-flags", emblem_info_flags.get_unsafe(), 3);
	packet->write_integer("emblem-primary-color", primary_color_index, 6);
	packet->write_integer("emblem-secondary-color", secondary_color_index, 6);
	packet->write_integer("emblem-background-color", background_color_index, 6);
}

s_s3d_player_armor_configuration_loadout* __cdecl player_get_armor_loadout(player_datum* player)
{
	s_s3d_player_armor_configuration_loadout& loadout = player->configuration.host.armor.loadouts[player->armor_loadout_index];

	return &loadout;
}

// this function gets called in `player_spawn` -> `game_engine_add_starting_equipment`
// $TODO: hook `game_engine_add_starting_equipment` and reimplement the original functionality
s_s3d_player_weapon_configuration_loadout* __cdecl player_get_weapon_loadout(player_datum* player)
{
	s_s3d_player_weapon_configuration_loadout& loadout = player->configuration.host.weapon.loadouts[player->weapon_loadout_index];

	// allow player traits override
	// $TODO: pull these from tags
	loadout.primary_weapon_index = 0xFF;
	loadout.secondary_weapon_index = 0xFF;

	return &loadout;
}

// find a better name?
bool customized_area_selection_from_name(s_multiplayer_customized_model_selection& selection, char const* selection_name)
{
	char const* selection_name_str = selection.selection_name.get_string();

	if (selection.third_person_armor_object.index != NONE /*&& selection.first_person_armor_object.index != NONE*/)
	{
		if (selection_name_str && csstricmp(selection_name, selection_name_str) == 0)
			return true;
	}

	return false;
}

// find a better name?
long customized_spartan_character_from_name(s_multiplayer_customized_model_character& character, char const* region_or_biped_name, char const* selection_name)
{
	char const* armor_region = character.armor_region.get_string();
	char const* biped_region = character.biped_region.get_string();

	if (armor_region && csstricmp(region_or_biped_name, armor_region) == 0)
	{
		for (long selection_absolute_index = 0; selection_absolute_index < character.customized_selection.count; selection_absolute_index++)
		{
			s_multiplayer_customized_model_selection& customized_model_selection = character.customized_selection[selection_absolute_index];
			if (customized_area_selection_from_name(customized_model_selection, selection_name))
				return selection_absolute_index;
		}
	}

	if (biped_region && csstricmp(region_or_biped_name, biped_region) == 0)
	{
		for (long selection_absolute_index = 0; selection_absolute_index < character.customized_selection.count; selection_absolute_index++)
		{
			s_multiplayer_customized_model_selection& customized_model_selection = character.customized_selection[selection_absolute_index];
			if (customized_area_selection_from_name(customized_model_selection, selection_name))
				return selection_absolute_index;
		}
	}

	return 0;
}

long multiplayer_universal_data_get_absolute_equipment_block_index(char const* name)
{
	s_multiplayer_universal_globals_definition* universal_data = scenario_multiplayer_globals_try_and_get_universal_data();
	if (!universal_data)
		return 0;

	long result = 0;
	for (long equipment_absolute_index = 0; equipment_absolute_index < universal_data->equipment.count; equipment_absolute_index++)
	{
		s_multiplayer_equipment& equipment = universal_data->equipment[equipment_absolute_index];
		char const* equipment_name = equipment.name.get_string();
		if (equipment_name && csstricmp(name, equipment_name) == 0)
		{
			result = equipment_absolute_index;
			break;
		}
	}

	return result;
}

// find a better name?
long multiplayer_universal_data_get_absolute_customized_spartan_character_block_index(char const* region_or_biped_name, char const* selection_name)
{
	if (!g_cache_file_globals.tags_loaded)
		return 0;

	s_multiplayer_universal_globals_definition* universal_data = scenario_multiplayer_globals_try_and_get_universal_data();
	if (!universal_data)
		return 0;

	long result = 0;
	for (s_multiplayer_customized_model_character& customized_model_character : universal_data->customized_spartan_characters)
	{
		result = customized_spartan_character_from_name(customized_model_character, region_or_biped_name, selection_name);
		if (result != 0)
			break;
	}

	return result;
}

// find a better name?
short multiplayer_universal_data_get_absolute_weapons_selection_block_index(char const* selection_name)
{
	s_multiplayer_universal_globals_definition* universal_data = scenario_multiplayer_globals_try_and_get_universal_data();
	if (!universal_data)
		return short(0xFFFD);

	if (universal_data->weapon_selections.count)
	{
		for (short weapon_selection_index = 0; weapon_selection_index < universal_data->weapon_selections.count; weapon_selection_index++)
		{
			s_multiplayer_weapon_selection& weapon_selection = universal_data->weapon_selections[weapon_selection_index];

			char const* name = weapon_selection.name.get_string();
			if (name && csstricmp(selection_name, name) == 0)
				return weapon_selection_index;
		}
	}

	return short(0xFFFD);
}

bool scenario_is_solo()
{
	struct scenario* scenario = global_scenario_get();
	return scenario && scenario->type == _scenario_type_solo;
}

long get_spartan_representation_index()
{
	static string_id sp_name = string_id_retrieve("spartan");
	static string_id mp_name = string_id_retrieve("mp_spartan");

	string_id name = scenario_is_solo() ? sp_name : mp_name;

	if (s_game_globals* game_globals = scenario_get_game_globals())
	{
		for (long i = 0; i < game_globals->player_representation.count; i++)
		{
			if (name == game_globals->player_representation[i].name.get_value())
				return i;
		}
	}

	return 2;
}

long get_elite_representation_index()
{
	static string_id sp_name = string_id_retrieve("sp_elite");
	static string_id mp_name = string_id_retrieve("mp_elite");

	string_id name = scenario_is_solo() ? sp_name : mp_name;

	if (s_game_globals* game_globals = scenario_get_game_globals())
	{
		for (long i = 0; i < game_globals->player_representation.count; i++)
		{
			if (name == game_globals->player_representation[i].name.get_value())
				return i;
		}
	}

	return 3;
}

void apply_player_representation_fixup()
{
	static t_value_type<long> spartan_representation_index = { .value = 2 };
	static t_value_type<long> elite_representation_index = { .value = 3 };

	static dword const spartan_representation_addresses[] = { 0x00537482 + 1, 0x0053761A + 1, 0x00539EB9 + 1, 0x00539FBD + 1, 0x0053A738 + 1, 0x0053A7C6 + 1 };
	static dword const elite_representation_addresses[] = { 0x0053748F + 1, 0x00537627 + 1, 0x00539EC6 + 1, 0x00539FCA + 1, 0x0053A745 + 1, 0x0053A7BE + 1 };

	DATA_PATCH_ARRAY_DECLARE2(spartan_representation_addresses, spartan_representation_index, spartan_representation_index.bytes);
	DATA_PATCH_ARRAY_DECLARE2(elite_representation_addresses, elite_representation_index, elite_representation_index.bytes);

	spartan_representation_index.value = get_spartan_representation_index();
	elite_representation_index.value = get_elite_representation_index();

	spartan_representation_index_patch.apply(false);
	elite_representation_index_patch.apply(false);
}

