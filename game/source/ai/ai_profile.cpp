#include "ai/ai_profile.hpp"

#include "creatures/creatures.hpp"
#include "interface/interface.hpp"
#include "interface/interface_constants.hpp"
#include "main/console.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "physics/collisions.hpp"
#include "render/render_debug.hpp"
#include "text/draw_string.hpp"
#include "units/bipeds.hpp"

#define AI_METER_HISTORY_TICKS 60

short global_ai_profile_draw_string_position;
char profilestring[2048]{};
ai_profile_state ai_profile{};

ai_meter_definition global_ai_meter_definitions[NUMBER_OF_AI_METERS]
{
	{ _ai_meter_encounter_active, 0, ai_meter_actor },
	{ _ai_meter_actor, 0, NULL },
	{ _ai_meter_actor_active, 0, NULL },
	{ _ai_meter_actor_non_dormant, 0, NULL },

	{ _ai_meter_unit, 0, ai_meter_unit },
	{ _ai_meter_unit_active, 0, NULL },
	{ _ai_meter_unit_non_dormant, 0, NULL },

	{ _ai_meter_prop, 0, ai_meter_prop },
	{ _ai_meter_prop_acknowledged_body, 0, NULL },
	{ _ai_meter_prop_orphaned_body, 0, NULL },
	{ _ai_meter_prop_unacknowledged_body, 0, NULL },
	{ _ai_meter_prop_acknowledged_enemy, 0, NULL },
	{ _ai_meter_prop_orphaned_enemy, 0, NULL },
	{ _ai_meter_prop_unacknowledged_enemy, 0, NULL },
	{ _ai_meter_prop_acknowledged_friend, 0, NULL },
	{ _ai_meter_prop_orphaned_friend, 0, NULL },
	{ _ai_meter_prop_unacknowledged_friend, 0, NULL },

	{ _ai_meter_swarm_actor, 0, ai_meter_swarm_actor },
	{ _ai_meter_swarm_cache, 0, ai_meter_swarm_cache },

	{ _ai_meter_collision_vector, 0, NULL },
	{ _ai_meter_line_of_sight, 0, NULL },
	{ _ai_meter_line_of_fire, 0, NULL },
	{ _ai_meter_path_flood, 0, NULL },
	{ _ai_meter_path_find, 0, NULL },
	{ _ai_meter_action_change, 0, NULL },
	{ _ai_meter_firing_point_eval, 0, NULL }
};

char const* spray_mode_names[NUMBER_OF_AI_PROFILE_RENDER_SPRAY_MODES]
{
	"none",
	"actions",
	"activation status"
};

short ai_meter_actor()
{
	TLS_DATA_GET_VALUE_REFERENCE(actor_data);
	short member_count = (short)actor_data->actual_count;
	return member_count;
}

short ai_meter_prop()
{
	TLS_DATA_GET_VALUE_REFERENCE(prop_data);
	short member_count = (short)prop_data->actual_count;
	return member_count;
}

short ai_meter_swarm_actor()
{
	short member_count = 0;
	actor_iterator iterator{};
	actor_iterator_new(&iterator, false);
	for (actor_datum* actor = actor_iterator_next(&iterator); actor; actor = actor_iterator_next(&iterator))
		member_count += actor->meta.swarm;
	return member_count;
}

short ai_meter_swarm_cache()
{
	TLS_DATA_GET_VALUE_REFERENCE(swarm_data);
	short member_count = (short)swarm_data->actual_count;
	return member_count;
}

short ai_meter_unit()
{
	short member_count = 0;
	actor_iterator iterator{};
	actor_iterator_new(&iterator, false);
	for (actor_datum* actor = actor_iterator_next(&iterator); actor; actor = actor_iterator_next(&iterator))
	{
		long swarm_index = actor->meta.swarm_index;
		if (swarm_index == NONE)
		{
			member_count++;
		}
		else
		{
			swarm_datum* swarm = swarm_get(swarm_index);
			member_count += swarm->member_count;
		}
	}
	return member_count;
}

