#include "ai/ai_script.hpp"

#include "ai/ai.hpp"
#include "cseries/cseries_events.hpp"
#include "hs/hs_runtime.hpp"

#include <string.h>

bool ai_print_scripting = false;

//.text:0143BD40 ; bool __cdecl actor_member_of_ai_index(int32, int32)
//.text:0143BDC0 ; int16 __cdecl ai_assign_actors_to_seats(const int32*, int16, const unit_seat_source*, int16, bool)
//.text:0143C800 ; void __cdecl ai_attach_cs(int32, int16)
//.text:0143C850 ; bool __cdecl ai_cast(int32, int32, bool, int16, int32)
//.text:0143C870 ; bool __cdecl ai_cast(int32, int32, bool, int16, int32, int32)
//.text:0143C8A0 ; bool __cdecl ai_cast(int32, int32, bool, int16, int32, int32, int32)
//.text:0143C8E0 ; bool __cdecl ai_cast(int32, int32, bool, int16, int32, int32, int32, int32)
//.text:0143C920 ; bool __cdecl ai_cast(int32, int32, bool, int16, int32, int32, int32, int32, int32)
//.text:0143C970 ; bool __cdecl ai_cast(int32, int32, bool, int16, int32, int32, int32, int32, int32, int32)
//.text:0143C9C0 ; bool __cdecl ai_cast(int32, int32, bool, int16, int32, int32, int32, int32, int32, int32, int32)
//.text:0143CA10 ; int16 __cdecl ai_enter_squad_vehicles(int32)
//.text:0143CB60 ; int16 __cdecl ai_fighting_count(int32)
//.text:0143CC30 ; 

bool __cdecl ai_handle_script_verification(bool suppress_errors)
{
	return INVOKE(0x0143CC40, ai_handle_script_verification, suppress_errors);

	// $IMPLEMENT

	//bool success;
	//const scenario* scenario;
	//{
	//	short squad_index;
	//	{
	//		command_script_datum* script;
	//		c_data_iterator<command_script_datum> cs_iterator;
	//	}
	//}
}

//.text:0143CD00 ; int32 __cdecl ai_index_actor_get_actor_index(int32)
//.text:0143CD60 ; void __cdecl ai_index_actor_iterator_new(int32, ai_index_actor_iterator*)
//.text:0143CE10 ; actor_datum* __cdecl ai_index_actor_iterator_next(ai_index_actor_iterator*)

bool __cdecl ai_index_from_string(struct scenario* scenario, const char* ai_string, int32* ai_index_reference)
{
	ASSERT(ai_string && ai_index_reference);

	// $IMPLEMENT

	return false;
}

//.text:0143CEE0 ; int32 __cdecl ai_index_get_actor(int32)
//.text:0143CFD0 ; int32 __cdecl ai_index_get_vehicle(int32, int16)
//.text:0143D130 ; bool __cdecl ai_index_player_nearby(int32, real32)
//.text:0143D250 ; void __cdecl ai_index_squad_iterator_new(int32, ai_index_squad_iterator*)
//.text:0143D400 ; squad_datum* __cdecl ai_index_squad_iterator_next(ai_index_squad_iterator*)
//.text:0143D500 ; void __cdecl ai_index_to_string(int32, struct scenario*, char*, int32)
//.text:0143D5D0 ; void __cdecl ai_magically_see(int32, int32)
//.text:0143D700 ; void __cdecl ai_magically_see_object(int32, int32)
//.text:0143D830 ; int16 __cdecl ai_play_line(int32, int32)
//.text:0143D8F0 ; int16 __cdecl ai_play_line_at_player(int32, int32)
//.text:0143DAA0 ; int16 __cdecl ai_play_line_on_object(int32, int32)
//.text:0143DAE0 ; int16 __cdecl ai_play_line_on_object_for_team(int32, int32, int16)
//.text:0143DB90 ; int16 __cdecl ai_play_line_on_point_set2(int32, int32, int16, int32)
//.text:0143DD50 ; int16 __cdecl ai_play_line_on_point_set(int32, int32, int16)
//.text:0143DD70 ; int32 __cdecl ai_player_get_vehicle_squad(int32)
//.text:0143DE00 ; void __cdecl ai_queue_cs(int32, int16)
//.text:0143DE50 ; void __cdecl ai_release(int32, int32)
//.text:0143DEA0 ; void __cdecl ai_release_all(int32)
//.text:0143DF10 ; void __cdecl ai_reserve2(int32, int32, bool, int16)
//.text:0143DF70 ; void __cdecl ai_reserve(int32, int32, int16)

void __cdecl ai_script_dispose()
{
	INVOKE(0x0143DF90, ai_script_dispose);
}

