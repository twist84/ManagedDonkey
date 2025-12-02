#pragma once

#include "ai/activities.hpp"
#include "ai/actor_dynamic_firing_position.hpp"
#include "ai/actor_moving_flying_vehicle_obstacle_avoidance.hpp"
#include "ai/actors.hpp"
#include "ai/ai_dialogue.hpp"
#include "ai/ai_flocks.hpp"
#include "ai/ai_player.hpp"
#include "ai/ai_profile.hpp"
#include "ai/ai_script.hpp"
#include "ai/behavior.hpp"
#include "ai/clump.hpp"
#include "ai/event_handling.hpp"
#include "ai/formations.hpp"
#include "ai/joint_behavior.hpp"
#include "ai/objectives.hpp"
#include "ai/path.hpp"
#include "ai/props.hpp"
#include "ai/squad_patrol.hpp"
#include "ai/squads.hpp"
#include "ai/swarms.hpp"
#include "cseries/cseries.hpp"
#include "render/render_visibility_collection.hpp"

struct ai_mission_critical_actor
{
	int32 object_index;
	int32 character_definition_index;
	int16 flags;
	int16 teleport_ticks;
	real32 teleport_range;
};
static_assert(sizeof(ai_mission_critical_actor) == 0x10);

struct ai_globals_type
{
	bool ai_active;
	bool ai_initialized_for_map;
	bool ai_has_control_data;
	bool selected_firing_position_this_tick;
	bool built_path_this_tick;
	bool fast_and_dumb_ai;
	int16 last_highest_pathfinding_service_timer;
	int16 current_highest_pathfinding_service_timer;
	bool pathfinding_time_given_this_frame;
	int16 last_highest_perception_service_timer;
	int16 current_highest_perception_service_timer;
	bool perception_time_given_this_frame;
	int16 current_objective_index;
	int32 first_squadless_actor_index;
	int32 highest_clump_refresh_weight;
	real32 major_upgrade_error;
	bool dialogue_enabled;
	int16 dialogue_suppression_ticks;
	bool ai_player_dialogue_enable;
	int32 last_chatter_time[2];
	int32 last_talk_time[2];
	int32 last_shout_time[2];
	int16 spatial_effects_first_index;
	int16 spatial_effects_last_index;
	ai_sound_data spatial_effects[32];
	bool grenades_enabled;
	int32 infection_allowed_time;
	int16 mounted_weapon_unit_count;
	int32 mounted_weapon_unit_indices[8];
	int32 last_squad_placed;
	int16 perception_refresh_last_index;
	int16 active_ai_count;

	int32 __unknown3F8;

