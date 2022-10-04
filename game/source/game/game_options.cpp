#include "game/game_options.hpp"

#include <string.h>

c_game_variant::c_game_variant()
{
	m_game_engine_index = _game_engine_base_variant;
	memset(variant_data, 0, sizeof(variant_data));
}