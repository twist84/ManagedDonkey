#include "saved_games/saved_film.hpp"

#include "game/game.hpp"

bool g_saved_films_show_timestamp = false;

void saved_film_render_debug()
{
	if (game_in_progress() && game_is_playback() && g_saved_films_show_timestamp)
	{
		// #TODO: implement
	}
}