	int32 last_player_shooting_time;
	int32 last_prop_track_cleanup_time;
	ai_mission_critical_actor mission_critical_units[8];
	s_cluster_reference cached_cluster_ref;
	s_game_cluster_bit_vectors cached_pas;
};
static_assert(sizeof(ai_globals_type) == 0x688);
static_assert(0x000 == OFFSETOF(ai_globals_type, ai_active));
static_assert(0x001 == OFFSETOF(ai_globals_type, ai_initialized_for_map));
static_assert(0x002 == OFFSETOF(ai_globals_type, ai_has_control_data));
static_assert(0x003 == OFFSETOF(ai_globals_type, selected_firing_position_this_tick));
static_assert(0x004 == OFFSETOF(ai_globals_type, built_path_this_tick));
static_assert(0x005 == OFFSETOF(ai_globals_type, fast_and_dumb_ai));
static_assert(0x006 == OFFSETOF(ai_globals_type, last_highest_pathfinding_service_timer));
static_assert(0x008 == OFFSETOF(ai_globals_type, current_highest_pathfinding_service_timer));
static_assert(0x00A == OFFSETOF(ai_globals_type, pathfinding_time_given_this_frame));
static_assert(0x00C == OFFSETOF(ai_globals_type, last_highest_perception_service_timer));
static_assert(0x00E == OFFSETOF(ai_globals_type, current_highest_perception_service_timer));
static_assert(0x010 == OFFSETOF(ai_globals_type, perception_time_given_this_frame));
static_assert(0x012 == OFFSETOF(ai_globals_type, current_objective_index));
static_assert(0x014 == OFFSETOF(ai_globals_type, first_squadless_actor_index));
static_assert(0x018 == OFFSETOF(ai_globals_type, highest_clump_refresh_weight));
static_assert(0x01C == OFFSETOF(ai_globals_type, major_upgrade_error));
static_assert(0x020 == OFFSETOF(ai_globals_type, dialogue_enabled));
static_assert(0x022 == OFFSETOF(ai_globals_type, dialogue_suppression_ticks));
static_assert(0x024 == OFFSETOF(ai_globals_type, ai_player_dialogue_enable));
static_assert(0x028 == OFFSETOF(ai_globals_type, last_chatter_time));
static_assert(0x030 == OFFSETOF(ai_globals_type, last_talk_time));
static_assert(0x038 == OFFSETOF(ai_globals_type, last_shout_time));
static_assert(0x040 == OFFSETOF(ai_globals_type, spatial_effects_first_index));
static_assert(0x042 == OFFSETOF(ai_globals_type, spatial_effects_last_index));
static_assert(0x044 == OFFSETOF(ai_globals_type, spatial_effects));
static_assert(0x3C4 == OFFSETOF(ai_globals_type, grenades_enabled));
static_assert(0x3C8 == OFFSETOF(ai_globals_type, infection_allowed_time));
static_assert(0x3CC == OFFSETOF(ai_globals_type, mounted_weapon_unit_count));
static_assert(0x3D0 == OFFSETOF(ai_globals_type, mounted_weapon_unit_indices));
static_assert(0x3F0 == OFFSETOF(ai_globals_type, last_squad_placed));
static_assert(0x3F4 == OFFSETOF(ai_globals_type, perception_refresh_last_index));
static_assert(0x3F6 == OFFSETOF(ai_globals_type, active_ai_count));
static_assert(0x3F8 == OFFSETOF(ai_globals_type, __unknown3F8));
static_assert(0x3FC == OFFSETOF(ai_globals_type, last_player_shooting_time));
static_assert(0x400 == OFFSETOF(ai_globals_type, last_prop_track_cleanup_time));
static_assert(0x404 == OFFSETOF(ai_globals_type, mission_critical_units));
static_assert(0x484 == OFFSETOF(ai_globals_type, cached_cluster_ref));
static_assert(0x488 == OFFSETOF(ai_globals_type, cached_pas));

extern int32 __cdecl actor_endangering_player(bool must_be_attacking, bool build_player_mask, uns32* player_mask_out);
extern void __cdecl ai_debug_update();
extern void __cdecl ai_dispose();
extern void __cdecl ai_dispose_from_old_map();
extern void __cdecl ai_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask);
extern bool __cdecl ai_enemies_attacking_players(int32* attacking_object_index, uns32* player_mask_out);
extern bool __cdecl ai_enemies_can_see_player(int32* object_index);
extern void __cdecl ai_erase(int32 squad_index, bool delete_immediately);
extern bool __cdecl ai_get_active_clusters(int32 structure_bsp_index, uns32* activation_bitvector, int32 cluster_count);
extern void __cdecl ai_globals_initialize();
extern void __cdecl ai_globals_initialize_for_new_map();
extern void __cdecl ai_globals_set_ai_active(bool enable);
extern void __cdecl ai_globals_update();
extern void __cdecl ai_handle_bump(int32 biped_index, int32 object_index, const real_vector3d* old_velocity);
extern void __cdecl ai_handle_noisemaker(int32 equipment_index);
extern void __cdecl ai_initialize();
extern void __cdecl ai_initialize_for_new_map();
extern void __cdecl ai_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);
extern void __cdecl ai_place(int32 game_mode);
extern void __cdecl ai_reset();
extern bool __cdecl ai_test_line_of_fire(int32 actor_index, int32 ignore_unit_index, const real_point3d* origin, const real_vector3d* vector, int32* prop_index_reference);
extern int16 __cdecl ai_test_line_of_sight(const real_point3d* p0, s_cluster_reference p0_cluster_ref, const real_point3d* p1, s_cluster_reference p1_cluster_ref, int16 mode, bool test_line_of_fire, int32 ignore_object_index, int32 ignore_object_index2, bool ignore_vehicles, bool allow_early_out, int32* blocking_object_index_ref, bool* two_sided_obstruction_ref);
extern void __cdecl ai_update();
extern void __cdecl ai_verify_tags();

