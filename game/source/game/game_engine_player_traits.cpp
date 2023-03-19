#include "game/game_engine_player_traits.hpp"

#include "memory/byte_swapping.hpp"

void c_player_traits::byteswap()
{
	bswap_word_inplace(m_weapon_traits.m_initial_grenade_count);
	bswap_word_inplace(m_sensor_traits.m_motion_tracker_setting);
	bswap_word_inplace(m_sensor_traits.m_motion_tracker_range_setting);
}

