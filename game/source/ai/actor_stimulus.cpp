#include "ai/actor_stimulus.hpp"

#include "ai/actors.hpp"
#include "render/render_debug.hpp"

char const* const g_stimulus_names[k_stimulus_count]
{
	"surprise",
	"surprise_combat_start",
	"friendly_body",
	"shield_deplieted",
	"leader_dead",
	"peer_dead",
	"last_man",
	"stuck_with_grenade",
	"super_detonation",
	"null",
	"destructible_obstacle",
	"cover_friend",
	"activity",
	"changed target",
	"cover failed",
	"broken",
	"leader abandoned",
	"just flanked",
	"enemy vehicle",
	"turtle",
	"flood disperse"
};

// $TODO: `actor_debug_info`, `actor_debug_array`, `actor_debug_drawstack`

void stimuli_debug()
{
	//actor_iterator iterator{};
	//actor_iterator_new(&iterator, true);
	//while (actor_datum* actor = actor_iterator_next(&iterator))
	//{
	//	actor_debug_info* actor_debug = &actor_debug_array[DATUM_INDEX_TO_ABSOLUTE_INDEX(iterator.index)];
	//
	//	real_point3d position{};
	//	point_from_line3d(&actor->input.position.head, global_up3d, 0.1f, &position);
	//	for (long stimuli_index = 0; stimuli_index < k_stimulus_count; stimuli_index++)
	//	{
	//		if (TEST_BIT(actor_debug->actor_stimulus_flags, stimuli_index))
	//		{
	//			render_debug_string_at_point(ai_debug_drawstack(), g_stimulus_names[stimuli_index], global_real_argb_yellow);
	//		}
	//	}
	//}
}
