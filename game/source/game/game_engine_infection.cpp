#include "game/game_engine_infection.hpp"

#include "memory/byte_swapping.hpp"

REFERENCE_DECLARE(0x050D9F84, c_game_engine_infection_variant*, infection_variant);

void c_game_engine_infection_variant::byteswap()
{
	c_game_engine_base_variant::byteswap();

	bswap_word_inplace(m_safe_haven_movement_time);
	m_zombie_traits.byteswap();
	m_first_zombie_traits.byteswap();
	m_safe_haven_defender_traits.byteswap();
	m_last_human_traits.byteswap();

	ASSERT(array_is_zeroed(m_pad1));
}

void c_infection_engine::dump_settings(s_file_reference* file) const
{
	ASSERT(file != NULL);

	c_game_engine::dump_settings(file);

}

