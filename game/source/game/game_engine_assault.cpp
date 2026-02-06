#include "game/game_engine_assault.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/bitstream.hpp"
#include "memory/byte_swapping.hpp"

REFERENCE_DECLARE(0x0471AA14, c_game_engine_assault_variant*, assault_variant);

void c_game_engine_assault_variant::byteswap()
{
	c_game_engine_base_variant::byteswap();

	bswap_uns16_inplace(m_variant_flags);
	bswap_uns16_inplace(m_respawn);
	bswap_uns16_inplace(m_game_type);
	bswap_uns16_inplace(m_enemy_bomb_waypoint);
	bswap_uns16_inplace(m_score_to_win);
	bswap_uns16_inplace(m_score_unknown0);
	bswap_uns16_inplace(m_score_unknown1);
	bswap_uns16_inplace(m_score_unknown2);
	bswap_uns16_inplace(m_score_unknown3);
	bswap_uns16_inplace(m_sudden_death_time);
	bswap_uns16_inplace(m_bomb_reset_time);
	bswap_uns16_inplace(m_bomb_arming_time);
	bswap_uns16_inplace(m_bomb_disarming_time);
	bswap_uns16_inplace(m_bomb_fuse_time);
	m_carrier_traits.byteswap();
	m_arming_traits.byteswap();

	ASSERT(array_is_zeroed(m_pad1));
}

void c_game_engine_assault_variant::set(const c_game_engine_assault_variant* variant, bool force)
{
	ASSERT(variant != nullptr);

	c_game_engine_base_variant::set(variant, force);

	get_miscellaneous_options_writeable()->set_teams_enabled(true);
	set_game_type(variant->get_game_type());
	set_respawn(variant->get_respawn());
	set_enemy_bomb_waypoint(variant->get_enemy_bomb_waypoint());
	set_reset_bomb_on_disarm(variant->get_reset_bomb_on_disarm());
	set_score_to_win(variant->get_score_to_win());
	set_sudden_death_time(variant->get_sudden_death_time());
	set_bomb_arming_time(variant->get_bomb_arming_time());
	set_bomb_disarming_time(variant->get_bomb_disarming_time());
	set_bomb_fuse_time(variant->get_bomb_fuse_time());
	set_bomb_reset_time(variant->get_bomb_reset_time());
	set_carrier_traits(variant->get_carrier_traits(), force);
	set_arming_traits(variant->get_arming_traits(), force);

	zero_array(m_pad1);
}

//void c_game_engine_assault_variant::set(const s_game_engine_assault_variant_definition* definition, bool force)
//{
//}

void c_game_engine_assault_variant::encode_to_mcc(c_bitstream* packet) const
{
	c_game_engine_base_variant::encode_to_mcc(packet);

	bool reset_bomb_on_disarm = get_reset_bomb_on_disarm();
	int16 sudden_death_time = get_sudden_death_time();
	int16 bomb_arming_time = get_bomb_arming_time();
	int16 bomb_disarming_time = get_bomb_disarming_time();
	int16 bomb_fuse_time = get_bomb_fuse_time();
	int16 bomb_reset_time = get_bomb_reset_time();

	packet->write_bool("assault-reset-bomb-on-disarm", reset_bomb_on_disarm);
	packet->write_integer("assault-bomb-arming-time", sudden_death_time, 9);
	packet->write_integer("assault-bomb-arming-time", bomb_arming_time, 5);
	packet->write_integer("assault-bomb-disarming-time", bomb_disarming_time, 5);
	packet->write_integer("assault-bomb-fuse-time", bomb_fuse_time, 5);
	packet->write_integer("assault-bomb-reset-time", bomb_reset_time, 6);
	get_carrier_traits()->encode_to_mcc(packet);
	get_arming_traits()->encode_to_mcc(packet);
}

