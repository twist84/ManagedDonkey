#include "ai/ai_dialogue.hpp"

#include "ai/actors.hpp"
#include "render/render_debug.hpp"

double __cdecl dialogue_player_weighting(long unit_index)
{
	return INVOKE(0x014395E0, dialogue_player_weighting, unit_index);
}

short __cdecl dialogue_team_get(short team)
{
	return INVOKE(0x01439FE0, dialogue_team_get, team);
}

void ai_dialogue_render_player_weights()
{
	actor_iterator iterator{};
	actor_iterator_new(&iterator, true);
	while (actor_datum* actor = actor_iterator_next(&iterator))
	{
		real_argb_color const* color = dialogue_team_get(actor->meta.team) == 1 ? global_real_argb_red : global_real_argb_green;

		char string[50]{};
		csnzprintf(string, sizeof(string), "%.2f", dialogue_player_weighting(actor->meta.unit_index));
		render_debug_string_at_point(&actor->input.position.head_position, string, color);
	}
}

