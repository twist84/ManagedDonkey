#include "game/game_engine_traits.hpp"

#include "cseries/cseries_events.hpp"
#include "game/multiplayer_definitions.hpp"
#include "main/main_game.hpp"
#include "memory/bitstream.hpp"
#include "memory/byte_swapping.hpp"

void c_game_engine_miscellaneous_options::byteswap()
{
}

void c_game_engine_miscellaneous_options::set(const c_game_engine_miscellaneous_options* options, bool force)
{
	ASSERT(options != nullptr);

	set_teams_enabled(options->get_teams_enabled());
	set_round_reset_players(options->get_round_reset_players());
	set_round_reset_map(options->get_round_reset_map());
	set_round_time_limit_minutes(options->get_round_time_limit_minutes());
	set_round_limit(options->get_round_limit());
	set_early_victory_win_count(options->get_early_victory_win_count());
}

//void c_game_engine_miscellaneous_options::set(const s_game_engine_miscellaneous_options_definition* definition, bool force)
//{
//}

void c_game_engine_miscellaneous_options::encode_to_mcc(c_bitstream* packet) const
{
	bool teams_enabled = get_teams_enabled();
	bool round_reset_players = get_round_reset_players();
	bool round_reset_map = get_round_reset_map();
	int32 round_time_limit_minutes = get_round_time_limit_minutes();
	int32 round_limit = get_round_limit();
	int32 early_victory_win_count = get_early_victory_win_count();

	packet->write_bool("miscellaneous-options-teams", get_teams_enabled());
	packet->write_bool("miscellaneous-options-round-reset-players", get_round_reset_players());
	packet->write_bool("miscellaneous-options-round-reset-map", get_round_reset_map());
	packet->write_integer("miscellaneous-options-round-time-limit-minutes", get_round_time_limit_minutes(), 8);
	packet->write_integer("miscellaneous-options-round-limit", get_round_limit(), 4);
	packet->write_integer("miscellaneous-options-early-victory-win-count", get_early_victory_win_count(), 4);
}

