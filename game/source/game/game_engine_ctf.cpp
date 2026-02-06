#include "game/game_engine_ctf.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/bitstream.hpp"
#include "memory/byte_swapping.hpp"

REFERENCE_DECLARE(0x050D96BC, c_game_engine_ctf_variant*, ctf_variant);

void c_game_engine_ctf_variant::byteswap()
{
	c_game_engine_base_variant::byteswap();

	bswap_uns16_inplace(m_touch_return_timeout);
	bswap_uns16_inplace(m_sudden_death_time);
	bswap_uns16_inplace(m_score_to_win);
	bswap_uns16_inplace(m_score_unknown);
	bswap_uns16_inplace(m_flag_reset_time);
	m_carrier_traits.byteswap();

	ASSERT(array_is_zeroed(m_pad1));
}

void c_game_engine_ctf_variant::set(const c_game_engine_ctf_variant* variant, bool force)
{
	ASSERT(variant != nullptr);

	c_game_engine_base_variant::set(variant, force);

	get_miscellaneous_options_writeable()->set_teams_enabled(true);
	set_home_flag_waypoint(variant->get_home_flag_waypoint());
	set_game_type(variant->get_game_type());
	set_respawn(variant->get_respawn());
	set_score_to_win(variant->get_score_to_win());
	set_sudden_death_time(variant->get_sudden_death_time());
	set_flag_reset_time(variant->get_flag_reset_time());
	set_touch_return_timeout(variant->get_touch_return_timeout());
	set_flag_at_home_to_score(variant->get_flag_at_home_to_score());
	set_carrier_traits(variant->get_carrier_traits(), force);

	zero_array(m_pad1);
}

//void c_game_engine_ctf_variant::set(const s_game_engine_ctf_variant_definition* definition, bool force)
//{
//}

void c_game_engine_ctf_variant::encode_to_mcc(c_bitstream* packet) const
{
	c_game_engine_base_variant::encode_to_mcc(packet);

	bool flag_at_home_to_score = get_flag_at_home_to_score();
	e_ctf_home_flag_waypoint_settings home_flag_waypoint = get_home_flag_waypoint();
	e_ctf_game_type_settings game_type = get_game_type();
	e_ctf_respawn_settings respawn = get_respawn();
	int16 score_to_win = get_score_to_win();
	int16 sudden_death_time = get_sudden_death_time();
	int16 flag_reset_time = get_flag_reset_time();
	int16 touch_return_timeout = get_touch_return_timeout();

	packet->write_bool("ctf-flag-at-home-to-score", flag_at_home_to_score);
	packet->write_integer("ctf-home-flag-waypoint", home_flag_waypoint, 2);
	packet->write_integer("ctf-game-type", game_type, 2);
	packet->write_integer("ctf-respawn", respawn, 2);
	packet->write_integer("ctf-score-to-win", score_to_win, 6);
	packet->write_signed_integer("ctf-sudden-death-time", sudden_death_time, 9);
	packet->write_integer("ctf-flag-reset-time", flag_reset_time, 9);
	packet->write_signed_integer("ctf-touch-return-time", touch_return_timeout, 6);
	get_carrier_traits()->encode_to_mcc(packet);
}

void c_game_engine_ctf_variant::decode_from_mcc(c_bitstream* packet)
{
	c_game_engine_base_variant::decode_from_mcc(packet);

	bool flag_at_home_to_score = packet->read_bool("ctf-flag-at-home-to-score");
	e_ctf_home_flag_waypoint_settings home_flag_waypoint = packet->read_enum<e_ctf_home_flag_waypoint_settings, 2>("ctf-home-flag-waypoint");
	e_ctf_game_type_settings game_type = packet->read_enum<e_ctf_game_type_settings, 2>("ctf-game-type");
	e_ctf_respawn_settings respawn = packet->read_enum<e_ctf_respawn_settings, 2>("ctf-respawn");
	int16 score_to_win = static_cast<int16>(packet->read_integer("ctf-score-to-win", 6));
	int16 sudden_death_time = static_cast<int16>(packet->read_signed_integer("ctf-sudden-death-time", 9));
	int16 flag_reset_time = static_cast<int16>(packet->read_integer("ctf-flag-reset-time", 9));
	int16 touch_return_timeout = static_cast<int16>(packet->read_signed_integer("ctf-touch-return-time", 6));
	get_carrier_traits_writeable()->decode_from_mcc(packet);

	set_flag_at_home_to_score(flag_at_home_to_score);
	set_home_flag_waypoint(home_flag_waypoint);
	set_game_type(game_type);
	set_respawn(respawn);
	set_score_to_win(score_to_win);
	set_sudden_death_time(sudden_death_time);
	set_flag_reset_time(flag_reset_time);
	set_touch_return_timeout(touch_return_timeout);
}

