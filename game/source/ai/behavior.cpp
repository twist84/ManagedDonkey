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

//REFERENCE_DECLARE_ARRAY(0x01994070, const char*, behavior_names, NUMBER_OF_BEHAVIORS);

//.text:01466970 ; 
//.text:01466990 ; 
//.text:014669A0 ; 
//.text:01466A20 ; 
//.text:01466AA0 ; 
//.text:01466AD0 ; public: __cdecl Behavior_impulse::Behavior_impulse(int16, int32, int16(__cdecl*)(int32, c_behavior_state*))
//.text:01466B00 ; public: __cdecl Mutex_group::Mutex_group(int16, int32, int16(__cdecl*)(int32, const s_behavior_input*, const s_behavior_arguments*), int16(__cdecl*)(int32, c_behavior_state*, c_behavior_state*), bool(__cdecl*)(int32, c_behavior_state*), void(__cdecl*)(int32, c_behavior_state*), int16, int16(__cdecl*)(int32, int16, c_behavior_state*), int16, child_entry*, void(__cdecl*)(c_behavior_state*))
//.text:01466B90 ; public: __cdecl Mutex_group::Mutex_group(int16, int32, int16(__cdecl*)(int32, const s_behavior_input*, const s_behavior_arguments*), int16(__cdecl*)(int32, c_behavior_state*, c_behavior_state*), bool(__cdecl*)(int32, c_behavior_state*), void(__cdecl*)(int32, c_behavior_state*), int16, int16(__cdecl*)(int32, int16, c_behavior_state*), int16, child_entry*, void(__cdecl*)(int32, c_behavior_state*, int32), void(__cdecl*)(int32, c_behavior_state*, int32), void(__cdecl*)(int32, c_behavior_state*, int32), void(__cdecl*)(int32, c_behavior_state*), void(__cdecl*)(int32, c_behavior_state*, bool), void(__cdecl*)(int32, c_behavior_state*), void(__cdecl*)(c_behavior_state*))
//.text:01466C20 ; public: __cdecl Primitive_behavior::Primitive_behavior(int16, int32, int16(__cdecl*)(int32, const s_behavior_input*, const s_behavior_arguments*), int16(__cdecl*)(int32, c_behavior_state*, c_behavior_state*), bool(__cdecl*)(int32, c_behavior_state*), void(__cdecl*)(int32, c_behavior_state*), int16, bool(__cdecl*)(int32, c_behavior_state*), void(__cdecl*)(int32, c_behavior_state*), void(__cdecl*)(c_behavior_state*))
//.text:01466CA0 ; public: __cdecl Primitive_behavior::Primitive_behavior(int16, int32, int16(__cdecl*)(int32, const s_behavior_input*, const s_behavior_arguments*), int16(__cdecl*)(int32, c_behavior_state*, c_behavior_state*), bool(__cdecl*)(int32, c_behavior_state*), void(__cdecl*)(int32, c_behavior_state*), int16, bool(__cdecl*)(int32, c_behavior_state*), void(__cdecl*)(int32, c_behavior_state*), void(__cdecl*)(int32, c_behavior_state*, int32), void(__cdecl*)(int32, c_behavior_state*, int32), void(__cdecl*)(int32, c_behavior_state*, int32), void(__cdecl*)(int32, c_behavior_state*), void(__cdecl*)(int32, c_behavior_state*, bool), void(__cdecl*)(int32, c_behavior_state*), void(__cdecl*)(c_behavior_state*))
//.text:01466D20 ; 
//.text:01466D30 ; bool __cdecl action_flee_current_position_exposed(int32, c_flee_state_data*)
//.text:01466ED0 ; void __cdecl action_flee_find_flee_position(int32, bool, c_flee_state_data*)
//.text:01467110 ; void __cdecl action_investigate_control(int32, c_behavior_state*)
//.text:01467270 ; bool __cdecl action_investigate_perform(int32, c_behavior_state*)
//.text:014677F0 ; bool __cdecl action_postsearch_perform(int32, c_behavior_state*)
//.text:01467900 ; void __cdecl action_selection(int32)
//.text:01467C40 ; void __cdecl action_uncover_control(int32, c_behavior_state*)
//.text:01467E20 ; bool __cdecl action_uncover_perform(int32, c_behavior_state*)
//.text:01468130 ; bool __cdecl actor_action_find_escape_from_danger(int32, int16*, real32*, real_vector2d*, bool*)
//.text:01468670 ; bool __cdecl actor_action_try_to_dive(int32, int16, real32, real_vector2d*, real32)
//.text:01468880 ; bool __cdecl actor_assess_danger(int32, real32, bool*, bool*, bool*, bool*, real32*)
//.text:01468EC0 ; void __cdecl actor_attach_equipment(int32)
//.text:01468F90 ; Behavior_header* __cdecl actor_behavior_get(actor_datum*, int16)
//.text:01468FE0 ; 
//.text:01469010 ; 
//.text:014690C0 ; 
//.text:014690F0 ; 

//c_behavior_state* __cdecl actor_behavior_state_get(const actor_datum* actor, int16 layer_index)
uns8* __cdecl actor_behavior_state_get(const actor_datum* actor, int16 layer_index)
{
	return INVOKE(0x01469180, actor_behavior_state_get, actor, layer_index);
}

