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

int16 global_ai_profile_draw_string_position;
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

const char* spray_mode_names[NUMBER_OF_AI_PROFILE_RENDER_SPRAY_MODES]
{
	"none",
	"actions",
	"activation status"
};

int16 ai_meter_actor()
{
	int16 member_count = (int16)actor_data->actual_count;
	return member_count;
}

int16 ai_meter_prop()
{
	int16 member_count = (int16)prop_data->actual_count;
	return member_count;
}

int16 ai_meter_swarm_actor()
{
	int16 member_count = 0;
	actor_iterator iterator{};
	actor_iterator_new(&iterator, false);
	for (actor_datum* actor = actor_iterator_next(&iterator); actor; actor = actor_iterator_next(&iterator))
		member_count += actor->meta.swarm;
	return member_count;
}

int16 ai_meter_swarm_cache()
{
	int16 member_count = (int16)swarm_data->actual_count;
	return member_count;
}

int16 ai_meter_unit()
{
	int16 member_count = 0;
	actor_iterator iterator{};
	actor_iterator_new(&iterator, false);
	for (actor_datum* actor = actor_iterator_next(&iterator); actor; actor = actor_iterator_next(&iterator))
	{
		int32 swarm_index = actor->meta.swarm_index;
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

int16 ai_profile_change_render_spray()
{
	ai_profile.render_spray_mode = (ai_profile.render_spray_mode + 1) % NUMBER_OF_AI_PROFILE_RENDER_SPRAY_MODES;
	console_printf("AI line-spray: %s", spray_mode_names[ai_profile.render_spray_mode]);
	return ai_profile.render_spray_mode;
}

void ai_profile_display(char* textbuffer, int32 textbuffer_size)
{
	int32 active_actors = count_actors(true);
	int32 actors = count_actors(false);
	csnzappendf(textbuffer, textbuffer_size, "%d/%d actors|n", active_actors, actors);
}

void ai_profile_dispose()
{
}

void ai_profile_dispose_from_old_map()
{
}

void ai_profile_draw_string(const char* string, int16 tab_count, const int16* tabs, const real_argb_color* color)
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
	int32 active_output_user = player_mapping_first_active_output_user();
	const s_observer_result* camera = observer_try_and_get_camera(active_output_user);
	if (ai_profile.render_spray_mode > _ai_profile_render_spray_none && camera && ai_globals->ai_initialized_for_map)
	{
		//main_set_single_thread_request_flag(_single_thread_for_ai_profile, true);

		if (actor_datum_available_to_current_thread())
		{
			real_point3d from_point{};
			// $TODO find out why 0.05f doesn't display correctly, possibly something wrong with the near plane
			point_from_line3d(&camera->position, &camera->forward, 1.0f /*0.05f*/, &from_point);

			actor_iterator iterator{};
			actor_iterator_new(&iterator, ai_profile.render_spray_mode != _ai_profile_render_spray_activation);
			while (actor_iterator_next(&iterator))
			{
				actor_datum* actor = DATUM_GET(actor_data, actor_datum, iterator.index);

				const real_argb_color* debug_color = NULL;
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
						int32 swarm_index = actor->meta.swarm_index;
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
	int16 tabs[] = { 150, 300 };
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
	int16 tabs[] = { 150, 300, 450 };
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
	int16 tabs[] = { 150, 300, 450 };
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
	int16 tabs[] = { 150, 300, 450 };
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
	int16 tabs[] = { 150 };
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
	for (int32 index = 0; index < NUMBER_OF_AI_METERS; index++)
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

		int16 history_next_index = (meter->history_next_index + 1) % AI_METER_HISTORY_TICKS;
		meter->history_max_index = MAX(meter->history_max_index, history_next_index);
		meter->history_next_index = history_next_index;

		meter->average_count = (real32)meter->average_total / meter->history_max_index;
	}

	csmemset(&ai_profile.ai_profile_info, 0, sizeof(s_ai_profile_info));
}

int32 count_actors(bool active_only)
{
	int32 actor_count = 0;
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

bool __cdecl actor_general_update_for_ai_meters(int32 actor_index)
{
	ai_profile.meters[_ai_meter_actor_active].current_count;
	actor_datum* actor = DATUM_GET(actor_data, actor_datum, actor_index);
	if (actor && actor->meta.swarm)
		ai_profile.meters[_ai_meter_unit_active].current_count++;

	return actor_general_update(actor_index);
}
HOOK_DECLARE_CALL(0x0142E08E, actor_general_update_for_ai_meters);
HOOK_DECLARE_CALL(0x01430180, actor_general_update_for_ai_meters);

bool __cdecl ai_test_line_of_fire_for_ai_meters(int32 actor_index, int32 ignore_unit_index, const real_point3d* origin, const real_vector3d* vector, int32* prop_index_reference)
{
	ai_profile.meters[_ai_meter_line_of_fire].current_count++;
	return ai_test_line_of_fire(actor_index, ignore_unit_index, origin, vector, prop_index_reference);
}
HOOK_DECLARE_CALL(0x01463025, ai_test_line_of_fire_for_ai_meters);

int16 __cdecl ai_test_line_of_sight_for_ai_meters(const real_point3d* p0, s_cluster_reference p0_cluster_ref, const real_point3d* p1, s_cluster_reference p1_cluster_ref, int16 mode, bool test_line_of_fire, int32 ignore_object_index, int32 ignore_object_index2, bool ignore_vehicles, bool allow_early_out, int32* blocking_object_index_ref, bool* two_sided_obstruction_ref)
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

bool __cdecl collision_test_line_for_ai_meters(s_collision_test_flags flags, const real_point3d* point0, const real_point3d* point1, int32 first_ignore_object_index, int32 second_ignore_object_index, collision_result* collision)
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

bool __cdecl collision_test_vector_for_ai_meters(s_collision_test_flags flags, const real_point3d* point, const real_vector3d* vector, int32 first_ignore_object_index, int32 second_ignore_object_index, collision_result* collision)
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

