#include "ai/ai_debug.hpp"

#include "ai/actor_perception.hpp"
#include "ai/actor_stimulus.hpp"
#include "ai/actors.hpp"
#include "ai/ai_hint_definitions.hpp"
#include "ai/ai_reference_frame.hpp"
#include "ai/ai_vehicles.hpp"
#include "ai/behavior.hpp"
#include "ai/sector.hpp"
#include "ai/sector_definitions.hpp"
#include "cache/cache_files.hpp"
#include "camera/observer.hpp"
#include "game/player_mapping.hpp"
#include "interface/interface.hpp"
#include "interface/interface_constants.hpp"
#include "main/console.hpp"
#include "memory/thread_local.hpp"
#include "objects/objects.hpp"
#include "render/render_debug.hpp"
#include "text/draw_string.hpp"
#include "units/dialogue_definitions.hpp"
#include "units/units.hpp"
#include "units/vehicles.hpp"

#include <climits>

real_point3d global_ai_debug_drawstack_next_position;
real32 global_ai_debug_drawstack_height;
real_point3d global_ai_debug_drawstack_last_position;

int16 global_ai_debug_string_position = 0;

//path_debug_storage* actor_path_debug_array;
//actor_debug_info* actor_debug_array;
//ai_debug_state ai_debug;

bool g_ai_render = true;
bool g_ai_render_firing_positions_all = false;
bool g_ai_render_lineoffire = false;
bool g_ai_render_lineofsight = false;
bool g_ai_render_ballistic_lineoffire = false;
int32 g_ai_debug_selected_actor_unit_index = NONE;
int32 g_ai_debug_selected_actor_index = NONE;
bool g_ai_debug_path = false;
bool g_ai_render_paths_failed = false;
bool g_ai_render_aiming_validity = false;
bool g_ai_render_all_actors = false;
bool g_ai_render_inactive_actors = false;
bool g_ai_render_dialogue = false;
bool g_ai_render_dialogue_queue = false;
bool g_ai_render_dialogue_player_weights = false;
bool g_ai_render_speech = false;
bool g_ai_print_speech = false;
bool g_ai_render_spatial_effects = false;
bool g_ai_render_clumps = false;
bool g_ai_render_clump_props = false;
bool g_ai_render_clump_props_all = false;
bool g_ai_render_clump_dialogue = false;
bool g_ai_render_decisions = false;
bool g_ai_render_decisions_all = false;
bool g_ai_render_behavior_stack_all = false;
bool g_ai_render_character_names = false;
bool g_ai_render_stimuli = false;
bool g_ai_render_sectors = false;
int32 g_ai_render_sectors_range_max = NONE;
int32 g_ai_render_sectors_range_min = NONE;
bool g_ai_render_sector_geometry_errors = false;
int32 g_ai_render_link_specific = 0;
bool g_ai_render_links = false;
bool g_ai_render_user_hints = false;
bool g_ai_render_hints = false;
bool g_ai_render_hints_detailed = false;
bool g_ai_render_object_hints_all = false;
bool g_ai_render_object_hints = false;
bool g_ai_render_object_properties = false;
bool g_ai_render_sector_bsps = false;
bool g_ai_render_giant_sector_bsps = false;
bool g_ai_render_sector_link_errors = false;
bool g_ai_render_intersection_links = false;
bool g_ai_render_non_walkable_sectors = false;
bool g_ai_render_threshold_links = false;
bool g_ai_render_orders = false;
bool g_ai_render_suppress_combat = false;
bool g_ai_render_vehicle_reservations = false;
bool g_ai_render_objectives = false;
bool g_ai_render_strength = false;
bool g_ai_debug_tracking_data = false;
bool g_ai_debug_perception_data = false;
bool g_ai_debug_combat_status = false;
bool g_ai_render_tracked_props_all = false;
bool g_ai_render_targets_all = false;
bool g_ai_render_joint_behaviors = false;
bool g_ai_render_flocks = false;
bool g_ai_render_command_scripts = false;
bool g_ai_render_dialogue_variants = false;
bool g_ai_render_dynamic_firing_positions = false;
bool g_ai_render_vehicle_interest = false;
bool g_ai_render_player_battle_vector = false;
bool g_ai_render_player_needs_vehicle = false;
bool g_ai_render_mission_critical = false;

