#pragma once

#include "cseries/cseries.hpp"

#define MAX_BEHAVIOR_LAYERS 8

enum
{
	_________BEHAVIOR_GROUPING_GENERAL________,
	_root_behavior,
	_null_behavior,
	_null_discrete_behavior,
	_obey_behavior,
	_guard_behavior,
	_follow_behavior,
	_ready_behavior,
	_smash_obstacle_impulse,
	_destroy_obstacle_behavior,
	_perch_behavior,
	_coverfriend_behavior,
	_blind_panic_behavior,
	_combat_behavior,
	_squad_patrol_behavior,

	_________BEHAVIOR_GROUPING_BROKEN________,
	_broken_behavior,
	_huddle_impulse,
	_huddle_behavior,
	_kamikaze_behavior,
	_broken_kamikaze_impulse,
	_broken_berserk_impulse,
	_broken_flee_impulse,
	_broken_scatter_impulse,

	_________BEHAVIOR_GROUPING_ENGAGE________,
	_equipment_impulse,
	_engage_behavior,
	_fight_behavior,
	_fight_positioning_behavior,
	_melee_charge_behavior,
	_melee_leaping_charge_behavior,
	_surprise_behavior,
	_grenade_impulse,
	_anti_vehicle_grenade_impulse,
	_stalk_behavior,
	_flank_behavior,
	_berserk_wander_impulse,
	_stalker_camo_control_impulse,
	_leader_abandoned_charge_impulse,
	_unassailable_enemy_grenade_impulse,
	_perimeter_behavior,
	_perimeter_timeout_component,
	_perimeter_spew_component,

	_________BEHAVIOR_GROUPING_BERSERK________,
	_shield_depleted_berserk_impulse,
	_last_man_berserk_impulse,
	_stuck_with_grenade_berserk_impulse,

	_________BEHAVIOR_GROUPING_PRESEARCH_______,
	_presearch_behavior,
	_presearch_uncover_impulse,
	_destroy_cover_behavior,
	_suppress_behavior,
	_grenade_uncover_impulse,
	_leap_on_cover_behavior,

	_________BEHAVIOR_GROUPING_LEADER________,
	_search_sync_behavior,
	_engage_sync_behavior,

	_________BEHAVIOR_GROUPING_SEARCH________,
	_search_behavior,
	_uncover_behavior,
	_investigate_behavior,
	_pursuit_sync_behavior,
	_find_pursuit_behavior,
	_refresh_target_impulse,
	_sense_target_impulse,
	_postsearch_behavior,
	_coverme_investigate_impulse,

	_________BEHAVIOR_GROUPING_SELF_DEFENSE________,
	_self_preservation_behavior,
	_cover_behavior,
	_cover_peek_behavior,
	_avoid_behavior,
	_evasion_impulse,
	_dive_impulse,
	_danger_cover_impulse,
	_danger_crouch_impulse,
	_proximity_melee_impulse,
	_proximity_self_preserve_impulse,
	_unreachable_enemy_self_preservation_impulse,
	_unassailable_enemy_self_preservation_impulse,
	_scary_target_self_preservation_impulse,
	_emerge_behavior,
	_shield_depleted_cover_impulse,
	_kungfu_cover_impulse,

	_________BEHAVIOR_GROUPING_RETREAT________,
	_retreat_behavior,
	_retreat_grenade_impulse,
	_flee_behavior,
	_cower_behavior,
	_low_shield_retreat_impulse,
	_scary_target_retreat_impulse,
	_leader_dead_retreat_impulse,
	_peer_dead_retreat_impulse,
	_danger_retreat_impulse,
	_proximity_retreat_impulse,
	_charge_when_cornered_impulse,
	_surprise_retreat_impulse,
	_overheated_weapon_retreat_impulse,

	_________BEHAVIOR_GROUPING_AMBUSH________,
	_ambush_behavior,
	_coordinated_ambush_behavior,
	_proximity_ambush_impulse,
	_vulnerable_enemy_ambush_impulse,
	_nowhere_to_run_ambush_impulse,

	_________BEHAVIOR_GROUPING_VEHICLE________,
	_enter_vehicle_behavior,
	_vehicle_enter_friendly_impulse,
	_vehicle_enter_impulse,
	_vehicle_entry_engage_impulse,
	_vehicle_board_behavior,
	_vehicle_fight_behavior,
	_vehicle_fight_boost_component,
	_vehicle_charge_behavior,
	_vehicle_ram_behavior,
	_vehicle_cover_behavior,
	_damage_vehicle_cover_impulse,
	_exposed_rear_cover_impulse,
	_player_endagered_cover_impulse,
	_vehicle_avoid_behavior,
	_vehicle_pickup_behavior,
	_vehicle_pickup_player_behavior,
	_vehicle_exit_impulse,
	_danger_vehicle_exit_impulse,
	_vehicle_flip_impulse,
	_vehicle_flip_behavior,
	_vehicle_turtle_impulse,
	_vehicle_engage_patrol_impulse,
	_vehicle_engage_wander_impulse,

