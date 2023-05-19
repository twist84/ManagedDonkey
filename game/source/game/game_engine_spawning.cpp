#include "game/game_engine_spawning.hpp"

#include "game/game.hpp"
#include "game/game_engine_util.hpp"
#include "memory/data.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"

HOOK_DECLARE(0x005A41D0, game_engine_should_spawn_player);

bool __cdecl game_engine_should_spawn_player(long player_index)
{
	//INVOKE(0x005A41D0, game_engine_should_spawn_player, player_index);

	ASSERT(game_is_authoritative());
	
	if (!current_game_engine())
		return true;
	
	if (!game_engine_player_is_playing(player_index))
		return false;
	
	if (!game_engine_in_round())
		return false;

	TLS_DATA_GET_VALUE_REFERENCE(player_data);

	player_datum* player = (player_datum*)datum_try_and_get(player_data, player_index);

	if ((game_is_survival() || game_is_multiplayer()) && (game_engine_round_time_get() + player->respawn_timer) < game_engine_get_pre_round_ticks())
	{
		game_engine_player_set_spawn_timer(player_index, game_engine_get_pre_round_ticks() - game_engine_round_time_get());
		return false;
	}
	
	return player->respawn_forced || player->respawn_timer <= 0;
}

//.text:006D0440 ; s_player_spawn_influence const* __cdecl game_engine_ally_bias_spawn_influence_try_and_get();
//.text:006D04A0 ; bool __cdecl game_engine_build_equipment_spawn_influencer(long object_index, s_dynamic_spawn_influencer* influencer);
//.text:006D0590 ; bool __cdecl game_engine_build_player_spawn_influencer(s_player_spawn_influence const* player_spawn_influence, real_matrix4x3 const*, s_dynamic_spawn_influencer* influencer);
//.text:006D0670 ; bool __cdecl game_engine_build_player_spawn_influencer(s_player_spawn_influence const* player_spawn_influence, real_matrix4x3 const*, s_dynamic_spawn_influencer* influencer);
//.text:006D0750 ; bool __cdecl game_engine_build_projectile_spawn_influencer(long object_index, s_dynamic_spawn_influencer* influencer);
//.text:006D0890 ; bool __cdecl game_engine_build_respawn_zone_influencer(long object_index, s_dynamic_spawn_influencer* influencer);
//.text:006D09C0 ; bool __cdecl game_engine_build_vehicle_spawn_influencer(long object_index, s_dynamic_spawn_influencer* influencer);
//.text:006D0B50 ; bool __cdecl game_engine_build_weapon_spawn_influencer(long object_index, s_dynamic_spawn_influencer* influencer);
//.text:006D0C50 ; real __cdecl game_engine_calculate_cumulative_spawn_location_weight(real_point3d const* point, bool a2, bool a3, real a4, real a5, s_dynamic_spawn_influencer_collection const* spawn_influencer_collection);
//.text:006D0D50 ; s_netgame_goal_spawn_influence const* __cdecl game_engine_ctf_flag_away_spawn_influence_try_and_get();
//.text:006D0DB0 ; s_player_spawn_influence const* __cdecl game_engine_dead_teammate_spawn_influence_try_and_get(real* duration);
//.text:006D0E40 ; s_player_spawn_influence const* __cdecl game_engine_enemy_bias_spawn_influence_try_and_get();
//.text:006D0E40 ; s_player_spawn_influence const* __cdecl game_engine_enemy_bias_spawn_influence_try_and_get();
//.text:006D0EA0 ; s_player_spawn_influence const* __cdecl game_engine_enemy_forbid_spawn_influence_try_and_get();
//.text:006D0F00 ; s_equipment_spawn_influence const* __cdecl game_engine_equipment_spawn_influence_try_and_get();
//.text:006D1030 ; s_netgame_goal_spawn_influence const* __cdecl game_engine_infection_safe_zone_human_spawn_influence_try_and_get();
//.text:006D1090 ; s_netgame_goal_spawn_influence const* __cdecl game_engine_infection_safe_zone_zombie_spawn_influence_try_and_get();
//.text:006D10F0 ; s_netgame_goal_spawn_influence const* __cdecl game_engine_koth_hill_spawn_influence_try_and_get();
//.text:006D1190 ; s_netgame_goal_spawn_influence const* __cdecl game_engine_oddball_spawn_influence_try_and_get();
//.text:006D1290 ; s_projectile_spawn_influence const* __cdecl game_engine_projectile_spawn_influence_try_and_get(long object_index, vector3d* translational_velocity);
//.text:006D1370 ; s_player_spawn_influence const* __cdecl game_engine_selected_ally_bias_spawn_influence_try_and_get();
//.text:006D13D0 ; void __cdecl game_engine_spawn_influence_add_equipment_influence(player_datum const* player, s_dynamic_spawn_influencer_collection* spawn_influencer_collection);
//.text:006D14D0 ; void __cdecl game_engine_spawn_influence_add_netgame_goal_object_influence(long player_index, player_datum const* player, s_dynamic_spawn_influencer_collection* spawn_influencer_collection);
//.text:006D1700 ; void __cdecl game_engine_spawn_influence_add_projectile_influence(player_datum const* player, s_dynamic_spawn_influencer_collection* spawn_influencer_collection);
//.text:006D1800 ; bool __cdecl game_engine_spawn_influencer_build(e_dynamic_spawn_influencer_type influencer_type, long object_index, s_dynamic_spawn_influencer* influencer, real* duration);
//.text:006D1990 ; game_engine_spawn_influencer_get_?;
//.text:006D19D0 ; game_engine_spawn_influencer_get_?;
//.text:006D1A20 ; game_engine_spawn_influencer_get_?;
//.text:006D1A40 ; void __cdecl game_engine_spawn_influencer_record_player_death(long player_index);
//.text:006D1B10 ; void __cdecl game_engine_spawn_influencer_record_player_spawn(long player_index); // nullsub
//.text:006D1B20 ; bool __cdecl game_engine_spawn_influencer_test_point(real_point3d const* point, s_dynamic_spawn_influencer const* influencer, real* out_percentage_of_nominal_weight);
//.text:006D1D70 ; return true;
//.text:006D1D80 ; void __cdecl game_engine_spawn_influencers_add_ally_bias(long player_index, player_datum const* player, s_dynamic_spawn_influencer_collection* spawn_influencer_collection);
//.text:006D1E60 ; void __cdecl game_engine_spawn_influencers_add_dead_teammate_influence(long player_index, player_datum const* player, s_dynamic_spawn_influencer_collection* spawn_influencer_collection);
//.text:006D1FF0 ; void __cdecl game_engine_spawn_influencers_add_enemy_bias_influence(long player_index, player_datum const* player, s_dynamic_spawn_influencer_collection* spawn_influencer_collection);
//.text:006D2110 ; void __cdecl game_engine_spawn_influencers_add_enemy_forbid_influence(long player_index, player_datum const* player, s_dynamic_spawn_influencer_collection* spawn_influencer_collection);
//.text:006D2230 ; void __cdecl game_engine_spawn_influencers_add_user_placed_influence(long player_index, s_dynamic_spawn_influencer_collection* spawn_influencer_collection);
//.text:006D2360 ; void __cdecl game_engine_spawn_influencers_add_vehicle_influence(player_datum const* player, s_dynamic_spawn_influencer_collection* spawn_influencer_collection);
//.text:006D2460 ; void __cdecl game_engine_spawn_influencers_add_weapon_influence(long player_index, player_datum const* player, s_dynamic_spawn_influencer_collection* spawn_influencer_collection);
//.text:006D26A0 ; void __cdecl game_engine_spawn_influencers_build_collection_for_player(long player_index, s_dynamic_spawn_influencer_collection* spawn_influencer_collection);
//.text:006D2740 ; void __cdecl game_engine_spawn_influencers_dispose();
//.text:006D2750 ; void __cdecl game_engine_spawn_influencers_dispose_from_old_map();
//.text:006D2760 ; void __cdecl game_engine_spawn_influencers_initialize();
//.text:006D2770 ; void __cdecl game_engine_spawn_influencers_initialize_for_new_map();
//.text:006D27D0 ; void __cdecl game_engine_spawn_influencers_initialize_for_new_round();
//.text:006D2830 ; void __cdecl game_engine_spawn_influencers_render();
//.text:006D2840 ; void __cdecl game_engine_spawn_influencers_update();
//.text:006D2940 ; s_netgame_goal_spawn_influence const* __cdecl game_engine_territories_ally_spawn_influence_try_and_get();
//.text:006D29A0 ; s_netgame_goal_spawn_influence const* __cdecl game_engine_territories_enemy_spawn_influence_try_and_get();
//.text:006D2A00 ; s_vehicle_spawn_influence const* __cdecl game_engine_vehicle_spawn_influence_try_and_get(long object_index, vector3d* translational_velocity, real* bounding_sphere_radius);
//.text:006D2B80 ; s_netgame_goal_spawn_influence const* __cdecl game_engine_vip_spawn_influence_try_and_get();
//.text:006D2BE0 ; s_weapon_spawn_influence const* __cdecl game_engine_weapon_spawn_influence_try_and_get(long object_index);

