#include "game/game_engine_oddball.hpp"

#include "memory/byte_swapping.hpp"

REFERENCE_DECLARE(0x050D9F74, c_game_engine_oddball_variant*, oddball_variant);

void c_game_engine_oddball_variant::byteswap()
{
	c_game_engine_base_variant::byteswap();

	bswap_dword_inplace(m_variant_flags);
	bswap_word_inplace(m_score_to_win);
	bswap_word_inplace(m_score_unknown);
	bswap_word_inplace(m_carrying_points);
	bswap_word_inplace(m_ball_spawn_delay);
	bswap_word_inplace(m_ball_inactive_respawn_delay);
	m_carrier_traits.byteswap();

	ASSERT(array_is_zeroed(m_pad1));
}

void c_game_engine_oddball_variant::set(c_game_engine_oddball_variant const* variant, bool force)
{
	ASSERT(variant != NULL);

	c_game_engine_base_variant::set(variant, force);

	set_auto_ball_pickup(variant->get_auto_ball_pickup());
	set_ball_effect_enabled(variant->get_ball_effect_enabled());
	set_score_to_win(variant->get_score_to_win());
	set_carrying_points(variant->get_carrying_points());
	set_kill_points(variant->get_kill_points());
	set_ball_kill_points(variant->get_ball_kill_points());
	set_carrier_kill_points(variant->get_carrier_kill_points());
	set_ball_count(variant->get_ball_count());
	set_ball_spawn_delay(variant->get_ball_spawn_delay());
	set_ball_inactive_respawn_delay(variant->get_ball_inactive_respawn_delay());
	set_carrier_traits(variant->get_carrier_traits(), force);

	zero_array(m_pad1);
}

//void c_game_engine_oddball_variant::set(s_game_engine_oddball_variant_definition const* definition, bool force)
//{
//}

void c_game_engine_oddball_variant::encode_to_mcc(c_bitstream* packet) const
{
	c_game_engine_base_variant::encode_to_mcc(packet);

	bool auto_ball_pickup = get_auto_ball_pickup();
	bool ball_effect_enabled = get_ball_effect_enabled();
	short score_to_win = get_score_to_win();
	char carrying_points = get_carrying_points();
	char kill_points = get_kill_points();
	char ball_kill_points = get_ball_kill_points();
	char carrier_kill_points = get_carrier_kill_points();
	char ball_count = get_ball_count();
	short ball_spawn_delay = get_ball_spawn_delay();
	short ball_inactive_respawn_delay = get_ball_inactive_respawn_delay();

	packet->write_bool("oddball-auto-ball-pickup", auto_ball_pickup);
	packet->write_bool("oddball-ball-effect-enabled", ball_effect_enabled);
	packet->write_signed_integer("oddball-score-to-win", score_to_win, 11);
	packet->write_signed_integer("oddball-carrying-points", carrying_points, 5);
	packet->write_signed_integer("oddball-kill-points", kill_points, 5);
	packet->write_signed_integer("oddball-ball-kill-points", ball_kill_points, 5);
	packet->write_signed_integer("oddball-carrier-kill-points", carrier_kill_points, 5);
	packet->write_integer("oddball-ball-count", ball_count, 2);
	packet->write_integer("oddball-ball-spawn-delay", ball_spawn_delay, 7);
	packet->write_integer("oddball-ball-inactive-respawn-delay", ball_inactive_respawn_delay, 7);
	get_carrier_traits()->encode_to_mcc(packet);
}