//.text:014691B0 ; int16 __cdecl actor_behavior_state_offset_get(actor_datum*, int16)
//.text:014691E0 ; bool __cdecl actor_can_grapple(int32)
//.text:01469270 ; bool __cdecl actor_executing(int32, int16)
//.text:01469290 ; int16 __cdecl actor_executing_layer(int32, int16)
//.text:01469320 ; void __cdecl actor_get_desired_follow_range(int32, int32, real32*, real32*)
//.text:014693B0 ; 
//.text:014693F0 ; void __cdecl actor_request_search(int32)
//.text:01469460 ; bool __cdecl actor_search_allowed(int32)
//.text:01469560 ; bool __cdecl actor_should_run(int32)
//.text:014695D0 ; bool __cdecl actor_should_search(int32)
//.text:01469640 ; void __cdecl actor_throw_equipment(int32, int16)
//.text:01469A70 ; void __cdecl ai_handle_deployed_turret(int32, int32)
//.text:01469B10 ; 
//.text:01469B50 ; bool __cdecl behavior_activity_activate(int32, c_behavior_state*)
//.text:01469C70 ; bool __cdecl behavior_activity_command_script_allows_activity(int32)
//.text:01469CF0 ; void __cdecl behavior_activity_deactivate(int32, c_behavior_state*)
//.text:01469D90 ; int16 __cdecl behavior_activity_decide(int32, int16, c_behavior_state*)
//.text:01469E60 ; bool __cdecl behavior_activity_default_activate(int32, c_behavior_state*)
//.text:01469ED0 ; void __cdecl behavior_activity_default_control(int32, c_behavior_state*)
//.text:01469F70 ; void __cdecl behavior_activity_default_deactivate(int32, c_behavior_state*)
//.text:01469FD0 ; int16 __cdecl behavior_activity_default_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0146A000 ; int16 __cdecl behavior_activity_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0146A060 ; int16 __cdecl behavior_activity_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0146A340 ; bool __cdecl behavior_alert_activate(int32, c_behavior_state*)
//.text:0146A390 ; int16 __cdecl behavior_alert_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0146A3A0 ; int16 __cdecl behavior_alert_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0146A3B0 ; bool __cdecl behavior_allowed_to_investigate(int32, int32)
//.text:0146A500 ; bool __cdecl behavior_ambush_activate(int32, c_behavior_state*)
//.text:0146A510 ; void __cdecl behavior_ambush_deactivate(int32, c_behavior_state*)
//.text:0146A520 ; int16 __cdecl behavior_ambush_decide(int32, int16, c_behavior_state*)
//.text:0146A550 ; int16 __cdecl behavior_ambush_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0146A560 ; bool __cdecl behavior_avoid_activate(int32, c_behavior_state*)
//.text:0146A5B0 ; void __cdecl behavior_avoid_control(int32, c_behavior_state*)
//.text:0146A6D0 ; 
//.text:0146ACC0 ; int16 __cdecl behavior_avoid_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0146AD60 ; int16 __cdecl behavior_avoid_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0146ADC0 ; bool __cdecl behavior_avoid_perform(int32, c_behavior_state*)
//.text:0146ADD0 ; bool __cdecl behavior_board_activate(int32, c_behavior_state*)
//.text:0146AE30 ; void __cdecl behavior_board_control(int32, c_behavior_state*)
//.text:0146AEA0 ; void __cdecl behavior_board_deactivate(int32, c_behavior_state*)
//.text:0146AF90 ; void __cdecl behavior_board_flush_object(int32, c_behavior_state*, int32)
//.text:0146AFB0 ; void __cdecl behavior_board_flush_prop(int32, c_behavior_state*, int32)
//.text:0146AFD0 ; int16 __cdecl behavior_board_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0146B160 ; int16 __cdecl behavior_board_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0146B320 ; bool __cdecl behavior_board_perform(int32, c_behavior_state*)
//.text:0146B6A0 ; bool __cdecl behavior_broken_activate(int32, c_behavior_state*)
//.text:0146B6C0 ; int16 __cdecl behavior_broken_decide(int32, int16, c_behavior_state*)
//.text:0146B6D0 ; int16 __cdecl behavior_broken_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0146B6E0 ; int32 __cdecl behavior_build_condition_flags(int32)
//.text:0146B7A0 ; bool __cdecl behavior_change(int32, int16, int16, c_behavior_state*)
//.text:0146B980 ; bool __cdecl behavior_check_friend_activate(int32, c_behavior_state*)
//.text:0146BB10 ; void __cdecl behavior_check_friend_control(int32, c_behavior_state*)
//.text:0146BC70 ; int16 __cdecl behavior_check_friend_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0146BD20 ; int16 __cdecl post_combat_behavior_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0146BD80 ; bool __cdecl postcombat_behavior_perform(int32, c_behavior_state*)
//.text:0146BE30 ; void __cdecl behavior_clear_layer_state(actor_datum*, int16)
//.text:0146BE80 ; bool __cdecl behavior_combat_activate(int32, c_behavior_state*)
//.text:0146BED0 ; int16 __cdecl behavior_combat_decide(int32, int16, c_behavior_state*)
//.text:0146BEE0 ; int16 __cdecl behavior_combat_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0146BF40 ; int16 __cdecl behavior_combat_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0146BFB0 ; void __cdecl behavior_control(int32)
//.text:0146C040 ; int32 __cdecl behavior_coordinated_ambush_activate(int32, c_behavior_state*)
//.text:0146C0B0 ; int16 __cdecl behavior_coordinated_ambush_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0146C0C0 ; int16 __cdecl behavior_coordinated_ambush_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0146C140 ; int16 __cdecl behavior_coordinated_ambush_select_actors(int32, int32, c_behavior_state*)
//.text:0146C1E0 ; bool __cdecl behavior_cover_activate(int32, c_behavior_state*)
//.text:0146C360 ; void __cdecl behavior_cover_clear_non_deterministic_behavior_state(c_behavior_state*)
//.text:0146C370 ; void __cdecl behavior_cover_control(int32, c_behavior_state*)
//.text:0146C7A0 ; void __cdecl behavior_cover_deactivate(int32, c_behavior_state*)
//.text:0146C820 ; bool __cdecl behavior_cover_find_cover_point(int32, c_cover_state_data*)
//.text:0146C9B0 ; int16 __cdecl behavior_cover_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0146C9F0 ; bool __cdecl behavior_cover_navigate_to_cover_point(int32, firing_position_ref, firing_position*, c_cover_state_data*, path_state*, bool, int32)
//.text:0146CC20 ; int16 __cdecl behavior_cover_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0146CCC0 ; bool __cdecl behavior_cover_peek_activate(int32, c_behavior_state*)
//.text:0146CD40 ; void __cdecl behavior_cover_peek_control(int32, c_behavior_state*)
//.text:0146CDA0 ; void __cdecl behavior_cover_peek_deactivate(int32, c_behavior_state*)
//.text:0146CE10 ; int16 __cdecl behavior_cover_peek_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0146CF60 ; bool __cdecl behavior_cover_peek_perform(int32, c_behavior_state*)
//.text:0146CFD0 ; bool __cdecl behavior_cover_perform(int32, c_behavior_state*)
//.text:0146D1F0 ; bool __cdecl behavior_coverfriend_activate(int32, c_behavior_state*)
//.text:0146D2B0 ; void __cdecl behavior_coverfriend_control(int32, c_behavior_state*)
//.text:0146D480 ; void __cdecl behavior_coverfriend_flush_actor(int32, c_behavior_state*, int32)
//.text:0146D4A0 ; int16 __cdecl behavior_coverfriend_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0146D510 ; int16 __cdecl behavior_coverfriend_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0146D550 ; bool __cdecl behavior_cower_activate(int32, c_behavior_state*)
//.text:0146D660 ; void __cdecl behavior_cower_clear_non_deterministic_behavior_state(c_behavior_state*)
//.text:0146D670 ; int16 __cdecl behavior_cower_decide(int32, int16, c_behavior_state*)
//.text:0146D9A0 ; void __cdecl behavior_cower_flush_structure_indices(int32, c_behavior_state*)
//.text:0146D9B0 ; int16 __cdecl behavior_cower_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0146D9C0 ; int16 __cdecl behavior_cower_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0146DA20 ; bool __cdecl behavior_cure_isolation_activate(int32, c_behavior_state*)
//.text:0146DA40 ; void __cdecl behavior_cure_isolation_control(int32, c_behavior_state*)
//.text:0146DBF0 ; void __cdecl behavior_cure_isolation_debug(int32, c_behavior_state*)
//.text:0146DC00 ; int16 __cdecl behavior_cure_isolation_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0146DC40 ; int16 __cdecl behavior_cure_isolation_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0146DCB0 ; bool __cdecl behavior_cure_isolation_perform(int32, c_behavior_state*)
//.text:0146E2A0 ; int16 __cdecl behavior_decide_internal(int32, child_entry**, int16, int16, c_behavior_state*, c_behavior_state*, int16, int16*, int16*)
//.text:0146E5C0 ; int16 __cdecl behavior_decide_prioritized(int32, int16, c_behavior_state*)
//.text:0146E700 ; int16 __cdecl behavior_decide_sequential(int32, int16, c_behavior_state*)
//.text:0146E830 ; int16 __cdecl behavior_decide_sequential_looping(int32, int16, c_behavior_state*)
//.text:0146E9A0 ; int16 __cdecl behavior_decide_shoot_corpse(int32, int16, c_behavior_state*)
//.text:0146EB60 ; bool __cdecl behavior_deploy_turret_activate(int32, c_behavior_state*)
//.text:0146EB90 ; void __cdecl behavior_deploy_turret_control(int32, c_behavior_state*)
//.text:0146EC00 ; void __cdecl behavior_deploy_turret_flush_object(int32, c_behavior_state*, int32)
//.text:0146EC20 ; int16 __cdecl behavior_deploy_turret_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0146EC70 ; int16 __cdecl behavior_deploy_turret_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0146EE90 ; bool __cdecl behavior_deploy_turret_perform(int32, c_behavior_state*)
//.text:0146F140 ; void __cdecl behavior_deploy_turret_setup(c_behavior_state*, int32)
//.text:0146F170 ; bool __cdecl behavior_destroy_cover_activate(int32, c_behavior_state*)
//.text:0146F180 ; void __cdecl behavior_destroy_cover_control(int32, c_behavior_state*)
//.text:0146F210 ; int16 __cdecl behavior_destroy_cover_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0146F2B0 ; int16 __cdecl behavior_destroy_cover_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0146F2E0 ; bool __cdecl behavior_destroy_cover_perform(int32, c_behavior_state*)
//.text:0146F2F0 ; bool __cdecl behavior_destroy_obstacle_activate(int32, c_behavior_state*)
//.text:0146F340 ; void __cdecl behavior_destroy_obstacle_control(int32, c_behavior_state*)
//.text:0146F400 ; int16 __cdecl behavior_destroy_obstacle_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0146F480 ; int16 __cdecl behavior_destroy_obstacle_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0146F500 ; bool __cdecl behavior_destroy_obstacle_perform(int32, c_behavior_state*)
//.text:0146F620 ; int32 __cdecl behavior_emerge_activate(int32, c_behavior_state*)
//.text:0146F6A0 ; void __cdecl behavior_emerge_control(int32, c_behavior_state*, joint_state_datum*)
//.text:0146F790 ; int16 __cdecl behavior_emerge_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0146F7A0 ; int16 __cdecl behavior_emerge_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0146F920 ; bool __cdecl behavior_emerge_perform(int32, c_behavior_state*, joint_state_datum*)
//.text:0146F930 ; int16 __cdecl behavior_emerge_select_actors(int32, int32, c_behavior_state*)
//.text:0146FAE0 ; bool __cdecl behavior_engage_activate(int32, c_behavior_state*)
//.text:0146FB00 ; int16 __cdecl behavior_engage_decide(int32, int16, c_behavior_state*)
//.text:0146FE10 ; int16 __cdecl behavior_engage_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0146FE70 ; int16 __cdecl behavior_engage_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0146FED0 ; bool __cdecl behavior_engage_sync_activate(int32, c_behavior_state*)
//.text:0146FF40 ; void __cdecl behavior_engage_sync_control(int32, c_behavior_state*)
//.text:0146FFB0 ; int16 __cdecl behavior_engage_sync_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:01470040 ; int16 __cdecl behavior_engage_sync_next(int32, c_behavior_state*, c_behavior_state*)
//.text:01470070 ; bool __cdecl behavior_engage_sync_perform(int32, c_behavior_state*)
//.text:014701A0 ; 
//.text:01470290 ; 
//.text:01470360 ; 
//.text:01470370 ; 
//.text:01470380 ; 
//.text:014703E0 ; bool __cdecl behavior_enter_vehicle_activate(int32, c_behavior_state*)
//.text:014704C0 ; void __cdecl behavior_enter_vehicle_control(int32, c_behavior_state*)
//.text:014706A0 ; int16 __cdecl behavior_enter_vehicle_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:01470720 ; int16 __cdecl behavior_enter_vehicle_next(int32, c_behavior_state*, c_behavior_state*)
//.text:014707A0 ; bool __cdecl behavior_enter_vehicle_perform(int32, c_behavior_state*)
//.text:01470AB0 ; bool __cdecl behavior_fight_activate(int32, c_behavior_state*)
//.text:01470B60 ; void __cdecl behavior_fight_clear_non_deterministic_behavior_state(c_behavior_state*)
//.text:01470B70 ; void __cdecl behavior_fight_control(int32, c_behavior_state*)
//.text:01470D00 ; int16 __cdecl behavior_fight_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:01470D40 ; int16 __cdecl behavior_fight_next(int32, c_behavior_state*, c_behavior_state*)
//.text:01470E10 ; bool __cdecl behavior_fight_perform(int32, c_behavior_state*)
//.text:01471040 ; void __cdecl behavior_fight_positioning_control(int32, c_behavior_state*)
//.text:01471080 ; int16 __cdecl behavior_fight_positioning_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:014710B0 ; int16 __cdecl behavior_find_layer(actor_datum*, int16, int16)
//.text:01471180 ; bool __cdecl behavior_flank_activate(int32, c_behavior_state*)
//.text:014711A0 ; void __cdecl behavior_flank_control(int32, c_behavior_state*)
//.text:01471230 ; void __cdecl behavior_flank_deactivate(int32, c_behavior_state*)
//.text:01471240 ; int16 __cdecl behavior_flank_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:014712C0 ; int16 __cdecl behavior_flank_next(int32, c_behavior_state*, c_behavior_state*)
//.text:014713B0 ; bool __cdecl behavior_flank_perform(int32, c_behavior_state*)
//.text:01471610 ; bool __cdecl behavior_flee_activate(int32, c_behavior_state*)
//.text:014716D0 ; void __cdecl behavior_flee_control(int32, c_behavior_state*)
//.text:014718A0 ; void __cdecl behavior_flee_deactivate(int32, c_behavior_state*)
//.text:014718B0 ; int16 __cdecl behavior_flee_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:01471930 ; int16 __cdecl behavior_flee_next(int32, c_behavior_state*, c_behavior_state*)
//.text:01471960 ; bool __cdecl behavior_flee_perform(int32, c_behavior_state*)
//.text:01471AE0 ; bool __cdecl behavior_flee_setup(c_flee_state_data*, int32, int32, int32)
//.text:01471B00 ; bool __cdecl behavior_flight_wander_activate(int32, c_behavior_state*)
//.text:01471B50 ; void __cdecl behavior_flight_wander_control(int32, c_behavior_state*)
//.text:01471BA0 ; int16 __cdecl behavior_flight_wander_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:01471BB0 ; int16 __cdecl behavior_flight_wander_next(int32, c_behavior_state*, c_behavior_state*)
//.text:01471BE0 ; bool __cdecl behavior_flight_wander_perform(int32, c_behavior_state*)
//.text:01471CC0 ; bool __cdecl behavior_follow_activate(int32, c_behavior_state*)
//.text:01471CE0 ; void __cdecl behavior_follow_control(int32, c_behavior_state*)
//.text:01471D60 ; int16 __cdecl behavior_follow_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:01471F70 ; int16 __cdecl behavior_follow_next(int32, c_behavior_state*, c_behavior_state*)
//.text:01472010 ; bool __cdecl behavior_follow_perform(int32, c_behavior_state*)
//.text:01472260 ; int32 __cdecl behavior_formation_activate(int32, c_behavior_state*)
//.text:01472360 ; void __cdecl behavior_formation_control(int32, c_behavior_state*, joint_state_datum*)
//.text:01472690 ; int16 __cdecl behavior_formation_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:014726D0 ; int16 __cdecl behavior_formation_next(int32, c_behavior_state*, c_behavior_state*)
//.text:01472720 ; bool __cdecl behavior_formation_perform(int32, c_behavior_state*, joint_state_datum*)
//.text:01472E10 ; int16 __cdecl behavior_formation_select_actors(int32, int32, c_behavior_state*)
//.text:01472F30 ; void __cdecl behavior_formation_setup(int32, int16, int32, int32, int32, c_formation_data*)
//.text:01472F60 ; bool __cdecl behavior_go_to_activate(int32, c_behavior_state*)
//.text:01472F80 ; int16 __cdecl behavior_go_to_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:01472F90 ; int16 __cdecl behavior_go_to_next(int32, c_behavior_state*, c_behavior_state*)
//.text:01472FC0 ; bool __cdecl behavior_go_to_perform(int32, c_behavior_state*)
//.text:01473120 ; bool __cdecl behavior_grapple_activate(int32, c_behavior_state*)
//.text:01473180 ; void __cdecl behavior_grapple_control(int32, c_behavior_state*)
//.text:01473310 ; void __cdecl behavior_grapple_deactivate(int32, c_behavior_state*)
//.text:01473350 ; void __cdecl behavior_grapple_flush_object(int32, c_behavior_state*, int32)
//.text:01473360 ; void __cdecl behavior_grapple_flush_prop(int32, c_behavior_state*, int32)
//.text:01473380 ; int16 __cdecl behavior_grapple_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:01473530 ; int16 __cdecl behavior_grapple_next(int32, c_behavior_state*, c_behavior_state*)
//.text:014736D0 ; bool __cdecl behavior_grapple_perform(int32, c_behavior_state*)
//.text:014739A0 ; bool __cdecl behavior_guard_activate(int32, c_behavior_state*)
//.text:01473A70 ; void __cdecl behavior_guard_control(int32, c_behavior_state*)
//.text:01473BF0 ; void __cdecl behavior_guard_flush_firing_position_indices(int32, c_behavior_state*, bool)
//.text:01473C20 ; int16 __cdecl behavior_guard_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:01473C30 ; int16 __cdecl behavior_guard_next(int32, c_behavior_state*, c_behavior_state*)
//.text:01473C70 ; bool __cdecl behavior_guard_perform(int32, c_behavior_state*)
//.text:01474160 ; int32 __cdecl behavior_huddle_activate(int32, c_behavior_state*)
//.text:01474250 ; void __cdecl behavior_huddle_control(int32, c_behavior_state*, joint_state_datum*)
//.text:014742F0 ; void __cdecl behavior_huddle_deactivate(int32, c_behavior_state*, joint_state_datum*)
//.text:01474310 ; void __cdecl behavior_huddle_debug(int32, c_behavior_state*, joint_state_datum*)
//.text:01474320 ; void __cdecl behavior_huddle_flush_firing_points(int32, c_behavior_state*, bool)
//.text:01474360 ; int16 __cdecl behavior_huddle_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:014743A0 ; int16 __cdecl behavior_huddle_next(int32, c_behavior_state*, c_behavior_state*)
//.text:014743B0 ; bool __cdecl behavior_huddle_perform(int32, c_behavior_state*, joint_state_datum*)
//.text:01474550 ; bool __cdecl behavior_idle_activate(int32, c_behavior_state*)
//.text:014745A0 ; int16 __cdecl behavior_idle_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:014745B0 ; int16 __cdecl behavior_idle_next(int32, c_behavior_state*, c_behavior_state*)