void c_game_engine_assault_variant::decode_from_mcc(c_bitstream* packet)
{
	c_game_engine_base_variant::decode_from_mcc(packet);

	bool reset_bomb_on_disarm = packet->read_bool("assault-reset-bomb-on-disarm");
	int16 sudden_death_time = static_cast<int16>(packet->read_integer("assault-bomb-arming-time", 9));
	int16 bomb_arming_time = static_cast<int16>(packet->read_integer("assault-bomb-arming-time", 5));
	int16 bomb_disarming_time = static_cast<int16>(packet->read_integer("assault-bomb-disarming-time", 5));
	int16 bomb_fuse_time = static_cast<int16>(packet->read_integer("assault-bomb-fuse-time", 5));
	int16 bomb_reset_time = static_cast<int16>(packet->read_integer("assault-bomb-reset-time", 6));
	get_carrier_traits_writeable()->decode_from_mcc(packet);
	get_arming_traits_writeable()->decode_from_mcc(packet);

	set_reset_bomb_on_disarm(reset_bomb_on_disarm);
	set_sudden_death_time(sudden_death_time);
	set_bomb_arming_time(bomb_arming_time);
	set_bomb_disarming_time(bomb_disarming_time);
	set_bomb_fuse_time(bomb_fuse_time);
	set_bomb_reset_time(bomb_reset_time);
}

bool c_game_engine_assault_variant::get_reset_bomb_on_disarm() const
{
	return m_variant_flags.test(_assault_variant_flags_reset_bomb_on_disarm);
}

void c_game_engine_assault_variant::set_reset_bomb_on_disarm(bool reset_bomb_on_disarm)
{
	m_variant_flags.set(_assault_variant_flags_reset_bomb_on_disarm, reset_bomb_on_disarm);
}

bool c_game_engine_assault_variant::get_siege_mode() const
{
	return m_variant_flags.test(_assault_variant_flags_siege_mode);
}

void c_game_engine_assault_variant::set_siege_mode(bool siege_mode)
{
	m_variant_flags.set(_assault_variant_flags_siege_mode, siege_mode);
}

e_assault_respawn_settings c_game_engine_assault_variant::get_respawn() const
{
	return m_respawn;
}

void c_game_engine_assault_variant::set_respawn(e_assault_respawn_settings respawn)
{
	if (!VALID_INDEX(respawn, k_assault_respawn_settings))
	{
		event(_event_warning, "game_engine:assault: invalid respawn setting '%d' outside range '[0, %d)'!", respawn, k_assault_respawn_settings);

		m_respawn = k_assault_respawn_settings_default;
	}
	else
	{
		m_respawn = respawn;
	}
}

e_assault_game_type_settings c_game_engine_assault_variant::get_game_type() const
{
	return m_game_type;
}

void c_game_engine_assault_variant::set_game_type(e_assault_game_type_settings game_type)
{
	if (!VALID_INDEX(game_type, k_assault_game_type_settings))
	{
		event(_event_warning, "game_engine:assault: invalid game_type setting '%d' outside range '[0, %d)'!", game_type, k_assault_game_type_settings);

		m_game_type = k_assault_game_type_settings_default;
	}
	else
	{
		m_game_type = game_type;
	}
}

e_assault_enemy_bomb_waypoint_settings c_game_engine_assault_variant::get_enemy_bomb_waypoint() const
{
	return m_enemy_bomb_waypoint;
}

void c_game_engine_assault_variant::set_enemy_bomb_waypoint(e_assault_enemy_bomb_waypoint_settings enemy_bomb_waypoint)
{
	if (!VALID_INDEX(enemy_bomb_waypoint, k_assault_enemy_bomb_waypoint_settings))
	{
		event(_event_warning, "game_engine:assault: invalid enemy_bomb_waypoint setting '%d' outside range '[0, %d)'!", enemy_bomb_waypoint, k_assault_enemy_bomb_waypoint_settings);

		m_enemy_bomb_waypoint = k_assault_enemy_bomb_waypoint_settings_default;
	}
	else
	{
		m_enemy_bomb_waypoint = enemy_bomb_waypoint;
	}
}

int16 c_game_engine_assault_variant::get_score_to_win() const
{
	return m_score_to_win;
}