void __cdecl ai_debug_render()
{
	bool ai_debug_requested = g_ai_render
		&& (g_ai_render_firing_positions_all
		|| g_ai_render_lineoffire
		|| g_ai_render_lineofsight
		|| g_ai_render_ballistic_lineoffire
		|| g_ai_debug_selected_actor_unit_index != NONE && (g_ai_debug_selected_actor_index == NONE || g_ai_render_all_actors)
		|| g_ai_debug_selected_actor_index != NONE && !g_ai_render_all_actors
		|| g_ai_debug_path
		|| g_ai_render_paths_failed
		|| g_ai_render_aiming_validity
		|| g_ai_render_all_actors
		|| g_ai_render_dialogue
		|| g_ai_render_dialogue_queue
		|| g_ai_render_dialogue_player_weights
		|| g_ai_render_speech
		|| g_ai_print_speech
		|| g_ai_render_spatial_effects
		|| g_ai_render_clumps
		|| g_ai_render_clump_props
		|| g_ai_render_clump_props_all
		|| g_ai_render_decisions
		|| g_ai_render_decisions_all
		|| g_ai_render_behavior_stack_all
		|| g_ai_render_stimuli
		|| g_ai_render_sectors
		|| g_ai_render_sector_geometry_errors
		|| g_ai_render_link_specific != NONE
		|| g_ai_render_links
		|| g_ai_render_user_hints
		|| g_ai_render_hints
		|| g_ai_render_object_hints_all
		|| g_ai_render_object_hints
		|| g_ai_render_object_properties
		|| g_ai_render_sector_bsps
		|| g_ai_render_giant_sector_bsps
		|| g_ai_render_sector_link_errors
		|| g_ai_render_intersection_links
		|| g_ai_render_non_walkable_sectors
		|| g_ai_render_threshold_links
		|| g_ai_render_orders
		|| g_ai_render_suppress_combat
		|| g_ai_render_objectives
		|| g_ai_render_strength
		|| g_ai_debug_tracking_data
		|| g_ai_debug_perception_data
		|| g_ai_debug_combat_status
		|| g_ai_render_tracked_props_all
		|| g_ai_render_targets_all
		|| g_ai_render_joint_behaviors
		|| g_ai_render_flocks
		|| g_ai_render_command_scripts
		|| g_ai_render_dialogue_variants
		|| g_ai_render_vehicle_interest
		|| g_ai_render_player_battle_vector);
	
	//main_set_single_thread_request_flag(_single_thread_for_actor_debug, g_dialogue_debug_enabled | ai_debug_requested);
	if (actor_datum_available_to_current_thread() && ai_globals->ai_initialized_for_map)
	{
		rectangle2d bounds{};
		interface_get_current_display_settings(nullptr, nullptr, nullptr, &bounds);
		global_ai_debug_string_position = bounds.y1 - 20;
		//dword_143C86DD8 = (dword_143C86DD8 + 1) % 1000;
		if (g_ai_debug_selected_actor_index != NONE)
		{
			actor_datum* actor = DATUM_GET(actor_data, actor_datum, g_ai_debug_selected_actor_index);
			g_ai_debug_selected_actor_unit_index = actor->meta.unit_index;
		}

#if 0
		if (g_ai_debug)
		{
			ai_debug_select_this_actor();
		}
#endif

		if (g_ai_render)
		{
#if 0
			if (g_ai_render_firing_positions_all)
			{
				ai_debug_render_all_firing_positions();
			}

			if (g_ai_render_lineoffire)
			{
				ai_debug_render_lineoffire();
			}

			if (g_ai_render_lineofsight)
			{
				ai_debug_render_lineofsight();
			}

			if (g_ai_render_ballistic_lineoffire)
			{
				ai_debug_render_ballistic_lineoffire();
			}

			if (g_ai_debug_selected_actor_unit_index != NONE && (g_ai_debug_selected_actor_index == NONE || g_ai_render_all_actors))
			{
				ai_debug_render_squad_members(g_ai_debug_selected_actor_unit_index);
			}
#endif

			if (g_ai_debug_selected_actor_index != NONE && !g_ai_render_all_actors)
			{
				ai_debug_render_actor(g_ai_debug_selected_actor_index, true, nullptr);
			}

#if 0
			if (g_ai_debug_path)
			{
				ai_debug_render_path();
			}

			if (g_ai_render_paths_failed)
			{
				ai_debug_render_paths_failed();
			}
#endif

			if (g_ai_render_all_actors)
			{
				ai_debug_render_all_actors(g_ai_render_inactive_actors);
			}

#if 0
			if (g_ai_render_dialogue)
			{
				ai_debug_dialogue();
			}

			if (g_ai_render_dialogue_queue)
			{
				ai_dialogue_render_queue();
			}
#endif

			if (g_ai_render_dialogue_player_weights)
			{
				ai_dialogue_render_player_weights();
			}

#if 0
			if (g_ai_render_speech_enabled || g_ai_print_speech)
			{
				ai_debug_render_speech();
			}

			if (g_ai_render_spatial_effects)
			{
				ai_debug_render_spatial_effects();
			}

			if (g_ai_render_clumps_enabled || g_ai_render_clump_props_enabled || g_ai_render_clump_dialogue || g_ai_render_clump_props_all)
			{
				ai_debug_render_clumps();
			}

			if (g_ai_render_decisions)
			{
				ai_debug_render_decisions(false);
			}

			if (g_ai_render_decisions_all)
			{
				ai_debug_render_decisions(true);
			}
#endif

			if (g_ai_render_behavior_stack_all)
			{
				ai_debug_render_behavior_stacks_all();
			}

			if (g_ai_render_character_names)
			{
				ai_debug_render_character_names();
			}

			if (g_ai_render_stimuli)
			{
				stimuli_debug();
			}

			if (g_ai_render_sectors)
			{
				ai_debug_render_sectors();
			}

#if 0
			if (g_ai_render_sector_geometry_errors)
			{
				ai_render_sector_geometry_errors();
			}

			if (g_ai_render_link_specific != NONE)
			{
				ai_debug_render_link_specific();
			}

			if (g_ai_render_links)
			{
				ai_debug_render_links();
			}
#endif

			if (g_ai_render_user_hints)
			{
				ai_render_user_hints();
			}

			if (g_ai_render_hints)
			{
				ai_render_hints();
			}

			if (g_ai_render_object_hints_all)
			{
				ai_render_object_hints(false);
			}
			else if (g_ai_render_object_hints)
			{
				ai_render_object_hints(true);
			}

			if (g_ai_render_object_properties)
			{
				ai_render_object_properties();
			}

#if 0
			if (g_ai_render_sector_bsps)
			{
				sector_bsps_debug();
			}
			else if (g_ai_render_giant_sector_bsps)
			{
				giant_sector_bsps_debug();
			}

			if (g_ai_render_sector_link_errors)
			{
				ai_debug_render_sector_link_errors();
			}
#endif

			if (g_ai_render_intersection_links)
			{
				ai_debug_render_intersection_links();
			}

#if 0
			if (g_ai_render_non_walkable_sectors)
			{
				ai_debug_render_non_walkable_sectors();
			}
#endif

			if (g_ai_render_threshold_links)
			{
				ai_debug_render_threshold_links();
			}

			if (g_ai_render_objectives || g_ai_render_strength)
			{
				ai_debug_render_squads();
			}

			if (g_ai_render_suppress_combat)
			{
				ai_debug_render_suppress_combat();
			}

#if 0
			if (g_ai_render_ai_iterator != NONE)
			{
				ai_debug_render_ai_iterator();
			}
#endif

			if (g_ai_render_vehicle_reservations)
			{
				ai_debug_render_vehicle_reservations();
			}

			if (g_ai_debug_tracking_data)
			{
				ai_debug_tracking_data();
			}

			if (g_ai_debug_perception_data)
			{
				ai_debug_perception_data();
			}

			if (g_ai_debug_combat_status)
			{
				debug_combat_status();
			}

			if (g_ai_render_tracked_props_all)
			{
				ai_debug_render_tracked_props_all();
			}

			if (g_ai_render_targets_all)
			{
				ai_debug_render_targets_all();
			}

#if 0
			if (g_ai_render_joint_behaviors)
			{
				debug_render_joint_behaviors();
			}

			if (g_ai_render_flocks)
			{
				debug_render_flocks();
			}
#endif

			if (g_ai_render_command_scripts)
			{
				render_command_scripts();
			}

			if (g_ai_render_dialogue_variants)
			{
				render_dialogue_variants();
			}

			if (g_ai_render_dynamic_firing_positions)
			{
				ai_debug_render_dynamic_firing_positions();
			}

#if 0
			if (!ai_hide_actor_errors)
			{
				debug_render_actor_errors();
			}
#endif

			if (g_ai_render_vehicle_interest)
			{
				debug_render_vehicle_interest();
			}

			if (g_ai_render_player_battle_vector)
			{
				debug_render_player_battle_vector();
			}

			if (g_ai_render_player_needs_vehicle)
			{
				debug_render_player_needs_vehicle();
			}

#if 0
			if (g_ai_render_mission_critical)
			{
				debug_render_mission_critical_ai();
			}
#endif
		}

#if 0
		if (g_dialogue_debug)
		{
			ai_dialogue_render_records();
		}
#endif
	}
}