short ai_profile_change_render_spray()
{
	ai_profile.render_spray_mode = (ai_profile.render_spray_mode + 1) % NUMBER_OF_AI_PROFILE_RENDER_SPRAY_MODES;
	console_printf("AI line-spray: %s", spray_mode_names[ai_profile.render_spray_mode]);
	return ai_profile.render_spray_mode;
}

void ai_profile_display(char* textbuffer, long textbuffer_size)
{
	long active_actors = count_actors(true);
	long actors = count_actors(false);
	csnzappendf(textbuffer, textbuffer_size, "%d/%d actors|n", active_actors, actors);
}

void ai_profile_dispose()
{
}

void ai_profile_dispose_from_old_map()
{
}

void ai_profile_draw_string(char const* string, short tab_count, short const* tabs, real_argb_color const* color)
{
	c_rasterizer_draw_string draw_string{};
	c_font_cache_mt_safe font_cache{};
	rectangle2d bounds{};
	point2d cursor{};

	bounds.x0 = 0x0000;
	bounds.y1 = 0x7FFF;
	bounds.y0 = global_ai_profile_draw_string_position;
	bounds.x1 = 0x7FFF;

	if (color)
		color = global_real_argb_white;

	interface_set_bitmap_text_draw_mode(
		&draw_string,
		_terminal_font,
		_text_style_plain,
		_text_justification_left,
		0,
		5,
		0);

	draw_string.set_color(color);
	draw_string.set_tab_stops(tabs, tab_count);
	draw_string.set_bounds(&bounds);
	draw_string.draw(&font_cache, string);
	draw_string.get_cursor(&cursor);
	global_ai_profile_draw_string_position += bounds.y0 - cursor.y;
}

void ai_profile_initialize()
{
	csmemset(&ai_profile, 0, sizeof(ai_profile));
	ai_profile.show = true;

	//ai_profile.disable_ai = true;
	//ai_profile.move_randomly = true;
	//ai_profile.show_stats = true;
	//ai_profile.show_actors = true;
	//ai_profile.show_swarms = true;
	//ai_profile.show_paths = true;
	//ai_profile.show_line_of_sight = true;
	//ai_profile.show_prop_types = true;
	//ai_profile.show_sound_distance = true;
}

void ai_profile_initialize_for_new_map()
{
	csmemset(&ai_profile.meters, 0, sizeof(ai_profile.meters));
}

void ai_profile_render()
{
	TLS_DATA_GET_VALUE_REFERENCE(ai_globals);
	if (ai_globals->ai_initialized_for_map)
	{
		rectangle2d bounds{};
		interface_get_current_display_settings(NULL, NULL, NULL, &bounds);
		global_ai_profile_draw_string_position = bounds.y1 - 20;
		ai_profile_render_spray();
		if (ai_profile.show)
		{
			if (ai_profile.show_prop_types)
				ai_profile_show_prop_types();

			if (ai_profile.show_line_of_sight)
				ai_profile_show_line_of_sight();

			if (ai_profile.show_paths)
				ai_profile_show_paths();

			if (ai_profile.show_swarms)
				ai_profile_show_swarms();

			if (ai_profile.show_actors)
				ai_profile_show_actors();
		}
	}
}