void c_game_engine_assault_variant::set_score_to_win(int16 score_to_win)
{
	if (!VALID_INDEX(score_to_win, 50))
	{
		event(_event_warning, "game_engine:ctf: invalid score_to_win setting '%d' outside range '[%d, %d]'!", score_to_win, 0, 50);

		m_score_to_win = 3;
	}
	else
	{
		m_score_to_win = score_to_win;
	}
}

int16 c_game_engine_assault_variant::get_sudden_death_time() const
{
	return m_sudden_death_time;
}

void c_game_engine_assault_variant::set_sudden_death_time(int16 sudden_death_time)
{
	if (!VALID_INDEX(sudden_death_time + 1, k_assault_variant_sudden_death_time))
	{
		event(_event_warning, "game_engine:assault: invalid sudden_death_time setting '%d' outside range '[%d, %d]'!", sudden_death_time, _assault_variant_sudden_death_time_infinite, _assault_variant_sudden_death_time_5_minute);

		m_sudden_death_time = k_assault_variant_sudden_death_time_default;
	}
	else
	{
		m_sudden_death_time = sudden_death_time;
	}
}

int16 c_game_engine_assault_variant::get_bomb_reset_time() const
{
	return m_bomb_reset_time;
}

void c_game_engine_assault_variant::set_bomb_reset_time(int16 bomb_reset_time)
{
	if (!VALID_INDEX(bomb_reset_time, 90))
	{
		event(_event_warning, "game_engine:assault: invalid bomb_reset_time setting '%d' outside range '[%d, %d]'!", bomb_reset_time, 0, 90);

		m_bomb_reset_time = 30;
	}
	else
	{
		m_bomb_reset_time = bomb_reset_time;
	}
}

int16 c_game_engine_assault_variant::get_bomb_arming_time() const
{
	return m_bomb_arming_time;
}

void c_game_engine_assault_variant::set_bomb_arming_time(int16 bomb_arming_time)
{
	if (!VALID_INDEX(bomb_arming_time, 90))
	{
		event(_event_warning, "game_engine:assault: invalid bomb_arming_time setting '%d' outside range '[%d, %d]'!", bomb_arming_time, 0, 90);

		m_bomb_arming_time = 35;
	}
	else
	{
		m_bomb_arming_time = bomb_arming_time;
	}
}

int16 c_game_engine_assault_variant::get_bomb_disarming_time() const
{
	return m_bomb_disarming_time;
}

void c_game_engine_assault_variant::set_bomb_disarming_time(int16 bomb_disarming_time)
{
	if (!VALID_INDEX(bomb_disarming_time, 90))
	{
		event(_event_warning, "game_engine:assault: invalid bomb_disarming_time setting '%d' outside range '[%d, %d]'!", bomb_disarming_time, 0, 90);

		m_bomb_disarming_time = 35;
	}
	else
	{
		m_bomb_disarming_time = bomb_disarming_time;
	}
}

int16 c_game_engine_assault_variant::get_bomb_fuse_time() const
{
	return m_bomb_fuse_time;
}

void c_game_engine_assault_variant::set_bomb_fuse_time(int16 bomb_fuse_time)
{
	if (!VALID_INDEX(bomb_fuse_time, 90))
	{
		event(_event_warning, "game_engine:assault: invalid bomb_fuse_time setting '%d' outside range '[%d, %d]'!", bomb_fuse_time, 0, 90);

		m_bomb_fuse_time = 35;
	}
	else
	{
		m_bomb_fuse_time = bomb_fuse_time;
	}
}

c_player_traits* c_game_engine_assault_variant::get_carrier_traits_writeable()
{
	return &m_carrier_traits;
}

const c_player_traits* c_game_engine_assault_variant::get_carrier_traits() const
{
	return &m_carrier_traits;
}

void c_game_engine_assault_variant::set_carrier_traits(const c_player_traits* traits, bool force)
{
	m_carrier_traits.set(traits, force);
}

c_player_traits* c_game_engine_assault_variant::get_arming_traits_writeable()
{
	return &m_arming_traits;
}