bool c_game_engine_ctf_variant::get_flag_at_home_to_score() const
{
	return m_variant_flags.test(_ctf_variant_flags_flag_at_home_to_score_flag);
}

void c_game_engine_ctf_variant::set_flag_at_home_to_score(bool flag_at_home_to_score)
{
	m_variant_flags.set(_ctf_variant_flags_flag_at_home_to_score_flag, flag_at_home_to_score);
}

e_ctf_home_flag_waypoint_settings c_game_engine_ctf_variant::get_home_flag_waypoint() const
{
	return m_home_flag_waypoint;
}

void c_game_engine_ctf_variant::set_home_flag_waypoint(e_ctf_home_flag_waypoint_settings home_flag_waypoint)
{
	if (!VALID_INDEX(home_flag_waypoint, k_ctf_home_flag_waypoint_settings))
	{
		event(_event_warning, "game_engine:ctf: invalid home_flag_waypoint setting '%d' outside range '[0, %d)'!", home_flag_waypoint, k_ctf_home_flag_waypoint_settings);

		m_home_flag_waypoint = k_ctf_home_flag_waypoint_settings_default;
	}
	else
	{
		m_home_flag_waypoint = home_flag_waypoint;
	}
}

e_ctf_game_type_settings c_game_engine_ctf_variant::get_game_type() const
{
	return m_game_type;
}

void c_game_engine_ctf_variant::set_game_type(e_ctf_game_type_settings game_type)
{
	if (!VALID_INDEX(m_game_type, k_ctf_game_type_settings))
	{
		event(_event_warning, "game_engine:ctf: invalid game_type setting '%d' outside range '[0, %d)'!", game_type, k_ctf_game_type_settings);

		m_game_type = k_ctf_game_type_settings_default;
	}
	else
	{
		m_game_type = game_type;
	}
}

e_ctf_respawn_settings c_game_engine_ctf_variant::get_respawn() const
{
	return m_respawn;
}

void c_game_engine_ctf_variant::set_respawn(e_ctf_respawn_settings respawn)
{
	if (!VALID_INDEX(respawn, k_ctf_respawn_settings))
	{
		event(_event_warning, "game_engine:ctf: invalid respawn setting '%d' outside range '[0, %d)'!", respawn, k_ctf_respawn_settings);

		m_respawn = k_ctf_respawn_settings_default;
	}
	else
	{
		m_respawn = respawn;
	}
}

int16 c_game_engine_ctf_variant::get_touch_return_timeout() const
{
	return m_touch_return_timeout;
}

void c_game_engine_ctf_variant::set_touch_return_timeout(int16 touch_return_timeout)
{
	m_touch_return_timeout = touch_return_timeout;
}

int16 c_game_engine_ctf_variant::get_sudden_death_time() const
{
	return m_sudden_death_time;
}

void c_game_engine_ctf_variant::set_sudden_death_time(int16 sudden_death_time)
{
	if (!VALID_INDEX(sudden_death_time + 1, k_ctf_sudden_death_times))
	{
		event(_event_warning, "game_engine:ctf: invalid sudden_death_time setting '%d' outside range '[%d, %d]'!", sudden_death_time, _ctf_sudden_death_time_infinite, _ctf_sudden_death_time_5_minute);

		m_sudden_death_time = k_ctf_sudden_death_times_default;
	}
	else
	{
		m_sudden_death_time = sudden_death_time;
	}
}

int16 c_game_engine_ctf_variant::get_score_to_win() const
{
	return m_score_to_win;
}

void c_game_engine_ctf_variant::set_score_to_win(int16 score_to_win)
{
	if (!VALID_INDEX(score_to_win, 50))
	{
		event(_event_warning, "game_engine:ctf: invalid score_to_win setting '%d' outside range '[%d, %d]'!", score_to_win, 0, 50);

		m_score_to_win = 5;
	}
	else
	{
		m_score_to_win = score_to_win;
	}
}

int16 c_game_engine_ctf_variant::get_flag_reset_time() const
{
	return m_flag_reset_time;
}

void c_game_engine_ctf_variant::set_flag_reset_time(int16 flag_reset_time)
{
	if (!VALID_INDEX(flag_reset_time, 300))
	{
		event(_event_warning, "game_engine:ctf: invalid flag_reset_time setting '%d' outside range '[%d, %d]'!", flag_reset_time, 0, 300);

		m_flag_reset_time = 30;
	}
	else
	{
		m_flag_reset_time = flag_reset_time;
	}
}