void ai_profile_render_spray()
{
	TLS_DATA_GET_VALUE_REFERENCE(actor_data);
	TLS_DATA_GET_VALUE_REFERENCE(ai_globals);
	long active_output_user = player_mapping_first_active_output_user();
	s_observer_result const* camera = observer_try_and_get_camera(active_output_user);
	if (ai_profile.render_spray_mode > _ai_profile_render_spray_none && camera && ai_globals->ai_initialized_for_map)
	{
		//main_set_single_thread_request_flag(_single_thread_for_ai_profile, true);

		if (actor_datum_available_to_current_thread())
		{
			real_point3d from_point{};
			// $TODO: find out why 0.05f doesn't display correctly, possibly something wrong with the near plane
			point_from_line3d(&camera->position, &camera->forward, 1.0f /*0.05f*/, &from_point);

			actor_iterator iterator{};
			actor_iterator_new(&iterator, ai_profile.render_spray_mode != _ai_profile_render_spray_activation);
			while (actor_iterator_next(&iterator))
			{
				actor_datum* actor = DATUM_GET(actor_data, actor_datum, iterator.index);

				real_argb_color const* debug_color = NULL;
				if (ai_profile.render_spray_mode == _ai_profile_render_spray_actions)
				{
					debug_color = global_real_argb_black;
				}
				else if (ai_profile.render_spray_mode == _ai_profile_render_spray_activation)
				{
					debug_color = actor_activation_debug_color(iterator.index);
				}

				if (debug_color)
				{
					if (actor->meta.swarm)
					{
						long swarm_index = actor->meta.swarm_index;
						if (swarm_index)
						{
							swarm_creature_iterator creature_iterator{};
							swarm_creature_iterator_new(swarm_index, &creature_iterator);
							while (swarm_creature_iterator_next(&creature_iterator))
							{
								real_point3d head_position{};
								creature_get_head_position(creature_iterator.creature_index, &head_position);
								render_debug_line(true, &from_point, &head_position, debug_color);
							}
						}
					}
					else
					{
						render_debug_line(true, &from_point, &actor->input.position.head_position, debug_color);
					}
				}
			}
		}
	}
	else
	{
		//main_set_single_thread_request_flag(_single_thread_for_ai_profile, false);
	}
}

void ai_profile_show_actors()
{
	short tabs[] = { 150, 300 };
	csnzprintf(
		profilestring, sizeof(profilestring),
		"actors %d/%d/%d|units %d/%d%d",
		ai_profile.meters[_ai_meter_actor_non_dormant].last_count,
		ai_profile.meters[_ai_meter_actor_active].last_count,
		ai_profile.meters[_ai_meter_actor].last_count,
		ai_profile.meters[_ai_meter_unit_non_dormant].last_count,
		ai_profile.meters[_ai_meter_unit_active].last_count,
		ai_profile.meters[_ai_meter_unit].last_count);
	ai_profile_draw_string(profilestring, NUMBEROF(tabs), tabs, global_real_argb_white);
}

void ai_profile_show_line_of_sight()
{
	short tabs[] = { 150, 300, 450 };
	csnzprintf(
		profilestring, sizeof(profilestring),
		"collisions %d|tlineofsight %d|tlineoffire %d|tfiringpoint %d",
		ai_profile.meters[_ai_meter_collision_vector].last_count,
		ai_profile.meters[_ai_meter_line_of_sight].last_count,
		ai_profile.meters[_ai_meter_line_of_fire].last_count,
		ai_profile.meters[_ai_meter_firing_point_eval].last_count);
	ai_profile_draw_string(profilestring, NUMBEROF(tabs), tabs, global_real_argb_white);
}

void ai_profile_show_paths()
{
	short tabs[] = { 150, 300, 450 };
	csnzprintf(
		profilestring, sizeof(profilestring),
		"path_flood %d|tpath_find %d|taction_change %d",
		ai_profile.meters[_ai_meter_path_flood].last_count,
		ai_profile.meters[_ai_meter_path_find].last_count,
		ai_profile.meters[_ai_meter_action_change].last_count);
	ai_profile_draw_string(profilestring, NUMBEROF(tabs), tabs, global_real_argb_white);
}

void ai_profile_show_prop_types()
{
	short tabs[] = { 150, 300, 450 };
	csnzprintf(
		profilestring, sizeof(profilestring),
		"props a/o/u:|tenemy %d/%d/%d|tfriend %d/%d/%d|tdead %d/%d/%d",
		ai_profile.meters[_ai_meter_prop_acknowledged_enemy].last_count,
		ai_profile.meters[_ai_meter_prop_orphaned_enemy].last_count,
		ai_profile.meters[_ai_meter_prop_unacknowledged_enemy].last_count,
		ai_profile.meters[_ai_meter_prop_acknowledged_friend].last_count,
		ai_profile.meters[_ai_meter_prop_orphaned_friend].last_count,
		ai_profile.meters[_ai_meter_prop_unacknowledged_friend].last_count,
		ai_profile.meters[_ai_meter_prop_acknowledged_body].last_count,
		ai_profile.meters[_ai_meter_prop_orphaned_body].last_count,
		ai_profile.meters[_ai_meter_prop_unacknowledged_body].last_count);
	ai_profile_draw_string(profilestring, NUMBEROF(tabs), tabs, global_real_argb_white);
}

