#include "ai/ai_script.hpp"

#include "ai/ai.hpp"
#include "cseries/cseries_events.hpp"
#include "hs/hs_runtime.hpp"

#include <string.h>

bool ai_print_scripting = false;

//.text:0143BD40 ; 
//.text:0143BDC0 ; short __cdecl ai_assign_actors_to_seats(long const*, short, unit_seat_source const*, short, bool)
//.text:0143C800 ; cs_run_command_script?
//.text:0143C850 ; bool __cdecl ai_cast(long, long, bool, short, long)
//.text:0143C870 ; bool __cdecl ai_cast(long, long, bool, short, long, long)
//.text:0143C8A0 ; bool __cdecl ai_cast(long, long, bool, short, long, long, long)
//.text:0143C8E0 ; bool __cdecl ai_cast(long, long, bool, short, long, long, long, long)
//.text:0143C920 ; bool __cdecl ai_cast(long, long, bool, short, long, long, long, long, long)
//.text:0143C970 ; bool __cdecl ai_cast(long, long, bool, short, long, long, long, long, long, long)
//.text:0143C9C0 ; bool __cdecl ai_cast(long, long, bool, short, long, long, long, long, long, long, long)
//.text:0143CA10 ; short __cdecl ai_enter_squad_vehicles(long)
//.text:0143CB60 ; short __cdecl ai_fighting_count(long)
//.text:0143CC30 ; 
//.text:0143CC40 ; bool __cdecl ai_handle_script_verification(bool)
//.text:0143CD00 ; long __cdecl ai_index_actor_get_actor_index(long)
//.text:0143CD60 ; void __cdecl ai_index_actor_iterator_new(long, ai_index_actor_iterator*)
//.text:0143CE10 ; actor_datum* __cdecl ai_index_actor_iterator_next(ai_index_actor_iterator*)
//.text:0143CEE0 ; long __cdecl ai_index_get_actor(long)
//.text:0143CFD0 ; 
//.text:0143D130 ; bool __cdecl ai_index_player_nearby(long, real)
//.text:0143D250 ; void __cdecl ai_index_squad_iterator_new(long, ai_index_squad_iterator*)
//.text:0143D400 ; squad_datum* __cdecl ai_index_squad_iterator_next(ai_index_squad_iterator*)
//.text:0143D500 ; void __cdecl ai_index_to_string(long, struct scenario*, char*, long)
//.text:0143D5D0 ; void __cdecl ai_magically_see(long, long)
//.text:0143D700 ; void __cdecl ai_magically_see_object(long, long)
//.text:0143D830 ; short __cdecl ai_play_line(long, long)
//.text:0143D8F0 ; short __cdecl ai_play_line_at_player(long, long)
//.text:0143DAA0 ; short __cdecl ai_play_line_on_object(long, long)
//.text:0143DAE0 ; short __cdecl ai_play_line_on_object_for_team(long, long, short)
//.text:0143DB90 ; short __cdecl ai_play_line_on_point_set2(long, long, short, long)
//.text:0143DD50 ; short __cdecl ai_play_line_on_point_set(long, long, short)
//.text:0143DD70 ; long __cdecl ai_player_get_vehicle_squad(long)
//.text:0143DE00 ; void __cdecl ai_queue_cs(long, short)
//.text:0143DE50 ; 
//.text:0143DEA0 ; void __cdecl ai_release_all(long)
//.text:0143DF10 ; void __cdecl ai_reserve2(long, long, bool, short)
//.text:0143DF70 ; void __cdecl ai_reserve(long, long, short)
//.text:0143DF90 ; void __cdecl ai_script_dispose()
//.text:0143DFA0 ; void __cdecl ai_script_dispose_from_old_map()
//.text:0143DFB0 ; void __cdecl ai_script_initialize()
//.text:0143DFC0 ; void __cdecl ai_script_initialize_for_new_map()
//.text:0143DFD0 ; bool __cdecl ai_script_process_fragment(s_file_reference*, char*, char*)
//.text:0143E030 ; void __cdecl ai_scripting_abort_activity(long)
//.text:0143E070 ; long __cdecl ai_scripting_ai_index_from_spawn_formation(long, long)
//.text:0143E0F0 ; void __cdecl ai_scripting_allegiance(e_campaign_team, e_campaign_team)
//.text:0143E1C0 ; void __cdecl ai_scripting_allegiance_break(e_campaign_team, e_campaign_team)
//.text:0143E1F0 ; bool __cdecl ai_scripting_allegiance_broken(e_campaign_team, e_campaign_team)
//.text:0143E230 ; void __cdecl ai_scripting_allegiance_remove(e_campaign_team, e_campaign_team)
//.text:0143E260 ; void __cdecl ai_scripting_allow_dormant(long, bool)
//.text:0143E270 ; void __cdecl ai_scripting_berserk(long, bool)
//.text:0143E2C0 ; long __cdecl ai_scripting_body_count(long)
//.text:0143E350 ; void __cdecl ai_scripting_braindead(long, bool)
//.text:0143E3A0 ; void __cdecl ai_scripting_braindead_by_unit(long, bool)
//.text:0143E470 ; void __cdecl ai_scripting_bring_forward(long, real)
//.text:0143E4A0 ; void __cdecl ai_scripting_cannot_die(long, bool)
//.text:0143E530 ; bool __cdecl ai_scripting_carrying_player(long)
//.text:0143E590 ; 
//.text:0143E610 ; short __cdecl ai_scripting_combat_status(long)
//.text:0143E670 ; long __cdecl ai_scripting_count_internal(long, short, long*, real*)
//.text:0143E850 ; void __cdecl ai_scripting_deselect()
//.text:0143E860 ; void __cdecl ai_scripting_disposable(long, bool)
//.text:0143E960 ; void __cdecl ai_scripting_dont_do_avoidance(long, bool)
//.text:0143E9B0 ; 
//.text:0143E9C0 ; void __cdecl ai_scripting_engineer_explode(long)
//.text:0143EA20 ; void __cdecl ai_scripting_erase(long)

