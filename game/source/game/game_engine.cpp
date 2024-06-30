#include "game/game_engine.hpp"

#include "camera/saved_film_director.hpp"
#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "game/game_engine_util.hpp"
#include "game/game_grief.hpp"
#include "game/game_time.hpp"
#include "game/players.hpp"
#include "input/input_abstraction.hpp"
#include "interface/gui_screens/boot_betrayer/gui_screen_boot_betrayer.hpp"
#include "interface/gui_screens/scoreboard/gui_screen_scoreboard.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_mapping.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_window_manager.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "simulation/game_interface/simulation_game_action.hpp"
#include "tag_files/files.hpp"

#include <climits>

REFERENCE_DECLARE_ARRAY(0x0189ECF0, char const*, k_game_engine_end_conditions, k_game_engine_end_condition_count);

HOOK_DECLARE(0x00550B80, game_engine_get_statborg);

#ifndef DEDICATED_SERVER
HOOK_DECLARE(0x00551780, game_engine_interface_update);
#endif // DEDICATED_SERVER

HOOK_DECLARE(0x005521D0, game_engine_get_pre_round_ticks);
HOOK_DECLARE(0x00553660, game_engine_update_round_conditions);

c_game_statborg campaign_statborg{};

//.text:0054D7E0 ; void __cdecl game_engine_adjust_player_results_statistic(long, bool, enum e_game_results_statistic, long)
//.text:0054D840 ; bool __cdecl game_engine_adjust_player_score(long, long)
//.text:0054D940 ; void __cdecl game_engine_adjust_team_score_for_composition(enum e_game_team)
//.text:0054D980 ; bool __cdecl game_engine_allow_dynamic_lighting(void)
//.text:0054D990 ; bool __cdecl game_engine_allow_weapon_pickup(long, long)
//.text:0054D9C0 ; bool __cdecl game_engine_are_players_tied(void)
//.text:0054DA50 ; bool __cdecl game_engine_are_teams_tied(void)
//.text:0054DAA0 ; 
//.text:0054DAE0 ; 
//.text:0054DB00 ; void __cdecl game_engine_boot_player(long)
//.text:0054DB70 ; void __cdecl game_engine_build_allowable_team_designators(void)
//.text:0054DC00 ; void __cdecl game_engine_check_for_round_winner(void)
//.text:0054DD90 ; long __cdecl game_engine_choose_winner_index(void)
//.text:0054DDE0 ; void __cdecl game_engine_client_respond_to_new_state(long)
//.text:0054DEB0 ; long __cdecl game_engine_compare_players(long, long, bool)
//.text:0054E0A0 ; 
//.text:0054E0B0 ; long __cdecl game_engine_compare_teams(long, long, bool)
//.text:0054E210 ; float __cdecl game_engine_compute_object_function(long, long)
//.text:0054E250 ; 
//.text:0054E270 ; 
//.text:0054E290 ; void __cdecl game_engine_dispose(void)
//.text:0054E2A0 ; void __cdecl game_engine_dispose_from_old_map(void)
//.text:0054E2F0 ; void __cdecl game_engine_end_game(enum e_game_engine_end_condition)
//.text:0054E320 ; void __cdecl game_engine_end_round_internal(bool)

void __cdecl game_engine_end_round_with_winner(long team, bool a1, e_game_engine_end_condition condition)
{
	INVOKE(0x0054E390, game_engine_end_round_with_winner, team, a1, condition);
}

//.text:0054E890 ; 
//.text:0054E910 ; void __cdecl game_engine_game_ending(void)
//.text:0054E9B0 ; bool __cdecl game_engine_game_object_has_fallen_too_far(long)

void __cdecl game_engine_game_starting()
{
	INVOKE(0x0054EA60, game_engine_game_starting);
}

