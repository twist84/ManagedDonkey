#include "game/game_engine_slayer.hpp"

#include "memory/byte_swapping.hpp"

#include <assert.h>

void c_game_engine_slayer_variant::byteswap()
{
	static_cast<c_game_engine_base_variant*>(this)->byteswap();

	bswap_word_inplace(m_score_to_win);
	bswap_word_inplace(m_score_unknown);
	bswap_word_inplace(m_kill_points);
	m_leader_traits.byteswap();

	assert(array_is_zeroed(m_pad1));
}

