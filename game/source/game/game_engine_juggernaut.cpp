#include "game/game_engine_juggernaut.hpp"

#include "memory/byte_swapping.hpp"

REFERENCE_DECLARE(0x050D9F7C, c_game_engine_juggernaut_variant*, juggernaut_variant);

void c_game_engine_juggernaut_variant::byteswap()
{
	static_cast<c_game_engine_base_variant*>(this)->byteswap();

	bswap_word_inplace(m_score_to_win_round);
	bswap_word_inplace(m_score_unknown);

	ASSERT(array_is_zeroed(m_pad));

	m_juggernaut_traits.byteswap();

	ASSERT(array_is_zeroed(m_pad1));
}

void c_juggernaut_engine::dump_settings(s_file_reference* file) const
{
	ASSERT(file != NULL);

	c_game_engine::dump_settings(file);

}

