#include "game/game_engine_juggernaut.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/bitstream.hpp"
#include "memory/byte_swapping.hpp"

REFERENCE_DECLARE(0x050D9F7C, c_game_engine_juggernaut_variant*, juggernaut_variant);

void c_game_engine_juggernaut_variant::byteswap()
{
	c_game_engine_base_variant::byteswap();

	bswap_uns16_inplace(m_score_to_win_round);
	bswap_uns16_inplace(m_score_unknown);

	ASSERT(array_is_zeroed(m_pad));

	m_juggernaut_traits.byteswap();

	ASSERT(array_is_zeroed(m_pad1));
}

void c_game_engine_juggernaut_variant::set(const c_game_engine_juggernaut_variant* variant, bool force)
{
	ASSERT(variant != nullptr);

	c_game_engine_base_variant::set(variant, force);

	set_allied_against_juggernaut(variant->get_allied_against_juggernaut());
	set_respawn_on_lone_juggernaut(variant->get_respawn_on_lone_juggernaut());
	set_destination_zones_enabled(variant->get_destination_zones_enabled());
	set_score_to_win_round(static_cast<int16>(variant->get_score_to_win_round()));

	zero_array(m_pad);

	set_initial_juggernaut(variant->get_initial_juggernaut());
	set_next_juggernaut(variant->get_next_juggernaut());
	set_zone_movement(variant->get_zone_movement());
	set_zone_order(variant->get_zone_order());
	set_kill_points(variant->get_kill_points());
	set_juggernaut_kill_points(variant->get_juggernaut_kill_points());
	set_kill_as_juggernaut_points(variant->get_kill_as_juggernaut_points());
	set_destination_arrival_points(variant->get_destination_arrival_points());
	set_suicide_points(variant->get_suicide_points());
	set_betrayal_points(variant->get_betrayal_points());
	set_juggernaut_delay(variant->get_juggernaut_delay());
	set_juggernaut_traits(get_juggernaut_traits(), force);

	zero_array(m_pad1);
}

//void c_game_engine_juggernaut_variant::set(const s_game_engine_juggernaut_variant_definition* definition, bool force)
//{
//}

void c_game_engine_juggernaut_variant::encode_to_mcc(c_bitstream* packet) const
{
	c_game_engine_base_variant::encode_to_mcc(packet);

	bool allied_against_juggernaut = get_allied_against_juggernaut();
	bool respawn_on_lone_juggernaut = get_respawn_on_lone_juggernaut();
	bool destination_zones_enabled = get_destination_zones_enabled();
	int16 score_to_win_round = static_cast<int16>(get_score_to_win_round());
	e_juggernaut_initial_juggernaut_settings initial_juggernaut = get_initial_juggernaut();
	e_juggernaut_next_juggernaut_settings next_juggernaut = get_next_juggernaut();
	e_juggernaut_zone_movement_settings zone_movement = get_zone_movement();
	e_juggernaut_zone_order_settings zone_order = get_zone_order();
	int8 kill_points = get_kill_points();
	int8 juggernaut_kill_points = get_juggernaut_kill_points();
	int8 kill_as_juggernaut_points = get_kill_as_juggernaut_points();
	int8 destination_arrival_points = get_destination_arrival_points();
	int8 suicide_points = get_suicide_points();
	int8 betrayal_points = get_betrayal_points();
	uns8 juggernaut_delay = get_juggernaut_delay();

	packet->write_bool("juggernaut-allied-against-juggernaut", allied_against_juggernaut);
	packet->write_bool("juggernaut-respawn-on-lone-juggernaut", respawn_on_lone_juggernaut);
	packet->write_bool("juggernaut-destination-zones-enabled", destination_zones_enabled);
	packet->write_integer("juggernaut-score-to-win-round", score_to_win_round, 9);
	packet->write_integer("juggernaut-initial-juggernaut", initial_juggernaut, 2);
	packet->write_integer("juggernaut-next-juggernaut", next_juggernaut, 2);
	packet->write_integer("juggernaut-zone-movement", zone_movement, 4);
	packet->write_integer("juggernaut-zone-order", zone_order, 1);
	packet->write_signed_integer("juggernaut-kill-points", kill_points, 5);
	packet->write_signed_integer("juggernaut-juggernaut-kill-points", juggernaut_kill_points, 5);
	packet->write_signed_integer("juggernaut-kill-as-juggernaut-points", kill_as_juggernaut_points, 5);
	packet->write_signed_integer("juggernaut-destination-arrival-points", destination_arrival_points, 5);
	packet->write_signed_integer("juggernaut-suicide-points", suicide_points, 5);
	packet->write_signed_integer("juggernaut-betrayal-points", betrayal_points, 5);
	packet->write_integer("juggernaut-juggernaut-delay", juggernaut_delay, 4);
	get_juggernaut_traits()->encode_to_mcc(packet);
}

