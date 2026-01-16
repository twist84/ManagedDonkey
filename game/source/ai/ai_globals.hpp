#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct s_ai_globals_gravemind_definition;
struct s_ai_globals_styles_definition;
struct s_ai_globals_formation_definition;
struct s_ai_globals_squad_template_definition;
struct s_ai_globals_definition
{
	real32 ai_infantry_on_ai_weapon_damage_scale;
	real32 ai_vehicle_on_ai_weapon_damage_scale;
	real32 ai_player_vehicle_on_ai_weapon_damage_scale;
	real32 danger_broadly_facing;
	real32 danger_shooting_near;
	real32 danger_shooting_at;
	real32 danger_extremely_close;
	real32 danger_shield_damage;
	real32 danger_exetended_shield_damage;
	real32 danger_body_damage;
	real32 danger_extended_body_damage;
	c_typed_tag_reference<AI_DIALOGUE_GLOBALS_TAG, INVALID_TAG> global_dialogue;
	c_string_id default_mission_dialogue_sound_effect_id;
	real32 jump_speeds[6];
	real32 jump_down_heights[6];
	real32 climb_ranges[3][2];
	real32 vault_ranges[2][2];

	//PATHFINDING SEARCH RANGES
	// 
	// The maximum ranges to which firing point evaluations will do pathfinding searches. INCREASING THESE VALUES WILL ALMOST CERTAINLY HAVE A NEGATIVE IMPACT ON PERFORMANCE.
	real32 search_range_infantry;
	real32 search_range_flying;
	real32 search_range_vehicle;
	real32 search_range_giant;
	c_typed_tag_block<s_ai_globals_gravemind_definition> gravemind_properties;
	real32 scary_target_threhold;
	real32 scary_weapon_threhold;
	real32 player_scariness;
	real32 berserking_actor_scariness;
	real32 kamikazeing_actor_scariness;
	real32 invincible_scariness;
	real32 morph_delays[3];

	// RESURRECTION
	// 
	// A few properties to help define when it is safe for a mission critical character to ressurect.
	real32 min_death_time;
	real32 projectile_distance;
	real32 idle_clump_distance;
	real32 dangerous_clump_distance;
	c_typed_tag_block<s_ai_globals_styles_definition> styles;

	// Added ODST

	c_typed_tag_block<s_ai_globals_formation_definition> spawn_formations;
	c_typed_tag_block<s_ai_globals_squad_template_definition> squad_templates;

	// CLUMP THROTTLING
	// 
	// Some values to help you control how much guys will throttle when they want to stick with the rest of their squad.
	real32 stop_dist;
	real32 resume_dist;
	real32 min_dist;
	real32 max_dist;
	real32 min_scale;

	// SQUAD PATROLLING
	// 
	// Parameters related to squad patrolling.
	real32 passthrough_chance;
	real32 search_phase_skip_chance;
	real32 patrol_transition_timeout;
	real32 patrol_maneuver_timeout;
	real_bounds patrol_search_firing_point_time;
	real32 patrol_isolation_distance;
	real32 patrol_isolation_time;

	// KUNGFU CONROL
	// 
	// These parameters control how the kung - fu circle works(i.e.when tasks have kungfu - count associated with them)
	real32 kungfu_deactivation_delay;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_ai_globals_definition) == 0x144);

struct s_ai_globals_gravemind_definition
{
	real32 min_retreat_time;
	real32 ideal_retreat_time;
	real32 max_retreat_time;
};
COMPILE_ASSERT(sizeof(s_ai_globals_gravemind_definition) == 0xC);

struct s_ai_globals_styles_definition
{
	c_typed_tag_reference<STYLE_TAG, INVALID_TAG> style;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_ai_globals_styles_definition) == 0x10);

struct s_ai_globals_formation_definition
{
	c_typed_tag_reference<FORMATION_TAG, INVALID_TAG> formation;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_ai_globals_formation_definition) == 0x10);

struct s_ai_globals_squad_template_definition
{
	c_typed_tag_reference<SQUAD_TEMPLATE_TAG, INVALID_TAG> squad_template;

	void update_reference_names();
};
COMPILE_ASSERT(sizeof(s_ai_globals_squad_template_definition) == 0x10);