//.text:0054EC60 ; void __cdecl game_engine_garbage_collect(void)
//.text:0054EFC0 ; void __cdecl game_engine_garbage_collect_for_round_switch(void)
//.text:0054F220 ; 
//.text:0054F250 ; game_engine_get_change_colors, modified by saber
//.text:0054F5E0 ; long __cdecl game_engine_get_current_talker(long)
//.text:0054F670 ; game_engine_get_damage_multiplier, modified by saber
//.text:0054FC10 ; long __cdecl game_engine_get_finalized_player_place(long)
//.text:0054FCC0 ; long __cdecl game_engine_get_finalized_player_score(long)
//.text:0054FDC0 ; long __cdecl game_engine_get_finalized_team_place(enum e_multiplayer_team)
//.text:0054FE70 ; long __cdecl game_engine_get_game_object_list(void)
//.text:0054FEF0 ; bool __cdecl game_engine_get_hud_interface_state(enum e_output_user_index, struct game_engine_interface_state *)
//.text:00550410 ; void __cdecl game_engine_get_in_game_string(long, class c_static_wchar_string<256> *)

void __cdecl game_engine_get_multiplayer_string(string_id id, c_static_wchar_string<1024>* out_multiplayer_string)
{
	INVOKE(0x00550460, game_engine_get_multiplayer_string, id, out_multiplayer_string);
}

//.text:005504C0 ; long __cdecl game_engine_get_number_of_rounds(void)
//.text:005504E0 ; long __cdecl game_engine_get_player_assists(long)
//.text:00550580 ; long __cdecl game_engine_get_player_deaths(long)
//.text:00550620 ; long __cdecl game_engine_get_player_kills(long)
//.text:005506C0 ; 

long __cdecl game_engine_get_player_place(long absolute_player_index)
{
	return INVOKE(0x005506F0, game_engine_get_player_place, absolute_player_index);
}

long __cdecl game_engine_get_player_score(long absolute_player_index)
{
	return INVOKE(0x00550720, game_engine_get_player_score, absolute_player_index);
}

long __cdecl game_engine_get_player_score_for_display(long absolute_player_index, bool final_score)
{
	return INVOKE(0x005507D0, game_engine_get_player_score_for_display, absolute_player_index, final_score);
}

e_game_engine_status __cdecl game_engine_get_player_state_index(long player_index)
{
	return INVOKE(0x00550830, game_engine_get_player_state_index, player_index);
}

//.text:00550A30 ; 
//.text:00550AD0 ; long __cdecl game_engine_get_round(void)
//.text:00550B00 ; 
//.text:00550B30 ; public: long __cdecl c_game_engine_miscellaneous_options::get_round_time_limit_seconds(void)const
//.text:00550B50 ; long __cdecl game_engine_get_score_to_win_round(void)

c_game_statborg* __cdecl game_engine_get_statborg()
{
	//00550B80

	TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);
	TLS_DATA_GET_VALUE_REFERENCE(g_survival_mode_globals);

	if (current_game_engine())
		return &game_engine_globals->statborg;

	//if (game_is_survival())
	//	return survival_mode_get_statborg();

	if (game_is_survival())
		return &g_survival_mode_globals->statborg;

	return &campaign_statborg;
}

//.text:00550BC0 ; void __cdecl game_engine_get_team_color(enum e_game_team, union real_rgb_color *)

long __cdecl game_engine_get_team_place(long team)
{
	return INVOKE(0x00550BF0, game_engine_get_team_place, team);
}

//.text:00550C20 ; long __cdecl game_engine_get_team_rounds_won(enum e_game_team)
//.text:00550C90 ; long __cdecl game_engine_get_team_score(enum e_game_team)

//
long __cdecl game_engine_get_team_score_for_display(long team, bool final_score)
{
	return INVOKE(0x00550D10, game_engine_get_team_score_for_display, team, final_score);
}

//.text:00550D70 ; long __cdecl game_engine_get_time_left_in_ticks(bool)
//.text:00550E10 ; void __cdecl game_engine_get_timer(long *, long *)
//.text:00550E80 ; enum e_game_engine_type __cdecl game_engine_get_type(void)