void c_game_engine_juggernaut_variant::decode_from_mcc(c_bitstream* packet)
{
	c_game_engine_base_variant::decode_from_mcc(packet);

	bool allied_against_juggernaut = packet->read_bool("juggernaut-allied-against-juggernaut");
	bool respawn_on_lone_juggernaut = packet->read_bool("juggernaut-respawn-on-lone-juggernaut");
	bool destination_zones_enabled = packet->read_bool("juggernaut-destination-zones-enabled");
	int16 score_to_win_round = static_cast<int16>(packet->read_integer("juggernaut-score-to-win-round", 9));
	e_juggernaut_initial_juggernaut_settings initial_juggernaut = packet->read_enum<e_juggernaut_initial_juggernaut_settings, 2>("juggernaut-initial-juggernaut");
	e_juggernaut_next_juggernaut_settings next_juggernaut = packet->read_enum<e_juggernaut_next_juggernaut_settings, 2>("juggernaut-next-juggernaut");
	e_juggernaut_zone_movement_settings zone_movement = packet->read_enum<e_juggernaut_zone_movement_settings, 4>("juggernaut-zone-movement");
	e_juggernaut_zone_order_settings zone_order = packet->read_enum<e_juggernaut_zone_order_settings, 1>("juggernaut-zone-order");
	int8 kill_points = static_cast<int8>(packet->read_signed_integer("juggernaut-kill-points", 5));
	int8 juggernaut_kill_points = static_cast<int8>(packet->read_signed_integer("juggernaut-juggernaut-kill-points", 5));
	int8 kill_as_juggernaut_points = static_cast<int8>(packet->read_signed_integer("juggernaut-kill-as-juggernaut-points", 5));
	int8 destination_arrival_points = static_cast<int8>(packet->read_signed_integer("juggernaut-destination-arrival-points", 5));
	int8 suicide_points = static_cast<int8>(packet->read_signed_integer("juggernaut-suicide-points", 5));
	int8 betrayal_points = static_cast<int8>(packet->read_signed_integer("juggernaut-betrayal-points", 5));
	uns8 juggernaut_delay = static_cast<uns8>(packet->read_integer("juggernaut-juggernaut-delay", 4));
	get_juggernaut_traits_writeable()->decode_from_mcc(packet);

	set_allied_against_juggernaut(allied_against_juggernaut);
	set_respawn_on_lone_juggernaut(respawn_on_lone_juggernaut);
	set_destination_zones_enabled(destination_zones_enabled);
	set_score_to_win_round(score_to_win_round);
	set_initial_juggernaut(initial_juggernaut);
	set_next_juggernaut(next_juggernaut);
	set_zone_movement(zone_movement);
	set_zone_order(zone_order);
	set_kill_points(kill_points);
	set_juggernaut_kill_points(juggernaut_kill_points);
	set_kill_as_juggernaut_points(kill_as_juggernaut_points);
	set_destination_arrival_points(destination_arrival_points);
	set_suicide_points(suicide_points);
	set_betrayal_points(betrayal_points);
	set_juggernaut_delay(juggernaut_delay);
}

bool c_game_engine_juggernaut_variant::get_allied_against_juggernaut() const
{
	return m_variant_flags.test(_juggernaut_variant_flags_allied_against_juggernaut);
}

void c_game_engine_juggernaut_variant::set_allied_against_juggernaut(bool allied_against_juggernaut)
{
	m_variant_flags.set(_juggernaut_variant_flags_allied_against_juggernaut, allied_against_juggernaut);
}

bool c_game_engine_juggernaut_variant::get_respawn_on_lone_juggernaut() const
{
	return m_variant_flags.test(_juggernaut_variant_flags_respawn_on_lone_juggernaut);
}