void ai_profile_show_swarms()
{
	short tabs[] = { 150 };
	csnzprintf(
		profilestring, sizeof(profilestring),
		"swarms %d/%d/%d",
		ai_profile.meters[_ai_meter_swarm_cache].last_count,
		ai_profile.meters[_ai_meter_swarm_actor].last_count,
		32);
	ai_profile_draw_string(profilestring, NUMBEROF(tabs), tabs, global_real_argb_white);
}

void ai_profile_update()
{
	for (long index = 0; index < NUMBER_OF_AI_METERS; index++)
	{
		ai_meter_definition* definition = &global_ai_meter_definitions[index];
		ai_meter* meter = &ai_profile.meters[index];

		ASSERT(definition->meter_id == index);

		if (definition->value_callback)
			meter->current_count = definition->value_callback();

		meter->last_count = meter->current_count;
		meter->current_count = 0;

		ASSERT((meter->history_next_index >= 0) && (meter->history_next_index < AI_METER_HISTORY_TICKS));

		if (meter->history_next_index < meter->history_max_index)
			meter->average_total -= meter->history_count[meter->history_next_index];

		meter->history_count[meter->history_next_index] = meter->last_count;

		short history_next_index = (meter->history_next_index + 1) % AI_METER_HISTORY_TICKS;
		meter->history_max_index = MAX(meter->history_max_index, history_next_index);
		meter->history_next_index = history_next_index;

		meter->average_count = (real)meter->average_total / meter->history_max_index;
	}

	csmemset(&ai_profile.ai_profile_info, 0, sizeof(s_ai_profile_info));
}

long count_actors(bool active_only)
{
	long actor_count = 0;
	c_object_iterator<biped_datum> iterator;
	iterator.begin(_object_mask_biped, active_only);
	while (iterator.next())
	{
		biped_datum* biped = iterator.get_datum();
		if (biped->unit.actor_index != NONE)
		{
			actor_count++;
		}
	}

	return actor_count;
}

// hooks for ai meters

bool __cdecl actor_general_update_for_ai_meters(long actor_index)
{
	TLS_DATA_GET_VALUE_REFERENCE(actor_data);
	ai_profile.meters[_ai_meter_actor_active].current_count;
	actor_datum* actor = DATUM_GET(actor_data, actor_datum, actor_index);
	if (actor && actor->meta.swarm)
		ai_profile.meters[_ai_meter_unit_active].current_count++;

	return actor_general_update(actor_index);
}
HOOK_DECLARE_CALL(0x0142E08E, actor_general_update_for_ai_meters);
HOOK_DECLARE_CALL(0x01430180, actor_general_update_for_ai_meters);

bool __cdecl ai_test_line_of_fire_for_ai_meters(long actor_index, long ignore_unit_index, real_point3d const* origin, real_vector3d const* vector, long* prop_index_reference)
{
	ai_profile.meters[_ai_meter_line_of_fire].current_count++;
	return ai_test_line_of_fire(actor_index, ignore_unit_index, origin, vector, prop_index_reference);
}
HOOK_DECLARE_CALL(0x01463025, ai_test_line_of_fire_for_ai_meters);

