#pragma once

#include "ai/joint_behavior.hpp"
#include "cseries/cseries.hpp"

struct prop_ref_datum :
	s_datum_header
{
	int32 __cdecl get_object_index() const
	{
		return object_index;
	}

	int8 type;
	int8 dialogue_event;
	int8 dialogue_index;
	uns8 dialogue_ticks;
	int8 stimulus;
	uns8 stimulus_decay_ticks;
	int32 actor_index;
	int32 prop_index;
	int32 object_index;
	int32 tracking_index;
	real32 awareness;
	int8 status;
	int8 perception;
	int8 visibility : 4;
	int8 line_of_sight : 4;
	uns8 flags;
	int16 status_refresh_timer;
	int16 state_refresh_timer;
	int32 actor_next_prop_ref_index;
	int32 actor_prev_prop_ref_index;
	int32 clump_next_prop_ref_index;
	int32 clump_prev_prop_ref_index;
	real32 distance;
	real32 salience;
};
COMPILE_ASSERT(sizeof(prop_ref_datum) == 0x3C);

struct zone_area_ref
{
	int16 zone_index;
	int16 area_index;
};
COMPILE_ASSERT(sizeof(zone_area_ref) == 0x4);

struct prop_state
{
	int32 last_update_time;
	real_point3d body_position;
	real_point3d center_of_mass;
	real_vector3d velocity;
	real_vector3d aiming;
	s_location body_location;
	real_point3d head_position;
	int32 vehicle_index;
	c_sector_ref pathfinding_sector_ref;
	c_ai_point3d pathfinding_point;
	int32 death_time;
	int32 posture;
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
COMPILE_ASSERT(sizeof(prop_state) == 0x70);

struct prop_datum :
	s_datum_header
{
	int16 type;
	int16 status;
	int32 object_index;
	real32 importance;
	real32 salience;
	int16 prop_class;
	int32 forced_retention_time;
	int32 next_prop_index;
	int32 first_prop_ref_index;
	int32 actor_index;
	int16 team_index;
	bool swarm;
	bool enemy;
	bool ally;
	bool player;
	bool ignore;
	int16 flags;
	uns8 dialogue_ability_flags;
	uns8 dialogue_completed_flags;
	int16 magic_sight_ticks;
	int16 comment_timer;
	int16 remind_timer;
	zone_area_ref discarded_pursuit_areas[5];
	int8 current_pursuit_index;
	int8 num_discarded_areas;
	zone_area_ref pursuit_area;
	prop_state state;
};
COMPILE_ASSERT(sizeof(prop_datum) == 0xC4);

struct prop_view
{
	int16 flags;
	int16 prop_class;
	int16 audibility;
	int16 ineffability;
	int16 line_of_sight;
	int16 visible_ticks;
	int32 last_perceived_time;
	int32 last_visible_time;
	int32 first_sighted_time;
	c_ai_point3d last_visible_position;
	real_vector3d actor_to_prop;
	int8 quantized_facing;
	int32 last_unreachable_time;
	int32 unassailable_reset_time;
	real32 awareness_of_me;
	real32 scariness;
	int16 advancing_ticks;
	int16 search_stage;
	pursuit_location pursuit_location;
	int16 orphan_visibility;
	int16 orphan_los;
	int16 orphan_lifespan_ticks;
	int16 orphan_inspection_ticks;
	real_vector3d orphan_hint_vector;
	int16 ticks_until_orphan;
	int32 orphaned_time;
	real_point3d last_new_position;
};
COMPILE_ASSERT(sizeof(prop_view) == 0x8C);

struct tracking_datum :
	s_datum_header
{
	prop_state state;
	prop_view view;
};
COMPILE_ASSERT(sizeof(tracking_datum) == 0x100);

struct actor_prop_ref_iterator
{
	int32 index;
	int32 next_index;
};
COMPILE_ASSERT(sizeof(actor_prop_ref_iterator) == 0x8);

extern prop_state* __cdecl prop_state_get(const prop_ref_datum* pref);
extern void __cdecl props_dispose();
extern void __cdecl props_dispose_from_old_map();
extern void __cdecl props_initialize();
extern void __cdecl props_initialize_for_new_map();
extern void __cdecl actor_prop_ref_iterator_new(actor_prop_ref_iterator* iterator, int32 actor_index);
extern prop_ref_datum* __cdecl actor_prop_ref_iterator_next(actor_prop_ref_iterator* iterator);