void c_game_engine_juggernaut_variant::set_respawn_on_lone_juggernaut(bool respawn_on_lone_juggernaut)
{
	m_variant_flags.set(_juggernaut_variant_flags_respawn_on_lone_juggernaut, respawn_on_lone_juggernaut);
}

bool c_game_engine_juggernaut_variant::get_destination_zones_enabled() const
{
	return m_variant_flags.test(_juggernaut_variant_flags_destination_zones_enabled);
}

void c_game_engine_juggernaut_variant::set_destination_zones_enabled(bool destination_zones_enabled)
{
	m_variant_flags.set(_juggernaut_variant_flags_destination_zones_enabled, destination_zones_enabled);
}

void c_game_engine_juggernaut_variant::set_score_to_win_round(int16 score_to_win_round)
{
	if (!VALID_INDEX(score_to_win_round, 500))
	{
		event(_event_warning, "game_engine:juggernaut: invalid score_to_win_round setting '%d' outside range '[%d, %d]'!", score_to_win_round, 0, 500);

		m_score_to_win_round = 15;
	}
	else
	{
		m_score_to_win_round = score_to_win_round;
	}
}

e_juggernaut_initial_juggernaut_settings c_game_engine_juggernaut_variant::get_initial_juggernaut() const
{
	return m_initial_juggernaut;
}

void c_game_engine_juggernaut_variant::set_initial_juggernaut(e_juggernaut_initial_juggernaut_settings initial_juggernaut)
{
	if (!VALID_INDEX(initial_juggernaut, k_juggernaut_initial_juggernaut_settings))
	{
		event(_event_warning, "game_engine:juggernaut: invalid initial_juggernaut setting '%d' outside range '[0, %d)'!", initial_juggernaut, k_juggernaut_next_juggernaut_settings);

		m_initial_juggernaut = k_juggernaut_initial_juggernaut_settings_default;
	}
	else
	{
		m_initial_juggernaut = initial_juggernaut;
	}
}

e_juggernaut_next_juggernaut_settings c_game_engine_juggernaut_variant::get_next_juggernaut() const
{
	return m_next_juggernaut;
}

void c_game_engine_juggernaut_variant::set_next_juggernaut(e_juggernaut_next_juggernaut_settings next_juggernaut)
{
	if (!VALID_INDEX(next_juggernaut, k_juggernaut_next_juggernaut_settings))
	{
		event(_event_warning, "game_engine:juggernaut: invalid next_juggernaut setting '%d' outside range '[0, %d)'!", next_juggernaut, k_juggernaut_next_juggernaut_settings);

		m_zone_movement = k_juggernaut_next_juggernaut_settings_default;
	}
	else
	{
		m_zone_movement = next_juggernaut;
	}
}

e_juggernaut_zone_movement_settings c_game_engine_juggernaut_variant::get_zone_movement() const
{
	return m_zone_movement;
}

int32 c_game_engine_juggernaut_variant::get_zone_movement_time_in_seconds() const
{
	//ASSERT(VALID_INDEX(m_zone_movement, k_number_of_juggernaut_zone_movement_settings));
	ASSERT(VALID_INDEX(get_zone_movement(), k_number_of_juggernaut_zone_movement_settings));

	return k_juggernaut_zone_movement_values[get_zone_movement()];
}

void c_game_engine_juggernaut_variant::set_zone_movement(e_juggernaut_zone_movement_settings zone_movement)
{
	if (!VALID_INDEX(zone_movement, k_number_of_juggernaut_zone_movement_settings))
	{
		event(_event_warning, "game_engine:juggernaut: invalid zone_movement setting '%d' outside range '[0, %d)'!", zone_movement, k_number_of_juggernaut_zone_movement_settings);

		m_zone_movement = k_juggernaut_zone_movement_settings_default;
	}
	else
	{
		m_zone_movement = zone_movement;
	}
}

e_juggernaut_zone_order_settings c_game_engine_juggernaut_variant::get_zone_order() const
{
	return m_zone_order;
}

