#pragma once

#include "ai/actor_dynamic_firing_position.hpp"
#include "ai/actors.hpp"
#include "ai/ai_dialogue.hpp"
#include "ai/ai_flocks.hpp"
#include "ai/ai_player.hpp"
#include "ai/clump.hpp"
#include "ai/formations.hpp"
#include "ai/joint_behavior.hpp"
#include "ai/objectives.hpp"
#include "ai/props.hpp"
#include "ai/squad_patrol.hpp"
#include "ai/squads.hpp"
#include "ai/swarms.hpp"
#include "cseries/cseries.hpp"

struct ai_globals_type
{
	bool ai_active;
	bool ai_initialized_for_map;
	bool ai_has_control_data;
	bool selected_firing_position_this_tick;
	bool built_path_this_tick;
	bool fast_and_dumb_ai;
	short last_highest_pathfinding_service_timer;
	short current_highest_pathfinding_service_timer;
	bool pathfinding_time_given_this_frame;
	short last_highest_perception_service_timer;
	short current_highest_perception_service_timer;
	bool perception_time_given_this_frame;
	short current_objective_index;
	long first_squadless_actor_index;
	long highest_clump_refresh_weight;
	real major_upgrade_error;
	bool dialogue_enabled;
	byte __data21[31];
	word __unknown40;
	word __unknown42;
	byte __unknown44[0x380];
	byte __data3C4[0x2C4];
};
static_assert(sizeof(ai_globals_type) == 0x688);

extern void __cdecl ai_dispose();
extern void __cdecl ai_dispose_from_old_map();
extern void __cdecl ai_dispose_from_old_structure_bsp(dword deactivating_structure_bsp_mask);
extern bool __cdecl ai_enemies_can_see_player(long* out_unit_index);
extern void __cdecl ai_erase(long squad_index, bool delete_immediately);
extern bool __cdecl ai_get_active_clusters(long a1, dword* a2, long a3);
extern void __cdecl ai_globals_set_ai_active(bool ai_active);
extern void __cdecl ai_handle_bump(long object_index, long bump_object_index, vector3d const* linear_velocity);
extern void __cdecl ai_initialize();
extern void __cdecl ai_initialize_for_new_map();
extern void __cdecl ai_initialize_for_new_structure_bsp(dword activating_structure_bsp_mask);
extern void __cdecl ai_update();