void c_game_engine_oddball_variant::decode_from_mcc(c_bitstream* packet)
{
	c_game_engine_base_variant::decode_from_mcc(packet);

	bool auto_ball_pickup = packet->read_bool("oddball-auto-ball-pickup");
	bool ball_effect_enabled = packet->read_bool("oddball-ball-effect-enabled");
	short score_to_win = static_cast<short>(packet->read_signed_integer("oddball-score-to-win", 11));
	char carrying_points = static_cast<char>(packet->read_signed_integer("oddball-carrying-points", 5));
	char kill_points = static_cast<char>(packet->read_signed_integer("oddball-kill-points", 5));
	char ball_kill_points = static_cast<char>(packet->read_signed_integer("oddball-ball-kill-points", 5));
	char carrier_kill_points = static_cast<char>(packet->read_signed_integer("oddball-carrier-kill-points", 5));
	char ball_count = static_cast<char>(packet->read_integer("oddball-ball-count", 2));
	short ball_spawn_delay = static_cast<short>(packet->read_integer("oddball-ball-spawn-delay", 7));
	short ball_inactive_respawn_delay = static_cast<short>(packet->read_integer("oddball-ball-inactive-respawn-delay", 7));
	get_carrier_traits_writeable()->decode_from_mcc(packet);

	set_auto_ball_pickup(auto_ball_pickup);
	set_ball_effect_enabled(ball_effect_enabled);
	set_score_to_win(score_to_win);
	set_carrying_points(carrying_points);
	set_kill_points(kill_points);
	set_ball_kill_points(ball_kill_points);
	set_carrier_kill_points(carrier_kill_points);
	set_ball_count(ball_count);
	set_ball_spawn_delay(ball_spawn_delay);
	set_ball_inactive_respawn_delay(ball_inactive_respawn_delay);
}

bool c_game_engine_oddball_variant::get_auto_ball_pickup() const
{
	return m_variant_flags.test(_oddball_variant_flags_auto_ball_pickup);
}

void c_game_engine_oddball_variant::set_auto_ball_pickup(bool auto_ball_pickup)
{
	m_variant_flags.set(_oddball_variant_flags_auto_ball_pickup, auto_ball_pickup);
}

bool c_game_engine_oddball_variant::get_ball_effect_enabled() const
{
	return m_variant_flags.test(_oddball_variant_flags_ball_effect_enabled);
}

void c_game_engine_oddball_variant::set_ball_effect_enabled(bool ball_effect_enabled)
{
	m_variant_flags.set(_oddball_variant_flags_ball_effect_enabled, ball_effect_enabled);
}

short c_game_engine_oddball_variant::get_score_to_win() const
{
	return m_score_to_win;
}

void c_game_engine_oddball_variant::set_score_to_win(short score_to_win)
{
	if (!VALID_INDEX(score_to_win + 1, 1001))
	{
		c_console::write_line("game_engine:oddball: invalid score_to_win setting '%d' outside range '[%d, %d]'!", score_to_win, -1, 1000);

		m_score_to_win = 200;
	}
	else
	{
		m_score_to_win = score_to_win;
	}
}

char c_game_engine_oddball_variant::get_carrying_points() const
{
	return m_carrying_points;
}

void c_game_engine_oddball_variant::set_carrying_points(char carrying_points)
{
	if (!VALID_INDEX(carrying_points + 10, 20))
	{
		c_console::write_line("game_engine:oddball: invalid carrying_points setting '%d' outside range '[%d, %d]'!", carrying_points, -10, 10);

		m_carrying_points = 1;
	}
	else
	{
		m_carrying_points = carrying_points;
	}
}

char c_game_engine_oddball_variant::get_kill_points() const
{
	return m_kill_points;
}

void c_game_engine_oddball_variant::set_kill_points(char kill_points)
{
	if (!VALID_INDEX(kill_points + 10, 20))
	{
		c_console::write_line("game_engine:oddball: invalid kill_points setting '%d' outside range '[%d, %d]'!", kill_points, -10, 10);

		m_kill_points = 1;
	}
	else
	{
		m_kill_points = kill_points;
	}
}

char c_game_engine_oddball_variant::get_ball_kill_points() const
{
	return m_ball_kill_points;
}

void c_game_engine_oddball_variant::set_ball_kill_points(char ball_kill_points)
{
	if (!VALID_INDEX(ball_kill_points + 10, 20))
	{
		c_console::write_line("game_engine:oddball: invalid ball_kill_points setting '%d' outside range '[%d, %d]'!", ball_kill_points, -10, 10);

		m_ball_kill_points = 0;
	}
	else
	{
		m_ball_kill_points = ball_kill_points;
	}
}

char c_game_engine_oddball_variant::get_carrier_kill_points() const
{
	return m_carrier_kill_points;
}

