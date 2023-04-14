#include "game/game_engine_assault.hpp"

#include "memory/byte_swapping.hpp"

REFERENCE_DECLARE(0x0471AA14, c_game_engine_assault_variant*, assault_variant);

void c_game_engine_assault_variant::byteswap()
{
	c_game_engine_base_variant::byteswap();

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

	ASSERT(array_is_zeroed(m_pad1));
}

void c_assault_engine::dump_settings(s_file_reference* file) const
{
	ASSERT(file != NULL);

	c_ctf_engine::dump_settings(file);

}