void __cdecl game_engine_get_universal_string(long a1, c_static_wchar_string<256>* formatted_string)
{
	INVOKE(0x00550EA0, game_engine_get_universal_string, a1, formatted_string);
}

//.text:00550EF0 ; long __cdecl game_engine_get_winning_player(void)
//.text:00550F50 ; enum e_multiplayer_team __cdecl game_engine_get_winning_team(void)
//.text:00550F80 ; bool __cdecl game_engine_grenades_on_map_allowed(void)

bool __cdecl game_engine_in_round()
{
	return INVOKE(0x00550F90, game_engine_in_round);
}

//.text:00550FF0 ; void __cdecl game_engine_initialize(void)
//.text:00551140 ; void __cdecl game_engine_initialize_for_new_game(void)
//.text:005511A0 ; void __cdecl game_engine_initialize_for_new_map(void)
//.text:00551590 ; void __cdecl game_engine_initialize_for_new_round(void)
//.text:00551750 ; 

void __cdecl game_engine_interface_update(float world_seconds_elapsed)
{
	//INVOKE(0x00551780, game_engine_interface_update, world_seconds_elapsed);

	TLS_DATA_GET_VALUE_REFERENCE(player_data);
	TLS_DATA_GET_VALUE_REFERENCE(local_game_engine_globals);

	if (game_in_progress() && !game_is_ui_shell())
	{
		for (e_output_user_index output_user_index = player_mapping_first_active_output_user(); output_user_index != NONE; output_user_index = player_mapping_next_active_output_user(output_user_index))
		{
			long player_index = player_mapping_get_player_by_output_user(output_user_index);
			player_datum* player = (player_datum*)datum_try_and_get(*player_data, player_index);
			e_controller_index controller_index = controller_index_from_output_user_index(output_user_index);
			if (controller_index != k_no_controller)
			{
				s_game_input_state* input_state = NULL;
				input_abstraction_get_input_state(controller_index, &input_state);
				bool back_pressed = input_state && input_state->get_button(_button_action_back).down_frames() != 0;

				if (!current_game_engine() || game_engine_in_round())
				{
					local_game_engine_globals->__time0 = LONG_MAX;
				}
				else if (local_game_engine_globals->__time0 == LONG_MAX)
				{
					local_game_engine_globals->__time0 = game_time_get() + game_seconds_integer_to_ticks(1);
				}

				if (user_interface_should_show_console_scoreboard(NULL))
				{
					c_gui_screen_scoreboard::hide_scoreboard(controller_index);
				}
				else
				{
					c_gui_screen_scoreboard* screen = c_gui_screen_scoreboard::get_scoreboard_screen(controller_index);
					if (!screen || !window_manager_get()->get_screen_above(screen->get_render_window(), screen))
					{
						bool show_scoreboard = back_pressed && (game_is_cooperative() || game_is_multiplayer()) && player_control_get_zoom_level(output_user_index) == (short)NONE;

						if (current_game_engine())
						{
							if (game_time_get() >= local_game_engine_globals->__time0)
								show_scoreboard = true;

							if (player->unit_index == NONE
								&& player->respawn_timer_countdown <= 1
								&& !TEST_BIT(player->flags, _player_sitting_out_bit)
								&& !game_engine_player_is_out_of_lives(player_index)
								&& game_time_get() > game_seconds_integer_to_ticks(3)
								&& TEST_BIT(player->flags, _player_active_in_game_bit)
								&& (!game_is_playback()
									|| director_get(output_user_index)->get_type() != _director_mode_saved_film
									|| !director_get(output_user_index)->in_free_camera_mode()))
							{
								show_scoreboard = true;
							}
						}

						if (show_scoreboard)
							c_gui_screen_scoreboard::show_scoreboard(controller_index, back_pressed);
						else
							c_gui_screen_scoreboard::hide_scoreboard(controller_index);
					}
				}

				if (current_game_engine())
				{
					long griefer_player_index = NONE;
					if (game_engine_player_is_dead_and_betrayed_by_griefer(player_index, &griefer_player_index))
					{
						if (game_grief_can_eject(player_index) && !game_grief_get_ui_active_for_local_user(controller_index))
						{
							player_datum* griefer = (player_datum*)datum_try_and_get(*player_data, griefer_player_index);
							e_window_index window_index = user_interface_get_window_for_controller(controller_index);
				
							if (c_load_boot_betrayer_screen_message* message = new c_load_boot_betrayer_screen_message(controller_index, window_index, STRING_ID(gui, top_most), &player->player_identifier, &griefer->player_identifier))
							{
								user_interface_messaging_post(message);
								sub_6790A0(controller_index, true);
							}
						}
					}
				}
			}
		}
	}

	game_engine_update_global_fade_timers(world_seconds_elapsed);
}

