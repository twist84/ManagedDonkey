#include "game/game_engine_traits.hpp"

#include "memory/byte_swapping.hpp"

void c_game_engine_miscellaneous_options::byteswap()
{
}

bool c_game_engine_miscellaneous_options::get_teams_enabled() const
{
	return m_flags.test(_game_engine_miscellaneous_option_teams_enabled);
}

void c_game_engine_miscellaneous_options::set_teams_enabled(bool teams_enabled)
{
	return m_flags.set(_game_engine_miscellaneous_option_teams_enabled, teams_enabled);
}

bool c_game_engine_miscellaneous_options::get_round_reset_players() const
{
	return m_flags.test(_game_engine_miscellaneous_option_round_reset_players);
}

void c_game_engine_miscellaneous_options::set_round_reset_players(bool round_reset_players)
{
	return m_flags.set(_game_engine_miscellaneous_option_round_reset_players, round_reset_players);
}

bool c_game_engine_miscellaneous_options::get_round_reset_map() const
{
	return m_flags.test(_game_engine_miscellaneous_option_round_reset_map);
}

void c_game_engine_miscellaneous_options::set_round_reset_map(bool round_reset_map)
{
	return m_flags.set(_game_engine_miscellaneous_option_round_reset_map, round_reset_map);
}

byte c_game_engine_miscellaneous_options::get_round_time_limit_minutes() const
{
	return m_round_time_limit_minutes;
}

void c_game_engine_miscellaneous_options::set_round_time_limit_minutes(long round_time_limit_minutes)
{
	m_round_time_limit_minutes = round_time_limit_minutes;
}

long c_game_engine_miscellaneous_options::get_round_time_limit_seconds() const
{
	return 60 * get_round_time_limit_minutes();
}

void c_game_engine_miscellaneous_options::set_round_time_limit_seconds(long round_time_limit_seconds)
{
	set_round_time_limit_minutes(round_time_limit_seconds / 60);
}

byte c_game_engine_miscellaneous_options::get_round_limit() const
{
	return m_round_limit;
}

void c_game_engine_miscellaneous_options::set_round_limit(long round_limit)
{
	m_round_limit = round_limit;
}

byte c_game_engine_miscellaneous_options::get_early_victory_win_count() const
{
	return m_early_victory_win_count;
}

void c_game_engine_miscellaneous_options::set_early_victory_win_count(long early_victory_win_count)
{
	m_early_victory_win_count = early_victory_win_count;
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

