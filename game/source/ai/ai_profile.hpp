#pragma once

#include "cseries/cseries.hpp"

enum
{
	_ai_profile_render_spray_none = 0,
	_ai_profile_render_spray_actions,
	_ai_profile_render_spray_activation,
	
	NUMBER_OF_AI_PROFILE_RENDER_SPRAY_MODES
};

enum
{
	_ai_meter_encounter_active = 0,
	_ai_meter_actor,
	_ai_meter_actor_active,
	_ai_meter_actor_non_dormant,
	_ai_meter_unit,
	_ai_meter_unit_active,
	_ai_meter_unit_non_dormant,
	_ai_meter_prop,
	_ai_meter_prop_acknowledged_body,
	_ai_meter_prop_orphaned_body,
	_ai_meter_prop_unacknowledged_body,
	_ai_meter_prop_acknowledged_enemy,
	_ai_meter_prop_orphaned_enemy,
	_ai_meter_prop_unacknowledged_enemy,
	_ai_meter_prop_acknowledged_friend,
	_ai_meter_prop_orphaned_friend,
	_ai_meter_prop_unacknowledged_friend,
	_ai_meter_swarm_actor,
	_ai_meter_swarm_cache,
	_ai_meter_collision_vector,
	_ai_meter_line_of_sight,
	_ai_meter_line_of_fire,
	_ai_meter_path_flood,
	_ai_meter_path_find,
	_ai_meter_action_change,
	_ai_meter_firing_point_eval,

	NUMBER_OF_AI_METERS
};

struct ai_meter
{
	short current_count;
	short last_count;
	real average_count;
	long average_total;
	short history_next_index;
	short history_max_index;
	short history_count[60];
};
static_assert(sizeof(ai_meter) == 0x88);

struct ai_meter_definition
{
	short meter_id;
	short flags;
	short(__cdecl* value_callback)();
};
static_assert(sizeof(ai_meter_definition) == 0x8);

struct s_ai_profile_info
{
	short ai_updates;
	short line_of_sight;
	short test_ray2d;
	short firing_position_evaluation;
	short perception_line_of_sight;
};
static_assert(sizeof(s_ai_profile_info) == 0xA);

struct ai_profile_state
{
	bool disable_ai;
	bool move_randomly;
	short render_spray_mode;
	bool show;
	bool show_stats;
	bool show_actors;
	bool show_swarms;
	bool show_paths;
	bool show_line_of_sight;
	bool show_prop_types;
	bool show_sound_distance;
	ai_meter meters[NUMBER_OF_AI_METERS];
	s_ai_profile_info ai_profile_info;
};
static_assert(sizeof(ai_profile_state) == 0xDE8);

extern short global_ai_profile_draw_string_position;
extern char profilestring[2048];
extern ai_profile_state ai_profile;

extern short ai_meter_actor();
extern short ai_meter_prop();
extern short ai_meter_swarm_actor();
extern short ai_meter_swarm_cache();
extern short ai_meter_unit();
extern short ai_profile_change_render_spray();
extern void ai_profile_display(char* textbuffer, long textbuffer_size);
extern void ai_profile_dispose();
extern void ai_profile_dispose_from_old_map();
extern void ai_profile_draw_string(char const* string, short tab_count, short const* tabs, real_argb_color const* color);
extern void ai_profile_initialize();
extern void ai_profile_initialize_for_new_map();
extern void ai_profile_render();
extern void ai_profile_render_spray();
extern void ai_profile_show_actors();
extern void ai_profile_show_line_of_sight();
extern void ai_profile_show_paths();
extern void ai_profile_show_prop_types();
extern void ai_profile_show_swarms();
extern void ai_profile_update();
extern long count_actors(bool active_only);