//.text:005519E0 ; bool __cdecl game_engine_monitoring_object(long)
//.text:00551A10 ; void __cdecl game_engine_multiplayer_weapon_drop_internal(long, long)
//.text:00551A90 ; struct s_multiplayer_weapon_tracker const * __cdecl game_engine_multiplayer_weapon_get(short)
//.text:00551B00 ; struct s_multiplayer_weapon_tracker * __cdecl game_engine_multiplayer_weapon_get_internal(long)
//.text:00551B40 ; void __cdecl game_engine_multiplayer_weapon_reset(long, union real_point3d const *)
//.text:00551BE0 ; void __cdecl game_engine_multiplayer_weapons_delete_all(void)
//.text:00551C50 ; bool __cdecl game_engine_nearby_player_running_towards_object(long)
//.text:00551DA0 ; long __cdecl game_engine_object_get_emblem_player(long)
//.text:00551E40 ; 
//.text:00551E50 ; bool __cdecl game_engine_player_has_infinite_ammo(long)
//.text:00551EA0 ; bool __cdecl game_engine_player_has_shield(long)
//.text:00551F00 ; bool __cdecl game_engine_player_is_being_respawned(long)

bool __cdecl game_engine_player_is_dead_and_betrayed_by_griefer(long player_index, long* griefer_player_index)
{
	return INVOKE(0x00551F50, game_engine_player_is_dead_and_betrayed_by_griefer, player_index, griefer_player_index);
}

bool __cdecl game_engine_player_is_out_of_lives(long player_index)
{
	return INVOKE(0x00552020, game_engine_player_is_out_of_lives, player_index);
}

bool __cdecl game_engine_player_is_playing(long player_index)
{
	return INVOKE(0x00552090, game_engine_player_is_playing, player_index);
}

void __cdecl game_engine_player_set_spawn_timer(long player_index, long spawn_time)
{
	INVOKE(0x00552100, game_engine_player_set_spawn_timer, player_index, spawn_time);
}

//.text:00552180 ; bool __cdecl game_engine_player_waiting_to_spawn_initially(long)
//.text:005521C0 ; 

// ED:ServerCountdown
long const k_initial_loadout_selection_time_seconds = 4;

long __cdecl game_engine_get_pre_round_ticks()
{
	//INVOKE(0x005521D0, game_engine_get_pre_round_ticks);
	//return game_seconds_integer_to_ticks(24);

	if (game_is_multiplayer() && current_game_variant())
		return game_seconds_integer_to_ticks(k_initial_loadout_selection_time_seconds);

	return game_seconds_integer_to_ticks(10);
}

long __cdecl game_engine_get_post_round_ticks()
{
	//return game_seconds_integer_to_ticks(14);

	if (game_is_finished())
	{
		if (game_is_campaign())
			return game_seconds_integer_to_ticks(1);

		return game_seconds_integer_to_ticks(7);
	}

	return game_seconds_integer_to_ticks(4);
}

