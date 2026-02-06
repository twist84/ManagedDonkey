#include "game/game_engine_king.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/bitstream.hpp"
#include "memory/byte_swapping.hpp"

REFERENCE_DECLARE(0x050D9F78, c_game_engine_king_variant*, king_variant);

void c_game_engine_king_variant::byteswap()
{
	c_game_engine_base_variant::byteswap();

	bswap_uns32_inplace(m_variant_flags);
	bswap_uns16_inplace(m_score_to_win);
	bswap_uns16_inplace(m_score_unknown);
	m_inside_hill_traits.byteswap();

	ASSERT(array_is_zeroed(m_pad1));
}

void c_game_engine_king_variant::set(const c_game_engine_king_variant* variant, bool force)
{
	ASSERT(variant != nullptr);

	c_game_engine_base_variant::set(variant, force);

	set_opaque_hill(variant->get_opaque_hill());
	set_score_to_win(variant->get_score_to_win());
	set_moving_hill(variant->get_moving_hill());
	set_moving_hill_order(variant->get_moving_hill_order());
	set_inside_hill_points(variant->get_inside_hill_points());
	set_outside_hill_points(variant->get_outside_hill_points());
	set_uncontested_hill_bonus(variant->get_uncontested_hill_bonus());
	set_kill_points(variant->get_kill_points());
	set_inside_hill_traits(variant->get_inside_hill_traits(), force);

	zero_array(m_pad1);
}

//void c_game_engine_king_variant::set(const s_game_engine_king_variant_definition* definition, bool force)
//{
//}

void c_game_engine_king_variant::encode_to_mcc(c_bitstream* packet) const
{
	c_game_engine_base_variant::encode_to_mcc(packet);

	bool opaque_hill = get_opaque_hill();
	int16 score_to_win = get_score_to_win();
	e_king_moving_hill_settings moving_hill = get_moving_hill();
	e_king_moving_hill_order_settings moving_hill_order = get_moving_hill_order();
	int8 inside_hill_points = get_inside_hill_points();
	int8 outside_hill_points = get_outside_hill_points();
	int8 uncontested_hill_bonus = get_uncontested_hill_bonus();
	int8 kill_points = get_kill_points();

	packet->write_bool("king-opaque-hill", opaque_hill);
	packet->write_integer("king-score-to-win", score_to_win, 10);
	packet->write_integer("king-moving-hill", moving_hill, 4);
	packet->write_integer("king-moving-hill-order", moving_hill_order, 2);
	packet->write_signed_integer("king_inside_hill_points", inside_hill_points, 5);
	packet->write_signed_integer("king_outside_hill_points", outside_hill_points, 5);
	packet->write_signed_integer("king_uncontested_hill_bonus", uncontested_hill_bonus, 5);
	packet->write_signed_integer("king_kill_points", kill_points, 5);
	get_inside_hill_traits()->encode_to_mcc(packet);
}

void c_game_engine_king_variant::decode_from_mcc(c_bitstream* packet)
{
	c_game_engine_base_variant::decode_from_mcc(packet);

	bool opaque_hill = packet->read_bool("king-opaque-hill");
	int16 score_to_win = static_cast<int16>(packet->read_integer("king-score-to-win", 10));
	e_king_moving_hill_settings moving_hill = packet->read_enum<e_king_moving_hill_settings, 4>("king-moving-hill");
	e_king_moving_hill_order_settings moving_hill_order = packet->read_enum<e_king_moving_hill_order_settings, 2>("king-moving-hill-order");
	int8 inside_hill_points = static_cast<int8>(packet->read_signed_integer("king_inside_hill_points", 5));
	int8 outside_hill_points = static_cast<int8>(packet->read_signed_integer("king_outside_hill_points", 5));
	int8 uncontested_hill_bonus = static_cast<int8>(packet->read_signed_integer("king_uncontested_hill_bonus", 5));
	int8 kill_points = static_cast<int8>(packet->read_signed_integer("king_kill_points", 5));
	get_inside_hill_traits_writeable()->decode_from_mcc(packet);

	set_opaque_hill(opaque_hill);
	set_score_to_win(score_to_win);
	set_moving_hill(moving_hill);
	set_moving_hill_order(moving_hill_order);
	set_inside_hill_points(inside_hill_points);
	set_outside_hill_points(outside_hill_points);
	set_uncontested_hill_bonus(uncontested_hill_bonus);
	set_kill_points(kill_points);
}

bool c_game_engine_king_variant::get_opaque_hill() const
{
	return m_variant_flags.test(_king_flags_setting_opaque_hill);
}

void c_game_engine_king_variant::set_opaque_hill(bool opaque_hill)
{
	m_variant_flags.set(_king_flags_setting_opaque_hill, opaque_hill);
}

int16 c_game_engine_king_variant::get_score_to_win() const
{
	return m_score_to_win;
}

void c_game_engine_king_variant::set_score_to_win(int16 score_to_win)
{
	if (!VALID_INDEX(score_to_win, 1000))
	{
		event(_event_warning, "game_engine:king: invalid score_to_win setting '%d' outside range '[%d, %d]'!", score_to_win, 0, 1000);

		m_score_to_win = 100;
	}
	else
	{
		m_score_to_win = score_to_win;
	}
}

e_king_moving_hill_settings c_game_engine_king_variant::get_moving_hill() const
{
	return m_moving_hill;
}

int16 c_game_engine_king_variant::get_hill_move_in_seconds() const
{
	e_king_moving_hill_settings setting = get_moving_hill();

	const int32 minimum_value = _king_moving_hill_settings_10_seconds;
	if (IN_RANGE_INCLUSIVE(setting, minimum_value, _king_moving_hill_settings_5_minutes))
		return k_king_moving_hill_values[setting - minimum_value];

	return k_king_moving_hill_values[k_king_moving_hill_settings_default - minimum_value];
}