c_player_traits* c_game_engine_ctf_variant::get_carrier_traits_writeable()
{
	return &m_carrier_traits;
}

const c_player_traits* c_game_engine_ctf_variant::get_carrier_traits() const
{
	return &m_carrier_traits;
}

void c_game_engine_ctf_variant::set_carrier_traits(const c_player_traits* traits, bool force)
{
	m_carrier_traits.set(traits, force);
}

void c_ctf_engine::dump_settings(s_file_reference* file) const
{
	ASSERT(file != nullptr);

	c_game_engine::dump_settings(file);

	file_printf(file, "---------- CTF variant settings\r\n");

	file_printf(file, "score to win= %d\r\n", ctf_variant->get_score_to_win());
	
	file_printf(file, "home flag waypoint= ");
	switch (ctf_variant->get_home_flag_waypoint())
	{
	case _ctf_home_flag_waypoint_settings_never:
		file_printf(file, "NEVER\r\n");
		break;
	case _ctf_home_flag_waypoint_settings_when_not_carried_by_enemy:
		file_printf(file, "WHEN NOT CARRIED BY ENEMY\r\n");
		break;
	case _ctf_home_flag_waypoint_settings_away_from_home:
		file_printf(file, "AWAY FROM HOME\r\n");
		break;
	case _ctf_home_flag_waypoint_settings_always:
		file_printf(file, "ALWAYS\r\n");
		break;
	default:
		file_printf(file, "<current option setting not exported!>\r\n");
		break;
	}
	
	file_printf(file, "game type= ");
	switch (ctf_variant->get_game_type())
	{
	case _ctf_game_type_settings_multi_flag:
		file_printf(file, "MULTIFLAG\r\n");
		break;
	case _ctf_game_type_settings_single_flag:
		file_printf(file, "SINGLE FLAG\r\n");
		break;
	case _ctf_game_type_settings_neutral_flag:
		file_printf(file, "NEUTRAL FLAG\r\n");
		break;
	default:
		file_printf(file, "<current option setting not exported!>\r\n");
		break;
	}
	
	file_printf(file, "respawn= ");
	switch (ctf_variant->get_respawn())
	{
	case _ctf_respawn_settings_normal:
		file_printf(file, "NORMAL\r\n");
		break;
	case _ctf_respawn_settings_on_friendly_capture:
		file_printf(file, "ON FRIENDLY CAPTURE\r\n");
		break;
	case _ctf_respawn_settings_on_enemy_capture:
		file_printf(file, "ON ENEMY CAPTURE\r\n");
		break;
	case _ctf_respawn_settings_on_any_capture:
		file_printf(file, "ON ANY CAPTURE\r\n");
		break;
	default:
		file_printf(file, "<current option setting not exported!>\r\n");
		break;
	}
	
	//file_printf(file, "sudden death time (negative is infinite)= %d sec\r\n", ctf_variant->get_sudden_death_time());
	file_printf(file, "sudden death time= ");
	switch (ctf_variant->get_sudden_death_time())
	{
	case _ctf_sudden_death_time_infinite:
		file_printf(file, "INFINITE\r\n");
		break;
	case _ctf_sudden_death_time_off:
		file_printf(file, "OFF\r\n");
		break;
	default:
		file_printf(file, "%d SECONDS\r\n", ctf_variant->get_sudden_death_time());
		break;
	}

	//file_printf(file, "touch return timeout (negative is off)= %d sec\r\n", ctf_variant->get_touch_return_timeout());
	file_printf(file, "touch return timeout= ");
	switch (ctf_variant->get_touch_return_timeout())
	{
	case _ctf_touch_return_settings_off:
		file_printf(file, "OFF\r\n");
		break;
	case _ctf_touch_return_settings_instant:
		file_printf(file, "INSTANT\r\n");
		break;
	default:
		file_printf(file, "%d SECONDS\r\n", ctf_variant->get_touch_return_timeout());
		break;
	}

	file_printf(file, "flag reset time= %d sec\r\n", ctf_variant->get_flag_reset_time());
	file_printf(file, "flag %s be at home to score\r\n", ctf_variant->get_flag_at_home_to_score() ? "SHOULD" : "DOESN'T HAVE TO");
	dump_player_trait_settings("flag carrier traits", ctf_variant->get_carrier_traits(), file);

	file_printf(file, "---------- END CTF variant settings\r\n");
}

