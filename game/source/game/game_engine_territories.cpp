#include "game/game_engine_territories.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/bitstream.hpp"
#include "memory/byte_swapping.hpp"

REFERENCE_DECLARE(0x050D9F80, c_game_engine_territories_variant*, territories_variant);

void c_game_engine_territories_variant::byteswap()
{
	c_game_engine_base_variant::byteswap();

	bswap_uns16_inplace(m_variant_flags);
	bswap_uns16_inplace(m_respawn_on_capture);
	bswap_uns16_inplace(m_capture_time);
	bswap_uns16_inplace(m_sudden_death_time);
	m_defender_traits.byteswap();
	m_attacker_traits.byteswap();
}

void c_game_engine_territories_variant::set(const c_game_engine_territories_variant* variant, bool force)
{
	ASSERT(variant != nullptr);

	c_game_engine_base_variant::set(variant, force);

	set_one_sided(variant->get_one_sided());
	set_lock_after_first_capture(variant->get_lock_after_first_capture());
	set_respawn_on_capture(variant->get_respawn_on_capture());
	set_capture_time(variant->get_capture_time());
	set_sudden_death_time(variant->get_sudden_death_time());
	set_defender_traits(variant->get_defender_traits(), force);
	set_attacker_traits(variant->get_attacker_traits(), force);
}

//void c_game_engine_territories_variant::set(const s_game_engine_territories_variant_definition* definition, bool force)
//{
//}

void c_game_engine_territories_variant::encode_to_mcc(c_bitstream* packet) const
{
	c_game_engine_base_variant::encode_to_mcc(packet);

	bool one_sided = get_one_sided();
	bool lock_after_first_capture = get_lock_after_first_capture();
	e_territories_respawn_on_capture_settings respawn_on_capture = get_respawn_on_capture();
	int16 capture_time = get_capture_time();

	packet->write_bool("territories-one-sided", one_sided);
	packet->write_bool("territories-lock-after-first-capture", lock_after_first_capture);
	packet->write_integer("territories-respawn-on-capture", respawn_on_capture, 2);
	packet->write_integer("territories-capture-time", capture_time, 7);
	get_defender_traits()->encode_to_mcc(packet);
	get_attacker_traits()->encode_to_mcc(packet);
}

void c_game_engine_territories_variant::decode_from_mcc(c_bitstream* packet)
{
	c_game_engine_base_variant::decode_from_mcc(packet);

	bool one_sided = packet->read_bool("territories-one-sided");
	bool lock_after_first_capture = packet->read_bool("territories-lock-after-first-capture");
	e_territories_respawn_on_capture_settings respawn_on_capture = packet->read_enum<e_territories_respawn_on_capture_settings, 2>("territories-respawn-on-capture");
	int16 capture_time = static_cast<int16>(packet->read_integer("territories-capture-time", 7));
	get_defender_traits_writeable()->decode_from_mcc(packet);
	get_attacker_traits_writeable()->decode_from_mcc(packet);

	set_one_sided(one_sided);
	set_lock_after_first_capture(lock_after_first_capture);
	set_respawn_on_capture(respawn_on_capture);
	set_capture_time(capture_time);
}

bool c_game_engine_territories_variant::get_one_sided() const
{
	return m_variant_flags.test(_territories_variant_flags_one_sided);
}

void c_game_engine_territories_variant::set_one_sided(bool one_sided)
{
	return m_variant_flags.set(_territories_variant_flags_one_sided, one_sided);
}

bool c_game_engine_territories_variant::get_lock_after_first_capture() const
{
	return m_variant_flags.test(_territories_variant_flags_lock_after_first_capture);
}

void c_game_engine_territories_variant::set_lock_after_first_capture(bool lock_after_first_capture)
{
	return m_variant_flags.set(_territories_variant_flags_lock_after_first_capture, lock_after_first_capture);
}

e_territories_respawn_on_capture_settings c_game_engine_territories_variant::get_respawn_on_capture() const
{
	return m_respawn_on_capture;
}

void c_game_engine_territories_variant::set_respawn_on_capture(e_territories_respawn_on_capture_settings respawn_on_capture)
{
	if (!VALID_INDEX(respawn_on_capture, k_territories_respawn_on_capture_settings))
	{
		event(_event_warning, "game_engine:territories: invalid respawn_on_capture setting '%d' outside range '[0, %d)'!", respawn_on_capture, k_territories_respawn_on_capture_settings);

		m_respawn_on_capture = k_territories_respawn_on_capture_settings_default;
	}
	else
	{
		m_respawn_on_capture = respawn_on_capture;
	}
}

