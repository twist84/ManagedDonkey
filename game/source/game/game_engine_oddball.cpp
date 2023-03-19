#include "game/game_engine_oddball.hpp"

#include "memory/byte_swapping.hpp"

#include <assert.h>

void c_game_engine_oddball_variant::byteswap()
{
	static_cast<c_game_engine_base_variant*>(this)->byteswap();

	bswap_dword_inplace(m_variant_flags);
	bswap_word_inplace(m_score_to_win);
	bswap_word_inplace(m_carrying_points);
	bswap_word_inplace(m_unknown_points);
	bswap_word_inplace(m_ball_spawn_delay);
	bswap_word_inplace(m_ball_inactive_respawn);
	m_carrier_traits.byteswap();

	assert(array_is_zeroed(m_pad1));
}