//.text:005521F0 ; void __cdecl game_engine_recompute_team_score(enum e_multiplayer_team, long)
//.text:00552230 ; void __cdecl game_engine_reset_players_for_new_round(void)
//.text:00552280 ; void __cdecl game_engine_reset_round_conditions_for_new_round(void)
//.text:005522B0 ; void __cdecl game_engine_reset_stats_for_round_switch(void)
//.text:00552340 ; void __cdecl game_engine_respond_to_betrayal(long, long)
//.text:005523A0 ; bool __cdecl game_engine_round_condition_test(enum e_game_engine_round_condition)

long __cdecl game_engine_round_time_get()
{
	//INVOKE(0x005523D0, game_engine_round_time_get);

	TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);

	if ((game_time_get() - game_engine_globals->round_timer) >= 0)
		return game_time_get() - game_engine_globals->round_timer;

	return 0;
}

bool __cdecl game_engine_running()
{
	//return INVOKE(0x00552410, game_engine_running);

	return current_game_engine() != NULL;
}

//.text:00552420 ; void __cdecl game_engine_sandbox_get_string(long, class c_static_wchar_string<1024> *)
//.text:00552480 ; 
//.text:005524D0 ; void __cdecl game_engine_set_desired_state(long)
//.text:00552500 ; void __cdecl game_engine_set_player_navpoint_action(long, char)
//.text:00552590 ; bool __cdecl game_engine_should_auto_pickup_weapon(long, long)
//.text:005525C0 ; bool __cdecl game_engine_should_end_round(long *, bool *, enum e_game_engine_end_condition *)
//.text:005526F0 ; void __cdecl game_engine_spawn_monitor_detach(long)

void __cdecl game_engine_update()
{
	INVOKE(0x00552740, game_engine_update);

	//if (current_game_engine())
	//{
	//	TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);
	//
	//	game_engine_update_time();
	//	game_engine_update_round_conditions();
	//	game_engine_update_multiplayer_sound();
	//	game_engine_garbage_collect();
	//	game_engine_globals->candy_monitor_manager.update();
	//	game_engine_update_talkers();
	//	game_engine_teleporters_update();
	//
	//	c_player_in_game_iterator player_in_game_iter{};
	//	while (player_in_game_iter.next())
	//		game_engine_update_player(player_in_game_iter.get_index());
	//
	//	game_engine_spawn_influencers_update();
	//
	//	if (game_engine_has_teams())
	//	{
	//		for (long team_index = 0; team_index < 8; team_index++)
	//		{
	//			if (game_engine_is_team_active(team_index))
	//				game_engine_update_team(team_index);
	//		}
	//	}
	//
	//	current_game_engine()->update();
	//
	//	if (!game_is_predicted())
	//		game_engine_scoring_update_leaders();
	//
	//	game_engine_update_player_states();
	//	sub_553210();
	//	game_engine_update_player_sitting_out();
	//	game_engine_check_for_round_winner();
	//
	//	if (game_engine_has_teams() && game_is_authoritative())
	//	{
	//		if (TEST_MASK(game_engine_globals->valid_teams, ~game_engine_globals->initial_teams) ||
	//			TEST_MASK(game_engine_globals->game_simulation, ~game_engine_globals->valid_teams) ||
	//			TEST_MASK(game_engine_globals->active_teams, ~game_engine_globals->game_simulation))
	//		{
	//			game_engine_end_round_with_winner(NONE, true, _game_engine_end_condition_game_end_invalid_team_mapping);
	//		}
	//	}
	//
	//	game_engine_globals->__unknownF9AA = game_engine_in_round() ? 0 : game_engine_globals->__unknownF9AA + 1;
	//	game_engine_events_update();
	//}
	//else if (game_is_survival())
	//{
	//	game_engine_update_multiplayer_sound();
	//	game_engine_events_update();
	//}
}

