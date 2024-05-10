#include "ai/ai_player.hpp"

#include "ai/ai_debug.hpp"
#include "memory/thread_local.hpp"
#include "render/render_debug.hpp"

void debug_render_player_battle_vector()
{
	TLS_DATA_GET_VALUE_REFERENCE(g_ai_players);
	TLS_DATA_GET_VALUE_REFERENCE(player_data);

	for (short ai_player_index = 0; ai_player_index < 4; ai_player_index++)
	{
		ai_player_state& ai_player = g_ai_players[ai_player_index];
		if (ai_player.player_index != NONE)
		{
			player_datum* player = (player_datum*)datum_get(*player_data, ai_player.player_index);
			if (player->unit_index != NONE)
			{
				real_point3d unit_head_position{};
				unit_get_head_position(player->unit_index, &unit_head_position);
				ai_debug_drawstack_setup(&unit_head_position);

				real_argb_color const* color = magnitude3d(&ai_player.battle_vector) > 0.7f ? global_real_argb_green : global_real_argb_red;

				render_debug_vector(true, &unit_head_position, &ai_player.battle_vector, 1.0f, color);
				render_debug_string_at_point(ai_debug_drawstack(), c_string_builder("ticks since shooting: %i", ai_player.ticks_since_shooting).get_string(), color);
				render_debug_string_at_point(ai_debug_drawstack(), c_string_builder("ticks since threatening: %i", ai_player.ticks_since_threatening).get_string(), color);
			}
		}
	}
}

