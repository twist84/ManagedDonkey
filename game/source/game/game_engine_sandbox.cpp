#include "game/game_engine_sandbox.hpp"

#include "memory/byte_swapping.hpp"

REFERENCE_DECLARE(0x0240648C, c_game_engine_sandbox_variant*, sandbox_variant);

void c_game_engine_sandbox_variant::byteswap()
{
	static_cast<c_game_engine_base_variant*>(this)->byteswap();

	bswap_word_inplace(m_respawn_time);
	m_player_traits.byteswap();
}

void c_sandbox_engine::dump_settings(s_file_reference* file) const
{
	ASSERT(file != NULL);

	c_game_engine::dump_settings(file);

}