void c_game_engine_juggernaut_variant::set_zone_order(e_juggernaut_zone_order_settings zone_order)
{
	if (!VALID_INDEX(zone_order, k_juggernaut_zone_order_settings))
	{
		event(_event_warning, "game_engine:juggernaut: invalid zone_order setting '%d' outside range '[0, %d)'!", zone_order, k_juggernaut_zone_order_settings);

		m_zone_order = k_juggernaut_zone_order_settings_default;
	}
	else
	{
		m_zone_order = zone_order;
	}
}

int8 c_game_engine_juggernaut_variant::get_kill_points() const
{
	return m_kill_points;
}

void c_game_engine_juggernaut_variant::set_kill_points(int8 kill_points)
{
	if (!VALID_INDEX(kill_points + 10, 20))
	{
		event(_event_warning, "game_engine:juggernaut: invalid kill_points setting '%d' outside range '[%d, %d]'!", kill_points, -10, 10);

		m_kill_points = 0;
	}
	else
	{
		m_kill_points = kill_points;
	}
}

int8 c_game_engine_juggernaut_variant::get_juggernaut_kill_points() const
{
	return m_juggernaut_kill_points;
}

void c_game_engine_juggernaut_variant::set_juggernaut_kill_points(int8 juggernaut_kill_points)
{
	if (!VALID_INDEX(juggernaut_kill_points + 10, 20))
	{
		event(_event_warning, "game_engine:juggernaut: invalid juggernaut_kill_points setting '%d' outside range '[%d, %d]'!", juggernaut_kill_points, -10, 10);

		m_juggernaut_kill_points = 1;
	}
	else
	{
		m_juggernaut_kill_points = juggernaut_kill_points;
	}
}

int8 c_game_engine_juggernaut_variant::get_kill_as_juggernaut_points() const
{
	return m_kill_as_juggernaut_points;
}

void c_game_engine_juggernaut_variant::set_kill_as_juggernaut_points(int8 kill_as_juggernaut_points)
{
	if (!VALID_INDEX(kill_as_juggernaut_points + 10, 20))
	{
		event(_event_warning, "game_engine:juggernaut: invalid kill_as_juggernaut_points setting '%d' outside range '[%d, %d]'!", kill_as_juggernaut_points, -10, 10);

		m_kill_as_juggernaut_points = 1;
	}
	else
	{
		m_kill_as_juggernaut_points = kill_as_juggernaut_points;
	}
}

int8 c_game_engine_juggernaut_variant::get_destination_arrival_points() const
{
	return m_destination_arrival_points;
}

void c_game_engine_juggernaut_variant::set_destination_arrival_points(int8 destination_arrival_points)
{
	if (!VALID_INDEX(destination_arrival_points + 10, 20))
	{
		event(_event_warning, "game_engine:juggernaut: invalid destination_arrival_points setting '%d' outside range '[%d, %d]'!", destination_arrival_points, -10, 10);

		m_destination_arrival_points = 1;
	}
	else
	{
		m_destination_arrival_points = destination_arrival_points;
	}
}

int8 c_game_engine_juggernaut_variant::get_suicide_points() const
{
	return m_suicide_points;
}

void c_game_engine_juggernaut_variant::set_suicide_points(int8 suicide_points)
{
	if (!VALID_INDEX(suicide_points + 10, 20))
	{
		event(_event_warning, "game_engine:juggernaut: invalid suicide_points setting '%d' outside range '[%d, %d]'!", suicide_points, -10, 10);

		m_suicide_points = 1;
	}
	else
	{
		m_suicide_points = suicide_points;
	}
}

int8 c_game_engine_juggernaut_variant::get_betrayal_points() const
{
	return m_betrayal_points;
}

void c_game_engine_juggernaut_variant::set_betrayal_points(int8 betrayal_points)
{
	if (!VALID_INDEX(betrayal_points + 10, 20))
	{
		event(_event_warning, "game_engine:juggernaut: invalid betrayal_points setting '%d' outside range '[%d, %d]'!", betrayal_points, -10, 10);

		m_betrayal_points = 1;
	}
	else
	{
		m_betrayal_points = betrayal_points;
	}
}

uns8 c_game_engine_juggernaut_variant::get_juggernaut_delay() const
{
	return m_juggernaut_delay;
}

void c_game_engine_juggernaut_variant::set_juggernaut_delay(uns8 juggernaut_delay)
{
	if (!VALID_INDEX(juggernaut_delay, 10))
	{
		event(_event_warning, "game_engine:juggernaut: invalid juggernaut_delay setting '%d' outside range '[%d, %d]'!", juggernaut_delay, 0, 10);

		m_juggernaut_delay = 0;
	}
	else
	{
		m_juggernaut_delay = juggernaut_delay;
	}
}

