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

bool c_game_engine_respawn_options::get_inherit_respawn_time_enabled() const
{
	return m_flags.test(_game_engine_respawn_options_inherit_respawn_time_enabled);
}

void c_game_engine_respawn_options::set_inherit_respawn_time_enabled(bool inherit_respawn_time_enabled)
{
	return m_flags.set(_game_engine_respawn_options_inherit_respawn_time_enabled, inherit_respawn_time_enabled);
}

bool c_game_engine_respawn_options::get_respawn_with_teammate_enabled() const
{
	return m_flags.test(_game_engine_respawn_options_respawn_with_teammate_enabled);
}

void c_game_engine_respawn_options::set_respawn_with_teammate_enabled(bool respawn_with_teammate_enabled)
{
	return m_flags.set(_game_engine_respawn_options_respawn_with_teammate_enabled, respawn_with_teammate_enabled);
}

bool c_game_engine_respawn_options::get_respawn_at_location_enabled() const
{
	return m_flags.test(_game_engine_respawn_options_respawn_at_location_enabled);
}

void c_game_engine_respawn_options::set_respawn_at_location_enabled(bool respawn_at_location_enabled)
{
	return m_flags.set(_game_engine_respawn_options_respawn_at_location_enabled, respawn_at_location_enabled);
}

bool c_game_engine_respawn_options::get_respawn_on_kills_enabled() const
{
	return m_flags.test(_game_engine_respawn_options_respawn_on_kills_enabled);
}

void c_game_engine_respawn_options::set_respawn_on_kills_enabled(bool respawn_on_kills_enabled)
{
	return m_flags.set(_game_engine_respawn_options_respawn_on_kills_enabled, respawn_on_kills_enabled);
}

bool c_game_engine_respawn_options::get_auto_respawn_disabled() const
{
	return m_flags.test(_game_engine_respawn_options_auto_respawn_disabled);
}

void c_game_engine_respawn_options::set_auto_respawn_disabled(bool auto_respawn_disabled)
{
	return m_flags.set(_game_engine_respawn_options_auto_respawn_disabled, auto_respawn_disabled);
}

byte c_game_engine_respawn_options::get_lives_per_round() const
{
	return m_lives_per_round;
}

void c_game_engine_respawn_options::set_lives_per_round(long lives_per_round)
{
	m_lives_per_round = lives_per_round;
}

byte c_game_engine_respawn_options::get_team_lives_per_round() const
{
	return m_team_lives_per_round;
}

void c_game_engine_respawn_options::set_team_lives_per_round(long team_lives_per_round)
{
	m_team_lives_per_round = team_lives_per_round;
}

byte c_game_engine_respawn_options::get_respawn_time_seconds() const
{
	return m_respawn_time_seconds;
}

void c_game_engine_respawn_options::set_respawn_time_seconds(long respawn_time_seconds)
{
	m_respawn_time_seconds = respawn_time_seconds;
}

byte c_game_engine_respawn_options::get_suicide_penalty_seconds() const
{
	return m_suicide_penalty_seconds;
}

void c_game_engine_respawn_options::set_suicide_penalty_seconds(long suicide_penalty_seconds)
{
	m_suicide_penalty_seconds = suicide_penalty_seconds;
}

byte c_game_engine_respawn_options::get_betrayal_penalty_seconds() const
{
	return m_betrayal_penalty_seconds;
}

void c_game_engine_respawn_options::set_betrayal_penalty_seconds(long betrayal_penalty_seconds)
{
	m_betrayal_penalty_seconds = betrayal_penalty_seconds;
}

byte c_game_engine_respawn_options::get_unknown_penalty_seconds() const
{
	return m_unknown_penalty_seconds;
}

void c_game_engine_respawn_options::set_unknown_penalty_seconds(long unknown_penalty_seconds)
{
	m_unknown_penalty_seconds = unknown_penalty_seconds;
}

byte c_game_engine_respawn_options::get_respawn_growth_seconds() const
{
	return m_respawn_growth_seconds;
}

void c_game_engine_respawn_options::set_respawn_growth_seconds(long respawn_growth_seconds)
{
	m_respawn_growth_seconds = respawn_growth_seconds;
}

byte c_game_engine_respawn_options::get_respawn_player_traits_duration_seconds() const
{
	return m_respawn_player_traits_duration_seconds;
}

void c_game_engine_respawn_options::set_respawn_player_traits_duration_seconds(long respawn_player_traits_duration_seconds)
{
	m_respawn_player_traits_duration_seconds = respawn_player_traits_duration_seconds;
}

c_player_traits const* c_game_engine_respawn_options::get_respawn_player_traits() const
{
	return &m_respawn_player_traits;
}

c_player_traits* c_game_engine_respawn_options::get_respawn_player_traits_writeable()
{
	return &m_respawn_player_traits;
}

//void c_game_engine_respawn_options::set_respawn_player_traits(c_player_traits const* respawn_player_traits, bool)
//{
//	m_respawn_player_traits = *respawn_player_traits;
//}

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