int16 __cdecl behavior_index_by_name(const char* name)
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
//.text:01474CB0 ; bool __cdecl behavior_investigate_activate(int32, c_behavior_state*)
//.text:01474E20 ; void __cdecl behavior_investigate_clear_non_deterministic_behavior_state(c_behavior_state*)
//.text:01474E30 ; int16 __cdecl behavior_investigate_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:01474EB0 ; int16 __cdecl behavior_investigate_next(int32, c_behavior_state*, c_behavior_state*)
//.text:01474F80 ; bool __cdecl behavior_kamikaze_activate(int32, c_behavior_state*)
//.text:01474FB0 ; void __cdecl behavior_kamikaze_control(int32, c_behavior_state*)
//.text:014750B0 ; bool __cdecl behavior_kamikaze_create_grenades(int32, c_kamikaze_state_data*)
//.text:01475240 ; void __cdecl behavior_kamikaze_deactivate(int32, c_behavior_state*)
//.text:01475250 ; void __cdecl behavior_kamikaze_flush_object(int32, c_behavior_state*, int32)
//.text:014752A0 ; int16 __cdecl behavior_kamikaze_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:014752E0 ; int16 __cdecl behavior_kamikaze_next(int32, c_behavior_state*, c_behavior_state*)
//.text:01475330 ; bool __cdecl behavior_kamikaze_perform(int32, c_behavior_state*)
//.text:01475430 ; bool __cdecl behavior_leap_on_cover_activate(int32, c_behavior_state*)
//.text:014754D0 ; void __cdecl behavior_leap_on_cover_clear_non_deterministic_behavior_state(c_behavior_state*)
//.text:014754E0 ; void __cdecl behavior_leap_on_cover_control(int32, c_behavior_state*)
//.text:014755C0 ; void __cdecl behavior_leap_on_cover_deactivate(int32, c_behavior_state*)
//.text:014755F0 ; void __cdecl behavior_leap_on_cover_flush_structure_indices(int32, c_behavior_state*)
//.text:01475600 ; int16 __cdecl behavior_leap_on_cover_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:01475750 ; int16 __cdecl behavior_leap_on_cover_next(int32, c_behavior_state*, c_behavior_state*)
//.text:014758D0 ; bool __cdecl behavior_leap_on_cover_perform(int32, c_behavior_state*)
//.text:014759B0 ; bool __cdecl behavior_melee_charge_activate(int32, c_behavior_state*)
//.text:01475B00 ; void __cdecl behavior_melee_charge_control(int32, c_behavior_state*)
//.text:01475B90 ; void __cdecl behavior_melee_charge_deactivate(int32, c_behavior_state*)
//.text:01475BD0 ; int16 __cdecl behavior_melee_charge_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:01475F10 ; bool __cdecl behavior_melee_charge_perform(int32, c_behavior_state*)
//.text:014769B0 ; bool __cdecl behavior_melee_leaping_charge_activate(int32, c_behavior_state*)
//.text:01476A10 ; void __cdecl behavior_melee_leaping_charge_control(int32, c_behavior_state*)
//.text:01476AD0 ; void __cdecl behavior_melee_leaping_charge_deactivate(int32, c_behavior_state*)
//.text:01476B10 ; int16 __cdecl behavior_melee_leaping_charge_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:01476D10 ; bool __cdecl behavior_melee_leaping_charge_perform(int32, c_behavior_state*)
//.text:01477640 ; int16 __cdecl behavior_null_discrete_next(int32, c_behavior_state*, c_behavior_state*)
//.text:01477650 ; int16 __cdecl behavior_null_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:01477660 ; int16 __cdecl behavior_null_next(int32, c_behavior_state*, c_behavior_state*)
//.text:01477670 ; bool __cdecl behavior_obey_activate(int32, c_behavior_state*)
//.text:01477680 ; void __cdecl behavior_obey_deactivate(int32, c_behavior_state*)
//.text:014776D0 ; int16 __cdecl behavior_obey_decide(int32, int16, c_behavior_state*)
//.text:01477840 ; int16 __cdecl behavior_obey_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:014778B0 ; int16 __cdecl behavior_obey_next(int32, c_behavior_state*, c_behavior_state*)
//.text:01477900 ; bool __cdecl behavior_patrol_activate(int32, c_behavior_state*)
//.text:014779E0 ; void __cdecl behavior_patrol_debug(int32, c_behavior_state*)
//.text:014779F0 ; int16 __cdecl behavior_patrol_decide(int32, int16, c_behavior_state*)
//.text:01477D80 ; int16 __cdecl behavior_patrol_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:01477DC0 ; bool __cdecl behavior_perch_activate(int32, c_behavior_state*)
//.text:01477EB0 ; void __cdecl behavior_perch_control(int32, c_behavior_state*)
//.text:01477F30 ; void __cdecl behavior_perch_flush_firing_position_indices(int32, c_behavior_state*, bool)
//.text:01477F60 ; int16 __cdecl behavior_perch_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:01477FF0 ; int16 __cdecl behavior_perch_next(int32, c_behavior_state*, c_behavior_state*)
//.text:014781C0 ; bool __cdecl behavior_perch_perform(int32, c_behavior_state*)
//.text:01478360 ; bool __cdecl behavior_perimeter_activate(int32, c_behavior_state*)
//.text:014784B0 ; void __cdecl behavior_perimeter_control(int32, c_behavior_state*)
//.text:01478590 ; int16 __cdecl behavior_perimeter_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:01478720 ; int16 __cdecl behavior_perimeter_next(int32, c_behavior_state*, c_behavior_state*)
//.text:01478A60 ; bool __cdecl behavior_perimeter_perform(int32, c_behavior_state*)
//.text:01478D30 ; bool __cdecl behavior_pickup_player_activate(int32, c_behavior_state*)
//.text:01478D90 ; void __cdecl behavior_pickup_player_control(int32, c_behavior_state*)
//.text:01478DA0 ; int16 __cdecl behavior_pickup_player_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:01478E40 ; int16 __cdecl behavior_pickup_player_next(int32, c_behavior_state*, c_behavior_state*)
//.text:01478E90 ; bool __cdecl behavior_pickup_player_perform(int32, c_behavior_state*)
//.text:01478EA0 ; bool __cdecl behavior_postcombat_activate(int32, c_behavior_state*)
//.text:01478F50 ; void __cdecl behavior_postcombat_approach_flush_actor(int32, c_behavior_state*, int32)
//.text:01478F70 ; void __cdecl behavior_postcombat_clear_non_deterministic_behavior_state(c_behavior_state*)
//.text:01478F80 ; int16 __cdecl behavior_postcombat_decide(int32, int16, c_behavior_state*)
//.text:01479070 ; void __cdecl behavior_postcombat_flush_prop(int32, c_behavior_state*, int32)
//.text:01479090 ; int16 __cdecl behavior_postcombat_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:01479100 ; int16 __cdecl behavior_postcombat_next(int32, c_behavior_state*, c_behavior_state*)
//.text:01479120 ; bool __cdecl behavior_postombat_approach_activate(int32, c_behavior_state*)
//.text:01479170 ; void __cdecl behavior_postombat_approach_control(int32, c_behavior_state*)
//.text:01479180 ; int16 __cdecl behavior_postombat_approach_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:014791D0 ; int16 __cdecl behavior_postombat_approach_next(int32, c_behavior_state*, c_behavior_state*)
//.text:01479250 ; bool __cdecl behavior_postombat_approach_perform(int32, c_behavior_state*)
//.text:014794D0 ; bool __cdecl behavior_postpostcombat_activate(int32, c_behavior_state*)
//.text:01479520 ; bool __cdecl behavior_postsearch_activate(int32, c_behavior_state*)
//.text:01479540 ; void __cdecl behavior_postsearch_control(int32, c_behavior_state*)
//.text:01479570 ; int16 __cdecl behavior_postsearch_next(int32, c_behavior_state*, c_behavior_state*)
//.text:014795C0 ; bool __cdecl behavior_posture_activate(int32, c_behavior_state*)
//.text:01479760 ; void __cdecl behavior_posture_control(int32, c_behavior_state*)
//.text:01479770 ; void __cdecl behavior_posture_deactivate(int32, c_behavior_state*)
//.text:01479780 ; int16 __cdecl behavior_posture_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:014797F0 ; int16 __cdecl behavior_posture_next(int32, c_behavior_state*, c_behavior_state*)
//.text:01479840 ; bool __cdecl behavior_posture_perform(int32, c_behavior_state*)
//.text:014798D0 ; int32 __cdecl behavior_presearch_activate(int32, c_behavior_state*)
//.text:01479BB0 ; void __cdecl behavior_presearch_deactivate(int32, c_behavior_state*, joint_state_datum*)
//.text:01479C50 ; int16 __cdecl behavior_presearch_decide(int32, int16, c_behavior_state*, joint_state_datum*)
//.text:01479E20 ; void __cdecl behavior_presearch_flush_prop(int32, c_behavior_state*, int32)
//.text:01479E80 ; int16 __cdecl behavior_presearch_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:01479FB0 ; int16 __cdecl behavior_presearch_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147A1D0 ; bool __cdecl behavior_pursuit_activate(int32, c_behavior_state*)
//.text:0147A230 ; void __cdecl behavior_pursuit_clear_non_deterministic_behavior_state(c_behavior_state*)
//.text:0147A240 ; void __cdecl behavior_pursuit_control(int32, c_behavior_state*)
//.text:0147A310 ; int16 __cdecl behavior_pursuit_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147A350 ; int16 __cdecl behavior_pursuit_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147A410 ; int32 __cdecl behavior_pursuit_sync_activate(int32, c_behavior_state*)
//.text:0147A6B0 ; void __cdecl behavior_pursuit_sync_control(int32, c_behavior_state*, joint_state_datum*)
//.text:0147A6E0 ; void __cdecl behavior_pursuit_sync_flush_object(int32, c_behavior_state*, int32)
//.text:0147A720 ; void __cdecl behavior_pursuit_sync_flush_structure_indices(int32, c_behavior_state*)
//.text:0147A760 ; int16 __cdecl behavior_pursuit_sync_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147A770 ; int16 __cdecl behavior_pursuit_sync_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147A940 ; int16 __cdecl behavior_pursuit_sync_select_actors(int32, int32, c_behavior_state*)
//.text:0147AA80 ; bool __cdecl behavior_ready_activate(int32, c_behavior_state*)
//.text:0147AB70 ; void __cdecl behavior_ready_deactivate(int32, c_behavior_state*)
//.text:0147AB80 ; int16 __cdecl behavior_ready_decide(int32, int16, c_behavior_state*)
//.text:0147ABF0 ; int16 __cdecl behavior_ready_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147AC00 ; int16 __cdecl behavior_ready_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147AC30 ; bool __cdecl behavior_retreat_activate(int32, c_behavior_state*)
//.text:0147ACD0 ; void __cdecl behavior_retreat_deactivate(int32, c_behavior_state*)
//.text:0147AD10 ; int16 __cdecl behavior_retreat_decide(int32, int16, c_behavior_state*)
//.text:0147AD60 ; int16 __cdecl behavior_retreat_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147AD70 ; int16 __cdecl behavior_retreat_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147ADD0 ; int16 __cdecl behavior_root_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147ADE0 ; int16 __cdecl behavior_root_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147ADF0 ; bool __cdecl behavior_search_activate(int32, c_behavior_state*)
//.text:0147AF10 ; void __cdecl behavior_search_deactivate(int32, c_behavior_state*)
//.text:0147AF50 ; int16 __cdecl behavior_search_decide(int32, int16, c_behavior_state*)
//.text:0147B110 ; int16 __cdecl behavior_search_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147B190 ; int16 __cdecl behavior_search_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147B2B0 ; void __cdecl behavior_search_sync_control(int32, c_behavior_state*)
//.text:0147B2F0 ; int16 __cdecl behavior_search_sync_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147B320 ; int16 __cdecl behavior_search_sync_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147B360 ; bool __cdecl behavior_self_preservation_activate(int32, c_behavior_state*)
//.text:0147B3E0 ; int16 __cdecl behavior_self_preservation_decide(int32, int16, c_behavior_state*)
//.text:0147B480 ; int16 __cdecl behavior_self_preservation_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147B490 ; int16 __cdecl behavior_self_preservation_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147B570 ; int16 __cdecl behavior_self_preserve_time_left(int32)
//.text:0147B640 ; bool __cdecl behavior_shoot_corpse_activate(int32, c_behavior_state*)
//.text:0147B6F0 ; int16 __cdecl behavior_shoot_corpse_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147B790 ; int16 __cdecl behavior_shoot_corpse_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147B7E0 ; bool __cdecl behavior_sleep_activate(int32, c_behavior_state*)
//.text:0147B910 ; void __cdecl behavior_sleep_control(int32, c_behavior_state*)
//.text:0147B950 ; void __cdecl behavior_sleep_deactivate(int32, c_behavior_state*)
//.text:0147B9A0 ; int16 __cdecl behavior_sleep_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147B9D0 ; int16 __cdecl behavior_sleep_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147B9E0 ; bool __cdecl behavior_sleep_perform(int32, c_behavior_state*)
//.text:0147BA90 ; bool __cdecl behavior_squad_patrol_activate(int32, c_behavior_state*)
//.text:0147BAD0 ; void __cdecl behavior_squad_patrol_control(int32, c_behavior_state*)
//.text:0147BC10 ; void __cdecl behavior_squad_patrol_deactivate(int32, c_behavior_state*)
//.text:0147BC40 ; int16 __cdecl behavior_squad_patrol_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147BC90 ; int16 __cdecl behavior_squad_patrol_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147BCE0 ; bool __cdecl behavior_squad_patrol_perform(int32, c_behavior_state*)
//.text:0147C0A0 ; 
//.text:0147C0E0 ; 
//.text:0147C120 ; bool __cdecl behavior_stalk_activate(int32, c_behavior_state*)
//.text:0147C180 ; void __cdecl behavior_stalk_control(int32, c_behavior_state*)
//.text:0147C230 ; void __cdecl behavior_stalk_deactivate(int32, c_behavior_state*)
//.text:0147C270 ; void __cdecl behavior_stalk_flush_prop(int32, c_behavior_state*, int32)
//.text:0147C290 ; int16 __cdecl behavior_stalk_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147C2C0 ; int16 __cdecl behavior_stalk_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147C3B0 ; bool __cdecl behavior_stalk_perform(int32, c_behavior_state*)
//.text:0147C5B0 ; void __cdecl behavior_state_fail_child(c_behavior_state*)
//.text:0147C5D0 ; bool __cdecl behavior_stunned_activate(int32, c_behavior_state*)
//.text:0147C6D0 ; void __cdecl behavior_stunned_control(int32, c_behavior_state*)
//.text:0147C780 ; int16 __cdecl behavior_stunned_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147C7C0 ; int16 __cdecl behavior_stunned_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147C7F0 ; bool __cdecl behavior_suppress_activate(int32, c_behavior_state*)
//.text:0147C9A0 ; void __cdecl behavior_suppress_clear_non_deterministic_behavior_state(c_behavior_state*)
//.text:0147C9B0 ; void __cdecl behavior_suppress_control(int32, c_behavior_state*)
//.text:0147CB10 ; int16 __cdecl behavior_suppress_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147CBF0 ; int16 __cdecl behavior_suppress_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147CCD0 ; int32 __cdecl behavior_surprise_activate(int32, c_behavior_state*)
//.text:0147CE10 ; void __cdecl behavior_surprise_control(int32, c_behavior_state*, joint_state_datum*)
//.text:0147CE60 ; void __cdecl behavior_surprise_flush_object(int32, c_behavior_state*, int32)
//.text:0147CEA0 ; int16 __cdecl behavior_surprise_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147D130 ; int16 __cdecl behavior_surprise_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147D270 ; bool __cdecl behavior_surprise_perform(int32, c_behavior_state*, joint_state_datum*)
//.text:0147D340 ; bool __cdecl behavior_uncover_activate(int32, c_behavior_state*)
//.text:0147D3B0 ; void __cdecl behavior_uncover_clear_non_deterministic_behavior_state(c_behavior_state*)
//.text:0147D3C0 ; void __cdecl behavior_uncover_deactivate(int32, c_behavior_state*)
//.text:0147D3D0 ; int16 __cdecl behavior_uncover_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147D480 ; int16 __cdecl behavior_uncover_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147D4B0 ; int16 __cdecl behavior_update_running_child(int32, int16, c_behavior_state*, int16*)
//.text:0147D6A0 ; bool __cdecl behavior_vehicle_avoid_activate(int32, c_behavior_state*)
//.text:0147D6B0 ; void __cdecl behavior_vehicle_avoid_control(int32, c_behavior_state*)
//.text:0147D7B0 ; int16 __cdecl behavior_vehicle_avoid_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147D860 ; int16 __cdecl behavior_vehicle_avoid_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147D930 ; bool __cdecl behavior_vehicle_avoid_perform(int32, c_behavior_state*)
//.text:0147D940 ; bool __cdecl behavior_vehicle_charge_activate(int32, c_behavior_state*)
//.text:0147DA20 ; void __cdecl behavior_vehicle_charge_control(int32, c_behavior_state*)
//.text:0147DAC0 ; void __cdecl behavior_vehicle_charge_deactivate(int32, c_behavior_state*)
//.text:0147DB00 ; int16 __cdecl behavior_vehicle_charge_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147DCA0 ; int16 __cdecl behavior_vehicle_charge_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147DEE0 ; bool __cdecl behavior_vehicle_charge_perform(int32, c_behavior_state*)
//.text:0147DF90 ; bool __cdecl behavior_vehicle_cover_activate(int32, c_behavior_state*)
//.text:0147DFB0 ; void __cdecl behavior_vehicle_cover_control(int32, c_behavior_state*)
//.text:0147E1B0 ; int16 __cdecl behavior_vehicle_cover_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147E1F0 ; int16 __cdecl behavior_vehicle_cover_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147E310 ; bool __cdecl behavior_vehicle_cover_perform(int32, c_behavior_state*)
//.text:0147E700 ; bool __cdecl behavior_vehicle_fight_activate(int32, c_behavior_state*)
//.text:0147E710 ; void __cdecl behavior_vehicle_fight_control(int32, c_behavior_state*)
//.text:0147E8A0 ; int16 __cdecl behavior_vehicle_fight_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147E910 ; int16 __cdecl behavior_vehicle_fight_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147E920 ; bool __cdecl behavior_vehicle_fight_perform(int32, c_behavior_state*)
//.text:0147EB60 ; int32 __cdecl behavior_vehicle_flip_activate(int32, c_behavior_state*)
//.text:0147ECA0 ; void __cdecl behavior_vehicle_flip_control(int32, c_behavior_state*, joint_state_datum*)
//.text:0147ED80 ; void __cdecl behavior_vehicle_flip_deactivate(int32, c_behavior_state*, joint_state_datum*)
//.text:0147EDA0 ; void __cdecl behavior_vehicle_flip_debug(int32, c_behavior_state*, joint_state_datum*)
//.text:0147EDB0 ; void __cdecl behavior_vehicle_flip_flush_object(int32, c_behavior_state*, int32)
//.text:0147EDF0 ; void __cdecl behavior_vehicle_flip_flush_structure_indices(int32, c_behavior_state*)
//.text:0147EE10 ; int16 __cdecl behavior_vehicle_flip_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147EE50 ; int16 __cdecl behavior_vehicle_flip_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147EED0 ; bool __cdecl behavior_vehicle_flip_perform(int32, c_behavior_state*, joint_state_datum*)
//.text:0147F1E0 ; int16 __cdecl behavior_vehicle_flip_select_actors(int32, int32, c_behavior_state*)
//.text:0147F320 ; bool __cdecl behavior_vehicle_pickup_activate(int32, c_behavior_state*)
//.text:0147F360 ; void __cdecl behavior_vehicle_pickup_control(int32, c_behavior_state*)
//.text:0147F3B0 ; int16 __cdecl behavior_vehicle_pickup_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147F3D0 ; int16 __cdecl behavior_vehicle_pickup_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147F4A0 ; bool __cdecl behavior_vehicle_pickup_perform(int32, c_behavior_state*)
//.text:0147F830 ; bool __cdecl behavior_vehicle_ram_activate(int32, c_behavior_state*)
//.text:0147FA20 ; void __cdecl behavior_vehicle_ram_control(int32, c_behavior_state*)
//.text:0147FAF0 ; int16 __cdecl behavior_vehicle_ram_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147FBE0 ; int16 __cdecl behavior_vehicle_ram_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147FCB0 ; bool __cdecl behavior_vehicle_ram_perform(int32, c_behavior_state*)
//.text:0147FDC0 ; bool __cdecl behavior_wander_activate(int32, c_behavior_state*)
//.text:0147FDE0 ; void __cdecl behavior_wander_control(int32, c_behavior_state*)
//.text:0147FE20 ; int16 __cdecl behavior_wander_get_desire(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:0147FEC0 ; int16 __cdecl behavior_wander_next(int32, c_behavior_state*, c_behavior_state*)
//.text:0147FF10 ; bool __cdecl behavior_wander_perform(int32, c_behavior_state*)