void __cdecl game_engine_update_after_game()
{
	INVOKE(0x005528E0, game_engine_update_after_game);

	//if (current_game_engine() && game_is_authoritative())
	//{
	//	TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);
	//
	//	if (game_is_finished())
	//	{
	//		if (!game_engine_globals->game_finished)
	//		{
	//			game_engine_globals->game_finished = true;
	//			c_flags<long, qword, 64> flags = FLAG(3);
	//			simulation_action_game_engine_globals_update(flags);
	//
	//			if (game_engine_globals1->current_state == 1)
	//			{
	//				if (game_engine_globals1->desired_state == 1)
	//					game_engine_end_round_with_winner(game_engine_choose_winner_index(), false, _game_engine_end_condition_game_end_external);
	//				else
	//					game_engine_update_after_game_update_state();
	//			}
	//
	//			game_engine_reset_stats_for_round_switch();
	//
	//			s_game_engine_event_data event_data{};
	//			game_engine_initialize_event(_event_type_general, _game_engine_string_id_general_event_game_over, &event_data);
	//			game_engine_send_event(&event_data);
	//
	//			if (game_results_get_game_recording() && !game_results_get_game_finalized())
	//			{
	//				if (c_game_statborg* statborg = game_engine_get_statborg())
	//				{
	//					statborg->stats_pause();
	//					statborg->stats_finalize_for_game_end();
	//				}
	//
	//				game_results_set_recording_pause(false);
	//				game_engine_update_score_and_standing();
	//				game_results_set_game_finished();
	//				game_results_set_recording_pause(true);
	//				game_results_finalize();
	//			}
	//		}
	//	}
	//	else
	//	{
	//		if (game_engine_globals->current_state == 2)
	//		{
	//			if (--game_engine_globals->round_end_ticks <= 0)
	//			{
	//				if (game_engine_globals->round_index < 31)
	//				{
	//					game_engine_set_desired_state(3);
	//					game_engine_globals->candy_monitor_manager.dispose_from_old_round();
	//					game_engine_update_after_game_update_state();
	//				}
	//				else
	//				{
	//					game_engine_end_game(_game_engine_end_condition_game_end_exceeded_maximum_rounds);
	//				}
	//			}
	//			else
	//			{
	//				//printf("game engine exceeded maximum rounds, bailing!");
	//			};
	//		}
	//		else if (game_engine_globals_->current_state == 3 && --game_engine_globals_->round_end_ticks <= 0)
	//		{
	//			game_engine_set_desired_state(1);
	//			game_engine_update_after_game_update_state();
	//		}
	//		else
	//		{
	//			game_engine_update_after_game_update_state();
	//		}
	//	}
	//}
}

//.text:00552AC0 ; void __cdecl game_engine_update_after_game_update_state(void)

void __cdecl game_engine_update_global_fade_timers(real world_seconds_elapsed)
{
	INVOKE(0x00552D40, game_engine_update_global_fade_timers, world_seconds_elapsed);
}

//.text:00552FB0 ; void __cdecl game_engine_update_player(long)
//.text:00553200 ; void __cdecl game_engine_update_player_movement_traits(void)
//.text:00553210 ; 
//.text:00553290 ; void __cdecl game_engine_update_player_no_shield(long)
//.text:00553320 ; void __cdecl game_engine_update_player_sitting_out(void)
//.text:005533A0 ; void __cdecl game_engine_update_player_states(void)
//.text:005534C0 ; void __cdecl game_engine_update_player_target(long)

void __cdecl game_engine_update_round_conditions()
{
	//INVOKE(0x00553660, game_engine_update_round_conditions);

	if (!game_is_authoritative())
		return;

	TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);

	long round_time = game_engine_round_time_get();

	c_flags<long, byte, 8> round_condition_flags(round_time < 5);

	round_condition_flags.set(1, round_time < game_seconds_integer_to_ticks(1));
	round_condition_flags.set(2, round_time < game_seconds_integer_to_ticks(3));
	round_condition_flags.set(3, round_time < game_seconds_integer_to_ticks(4));
	round_condition_flags.set(4, round_time < game_engine_get_post_round_ticks());
	round_condition_flags.set(5, round_time < game_engine_get_pre_round_ticks());
	round_condition_flags.set(6, round_time < game_seconds_integer_to_ticks(5));
	round_condition_flags.set(7, round_time < game_seconds_integer_to_ticks(4));

	if (game_engine_globals->round_condition_flags == round_condition_flags)
		return;

	if (game_engine_globals->round_condition_flags.test(3) && !round_condition_flags.test(3))
	{
		TLS_DATA_GET_VALUE_REFERENCE(player_data);

		c_player_in_game_iterator player_iterator;
		player_iterator.begin();
		while (player_iterator.next())
		{
			current_game_engine()->emit_game_start_event(player_iterator.get_index());
		}
	}

	c_flags<long, qword, 64> flags(64);
	simulation_action_game_engine_globals_update(flags);
	game_engine_globals->round_condition_flags = round_condition_flags;
}

