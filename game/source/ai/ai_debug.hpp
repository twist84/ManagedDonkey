#pragma once

#include "cseries/cseries.hpp"

extern real_point3d global_ai_debug_drawstack_next_position;
extern real global_ai_debug_drawstack_height;
extern real_point3d global_ai_debug_drawstack_last_position;
extern short global_ai_debug_string_position;
extern bool ai_render;
extern bool ai_render_firing_positions_all;
extern bool ai_render_lineoffire;
extern bool ai_render_lineofsight;
extern bool ai_render_ballistic_lineoffire;
extern long ai_debug_selected_actor_unit_index;
extern long ai_debug_selected_actor_index;
extern bool ai_debug_path;
extern bool ai_render_paths_failed;
extern bool ai_render_aiming_validity;
extern bool ai_render_all_actors;
extern bool ai_render_dialogue;
extern bool ai_render_dialogue_queue;
extern bool ai_render_dialogue_player_weights;
extern bool ai_render_speech;
extern bool ai_print_speech;
extern bool ai_render_spatial_effects;
extern bool ai_render_clumps;
extern bool ai_render_clump_props;
extern bool ai_render_clump_props_all;
extern bool ai_render_decisions;
extern bool ai_render_decisions_all;
extern bool ai_render_behavior_stack_all;
extern bool ai_render_stimuli;
extern bool ai_render_sectors;
extern bool ai_render_sector_geometry_errors;
extern long ai_render_link_specific;
extern bool ai_render_links;
extern bool ai_render_user_hints;
extern bool ai_render_hints;
extern bool ai_render_object_hints_all;
extern bool ai_render_object_hints;
extern bool ai_render_object_properties;
extern bool ai_render_sector_bsps;
extern bool ai_render_giant_sector_bsps;
extern bool ai_render_sector_link_errors;
extern bool ai_render_intersection_links;
extern bool ai_render_non_walkable_sectors;
extern bool ai_render_threshold_links;
extern bool ai_render_orders;
extern bool ai_render_suppress_combat;
extern bool ai_render_objectives;
extern bool ai_render_strength;
extern bool ai_debug_tracking_data;
extern bool ai_debug_perception_data;
extern bool ai_debug_combat_status;
extern bool ai_render_tracked_props_all;
extern bool ai_render_targets_all;
extern bool ai_render_joint_behaviors;
extern bool ai_render_flocks;
extern bool ai_render_command_scripts;
extern bool ai_render_dialogue_variants;
extern bool ai_render_vehicle_interest;
extern bool ai_render_player_battle_vector;
extern bool ai_render_player_needs_vehicle;
extern bool ai_render_mission_critical;

extern void __cdecl ai_debug_render();

extern void ai_debug_drawstack_setup(real_point3d const* position);
extern real_point3d* ai_debug_drawstack();
extern real_point3d* ai_debug_drawstack_offset(real offset);

extern void debug_combat_status();
extern void render_dialogue_variants();