void __cdecl behaviors_initialize()
{
	INVOKE(0x01480210, behaviors_initialize);
}

//.text:01480340 ; int16 __cdecl berserk_get_impulse(int32, c_behavior_state*)
//.text:01480360 ; int16 __cdecl berserk_wander_get_impulse(int32, c_behavior_state*)
//.text:014803C0 ; bool __cdecl board_activate_helper(int32, c_vehicle_board_data*)
//.text:014804B0 ; 
//.text:014804E0 ; bool __cdecl boarding_find_seat(int32, int32, int32*, int16*, int16*, bool*)
//.text:014806E0 ; 
//.text:01480700 ; 
//.text:01480950 ; int16 __cdecl broken_berserk_impulse_get_impulse(int32, c_behavior_state*)
//.text:014809B0 ; int16 __cdecl broken_flee_impulse_get_impulse(int32, c_behavior_state*)
//.text:01480A40 ; int16 __cdecl broken_kamikaze_impulse_get_impulse(int32, c_behavior_state*)
//.text:01480B50 ; int16 __cdecl broken_scatter_get_impulse(int32, c_behavior_state*)
//.text:01480BB0 ; int16 __cdecl charge_children_next(int32, c_behavior_state*, c_behavior_state*)
//.text:01480C60 ; 
//.text:01480D30 ; void __cdecl charge_movement_helper(int32, c_charge_state_data*, real32, bool*, bool*, bool*)
//.text:01480E10 ; 
//.text:01480E30 ; child_entry** __cdecl child_list_get(int32, int16, int16*)
//.text:01480E90 ; 
//.text:01480EC0 ; bool __cdecl command_script_allows_activity_termination(int32)
//.text:01480F20 ; int16 __cdecl construct_child_list_internal(child_entry**, custom_child_list*, child_entry*, int16)
//.text:01481000 ; bool __cdecl cover_friend_continue(int32, int32, int16)
//.text:014810C0 ; bool __cdecl cover_point_exposed(int32, c_cover_state_data*, prop_ref_datum*)
//.text:01481140 ; int16 __cdecl coverme_investigate_get_impulse(int32, c_behavior_state*)
//.text:014813A0 ; bool __cdecl cure_isolation_attached_vehicle(int32, const seat_entry_info*, c_cure_isolation_data*, path_state*)
//.text:01481590 ; bool __cdecl cure_isolation_jump(int32, path_state*, real32, real32)
//.text:01481990 ; int16 __cdecl cure_isolation_test_vector(int32, const real_vector3d*, const real_vector3d*, const path_state*, real32, real32, real32*)
//.text:01481D70 ; bool __cdecl cure_isolation_undirected(int32, path_state*, real32, real32)
//.text:01482080 ; bool __cdecl cure_isolation_visible_enemy(int32, path_state*, real32, real32)
//.text:014823C0 ; int16 __cdecl damage_vehicle_cover_get_impulse(int32, c_behavior_state*)
//.text:014824D0 ; int16 __cdecl danger_cover_get_impulse(int32, c_behavior_state*)
//.text:014826D0 ; int16 __cdecl danger_crouch_get_impulse(int32, c_behavior_state*)
//.text:01482780 ; int16 __cdecl danger_vehicle_exit_get_impulse(int32, c_behavior_state*)
//.text:014828C0 ; int16 __cdecl dive_get_impulse(int32, c_behavior_state*)
//.text:01482C40 ; bool __cdecl emerge_wait_for_actor(int32, int32, real32*, int16*)
//.text:01482D50 ; 
//.text:01482D80 ; 
//.text:01483040 ; 
//.text:01483280 ; 
//.text:01483310 ; int16 __cdecl equipment_get_impulse(int32, const s_behavior_input*, const s_behavior_arguments*)
//.text:01483600 ; int16 __cdecl exposed_rear_cover_get_impulse(int32, c_behavior_state*)
//.text:014836F0 ; bool __cdecl fight_choose_fight_point(int32, c_fight_state_data*)
//.text:01483960 ; int16 __cdecl fight_predict_visibility(int32, const real_point3d*, int32, int16)
//.text:01483AD0 ; void __cdecl fight_reset_firing_position_timer(int32, c_fight_state_data*)
//.text:01483B90 ; bool __cdecl fighting_perception_valid(int32)
//.text:01483C30 ; bool __cdecl find_pursuit_position(int32, int32)
//.text:01483EE0 ; 
//.text:01483EF0 ; 
//.text:01483F00 ; 
//.text:01483F10 ; bool __cdecl flee_at_flee_position(int32, c_flee_state_data*)
//.text:01484050 ; void __cdecl flee_flush_actor(int32, c_behavior_state*, int32)
//.text:01484070 ; void __cdecl flee_flush_firing_position_indices(int32, c_behavior_state*, bool)
//.text:014840A0 ; bool __cdecl flip_try_direction(int32, int32, const c_ai_point3d*, c_sector_ref, const real_vector3d*, int16, bool, c_ai_point3d*, c_sector_ref*)
//.text:01484260 ; 
//.text:014842D0 ; bool __cdecl formation_can_fire_on_target(int32, int16, int16, real_vector2d*, real_vector2d*)
//.text:01484400 ; int16 __cdecl formation_find_position(int32, c_formation_data*, joint_state_datum*, real_point2d*, real_vector2d*, real_vector2d*, real_point2d*)
//.text:01484660 ; void __cdecl formation_get_facing(joint_state_datum*, real_vector2d*)
//.text:014847F0 ; int16 __cdecl formation_get_member_in_position(joint_state_datum*, int16)
//.text:014848A0 ; bool __cdecl formation_get_position(int32, int16, int16, const real_point2d*, const real_vector2d*, const real_vector2d*, real_point2d*)
//.text:014849B0 ; int16 __cdecl formation_member_get_position_index(int16, joint_state_datum*)
//.text:014849E0 ; c_behavior_state* __cdecl formation_member_get_state(joint_state_datum*, int16)
//.text:01484A50 ; void __cdecl generate_style_vector(int32, c_static_flags<224>*)
//.text:01484C50 ; public: static real64 __cdecl c_combat_range::get_actor_max_firing_range(int32)
//.text:01484C80 ; 
//.text:01484C90 ; int16 __cdecl guard_get_guard_state(int32)
//.text:01484D50 ; 
//.text:01484D80 ; 
//.text:01484F10 ; 
//.text:01484FD0 ; int16 __cdecl huddle_impulse_get_impulse(int32, c_behavior_state*)
//.text:01485080 ; int16 __cdecl impulse_anti_vehicle_grenade_get_impulse(int32, c_behavior_state*)
//.text:01485320 ; int16 __cdecl impulse_danger_retreat_get_impulse(int32, c_behavior_state*)
//.text:014853D0 ; int16 __cdecl impulse_evasion_get_impulse(int32, c_behavior_state*)
//.text:014855D0 ; int16 __cdecl impulse_grenade_get_impulse(int32, c_behavior_state*)
//.text:014857A0 ; int16 __cdecl impulse_grenade_uncover_get_impulse(int32, c_behavior_state*)
//.text:01485980 ; int16 __cdecl impulse_low_shield_retreat_get_impulse(int32, c_behavior_state*)
//.text:01485A00 ; int16 __cdecl impulse_nowhere_to_run_ambush_get_impulse(int32, c_behavior_state*)
//.text:01485A70 ; int16 __cdecl impulse_proximity_ambush_get_impulse(int32, c_behavior_state*)
//.text:01485B00 ; int16 __cdecl impulse_proximity_retreat_get_impulse(int32, c_behavior_state*)
//.text:01485BB0 ; int16 __cdecl impulse_scary_target_retreat_get_impulse(int32, c_behavior_state*)
//.text:01485CE0 ; int16 __cdecl impulse_vulnerable_enemy_ambush_get_impulse(int32, c_behavior_state*)
//.text:01485D90 ; 
//.text:01485DB0 ; void __cdecl kamikaze_detonate_grenades(c_kamikaze_state_data*)
//.text:01485E20 ; void __cdecl kamikaze_drop_grenades(c_kamikaze_state_data*)
//.text:01485EB0 ; 
//.text:01485F40 ; int16 __cdecl last_man_berserk_get_impulse(int32, c_behavior_state*)
//.text:01485FD0 ; int16 __cdecl leader_abandoned_charge_get_impulse(int32, c_behavior_state*)
//.text:014860E0 ; int16 __cdecl leader_dead_retreat_get_impulse(int32, c_behavior_state*)
//.text:014861F0 ; void __cdecl leap_on_cover_flush_object(int32, c_behavior_state*, int32)
//.text:01486210 ; bool __cdecl leap_on_cover_object_get_target_point(int32, int32, real_point3d*)
//.text:014862C0 ; void __cdecl mark_executing_behaviors(int32)
//.text:01486360 ; bool __cdecl melee_activate_helper(int32, c_behavior_state*, bool)
//.text:01486690 ; int16 __cdecl overheated_weapon_retreat_get_impulse(int32, c_behavior_state*)
//.text:014866E0 ; 
//.text:014866F0 ; bool __cdecl patrol_choose_next_point(int32, c_patrol_state_data*, c_ai_point3d*, c_sector_ref*)
//.text:01486A20 ; void __cdecl patrol_choose_random_point(int16, c_patrol_state_data*, bool)
//.text:01486AE0 ; int16 __cdecl peer_dead_retreat_get_impulse(int32, c_behavior_state*)
//.text:01486BD0 ; int32 __cdecl pickup_find_passenger(int32, int16*)
//.text:01486DB0 ; int16 __cdecl player_endangered_cover_get_impulse(int32, c_behavior_state*)
//.text:01486EA0 ; int16 __cdecl postcombat_decide(int32, int16, c_behavior_state*)
//.text:01486F20 ; void __cdecl postcombat_flush_structure_indices(int32, c_behavior_state*)
//.text:01486F40 ; int16 __cdecl presearch_uncover_get_impulse(int32, c_behavior_state*)
//.text:01486FC0 ; int16 __cdecl proximity_melee_get_impulse(int32, c_behavior_state*)
//.text:014870F0 ; int16 __cdecl proximity_self_preserve_get_impulse(int32, c_behavior_state*)
//.text:014871B0 ; bool __cdecl pursuit_area_discarded(int32, int16, int16)
//.text:01487220 ; void __cdecl pursuit_clear_areas(int32)
//.text:01487250 ; bool __cdecl pursuit_discard_area(int32)
//.text:014872C0 ; bool __cdecl pursuit_set_area(int32, int16, int16)
//.text:01487320 ; bool __cdecl ready_to_move(int32, const c_patrol_state_data*)
//.text:01487370 ; int16 __cdecl refresh_target_get_impulse(int32, c_behavior_state*)
//.text:014873E0 ; void __cdecl retreat_flush_prop(int32, c_behavior_state*, int32)
//.text:01487400 ; int16 __cdecl retreat_grenade_get_impulse(int32, c_behavior_state*)
//.text:01487660 ; 
//.text:01487680 ; int16 __cdecl scary_target_self_preservation_get_impulse(int32, c_behavior_state*)
//.text:01487770 ; int16 __cdecl sense_target_get_impulse(int32, c_behavior_state*)
//.text:01487940 ; 
//.text:01487970 ; 
//.text:01487990 ; 
//.text:014879C0 ; 
//.text:014879E0 ; int16 __cdecl shield_depleted_berserk_get_impulse(int32, c_behavior_state*)
//.text:01487B60 ; int16 __cdecl shield_depleted_cover_get_impulse(int32, c_behavior_state*)
//.text:01487C50 ; int16 __cdecl stalker_camo_control_get_impulse(int32, c_behavior_state*)
//.text:01487CF0 ; int16 __cdecl stuck_with_grenade_berserk_get_impulse(int32, c_behavior_state*)
//.text:01487D60 ; int16 __cdecl surprise_retreat_get_impulse(int32, c_behavior_state*)
//.text:01487E00 ; bool __cdecl target_on_board(int32)
//.text:01487E90 ; 
//.text:01487EB0 ; 
//.text:01487ED0 ; 
//.text:01487EF0 ; 
//.text:01487F20 ; 
//.text:01488150 ; int16 __cdecl unassailable_enemy_grenade_get_impulse(int32, c_behavior_state*)
//.text:014882A0 ; int16 __cdecl unassailable_enemy_self_preservation_get_impulse(int32, c_behavior_state*)
//.text:014883D0 ; int16 __cdecl unreachable_enemy_self_preservation_get_impulse(int32, c_behavior_state*)
//.text:01488470 ; bool __cdecl vehicle_charge_angle_valid(int32, character_vehicle_properties*, real32, int32, bool)
//.text:01488570 ; real32 __cdecl vehicle_charge_get_min_weapon_range(int32)
//.text:01488670 ; bool __cdecl vehicle_charge_range_valid(int32, real32*, int32)
//.text:01488940 ; bool __cdecl vehicle_cover_safe(int32, int32)
//.text:014889E0 ; int16 __cdecl vehicle_engage_patrol_get_impulse(int32, c_behavior_state*)
//.text:01488A40 ; int16 __cdecl vehicle_engage_wander_get_impulse(int32, c_behavior_state*)
//.text:01488A80 ; int16 __cdecl vehicle_enter_friendly_get_impulse(int32, c_behavior_state*)
//.text:01488CC0 ; int16 __cdecl vehicle_enter_get_impulse(int32, c_behavior_state*)
//.text:01488EC0 ; int16 __cdecl vehicle_entry_engage_get_impulse(int32, c_behavior_state*)
//.text:014892F0 ; int16 __cdecl vehicle_exit_get_impulse(int32, c_behavior_state*)
//.text:014897C0 ; bool __cdecl vehicle_find_destination(int32, int32, real_point3d*, c_ai_point3d*, c_sector_ref*, bool*, bool)
//.text:01489D30 ; bool __cdecl vehicle_flip_find_target(int32, c_vehicle_flip_data*, joint_vehicle_flip_data*)
//.text:0148A000 ; int16 __cdecl vehicle_flip_get_impulse(int32, c_behavior_state*)
//.text:0148A0B0 ; void __cdecl vehicle_pickup_flush_object(int32, c_behavior_state*, int32)
//.text:0148A0D0 ; int16 __cdecl vehicle_turtle_get_impulse(int32, c_behavior_state*)
//.text:0148A230 ; bool __cdecl vehicle_wants_to_move_at_all_costs(int32)

int16 actor_behavior_index_get(const actor_datum* actor, int16 layer_index)
{
	ASSERT(layer_index >= 0);
	ASSERT(layer_index < MAX_BEHAVIOR_LAYERS);
	ASSERT(layer_index <= actor->state.leaf_layer);

	if (VALID_INDEX(layer_index, MAX_BEHAVIOR_LAYERS))
		return *(int16*)actor_behavior_state_get(actor, layer_index);

	return NONE;
};

const char* behavior_names[NUMBER_OF_BEHAVIORS]
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

