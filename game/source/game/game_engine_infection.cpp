#include "game/game_engine_infection.hpp"

#include "memory/byte_swapping.hpp"

#include <assert.h>

void c_game_engine_infection_variant::byteswap()
{
	static_cast<c_game_engine_base_variant*>(this)->byteswap();

	bswap_word_inplace(m_safe_haven_movement_time);
	m_zombie_traits.byteswap();
	m_first_zombie_traits.byteswap();
	m_safe_haven_defender_traits.byteswap();
	m_last_human_traits.byteswap();

	assert(array_is_zeroed(m_pad1));
}

