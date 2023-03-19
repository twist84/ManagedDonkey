#include "game/game_engine_sandbox.hpp"

#include "memory/byte_swapping.hpp"

void c_game_engine_sandbox_variant::byteswap()
{
	bswap_word_inplace(m_respawn_time);
	m_all_player_traits.byteswap();
}

