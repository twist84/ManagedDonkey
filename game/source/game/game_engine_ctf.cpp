#include "game/game_engine_ctf.hpp"

#include "memory/byte_swapping.hpp"

REFERENCE_DECLARE(0x050D96BC, c_game_engine_ctf_variant*, ctf_variant);

void c_game_engine_ctf_variant::byteswap()
{
	static_cast<c_game_engine_base_variant*>(this)->byteswap();

	bswap_word_inplace(m_touch_return_timeout);
	bswap_word_inplace(m_sudden_death_time);
	bswap_word_inplace(m_score_to_win);
	bswap_word_inplace(m_score_unknown);
	bswap_word_inplace(m_flag_reset_time);
	m_carrier_traits.byteswap();

	ASSERT(array_is_zeroed(m_pad1));
}