void __cdecl ai_script_dispose_from_old_map()
{
	INVOKE(0x0143DFA0, ai_script_dispose_from_old_map);
}

void __cdecl ai_script_initialize()
{
	INVOKE(0x0143DFB0, ai_script_initialize);
}

void __cdecl ai_script_initialize_for_new_map()
{
	INVOKE(0x0143DFC0, ai_script_initialize_for_new_map);
}

//.text:0143DFD0 ; bool __cdecl ai_script_process_fragment(s_file_reference*, char*, char*)
//.text:0143E030 ; void __cdecl ai_scripting_abort_activity(int32)
//.text:0143E070 ; int32 __cdecl ai_scripting_ai_index_from_spawn_formation(int32, int32)

void __cdecl ai_scripting_allegiance(int16 team1_index, int16 team2_index)
{
	INVOKE(0x0143E0F0, ai_scripting_allegiance, team1_index, team2_index);
}

//.text:0143E1C0 ; void __cdecl ai_scripting_allegiance_break(int16, int16)
//.text:0143E1F0 ; bool __cdecl ai_scripting_allegiance_broken(int16, int16)
//.text:0143E230 ; void __cdecl ai_scripting_allegiance_remove(int16, int16)
//.text:0143E260 ; void __cdecl ai_scripting_allow_dormant(int32, bool)
//.text:0143E270 ; void __cdecl ai_scripting_berserk(int32, bool)
//.text:0143E2C0 ; int32 __cdecl ai_scripting_body_count(int32)
//.text:0143E350 ; void __cdecl ai_scripting_braindead(int32, bool)
//.text:0143E3A0 ; void __cdecl ai_scripting_braindead_by_unit(int32, bool)
//.text:0143E470 ; void __cdecl ai_scripting_bring_forward(int32, real32)
//.text:0143E4A0 ; void __cdecl ai_scripting_cannot_die(int32, bool)
//.text:0143E530 ; bool __cdecl ai_scripting_carrying_player(int32)
//.text:0143E590 ; 
//.text:0143E610 ; int16 __cdecl ai_scripting_combat_status(int32)
//.text:0143E670 ; int32 __cdecl ai_scripting_count_internal(int32, int16, int32*, real32*)
//.text:0143E850 ; void __cdecl ai_scripting_deselect()
//.text:0143E860 ; void __cdecl ai_scripting_disposable(int32, bool)
//.text:0143E960 ; void __cdecl ai_scripting_dont_do_avoidance(int32, bool)
//.text:0143E9B0 ; void __cdecl ai_scripting_drop_ai(const char*, const char*)
//.text:0143E9C0 ; void __cdecl ai_scripting_engineer_explode(int32)
//.text:0143EA20 ; void __cdecl ai_scripting_erase(int32)

void __cdecl ai_scripting_erase_all()
{
	//INVOKE(0x0143EBC0, ai_scripting_erase_all);

	if (ai_print_scripting)
	{
		event(_event_warning, "ai: %s: ai_erase_all", hs_runtime_get_executing_thread_name());
	}

	ai_erase(NONE, false);
}

