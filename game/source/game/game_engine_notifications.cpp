#include "game/game_engine_notifications.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game_engine_team.hpp"
#include "game/players.hpp"
#include "math/random_math.hpp"
#include "memory/thread_local.hpp"
#include "tag_files/string_ids.hpp"

//.text:005A0750 ; 
//.text:005A0780 ; 
//.text:005A0880 ; 
//.text:005A08D0 ; 
//.text:005A0900 ; 
//.text:005A09E0 ; int16 __cdecl game_engine_get_next_shot_id()
//.text:005A0A10 ; void __cdecl game_engine_handle_deleted_object(int32 object_index)
//.text:005A0A90 ; int32 __cdecl game_engine_inherit_respawn_timer(int32 dead_player_index, int32 current_respawn_time, int32 minimum_respawn_time)
//.text:005A0B40 ; void __cdecl game_engine_multiplayer_weapon_deregister(int32 weapon_index)
//.text:005A0C50 ; void __cdecl game_engine_multiplayer_weapon_dropped(int32 weapon_index, int32 unit_index)
//.text:005A0C70 ; void __cdecl game_engine_multiplayer_weapon_handle_player_change(int32 weapon_index)
//.text:005A0CD0 ; void __cdecl game_engine_multiplayer_weapon_picked_up(int32 weapon_index, int32 unit_index)
//.text:005A0D50 ; void __cdecl game_engine_multiplayer_weapon_register(int32 weapon_index, int16 multiplayer_weapon_identifier)
//.text:005A0E40 ; int16 __cdecl game_engine_notify_shot_fired(int32 player_index, const s_damage_reporting_info& damage_reporting_info)
//.text:005A0F40 ; void __cdecl game_engine_notify_shot_hit_player(int32 shooting_player_index, int32 shot_player_index, int16 shot_id)
//.text:005A0F50 ; void __cdecl game_engine_player_activated(int32 player_index)
//.text:005A0F70 ; void __cdecl game_engine_player_added(int32 player_index)

void __cdecl game_engine_player_changed_handicap_levels(int32 player_index, int16 old_handicap_level, int16 new_handicap_level)
{
	INVOKE(0x005A1130, game_engine_player_changed_handicap_levels, player_index, old_handicap_level, new_handicap_level);
}

void __cdecl game_engine_player_changed_indices(int32 player_old_index, int32 player_new_index)
{
	INVOKE(0x005A1230, game_engine_player_changed_indices, player_old_index, player_new_index);
}

void __cdecl game_engine_player_changed_names(int32 player_index, const wchar_t* old_name, const wchar_t* new_name)
{
	INVOKE(0x005A12B0, game_engine_player_changed_names, player_index, old_name, new_name);
}

void __cdecl game_engine_player_changed_teams(int32 player_index, e_game_team old_team_index, e_game_team new_team_index)
{
	INVOKE(0x005A12C0, game_engine_player_changed_teams, player_index, old_team_index, new_team_index);

#if 0 // $TODO
	if (current_game_engine() && game_engine_has_teams())
	{
		game_engine_get_statborg()->player_changed_teams(player_index);
		current_game_engine()->player_changed_teams(player_index);

		if (game_is_authoritative())
		{
			game_engine_recompute_active_teams();
			game_engine_adjust_team_score_for_composition(old_team_index);
			game_engine_adjust_team_score_for_composition(new_team_index);
		}

		if (!game_is_predicted() && game_time_get() > 1)
		{
			s_game_engine_event_data event_data{};
			game_engine_initialize_event(_game_engine_event_type_general, STRING_ID(game_engine, general_event_player_switched_team), &event_data);
			game_engine_set_event_cause_player_and_team(player_index, &event_data);
			event_data.effect_team_index = new_team_index;
			game_engine_send_event(&event_data);
		}
	}
#endif
}

//.text:005A1370 ; 
//.text:005A1380 ; void __cdecl game_engine_player_damaged_player(int32 killing_player_index, int32 killing_object_index, int32 dead_player_index, bool friendly_fire)

void __cdecl game_engine_player_deleted(int32 player_index)
{
	INVOKE(0x005A1460, game_engine_player_deleted, player_index);
}

//.text:005A14B0 ; void __cdecl game_engine_player_fired_weapon(int32 firing_player_index)
//.text:005A14D0 ; void __cdecl game_engine_player_indices_swapped(int32 player_absolute_index_a, int32 player_absolute_index_b)
//.text:005A14F0 ; void __cdecl game_engine_player_killed(int32 killing_player_index, int32 killing_object_index, int32 dead_player_index, bool friendly_fire, const s_damage_reporting_info& damage_reporting_info, bool killer_did_death_blow)
//.text:005A2370 ; void __cdecl game_engine_player_left(int32 player_index)

void __cdecl game_engine_player_nearby_item(int32 player_index, int32 item_index)
{
	INVOKE(0x005A2440, game_engine_player_nearby_item, player_index, item_index);
}

void __cdecl game_engine_player_prepare_to_change_team(int32 player_index, e_game_team team_index)
{
	//INVOKE(0x005A24C0, game_engine_player_prepare_to_change_team, player_index, team_index);

	static bool x_should_kill_player_on_change_team = true;
	if (x_should_kill_player_on_change_team && current_game_engine() && game_engine_has_teams() && !game_is_predicted())
	{
		player_datum* player = DATUM_GET(player_data, player_datum, player_index);

		int32 respawn_timer = player->respawn_timer;
		if (respawn_timer <= 2 * game_tick_rate())
		{
			respawn_timer = 2 * game_tick_rate();
		}

		if (team_index != player->configuration.host.team_index && player->unit_index != NONE)
		{
			random_seed_allow_use();
			object_kill_instantly(player->unit_index, false, true, true, NULL, { .type = _damage_reporting_type_guardians });
			random_seed_disallow_use();

			if (player->unit_index != NONE)
			{
				event(_event_error, "killed player 0x%08X but failed to detach from unit 0x%08X %s",
					player_index,
					player->unit_index,
					object_describe(player->unit_index));

				player_set_unit_index(player_index, NONE);
				player->respawn_timer = respawn_timer;
			}
		}
	}
}

//.text:005A2570 ; void __cdecl game_engine_player_rejoined(int32 player_index)
//.text:005A2670 ; void __cdecl game_engine_reset_respawn_stagger_variables()
//.text:005A26A0 ; void __cdecl game_engine_setup_player_for_respawn(int32 player_index)
//.text:005A2860 ; void __cdecl game_engine_unit_entered_vehicle(int32 unit_index, int32 vehicle_index)
//.text:005A2930 ; void __cdecl game_engine_unit_exited_vehicle(int32 unit_index, int32 vehicle_index)
//.text:005A29C0 ; void __cdecl game_engine_user_has_new_unit_index(int32 user_index, bool unit_index)

