#include "game/game_engine_slayer.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "memory/bitstream.hpp"
#include "memory/byte_swapping.hpp"

REFERENCE_DECLARE(0x050D9F70, c_game_engine_slayer_variant*, slayer_variant);

void c_game_engine_slayer_variant::byteswap()
{
	c_game_engine_base_variant::byteswap();

	bswap_uns16_inplace(m_score_to_win);
	bswap_uns16_inplace(m_score_unknown);
	bswap_uns16_inplace(m_kill_points);
	m_leader_traits.byteswap();

	ASSERT(array_is_zeroed(m_pad1));
}

void c_game_engine_slayer_variant::set(const c_game_engine_slayer_variant* variant, bool force)
{
	ASSERT(variant != nullptr);

	c_game_engine_base_variant::set(variant, force);

	set_score_to_win(variant->get_score_to_win());
	set_kill_points(variant->get_kill_points());
	set_assist_points(variant->get_assist_points());
	set_death_points(variant->get_death_points());
	set_suicide_points(variant->get_suicide_points());
	set_betrayal_points(variant->get_betrayal_points());
	set_leader_killed_points(variant->get_leader_killed_points());
	set_elimination_points(variant->get_elimination_points());
	set_assassination_points(variant->get_assassination_points());
	set_headshot_points(variant->get_headshot_points());
	set_melee_points(variant->get_melee_points());
	set_sticky_points(variant->get_sticky_points());
	set_splatter_points(variant->get_splatter_points());
	set_killing_spree_points(variant->get_killing_spree_points());
	set_leader_traits(get_leader_traits(), force);
}

//void c_game_engine_slayer_variant::set(const s_game_engine_slayer_variant_definition* definition, bool force)
//{
//}

void c_game_engine_slayer_variant::encode_to_mcc(c_bitstream* packet) const
{
	c_game_engine_base_variant::encode_to_mcc(packet);

	int16 score_to_win = get_score_to_win();
	int8 kill_points = get_kill_points();
	int8 assist_points = get_assist_points();
	int8 death_points = get_death_points();
	int8 suicide_points = get_suicide_points();
	int8 betrayal_points = get_betrayal_points();
	int8 leader_killed_points = get_leader_killed_points();
	int8 elimination_points = get_elimination_points();
	int8 assassination_points = get_assassination_points();
	int8 headshot_points = get_headshot_points();
	int8 melee_points = get_melee_points();
	int8 sticky_points = get_sticky_points();
	int8 splatter_points = get_splatter_points();
	int8 killing_spree_points = get_killing_spree_points();

	packet->write_signed_integer("slayer-score-to-win", score_to_win, 10);
	packet->write_signed_integer("slayer-kill-points", kill_points, 5);
	packet->write_signed_integer("slayer-assist-points", assist_points, 5);
	packet->write_signed_integer("slayer-death-points", death_points, 5);
	packet->write_signed_integer("slayer-suicide-points", suicide_points, 5);
	packet->write_signed_integer("slayer-betrayal-points", betrayal_points, 5);
	packet->write_signed_integer("slayer-leader-killed-points", leader_killed_points, 5);
	packet->write_signed_integer("slayer-elimination-points", elimination_points, 5);
	packet->write_signed_integer("slayer-assassination-points", assassination_points, 5);
	packet->write_signed_integer("slayer-headshot-points", headshot_points, 5);
	packet->write_signed_integer("slayer-melee-points", melee_points, 5);
	packet->write_signed_integer("slayer-sticky-points", sticky_points, 5);
	packet->write_signed_integer("slayer-splatter-points", splatter_points, 5);
	packet->write_signed_integer("slayer-killing-spree-points", killing_spree_points, 5);
	get_leader_traits()->encode_to_mcc(packet);
}

void c_game_engine_slayer_variant::decode_from_mcc(c_bitstream* packet)
{
	c_game_engine_base_variant::decode_from_mcc(packet);

	int16 score_to_win = static_cast<int16>(packet->read_signed_integer("slayer-score-to-win", 10));
	int8 kill_points = static_cast<int8>(packet->read_signed_integer("slayer-kill-points", 5));
	int8 assist_points = static_cast<int8>(packet->read_signed_integer("slayer-assist-points", 5));
	int8 death_points = static_cast<int8>(packet->read_signed_integer("slayer-death-points", 5));
	int8 suicide_points = static_cast<int8>(packet->read_signed_integer("slayer-suicide-points", 5));
	int8 betrayal_points = static_cast<int8>(packet->read_signed_integer("slayer-betrayal-points", 5));
	int8 leader_killed_points = static_cast<int8>(packet->read_signed_integer("slayer-leader-killed-points", 5));
	int8 elimination_points = static_cast<int8>(packet->read_signed_integer("slayer-elimination-points", 5));
	int8 assassination_points = static_cast<int8>(packet->read_signed_integer("slayer-assassination-points", 5));
	int8 headshot_points = static_cast<int8>(packet->read_signed_integer("slayer-headshot-points", 5));
	int8 melee_points = static_cast<int8>(packet->read_signed_integer("slayer-melee-points", 5));
	int8 sticky_points = static_cast<int8>(packet->read_signed_integer("slayer-sticky-points", 5));
	int8 splatter_points = static_cast<int8>(packet->read_signed_integer("slayer-splatter-points", 5));
	int8 killing_spree_points = static_cast<int8>(packet->read_signed_integer("slayer-killing-spree-points", 5));
	get_leader_traits_writeable()->decode_from_mcc(packet);

	set_score_to_win(score_to_win);
	set_kill_points(kill_points);
	set_assist_points(assist_points);
	set_death_points(death_points);
	set_suicide_points(suicide_points);
	set_betrayal_points(betrayal_points);
	set_leader_killed_points(leader_killed_points);
	set_elimination_points(elimination_points);
	set_assassination_points(assassination_points);
	set_headshot_points(headshot_points);
	set_melee_points(melee_points);
	set_sticky_points(sticky_points);
	set_splatter_points(splatter_points);
	set_killing_spree_points(killing_spree_points);
}