int16 c_game_engine_territories_variant::get_capture_time() const
{
	return m_capture_time;
}

void c_game_engine_territories_variant::set_capture_time(int16 capture_time)
{
	if (!VALID_INDEX(capture_time, k_territories_capture_time_settings))
	{
		event(_event_warning, "game_engine:territories: invalid capture_time setting '%d' outside range '[%d, %d]'!", capture_time, _territories_capture_time_settings_instant, _territories_capture_time_settings_2_minutes);

		m_capture_time = k_territories_capture_time_settings_default;
	}
	else
	{
		m_capture_time = capture_time;
	}
}

int16 c_game_engine_territories_variant::get_sudden_death_time() const
{
	return m_sudden_death_time;
}

bool c_game_engine_territories_variant::get_sudden_death_enabled() const
{
	return m_sudden_death_time != _territories_sudden_death_settings_off;
}

void c_game_engine_territories_variant::set_sudden_death_time(int16 sudden_death_time)
{
	if (!VALID_INDEX(sudden_death_time + 1, k_territories_sudden_death_settings))
	{
		event(_event_warning, "game_engine:territories: invalid sudden_death_time setting '%d' outside range '[%d, %d]'!", sudden_death_time, _territories_sudden_death_settings_indefinite, _territories_sudden_death_settings_5_minutes);

		m_sudden_death_time = k_territories_sudden_death_settings_default;
	}
	else
	{
		m_sudden_death_time = sudden_death_time;
	}
}

c_player_traits* c_game_engine_territories_variant::get_defender_traits_writeable()
{
	return &m_defender_traits;
}

const c_player_traits* c_game_engine_territories_variant::get_defender_traits() const
{
	return &m_defender_traits;
}

void c_game_engine_territories_variant::set_defender_traits(const c_player_traits* traits, bool force)
{
	m_defender_traits.set(traits, force);
}

c_player_traits* c_game_engine_territories_variant::get_attacker_traits_writeable()
{
	return &m_attacker_traits;
}

const c_player_traits* c_game_engine_territories_variant::get_attacker_traits() const
{
	return &m_attacker_traits;
}

void c_game_engine_territories_variant::set_attacker_traits(const c_player_traits* traits, bool force)
{
	m_attacker_traits.set(traits, force);
}

void c_territories_engine::dump_settings(s_file_reference* file) const
{
	ASSERT(file != nullptr);

	c_game_engine::dump_settings(file);

	file_printf(file, "---------- TERRITORIES variant settings\r\n");

	file_printf(file, "one-sided= %s\r\n", territories_variant->get_one_sided() ? "TRUE" : "FALSE");
	file_printf(file, "lock after first capture= %s\r\n", territories_variant->get_lock_after_first_capture() ? "TRUE" : "FALSE");

	file_printf(file, "respawn on capture= ");
	switch (territories_variant->get_respawn_on_capture())
	{
	case _territories_respawn_on_capture_settings_disabled:
		file_printf(file, "DISABLED\r\n");
		break;
	case _territories_respawn_on_capture_settings_friendly:
		file_printf(file, "FRIENDLY\r\n");
		break;
	case _territories_respawn_on_capture_settings_enemy:
		file_printf(file, "ENEMY\r\n");
		break;
	case _territories_respawn_on_capture_settings_all:
		file_printf(file, "ALL\r\n");
		break;
	}

	file_printf(file, "sudden death time= ");
	switch (territories_variant->get_sudden_death_time())
	{
	case _territories_sudden_death_settings_indefinite:
		file_printf(file, "INDEFINITE\r\n");
		break;
	case _territories_sudden_death_settings_off:
		file_printf(file, "OFF\r\n");
		break;
	default:
		file_printf(file, "%d SECONDS\r\n", territories_variant->get_sudden_death_time());
		break;
	}

	//file_printf(file, "capture time= %d SECONDS\r\n", territories_variant->get_capture_time());
	file_printf(file, "capture time= ");
	switch (territories_variant->get_capture_time())
	{
	case _territories_capture_time_settings_instant:
		file_printf(file, "INSTANT\r\n");
		break;
	default:
		file_printf(file, "%d SECONDS\r\n", territories_variant->get_capture_time());
		break;
	}

	dump_player_trait_settings("defender", territories_variant->get_defender_traits(), file);
	dump_player_trait_settings("attacker", territories_variant->get_attacker_traits(), file);

	file_printf(file, "---------- END TERRITORIES variant settings\r\n");
}