	_________BEHAVIOR_GROUPING_POSTCOMBAT________,
	_postcombat_behavior,
	_postpostcombat_behavior,
	_check_friend_behavior,
	_shoot_corpse_behavior,
	_postcombat_approach_behavior,

	_________BEHAVIOR_GROUPING_ALERT________,
	_alert_behavior,

	_________BEHAVIOR_GROUPING_IDLE________,
	_idle_behavior,
	_inspect_behavior,
	_wander_behavior,
	_flight_wander_behavior,
	_patrol_behavior,
	_sleep_behavior,

	_________BEHAVIOR_GROUPING_BUGGERS________,
	_bugger_group_uncover_behavior,

	_________BEHAVIOR_GROUPING_SWARMS________,
	_swarm_root_behavior,
	_swarm_attack_behavior,
	_support_attack_behavior,
	_infect_impulse,
	_swarm_scatter_behavior,

	_________BEHAVIOR_GROUPING_COMBATFORMS________,
	_combatform_berserk_control_impulse,
	_flood_eject_parasite_behavior,

	_________BEHAVIOR_GROUPING_SENTINELS________,
	_sentinel_enforcer_weapon_control_impulse,
	_grapple_behavior,

	_________BEHAVIOR_GROUPING_ENGINEER________,
	_engineer_control_behavior,
	_engineer_control_slave_behavior,
	_engineer_control_free_behavior,
	_engineer_control_equipment_behavior,
	_engineer_explode_behavior,
	_engineer_broken_detonation_behavior,
	_boost_allies_behavior,

	_________BEHAVIOR_GROUPING_GUARDIANS________,
	_surge_behavior,
	_guardian_proximity_surge_impulse,
	_guardian_danger_surge_impulse,
	_guardian_isolation_surge_impulse,

	_________BEHAVIOR_GROUPING_PUREFORMS________,
	_pureform_group_morph_impulse_behavior,
	_pureform_arrival_morph_impulse_behavior,
	_pureform_pureform_default_impulse_behavior,
	_pureform_search_morph_behavior,
	_pureform_stealth_active_camo_control_behavior,
	_pureform_stealth_damage_morph_behavior,
	_pureform_stealth_stalk_behavior,
	_stalk_thwarted_component,
	_pureform_stealth_stalk_group_behavior,
	_pureform_stealth_charge_recover_behavior,
	_pureform_ranged_proximity_morph_behavior,
	_pureform_tank_distance_damage_morph_behavior,
	_pureform_tank_throttle_control_behavior,
	_pureform_stealth_morph_behavior,
	_pureform_tank_morph_behavior,
	_pureform_ranged_morph_behavior,
	_pureform_ranged_turtle_behavior,
	_pureform_ranged_uncover_behavior,

	_________BEHAVIOR_GROUPING_SCARAB________,
	_scarab_root_behavior,
	_scarab_cure_isolation_behavior,
	_scarab_combat_behavior,
	_scarab_fight_behavior,
	_scarab_target_lock_behavior,
	_scarab_search_behavior,
	_scarab_search_pause_behavior,

	_________BEHAVIOR_GROUPING_ATOMS________,
	_go_to_behavior,

	_________BEHAVIOR_GROUPING_ACTIVITIES___,
	_activity_behavior,
	_posture_behavior,
	_activity_default_behavior,

	_________BEHAVIOR_GROUPING_SPECIAL________,
	_formation_behavior,
	_scared_by_leader_behavior,
	_stunned_behavior,
	_cure_isolation_behavior,
	_deploy_turret_behavior,

	NUMBER_OF_BEHAVIORS
};

//extern const char*(&behavior_names)[NUMBER_OF_BEHAVIORS];
extern const char* behavior_names[NUMBER_OF_BEHAVIORS];

struct actor_datum;

extern int16 __cdecl behavior_index_by_name(char* name);
extern void __cdecl behaviors_initialize();
extern uns8* __cdecl actor_behavior_state_get(const actor_datum* actor, int16 layer_index);

extern int16 actor_behavior_index_get(const actor_datum* actor, int16 layer_index);

