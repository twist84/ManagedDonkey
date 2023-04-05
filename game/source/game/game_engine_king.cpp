#include "game/game_engine_king.hpp"

#include "memory/byte_swapping.hpp"

REFERENCE_DECLARE(0x050D9F78, c_game_engine_king_variant*, king_variant);

void c_game_engine_king_variant::byteswap()
{
	static_cast<c_game_engine_base_variant*>(this)->byteswap();

	bswap_dword_inplace(m_variant_flags);
	bswap_word_inplace(m_score_to_win);
	bswap_word_inplace(m_score_unknown);
	m_inside_hill_traits.byteswap();

	ASSERT(array_is_zeroed(m_pad1));
}

