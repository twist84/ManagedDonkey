#pragma once

#include "ai/joint_behavior.hpp"
#include "cseries/cseries.hpp"

struct prop_ref_datum :
	s_datum_header
{
	long __cdecl get_object_index() const
	{
		return object_index;
	}

	char type;
	char dialogue_event;
	char dialogue_index;
	uint8 dialogue_ticks;
	char stimulus;
	uint8 stimulus_decay_ticks;
	long actor_index;
	long prop_index;
	long object_index;
	long tracking_index;
	real32 awareness;
	char status;
	char perception;
	char visibility : 4;
	char line_of_sight : 4;
	uint8 flags;
	short status_refresh_timer;
	short state_refresh_timer;
	long actor_next_prop_ref_index;
	long actor_prev_prop_ref_index;
	long clump_next_prop_ref_index;
	long clump_prev_prop_ref_index;
	real32 distance;
	real32 salience;
};
static_assert(sizeof(prop_ref_datum) == 0x3C);

struct zone_area_ref
{
	short zone_index;
	short area_index;
};
static_assert(sizeof(zone_area_ref) == 0x4);

struct prop_state
{
	long last_update_time;
	real_point3d body_position;
	real_point3d center_of_mass;
	real_vector3d velocity;
	real_vector3d aiming;
	s_location body_location;
	real_point3d head_position;
	long vehicle_index;
	c_sector_ref pathfinding_sector_ref;
	c_ai_point3d pathfinding_point;
	long death_time;
	long posture;
	bool dead;
	bool noncombat;
	bool in_combat;
	bool fighting;
	bool shooting;
	bool flying;
	bool vehicle_gunner;
	bool dangerous_vehicle_driver;
	bool preferred_target;
	bool underwater;
	bool pathfinding_data_valid;
};
static_assert(sizeof(prop_state) == 0x70);

struct prop_datum :
	s_datum_header
{
	short type;
	short status;
	long object_index;
	real32 importance;
	real32 salience;
	short prop_class;
	long forced_retention_time;
	long next_prop_index;
	long first_prop_ref_index;
	long actor_index;
	short team_index;
	bool swarm;
	bool enemy;
	bool ally;
	bool player;
	bool ignore;
	short flags;
	uint8 dialogue_ability_flags;
	uint8 dialogue_completed_flags;
	short magic_sight_ticks;
	short comment_timer;
	short remind_timer;
	zone_area_ref discarded_pursuit_areas[5];
	char current_pursuit_index;
	char num_discarded_areas;
	zone_area_ref pursuit_area;
	prop_state state;
};
static_assert(sizeof(prop_datum) == 0xC4);

struct prop_view
{
	short flags;
	short prop_class;
	short audibility;
	short ineffability;
	short line_of_sight;
	short visible_ticks;
	long last_perceived_time;
	long last_visible_time;
	long first_sighted_time;
	c_ai_point3d last_visible_position;
	real_vector3d actor_to_prop;
	char quantized_facing;
	long last_unreachable_time;
	long unassailable_reset_time;
	real32 awareness_of_me;
	real32 scariness;
	short advancing_ticks;
	short search_stage;
	pursuit_location pursuit_location;
	short orphan_visibility;
	short orphan_los;
	short orphan_lifespan_ticks;
	short orphan_inspection_ticks;
	real_vector3d orphan_hint_vector;
	short ticks_until_orphan;
	long orphaned_time;
	real_point3d last_new_position;
};
static_assert(sizeof(prop_view) == 0x8C);

struct tracking_datum :
	s_datum_header
{
	prop_state state;
	prop_view view;
};
static_assert(sizeof(tracking_datum) == 0x100);

struct actor_prop_ref_iterator
{
	long index;
	long next_index;
};
static_assert(sizeof(actor_prop_ref_iterator) == 0x8);

extern prop_state* __cdecl prop_state_get(prop_ref_datum const* pref);
extern void __cdecl props_dispose();
extern void __cdecl props_dispose_from_old_map();
extern void __cdecl props_initialize();
extern void __cdecl props_initialize_for_new_map();
extern void __cdecl actor_prop_ref_iterator_new(actor_prop_ref_iterator* iterator, long actor_index);
extern prop_ref_datum* __cdecl actor_prop_ref_iterator_next(actor_prop_ref_iterator* iterator);