//.text:0143EBD0 ; void __cdecl ai_scripting_erase_inactive(int32, int16)
//.text:0143EC40 ; void __cdecl ai_scripting_exit_vehicle2(int32)
//.text:0143EC60 ; void __cdecl ai_scripting_exit_vehicle(int32, int32)
//.text:0143ED10 ; bool __cdecl ai_scripting_flock_create(int32)
//.text:0143ED70 ; bool __cdecl ai_scripting_flock_definition_set(int32, int32)
//.text:0143EDD0 ; bool __cdecl ai_scripting_flock_delete(int32)
//.text:0143EE00 ; void __cdecl ai_scripting_flock_unperch(int32)
//.text:0143EE30 ; void __cdecl ai_scripting_flood_disperse(int32, int32)
//.text:0143F090 ; void __cdecl ai_scripting_force_active(int32, bool)
//.text:0143F1E0 ; void __cdecl ai_scripting_force_active_by_unit(int32, bool)
//.text:0143F240 ; int32 __cdecl ai_scripting_get_object(int32)
//.text:0143F280 ; int32 __cdecl ai_scripting_get_squad(int32)
//.text:0143F2F0 ; int32 __cdecl ai_scripting_get_turret_ai(int32, int16)
//.text:0143F410 ; int32 __cdecl ai_scripting_get_unit(int32)
//.text:0143F450 ; void __cdecl ai_scripting_grunt_kamikaze(int32)
//.text:0143F4B0 ; void __cdecl ai_scripting_ignore(int32, bool)
//.text:0143F510 ; int32 __cdecl ai_scripting_in_vehicle_count(int32)
//.text:0143F530 ; bool __cdecl ai_scripting_is_attacking(int32)
//.text:0143F540 ; void __cdecl ai_scripting_kill(int32)
//.text:0143F560 ; void __cdecl ai_scripting_kill_internal(int32, bool, bool)
//.text:0143F5D0 ; void __cdecl ai_scripting_kill_multiplayer_cinematic(int32)
//.text:0143F5F0 ; void __cdecl ai_scripting_kill_silent(int32)
//.text:0143F610 ; void __cdecl ai_scripting_kill_silent_multiplayer_cinematic(int32)
//.text:0143F630 ; int32 __cdecl ai_scripting_living_count(int32)
//.text:0143F650 ; real64 __cdecl ai_scripting_living_fraction(int32)
//.text:0143F6A0 ; void __cdecl ai_scripting_migrate(int32, int32)
//.text:0143F6D0 ; void __cdecl ai_scripting_migrate_and_speak(int32, int32, const char*)
//.text:0143F720 ; void __cdecl ai_scripting_migrate_by_unit(int32, int32)
//.text:0143F7C0 ; void __cdecl ai_scripting_migrate_by_unit_internal(int32, int32, bool, bool)
//.text:0143F7D0 ; void __cdecl ai_scripting_migrate_infantry(int32, int32)
//.text:0143F920 ; void __cdecl ai_scripting_migrate_internal(int32, int32, bool, bool)
//.text:0143FAC0 ; int32 __cdecl ai_scripting_nearest_point(int32, int32)
//.text:0143FBD0 ; int32 __cdecl ai_scripting_nonswarm_count(int32)
//.text:0143FBF0 ; bool __cdecl ai_scripting_pathfinding_generate_all()
//.text:0143FC00 ; void __cdecl ai_scripting_place2(int32, int16)
//.text:0143FC20 ; void __cdecl ai_scripting_place2_internal(int32, int16, bool)
//.text:0143FD40 ; void __cdecl ai_scripting_place2_multiplayer_cinematic(int32, int16)
//.text:0143FD60 ; void __cdecl ai_scripting_place(int32)
//.text:0143FD80 ; void __cdecl ai_scripting_place_in_vehicle(int32, int32)
//.text:0143FE30 ; void __cdecl ai_scripting_place_internal(int32, bool)
//.text:0143FFD0 ; void __cdecl ai_scripting_place_multiplayer_cinematic(int32)
//.text:0143FFF0 ; bool __cdecl ai_scripting_player_any_needs_vehicle()
//.text:01440060 ; bool __cdecl ai_scripting_player_in_vehicle(int32)
//.text:01440110 ; bool __cdecl ai_scripting_player_needs_vehicle(int32)
//.text:01440160 ; void __cdecl ai_scripting_playfight(int32, bool)
//.text:01440170 ; int32 __cdecl ai_scripting_point_count(int32)
//.text:014401C0 ; int32 __cdecl ai_scripting_point_set_get_point(int32, int16)
//.text:01440230 ; void __cdecl ai_scripting_prefer_target(int32, bool)
//.text:014402A0 ; void __cdecl ai_scripting_prefer_target_ai(int32, int32, bool)
//.text:01440300 ; void __cdecl ai_scripting_prefer_target_team(int32, e_game_team)
//.text:01440350 ; int32 __cdecl ai_scripting_random_point(int32, real32, real32, real32)
//.text:01440880 ; void __cdecl ai_scripting_reconnect()
//.text:01440890 ; void __cdecl ai_scripting_render_paths_all()
//.text:014408A0 ; void __cdecl ai_scripting_renew(int32)
//.text:014409E0 ; void __cdecl ai_scripting_resurrect(int32)
//.text:01440A70 ; bool __cdecl ai_scripting_rotate_scenario(real32, real32, real32, real32)
//.text:01440A80 ; void __cdecl ai_scripting_select_squad(int32)
//.text:01440A90 ; void __cdecl ai_scripting_set_active_camo(int32, bool)
//.text:01440AE0 ; void __cdecl ai_scripting_set_activity(int32, int32)
//.text:01440B50 ; void __cdecl ai_scripting_set_blind(int32, bool)
//.text:01440BF0 ; void __cdecl ai_scripting_set_deaf(int32, bool)
//.text:01440C90 ; void __cdecl ai_scripting_set_targeting_group2(int32, int16, bool)
//.text:01440D00 ; void __cdecl ai_scripting_set_targeting_group(int32, int16)
//.text:01440D20 ; void __cdecl ai_scripting_set_team(int32, int16)
//.text:01440DC0 ; void __cdecl ai_scripting_set_weapon_up(int32, bool)
//.text:01440E10 ; real64 __cdecl ai_scripting_strength(int32)
//.text:01440E40 ; void __cdecl ai_scripting_suppress_combat(int32, bool)
//.text:01440E90 ; void __cdecl ai_scripting_survival_cleanup(int32, bool, bool)
//.text:01440F00 ; int32 __cdecl ai_scripting_swarm_count(int32)
//.text:01440F20 ; void __cdecl ai_scripting_teleport(int32, int32)
//.text:014411F0 ; void __cdecl ai_scripting_teleport_to_spawn_point_if_outside_bsp(int32)
//.text:01441490 ; bool __cdecl ai_scripting_translate_scenario(real32, real32, real32)
//.text:014414A0 ; int16 __cdecl ai_scripting_vehicle_count(int32)
//.text:01441510 ; bool __cdecl ai_scripting_vitality_pinned(int32)
//.text:014415B0 ; bool __cdecl ai_scripting_wall_lean(int32)
//.text:01441600 ; int16 __cdecl ai_spawn_count(int32)
//.text:01441660 ; void __cdecl ai_stack_cs(int32, int32)
//.text:014416B0 ; void __cdecl ai_vehicle_enter2(int32, int32)
//.text:014416D0 ; void __cdecl ai_vehicle_enter(int32, int32, int32)
//.text:014416F0 ; void __cdecl ai_vehicle_enter_immediate2(int32, int32)
//.text:01441710 ; void __cdecl ai_vehicle_enter_immediate(int32, int32, int32)
//.text:01441730 ; void __cdecl ai_vehicle_enter_internal(int32, int32, int32, bool)
//.text:01441800 ; void __cdecl ai_vehicle_flip(int32)
//.text:01441840 ; int32 __cdecl ai_vehicle_get(int32)
//.text:014418C0 ; int32 __cdecl ai_vehicle_get_from_spawn_point(int32)
//.text:014418F0 ; int32 __cdecl ai_vehicle_get_from_squad(int32, int16)
//.text:01441920 ; int32 __cdecl ai_vehicle_get_from_starting_location(int32)
//.text:014419B0 ; bool __cdecl ai_vehicle_overturned(int32)
//.text:014419D0 ; bool __cdecl ai_vehicle_reserve(int32, bool)
//.text:01441A90 ; bool __cdecl ai_vehicle_reserve_seat(int32, int32, bool)
//.text:01441B70 ; public: void c_data_iterator<command_script_datum>::begin(s_data_array*)
//.text:01441B90 ; public: void __cdecl c_static_flags_no_init<20>::clear()
//.text:01441BA0 ; bool __cdecl cs_command_script_queued(int32, int32)
//.text:01441C50 ; bool __cdecl cs_command_script_running(int32, int32)
//.text:01441CE0 ; int16 __cdecl cs_number_queued(int32)
//.text:01441D60 ; bool __cdecl flock_start(int32)
//.text:01441DB0 ; bool __cdecl flock_stop(int32)
//.text:01441E00 ; public: command_script_datum* c_data_iterator<command_script_datum>::get_datum() const
//.text:01441E10 ; public: int32 c_data_iterator<command_script_datum>::get_index() const
//.text:01441E20 ; public: bool c_data_iterator<command_script_datum>::next()
//.text:01441E40 ; int32 __cdecl object_get_ai(int32)
//.text:01441EA0 ; int32 __cdecl object_index_from_ai(int32, e_hs_type)

int32 __cdecl object_list_from_ai_reference(int32 ai_index)
{
	return INVOKE(0x01441F90, object_list_from_ai_reference, ai_index);

	// $IMPLEMENT

	//int32 object_list_index;
	//{
	//	ai_index_actor_iterator iterator;
	//	const actor_datum* actor;
	//	{
	//		swarm_creature_iterator creature_iterator;
	//		const creature_datum* creature;
	//	}
	//}
}

//.text:01442040 ; bool __cdecl play_line_compare(int32, int32, const void*)
//.text:014420D0 ; int16 __cdecl play_line_compile_actor_list(int32, int32*, int16, int16)
//.text:01442150 ; bool __cdecl point_compare(int32, int32, void*)
//.text:01442170 ; int16 __cdecl point_distance(const cs_point*)
//.text:01442280 ; public: void __cdecl c_static_flags_no_init<20>::set(int32, bool)
//.text:014422B0 ; bool __cdecl squad_member_of_ai_index(int32, int32)
//.text:01442370 ; public: bool __cdecl c_static_flags_no_init<20>::test(int32) const

