#include "ai/behavior.hpp"

#include "ai/actors.hpp"

// ai/behavior/behavior_activity.inl
// ai/behavior/behavior_avoid.inl
// ai/behavior/behavior_broken.inl
// ai/behavior/behavior_charge.inl
// ai/behavior/behavior_cover.inl
// ai/behavior/behavior_cower.inl
// ai/behavior/behavior_cure_isolation.inl
// ai/behavior/behavior_deploy_turret.inl
// ai/behavior/behavior_engage.inl
// ai/behavior/behavior_engineer.inl
// ai/behavior/behavior_equipment.inl
// ai/behavior/behavior_fight.inl
// ai/behavior/behavior_flank.inl
// ai/behavior/behavior_flee.inl
// ai/behavior/behavior_formation.inl
// ai/behavior/behavior_grapple.inl
// ai/behavior/behavior_guard.inl
// ai/behavior/behavior_investigate.inl
// ai/behavior/behavior_patrol.inl
// ai/behavior/behavior_perch.inl
// ai/behavior/behavior_perimeter.inl
// ai/behavior/behavior_postcombat.inl
// ai/behavior/behavior_posture.inl
// ai/behavior/behavior_presearch.inl
// ai/behavior/behavior_pursuit.inl
// ai/behavior/behavior_ready.inl
// ai/behavior/behavior_retreat.inl
// ai/behavior/behavior_search.inl
// ai/behavior/behavior_self_preserve.inl
// ai/behavior/behavior_squad.inl
// ai/behavior/behavior_stalk.inl
// ai/behavior/behavior_stunned.inl
// ai/behavior/behavior_suppress.inl
// ai/behavior/behavior_surprise.inl
// ai/behavior/behavior_uncover.inl
// ai/behavior/behavior_vehicle.inl
// ai/behavior/behavior_vehicle_board.inl
// ai/behavior/behavior_vehicle_charge.inl
// ai/behavior/behavior_vehicle_cover.inl
// ai/behavior/behavior_vehicle_exit.inl
// ai/behavior/behavior_vehicle_fight.inl
// ai/behavior/behavior_vehicle_flip.inl
// ai/behavior/behavior_vehicle_pickup.inl
// ai/behavior/behavior_wander.inl

//REFERENCE_DECLARE_ARRAY(0x01994070, char const*, behavior_names, k_behavior_count);

//.text:01466970 ; 
//.text:01466990 ; 
//.text:014669A0 ; 
//.text:01466A20 ; 
//.text:01466AA0 ; 
//.text:01466AD0 ; public: __cdecl Behavior_impulse::Behavior_impulse(short, long, short(__cdecl*)(long, c_behavior_state*))
//.text:01466B00 ; public: __cdecl Mutex_group::Mutex_group(short, long, short(__cdecl*)(long, s_behavior_input const*, s_behavior_arguments const*), short(__cdecl*)(long, c_behavior_state*, c_behavior_state*), bool(__cdecl*)(long, c_behavior_state*), void(__cdecl*)(long, c_behavior_state*), short, short(__cdecl*)(long, short, c_behavior_state*), short, child_entry*, void(__cdecl*)(c_behavior_state*))
//.text:01466B90 ; public: __cdecl Mutex_group::Mutex_group(short, long, short(__cdecl*)(long, s_behavior_input const*, s_behavior_arguments const*), short(__cdecl*)(long, c_behavior_state*, c_behavior_state*), bool(__cdecl*)(long, c_behavior_state*), void(__cdecl*)(long, c_behavior_state*), short, short(__cdecl*)(long, short, c_behavior_state*), short, child_entry*, void(__cdecl*)(long, c_behavior_state*, long), void(__cdecl*)(long, c_behavior_state*, long), void(__cdecl*)(long, c_behavior_state*, long), void(__cdecl*)(long, c_behavior_state*), void(__cdecl*)(long, c_behavior_state*, bool), void(__cdecl*)(long, c_behavior_state*), void(__cdecl*)(c_behavior_state*))
//.text:01466C20 ; public: __cdecl Primitive_behavior::Primitive_behavior(short, long, short(__cdecl*)(long, s_behavior_input const*, s_behavior_arguments const*), short(__cdecl*)(long, c_behavior_state*, c_behavior_state*), bool(__cdecl*)(long, c_behavior_state*), void(__cdecl*)(long, c_behavior_state*), short, bool(__cdecl*)(long, c_behavior_state*), void(__cdecl*)(long, c_behavior_state*), void(__cdecl*)(c_behavior_state*))
//.text:01466CA0 ; public: __cdecl Primitive_behavior::Primitive_behavior(short, long, short(__cdecl*)(long, s_behavior_input const*, s_behavior_arguments const*), short(__cdecl*)(long, c_behavior_state*, c_behavior_state*), bool(__cdecl*)(long, c_behavior_state*), void(__cdecl*)(long, c_behavior_state*), short, bool(__cdecl*)(long, c_behavior_state*), void(__cdecl*)(long, c_behavior_state*), void(__cdecl*)(long, c_behavior_state*, long), void(__cdecl*)(long, c_behavior_state*, long), void(__cdecl*)(long, c_behavior_state*, long), void(__cdecl*)(long, c_behavior_state*), void(__cdecl*)(long, c_behavior_state*, bool), void(__cdecl*)(long, c_behavior_state*), void(__cdecl*)(c_behavior_state*))
//.text:01466D20 ; 
//.text:01466D30 ; bool __cdecl action_flee_current_position_exposed(long, c_flee_state_data*)
//.text:01466ED0 ; void __cdecl action_flee_find_flee_position(long, bool, c_flee_state_data*)
//.text:01467110 ; void __cdecl action_investigate_control(long, c_behavior_state*)
//.text:01467270 ; bool __cdecl action_investigate_perform(long, c_behavior_state*)
//.text:014677F0 ; bool __cdecl action_postsearch_perform(long, c_behavior_state*)
//.text:01467900 ; void __cdecl action_selection(long)
//.text:01467C40 ; void __cdecl action_uncover_control(long, c_behavior_state*)
//.text:01467E20 ; bool __cdecl action_uncover_perform(long, c_behavior_state*)
//.text:01468130 ; bool __cdecl actor_action_find_escape_from_danger(long, short*, real*, vector2d*, bool*)
//.text:01468670 ; bool __cdecl actor_action_try_to_dive(long, short, real, vector2d*, real)
//.text:01468880 ; bool __cdecl actor_assess_danger(long, real, bool*, bool*, bool*, bool*, real*)
//.text:01468EC0 ; void __cdecl actor_attach_equipment(long)
//.text:01468F90 ; Behavior_header* __cdecl actor_behavior_get(actor_datum*, short)
//.text:01468FE0 ; 
//.text:01469010 ; 
//.text:014690C0 ; 
//.text:014690F0 ; 

//c_behavior_state* __cdecl actor_behavior_state_get(actor_datum const* actor, short layer_index)
byte* __cdecl actor_behavior_state_get(actor_datum const* actor, short layer_index)
{
	return INVOKE(0x01469180, actor_behavior_state_get, actor, layer_index);
}