int16 c_game_engine_slayer_variant::get_score_to_win() const
{
	return m_score_to_win;
}

void c_game_engine_slayer_variant::set_score_to_win(int16 score_to_win)
{
	if (!VALID_INDEX(score_to_win + 1, 501))
	{
		event(_event_warning, "game_engine:slayer: invalid score_to_win setting '%d' outside range '[%d, %d]'!", score_to_win, -1, 520);

		m_score_to_win = 25;
	}
	else
	{
		m_score_to_win = score_to_win;
	}
}

int8 c_game_engine_slayer_variant::get_kill_points() const
{
	return m_kill_points;
}

void c_game_engine_slayer_variant::set_kill_points(int8 kill_points)
{
	if (!VALID_INDEX(kill_points + 10, 20))
	{
		event(_event_warning, "game_engine:slayer: invalid kill_points setting '%d' outside range '[%d, %d]'!", kill_points, -10, 10);

		m_kill_points = 1;
	}
	else
	{
		m_kill_points = kill_points;
	}
}

int8 c_game_engine_slayer_variant::get_assist_points() const
{
	return m_assist_points;
}

void c_game_engine_slayer_variant::set_assist_points(int8 assist_points)
{
	if (!VALID_INDEX(assist_points + 10, 20))
	{
		event(_event_warning, "game_engine:slayer: invalid assist_points setting '%d' outside range '[%d, %d]'!", assist_points, -10, 10);

		m_assist_points = 0;
	}
	else
	{
		m_assist_points = assist_points;
	}
}

int8 c_game_engine_slayer_variant::get_death_points() const
{
	return m_death_points;
}

void c_game_engine_slayer_variant::set_death_points(int8 death_points)
{
	if (!VALID_INDEX(death_points + 10, 20))
	{
		event(_event_warning, "game_engine:slayer: invalid death_points setting '%d' outside range '[%d, %d]'!", death_points, -10, 10);

		m_death_points = 0;
	}
	else
	{
		m_death_points = death_points;
	}
}

int8 c_game_engine_slayer_variant::get_suicide_points() const
{
	return m_suicide_points;
}

void c_game_engine_slayer_variant::set_suicide_points(int8 suicide_points)
{
	if (!VALID_INDEX(suicide_points + 10, 20))
	{
		event(_event_warning, "game_engine:slayer: invalid suicide_points setting '%d' outside range '[%d, %d]'!", suicide_points, -10, 10);

		m_suicide_points = -1;
	}
	else
	{
		m_suicide_points = suicide_points;
	}
}

int8 c_game_engine_slayer_variant::get_betrayal_points() const
{
	return m_betrayal_points;
}

void c_game_engine_slayer_variant::set_betrayal_points(int8 betrayal_points)
{
	if (!VALID_INDEX(betrayal_points + 10, 20))
	{
		event(_event_warning, "game_engine:slayer: invalid betrayal_points setting '%d' outside range '[%d, %d]'!", betrayal_points, -10, 10);

		m_betrayal_points = -1;
	}
	else
	{
		m_betrayal_points = betrayal_points;
	}
}

int8 c_game_engine_slayer_variant::get_leader_killed_points() const
{
	return m_leader_killed_points;
}

void c_game_engine_slayer_variant::set_leader_killed_points(int8 leader_killed_points)
{
	if (!VALID_INDEX(leader_killed_points + 10, 20))
	{
		event(_event_warning, "game_engine:slayer: invalid leader_killed_points setting '%d' outside range '[%d, %d]'!", leader_killed_points, -10, 10);

		m_leader_killed_points = 0;
	}
	else
	{
		m_leader_killed_points = leader_killed_points;
	}
}

int8 c_game_engine_slayer_variant::get_elimination_points() const
{
	return m_elimination_points;
}

void c_game_engine_slayer_variant::set_elimination_points(int8 elimination_points)
{
	if (!VALID_INDEX(elimination_points + 10, 20))
	{
		event(_event_warning, "game_engine:slayer: invalid elimination_points setting '%d' outside range '[%d, %d]'!", elimination_points, -10, 10);

		m_elimination_points = 0;
	}
	else
	{
		m_elimination_points = elimination_points;
	}
}