void ai_debug_drawstack_setup(const real_point3d* position)
{
	int32 user_index = player_mapping_first_active_output_user();
	const s_observer_result* camera = observer_try_and_get_camera(user_index);

	global_ai_debug_drawstack_last_position = *position;
	global_ai_debug_drawstack_next_position = global_ai_debug_drawstack_last_position;

	if (camera)
	{
		real_vector3d vector{};
		vector_from_points3d(&global_ai_debug_drawstack_last_position, &camera->position, &vector);
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

real_point3d* ai_debug_drawstack_offset(real32 offset)
{
	global_ai_debug_drawstack_last_position = global_ai_debug_drawstack_next_position;
	global_ai_debug_drawstack_next_position.z += offset;

	return &global_ai_debug_drawstack_last_position;
}

void ai_debug_string(const char* string, int16 tab_stop_count, const int16* tab_stops, const real_argb_color* color)
{
	c_rasterizer_draw_string draw_string;
	c_font_cache_mt_safe font_cache;

	rectangle2d bounds{};
	bounds.x0 = 0;
	bounds.y0 = global_ai_debug_string_position;
	bounds.x1 = SHRT_MAX;
	bounds.y1 = SHRT_MAX;

	if (!color)
	{
		color = global_real_argb_white;
	}

	interface_set_bitmap_text_draw_mode(&draw_string, _terminal_font, _text_style_plain, _text_justification_left, 0, 5, 0);

	draw_string.set_color(color);
	draw_string.set_tab_stops(tab_stops, tab_stop_count);
	draw_string.set_bounds(&bounds);
	draw_string.draw(&font_cache, string);

	point2d cursor{};
	draw_string.get_cursor(&cursor);
	global_ai_debug_string_position -= cursor.y - bounds.y0;
}

void render_command_scripts_helper(actor_datum* actor, int32 command_script_index)
{
	// $IMPLEMENT

	// $TODO `cs_atom_string`

	//command_script_datum* command_script = DATUM_GET(command_script_data, command_script_datum, command_script_index);
	//
	//if (command_script->next_cs != NONE)
	//{
	//	render_command_scripts_helper(actor, command_script->next_cs);
	//}
	//
	//if (command_script->thread_index != NONE)
	//{
	//	hs_thread* thread = hs_thread_get(command_script->thread_index);
	//	hs_script* script = nullptr;
	//	const real_argb_color* color = nullptr;
	//
	//	if (thread->script_index != NONE)
	//	{
	//		script = TAG_BLOCK_GET_ELEMENT(&global_scenario_get()->hs_scripts, thread->script_index, hs_script);
	//	}
	//
	//	int16 atom_channel_start = actor->meta.swarm ? 1 : 3;
	//	for (int16 atom_channel = atom_channel_start - 1; atom_channel >= 0; atom_channel--)
	//	{
	//		if (command_script_index == actor->commands.first_command_script_index)
	//		{
	//			color = global_real_argb_white;
	//		}
	//		else
	//		{
	//			color = global_real_argb_grey;
	//		}
	//
	//		char string[202]{};
	//		switch (command_script->control[atom_channel].status)
	//		{
	//		case NONE:
	//		{
	//			csstrnzcpy(string, "(NONE)", NUMBEROF(string));
	//		}
	//		break;
	//		case 0:
	//		{
	//			csstrnzcpy(string, "(FAILED)", NUMBEROF(string));
	//		}
	//		break;
	//		case 1:
	//		{
	//			csstrnzcpy(string, "(FINISHED)", NUMBEROF(string));
	//		}
	//		break;
	//		case 2:
	//		{
	//			string[0] = '(';
	//			cs_atom_string(command_script_index, atom_channel, &string[1], NUMBEROF(string) - 2);
	//			csstrnzcat(string, ")", NUMBEROF(string));
	//		}
	//		break;
	//		}
	//
	//		render_debug_string_at_point(ai_debug_drawstack(), string, color);
	//	}
	//
	//	if (script)
	//	{
	//		render_debug_string_at_point(ai_debug_drawstack(), script->name, color);
	//	}
	//	else
	//	{
	//		c_static_string<256> string;
	//		render_debug_string_at_point(ai_debug_drawstack(),
	//			string.print("%s: Pri(%i) %s",
	//			hs_thread_format(command_script->thread_index),
	//			command_script->priority,
	//			command_script->reserve_mandatory ? "[M]" : " "), color);
	//	}
	//}
}

void render_command_scripts()
{
	//actor_iterator iterator{};
	//actor_iterator_new(&iterator, false);
	//while (actor_datum* actor = actor_iterator_next(&iterator))
	//{
	//	if (actor->commands.first_command_script_index != NONE)
	//	{
	//		real_point3d position{};
	//		point_from_line3d(&actor->input.position.head_position, global_up3d, 0.1f, &position);
	//		ai_debug_drawstack_setup(&position);
	//		render_command_scripts_helper(actor, actor->commands.first_command_script_index);
	//	}
	//}
}

// $TODO figure out parameter names
void ai_debug_render_actor(int32 actor_index, bool actor_selected, int32* a3)
{
}

void ai_debug_render_all_actors(bool inactive)
{
	// $IMPLEMENT `ai_debug_render_actor`

#if 0
	actor_iterator iterator;
	actor_iterator_new(&iterator, !inactive);
	while (actor_iterator_next(&iterator))
	{
		ai_debug_render_actor(iterator.index, iterator.index == g_ai_debug_selected_actor_index, nullptr);
	}
#endif
}

void ai_debug_render_behavior_stacks_all()
{
	actor_iterator iterator{};
	actor_iterator_new(&iterator, false);
	while (actor_datum* actor = actor_iterator_next(&iterator))
	{
		real_point3d position{};
		point_from_line3d(&actor->input.position.head_position, global_up3d, 0.1f, &position);
		ai_debug_drawstack_setup(&position);

		for (int16 layer_index = 0; layer_index <= actor->state.leaf_layer; layer_index++)
		{
			render_debug_string_at_point(ai_debug_drawstack(), behavior_names[actor_behavior_index_get(actor, layer_index)], global_real_argb_green);
		}
	}
}

void ai_debug_render_character_names()
{
	actor_iterator iterator{};
	actor_iterator_new(&iterator, false);
	while (actor_datum* actor = actor_iterator_next(&iterator))
	{
		real_point3d position{};
		point_from_line3d(&actor->input.position.head_position, global_up3d, 0.1f, &position);
		ai_debug_drawstack_setup(&position);

		render_debug_string_at_point(ai_debug_drawstack(), c_string_builder("%s", tag_name_strip_path(tag_get_name(actor->meta.character_definition_index))).get_string(), global_real_argb_green);
	}
}

void ai_debug_render_sectors()
{
	for (int16 structure_bsp_index = 0; structure_bsp_index < global_scenario_get()->structure_bsp_references.count; structure_bsp_index++)
	{
		if (TEST_BIT(global_structure_bsp_active_mask_get(), structure_bsp_index))
		{
			const pathfinding_data* pf_data = pathfinding_data_get(structure_bsp_index);
			if (pf_data)
			{
				int32 sectors_range_min = 0;
				int32 sectors_range_max = pf_data->sectors.count;

				if (g_ai_render_sectors_range_min != NONE)
				{
					sectors_range_min = g_ai_render_sectors_range_min;
				}

				if (g_ai_render_sectors_range_max != NONE)
				{
					sectors_range_max = g_ai_render_sectors_range_max;
				}

				for (int32 link_index = 0; link_index < pf_data->sector_links.count; link_index++)
				{
					sector_link& link = pf_data->sector_links[link_index];

					if (IN_RANGE_INCLUSIVE(link.sectors[0], sectors_range_min, sectors_range_max) ||
						IN_RANGE_INCLUSIVE(link.sectors[1], sectors_range_min, sectors_range_max))
					{
						sector_link_render_debug(link_index, pf_data, nullptr, false);
					}
				}
			}
		}
	}
}

void ai_render_object_properties()
{
	const char* const ai_size_names[k_ai_size_count]
	{
		"default",
		"tiny",
		"small",
		"medium",
		"large",
		"huge",
		"immobile"
	};

	const char* const leap_size_names[k_global_ai_jump_height_count]
	{
		"NONE",
		"down",
		"step",
		"crouch",
		"stand",
		"storey",
		"tower",
		"infinite"
	};

	c_object_iterator<object_datum> object_iterator;
	object_iterator.begin(_object_mask_crate | _object_mask_unit, 0);
	while (object_iterator.next())
	{
		object_datum* object = object_iterator.get_datum();
		struct object_definition* object_definition = TAG_GET(OBJECT_TAG, struct object_definition, object->definition_index);

		if (object_definition->object.ai_properties.count)
		{
			object_ai_properties& ai_properties = object_definition->object.ai_properties[0];
			ai_debug_drawstack_setup(&object->object.bounding_sphere_center);

			render_debug_string_at_point(ai_debug_drawstack(), c_string_builder("leap size: %s", leap_size_names[ai_properties.leap_jump_speed.get()]).get_string(), global_real_argb_blue);
			render_debug_string_at_point(ai_debug_drawstack(), ai_size_names[ai_properties.ai_size.get()], global_real_argb_blue);
		}
	}
}

void ai_debug_render_intersection_links()
{
	for (int16 structure_bsp_index = 0; structure_bsp_index < global_scenario_get()->structure_bsp_references.count; structure_bsp_index++)
	{
		if (TEST_BIT(global_structure_bsp_active_mask_get(), structure_bsp_index))
		{
			const pathfinding_data* pf_data = pathfinding_data_get(structure_bsp_index);
			if (pf_data)
			{
				for (int32 link_index = 0; link_index < pf_data->sector_links.count; link_index++)
				{
					if (TEST_BIT(pf_data->sector_links[link_index].flags, _sector_link_section_link_bit))
					{
						sector_link_render_debug(link_index, pf_data, nullptr, false);
					}
				}
			}
		}
	}
}

void ai_debug_render_threshold_links()
{
	for (int16 structure_bsp_index = 0; structure_bsp_index < global_scenario_get()->structure_bsp_references.count; structure_bsp_index++)
	{
		if (TEST_BIT(global_structure_bsp_active_mask_get(), structure_bsp_index))
		{
			const pathfinding_data* pf_data = pathfinding_data_get(structure_bsp_index);
			if (pf_data)
			{
				for (int32 link_index = 0; link_index < pf_data->sector_links.count; link_index++)
				{
					if (TEST_BIT(pf_data->sector_links[link_index].flags, _sector_link_threshold_bit))
					{
						sector_link_render_debug(link_index, pf_data, global_real_argb_orange, true);
					}
				}
			}
		}
	}
}

void ai_debug_render_squads()
{
	squad_iterator squad_iter{};
	squad_iterator_new(&squad_iter);
	while (squad_datum* squad = squad_iterator_next(&squad_iter))
	{
		struct scenario* scenario = global_scenario_get();
		s_squad_definition* squad_def = &scenario->squads[DATUM_INDEX_TO_ABSOLUTE_INDEX(squad_iter.squad_index)];
		real_point3d position = *global_origin3d;

		int16 scale = 0;
		{
			squad_actor_iterator squad_actor_iter{};
			squad_actor_iterator_new(&squad_actor_iter, squad_iter.squad_index, true);
			while (actor_datum* squad_actor = squad_actor_iterator_next(&squad_actor_iter))
			{
				add_vectors3d((real_vector3d*)&position, (real_vector3d*)&squad_actor->input.position.head_position, (real_vector3d*)&position);
				add_vectors3d((real_vector3d*)&position, (real_vector3d*)&squad_actor->input.position.body_position, (real_vector3d*)&position);
				scale += 2;
			}
		}

		if (scale > 0)
		{
			scale_vector3d((real_vector3d*)&position, 1.0f / (real32)scale, (real_vector3d*)&position);
			ai_debug_drawstack_setup(&position);
			render_debug_sphere(true, &position, 0.3f, global_real_argb_red);

			squad_actor_iterator squad_actor_iter{};
			squad_actor_iterator_new(&squad_actor_iter, squad_iter.squad_index, true);
			while (actor_datum* squad_actor = squad_actor_iterator_next(&squad_actor_iter))
			{
				real_point3d center{};
				real32 radius = 0.0f;

				if (squad_actor->meta.unit_index == NONE)
				{
					center = squad_actor->input.position.body_position;
				}
				else
				{
					object_get_bounding_sphere(squad_actor->meta.unit_index, &center, &radius);
				}

				render_debug_line(true, &position, &center, squad_actor->meta.squad_limbo ? global_real_argb_yellow : global_real_argb_red);
			}

			if (g_ai_render_strength)
			{
				char string[128]{};
				csnzprintf(string, sizeof(string), "Strength: %f", squad->current_strength_fraction);
				render_debug_string_at_point(ai_debug_drawstack(), string, global_real_argb_aqua);
			}

			if (g_ai_render_objectives)
			{
				ai_debug_render_objectives(squad_iter.squad_index, &position);
			}

			render_debug_string_at_point(ai_debug_drawstack(), squad_def->name.get_string(), global_real_argb_red);
		}
	}
}

void ai_debug_render_suppress_combat()
{
	actor_iterator iterator{};
	actor_iterator_new(&iterator, true);
	while (actor_datum* actor = actor_iterator_next(&iterator))
	{
		if (actor->state.suppress_combat)
		{
			render_debug_string_at_point(&actor->input.position.head_position, "COMBAT SUPPRESSED", global_real_argb_blue);
		}
	}
}

void ai_debug_render_vehicle_reservations()
{
	c_object_iterator<vehicle_datum> vehicle_iter;
	vehicle_iter.begin(_object_mask_vehicle, 0);
	while (vehicle_iter.next())
	{
		unit_seat_source sources[k_maximum_hs_unit_seats]{};
		int16 source_count = unit_get_all_seats(vehicle_iter.get_index(), sources, NUMBEROF(sources), false);
		for (int16 source_index = 0; source_index < source_count; source_index++)
		{
			unit_seat_source& source = sources[source_index];
			if (!source.flags->test(_unit_seat_allow_ai_noncombatants_bit))
			{
				vehicle_datum* vehicle = VEHICLE_GET(source.vehicle_index);

				real_point3d seat_position{};
				vehicle_get_seat_position(source.vehicle_index, source.seat_index, &seat_position);

				if (BIT_VECTOR_TEST_FLAG(vehicle->vehicle.disallowed_seats, source.seat_index))
				{
					seat_position.z += 0.1f;
					render_debug_sphere(true, &seat_position, 0.05f, global_real_argb_orange);
				}
				else
				{
					for (int16 i = 0; i < 2; i++)
					{
						const real_argb_color* color = global_real_argb_white;
						int32 actor_index = ai_vehicle_get_reservation(source.vehicle_index, source.seat_index, i == 0 ? _campaign_team_player : _campaign_team_flood);
						seat_position.z += 0.1f;

						if (actor_index == NONE)
						{
							color = global_real_argb_red;
						}
						else
						{
							actor_datum* actor = DATUM_GET(actor_data, actor_datum, actor_index);
							switch (actor->vehicle.attachment_status)
							{
							case 1: color = global_real_argb_yellow; break;
							case 2: color = global_real_argb_violet; break;
							case 3: color = global_real_argb_green;  break;
							case 4: color = global_real_argb_blue;   break;
							}

							render_debug_line(true, &actor->input.position.head_position, &seat_position, color);
						}

						render_debug_sphere(true, &seat_position, 0.05f, color);
					}
				}
			}
		}
	}
}

void ai_debug_tracking_data()
{
	char string[50]{};
	int16 total_tracking_index = 0;

	actor_iterator iterator{};
	actor_iterator_new(&iterator, true);
	while (actor_datum* actor = actor_iterator_next(&iterator))
	{
		real_point3d position{};
		point_from_line3d(&actor->input.position.head_position, global_up3d, 0.1f, &position);
		ai_debug_drawstack_setup(&position);

		int16 tracking_index = 0;
		actor_prop_ref_iterator actor_prop_ref_iter{};
		actor_prop_ref_iterator_new(&actor_prop_ref_iter, iterator.index);
		while (prop_ref_datum* actor_prop_ref = actor_prop_ref_iterator_next(&actor_prop_ref_iter))
		{
			if (actor_prop_ref->tracking_index != NONE)
			{
				tracking_index++;
			}
		}

		csnzprintf(string, sizeof(string), "Tracking data: %i", tracking_index);
		render_debug_string_at_point(ai_debug_drawstack(), string, global_real_argb_green);

		total_tracking_index += tracking_index;
	}

	csnzprintf(string, sizeof(string), "Total tracking data: %i", total_tracking_index);
	ai_debug_string(string, 0, nullptr, global_real_argb_green);
}

void ai_debug_perception_data()
{
	c_rasterizer_draw_string draw_string;
	c_font_cache_mt_safe font_cache;
	char string[500]{};

	csnzprintf(string, sizeof(string), "Total clumps: %i/%i|nTotal clump props: %i/%i|n, Total prop-refs: %i/%i|nTotal tracks:%i/%i",
		clump_data->actual_count, 20,
		prop_data->actual_count, 256,
		prop_ref_data->actual_count, 1024,
		tracking_data->actual_count, 100);

	draw_string.draw(&font_cache, string);
}

void debug_combat_status()
{
	actor_iterator iterator{};
	actor_iterator_new(&iterator, true);
	while (actor_datum* actor = actor_iterator_next(&iterator))
	{
		real_point3d position{};
		point_from_line3d(&actor->input.position.head_position, global_up3d, 0.1f, &position);
		ai_debug_drawstack_setup(&position);

		render_debug_string_at_point(ai_debug_drawstack(), c_string_builder("Combat status: %i", actor->state.combat_status).get_string(), global_real_argb_green);
		render_debug_string_at_point(ai_debug_drawstack(), c_string_builder("Highest prop class: %i", actor->situation.highest_prop_class).get_string(), global_real_argb_green);
	}
}

void ai_debug_render_tracked_props_all()
{
	actor_iterator iterator{};
	actor_iterator_new(&iterator, true);
	while (actor_datum* actor = actor_iterator_next(&iterator))
	{
		actor_prop_ref_iterator actor_prop_ref_iter{};
		actor_prop_ref_iterator_new(&actor_prop_ref_iter, iterator.index);
		while (prop_ref_datum* actor_prop_ref = actor_prop_ref_iterator_next(&actor_prop_ref_iter))
		{
			if (actor_prop_ref->tracking_index != NONE)
			{
				real_point3d position{};
				object_get_origin(actor_prop_ref->object_index, &position);
				if (actor_prop_ref_iter.index == actor->target.target_prop_index)
				{
					render_debug_line(true, &actor->input.position.head_position, &position, global_real_argb_red);
				}
				else
				{
					render_debug_line(true, &actor->input.position.head_position, &position, global_real_argb_yellow);
				}
			}
		}
	}
}

void ai_debug_render_targets_all()
{
	actor_iterator iterator{};
	actor_iterator_new(&iterator, true);
	while (actor_datum* actor = actor_iterator_next(&iterator))
	{
		if (actor->target.target_prop_index != NONE)
		{
			prop_ref_datum* prop_ref = DATUM_GET(prop_ref_data, prop_ref_datum, actor->target.target_prop_index);

			real_point3d origin{};
			object_get_origin(prop_ref->get_object_index(), &origin);
			render_debug_line(true, &actor->input.position.head_position, &origin, global_real_argb_red);
		}
	}
}

void render_dialogue_variants()
{
	actor_iterator iterator{};
	actor_iterator_new(&iterator, false);
	while (actor_datum* actor = actor_iterator_next(&iterator))
	{
		if (actor->meta.unit_index != NONE)
		{
			unit_datum* unit = UNIT_GET(actor->meta.unit_index);
			s_seat_storage* seat_storage = OBJECT_HEADER_BLOCK_GET(actor->meta.unit_index, s_seat_storage, &unit->unit.seat_storage);
			if (seat_storage->dialogue_definition_index != NONE)
			{
				s_dialogue_definition* dialogue_definition = TAG_GET(DIALOGUE_TAG, s_dialogue_definition, seat_storage->dialogue_definition_index);
				const char* mission_dialogue_designator_name = dialogue_definition->mission_dialogue_designator.get_string();
				render_debug_string_at_point(&actor->input.position.head_position, mission_dialogue_designator_name, global_real_argb_white);
			}
		}
	}
}

void ai_debug_render_dynamic_firing_positions()
{
	c_data_iterator<dynamic_firing_set_datum> dynamic_firing_set_iter;
	dynamic_firing_set_iter.begin(dynamic_firing_set_data);
	while (dynamic_firing_set_iter.next())
	{
		real_point3d position{};
		const real_argb_color* color = nullptr;

		dynamic_firing_set_datum* dynamic_firing_set = dynamic_firing_set_iter.get_datum();
		if (dynamic_firing_set->support_object_index == NONE)
		{
			real_point3d ai_point_position{};
			ai_point_get_position(&dynamic_firing_set->position, &ai_point_position);
			render_debug_sphere(true, &ai_point_position, 0.5f, global_real_argb_orange);

			position = ai_point_position;
			color = global_real_argb_orange;
		}
		else
		{
			object_datum* object = object_get(dynamic_firing_set->support_object_index);
			render_debug_sphere(true, &object->object.bounding_sphere_center, object->object.bounding_sphere_radius, global_real_argb_red);
			position = object->object.bounding_sphere_center;
			color = global_real_argb_red;
		}

		ai_debug_drawstack_setup(&position);
		render_debug_string_at_point(ai_debug_drawstack(), c_string_builder("members: %i", dynamic_firing_set->num_members).get_string(), color);

		if (dynamic_firing_set->support_object_index != NONE)
		{
			render_debug_string_at_point(ai_debug_drawstack(), "No object", global_real_argb_orange);
		}
	}


	actor_iterator iterator{};
	actor_iterator_new(&iterator, false);
	while (actor_datum* actor = actor_iterator_next(&iterator))
	{
		if (actor->firing_positions.dynamic_firing_set_index != NONE)
		{
			dynamic_firing_set_datum* dynamic_firing_set = DATUM_GET(dynamic_firing_set_data, dynamic_firing_set_datum, actor->firing_positions.dynamic_firing_set_index);

			real_point3d ai_point_position{};
			ai_point_get_position(&dynamic_firing_set->position, &ai_point_position);
			render_debug_line(true, &actor->input.position.head_position, &ai_point_position, global_real_argb_blue);
		}
	}
}

void ai_debug_change_selected_squad(bool next_squad)
{
	console_warning("Unimplemented: " __FUNCTION__);
}

void ai_debug_change_selected_actor(bool next_actor)
{
	console_warning("Unimplemented: " __FUNCTION__);
}