//.text:014691B0 ; short __cdecl actor_behavior_state_offset_get(actor_datum*, short)
//.text:014691E0 ; bool __cdecl actor_can_grapple(long)
//.text:01469270 ; bool __cdecl actor_executing(long, short)
//.text:01469290 ; short __cdecl actor_executing_layer(long, short)
//.text:01469320 ; void __cdecl actor_get_desired_follow_range(long, long, real*, real*)
//.text:014693B0 ; 
//.text:014693F0 ; void __cdecl actor_request_search(long)
//.text:01469460 ; bool __cdecl actor_search_allowed(long)
//.text:01469560 ; bool __cdecl actor_should_run(long)
//.text:014695D0 ; bool __cdecl actor_should_search(long)
//.text:01469640 ; void __cdecl actor_throw_equipment(long, short)
//.text:01469A70 ; void __cdecl ai_handle_deployed_turret(long, long)
//.text:01469B10 ; 
//.text:01469B50 ; bool __cdecl behavior_activity_activate(long, c_behavior_state*)
//.text:01469C70 ; bool __cdecl behavior_activity_command_script_allows_activity(long)
//.text:01469CF0 ; void __cdecl behavior_activity_deactivate(long, c_behavior_state*)
//.text:01469D90 ; short __cdecl behavior_activity_decide(long, short, c_behavior_state*)
//.text:01469E60 ; bool __cdecl behavior_activity_default_activate(long, c_behavior_state*)
//.text:01469ED0 ; void __cdecl behavior_activity_default_control(long, c_behavior_state*)
//.text:01469F70 ; void __cdecl behavior_activity_default_deactivate(long, c_behavior_state*)
//.text:01469FD0 ; short __cdecl behavior_activity_default_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0146A000 ; short __cdecl behavior_activity_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0146A060 ; short __cdecl behavior_activity_next(long, c_behavior_state*, c_behavior_state*)
//.text:0146A340 ; bool __cdecl behavior_alert_activate(long, c_behavior_state*)
//.text:0146A390 ; short __cdecl behavior_alert_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0146A3A0 ; short __cdecl behavior_alert_next(long, c_behavior_state*, c_behavior_state*)
//.text:0146A3B0 ; bool __cdecl behavior_allowed_to_investigate(long, long)
//.text:0146A500 ; bool __cdecl behavior_ambush_activate(long, c_behavior_state*)
//.text:0146A510 ; void __cdecl behavior_ambush_deactivate(long, c_behavior_state*)
//.text:0146A520 ; short __cdecl behavior_ambush_decide(long, short, c_behavior_state*)
//.text:0146A550 ; short __cdecl behavior_ambush_next(long, c_behavior_state*, c_behavior_state*)
//.text:0146A560 ; bool __cdecl behavior_avoid_activate(long, c_behavior_state*)
//.text:0146A5B0 ; void __cdecl behavior_avoid_control(long, c_behavior_state*)
//.text:0146A6D0 ; 
//.text:0146ACC0 ; short __cdecl behavior_avoid_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0146AD60 ; short __cdecl behavior_avoid_next(long, c_behavior_state*, c_behavior_state*)
//.text:0146ADC0 ; bool __cdecl behavior_avoid_perform(long, c_behavior_state*)
//.text:0146ADD0 ; bool __cdecl behavior_board_activate(long, c_behavior_state*)
//.text:0146AE30 ; void __cdecl behavior_board_control(long, c_behavior_state*)
//.text:0146AEA0 ; void __cdecl behavior_board_deactivate(long, c_behavior_state*)
//.text:0146AF90 ; void __cdecl behavior_board_flush_object(long, c_behavior_state*, long)
//.text:0146AFB0 ; void __cdecl behavior_board_flush_prop(long, c_behavior_state*, long)
//.text:0146AFD0 ; short __cdecl behavior_board_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0146B160 ; short __cdecl behavior_board_next(long, c_behavior_state*, c_behavior_state*)
//.text:0146B320 ; bool __cdecl behavior_board_perform(long, c_behavior_state*)
//.text:0146B6A0 ; bool __cdecl behavior_broken_activate(long, c_behavior_state*)
//.text:0146B6C0 ; short __cdecl behavior_broken_decide(long, short, c_behavior_state*)
//.text:0146B6D0 ; short __cdecl behavior_broken_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0146B6E0 ; long __cdecl behavior_build_condition_flags(long)
//.text:0146B7A0 ; bool __cdecl behavior_change(long, short, short, c_behavior_state*)
//.text:0146B980 ; bool __cdecl behavior_check_friend_activate(long, c_behavior_state*)
//.text:0146BB10 ; void __cdecl behavior_check_friend_control(long, c_behavior_state*)
//.text:0146BC70 ; short __cdecl behavior_check_friend_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0146BD20 ; short __cdecl post_combat_behavior_next(long, c_behavior_state*, c_behavior_state*)
//.text:0146BD80 ; bool __cdecl postcombat_behavior_perform(long, c_behavior_state*)
//.text:0146BE30 ; void __cdecl behavior_clear_layer_state(actor_datum*, short)
//.text:0146BE80 ; bool __cdecl behavior_combat_activate(long, c_behavior_state*)
//.text:0146BED0 ; short __cdecl behavior_combat_decide(long, short, c_behavior_state*)
//.text:0146BEE0 ; short __cdecl behavior_combat_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0146BF40 ; short __cdecl behavior_combat_next(long, c_behavior_state*, c_behavior_state*)
//.text:0146BFB0 ; void __cdecl behavior_control(long)
//.text:0146C040 ; long __cdecl behavior_coordinated_ambush_activate(long, c_behavior_state*)
//.text:0146C0B0 ; short __cdecl behavior_coordinated_ambush_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0146C0C0 ; short __cdecl behavior_coordinated_ambush_next(long, c_behavior_state*, c_behavior_state*)
//.text:0146C140 ; short __cdecl behavior_coordinated_ambush_select_actors(long, long, c_behavior_state*)
//.text:0146C1E0 ; bool __cdecl behavior_cover_activate(long, c_behavior_state*)
//.text:0146C360 ; void __cdecl behavior_cover_clear_non_deterministic_behavior_state(c_behavior_state*)
//.text:0146C370 ; void __cdecl behavior_cover_control(long, c_behavior_state*)
//.text:0146C7A0 ; void __cdecl behavior_cover_deactivate(long, c_behavior_state*)
//.text:0146C820 ; bool __cdecl behavior_cover_find_cover_point(long, c_cover_state_data*)
//.text:0146C9B0 ; short __cdecl behavior_cover_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0146C9F0 ; bool __cdecl behavior_cover_navigate_to_cover_point(long, firing_position_ref, firing_position*, c_cover_state_data*, path_state*, bool, long)
//.text:0146CC20 ; short __cdecl behavior_cover_next(long, c_behavior_state*, c_behavior_state*)
//.text:0146CCC0 ; bool __cdecl behavior_cover_peek_activate(long, c_behavior_state*)
//.text:0146CD40 ; void __cdecl behavior_cover_peek_control(long, c_behavior_state*)
//.text:0146CDA0 ; void __cdecl behavior_cover_peek_deactivate(long, c_behavior_state*)
//.text:0146CE10 ; short __cdecl behavior_cover_peek_next(long, c_behavior_state*, c_behavior_state*)
//.text:0146CF60 ; bool __cdecl behavior_cover_peek_perform(long, c_behavior_state*)
//.text:0146CFD0 ; bool __cdecl behavior_cover_perform(long, c_behavior_state*)
//.text:0146D1F0 ; bool __cdecl behavior_coverfriend_activate(long, c_behavior_state*)
//.text:0146D2B0 ; void __cdecl behavior_coverfriend_control(long, c_behavior_state*)
//.text:0146D480 ; void __cdecl behavior_coverfriend_flush_actor(long, c_behavior_state*, long)
//.text:0146D4A0 ; short __cdecl behavior_coverfriend_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0146D510 ; short __cdecl behavior_coverfriend_next(long, c_behavior_state*, c_behavior_state*)
//.text:0146D550 ; bool __cdecl behavior_cower_activate(long, c_behavior_state*)
//.text:0146D660 ; void __cdecl behavior_cower_clear_non_deterministic_behavior_state(c_behavior_state*)
//.text:0146D670 ; short __cdecl behavior_cower_decide(long, short, c_behavior_state*)
//.text:0146D9A0 ; void __cdecl behavior_cower_flush_structure_indices(long, c_behavior_state*)
//.text:0146D9B0 ; short __cdecl behavior_cower_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0146D9C0 ; short __cdecl behavior_cower_next(long, c_behavior_state*, c_behavior_state*)
//.text:0146DA20 ; bool __cdecl behavior_cure_isolation_activate(long, c_behavior_state*)
//.text:0146DA40 ; void __cdecl behavior_cure_isolation_control(long, c_behavior_state*)
//.text:0146DBF0 ; void __cdecl behavior_cure_isolation_debug(long, c_behavior_state*)
//.text:0146DC00 ; short __cdecl behavior_cure_isolation_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0146DC40 ; short __cdecl behavior_cure_isolation_next(long, c_behavior_state*, c_behavior_state*)
//.text:0146DCB0 ; bool __cdecl behavior_cure_isolation_perform(long, c_behavior_state*)
//.text:0146E2A0 ; short __cdecl behavior_decide_internal(long, child_entry**, short, short, c_behavior_state*, c_behavior_state*, short, short*, short*)
//.text:0146E5C0 ; short __cdecl behavior_decide_prioritized(long, short, c_behavior_state*)
//.text:0146E700 ; short __cdecl behavior_decide_sequential(long, short, c_behavior_state*)
//.text:0146E830 ; short __cdecl behavior_decide_sequential_looping(long, short, c_behavior_state*)
//.text:0146E9A0 ; short __cdecl behavior_decide_shoot_corpse(long, short, c_behavior_state*)
//.text:0146EB60 ; bool __cdecl behavior_deploy_turret_activate(long, c_behavior_state*)
//.text:0146EB90 ; void __cdecl behavior_deploy_turret_control(long, c_behavior_state*)
//.text:0146EC00 ; void __cdecl behavior_deploy_turret_flush_object(long, c_behavior_state*, long)
//.text:0146EC20 ; short __cdecl behavior_deploy_turret_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0146EC70 ; short __cdecl behavior_deploy_turret_next(long, c_behavior_state*, c_behavior_state*)
//.text:0146EE90 ; bool __cdecl behavior_deploy_turret_perform(long, c_behavior_state*)
//.text:0146F140 ; void __cdecl behavior_deploy_turret_setup(c_behavior_state*, long)
//.text:0146F170 ; bool __cdecl behavior_destroy_cover_activate(long, c_behavior_state*)
//.text:0146F180 ; void __cdecl behavior_destroy_cover_control(long, c_behavior_state*)
//.text:0146F210 ; short __cdecl behavior_destroy_cover_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0146F2B0 ; short __cdecl behavior_destroy_cover_next(long, c_behavior_state*, c_behavior_state*)
//.text:0146F2E0 ; bool __cdecl behavior_destroy_cover_perform(long, c_behavior_state*)
//.text:0146F2F0 ; bool __cdecl behavior_destroy_obstacle_activate(long, c_behavior_state*)
//.text:0146F340 ; void __cdecl behavior_destroy_obstacle_control(long, c_behavior_state*)
//.text:0146F400 ; short __cdecl behavior_destroy_obstacle_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0146F480 ; short __cdecl behavior_destroy_obstacle_next(long, c_behavior_state*, c_behavior_state*)
//.text:0146F500 ; bool __cdecl behavior_destroy_obstacle_perform(long, c_behavior_state*)
//.text:0146F620 ; long __cdecl behavior_emerge_activate(long, c_behavior_state*)
//.text:0146F6A0 ; void __cdecl behavior_emerge_control(long, c_behavior_state*, joint_state_datum*)
//.text:0146F790 ; short __cdecl behavior_emerge_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0146F7A0 ; short __cdecl behavior_emerge_next(long, c_behavior_state*, c_behavior_state*)
//.text:0146F920 ; bool __cdecl behavior_emerge_perform(long, c_behavior_state*, joint_state_datum*)
//.text:0146F930 ; short __cdecl behavior_emerge_select_actors(long, long, c_behavior_state*)
//.text:0146FAE0 ; bool __cdecl behavior_engage_activate(long, c_behavior_state*)
//.text:0146FB00 ; short __cdecl behavior_engage_decide(long, short, c_behavior_state*)
//.text:0146FE10 ; short __cdecl behavior_engage_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0146FE70 ; short __cdecl behavior_engage_next(long, c_behavior_state*, c_behavior_state*)
//.text:0146FED0 ; bool __cdecl behavior_engage_sync_activate(long, c_behavior_state*)
//.text:0146FF40 ; void __cdecl behavior_engage_sync_control(long, c_behavior_state*)
//.text:0146FFB0 ; short __cdecl behavior_engage_sync_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:01470040 ; short __cdecl behavior_engage_sync_next(long, c_behavior_state*, c_behavior_state*)
//.text:01470070 ; bool __cdecl behavior_engage_sync_perform(long, c_behavior_state*)
//.text:014701A0 ; 
//.text:01470290 ; 
//.text:01470360 ; 
//.text:01470370 ; 
//.text:01470380 ; 
//.text:014703E0 ; bool __cdecl behavior_enter_vehicle_activate(long, c_behavior_state*)
//.text:014704C0 ; void __cdecl behavior_enter_vehicle_control(long, c_behavior_state*)
//.text:014706A0 ; short __cdecl behavior_enter_vehicle_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:01470720 ; short __cdecl behavior_enter_vehicle_next(long, c_behavior_state*, c_behavior_state*)
//.text:014707A0 ; bool __cdecl behavior_enter_vehicle_perform(long, c_behavior_state*)
//.text:01470AB0 ; bool __cdecl behavior_fight_activate(long, c_behavior_state*)
//.text:01470B60 ; void __cdecl behavior_fight_clear_non_deterministic_behavior_state(c_behavior_state*)
//.text:01470B70 ; void __cdecl behavior_fight_control(long, c_behavior_state*)
//.text:01470D00 ; short __cdecl behavior_fight_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:01470D40 ; short __cdecl behavior_fight_next(long, c_behavior_state*, c_behavior_state*)
//.text:01470E10 ; bool __cdecl behavior_fight_perform(long, c_behavior_state*)
//.text:01471040 ; void __cdecl behavior_fight_positioning_control(long, c_behavior_state*)
//.text:01471080 ; short __cdecl behavior_fight_positioning_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:014710B0 ; short __cdecl behavior_find_layer(actor_datum*, short, short)
//.text:01471180 ; bool __cdecl behavior_flank_activate(long, c_behavior_state*)
//.text:014711A0 ; void __cdecl behavior_flank_control(long, c_behavior_state*)
//.text:01471230 ; void __cdecl behavior_flank_deactivate(long, c_behavior_state*)
//.text:01471240 ; short __cdecl behavior_flank_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:014712C0 ; short __cdecl behavior_flank_next(long, c_behavior_state*, c_behavior_state*)
//.text:014713B0 ; bool __cdecl behavior_flank_perform(long, c_behavior_state*)
//.text:01471610 ; bool __cdecl behavior_flee_activate(long, c_behavior_state*)
//.text:014716D0 ; void __cdecl behavior_flee_control(long, c_behavior_state*)
//.text:014718A0 ; void __cdecl behavior_flee_deactivate(long, c_behavior_state*)
//.text:014718B0 ; short __cdecl behavior_flee_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:01471930 ; short __cdecl behavior_flee_next(long, c_behavior_state*, c_behavior_state*)
//.text:01471960 ; bool __cdecl behavior_flee_perform(long, c_behavior_state*)
//.text:01471AE0 ; bool __cdecl behavior_flee_setup(c_flee_state_data*, long, long, long)
//.text:01471B00 ; bool __cdecl behavior_flight_wander_activate(long, c_behavior_state*)
//.text:01471B50 ; void __cdecl behavior_flight_wander_control(long, c_behavior_state*)
//.text:01471BA0 ; short __cdecl behavior_flight_wander_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:01471BB0 ; short __cdecl behavior_flight_wander_next(long, c_behavior_state*, c_behavior_state*)
//.text:01471BE0 ; bool __cdecl behavior_flight_wander_perform(long, c_behavior_state*)
//.text:01471CC0 ; bool __cdecl behavior_follow_activate(long, c_behavior_state*)
//.text:01471CE0 ; void __cdecl behavior_follow_control(long, c_behavior_state*)
//.text:01471D60 ; short __cdecl behavior_follow_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:01471F70 ; short __cdecl behavior_follow_next(long, c_behavior_state*, c_behavior_state*)
//.text:01472010 ; bool __cdecl behavior_follow_perform(long, c_behavior_state*)
//.text:01472260 ; long __cdecl behavior_formation_activate(long, c_behavior_state*)
//.text:01472360 ; void __cdecl behavior_formation_control(long, c_behavior_state*, joint_state_datum*)
//.text:01472690 ; short __cdecl behavior_formation_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:014726D0 ; short __cdecl behavior_formation_next(long, c_behavior_state*, c_behavior_state*)
//.text:01472720 ; bool __cdecl behavior_formation_perform(long, c_behavior_state*, joint_state_datum*)
//.text:01472E10 ; short __cdecl behavior_formation_select_actors(long, long, c_behavior_state*)
//.text:01472F30 ; void __cdecl behavior_formation_setup(long, short, long, long, long, c_formation_data*)
//.text:01472F60 ; bool __cdecl behavior_go_to_activate(long, c_behavior_state*)
//.text:01472F80 ; short __cdecl behavior_go_to_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:01472F90 ; short __cdecl behavior_go_to_next(long, c_behavior_state*, c_behavior_state*)
//.text:01472FC0 ; bool __cdecl behavior_go_to_perform(long, c_behavior_state*)
//.text:01473120 ; bool __cdecl behavior_grapple_activate(long, c_behavior_state*)
//.text:01473180 ; void __cdecl behavior_grapple_control(long, c_behavior_state*)
//.text:01473310 ; void __cdecl behavior_grapple_deactivate(long, c_behavior_state*)
//.text:01473350 ; void __cdecl behavior_grapple_flush_object(long, c_behavior_state*, long)
//.text:01473360 ; void __cdecl behavior_grapple_flush_prop(long, c_behavior_state*, long)
//.text:01473380 ; short __cdecl behavior_grapple_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:01473530 ; short __cdecl behavior_grapple_next(long, c_behavior_state*, c_behavior_state*)
//.text:014736D0 ; bool __cdecl behavior_grapple_perform(long, c_behavior_state*)
//.text:014739A0 ; bool __cdecl behavior_guard_activate(long, c_behavior_state*)
//.text:01473A70 ; void __cdecl behavior_guard_control(long, c_behavior_state*)
//.text:01473BF0 ; void __cdecl behavior_guard_flush_firing_position_indices(long, c_behavior_state*, bool)
//.text:01473C20 ; short __cdecl behavior_guard_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:01473C30 ; short __cdecl behavior_guard_next(long, c_behavior_state*, c_behavior_state*)
//.text:01473C70 ; bool __cdecl behavior_guard_perform(long, c_behavior_state*)
//.text:01474160 ; long __cdecl behavior_huddle_activate(long, c_behavior_state*)
//.text:01474250 ; void __cdecl behavior_huddle_control(long, c_behavior_state*, joint_state_datum*)
//.text:014742F0 ; void __cdecl behavior_huddle_deactivate(long, c_behavior_state*, joint_state_datum*)
//.text:01474310 ; void __cdecl behavior_huddle_debug(long, c_behavior_state*, joint_state_datum*)
//.text:01474320 ; void __cdecl behavior_huddle_flush_firing_points(long, c_behavior_state*, bool)
//.text:01474360 ; short __cdecl behavior_huddle_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:014743A0 ; short __cdecl behavior_huddle_next(long, c_behavior_state*, c_behavior_state*)
//.text:014743B0 ; bool __cdecl behavior_huddle_perform(long, c_behavior_state*, joint_state_datum*)
//.text:01474550 ; bool __cdecl behavior_idle_activate(long, c_behavior_state*)
//.text:014745A0 ; short __cdecl behavior_idle_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:014745B0 ; short __cdecl behavior_idle_next(long, c_behavior_state*, c_behavior_state*)