void c_game_engine_miscellaneous_options::decode_from_mcc(c_bitstream* packet)
{
	bool teams_enabled = packet->read_bool("miscellaneous-options-teams");
	bool round_reset_players = packet->read_bool("miscellaneous-options-round-reset-players");
	bool round_reset_map = packet->read_bool("miscellaneous-options-round-reset-map");
	int32 round_time_limit_minutes = static_cast<int32>(packet->read_integer("miscellaneous-options-round-time-limit-minutes", 8));
	int32 round_limit = static_cast<int32>(packet->read_integer("miscellaneous-options-round-limit", 4));
	int32 early_victory_win_count = static_cast<int32>(packet->read_integer("miscellaneous-options-early-victory-win-count", 4));

	set_teams_enabled(teams_enabled);
	set_round_reset_players(round_reset_players);
	set_round_reset_map(round_reset_map);
	set_round_time_limit_minutes(round_time_limit_minutes);
	set_round_limit(round_limit);
	set_early_victory_win_count(early_victory_win_count);
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

bool c_game_engine_miscellaneous_options::get_director_allow_player_switching() const
{
	return m_flags.test(_game_engine_miscellaneous_option_director_allow_player_switching);
}

void c_game_engine_miscellaneous_options::set_director_allow_player_switching(bool director_allow_player_switching)
{
	return m_flags.set(_game_engine_miscellaneous_option_director_allow_player_switching, director_allow_player_switching);
}

uns8 c_game_engine_miscellaneous_options::get_round_time_limit_minutes() const
{
	return m_round_time_limit_minutes;
}

void c_game_engine_miscellaneous_options::set_round_time_limit_minutes(int32 round_time_limit_minutes)
{
	m_round_time_limit_minutes = round_time_limit_minutes;
}

int32 c_game_engine_miscellaneous_options::get_round_time_limit_seconds() const
{
	return 60 * get_round_time_limit_minutes();
}

void c_game_engine_miscellaneous_options::set_round_time_limit_seconds(int32 round_time_limit_seconds)
{
	set_round_time_limit_minutes(round_time_limit_seconds / 60);
}

uns8 c_game_engine_miscellaneous_options::get_round_limit() const
{
	return m_round_limit;
}

void c_game_engine_miscellaneous_options::set_round_limit(int32 round_limit)
{
	m_round_limit = round_limit;
}

uns8 c_game_engine_miscellaneous_options::get_early_victory_win_count() const
{
	return m_early_victory_win_count;
}

void c_game_engine_miscellaneous_options::set_early_victory_win_count(int32 early_victory_win_count)
{
	m_early_victory_win_count = early_victory_win_count;
}

void c_game_engine_respawn_options::byteswap()
{
	m_respawn_player_traits.byteswap();
}

void c_game_engine_respawn_options::set(const c_game_engine_respawn_options* options, bool force)
{
	ASSERT(options != nullptr);

	set_inherit_respawn_time_enabled(options->get_inherit_respawn_time_enabled());
	set_respawn_with_teammate_enabled(options->get_respawn_with_teammate_enabled());
	set_respawn_at_location_enabled(options->get_respawn_at_location_enabled());
	set_respawn_on_kills_enabled(options->get_respawn_on_kills_enabled());
	set_lives_per_round(options->get_lives_per_round());
	set_team_lives_per_round(options->get_team_lives_per_round());
	set_respawn_time_seconds(options->get_respawn_time_seconds());
	set_suicide_penalty_seconds(options->get_suicide_penalty_seconds());
	set_betrayal_penalty_seconds(options->get_betrayal_penalty_seconds());
	set_respawn_growth_seconds(options->get_respawn_growth_seconds());
	set_respawn_player_traits_duration_seconds(options->get_respawn_player_traits_duration_seconds());
	set_respawn_player_traits(options->get_respawn_player_traits(), force);
}

//void c_game_engine_respawn_options::set(const s_game_engine_respawn_options_definition* definition, bool force)
//{
//}

void c_game_engine_respawn_options::encode_to_mcc(c_bitstream* packet) const
{
	bool inherit_respawn_time_enabled = get_inherit_respawn_time_enabled();
	bool respawn_with_teammate_enabled = get_respawn_with_teammate_enabled();
	bool respawn_at_location_enabled = get_respawn_at_location_enabled();
	bool respawn_on_kills_enabled = get_respawn_on_kills_enabled();
	uns8 lives_per_round = get_lives_per_round();
	uns8 team_lives_per_round = get_team_lives_per_round();
	uns8 respawn_time_seconds = get_respawn_time_seconds();
	uns8 suicide_penalty_seconds = get_suicide_penalty_seconds();
	uns8 betrayal_penalty_seconds = get_betrayal_penalty_seconds();
	uns8 respawn_growth_seconds = get_respawn_growth_seconds();
	uns8 respawn_player_traits_duration_seconds = get_respawn_player_traits_duration_seconds();

	packet->write_bool("respawn-options-inherit-respawn-time", inherit_respawn_time_enabled);
	packet->write_bool("respawn-options-respawn-with-teammate", respawn_with_teammate_enabled);
	packet->write_bool("respawn-options-respawn-at-location", respawn_at_location_enabled);
	packet->write_bool("respawn-options-respawn-on-kills", respawn_on_kills_enabled);
	packet->write_integer("respawn-options-lives-per-round", lives_per_round, 6);
	packet->write_integer("respawn-options-team-lives-per-round", team_lives_per_round, 7);
	packet->write_integer("respawn-options-respawn-time", respawn_time_seconds, 8);
	packet->write_integer("respawn-options-suicide-time", suicide_penalty_seconds, 8);
	packet->write_integer("respawn-options-betrayal-time", betrayal_penalty_seconds, 8);
	packet->write_integer("respawn-options-respawn-growth-time", respawn_growth_seconds, 4);
	packet->write_integer("respawn-options-player-traits-duration", respawn_player_traits_duration_seconds, 6);
	get_respawn_player_traits()->encode_to_mcc(packet);
}

void c_game_engine_respawn_options::decode_from_mcc(c_bitstream* packet)
{
	bool inherit_respawn_time_enabled = packet->read_bool("respawn-options-inherit-respawn-time");
	bool respawn_with_teammate_enabled = packet->read_bool("respawn-options-respawn-with-teammate");
	bool respawn_at_location_enabled = packet->read_bool("respawn-options-respawn-at-location");
	bool respawn_on_kills_enabled = packet->read_bool("respawn-options-respawn-on-kills");
	uns8 lives_per_round = static_cast<uns8>(packet->read_integer("respawn-options-lives-per-round", 6));
	uns8 team_lives_per_round = static_cast<uns8>(packet->read_integer("respawn-options-team-lives-per-round", 7));
	uns8 respawn_time_seconds = static_cast<uns8>(packet->read_integer("respawn-options-respawn-time", 8));
	uns8 suicide_penalty_seconds = static_cast<uns8>(packet->read_integer("respawn-options-suicide-time", 8));
	uns8 betrayal_penalty_seconds = static_cast<uns8>(packet->read_integer("respawn-options-betrayal-time", 8));
	uns8 respawn_growth_seconds = static_cast<uns8>(packet->read_integer("respawn-options-respawn-growth-time", 4));
	uns8 respawn_player_traits_duration_seconds = static_cast<uns8>(packet->read_integer("respawn-options-player-traits-duration", 6));
	get_respawn_player_traits_writeable()->decode_from_mcc(packet);

	set_inherit_respawn_time_enabled(inherit_respawn_time_enabled);
	set_respawn_with_teammate_enabled(respawn_with_teammate_enabled);
	set_respawn_at_location_enabled(respawn_at_location_enabled);
	set_respawn_on_kills_enabled(respawn_on_kills_enabled);
	set_lives_per_round(lives_per_round);
	set_team_lives_per_round(team_lives_per_round);
	set_respawn_time_seconds(respawn_time_seconds);
	set_suicide_penalty_seconds(suicide_penalty_seconds);
	set_betrayal_penalty_seconds(betrayal_penalty_seconds);
	set_respawn_growth_seconds(respawn_growth_seconds);
	set_respawn_player_traits_duration_seconds(respawn_player_traits_duration_seconds);
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

uns8 c_game_engine_respawn_options::get_lives_per_round() const
{
	return m_lives_per_round;
}

void c_game_engine_respawn_options::set_lives_per_round(int32 lives_per_round)
{
	m_lives_per_round = lives_per_round;
}

uns8 c_game_engine_respawn_options::get_team_lives_per_round() const
{
	return m_team_lives_per_round;
}

void c_game_engine_respawn_options::set_team_lives_per_round(int32 team_lives_per_round)
{
	m_team_lives_per_round = team_lives_per_round;
}

uns8 c_game_engine_respawn_options::get_respawn_time_seconds() const
{
	return m_respawn_time_seconds;
}

void c_game_engine_respawn_options::set_respawn_time_seconds(int32 respawn_time_seconds)
{
	m_respawn_time_seconds = respawn_time_seconds;
}

uns8 c_game_engine_respawn_options::get_suicide_penalty_seconds() const
{
	return m_suicide_penalty_seconds;
}

void c_game_engine_respawn_options::set_suicide_penalty_seconds(int32 suicide_penalty_seconds)
{
	m_suicide_penalty_seconds = suicide_penalty_seconds;
}

uns8 c_game_engine_respawn_options::get_betrayal_penalty_seconds() const
{
	return m_betrayal_penalty_seconds;
}

void c_game_engine_respawn_options::set_betrayal_penalty_seconds(int32 betrayal_penalty_seconds)
{
	m_betrayal_penalty_seconds = betrayal_penalty_seconds;
}

uns8 c_game_engine_respawn_options::get_unknown_penalty_seconds() const
{
	return m_unknown_penalty_seconds;
}

void c_game_engine_respawn_options::set_unknown_penalty_seconds(int32 unknown_penalty_seconds)
{
	m_unknown_penalty_seconds = unknown_penalty_seconds;
}

uns8 c_game_engine_respawn_options::get_respawn_growth_seconds() const
{
	return m_respawn_growth_seconds;
}

void c_game_engine_respawn_options::set_respawn_growth_seconds(int32 respawn_growth_seconds)
{
	m_respawn_growth_seconds = respawn_growth_seconds;
}

uns8 c_game_engine_respawn_options::get_respawn_player_traits_duration_seconds() const
{
	return m_respawn_player_traits_duration_seconds;
}

void c_game_engine_respawn_options::set_respawn_player_traits_duration_seconds(int32 respawn_player_traits_duration_seconds)
{
	m_respawn_player_traits_duration_seconds = respawn_player_traits_duration_seconds;
}

const c_player_traits* c_game_engine_respawn_options::get_respawn_player_traits() const
{
	return &m_respawn_player_traits;
}

c_player_traits* c_game_engine_respawn_options::get_respawn_player_traits_writeable()
{
	return &m_respawn_player_traits;
}

void c_game_engine_respawn_options::set_respawn_player_traits(const c_player_traits* traits, bool force)
{
	m_respawn_player_traits.set(traits, force);
}

void c_game_engine_social_options::byteswap()
{
	bswap_uns16_inplace(m_flags);
	bswap_uns16_inplace(m_team_changing);
}

void c_game_engine_social_options::set(const c_game_engine_social_options* options, bool force)
{
	ASSERT(options != nullptr);

	set_observers_enabled(options->get_observers_enabled());
	set_team_changing_enabled(options->get_team_changing_enabled());
	set_team_changing_balancing_only_enabled(options->get_team_changing_balancing_only_enabled());
	set_friendly_fire_enabled(options->get_friendly_fire_enabled());
	set_betrayal_booting_enabled(options->get_betrayal_booting_enabled());
	set_enemy_voice_enabled(options->get_enemy_voice_enabled());
	set_open_channel_voice_enabled(options->get_open_channel_voice_enabled());
	set_dead_player_voice_enabled(options->get_dead_player_voice_enabled());
}

//void c_game_engine_social_options::set(const s_game_engine_social_options_definition* definition, bool force)
//{
//}

void c_game_engine_social_options::encode_to_mcc(c_bitstream* packet) const
{
	bool observers_enabled = get_observers_enabled();
	int32 team_changing_setting = get_team_changing_setting();
	bool friendly_fire_enabled = get_friendly_fire_enabled();
	bool betrayal_booting_enabled = get_betrayal_booting_enabled();
	bool enemy_voice_enabled = get_enemy_voice_enabled();
	bool open_channel_voice_enabled = get_open_channel_voice_enabled();
	bool dead_player_voice_enabled = get_dead_player_voice_enabled();

	packet->write_bool("social-options-observers", observers_enabled);
	packet->write_integer("social-options-team-changing", team_changing_setting, 2);
	packet->write_bool("social-options-friendly-fire", friendly_fire_enabled);
	packet->write_bool("social-options-betrayal-booting", betrayal_booting_enabled);
	packet->write_bool("social-options-enemy-voice", enemy_voice_enabled);
	packet->write_bool("social-options-open-channel-voice", open_channel_voice_enabled);
	packet->write_bool("social-options-dead-player-voice", dead_player_voice_enabled);
}

void c_game_engine_social_options::decode_from_mcc(c_bitstream* packet)
{
	bool observers_enabled = packet->read_bool("social-options-observers");
	int32 team_changing_setting = packet->read_integer("social-options-team-changing", 2);
	bool friendly_fire_enabled = packet->read_bool("social-options-friendly-fire");
	bool betrayal_booting_enabled = packet->read_bool("social-options-betrayal-booting");
	bool enemy_voice_enabled = packet->read_bool("social-options-enemy-voice");
	bool open_channel_voice_enabled = packet->read_bool("social-options-open-channel-voice");
	bool dead_player_voice_enabled = packet->read_bool("social-options-dead-player-voice");

	set_observers_enabled(observers_enabled);
	set_team_changing_setting(team_changing_setting);
	set_friendly_fire_enabled(friendly_fire_enabled);
	set_betrayal_booting_enabled(betrayal_booting_enabled);
	set_enemy_voice_enabled(enemy_voice_enabled);
	set_open_channel_voice_enabled(open_channel_voice_enabled);
	set_dead_player_voice_enabled(dead_player_voice_enabled);
}

bool c_game_engine_social_options::get_friendly_fire_enabled() const
{
	return m_flags.test(_game_engine_social_options_friendly_fire_enabled);
}

void c_game_engine_social_options::set_friendly_fire_enabled(bool friendly_fire_enabled)
{
	m_flags.set(_game_engine_social_options_friendly_fire_enabled, friendly_fire_enabled);
}

bool c_game_engine_social_options::get_betrayal_booting_enabled() const
{
	return m_flags.test(_game_engine_social_options_betrayal_booting_enabled);
}

void c_game_engine_social_options::set_betrayal_booting_enabled(bool betrayal_booting_enabled)
{
	m_flags.set(_game_engine_social_options_betrayal_booting_enabled, betrayal_booting_enabled);
}

bool c_game_engine_social_options::get_enemy_voice_enabled() const
{
	return m_flags.test(_game_engine_social_options_enemy_voice_enabled);
}

void c_game_engine_social_options::set_enemy_voice_enabled(bool enemy_voice_enabled)
{
	m_flags.set(_game_engine_social_options_enemy_voice_enabled, enemy_voice_enabled);
}

bool c_game_engine_social_options::get_open_channel_voice_enabled() const
{
	return m_flags.test(_game_engine_social_options_open_channel_voice_enabled);
}

void c_game_engine_social_options::set_open_channel_voice_enabled(bool open_channel_voice_enabled)
{
	m_flags.set(_game_engine_social_options_open_channel_voice_enabled, open_channel_voice_enabled);
}

bool c_game_engine_social_options::get_dead_player_voice_enabled() const
{
	return m_flags.test(_game_engine_social_options_dead_player_voice_enabled);
}

void c_game_engine_social_options::set_dead_player_voice_enabled(bool dead_player_voice_enabled)
{
	m_flags.set(_game_engine_social_options_dead_player_voice_enabled, dead_player_voice_enabled);
}

bool c_game_engine_social_options::get_spartans_vs_elites_enabled() const
{
	return m_flags.test(_game_engine_social_options_spartans_vs_elites_enabled);
}

void c_game_engine_social_options::set_spartans_vs_elites_enabled(bool spartans_vs_elites_enabled)
{
	m_flags.set(_game_engine_social_options_spartans_vs_elites_enabled, spartans_vs_elites_enabled);
}

bool c_game_engine_social_options::get_observers_enabled() const
{
	return m_flags.test(_game_engine_social_options_observers_enabled);
}

void c_game_engine_social_options::set_observers_enabled(bool observers_enabled)
{
	m_flags.set(_game_engine_social_options_observers_enabled, observers_enabled);
}

bool c_game_engine_social_options::get_team_changing_enabled() const
{
	return m_flags.test(_game_engine_social_options_team_changing_enabled);
}

void c_game_engine_social_options::set_team_changing_enabled(bool team_changing_enabled)
{
	m_flags.set(_game_engine_social_options_team_changing_enabled, team_changing_enabled);
}

bool c_game_engine_social_options::get_team_changing_balancing_only_enabled() const
{
	return m_flags.test(_game_engine_social_options_team_changing_balancing_only_enabled);
}

void c_game_engine_social_options::set_team_changing_balancing_only_enabled(bool team_changing_balancing_only_enabled)
{
	m_flags.set(_game_engine_social_options_team_changing_balancing_only_enabled, team_changing_balancing_only_enabled);
}

//e_team_changing_type c_game_engine_social_options::get_team_changing_setting() const
int32 c_game_engine_social_options::get_team_changing_setting() const
{
	return m_team_changing;
}

//void c_game_engine_social_options::set_team_changing_setting(e_team_changing_type team_changing)
void c_game_engine_social_options::set_team_changing_setting(int32 team_changing)
{
	if (!VALID_INDEX(team_changing, 2))
	{
		event(_event_warning, "game_engine:social_option:team_changing: invalid team changing setting '%d'!", team_changing);
	}
	else
	{
		m_team_changing = team_changing;
	}
}

void c_game_engine_map_override_options::byteswap()
{
	bswap_uns32_inplace(m_flags);
	m_base_player_traits.byteswap();
	bswap_uns16_inplace(m_weapon_set_absolute_index);
	bswap_uns16_inplace(m_vehicle_set_absolute_index);
	m_red_powerup_traits.byteswap();
	m_blue_powerup_traits.byteswap();
	m_yellow_powerup_traits.byteswap();
}

void c_game_engine_map_override_options::set(const c_game_engine_map_override_options* options, bool force)
{
	ASSERT(options != nullptr);

	set_grenades_on_map_enabled(get_grenades_on_map_enabled());
	set_indestructible_vehicles_enabled(get_indestructible_vehicles_enabled());
	set_base_player_traits(get_base_player_traits(), force);
	set_weapon_set_absolute_index(get_weapon_set_absolute_index());
	set_vehicle_set_absolute_index(get_vehicle_set_absolute_index());
	set_red_powerup_traits(get_red_powerup_traits(), force);
	set_blue_powerup_traits(get_blue_powerup_traits(), force);
	set_yellow_powerup_traits(get_yellow_powerup_traits(), force);
	set_red_powerup_duration_seconds(get_red_powerup_duration_seconds());
	set_blue_powerup_duration_seconds(get_blue_powerup_duration_seconds());
	set_yellow_powerup_duration_seconds(get_yellow_powerup_duration_seconds());

	zero_array(pad);
}

//void c_game_engine_map_override_options::set(const s_game_engine_map_override_options_definition* definition, bool force)
//{
//}

void c_game_engine_map_override_options::encode_to_mcc(c_bitstream* packet) const
{
	bool grenades_on_map_enabled = get_grenades_on_map_enabled();
	bool indestructible_vehicles_enabled = get_indestructible_vehicles_enabled();
	int16 weapon_set_absolute_index = get_weapon_set_absolute_index();
	int16 vehicle_set_absolute_index = get_vehicle_set_absolute_index();
	uns8 red_powerup_duration_seconds = get_red_powerup_duration_seconds();
	uns8 blue_powerup_duration_seconds = get_blue_powerup_duration_seconds();
	uns8 yellow_powerup_duration_seconds = get_yellow_powerup_duration_seconds();

	packet->write_bool("map-override-grenades-on-map", grenades_on_map_enabled);
	packet->write_bool("map-override-indestructible-vehicles", indestructible_vehicles_enabled);
	get_base_player_traits()->encode_to_mcc(packet);
	packet->write_signed_integer("map-override-weapon-set",  weapon_set_absolute_index, 8);
	packet->write_signed_integer("map-override-vehicle-set", vehicle_set_absolute_index, 8);
	get_red_powerup_traits()->encode_to_mcc(packet);
	get_blue_powerup_traits()->encode_to_mcc(packet);
	get_yellow_powerup_traits()->encode_to_mcc(packet);
	packet->write_integer("map-override-red-powerup-duration", red_powerup_duration_seconds, 7);
	packet->write_integer("map-override-blue-powerup-duration", blue_powerup_duration_seconds, 7);
	packet->write_integer("map-override-yellow-powerup-duration", yellow_powerup_duration_seconds, 7);
}

void c_game_engine_map_override_options::decode_from_mcc(c_bitstream* packet)
{
	bool grenades_on_map_enabled = packet->read_bool("map-override-grenades-on-map");
	bool indestructible_vehicles_enabled = packet->read_bool("map-override-indestructible-vehicles");
	get_base_player_traits_writeable()->decode_from_mcc(packet);
	int16 weapon_set_absolute_index = static_cast<int16>(packet->read_signed_integer("map-override-weapon-set", 8));
	int16 vehicle_set_absolute_index = static_cast<int16>(packet->read_signed_integer("map-override-vehicle-set", 8));
	get_red_powerup_traits_writeable()->decode_from_mcc(packet);
	get_blue_powerup_traits_writeable()->decode_from_mcc(packet);
	get_yellow_powerup_traits_writeable()->decode_from_mcc(packet);
	uns8 red_powerup_duration_seconds = static_cast<uns8>(packet->read_integer("map-override-red-powerup-duration", 7));
	uns8 blue_powerup_duration_seconds = static_cast<uns8>(packet->read_integer("map-override-blue-powerup-duration", 7));
	uns8 yellow_powerup_duration_seconds = static_cast<uns8>(packet->read_integer("map-override-yellow-powerup-duration", 7));

	set_grenades_on_map_enabled(grenades_on_map_enabled);
	set_indestructible_vehicles_enabled(indestructible_vehicles_enabled);
	set_weapon_set_absolute_index(weapon_set_absolute_index);
	set_vehicle_set_absolute_index(vehicle_set_absolute_index);
	set_red_powerup_duration_seconds(red_powerup_duration_seconds);
	set_blue_powerup_duration_seconds(blue_powerup_duration_seconds);
	set_yellow_powerup_duration_seconds(yellow_powerup_duration_seconds);
}

bool c_game_engine_map_override_options::get_grenades_on_map_enabled() const
{
	return m_flags.test(_game_engine_map_override_option_grenades_on_map_enabled);
}

void c_game_engine_map_override_options::set_grenades_on_map_enabled(bool grenades_on_map_enabled)
{
	m_flags.set(_game_engine_map_override_option_grenades_on_map_enabled, grenades_on_map_enabled);
}

bool c_game_engine_map_override_options::get_indestructible_vehicles_enabled() const
{
	return m_flags.test(_game_engine_map_override_option_indestructible_vehicles_enabled);
}

void c_game_engine_map_override_options::set_indestructible_vehicles_enabled(bool indestructible_vehicles_enabled)
{
	m_flags.set(_game_engine_map_override_option_indestructible_vehicles_enabled, indestructible_vehicles_enabled);
}

c_player_traits* c_game_engine_map_override_options::get_base_player_traits_writeable()
{
	return &m_base_player_traits;
}

const c_player_traits* c_game_engine_map_override_options::get_base_player_traits() const
{
	return &m_base_player_traits;
}

void c_game_engine_map_override_options::set_base_player_traits(const c_player_traits* traits, bool force)
{
	m_base_player_traits.set(traits, force);
}

int16 c_game_engine_map_override_options::get_weapon_set_absolute_index() const
{
	return m_weapon_set_absolute_index;
}

void c_game_engine_map_override_options::set_weapon_set_absolute_index(int16 weapon_set_absolute_index)
{
	if (weapon_set_absolute_index == 0xFFFFi16 || weapon_set_absolute_index == 0xFFFEi16 || !main_game_loaded_map() || multiplayer_universal_data_weapon_set_try_and_get(weapon_set_absolute_index))
	{
		m_weapon_set_absolute_index = weapon_set_absolute_index;
	}
	else
	{
		event(_event_warning, "game_engine:map_option:weapon_set: invalid weapon set '%d'!", weapon_set_absolute_index);
		m_weapon_set_absolute_index = 0xFFFFi16;
	}
}

int16 c_game_engine_map_override_options::get_vehicle_set_absolute_index() const
{
	return m_vehicle_set_absolute_index;
}

void c_game_engine_map_override_options::set_vehicle_set_absolute_index(int16 vehicle_set_absolute_index)
{
	if (vehicle_set_absolute_index == 0xFFFFi16 || !main_game_loaded_map() || multiplayer_universal_data_vehicle_set_try_and_get(vehicle_set_absolute_index))
	{
		m_vehicle_set_absolute_index = vehicle_set_absolute_index;
	}
	else
	{
		event(_event_warning, "game_engine:map_option:vehicle_set: invalid vehicle set '%d'!", vehicle_set_absolute_index);

		//ms23: `m_weapon_set_absolute_index`
		m_vehicle_set_absolute_index = 0xFFFFi16;
	}
}

int32 c_game_engine_map_override_options::get_weapon_set() const
{
	return multiplayer_universal_data_get_weapon_set_name_from_absolute_index(get_weapon_set_absolute_index());
}

void c_game_engine_map_override_options::set_weapon_set(int32 weapon_set)
{
	set_weapon_set_absolute_index(multiplayer_universal_data_get_weapon_set_absolute_index_from_string_id(weapon_set));
}

int32 c_game_engine_map_override_options::get_vehicle_set() const
{
	return multiplayer_universal_data_get_vehicle_set_name_from_absolute_index(get_vehicle_set_absolute_index());
}

void c_game_engine_map_override_options::set_vehicle_set(int32 vehicle_set)
{
	set_vehicle_set_absolute_index(multiplayer_universal_data_get_vehicle_set_absolute_index_from_string_id(vehicle_set));
}

c_player_traits* c_game_engine_map_override_options::get_red_powerup_traits_writeable()
{
	return &m_red_powerup_traits;
}

const c_player_traits* c_game_engine_map_override_options::get_red_powerup_traits() const
{
	return &m_red_powerup_traits;
}

void c_game_engine_map_override_options::set_red_powerup_traits(const c_player_traits* traits, bool force)
{
	m_red_powerup_traits.set(traits, force);
}

c_player_traits* c_game_engine_map_override_options::get_blue_powerup_traits_writeable()
{
	return &m_blue_powerup_traits;
}

const c_player_traits* c_game_engine_map_override_options::get_blue_powerup_traits() const
{
	return &m_blue_powerup_traits;
}

void c_game_engine_map_override_options::set_blue_powerup_traits(const c_player_traits* traits, bool force)
{
	m_blue_powerup_traits.set(traits, force);
}

c_player_traits* c_game_engine_map_override_options::get_yellow_powerup_traits_writeable()
{
	return &m_yellow_powerup_traits;
}

const c_player_traits* c_game_engine_map_override_options::get_yellow_powerup_traits() const
{
	return &m_yellow_powerup_traits;
}

void c_game_engine_map_override_options::set_yellow_powerup_traits(const c_player_traits* traits, bool force)
{
	m_yellow_powerup_traits.set(traits, force);
}

uns8 c_game_engine_map_override_options::get_red_powerup_duration_seconds() const
{
	return m_red_powerup_duration_seconds;
}

void c_game_engine_map_override_options::set_red_powerup_duration_seconds(uns8 red_powerup_duration_seconds)
{
	m_red_powerup_duration_seconds = red_powerup_duration_seconds;
}

uns8 c_game_engine_map_override_options::get_blue_powerup_duration_seconds() const
{
	return m_blue_powerup_duration_seconds;
}

void c_game_engine_map_override_options::set_blue_powerup_duration_seconds(uns8 blue_powerup_duration_seconds)
{
	m_blue_powerup_duration_seconds = blue_powerup_duration_seconds;
}

uns8 c_game_engine_map_override_options::get_yellow_powerup_duration_seconds() const
{
	return m_yellow_powerup_duration_seconds;
}

void c_game_engine_map_override_options::set_yellow_powerup_duration_seconds(uns8 yellow_powerup_duration_seconds)
{
	m_yellow_powerup_duration_seconds = yellow_powerup_duration_seconds;
}

