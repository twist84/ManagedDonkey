#include "ai/ai_debug.hpp"

#include "ai/actor_stimulus.hpp"
#include "ai/actors.hpp"
#include "cache/cache_files.hpp"
#include "camera/observer.hpp"
#include "game/player_mapping.hpp"
#include "interface/interface_constants.hpp"
#include "memory/thread_local.hpp"
#include "objects/objects.hpp"
#include "render/render_debug.hpp"
#include "units/dialogue_definitions.hpp"
#include "units/units.hpp"

real_point3d global_ai_debug_drawstack_next_position;
real global_ai_debug_drawstack_height;
real_point3d global_ai_debug_drawstack_last_position;

short global_ai_debug_string_position = 0;

//path_debug_storage* actor_path_debug_array;
//actor_debug_info* actor_debug_array;
//ai_debug_state ai_debug;

bool ai_render = false;
bool ai_render_firing_positions_all = false;
bool ai_render_lineoffire = false;
bool ai_render_lineofsight = false;
bool ai_render_ballistic_lineoffire = false;
long ai_debug_selected_actor_unit_index = NONE;
long ai_debug_selected_actor_index = NONE;
bool ai_debug_path = false;
bool ai_render_paths_failed = false;
bool ai_render_aiming_validity = false;
bool ai_render_all_actors = false;
bool ai_render_dialogue = false;
bool ai_render_dialogue_queue = false;
bool ai_render_dialogue_player_weights = false;
bool ai_render_speech = false;
bool ai_print_speech = false;
bool ai_render_spatial_effects = false;
bool ai_render_clumps = false;
bool ai_render_clump_props = false;
bool ai_render_clump_props_all = false;
bool ai_render_decisions = false;
bool ai_render_decisions_all = false;
bool ai_render_behavior_stack_all = false;
bool ai_render_stimuli = false;
bool ai_render_sectors = false;
bool ai_render_sector_geometry_errors = false;
long ai_render_link_specific = 0;
bool ai_render_links = false;
bool ai_render_user_hints = false;
bool ai_render_hints = false;
bool ai_render_object_hints_all = false;
bool ai_render_object_hints = false;
bool ai_render_object_properties = false;
bool ai_render_sector_bsps = false;
bool ai_render_giant_sector_bsps = false;
bool ai_render_sector_link_errors = false;
bool ai_render_intersection_links = false;
bool ai_render_non_walkable_sectors = false;
bool ai_render_threshold_links = false;
bool ai_render_orders = false;
bool ai_render_suppress_combat = false;
bool ai_render_objectives = false;
bool ai_render_strength = false;
bool ai_debug_tracking_data = false;
bool ai_debug_perception_data = false;
bool ai_debug_combat_status = false;
bool ai_render_tracked_props_all = false;
bool ai_render_targets_all = false;
bool ai_render_joint_behaviors = false;
bool ai_render_flocks = false;
bool ai_render_command_scripts = false;
bool ai_render_dialogue_variants = false;
bool ai_render_vehicle_interest = false;
bool ai_render_player_battle_vector = false;
bool ai_render_player_needs_vehicle = false;
bool ai_render_mission_critical = false;