short __cdecl behavior_index_by_name(char* name)
{
	return INVOKE(0x014745C0, behavior_index_by_name, name);
}

//.text:01474600 ; 
//.text:01474650 ; 
//.text:014747E0 ; 
//.text:01474830 ; 
//.text:01474840 ; 
//.text:01474860 ; 
//.text:014748C0 ; 
//.text:014748F0 ; 
//.text:01474920 ; 
//.text:01474CB0 ; bool __cdecl behavior_investigate_activate(long, c_behavior_state*)
//.text:01474E20 ; void __cdecl behavior_investigate_clear_non_deterministic_behavior_state(c_behavior_state*)
//.text:01474E30 ; short __cdecl behavior_investigate_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:01474EB0 ; short __cdecl behavior_investigate_next(long, c_behavior_state*, c_behavior_state*)
//.text:01474F80 ; bool __cdecl behavior_kamikaze_activate(long, c_behavior_state*)
//.text:01474FB0 ; void __cdecl behavior_kamikaze_control(long, c_behavior_state*)
//.text:014750B0 ; bool __cdecl behavior_kamikaze_create_grenades(long, c_kamikaze_state_data*)
//.text:01475240 ; void __cdecl behavior_kamikaze_deactivate(long, c_behavior_state*)
//.text:01475250 ; void __cdecl behavior_kamikaze_flush_object(long, c_behavior_state*, long)
//.text:014752A0 ; short __cdecl behavior_kamikaze_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:014752E0 ; short __cdecl behavior_kamikaze_next(long, c_behavior_state*, c_behavior_state*)
//.text:01475330 ; bool __cdecl behavior_kamikaze_perform(long, c_behavior_state*)
//.text:01475430 ; bool __cdecl behavior_leap_on_cover_activate(long, c_behavior_state*)
//.text:014754D0 ; void __cdecl behavior_leap_on_cover_clear_non_deterministic_behavior_state(c_behavior_state*)
//.text:014754E0 ; void __cdecl behavior_leap_on_cover_control(long, c_behavior_state*)
//.text:014755C0 ; void __cdecl behavior_leap_on_cover_deactivate(long, c_behavior_state*)
//.text:014755F0 ; void __cdecl behavior_leap_on_cover_flush_structure_indices(long, c_behavior_state*)
//.text:01475600 ; short __cdecl behavior_leap_on_cover_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:01475750 ; short __cdecl behavior_leap_on_cover_next(long, c_behavior_state*, c_behavior_state*)
//.text:014758D0 ; bool __cdecl behavior_leap_on_cover_perform(long, c_behavior_state*)
//.text:014759B0 ; bool __cdecl behavior_melee_charge_activate(long, c_behavior_state*)
//.text:01475B00 ; void __cdecl behavior_melee_charge_control(long, c_behavior_state*)
//.text:01475B90 ; void __cdecl behavior_melee_charge_deactivate(long, c_behavior_state*)
//.text:01475BD0 ; short __cdecl behavior_melee_charge_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:01475F10 ; bool __cdecl behavior_melee_charge_perform(long, c_behavior_state*)
//.text:014769B0 ; bool __cdecl behavior_melee_leaping_charge_activate(long, c_behavior_state*)
//.text:01476A10 ; void __cdecl behavior_melee_leaping_charge_control(long, c_behavior_state*)
//.text:01476AD0 ; void __cdecl behavior_melee_leaping_charge_deactivate(long, c_behavior_state*)
//.text:01476B10 ; short __cdecl behavior_melee_leaping_charge_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:01476D10 ; bool __cdecl behavior_melee_leaping_charge_perform(long, c_behavior_state*)
//.text:01477640 ; short __cdecl behavior_null_discrete_next(long, c_behavior_state*, c_behavior_state*)
//.text:01477650 ; short __cdecl behavior_null_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:01477660 ; short __cdecl behavior_null_next(long, c_behavior_state*, c_behavior_state*)
//.text:01477670 ; bool __cdecl behavior_obey_activate(long, c_behavior_state*)
//.text:01477680 ; void __cdecl behavior_obey_deactivate(long, c_behavior_state*)
//.text:014776D0 ; short __cdecl behavior_obey_decide(long, short, c_behavior_state*)
//.text:01477840 ; short __cdecl behavior_obey_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:014778B0 ; short __cdecl behavior_obey_next(long, c_behavior_state*, c_behavior_state*)
//.text:01477900 ; bool __cdecl behavior_patrol_activate(long, c_behavior_state*)
//.text:014779E0 ; void __cdecl behavior_patrol_debug(long, c_behavior_state*)
//.text:014779F0 ; short __cdecl behavior_patrol_decide(long, short, c_behavior_state*)
//.text:01477D80 ; short __cdecl behavior_patrol_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:01477DC0 ; bool __cdecl behavior_perch_activate(long, c_behavior_state*)
//.text:01477EB0 ; void __cdecl behavior_perch_control(long, c_behavior_state*)
//.text:01477F30 ; void __cdecl behavior_perch_flush_firing_position_indices(long, c_behavior_state*, bool)
//.text:01477F60 ; short __cdecl behavior_perch_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:01477FF0 ; short __cdecl behavior_perch_next(long, c_behavior_state*, c_behavior_state*)
//.text:014781C0 ; bool __cdecl behavior_perch_perform(long, c_behavior_state*)
//.text:01478360 ; bool __cdecl behavior_perimeter_activate(long, c_behavior_state*)
//.text:014784B0 ; void __cdecl behavior_perimeter_control(long, c_behavior_state*)
//.text:01478590 ; short __cdecl behavior_perimeter_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:01478720 ; short __cdecl behavior_perimeter_next(long, c_behavior_state*, c_behavior_state*)
//.text:01478A60 ; bool __cdecl behavior_perimeter_perform(long, c_behavior_state*)
//.text:01478D30 ; bool __cdecl behavior_pickup_player_activate(long, c_behavior_state*)
//.text:01478D90 ; void __cdecl behavior_pickup_player_control(long, c_behavior_state*)
//.text:01478DA0 ; short __cdecl behavior_pickup_player_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:01478E40 ; short __cdecl behavior_pickup_player_next(long, c_behavior_state*, c_behavior_state*)
//.text:01478E90 ; bool __cdecl behavior_pickup_player_perform(long, c_behavior_state*)
//.text:01478EA0 ; bool __cdecl behavior_postcombat_activate(long, c_behavior_state*)
//.text:01478F50 ; void __cdecl behavior_postcombat_approach_flush_actor(long, c_behavior_state*, long)
//.text:01478F70 ; void __cdecl behavior_postcombat_clear_non_deterministic_behavior_state(c_behavior_state*)
//.text:01478F80 ; short __cdecl behavior_postcombat_decide(long, short, c_behavior_state*)
//.text:01479070 ; void __cdecl behavior_postcombat_flush_prop(long, c_behavior_state*, long)
//.text:01479090 ; short __cdecl behavior_postcombat_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:01479100 ; short __cdecl behavior_postcombat_next(long, c_behavior_state*, c_behavior_state*)
//.text:01479120 ; bool __cdecl behavior_postombat_approach_activate(long, c_behavior_state*)
//.text:01479170 ; void __cdecl behavior_postombat_approach_control(long, c_behavior_state*)
//.text:01479180 ; short __cdecl behavior_postombat_approach_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:014791D0 ; short __cdecl behavior_postombat_approach_next(long, c_behavior_state*, c_behavior_state*)
//.text:01479250 ; bool __cdecl behavior_postombat_approach_perform(long, c_behavior_state*)
//.text:014794D0 ; bool __cdecl behavior_postpostcombat_activate(long, c_behavior_state*)
//.text:01479520 ; bool __cdecl behavior_postsearch_activate(long, c_behavior_state*)
//.text:01479540 ; void __cdecl behavior_postsearch_control(long, c_behavior_state*)
//.text:01479570 ; short __cdecl behavior_postsearch_next(long, c_behavior_state*, c_behavior_state*)
//.text:014795C0 ; bool __cdecl behavior_posture_activate(long, c_behavior_state*)
//.text:01479760 ; void __cdecl behavior_posture_control(long, c_behavior_state*)
//.text:01479770 ; void __cdecl behavior_posture_deactivate(long, c_behavior_state*)
//.text:01479780 ; short __cdecl behavior_posture_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:014797F0 ; short __cdecl behavior_posture_next(long, c_behavior_state*, c_behavior_state*)
//.text:01479840 ; bool __cdecl behavior_posture_perform(long, c_behavior_state*)
//.text:014798D0 ; long __cdecl behavior_presearch_activate(long, c_behavior_state*)
//.text:01479BB0 ; void __cdecl behavior_presearch_deactivate(long, c_behavior_state*, joint_state_datum*)
//.text:01479C50 ; short __cdecl behavior_presearch_decide(long, short, c_behavior_state*, joint_state_datum*)
//.text:01479E20 ; void __cdecl behavior_presearch_flush_prop(long, c_behavior_state*, long)
//.text:01479E80 ; short __cdecl behavior_presearch_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:01479FB0 ; short __cdecl behavior_presearch_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147A1D0 ; bool __cdecl behavior_pursuit_activate(long, c_behavior_state*)
//.text:0147A230 ; void __cdecl behavior_pursuit_clear_non_deterministic_behavior_state(c_behavior_state*)
//.text:0147A240 ; void __cdecl behavior_pursuit_control(long, c_behavior_state*)
//.text:0147A310 ; short __cdecl behavior_pursuit_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147A350 ; short __cdecl behavior_pursuit_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147A410 ; long __cdecl behavior_pursuit_sync_activate(long, c_behavior_state*)
//.text:0147A6B0 ; void __cdecl behavior_pursuit_sync_control(long, c_behavior_state*, joint_state_datum*)
//.text:0147A6E0 ; void __cdecl behavior_pursuit_sync_flush_object(long, c_behavior_state*, long)
//.text:0147A720 ; void __cdecl behavior_pursuit_sync_flush_structure_indices(long, c_behavior_state*)
//.text:0147A760 ; short __cdecl behavior_pursuit_sync_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147A770 ; short __cdecl behavior_pursuit_sync_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147A940 ; short __cdecl behavior_pursuit_sync_select_actors(long, long, c_behavior_state*)
//.text:0147AA80 ; bool __cdecl behavior_ready_activate(long, c_behavior_state*)
//.text:0147AB70 ; void __cdecl behavior_ready_deactivate(long, c_behavior_state*)
//.text:0147AB80 ; short __cdecl behavior_ready_decide(long, short, c_behavior_state*)
//.text:0147ABF0 ; short __cdecl behavior_ready_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147AC00 ; short __cdecl behavior_ready_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147AC30 ; bool __cdecl behavior_retreat_activate(long, c_behavior_state*)
//.text:0147ACD0 ; void __cdecl behavior_retreat_deactivate(long, c_behavior_state*)
//.text:0147AD10 ; short __cdecl behavior_retreat_decide(long, short, c_behavior_state*)
//.text:0147AD60 ; short __cdecl behavior_retreat_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147AD70 ; short __cdecl behavior_retreat_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147ADD0 ; short __cdecl behavior_root_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147ADE0 ; short __cdecl behavior_root_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147ADF0 ; bool __cdecl behavior_search_activate(long, c_behavior_state*)
//.text:0147AF10 ; void __cdecl behavior_search_deactivate(long, c_behavior_state*)
//.text:0147AF50 ; short __cdecl behavior_search_decide(long, short, c_behavior_state*)
//.text:0147B110 ; short __cdecl behavior_search_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147B190 ; short __cdecl behavior_search_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147B2B0 ; void __cdecl behavior_search_sync_control(long, c_behavior_state*)
//.text:0147B2F0 ; short __cdecl behavior_search_sync_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147B320 ; short __cdecl behavior_search_sync_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147B360 ; bool __cdecl behavior_self_preservation_activate(long, c_behavior_state*)
//.text:0147B3E0 ; short __cdecl behavior_self_preservation_decide(long, short, c_behavior_state*)
//.text:0147B480 ; short __cdecl behavior_self_preservation_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147B490 ; short __cdecl behavior_self_preservation_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147B570 ; short __cdecl behavior_self_preserve_time_left(long)
//.text:0147B640 ; bool __cdecl behavior_shoot_corpse_activate(long, c_behavior_state*)
//.text:0147B6F0 ; short __cdecl behavior_shoot_corpse_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147B790 ; short __cdecl behavior_shoot_corpse_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147B7E0 ; bool __cdecl behavior_sleep_activate(long, c_behavior_state*)
//.text:0147B910 ; void __cdecl behavior_sleep_control(long, c_behavior_state*)
//.text:0147B950 ; void __cdecl behavior_sleep_deactivate(long, c_behavior_state*)
//.text:0147B9A0 ; short __cdecl behavior_sleep_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147B9D0 ; short __cdecl behavior_sleep_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147B9E0 ; bool __cdecl behavior_sleep_perform(long, c_behavior_state*)
//.text:0147BA90 ; bool __cdecl behavior_squad_patrol_activate(long, c_behavior_state*)
//.text:0147BAD0 ; void __cdecl behavior_squad_patrol_control(long, c_behavior_state*)
//.text:0147BC10 ; void __cdecl behavior_squad_patrol_deactivate(long, c_behavior_state*)
//.text:0147BC40 ; short __cdecl behavior_squad_patrol_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147BC90 ; short __cdecl behavior_squad_patrol_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147BCE0 ; bool __cdecl behavior_squad_patrol_perform(long, c_behavior_state*)
//.text:0147C0A0 ; 
//.text:0147C0E0 ; 
//.text:0147C120 ; bool __cdecl behavior_stalk_activate(long, c_behavior_state*)
//.text:0147C180 ; void __cdecl behavior_stalk_control(long, c_behavior_state*)
//.text:0147C230 ; void __cdecl behavior_stalk_deactivate(long, c_behavior_state*)
//.text:0147C270 ; void __cdecl behavior_stalk_flush_prop(long, c_behavior_state*, long)
//.text:0147C290 ; short __cdecl behavior_stalk_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147C2C0 ; short __cdecl behavior_stalk_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147C3B0 ; bool __cdecl behavior_stalk_perform(long, c_behavior_state*)
//.text:0147C5B0 ; void __cdecl behavior_state_fail_child(c_behavior_state*)
//.text:0147C5D0 ; bool __cdecl behavior_stunned_activate(long, c_behavior_state*)
//.text:0147C6D0 ; void __cdecl behavior_stunned_control(long, c_behavior_state*)
//.text:0147C780 ; short __cdecl behavior_stunned_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147C7C0 ; short __cdecl behavior_stunned_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147C7F0 ; bool __cdecl behavior_suppress_activate(long, c_behavior_state*)
//.text:0147C9A0 ; void __cdecl behavior_suppress_clear_non_deterministic_behavior_state(c_behavior_state*)
//.text:0147C9B0 ; void __cdecl behavior_suppress_control(long, c_behavior_state*)
//.text:0147CB10 ; short __cdecl behavior_suppress_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147CBF0 ; short __cdecl behavior_suppress_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147CCD0 ; long __cdecl behavior_surprise_activate(long, c_behavior_state*)
//.text:0147CE10 ; void __cdecl behavior_surprise_control(long, c_behavior_state*, joint_state_datum*)
//.text:0147CE60 ; void __cdecl behavior_surprise_flush_object(long, c_behavior_state*, long)
//.text:0147CEA0 ; short __cdecl behavior_surprise_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147D130 ; short __cdecl behavior_surprise_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147D270 ; bool __cdecl behavior_surprise_perform(long, c_behavior_state*, joint_state_datum*)
//.text:0147D340 ; bool __cdecl behavior_uncover_activate(long, c_behavior_state*)
//.text:0147D3B0 ; void __cdecl behavior_uncover_clear_non_deterministic_behavior_state(c_behavior_state*)
//.text:0147D3C0 ; void __cdecl behavior_uncover_deactivate(long, c_behavior_state*)
//.text:0147D3D0 ; short __cdecl behavior_uncover_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147D480 ; short __cdecl behavior_uncover_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147D4B0 ; short __cdecl behavior_update_running_child(long, short, c_behavior_state*, short*)
//.text:0147D6A0 ; bool __cdecl behavior_vehicle_avoid_activate(long, c_behavior_state*)
//.text:0147D6B0 ; void __cdecl behavior_vehicle_avoid_control(long, c_behavior_state*)
//.text:0147D7B0 ; short __cdecl behavior_vehicle_avoid_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147D860 ; short __cdecl behavior_vehicle_avoid_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147D930 ; bool __cdecl behavior_vehicle_avoid_perform(long, c_behavior_state*)
//.text:0147D940 ; bool __cdecl behavior_vehicle_charge_activate(long, c_behavior_state*)
//.text:0147DA20 ; void __cdecl behavior_vehicle_charge_control(long, c_behavior_state*)
//.text:0147DAC0 ; void __cdecl behavior_vehicle_charge_deactivate(long, c_behavior_state*)
//.text:0147DB00 ; short __cdecl behavior_vehicle_charge_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147DCA0 ; short __cdecl behavior_vehicle_charge_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147DEE0 ; bool __cdecl behavior_vehicle_charge_perform(long, c_behavior_state*)
//.text:0147DF90 ; bool __cdecl behavior_vehicle_cover_activate(long, c_behavior_state*)
//.text:0147DFB0 ; void __cdecl behavior_vehicle_cover_control(long, c_behavior_state*)
//.text:0147E1B0 ; short __cdecl behavior_vehicle_cover_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147E1F0 ; short __cdecl behavior_vehicle_cover_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147E310 ; bool __cdecl behavior_vehicle_cover_perform(long, c_behavior_state*)
//.text:0147E700 ; bool __cdecl behavior_vehicle_fight_activate(long, c_behavior_state*)
//.text:0147E710 ; void __cdecl behavior_vehicle_fight_control(long, c_behavior_state*)
//.text:0147E8A0 ; short __cdecl behavior_vehicle_fight_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147E910 ; short __cdecl behavior_vehicle_fight_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147E920 ; bool __cdecl behavior_vehicle_fight_perform(long, c_behavior_state*)
//.text:0147EB60 ; long __cdecl behavior_vehicle_flip_activate(long, c_behavior_state*)
//.text:0147ECA0 ; void __cdecl behavior_vehicle_flip_control(long, c_behavior_state*, joint_state_datum*)
//.text:0147ED80 ; void __cdecl behavior_vehicle_flip_deactivate(long, c_behavior_state*, joint_state_datum*)
//.text:0147EDA0 ; void __cdecl behavior_vehicle_flip_debug(long, c_behavior_state*, joint_state_datum*)
//.text:0147EDB0 ; void __cdecl behavior_vehicle_flip_flush_object(long, c_behavior_state*, long)
//.text:0147EDF0 ; void __cdecl behavior_vehicle_flip_flush_structure_indices(long, c_behavior_state*)
//.text:0147EE10 ; short __cdecl behavior_vehicle_flip_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147EE50 ; short __cdecl behavior_vehicle_flip_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147EED0 ; bool __cdecl behavior_vehicle_flip_perform(long, c_behavior_state*, joint_state_datum*)
//.text:0147F1E0 ; short __cdecl behavior_vehicle_flip_select_actors(long, long, c_behavior_state*)
//.text:0147F320 ; bool __cdecl behavior_vehicle_pickup_activate(long, c_behavior_state*)
//.text:0147F360 ; void __cdecl behavior_vehicle_pickup_control(long, c_behavior_state*)
//.text:0147F3B0 ; short __cdecl behavior_vehicle_pickup_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147F3D0 ; short __cdecl behavior_vehicle_pickup_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147F4A0 ; bool __cdecl behavior_vehicle_pickup_perform(long, c_behavior_state*)
//.text:0147F830 ; bool __cdecl behavior_vehicle_ram_activate(long, c_behavior_state*)
//.text:0147FA20 ; void __cdecl behavior_vehicle_ram_control(long, c_behavior_state*)
//.text:0147FAF0 ; short __cdecl behavior_vehicle_ram_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147FBE0 ; short __cdecl behavior_vehicle_ram_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147FCB0 ; bool __cdecl behavior_vehicle_ram_perform(long, c_behavior_state*)
//.text:0147FDC0 ; bool __cdecl behavior_wander_activate(long, c_behavior_state*)
//.text:0147FDE0 ; void __cdecl behavior_wander_control(long, c_behavior_state*)
//.text:0147FE20 ; short __cdecl behavior_wander_get_desire(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:0147FEC0 ; short __cdecl behavior_wander_next(long, c_behavior_state*, c_behavior_state*)
//.text:0147FF10 ; bool __cdecl behavior_wander_perform(long, c_behavior_state*)
//.text:01480210 ; void __cdecl behaviors_initialize()
//.text:01480340 ; short __cdecl berserk_get_impulse(long, c_behavior_state*)
//.text:01480360 ; short __cdecl berserk_wander_get_impulse(long, c_behavior_state*)
//.text:014803C0 ; bool __cdecl board_activate_helper(long, c_vehicle_board_data*)
//.text:014804B0 ; 
//.text:014804E0 ; bool __cdecl boarding_find_seat(long, long, long*, short*, short*, bool*)
//.text:014806E0 ; 
//.text:01480700 ; 
//.text:01480950 ; short __cdecl broken_berserk_impulse_get_impulse(long, c_behavior_state*)
//.text:014809B0 ; short __cdecl broken_flee_impulse_get_impulse(long, c_behavior_state*)
//.text:01480A40 ; short __cdecl broken_kamikaze_impulse_get_impulse(long, c_behavior_state*)
//.text:01480B50 ; short __cdecl broken_scatter_get_impulse(long, c_behavior_state*)
//.text:01480BB0 ; short __cdecl charge_children_next(long, c_behavior_state*, c_behavior_state*)
//.text:01480C60 ; 
//.text:01480D30 ; void __cdecl charge_movement_helper(long, c_charge_state_data*, real, bool*, bool*, bool*)
//.text:01480E10 ; 
//.text:01480E30 ; child_entry** __cdecl child_list_get(long, short, short*)
//.text:01480E90 ; 
//.text:01480EC0 ; bool __cdecl command_script_allows_activity_termination(long)
//.text:01480F20 ; short __cdecl construct_child_list_internal(child_entry**, custom_child_list*, child_entry*, short)
//.text:01481000 ; bool __cdecl cover_friend_continue(long, long, short)
//.text:014810C0 ; bool __cdecl cover_point_exposed(long, c_cover_state_data*, prop_ref_datum*)
//.text:01481140 ; short __cdecl coverme_investigate_get_impulse(long, c_behavior_state*)
//.text:014813A0 ; bool __cdecl cure_isolation_attached_vehicle(long, seat_entry_info const*, c_cure_isolation_data*, path_state*)
//.text:01481590 ; bool __cdecl cure_isolation_jump(long, path_state*, real, real)
//.text:01481990 ; short __cdecl cure_isolation_test_vector(long, vector3d const*, vector3d const*, path_state const*, real, real, real*)
//.text:01481D70 ; bool __cdecl cure_isolation_undirected(long, path_state*, real, real)
//.text:01482080 ; bool __cdecl cure_isolation_visible_enemy(long, path_state*, real, real)
//.text:014823C0 ; short __cdecl damage_vehicle_cover_get_impulse(long, c_behavior_state*)
//.text:014824D0 ; short __cdecl danger_cover_get_impulse(long, c_behavior_state*)
//.text:014826D0 ; short __cdecl danger_crouch_get_impulse(long, c_behavior_state*)
//.text:01482780 ; short __cdecl danger_vehicle_exit_get_impulse(long, c_behavior_state*)
//.text:014828C0 ; short __cdecl dive_get_impulse(long, c_behavior_state*)
//.text:01482C40 ; bool __cdecl emerge_wait_for_actor(long, long, real*, short*)
//.text:01482D50 ; 
//.text:01482D80 ; 
//.text:01483040 ; 
//.text:01483280 ; 
//.text:01483310 ; short __cdecl equipment_get_impulse(long, s_behavior_input const*, s_behavior_arguments const*)
//.text:01483600 ; short __cdecl exposed_rear_cover_get_impulse(long, c_behavior_state*)
//.text:014836F0 ; bool __cdecl fight_choose_fight_point(long, c_fight_state_data*)
//.text:01483960 ; short __cdecl fight_predict_visibility(long, real_point3d const*, long, short)
//.text:01483AD0 ; void __cdecl fight_reset_firing_position_timer(long, c_fight_state_data*)
//.text:01483B90 ; bool __cdecl fighting_perception_valid(long)
//.text:01483C30 ; bool __cdecl find_pursuit_position(long, long)
//.text:01483EE0 ; 
//.text:01483EF0 ; 
//.text:01483F00 ; 
//.text:01483F10 ; bool __cdecl flee_at_flee_position(long, c_flee_state_data*)
//.text:01484050 ; void __cdecl flee_flush_actor(long, c_behavior_state*, long)
//.text:01484070 ; void __cdecl flee_flush_firing_position_indices(long, c_behavior_state*, bool)
//.text:014840A0 ; bool __cdecl flip_try_direction(long, long, c_ai_point3d const*, c_sector_ref, vector3d const*, short, bool, c_ai_point3d*, c_sector_ref*)
//.text:01484260 ; 
//.text:014842D0 ; bool __cdecl formation_can_fire_on_target(long, short, short, vector2d*, vector2d*)
//.text:01484400 ; short __cdecl formation_find_position(long, c_formation_data*, joint_state_datum*, real_point2d*, vector2d*, vector2d*, real_point2d*)
//.text:01484660 ; void __cdecl formation_get_facing(joint_state_datum*, vector2d*)
//.text:014847F0 ; short __cdecl formation_get_member_in_position(joint_state_datum*, short)
//.text:014848A0 ; bool __cdecl formation_get_position(long, short, short, real_point2d const*, vector2d const*, vector2d const*, real_point2d*)
//.text:014849B0 ; short __cdecl formation_member_get_position_index(short, joint_state_datum*)
//.text:014849E0 ; c_behavior_state* __cdecl formation_member_get_state(joint_state_datum*, short)
//.text:01484A50 ; void __cdecl generate_style_vector(long, c_static_flags<224>*)
//.text:01484C50 ; public: static double __cdecl c_combat_range::get_actor_max_firing_range(long)
//.text:01484C80 ; 
//.text:01484C90 ; short __cdecl guard_get_guard_state(long)
//.text:01484D50 ; 
//.text:01484D80 ; 
//.text:01484F10 ; 
//.text:01484FD0 ; short __cdecl huddle_impulse_get_impulse(long, c_behavior_state*)
//.text:01485080 ; short __cdecl impulse_anti_vehicle_grenade_get_impulse(long, c_behavior_state*)
//.text:01485320 ; short __cdecl impulse_danger_retreat_get_impulse(long, c_behavior_state*)
//.text:014853D0 ; short __cdecl impulse_evasion_get_impulse(long, c_behavior_state*)
//.text:014855D0 ; short __cdecl impulse_grenade_get_impulse(long, c_behavior_state*)
//.text:014857A0 ; short __cdecl impulse_grenade_uncover_get_impulse(long, c_behavior_state*)
//.text:01485980 ; short __cdecl impulse_low_shield_retreat_get_impulse(long, c_behavior_state*)
//.text:01485A00 ; short __cdecl impulse_nowhere_to_run_ambush_get_impulse(long, c_behavior_state*)
//.text:01485A70 ; short __cdecl impulse_proximity_ambush_get_impulse(long, c_behavior_state*)
//.text:01485B00 ; short __cdecl impulse_proximity_retreat_get_impulse(long, c_behavior_state*)
//.text:01485BB0 ; short __cdecl impulse_scary_target_retreat_get_impulse(long, c_behavior_state*)
//.text:01485CE0 ; short __cdecl impulse_vulnerable_enemy_ambush_get_impulse(long, c_behavior_state*)
//.text:01485D90 ; 
//.text:01485DB0 ; void __cdecl kamikaze_detonate_grenades(c_kamikaze_state_data*)
//.text:01485E20 ; void __cdecl kamikaze_drop_grenades(c_kamikaze_state_data*)
//.text:01485EB0 ; 
//.text:01485F40 ; short __cdecl last_man_berserk_get_impulse(long, c_behavior_state*)
//.text:01485FD0 ; short __cdecl leader_abandoned_charge_get_impulse(long, c_behavior_state*)
//.text:014860E0 ; short __cdecl leader_dead_retreat_get_impulse(long, c_behavior_state*)
//.text:014861F0 ; void __cdecl leap_on_cover_flush_object(long, c_behavior_state*, long)
//.text:01486210 ; bool __cdecl leap_on_cover_object_get_target_point(long, long, real_point3d*)
//.text:014862C0 ; void __cdecl mark_executing_behaviors(long)
//.text:01486360 ; bool __cdecl melee_activate_helper(long, c_behavior_state*, bool)
//.text:01486690 ; short __cdecl overheated_weapon_retreat_get_impulse(long, c_behavior_state*)
//.text:014866E0 ; 
//.text:014866F0 ; bool __cdecl patrol_choose_next_point(long, c_patrol_state_data*, c_ai_point3d*, c_sector_ref*)
//.text:01486A20 ; void __cdecl patrol_choose_random_point(short, c_patrol_state_data*, bool)
//.text:01486AE0 ; short __cdecl peer_dead_retreat_get_impulse(long, c_behavior_state*)
//.text:01486BD0 ; long __cdecl pickup_find_passenger(long, short*)
//.text:01486DB0 ; short __cdecl player_endangered_cover_get_impulse(long, c_behavior_state*)
//.text:01486EA0 ; short __cdecl postcombat_decide(long, short, c_behavior_state*)
//.text:01486F20 ; void __cdecl postcombat_flush_structure_indices(long, c_behavior_state*)
//.text:01486F40 ; short __cdecl presearch_uncover_get_impulse(long, c_behavior_state*)
//.text:01486FC0 ; short __cdecl proximity_melee_get_impulse(long, c_behavior_state*)
//.text:014870F0 ; short __cdecl proximity_self_preserve_get_impulse(long, c_behavior_state*)
//.text:014871B0 ; bool __cdecl pursuit_area_discarded(long, short, short)
//.text:01487220 ; void __cdecl pursuit_clear_areas(long)
//.text:01487250 ; bool __cdecl pursuit_discard_area(long)
//.text:014872C0 ; bool __cdecl pursuit_set_area(long, short, short)
//.text:01487320 ; bool __cdecl ready_to_move(long, c_patrol_state_data const*)
//.text:01487370 ; short __cdecl refresh_target_get_impulse(long, c_behavior_state*)
//.text:014873E0 ; void __cdecl retreat_flush_prop(long, c_behavior_state*, long)
//.text:01487400 ; short __cdecl retreat_grenade_get_impulse(long, c_behavior_state*)
//.text:01487660 ; 
//.text:01487680 ; short __cdecl scary_target_self_preservation_get_impulse(long, c_behavior_state*)
//.text:01487770 ; short __cdecl sense_target_get_impulse(long, c_behavior_state*)
//.text:01487940 ; 
//.text:01487970 ; 
//.text:01487990 ; 
//.text:014879C0 ; 
//.text:014879E0 ; short __cdecl shield_depleted_berserk_get_impulse(long, c_behavior_state*)
//.text:01487B60 ; short __cdecl shield_depleted_cover_get_impulse(long, c_behavior_state*)
//.text:01487C50 ; short __cdecl stalker_camo_control_get_impulse(long, c_behavior_state*)
//.text:01487CF0 ; short __cdecl stuck_with_grenade_berserk_get_impulse(long, c_behavior_state*)
//.text:01487D60 ; short __cdecl surprise_retreat_get_impulse(long, c_behavior_state*)
//.text:01487E00 ; bool __cdecl target_on_board(long)
//.text:01487E90 ; 
//.text:01487EB0 ; 
//.text:01487ED0 ; 
//.text:01487EF0 ; 
//.text:01487F20 ; 
//.text:01488150 ; short __cdecl unassailable_enemy_grenade_get_impulse(long, c_behavior_state*)
//.text:014882A0 ; short __cdecl unassailable_enemy_self_preservation_get_impulse(long, c_behavior_state*)
//.text:014883D0 ; short __cdecl unreachable_enemy_self_preservation_get_impulse(long, c_behavior_state*)
//.text:01488470 ; bool __cdecl vehicle_charge_angle_valid(long, character_vehicle_properties*, real, long, bool)
//.text:01488570 ; real __cdecl vehicle_charge_get_min_weapon_range(long)
//.text:01488670 ; bool __cdecl vehicle_charge_range_valid(long, real*, long)
//.text:01488940 ; bool __cdecl vehicle_cover_safe(long, long)
//.text:014889E0 ; short __cdecl vehicle_engage_patrol_get_impulse(long, c_behavior_state*)
//.text:01488A40 ; short __cdecl vehicle_engage_wander_get_impulse(long, c_behavior_state*)
//.text:01488A80 ; short __cdecl vehicle_enter_friendly_get_impulse(long, c_behavior_state*)
//.text:01488CC0 ; short __cdecl vehicle_enter_get_impulse(long, c_behavior_state*)
//.text:01488EC0 ; short __cdecl vehicle_entry_engage_get_impulse(long, c_behavior_state*)
//.text:014892F0 ; short __cdecl vehicle_exit_get_impulse(long, c_behavior_state*)
//.text:014897C0 ; bool __cdecl vehicle_find_destination(long, long, real_point3d*, c_ai_point3d*, c_sector_ref*, bool*, bool)
//.text:01489D30 ; bool __cdecl vehicle_flip_find_target(long, c_vehicle_flip_data*, joint_vehicle_flip_data*)
//.text:0148A000 ; short __cdecl vehicle_flip_get_impulse(long, c_behavior_state*)
//.text:0148A0B0 ; void __cdecl vehicle_pickup_flush_object(long, c_behavior_state*, long)
//.text:0148A0D0 ; short __cdecl vehicle_turtle_get_impulse(long, c_behavior_state*)
//.text:0148A230 ; bool __cdecl vehicle_wants_to_move_at_all_costs(long)

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