int8 c_game_engine_slayer_variant::get_assassination_points() const
{
	return m_assassination_points;
}

void c_game_engine_slayer_variant::set_assassination_points(int8 assassination_points)
{
	if (!VALID_INDEX(assassination_points + 10, 20))
	{
		event(_event_warning, "game_engine:slayer: invalid assassination_points setting '%d' outside range '[%d, %d]'!", assassination_points, -10, 10);

		m_assassination_points = 0;
	}
	else
	{
		m_assassination_points = assassination_points;
	}
}

int8 c_game_engine_slayer_variant::get_headshot_points() const
{
	return m_headshot_points;
}

void c_game_engine_slayer_variant::set_headshot_points(int8 headshot_points)
{
	if (!VALID_INDEX(headshot_points + 10, 20))
	{
		event(_event_warning, "game_engine:slayer: invalid headshot_points setting '%d' outside range '[%d, %d]'!", headshot_points, -10, 10);

		m_headshot_points = 0;
	}
	else
	{
		m_headshot_points = headshot_points;
	}
}

int8 c_game_engine_slayer_variant::get_melee_points() const
{
	return m_melee_points;
}

void c_game_engine_slayer_variant::set_melee_points(int8 melee_points)
{
	if (!VALID_INDEX(melee_points + 10, 20))
	{
		event(_event_warning, "game_engine:slayer: invalid melee_points setting '%d' outside range '[%d, %d]'!", melee_points, -10, 10);

		m_melee_points = 0;
	}
	else
	{
		m_melee_points = melee_points;
	}
}

int8 c_game_engine_slayer_variant::get_sticky_points() const
{
	return m_sticky_points;
}

void c_game_engine_slayer_variant::set_sticky_points(int8 sticky_points)
{
	if (!VALID_INDEX(sticky_points + 10, 20))
	{
		event(_event_warning, "game_engine:slayer: invalid sticky_points setting '%d' outside range '[%d, %d]'!", sticky_points, -10, 10);

		m_sticky_points = 0;
	}
	else
	{
		m_sticky_points = sticky_points;
	}
}

int8 c_game_engine_slayer_variant::get_splatter_points() const
{
	return m_splatter_points;
}

void c_game_engine_slayer_variant::set_splatter_points(int8 splatter_points)
{
	if (!VALID_INDEX(splatter_points + 10, 20))
	{
		event(_event_warning, "game_engine:slayer: invalid splatter_points setting '%d' outside range '[%d, %d]'!", splatter_points, -10, 10);

		m_splatter_points = 0;
	}
	else
	{
		m_splatter_points = splatter_points;
	}
}

int8 c_game_engine_slayer_variant::get_killing_spree_points() const
{
	return m_killing_spree_points;
}

void c_game_engine_slayer_variant::set_killing_spree_points(int8 killing_spree_points)
{
	if (!VALID_INDEX(killing_spree_points + 10, 20))
	{
		event(_event_warning, "game_engine:slayer: invalid killing_spree_points setting '%d' outside range '[%d, %d]'!", killing_spree_points, -10, 10);

		m_killing_spree_points = 0;
	}
	else
	{
		m_killing_spree_points = killing_spree_points;
	}
}

c_player_traits* c_game_engine_slayer_variant::get_leader_traits_writeable()
{
	return &m_leader_traits;
}

const c_player_traits* c_game_engine_slayer_variant::get_leader_traits() const
{
	return &m_leader_traits;
}

void c_game_engine_slayer_variant::set_leader_traits(const c_player_traits* traits, bool force)
{
	m_leader_traits.set(traits, force);
}

void c_slayer_engine::dump_settings(s_file_reference* file) const
{
	ASSERT(file != nullptr);

	c_game_engine::dump_settings(file);

	file_printf(file, "---------- SLAYER variant settings\r\n");

	file_printf(file, "score to win= %d\r\n", slayer_variant->get_score_to_win());
	file_printf(file, "kill points= %d\r\n", slayer_variant->get_kill_points());
	file_printf(file, "assist points= %d\r\n", slayer_variant->get_assist_points());
	file_printf(file, "death points= %d\r\n", slayer_variant->get_death_points());
	file_printf(file, "betrayal points= %d\r\n", slayer_variant->get_betrayal_points());
	file_printf(file, "leader kill points= %d\r\n", slayer_variant->get_leader_killed_points());
	file_printf(file, "elimination points= %d\r\n", slayer_variant->get_elimination_points());
	file_printf(file, "assassination points= %d\r\n", slayer_variant->get_assassination_points());
	file_printf(file, "headshot points= %d\r\n", slayer_variant->get_headshot_points());
	file_printf(file, "melee points= %d\r\n", slayer_variant->get_melee_points());
	file_printf(file, "sticky points= %d\r\n", slayer_variant->get_sticky_points());
	file_printf(file, "splatter points= %d\r\n", slayer_variant->get_splatter_points());
	file_printf(file, "killing spree points= %d\r\n", slayer_variant->get_killing_spree_points());
	dump_player_trait_settings("leader", slayer_variant->get_leader_traits(), file);

	file_printf(file, "---------- END SLAYER variant settings\r\n");
}