void c_game_engine_king_variant::set_moving_hill(e_king_moving_hill_settings moving_hill)
{
	if (!VALID_INDEX(moving_hill, k_king_moving_hill_settings))
	{
		event(_event_warning, "game_engine:king: invalid moving_hill setting '%d' outside range '[0, %d)'!", moving_hill, k_king_moving_hill_settings);

		m_moving_hill = k_king_moving_hill_settings_default;
	}
	else
	{
		m_moving_hill = moving_hill;
	}
}

e_king_moving_hill_order_settings c_game_engine_king_variant::get_moving_hill_order() const
{
	return m_moving_hill_order;
}

void c_game_engine_king_variant::set_moving_hill_order(e_king_moving_hill_order_settings moving_hill_order)
{
	if (!VALID_INDEX(moving_hill_order, k_king_moving_hill_order_settings))
	{
		event(_event_warning, "game_engine:king: invalid moving_hill_order setting '%d' outside range '[0, %d)'!", moving_hill_order, k_king_moving_hill_order_settings);

		m_moving_hill_order = k_king_moving_hill_order_settings_default;
	}
	else
	{
		m_moving_hill_order = moving_hill_order;
	}
}

int8 c_game_engine_king_variant::get_uncontested_hill_bonus() const
{
	return m_uncontested_hill_bonus;
}

void c_game_engine_king_variant::set_uncontested_hill_bonus(int8 uncontested_hill_bonus)
{
	if (!VALID_INDEX(uncontested_hill_bonus + 10, 20))
	{
		event(_event_warning, "game_engine:king: invalid uncontested_hill_bonus setting '%d' outside range '[%d, %d]'!", uncontested_hill_bonus, -10, 10);

		m_uncontested_hill_bonus = 0;
	}
	else
	{
		m_uncontested_hill_bonus = uncontested_hill_bonus;
	}
}

int8 c_game_engine_king_variant::get_kill_points() const
{
	return m_kill_points;
}

void c_game_engine_king_variant::set_kill_points(int8 kill_points)
{
	if (!VALID_INDEX(kill_points + 10, 20))
	{
		event(_event_warning, "game_engine:king: invalid kill_points setting '%d' outside range '[%d, %d]'!", kill_points, -10, 10);

		m_kill_points = 0;
	}
	else
	{
		m_kill_points = kill_points;
	}
}

int8 c_game_engine_king_variant::get_inside_hill_points() const
{
	return m_inside_hill_points;
}

void c_game_engine_king_variant::set_inside_hill_points(int8 inside_hill_points)
{
	if (!VALID_INDEX(inside_hill_points + 10, 20))
	{
		event(_event_warning, "game_engine:king: invalid inside_hill_points setting '%d' outside range '[%d, %d]'!", inside_hill_points, -10, 10);

		m_inside_hill_points = 0;
	}
	else
	{
		m_inside_hill_points = inside_hill_points;
	}
}

int8 c_game_engine_king_variant::get_outside_hill_points() const
{
	return m_outside_hill_points;
}

void c_game_engine_king_variant::set_outside_hill_points(int8 outside_hill_points)
{
	if (!VALID_INDEX(outside_hill_points + 10, 20))
	{
		event(_event_warning, "game_engine:king: invalid outside_hill_points setting '%d' outside range '[%d, %d]'!", outside_hill_points, -10, 10);

		m_outside_hill_points = 0;
	}
	else
	{
		m_outside_hill_points = outside_hill_points;
	}
}

c_player_traits* c_game_engine_king_variant::get_inside_hill_traits_writeable()
{
	return &m_inside_hill_traits;
}

const c_player_traits* c_game_engine_king_variant::get_inside_hill_traits() const
{
	return &m_inside_hill_traits;
}

void c_game_engine_king_variant::set_inside_hill_traits(const c_player_traits* traits, bool force)
{
	m_inside_hill_traits.set(traits, force);
}

void c_king_engine::dump_settings(s_file_reference* file) const
{
	ASSERT(file != nullptr);

	c_game_engine::dump_settings(file);

	file_printf(file, "---------- KOTH variant settings\r\n");

	file_printf(file, "hill is%s OPAQUE\r\n", king_variant->get_opaque_hill() ? "" : " NOT");
	file_printf(file, "score to win= %d\r\n", king_variant->get_score_to_win());
	file_printf(file, "moving hill timeout= %d sec\r\n", king_variant->get_hill_move_in_seconds());

	file_printf(file, "hill move order= ");
	switch (king_variant->get_moving_hill_order())
	{
	case _king_moving_hill_order_settings_random:
		file_printf(file, "RANDOM\r\n");
		break;
	case _king_moving_hill_order_settings_sequence:
		file_printf(file, "SEQUENTIAL\r\n");
		break;
	default:
		file_printf(file, "<current option setting not exported!>\r\n");
		break;
	}

	file_printf(file, "inside hill points= %d \r\n", king_variant->get_inside_hill_points());
	file_printf(file, "outside hill points= %d\r\n", king_variant->get_outside_hill_points());
	file_printf(file, "uncontested hill bonus= %d\r\n", king_variant->get_uncontested_hill_bonus());
	file_printf(file, "kill points= %d\r\n", king_variant->get_kill_points());
	dump_player_trait_settings("inside hill traits", king_variant->get_inside_hill_traits(), file);

	file_printf(file, "---------- END KOTH variant settings\r\n");
}

