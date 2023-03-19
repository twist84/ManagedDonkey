#include "game/game_engine_assault.hpp"

#include "memory/byte_swapping.hpp"

#include <assert.h>

void c_game_engine_assault_variant::byteswap()
{
	static_cast<c_game_engine_base_variant*>(this)->byteswap();

	bswap_word_inplace(m_variant_flags);
	bswap_word_inplace(m_respawn);
	bswap_word_inplace(m_game_type);
	bswap_word_inplace(m_enemy_bomb_waypoint);
	bswap_word_inplace(m_score_to_win);
	bswap_word_inplace(m_score_unknown0);
	bswap_word_inplace(m_score_unknown1);
	bswap_word_inplace(m_score_unknown2);
	bswap_word_inplace(m_score_unknown3);
	bswap_word_inplace(m_sudden_death_time);
	bswap_word_inplace(m_bomb_reset_time);
	bswap_word_inplace(m_bomb_arming_time);
	bswap_word_inplace(m_bomb_disarming_time);
	bswap_word_inplace(m_bomb_fuse_time);
	m_carrier_traits.byteswap();
	m_arming_traits.byteswap();

	assert(array_is_zeroed(m_pad1));
}