c_player_traits* c_game_engine_juggernaut_variant::get_juggernaut_traits_writeable()
{
	return &m_juggernaut_traits;
}

const c_player_traits* c_game_engine_juggernaut_variant::get_juggernaut_traits() const
{
	return &m_juggernaut_traits;
}

void c_game_engine_juggernaut_variant::set_juggernaut_traits(const c_player_traits* traits, bool force)
{
	m_juggernaut_traits.set(traits, force);
}

void c_juggernaut_engine::dump_settings(s_file_reference* file) const
{
	ASSERT(file != nullptr);

	c_game_engine::dump_settings(file);

	file_printf(file, "----------- JUGGERNAUT variant settings\r\n");

	file_printf(file, "allied against juggernaut= %s\r\n", juggernaut_variant->get_allied_against_juggernaut() ? "ENABLED" : "DISABLED");
	file_printf(file, "respawn on lone juggernaut= %s\r\n", juggernaut_variant->get_respawn_on_lone_juggernaut() ? "ENABLED" : "DISABLED");
	file_printf(file, "score to win round= %d\r\n", juggernaut_variant->get_score_to_win_round());

	file_printf(file, "initial juggernaut= ");
	switch (juggernaut_variant->get_initial_juggernaut())
	{
	case _juggernaut_initial_juggernaut_settings_random:
		file_printf(file, "RANDOM\r\n");
		break;
	case _juggernaut_initial_juggernaut_settings_first_kill:
		file_printf(file, "FIRST KILL\r\n");
		break;
	case _juggernaut_initial_juggernaut_settings_first_death:
		file_printf(file, "FIRST DEATH\r\n");
		break;
	}

	file_printf(file, "next juggernaut= ");
	switch (juggernaut_variant->get_next_juggernaut())
	{
	case _juggernaut_next_juggernaut_settings_on_killing_juggernaut:
		file_printf(file, "ON KILL\r\n");
		break;
	case _juggernaut_next_juggernaut_settings_on_killed_by_juggernaut:
		file_printf(file, "TAG\r\n");
		break;
	case _juggernaut_next_juggernaut_settings_unchanged:
		file_printf(file, "UNCHANGED\r\n");
		break;
	case _juggernaut_next_juggernaut_settings_random:
		file_printf(file, "RANDOM\r\n");
		break;
	}

	file_printf(file, "zone movement= ");
	switch (juggernaut_variant->get_zone_movement())
	{
	case _juggernaut_zone_movement_settings_off:
		file_printf(file, "OFF\r\n");
		break;
	case _juggernaut_zone_movement_settings_on_arrival:
		file_printf(file, "ON ARRIVAL\r\n");
		break;
	case _juggernaut_zone_movement_settings_on_switch:
		file_printf(file, "ON JUGGERNAUT SWITCH\r\n");
		break;
	default:
		file_printf(file, "%d seconds\r\n", juggernaut_variant->get_zone_movement_time_in_seconds());
		break;
	}

	file_printf(file, "zone order= %s\r\n", !juggernaut_variant->get_zone_order() ? "RANDOM" : "SEQUENCE");

	file_printf(file, "kill points= %d\r\n", juggernaut_variant->get_kill_points());
	file_printf(file, "juggernaut kill points= %d\r\n", juggernaut_variant->get_juggernaut_kill_points());
	file_printf(file, "kill as juggernaut points= %d\r\n", juggernaut_variant->get_kill_as_juggernaut_points());
	file_printf(file, "destination arrival points= %d\r\n", juggernaut_variant->get_destination_arrival_points());
	file_printf(file, "suicide points= %d\r\n", juggernaut_variant->get_suicide_points());
	file_printf(file, "betrayal points= %d\r\n", juggernaut_variant->get_betrayal_points());
	file_printf(file, "juggernaut delay= %d\r\n", juggernaut_variant->get_juggernaut_delay());
	dump_player_trait_settings("juggernaut traits", juggernaut_variant->get_juggernaut_traits(), file);

	file_printf(file, "---------- END JUGGERNAUT variant settings\r\n");
}

