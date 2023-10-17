#include "data_mining/data_mine_usability.hpp"

#include "game/game.hpp"

s_persisted_usability_globals g_persisted_usability_globals;

s_persisted_usability_globals::s_persisted_usability_globals() :
	display_mission_segment(false),
	mission_segment_buffer(),
	user_experience_buffer()
{
}

void data_mine_render_mission_segment()
{
	if (g_persisted_usability_globals.display_mission_segment && game_in_progress() && game_is_campaign())
	{
		// #TODO: implement
	}
}