void c_game_engine_oddball_variant::set_carrier_kill_points(char carrier_kill_points)
{
	if (!VALID_INDEX(carrier_kill_points + 10, 20))
	{
		c_console::write_line("game_engine:oddball: invalid carrier_kill_points setting '%d' outside range '[%d, %d]'!", carrier_kill_points, -10, 10);

		m_carrier_kill_points = 0;
	}
	else
	{
		m_carrier_kill_points = carrier_kill_points;
	}
}

char c_game_engine_oddball_variant::get_ball_count() const
{
	return m_ball_count;
}

void c_game_engine_oddball_variant::set_ball_count(char ball_count)
{
	if (!VALID_INDEX(ball_count - 1, 2))
	{
		c_console::write_line("game_engine:oddball: invalid ball_count setting '%d' outside range '[%d, %d]'!", ball_count, 1, 3);

		m_ball_count = 1;
	}
	else
	{
		m_ball_count = ball_count;
	}
}

short c_game_engine_oddball_variant::get_ball_spawn_delay() const
{
	return m_ball_spawn_delay;
}

void c_game_engine_oddball_variant::set_ball_spawn_delay(short ball_spawn_delay)
{
	if (!VALID_INDEX(ball_spawn_delay, 120))
	{
		c_console::write_line("game_engine:oddball: invalid ball_spawn_delay setting '%d' outside range '[%d, %d]'!", ball_spawn_delay, 0, 120);

		m_ball_spawn_delay = 5;
	}
	else
	{
		m_ball_spawn_delay = ball_spawn_delay;
	}
}

short c_game_engine_oddball_variant::get_ball_inactive_respawn_delay() const
{
	return m_ball_inactive_respawn_delay;
}

void c_game_engine_oddball_variant::set_ball_inactive_respawn_delay(short ball_inactive_respawn_delay)
{
	if (!VALID_INDEX(ball_inactive_respawn_delay, 120))
	{
		c_console::write_line("game_engine:oddball: invalid ball_inactive_respawn_delay setting '%d' outside range '[%d, %d]'!", ball_inactive_respawn_delay, 0, 120);

		m_ball_inactive_respawn_delay = 5;
	}
	else
	{
		m_ball_inactive_respawn_delay = ball_inactive_respawn_delay;
	}
}

c_player_traits* c_game_engine_oddball_variant::get_carrier_traits_writeable()
{
	return &m_carrier_traits;
}

c_player_traits const* c_game_engine_oddball_variant::get_carrier_traits() const
{
	return &m_carrier_traits;
}

void c_game_engine_oddball_variant::set_carrier_traits(c_player_traits const* traits, bool force)
{
	m_carrier_traits.set(traits, force);
}


void c_oddball_engine::dump_settings(s_file_reference* file) const
{
	ASSERT(file != NULL);

	c_game_engine::dump_settings(file);

	file_printf(file, "---------- ODDBALL variant settings\r\n");

	file_printf(file, "auto ball pickup= %s\r\n", oddball_variant->get_auto_ball_pickup() ? "ENABLED" : "DISABLED");
	file_printf(file, "ball effect enabled= %s\r\n", oddball_variant->get_ball_effect_enabled() ? "ENABLED" : "DISABLED");
	file_printf(file, "score to win= %d\r\n", oddball_variant->get_score_to_win());
	file_printf(file, "carrying points= %d\r\n", oddball_variant->get_carrying_points());
	file_printf(file, "kill points= %d\r\n", oddball_variant->get_kill_points());
	file_printf(file, "ball kill points= %d\r\n", oddball_variant->get_ball_kill_points());
	file_printf(file, "ball carrier kill points= %d\r\n", oddball_variant->get_carrier_kill_points());
	file_printf(file, "ball count= %d\r\n", oddball_variant->get_ball_count());
	file_printf(file, "ball spawn delay= %d seconds\r\n", oddball_variant->get_ball_spawn_delay());
	file_printf(file, "ball inactive respawn delay= %d seconds\r\n", oddball_variant->get_ball_inactive_respawn_delay());
	dump_player_trait_settings("carrier", oddball_variant->get_carrier_traits(), file);

	file_printf(file, "---------- END ODDBALL variant settings\r\n");
}

