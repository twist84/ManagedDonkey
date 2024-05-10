#include "ai/behavior.hpp"

#include "ai/actors.hpp"

//REFERENCE_DECLARE_ARRAY(0x01994070, char const*, behavior_names, k_behavior_count);

short __cdecl behavior_index_by_name(char* name)
{
	return INVOKE(0x014745C0, behavior_index_by_name, name);
}

//c_behavior_state* __cdecl actor_behavior_state_get(actor_datum const* actor, short layer_index)
byte* __cdecl actor_behavior_state_get(actor_datum const* actor, short layer_index)
{
	return INVOKE(0x01469180, actor_behavior_state_get, actor, layer_index);
}

short actor_behavior_index_get(actor_datum const* actor, short layer_index)
{
	ASSERT(layer_index >= 0);
	ASSERT(layer_index < MAX_BEHAVIOR_LAYERS);
	ASSERT(layer_index <= actor->state.leaf_layer);

	if (VALID_INDEX(layer_index, MAX_BEHAVIOR_LAYERS))
		return *(short*)actor_behavior_state_get(actor, layer_index);

	return NONE;
};

char const* behavior_names[k_behavior_count]
{
	"------GENERAL------",
	"root",
	"null",
	"null_discrete",
	"obey",
	"guard",
	"follow behavior",
	"ready",
	"smash_obstacle",
	"destroy_obstacle",
	"perch",
	"cover_friend",
	"blind_panic",
	"combat",
	"squad_patrol_behavior",

	"------BROKEN------",
	"broken behavior",
	"huddle impulse",
	"huddle behavior",
	"kamikaze behavior",
	"broken kamikaze impulse",
	"broken berserk impulse",
	"broken flee impulse",
	"broken scatter impulse",

	"------ENGAGE------",
	"equipment",
	"engage",
	"fight",
	"fight_positioning",
	"melee_charge",
	"melee_leaping_charge",
	"surprise",
	"grenade_impulse",
	"anti_vehicle_grenade",
	"stalk",
	"flank",
	"berserk_wander_impulse",
	"stalker_camo_control",
	"leader_abandoned_berserk",
	"unassailable grenade impulse",
	"perimeter",
	"perimeter@timeout_morph",
	"perimeter@infection_spew",

	"------BERSERK------",
	"shield_depleted_berserk",
	"last_man_berserk",
	"stuck_with_grenade_berserk",

	"------PRESEARCH------",
	"presearch",
	"presearch uncover",
	"destroy_cover",
	"suppressing fire",
	"grenade_uncover",
	"leap_on_cover",

	"------LEADER------",
	"search_sync",
	"engage_sync",

	"------SEARCH------",
	"search",
	"uncover",
	"investigate",
	"pursuit_sync",
	"pursuit",
	"refresh_target",
	"sense_target",
	"postsearch",
	"coverme_investigate",

	"------SELF-DEFENSE------",
	"self_preservation",
	"cover",
	"cover peek",
	"avoid",
	"evasion_impulse",
	"dive_impulse",
	"danger cover impulse",
	"danger crouch impulse",
	"proximity_melee",
	"proximity_self_preservation",
	"unreachable_enemy_cover",
	"unassailable_enemy_cover",
	"scary_target_cover",
	"group_emerge",
	"shield_depleted_cover",
	"kungfu_cover",

	"------RETREAT------",
	"retreat",
	"retreat_grenade",
	"flee",
	"cower",
	"low_shield_retreat",
	"scary_target_retreat",
	"leader_dead_retreat",
	"peer_dead_retreat",
	"danger_retreat",
	"proximity_retreat",
	"charge when cornered",
	"surprise_retreat",
	"overheated_weapon_retreat",

	"------AMBUSH------",
	"ambush",
	"coordinated_ambush",
	"proximity_ambush",
	"vulnerable_enemy_ambush",
	"nowhere_to_run_ambush",

	"------VEHICLE------",
	"enter_vehicle",
	"enter_friendly_vehicle",
	"vehicle_enter_impulse",
	"vehicle_entry_engage_impulse",
	"vehicle_board",
	"vehicle_fight",
	"vehicle_fight@boost",
	"vehicle_charge",
	"vehicle_ram_behavior",
	"vehicle_cover",
	"damage_vehicle_cover",
	"exposed_rear_cover_impulse",
	"player_endagered_cover_impulse",
	"vehicle_avoid",
	"vehicle_pickup",
	"vehicle_player_pickup",
	"vehicle_exit_impulse",
	"danger_vehicle_exit_impulse",
	"vehicle_flip_impulse",
	"vehicle_flip",
	"vehicle_turtle",
	"vehicle_engage_patrol_impulse",
	"vehicle_engage_wander_impulse",

	"------POSTCOMBAT------",
	"postcombat",
	"post_postcombat",
	"check_friend",
	"shoot_corpse",
	"postcombat_approach",

	"------ALERT------",
	"alert",

	"------IDLE------",
	"idle",
	"inspect",
	"wander behavior",
	"flight_wander",
	"patrol",
	"fall_sleep",

	"------BUGGERS------",
	"bugger_ground_uncover",

	"------SWARMS------",
	"swarm_root",
	"swarm_attack",
	"support_attack",
	"infect",
	"scatter",

	"------COMBATFORMS------",
	"combat form berserk control",
	"eject parasite",

	"------SENTINELS------",
	"enforcer weapon control",
	"grapple",

	"------ENGINEER------",
	"engineer_control",
	"engineer_control@slave",
	"engineer_control@free",
	"engineer_control@equipment",
	"engineer_explode",
	"engineer_broken_detonation",
	"boost_allies",

	"------GUARDIANS------",
	"guardian_surge",
	"guardian_proximity_surge",
	"guardian_danger_surge",
	"guardian_isolation_surge",

	"------PUREFORMS------",
	"group_morph_impulse",
	"arrival_morph_impulse",
	"pureform_default_impulse",
	"search_morph",
	"stealth_active_camo_control",
	"stealth_damage_morph",
	"stealth_stalk",
	"stealth_stalk@thwarted",
	"stealth_stalk_group",
	"stealth_charge_recover",
	"ranged_proximity_morph",
	"tank_distance_damage_morph",
	"tank_throttle_control",
	"stealth_morph",
	"tank_morph",
	"ranged_morph",
	"ranged_turtle",
	"ranged_uncover",

	"------SCARAB------",
	"scarab_root",
	"scarab_cure_isolation",
	"scarab_combat",
	"scarab_fight",
	"scarab_target_lock",
	"scarab_search",
	"scarab_search_pause",

	"------ATOMS------",
	"go_to",

	"------ACTIVITIES------",
	"activity",
	"posture",
	"activity_default",

	"------SPECIAL------",
	"formation",
	"grunt scared by elite",
	"stunned",
	"cure_isolation",
	"deploy_turret"
};