void __cdecl ai_debug_render()
{
	TLS_DATA_GET_VALUE_REFERENCE(ai_globals);
	TLS_DATA_GET_VALUE_REFERENCE(actor_data);
	
	bool flag = ai_render
		&& (ai_render_firing_positions_all
		|| ai_render_lineoffire
		|| ai_render_lineofsight
		|| ai_render_ballistic_lineoffire
		|| ai_debug_selected_actor_unit_index != NONE && (ai_debug_selected_actor_index == NONE || ai_render_all_actors)
		|| ai_debug_selected_actor_index != NONE && !ai_render_all_actors
		|| ai_debug_path
		|| ai_render_paths_failed
		|| ai_render_aiming_validity
		|| ai_render_all_actors
		|| ai_render_dialogue
		|| ai_render_dialogue_queue
		|| ai_render_dialogue_player_weights
		|| ai_render_speech
		|| ai_print_speech
		|| ai_render_spatial_effects
		|| ai_render_clumps
		|| ai_render_clump_props
		|| ai_render_clump_props_all
		|| ai_render_decisions
		|| ai_render_decisions_all
		|| ai_render_behavior_stack_all
		|| ai_render_stimuli
		|| ai_render_sectors
		|| ai_render_sector_geometry_errors
		|| ai_render_link_specific != NONE
		|| ai_render_links
		|| ai_render_user_hints
		|| ai_render_hints
		|| ai_render_object_hints_all
		|| ai_render_object_hints
		|| ai_render_object_properties
		|| ai_render_sector_bsps
		|| ai_render_giant_sector_bsps
		|| ai_render_sector_link_errors
		|| ai_render_intersection_links
		|| ai_render_non_walkable_sectors
		|| ai_render_threshold_links
		|| ai_render_orders
		|| ai_render_suppress_combat
		|| ai_render_objectives
		|| ai_render_strength
		|| ai_debug_tracking_data
		|| ai_debug_perception_data
		|| ai_debug_combat_status
		|| ai_render_tracked_props_all
		|| ai_render_targets_all
		|| ai_render_joint_behaviors
		|| ai_render_flocks
		|| ai_render_command_scripts
		|| ai_render_dialogue_variants
		|| ai_render_vehicle_interest
		|| ai_render_player_battle_vector);
	
	//main_set_single_thread_request_flag(2, g_dialogue_debug_enabled | flag);
	if (!actor_datum_available_to_current_thread() || !ai_globals->__unknown1)
		return;
	
	short_rectangle2d bounds{};
	interface_get_current_display_settings(NULL, NULL, NULL, &bounds);
	global_ai_debug_string_position = bounds.y1 - 20;
	//dword_143C86DD8 = (dword_143C86DD8 + 1) % 1000;
	if (ai_debug_selected_actor_index != NONE)
	{
		actor_datum* actor = (actor_datum*)datum_get(*actor_data, ai_debug_selected_actor_index);
		ai_debug_selected_actor_unit_index = actor->meta.unit_index;
	}
	
	//if (ai_debug)
	//	ai_debug_select_this_actor();
	
	if (ai_render)
	{
		//if (ai_render_firing_positions_all)
		//	ai_debug_render_all_firing_positions();

		//if (ai_render_lineoffire)
		//	ai_debug_render_lineoffire();

		//if (ai_render_lineofsight)
		//	ai_debug_render_lineofsight();

		//if (ai_render_ballistic_lineoffire)
		//	ai_debug_render_ballistic_lineoffire();

		//if (ai_debug_selected_actor_unit_index != NONE && (ai_debug_selected_actor_index == NONE || ai_render_all_actors))
		//	ai_debug_render_squad_members(ai_debug_selected_actor_unit_index);

		//if (ai_debug_selected_actor_index != NONE && !ai_render_all_actors)
		//	ai_debug_render_actor(ai_debug_selected_actor_index, 1u, 0i64);

		//if (ai_debug_path)
		//	ai_debug_render_path();

		//if (ai_render_paths_failed)
		//	ai_debug_render_paths_failed();

		//if (ai_render_all_actors)
		//	ai_debug_render_all_actors(ai_render_inactive_actors);

		//if (ai_render_dialogue)
		//	ai_debug_dialogue();

		//if (ai_render_dialogue_queue)
		//	ai_dialogue_render_queue();

		//if (ai_render_dialogue_player_weights)
		//	ai_dialogue_render_player_weights();

		//if (ai_render_speech_enabled || ai_print_speech)
		//	ai_debug_render_speech();

		//if (ai_render_spatial_effects)
		//	ai_debug_render_spatial_effects();

		//if (ai_render_clumps_enabled || ai_render_clump_props_enabled || ai_render_clump_dialogue || ai_render_clump_props_all)
		//	ai_debug_render_clumps();

		//if (ai_render_decisions)
		//	ai_debug_render_decisions(0);

		//if (ai_render_decisions_all)
		//	ai_debug_render_decisions(1);

		//if (ai_render_behavior_stack_all)
		//	ai_debug_render_behavior_stacks_all();

		//if (ai_render_character_names)
		//	ai_debug_render_character_names();
	
		if (ai_render_stimuli)
			stimuli_debug();
	
		//if (ai_render_sectors)
		//	ai_debug_render_sectors();

		//if (ai_render_sector_geometry_errors)
		//	ai_render_sector_geometry_errors();

		//if (ai_render_link_specific != NONE)
		//	ai_debug_render_link_specific();

		//if (ai_render_links)
		//	ai_debug_render_links();

		//if (ai_render_user_hints)
		//	ai_render_user_hints();

		//if (ai_render_hints)
		//	ai_render_hints();

		//if (ai_render_object_hints_all)
		//	ai_render_object_hints(0);
		//else if (ai_render_object_hints)
		//	ai_render_object_hints(1);

		//if (ai_render_object_properties)
		//	ai_render_object_properties();

		//if (ai_render_sector_bsps)
		//	sector_bsps_debug();
		//else if (ai_render_giant_sector_bsps)
		//	giant_sector_bsps_debug();

		//if (ai_render_sector_link_errors)
		//	ai_debug_render_sector_link_errors();

		//if (ai_render_intersection_links)
		//	ai_debug_render_intersection_links();

		//if (ai_render_non_walkable_sectors)
		//	ai_debug_render_non_walkable_sectors();

		//if (ai_render_threshold_links)
		//	ai_debug_render_threshold_links();

		//if (ai_render_objectives_enabled || ai_render_strength)
		//	ai_debug_render_squads();

		//if (ai_render_suppress_combat)
		//	ai_debug_render_suppress_combat();

		//if (ai_render_ai_iterator != NONE)
		//	ai_debug_render_ai_iterator();

		//if (ai_render_vehicle_reservations)
		//	ai_debug_render_vehicle_reservations();

		//if (ai_debug_tracking_data)
		//	ai_debug_tracking_data();

		//if (ai_debug_perception_data)
		//	ai_debug_perception_data();

		if (ai_debug_combat_status)
			debug_combat_status();

		//if (ai_render_tracked_props_all)
		//	ai_debug_render_tracked_props_all();

		//if (ai_render_targets_all)
		//	ai_debug_render_targets_all();

		//if (ai_render_joint_behaviors)
		//	debug_render_joint_behaviors();

		//if (ai_render_flocks)
		//	debug_render_flocks();

		//if (ai_render_command_scripts)
		//	render_command_scripts();
	
		if (ai_render_dialogue_variants)
			render_dialogue_variants();
	
		//if (ai_render_dynamic_firing_positions)
		//	ai_debug_render_dynamic_firing_positions();

		//if (!ai_hide_actor_errors)
		//	debug_render_actor_errors();

		//if (ai_render_vehicle_interest)
		//	debug_render_vehicle_interest();

		//if (ai_render_player_battle_vector)
		//	debug_render_player_battle_vector();

		//if (ai_render_player_needs_vehicle)
		//	debug_render_player_needs_vehicle();

		//if (ai_render_mission_critical)
		//	debug_render_mission_critical_ai();
	}
	
	//if (g_dialogue_debug)
	//	ai_dialogue_render_records();
}

