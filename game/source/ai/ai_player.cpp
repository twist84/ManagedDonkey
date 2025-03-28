#include "ai/ai_player.hpp"

#include "ai/ai_debug.hpp"
#include "memory/thread_local.hpp"
#include "render/render_debug.hpp"

//.text:014355F0 ; 
//.text:01435610 ; 
//.text:01435620 ; void __cdecl __tls_set_g_ai_players_allocator(void*)
//.text:01435640 ; bool __cdecl actor_player_is_attacking(long, real_vector3d*)
//.text:014356E0 ; void __cdecl actor_player_looking_update(long)
//.text:014358A0 ; void __cdecl actor_player_prop_state_refresh(long, long)
//.text:01435B10 ; void __cdecl actor_player_prop_status_refresh(long, long, real_point3d const*, short)
//.text:01435B80 ; void __cdecl actor_player_update(long)
//.text:01435BA0 ; void __cdecl actor_player_update_combat_movement(long)
//.text:01435DE0 ; void __cdecl actor_player_update_vehicle_interest(long)
//.text:014363C0 ; 
//.text:01436420 ; 
//.text:01436480 ; short __cdecl ai_player_index_get(long)

void __cdecl ai_player_initialize()
{
	INVOKE(0x014364C0, ai_player_initialize);
}

//.text:01436520 ; void __cdecl ai_player_state_create(long)
//.text:01436590 ; void __cdecl ai_player_state_delete(long)
//.text:01436600 ; ai_player_state* __cdecl ai_player_state_get(long)
//.text:01436650 ; void __cdecl ai_player_state_handle_deleted_object(long)

bool __cdecl ai_player_state_needs_vehicle(short ai_player_index)
{
	//return INVOKE(0x014366A0, ai_player_state_needs_vehicle, ai_player_index);

	TLS_DATA_GET_VALUE_REFERENCE(g_ai_players);

	ai_player_state& ai_player = g_ai_players[ai_player_index];
	if (ai_player.player_index != NONE)
	{
		if ((game_time_get() - ai_player.needs_vehicle_time) < game_seconds_to_ticks_round(5.0f))
			return !ai_player.needs_vehicle;
		else
			return ai_player.needs_vehicle;
	}

	return false;
}

void __cdecl ai_player_state_update()
{
	INVOKE(0x01436700, ai_player_state_update);
}

void __cdecl ai_players_initialize_for_new_map()
{
	INVOKE(0x01436E10, ai_players_initialize_for_new_map);
}

void __cdecl ai_players_reset()
{
	INVOKE(0x01436E20, ai_players_reset);
}

//.text:01436E90 ; 
//.text:01436ED0 ; 
//.text:01436F00 ; 
//.text:01436F30 ; 
//.text:01437140 ; 

void debug_render_player_battle_vector()
{
	TLS_DATA_GET_VALUE_REFERENCE(g_ai_players);
	TLS_DATA_GET_VALUE_REFERENCE(player_data);

	for (short ai_player_index = 0; ai_player_index < 4; ai_player_index++)
	{
		ai_player_state& ai_player = g_ai_players[ai_player_index];
		if (ai_player.player_index != NONE)
		{
			player_datum* player = DATUM_GET(player_data, player_datum, ai_player.player_index);
			if (player->unit_index != NONE)
			{
				real_point3d unit_head_position{};
				unit_get_head_position(player->unit_index, &unit_head_position);
				ai_debug_drawstack_setup(&unit_head_position);

				real_argb_color const* color = magnitude3d(&ai_player.battle_vector) > 0.7f ? global_real_argb_green : global_real_argb_red;

				render_debug_vector(true, &unit_head_position, &ai_player.battle_vector, 1.0f, color);
				render_debug_string_at_point(ai_debug_drawstack(), c_string_builder("ticks since shooting: %i", ai_player.ticks_since_shooting).get_string(), color);
				render_debug_string_at_point(ai_debug_drawstack(), c_string_builder("ticks since threatening: %i", ai_player.ticks_since_player_threatening).get_string(), color);
			}
		}
	}
}

void debug_render_player_needs_vehicle()
{
	TLS_DATA_GET_VALUE_REFERENCE(g_ai_players);
	TLS_DATA_GET_VALUE_REFERENCE(player_data);

	for (short ai_player_index = 0; ai_player_index < 4; ai_player_index++)
	{
		ai_player_state& ai_player = g_ai_players[ai_player_index];
		if (ai_player.player_index != NONE)
		{
			player_datum* player = DATUM_GET(player_data, player_datum, ai_player.player_index);
			if (player->unit_index != NONE)
			{
				real_point3d unit_head_position{};
				unit_get_head_position(player->unit_index, &unit_head_position);

				real_argb_color const* color = ai_player_state_needs_vehicle(ai_player_index) ? global_real_argb_green : global_real_argb_red;

				render_debug_string_at_point(&unit_head_position, c_string_builder("need vehicle time: %.2f", game_ticks_to_seconds(real(game_time_get() - ai_player.needs_vehicle_time))).get_string(), color);
			}
		}
	}
}