//.text:005537F0 ; void __cdecl game_engine_update_score_and_standing(void)
//.text:00553B60 ; void __cdecl game_engine_update_talkers(void)
//.text:00553D20 ; void __cdecl game_engine_update_team(enum e_multiplayer_team)
//.text:00553DB0 ; void __cdecl game_engine_update_time(void)
//.text:00553FC0 ; bool __cdecl game_engine_validate_team_designator_for_new_map(enum e_multiplayer_team_designator)
//.text:00553FF0 ; bool __cdecl game_engine_vehicles_indestructible(void)
//.text:00554010 ; 
//.text:00554090 ; public: enum e_aura_setting __cdecl c_player_trait_appearance::get_aura_setting(void)const
//.text:005540A0 ; 
//.text:005540B0 ; 
//.text:005540C0 ; short __cdecl get_current_game_engine_index(void)
//.text:005540E0 ; 
//.text:005540F0 ; 
//.text:00554100 ; 
//.text:00554110 ; short __cdecl get_ever_active_team_or_player_count(void)
//.text:00554190 ; public: enum e_forced_change_color_setting __cdecl c_player_trait_appearance::get_forced_change_color_setting(void)const
//.text:005541A0 ; 
//.text:005541B0 ; char const * __cdecl get_game_engine_name(short)
//.text:005541C0 ; 
//.text:005541D0 ; 
//.text:005541E0 ; 
//.text:005541F0 ; public: enum e_infinite_ammo_setting __cdecl c_player_trait_weapons::get_infinite_ammo_setting(void)const
//.text:00554200 ; 
//.text:00554210 ; void __cdecl get_living_or_connecting_team_count(long *, long *, long *)
//.text:00554330 ; 
//.text:00554340 ; 
//.text:00554350 ; 
//.text:00554360 ; long __cdecl get_player_index_from_object_or_parents(long)
//.text:00554590 ; 
//.text:00554600 ; 
//.text:00554620 ; 
//.text:00554640 ; 
//.text:00554650 ; 
//.text:00554730 ; 
//.text:00554740 ; 
//.text:00554780 ; void __cdecl post_game_engine_globals_message(enum e_game_engine_globals_message_type, char, short)
//.text:005547F0 ; void __cdecl process_game_engine_globals_messages(void)
//.text:00554930 ; void __cdecl seconds_to_unicode_time_string(long, class c_static_wchar_string<256> *)
//.text:005549E0 ; 
//.text:00554A00 ; 
//.text:00554A40 ; 
//.text:00554A90 ; 
//.text:00554AC0 ; 
//.text:00554B00 ; 
//.text:00554B10 ; 
//.text:00554B30 ; 