void ai_debug_drawstack_setup(real_point3d const* position)
{
	e_output_user_index user_index = player_mapping_first_active_output_user();
	s_observer_result const* camera = observer_try_and_get_camera(user_index);

	global_ai_debug_drawstack_last_position = *position;
	global_ai_debug_drawstack_next_position = global_ai_debug_drawstack_last_position;

	if (camera)
	{
		vector3d vector{};
		vector_from_points3d(&global_ai_debug_drawstack_last_position, &camera->focus_point, &vector);
		global_ai_debug_drawstack_height = magnitude3d(&vector) / 40.0f;
	}
	else
	{
		global_ai_debug_drawstack_height = 0.05f;
	}
}

real_point3d* ai_debug_drawstack()
{
	global_ai_debug_drawstack_last_position = global_ai_debug_drawstack_next_position;
	point_from_line3d(&global_ai_debug_drawstack_last_position, global_up3d, global_ai_debug_drawstack_height, &global_ai_debug_drawstack_next_position);

	return &global_ai_debug_drawstack_last_position;
}

real_point3d* ai_debug_drawstack_offset(real offset)
{
	global_ai_debug_drawstack_last_position = global_ai_debug_drawstack_next_position;
	global_ai_debug_drawstack_next_position.z += offset;

	return &global_ai_debug_drawstack_last_position;
}

// #TODO: `actor_debug_info`, `actor_debug_array`, `actor_debug_drawstack`

void debug_combat_status()
{
	//actor_iterator iterator{};
	//actor_iterator_new(&iterator, true);
	//while (actor_datum* actor = actor_iterator_next(&iterator))
	//{
	//	// combat status
	//	{
	//		c_string_builder string("Combat status: %i", actor->state.combat_status);
	//		render_debug_string_at_point(actor_debug_drawstack(iterator.actor_index), string.get_string(), global_real_argb_green);
	//	}
	//
	//	// highest prop class
	//	{
	//		c_string_builder string("Highest prop class: %i", actor->situation.highest_prop_class);
	//		render_debug_string_at_point(actor_debug_drawstack(iterator.actor_index), string.get_string(), global_real_argb_green);
	//	}
	//}
}

void render_dialogue_variants()
{
	actor_iterator iterator{};
	actor_iterator_new(&iterator, false);
	while (actor_datum* actor = actor_iterator_next(&iterator))
	{
		if (actor->meta.unit_index != NONE)
		{
			unit_datum* unit = (unit_datum*)object_get_and_verify_type(actor->meta.unit_index, _object_mask_unit);
			s_seat_storage* seat_storage = (s_seat_storage*)object_header_block_get(actor->meta.unit_index, &unit->seat_storage);
			if (seat_storage->dialogue_definition_index != NONE)
			{
				s_dialogue_definition* dialogue_definition = (s_dialogue_definition*)tag_get(DIALOGUE_TAG, seat_storage->dialogue_definition_index);
				char const* mission_dialogue_designator_name = dialogue_definition->mission_dialogue_designator.get_string();
				render_debug_string_at_point(&actor->input.position.head, mission_dialogue_designator_name, global_real_argb_white);
			}
		}
	}
}