short __cdecl ai_test_line_of_sight_for_ai_meters(real_point3d const* p0, s_cluster_reference p0_cluster_ref, real_point3d const* p1, s_cluster_reference p1_cluster_ref, short mode, bool test_line_of_fire, long ignore_object_index, long ignore_object_index2, bool ignore_vehicles, bool allow_early_out, long* blocking_object_index_ref, bool* two_sided_obstruction_ref)
{
	ai_profile.ai_profile_info.line_of_sight++;
	ai_profile.meters[_ai_meter_line_of_sight].current_count++;
	return ai_test_line_of_sight(p0, p0_cluster_ref, p1, p1_cluster_ref, mode, test_line_of_fire, ignore_object_index, ignore_object_index2, ignore_vehicles, allow_early_out, blocking_object_index_ref, two_sided_obstruction_ref);
}
HOOK_DECLARE_CALL(0x01462400, ai_test_line_of_sight_for_ai_meters);
HOOK_DECLARE_CALL(0x01466E9C, ai_test_line_of_sight_for_ai_meters);
HOOK_DECLARE_CALL(0x014673FC, ai_test_line_of_sight_for_ai_meters);
HOOK_DECLARE_CALL(0x0146F5F2, ai_test_line_of_sight_for_ai_meters);
HOOK_DECLARE_CALL(0x01483ABF, ai_test_line_of_sight_for_ai_meters);
HOOK_DECLARE_CALL(0x014A358F, ai_test_line_of_sight_for_ai_meters);
HOOK_DECLARE_CALL(0x014A3889, ai_test_line_of_sight_for_ai_meters);
HOOK_DECLARE_CALL(0x014B941F, ai_test_line_of_sight_for_ai_meters);
HOOK_DECLARE_CALL(0x014B9584, ai_test_line_of_sight_for_ai_meters);

bool __cdecl collision_test_line_for_ai_meters(s_collision_test_flags flags, real_point3d const* point0, real_point3d const* point1, long first_ignore_object_index, long second_ignore_object_index, collision_result* collision)
{
	ai_profile.meters[_ai_meter_collision_vector].current_count++;
	return collision_test_line(flags, point0, point1, first_ignore_object_index, second_ignore_object_index, collision);
}
HOOK_DECLARE_CALL(0x01434BB7, collision_test_line_for_ai_meters);
HOOK_DECLARE_CALL(0x01434C30, collision_test_line_for_ai_meters);
HOOK_DECLARE_CALL(0x01434E4F, collision_test_line_for_ai_meters);
HOOK_DECLARE_CALL(0x01434E76, collision_test_line_for_ai_meters);
HOOK_DECLARE_CALL(0x01434EA0, collision_test_line_for_ai_meters);
HOOK_DECLARE_CALL(0x01434F8D, collision_test_line_for_ai_meters);
HOOK_DECLARE_CALL(0x01434FAE, collision_test_line_for_ai_meters);
HOOK_DECLARE_CALL(0x01434FCF, collision_test_line_for_ai_meters);

bool __cdecl collision_test_vector_for_ai_meters(s_collision_test_flags flags, real_point3d const* point, real_vector3d const* vector, long first_ignore_object_index, long second_ignore_object_index, collision_result* collision)
{
	ai_profile.meters[_ai_meter_collision_vector].current_count++;
	return collision_test_vector(flags, point, vector, first_ignore_object_index, second_ignore_object_index, collision);
}
HOOK_DECLARE_CALL(0x014B0F19, collision_test_vector_for_ai_meters);
HOOK_DECLARE_CALL(0x014B1FC1, collision_test_vector_for_ai_meters);

bool __cdecl path_state_find_for_ai_meters(path_state* state)
{
	REFERENCE_DECLARE(offset_pointer(state, 0x168), bool, destination_valid);
	ai_profile.meters[destination_valid ? _ai_meter_path_find : _ai_meter_path_flood].current_count++;
	return path_state_find(state);
}
HOOK_DECLARE_CALL(0x0145D882, path_state_find_for_ai_meters);
HOOK_DECLARE_CALL(0x0146D828, path_state_find_for_ai_meters);
HOOK_DECLARE_CALL(0x0146E00D, path_state_find_for_ai_meters);
HOOK_DECLARE_CALL(0x01479368, path_state_find_for_ai_meters);
HOOK_DECLARE_CALL(0x0148923B, path_state_find_for_ai_meters);
HOOK_DECLARE_CALL(0x014A0F4A, path_state_find_for_ai_meters);
HOOK_DECLARE_CALL(0x014A1530, path_state_find_for_ai_meters);
HOOK_DECLARE_CALL(0x014A19B7, path_state_find_for_ai_meters);