void __cdecl game_engine_dump_variant_settings(char const* filename)
{
	if (game_is_multiplayer() && current_game_engine() && current_game_variant())
	{
		s_file_reference file;
		if (file_reference_create_from_path(&file, filename, false))
		{
			if (!file_exists(&file))
				file_create(&file);

			dword error = 0;
			if (file_exists(&file) && file_open(&file, FLAG(_file_open_flag_desired_access_write), &error))
			{
				c_game_engine const* game_engine = current_game_engine();
				switch (game_engine->get_type())
				{
					case _game_engine_type_none:
						game_engine->dump_settings(&file);
						break;
					case _game_engine_type_ctf:
						dynamic_cast<c_ctf_engine const*>(game_engine)->dump_settings(&file);
						break;
					case _game_engine_type_slayer:
						dynamic_cast<c_slayer_engine const*>(game_engine)->dump_settings(&file);
						break;
					case _game_engine_type_oddball:
						dynamic_cast<c_oddball_engine const*>(game_engine)->dump_settings(&file);
						break;
					case _game_engine_type_king:
						dynamic_cast<c_king_engine const*>(game_engine)->dump_settings(&file);
						break;
					case _game_engine_type_sandbox:
						dynamic_cast<c_sandbox_engine const*>(game_engine)->dump_settings(&file);
						break;
					case _game_engine_type_vip:
						dynamic_cast<c_vip_engine const*>(game_engine)->dump_settings(&file);
						break;
					case _game_engine_type_juggernaut:
						dynamic_cast<c_juggernaut_engine const*>(game_engine)->dump_settings(&file);
						break;
					case _game_engine_type_territories:
						dynamic_cast<c_territories_engine const*>(game_engine)->dump_settings(&file);
						break;
					case _game_engine_type_assault:
						dynamic_cast<c_assault_engine const*>(game_engine)->dump_settings(&file);
						break;
					case _game_engine_type_infection:
						dynamic_cast<c_infection_engine const*>(game_engine)->dump_settings(&file);
						break;
				}
				file_close(&file);
			}
			else
			{
				generate_event(_event_level_error, "game_engine: failed to create file to write variant settings!");
			}
		}
	}
	else
	{
		generate_event(_event_level_error, "game_engine: this game cannot export game engine variant settings!");
	}
}

//void __cdecl post_game_engine_globals_message(enum e_game_engine_globals_message_type, char, short)
void __cdecl post_game_engine_globals_message(long message_type, char a2, short a3)
{
	INVOKE(0x00554780, post_game_engine_globals_message, message_type, a2, a3);
}

void __cdecl process_game_engine_globals_messages()
{
	INVOKE(0x005547F0, process_game_engine_globals_messages);
}

//.text:00572120 ; void __cdecl set_variant<s_game_engine_assault_variant_definition>(s_game_engine_assault_variant_definition const*, c_game_engine_base_variant*)
//.text:00572130 ; void __cdecl set_variant<s_game_engine_ctf_variant_definition>(s_game_engine_ctf_variant_definition const*, c_game_engine_base_variant*)
//.text:00572140 ; void __cdecl set_variant<s_game_engine_infection_variant_definition>(s_game_engine_infection_variant_definition const*, c_game_engine_base_variant*)
//.text:00572150 ; void __cdecl set_variant<s_game_engine_juggernaut_variant_definition>(s_game_engine_juggernaut_variant_definition const*, c_game_engine_base_variant*)
//.text:00572160 ; void __cdecl set_variant<s_game_engine_king_variant_definition>(s_game_engine_king_variant_definition const*, c_game_engine_base_variant*)
//.text:00572170 ; void __cdecl set_variant<s_game_engine_oddball_variant_definition>(s_game_engine_oddball_variant_definition const*, c_game_engine_base_variant*)
//.text:00572180 ; void __cdecl set_variant<s_game_engine_sandbox_variant_definition>(s_game_engine_sandbox_variant_definition const*, c_game_engine_base_variant*)
//.text:00572190 ; void __cdecl set_variant<s_game_engine_slayer_variant_definition>(s_game_engine_slayer_variant_definition const*, c_game_engine_base_variant*)
//.text:005721A0 ; void __cdecl set_variant<s_game_engine_territories_variant_definition>(s_game_engine_territories_variant_definition const*, c_game_engine_base_variant*)
//.text:005721B0 ; void __cdecl set_variant<s_game_engine_vip_variant_definition>(s_game_engine_vip_variant_definition const*, c_game_engine_base_variant*)

