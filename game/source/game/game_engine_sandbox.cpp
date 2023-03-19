#include "game/game_engine_sandbox.hpp"

#include "memory/byte_swapping.hpp"

void c_game_engine_sandbox_variant::byteswap()
{
	static_cast<c_game_engine_base_variant*>(this)->byteswap();

	bswap_word_inplace(m_respawn_time);
	m_all_player_traits.byteswap();
}