const c_player_traits* c_game_engine_assault_variant::get_arming_traits() const
{
	return &m_arming_traits;
}

void c_game_engine_assault_variant::set_arming_traits(const c_player_traits* traits, bool force)
{
	m_arming_traits.set(traits, force);
}

void c_assault_engine::dump_settings(s_file_reference* file) const
{
	ASSERT(file != nullptr);

	c_game_engine::dump_settings(file);

	file_printf(file, "---------- ASSAULT variant settings\r\n");

	file_printf(file, "score to win= %d\r\n", assault_variant->get_score_to_win());

	file_printf(file, "enemy bomb waypoint= ");
	switch (assault_variant->get_enemy_bomb_waypoint())
	{
	case _assault_enemy_bomb_waypoint_settings_never:
		file_printf(file, "NEVER\r\n");
		break;
	case _assault_enemy_bomb_waypoint_settings_when_not_carried_by_enemy:
		file_printf(file, "WHEN NOT CARRIED BY ENEMY\r\n");
		break;
	case _assault_enemy_bomb_waypoint_settings_when_armed:
		file_printf(file, "WHEN ARMED\r\n");
		break;
	case _assault_enemy_bomb_waypoint_settings_alway:
		file_printf(file, "ALWAYS\r\n");
		break;
	default:
		file_printf(file, "<current option setting not exported!>\r\n");
		break;
	}

	file_printf(file, "game type= ");
	switch (assault_variant->get_game_type())
	{
	case _assault_game_type_settings_multi_bomb:
		file_printf(file, "MULTI BOMB\r\n");
		break;
	case _assault_game_type_settings_one_bomb:
		file_printf(file, "SINGLE BOMB\r\n");
		break;
	case _assault_game_type_settings_neutral_bomb:
		file_printf(file, "NEUTRAL BOMB\r\n");
		break;
	default:
		file_printf(file, "<current option setting not exported!>\r\n");
		break;
	}

	file_printf(file, "respawn= ");
	switch (assault_variant->get_respawn())
	{
	case _assault_respawn_settings_disabled:
		file_printf(file, "DISABLED\r\n");
		break;
	case _assault_respawn_settings_on_friendly_detonation:
		file_printf(file, "ON FRIENDLY DETONATION\r\n");
		break;
	case _assault_respawn_settings_on_enemy_detonation:
		file_printf(file, "ON ENEMY DETONATION\r\n");
		break;
	case _assault_respawn_settings_on_any_detonation:
		file_printf(file, "ON ANY DETONATION\r\n");
		break;
	}

	//file_printf(file, "sudden death time (negative is infinite)= %d sec\r\n", assault_variant->get_sudden_death_time());
	file_printf(file, "sudden death time= ");
	switch (assault_variant->get_sudden_death_time())
	{
	case _assault_variant_sudden_death_time_infinite:
		file_printf(file, "INFINITE\r\n");
		break;
	case _assault_variant_sudden_death_time_off:
		file_printf(file, "OFF\r\n");
		break;
	default:
		file_printf(file, "%d SECONDS\r\n", assault_variant->get_sudden_death_time());
		break;
	}

	file_printf(file, "bomb arming time= %d sec\r\n", assault_variant->get_bomb_arming_time());
	file_printf(file, "bomb reset time= %d sec\r\n", assault_variant->get_bomb_reset_time());
	file_printf(file, "bomb disarming time= %d sec\r\n", assault_variant->get_bomb_disarming_time());
	file_printf(file, "bomb WILL%s reset on disarm\r\n", assault_variant->get_reset_bomb_on_disarm() ? "" : "NOT");
	file_printf(file, "bomb fuse time= %d\r\n", assault_variant->get_bomb_fuse_time());
	dump_player_trait_settings("bomb carrier traits", assault_variant->get_carrier_traits(), file);
	dump_player_trait_settings("bomb arming traits", assault_variant->get_arming_traits(), file);

	file_printf(file, "---------- END ASSAULT variant settings\r\n");
}