void __cdecl ai_scripting_erase_all()
{
	//INVOKE(0x0143EBC0, ai_scripting_erase_all);

	if (ai_print_scripting)
		generate_event(_event_level_warning, "ai: %s: ai_erase_all", hs_runtime_get_executing_thread_name());

	ai_erase(NONE, false);
}

//.text:0143EBD0 ; void __cdecl ai_scripting_erase_inactive(long, short)
//.text:0143EC40 ; void __cdecl ai_scripting_exit_vehicle2(long)
//.text:0143EC60 ; void __cdecl ai_scripting_exit_vehicle(long, long)
//.text:0143ED10 ; bool __cdecl ai_scripting_flock_create(long)
//.text:0143ED70 ; bool __cdecl ai_scripting_flock_definition_set(long, long)
//.text:0143EDD0 ; bool __cdecl ai_scripting_flock_delete(long)
//.text:0143EE00 ; void __cdecl ai_scripting_flock_unperch(long)
//.text:0143EE30 ; void __cdecl ai_scripting_flood_disperse(long, long)
//.text:0143F090 ; void __cdecl ai_scripting_force_active(long, bool)
//.text:0143F1E0 ; void __cdecl ai_scripting_force_active_by_unit(long, bool)
//.text:0143F240 ; long __cdecl ai_scripting_get_object(long)
//.text:0143F280 ; long __cdecl ai_scripting_get_squad(long)
//.text:0143F2F0 ; long __cdecl ai_scripting_get_turret_ai(long, short)
//.text:0143F410 ; long __cdecl ai_scripting_get_unit(long)
//.text:0143F450 ; void __cdecl ai_scripting_grunt_kamikaze(long)
//.text:0143F4B0 ; void __cdecl ai_scripting_ignore(long, bool)
//.text:0143F510 ; long __cdecl ai_scripting_in_vehicle_count(long)
//.text:0143F530 ; bool __cdecl ai_scripting_is_attacking(long)
//.text:0143F540 ; void __cdecl ai_scripting_kill(long)
//.text:0143F560 ; void __cdecl ai_scripting_kill_internal(long, bool, bool)
//.text:0143F5D0 ; void __cdecl ai_scripting_kill_multiplayer_cinematic(long)
//.text:0143F5F0 ; void __cdecl ai_scripting_kill_silent(long)
//.text:0143F610 ; void __cdecl ai_scripting_kill_silent_multiplayer_cinematic(long)
//.text:0143F630 ; long __cdecl ai_scripting_living_count(long)
//.text:0143F650 ; double __cdecl ai_scripting_living_fraction(long)
//.text:0143F6A0 ; void __cdecl ai_scripting_migrate(long, long)
//.text:0143F6D0 ; void __cdecl ai_scripting_migrate_and_speak(long, long, char const*)
//.text:0143F720 ; void __cdecl ai_scripting_migrate_by_unit(long, long)
//.text:0143F7C0 ; 
//.text:0143F7D0 ; void __cdecl ai_scripting_migrate_infantry(long, long)
//.text:0143F920 ; void __cdecl ai_scripting_migrate_internal(long, long, bool, bool)
//.text:0143FAC0 ; long __cdecl ai_scripting_nearest_point(long, long)
//.text:0143FBD0 ; long __cdecl ai_scripting_nonswarm_count(long)
//.text:0143FBF0 ; bool __cdecl ai_scripting_pathfinding_generate_all()
//.text:0143FC00 ; void __cdecl ai_scripting_place2(long, short)
//.text:0143FC20 ; void __cdecl ai_scripting_place2_internal(long, short, bool)
//.text:0143FD40 ; void __cdecl ai_scripting_place2_multiplayer_cinematic(long, short)
//.text:0143FD60 ; void __cdecl ai_scripting_place(long)
//.text:0143FD80 ; void __cdecl ai_scripting_place_in_vehicle(long, long)
//.text:0143FE30 ; void __cdecl ai_scripting_place_internal(long, bool)
//.text:0143FFD0 ; void __cdecl ai_scripting_place_multiplayer_cinematic(long)
//.text:0143FFF0 ; bool __cdecl ai_scripting_player_any_needs_vehicle()
//.text:01440060 ; ai_scripting_player_in_vehicle?
//.text:01440110 ; bool __cdecl ai_scripting_player_needs_vehicle(long)
//.text:01440160 ; void __cdecl ai_scripting_playfight(long, bool)
//.text:01440170 ; long __cdecl ai_scripting_point_count(long)
//.text:014401C0 ; long __cdecl ai_scripting_point_set_get_point(long, short)
//.text:01440230 ; void __cdecl ai_scripting_prefer_target(long, bool)
//.text:014402A0 ; void __cdecl ai_scripting_prefer_target_ai(long, long, bool)
//.text:01440300 ; void __cdecl ai_scripting_prefer_target_team(long, e_campaign_team)
//.text:01440350 ; long __cdecl ai_scripting_random_point(long, real, real, real)
//.text:01440880 ; void __cdecl ai_scripting_reconnect()
//.text:01440890 ; void __cdecl ai_scripting_render_paths_all()
//.text:014408A0 ; void __cdecl ai_scripting_renew(long)
//.text:014409E0 ; void __cdecl ai_scripting_resurrect(long)
//.text:01440A70 ; bool __cdecl ai_scripting_rotate_scenario(real, real, real, real)
//.text:01440A80 ; void __cdecl ai_scripting_select_squad(long)
//.text:01440A90 ; void __cdecl ai_scripting_set_active_camo(long, bool)
//.text:01440AE0 ; void __cdecl ai_scripting_set_activity(long, long)
//.text:01440B50 ; void __cdecl ai_scripting_set_blind(long, bool)
//.text:01440BF0 ; void __cdecl ai_scripting_set_deaf(long, bool)
//.text:01440C90 ; void __cdecl ai_scripting_set_targeting_group2(long, short, bool)
//.text:01440D00 ; void __cdecl ai_scripting_set_targeting_group(long, short)
//.text:01440D20 ; void __cdecl ai_scripting_set_team(long, short)
//.text:01440DC0 ; void __cdecl ai_scripting_set_weapon_up(long, bool)
//.text:01440E10 ; double __cdecl ai_scripting_strength(long)
//.text:01440E40 ; void __cdecl ai_scripting_suppress_combat(long, bool)
//.text:01440E90 ; void __cdecl ai_scripting_survival_cleanup(long, bool, bool)
//.text:01440F00 ; long __cdecl ai_scripting_swarm_count(long)
//.text:01440F20 ; void __cdecl ai_scripting_teleport(long, long)
//.text:014411F0 ; void __cdecl ai_scripting_teleport_to_spawn_point_if_outside_bsp(long)
//.text:01441490 ; ai_scripting_translate_scenario?
//.text:014414A0 ; short __cdecl ai_scripting_vehicle_count(long)
//.text:01441510 ; bool __cdecl ai_scripting_vitality_pinned(long)
//.text:014415B0 ; bool __cdecl ai_scripting_wall_lean(long)
//.text:01441600 ; short __cdecl ai_spawn_count(long)
//.text:01441660 ; void __cdecl ai_stack_cs(long, long)
//.text:014416B0 ; void __cdecl ai_vehicle_enter2(long, long)
//.text:014416D0 ; void __cdecl ai_vehicle_enter(long, long, long)
//.text:014416F0 ; void __cdecl ai_vehicle_enter_immediate2(long, long)
//.text:01441710 ; void __cdecl ai_vehicle_enter_immediate(long, long, long)
//.text:01441730 ; void __cdecl ai_vehicle_enter_internal(long, long, long, bool)
//.text:01441800 ; void __cdecl ai_vehicle_flip(long)
//.text:01441840 ; long __cdecl ai_vehicle_get(long)
//.text:014418C0 ; long __cdecl ai_vehicle_get_from_spawn_point(long)
//.text:014418F0 ; long __cdecl ai_vehicle_get_from_squad(long, short)
//.text:01441920 ; long __cdecl ai_vehicle_get_from_starting_location(long)
//.text:014419B0 ; bool __cdecl ai_vehicle_overturned(long)
//.text:014419D0 ; bool __cdecl ai_vehicle_reserve(long, bool)
//.text:01441A90 ; bool __cdecl ai_vehicle_reserve_seat(long, long, bool)
//.text:01441B70 ; 
//.text:01441B90 ; 
//.text:01441BA0 ; cs_command_script_queued?
//.text:01441C50 ; bool __cdecl cs_command_script_running(long, long)

bool ai_index_from_string(struct scenario* scenario, char const* ai_string, long* ai_index_reference)
{
	ASSERT(ai_string && ai_index_reference);

	// #TODO: implement

	return false;
}

