#include "game/game_engine_traits.hpp"

#include "memory/byte_swapping.hpp"

void c_game_engine_miscellaneous_options::byteswap()
{
}

void c_game_engine_respawn_options::byteswap()
{
	m_respawn_player_traits.byteswap();
}

void c_game_engine_social_options::byteswap()
{
	bswap_word_inplace(m_flags);
	bswap_word_inplace(m_team_changing);
}

void c_game_engine_map_override_options::byteswap()
{
	bswap_dword_inplace(m_flags);
	m_base_player_traits.byteswap();
	bswap_word_inplace(m_weapon_set_absolute_index);
	bswap_word_inplace(m_vehicle_set_absolute_index);
	m_red_powerup_traits.byteswap();
	m_blue_powerup_traits.byteswap();
	m_yellow_powerup_traits.byteswap();
}

