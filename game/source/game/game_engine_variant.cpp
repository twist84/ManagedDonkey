#include "game/game_engine_variant.hpp"

const char* k_game_engine_variant_names[k_game_engine_variant_count] =
{
	"base",
	"ctf",
	"slayer",
	"oddball",
	"king",
	"sandbox",
	"vip",
	"juggernaut",
	"territories",
	"assault",
	"infection"
};

const char* game_engine_variant_get_name(long game_engine_index)
{
	if (game_engine_index < _game_engine_base_variant || game_engine_index >= k_game_engine_variant_count)
		return "<invalid 'game_engine_index'>";

	return k_game_engine_variant_names[game_engine_index];
}