#include "game/game_engine_vip.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/bitstream.hpp"
#include "memory/byte_swapping.hpp"

REFERENCE_DECLARE(0x050D9F88, c_game_engine_vip_variant*, vip_variant);

void c_game_engine_vip_variant::byteswap()
{
	c_game_engine_base_variant::byteswap();

	bswap_uns16_inplace(m_score_to_win_round);
	bswap_uns16_inplace(m_score_unknown);
	bswap_uns16_inplace(m_variant_flags);

	ASSERT(array_is_zeroed(m_pad1));

	bswap_uns16_inplace(m_influence_radius);
	m_vip_team_traits.byteswap();
	m_vip_influence_traits.byteswap();
	m_vip_traits.byteswap();
}

void c_game_engine_vip_variant::set(const c_game_engine_vip_variant* variant, bool force)
{
	ASSERT(variant != nullptr);

	c_game_engine_base_variant::set(variant, force);

	set_single_vip(variant->get_single_vip());
	set_destination_zones_enabled(variant->get_destination_zones_enabled());
	set_end_round_on_vip_death(variant->get_should_end_round_on_vip_death());

	set_score_to_win_round(static_cast<int16>(variant->get_score_to_win_round()));
	set_kill_points(variant->get_kill_points());
	set_takedown_points(variant->get_takedown_points());
	set_kill_as_vip_points(variant->get_kill_as_vip_points());
	set_vip_death_points(variant->get_vip_death_points());
	set_destination_arrival_points(variant->get_destination_arrival_points());
	set_suicide_points(variant->get_suicide_points());
	set_vip_suicide_points(variant->get_vip_suicide_points());
	set_betrayal_points(variant->get_betrayal_points());
	set_vip_selection(variant->get_vip_selection());
	set_zone_movement(variant->get_zone_movement());
	set_zone_order(variant->get_zone_order());
	set_influence_radius(variant->get_influence_radius());

	zero_array(m_pad1);

	set_vip_team_traits(get_vip_team_traits(), force);
	set_vip_influence_traits(get_vip_influence_traits(), force);
	set_vip_traits(get_vip_traits(), force);
}

//void set(const s_game_engine_vip_variant_definition* definition, bool force);
//{
//}

void c_game_engine_vip_variant::encode_to_mcc(c_bitstream* packet) const
{
	c_game_engine_base_variant::encode_to_mcc(packet);

	bool single_vip = get_single_vip();
	bool destination_zones_enabled = get_destination_zones_enabled();
	bool should_end_round_on_vip_death = get_should_end_round_on_vip_death();
	int32 score_to_win_round = get_score_to_win_round();
	int8 kill_points = get_kill_points();
	int8 takedown_points = get_takedown_points();
	int8 kill_as_vip_points = get_kill_as_vip_points();
	int8 vip_death_points = get_vip_death_points();
	int8 destination_arrival_points = get_destination_arrival_points();
	int8 suicide_points = get_suicide_points();
	int8 vip_suicide_points = get_vip_suicide_points();
	int8 betrayal_points = get_betrayal_points();
	e_vip_vip_selection_settings vip_selection = get_vip_selection();
	e_vip_zone_movement_settings zone_movement = get_zone_movement();
	e_vip_zone_order_settings zone_order = get_zone_order();
	int16 influence_radius = get_influence_radius();

	packet->write_bool("vip-single-vip", single_vip);
	packet->write_bool("vip-destination-zones-enabled", destination_zones_enabled);
	packet->write_bool("vip-end-round-on-vip-death", should_end_round_on_vip_death);
	packet->write_integer("vip-score-to-win-round", score_to_win_round, 10);
	packet->write_signed_integer("vip-kill-points", kill_points, 5);
	packet->write_signed_integer("vip-takedown-points", takedown_points, 5);
	packet->write_signed_integer("vip-kill-as-vip-points", kill_as_vip_points, 5);
	packet->write_signed_integer("vip-vip-death-points", vip_death_points, 5);
	packet->write_signed_integer("vip-destination-arrival-points", destination_arrival_points, 5);
	packet->write_signed_integer("vip-suicide-points", suicide_points, 5);
	packet->write_signed_integer("vip-vip-suicide-points", vip_suicide_points, 5);
	packet->write_signed_integer("vip-betrayal-points", betrayal_points, 5);
	packet->write_integer("vip-vip-selection", vip_selection, 2);
	packet->write_integer("vip-zone-movement", zone_movement, 4);
	packet->write_integer("vip-zone-order", zone_order, 1);
	packet->write_integer("vip-influence-radius", influence_radius, 6);
	get_vip_team_traits()->encode_to_mcc(packet);
	get_vip_influence_traits()->encode_to_mcc(packet);
	get_vip_traits()->encode_to_mcc(packet);
}

void c_game_engine_vip_variant::decode_from_mcc(c_bitstream* packet)
{
	c_game_engine_base_variant::decode_from_mcc(packet);

	bool single_vip = packet->read_bool("vip-single-vip");
	bool destination_zones_enabled = packet->read_bool("vip-destination-zones-enabled");
	bool should_end_round_on_vip_death = packet->read_bool("vip-end-round-on-vip-death");
	int16 score_to_win_round = static_cast<int16>(packet->read_integer("vip-score-to-win-round", 10));
	int8 kill_points = static_cast<int8>(packet->read_signed_integer("vip-kill-points", 5));
	int8 takedown_points = static_cast<int8>(packet->read_signed_integer("vip-takedown-points", 5));
	int8 kill_as_vip_points = static_cast<int8>(packet->read_signed_integer("vip-kill-as-vip-points", 5));
	int8 vip_death_points = static_cast<int8>(packet->read_signed_integer("vip-vip-death-points", 5));
	int8 destination_arrival_points = static_cast<int8>(packet->read_signed_integer("vip-destination-arrival-points", 5));
	int8 suicide_points = static_cast<int8>(packet->read_signed_integer("vip-suicide-points", 5));
	int8 vip_suicide_points = static_cast<int8>(packet->read_signed_integer("vip-vip-suicide-points", 5));
	int8 betrayal_points = static_cast<int8>(packet->read_signed_integer("vip-betrayal-points", 5));
	e_vip_vip_selection_settings vip_selection = packet->read_enum<e_vip_vip_selection_settings, 2>("vip-vip-selection");
	e_vip_zone_movement_settings zone_movement = packet->read_enum<e_vip_zone_movement_settings, 4>("vip-zone-movement");
	e_vip_zone_order_settings zone_order = packet->read_enum<e_vip_zone_order_settings, 1>("vip-zone-order");
	int16 influence_radius = static_cast<int16>(packet->read_integer("vip-influence-radius", 6));
	get_vip_team_traits_writeable()->decode_from_mcc(packet);
	get_vip_influence_traits_writeable()->decode_from_mcc(packet);
	get_vip_traits_writeable()->decode_from_mcc(packet);

	set_single_vip(single_vip);
	set_destination_zones_enabled(destination_zones_enabled);
	set_end_round_on_vip_death(should_end_round_on_vip_death);
	set_score_to_win_round(score_to_win_round);
	set_kill_points(kill_points);
	set_takedown_points(takedown_points);
	set_kill_as_vip_points(kill_as_vip_points);
	set_vip_death_points(vip_death_points);
	set_destination_arrival_points(destination_arrival_points);
	set_suicide_points(suicide_points);
	set_vip_suicide_points(vip_suicide_points);
	set_betrayal_points(betrayal_points);
	set_vip_selection(vip_selection);
	set_zone_movement(zone_movement);
	set_zone_order(zone_order);
	set_influence_radius(influence_radius);
}

bool c_game_engine_vip_variant::get_single_vip() const
{
	return m_variant_flags.test(_vip_variant_flags_single_vip);
}

void c_game_engine_vip_variant::set_single_vip(bool single_vip)
{
	m_variant_flags.set(_vip_variant_flags_single_vip, single_vip);
}

bool c_game_engine_vip_variant::get_destination_zones_enabled() const
{
	return m_variant_flags.test(_vip_variant_flags_destination_zones_enabled);
}

void c_game_engine_vip_variant::set_destination_zones_enabled(bool destination_zones_enabled)
{
	m_variant_flags.set(_vip_variant_flags_destination_zones_enabled, destination_zones_enabled);
}

bool c_game_engine_vip_variant::get_should_end_round_on_vip_death() const
{
	return m_variant_flags.test(_vip_variant_flags_end_round_on_vip_death);
}

void c_game_engine_vip_variant::set_end_round_on_vip_death(bool end_round_on_vip_death)
{
	m_variant_flags.set(_vip_variant_flags_end_round_on_vip_death, end_round_on_vip_death);
}

void c_game_engine_vip_variant::set_score_to_win_round(int16 score_to_win_round)
{
	if (!VALID_INDEX(score_to_win_round, 500))
	{
		event(_event_warning, "game_engine:vip: invalid score_to_win_round setting '%d' outside range '[%d, %d]'!", score_to_win_round, 0, 500);

		m_score_to_win_round = 15;
	}
	else
	{
		m_score_to_win_round = score_to_win_round;
	}
}

int8 c_game_engine_vip_variant::get_kill_points() const
{
	return m_kill_points;
}

void c_game_engine_vip_variant::set_kill_points(int8 kill_points)
{
	if (!VALID_INDEX(kill_points + 10, 20))
	{
		event(_event_warning, "game_engine:vip: invalid kill_points setting '%d' outside range '[%d, %d]'!", kill_points, -10, 10);

		m_kill_points = 0;
	}
	else
	{
		m_kill_points = kill_points;
	}
}

int8 c_game_engine_vip_variant::get_takedown_points() const
{
	return m_takedown_points;
}

void c_game_engine_vip_variant::set_takedown_points(int8 takedown_points)
{
	if (!VALID_INDEX(takedown_points + 10, 20))
	{
		event(_event_warning, "game_engine:vip: invalid takedown_points setting '%d' outside range '[%d, %d]'!", takedown_points, -10, 10);

		m_takedown_points = 0;
	}
	else
	{
		m_takedown_points = takedown_points;
	}
}

int8 c_game_engine_vip_variant::get_kill_as_vip_points() const
{
	return m_kill_as_vip_points;
}

void c_game_engine_vip_variant::set_kill_as_vip_points(int8 kill_as_vip_points)
{
	if (!VALID_INDEX(kill_as_vip_points + 10, 20))
	{
		event(_event_warning, "game_engine:vip: invalid kill_as_vip_points setting '%d' outside range '[%d, %d]'!", kill_as_vip_points, -10, 10);

		m_kill_as_vip_points = 0;
	}
	else
	{
		m_kill_as_vip_points = kill_as_vip_points;
	}
}

int8 c_game_engine_vip_variant::get_vip_death_points() const
{
	return m_vip_death_points;
}

void c_game_engine_vip_variant::set_vip_death_points(int8 vip_death_points)
{
	if (!VALID_INDEX(vip_death_points + 10, 20))
	{
		event(_event_warning, "game_engine:vip: invalid vip_death_points setting '%d' outside range '[%d, %d]'!", vip_death_points, -10, 10);

		m_vip_death_points = 0;
	}
	else
	{
		m_vip_death_points = vip_death_points;
	}
}

int8 c_game_engine_vip_variant::get_destination_arrival_points() const
{
	return m_destination_arrival_points;
}

void c_game_engine_vip_variant::set_destination_arrival_points(int8 destination_arrival_points)
{
	if (!VALID_INDEX(destination_arrival_points + 10, 20))
	{
		event(_event_warning, "game_engine:vip: invalid destination_arrival_points setting '%d' outside range '[%d, %d]'!", destination_arrival_points, -10, 10);

		m_destination_arrival_points = 0;
	}
	else
	{
		m_destination_arrival_points = destination_arrival_points;
	}
}

int8 c_game_engine_vip_variant::get_suicide_points() const
{
	return m_suicide_points;
}

void c_game_engine_vip_variant::set_suicide_points(int8 suicide_points)
{
	if (!VALID_INDEX(suicide_points + 10, 20))
	{
		event(_event_warning, "game_engine:vip: invalid suicide_points setting '%d' outside range '[%d, %d]'!", suicide_points, -10, 10);

		m_suicide_points = 0;
	}
	else
	{
		m_suicide_points = suicide_points;
	}
}

int8 c_game_engine_vip_variant::get_betrayal_points() const
{
	return m_betrayal_points;
}

void c_game_engine_vip_variant::set_betrayal_points(int8 betrayal_points)
{
	if (!VALID_INDEX(betrayal_points + 10, 20))
	{
		event(_event_warning, "game_engine:vip: invalid betrayal_points setting '%d' outside range '[%d, %d]'!", betrayal_points, -10, 10);

		m_betrayal_points = 0;
	}
	else
	{
		m_betrayal_points = betrayal_points;
	}
}

int8 c_game_engine_vip_variant::get_vip_suicide_points() const
{
	return m_vip_suicide_points;
}

void c_game_engine_vip_variant::set_vip_suicide_points(int8 vip_suicide_points)
{
	if (!VALID_INDEX(vip_suicide_points + 10, 20))
	{
		event(_event_warning, "game_engine:vip: invalid vip_suicide_points setting '%d' outside range '[%d, %d]'!", vip_suicide_points, -10, 10);

		m_vip_suicide_points = 0;
	}
	else
	{
		m_vip_suicide_points = vip_suicide_points;
	}
}

e_vip_vip_selection_settings c_game_engine_vip_variant::get_vip_selection() const
{
	return m_vip_selection;
}

void c_game_engine_vip_variant::set_vip_selection(e_vip_vip_selection_settings vip_selection)
{
	if (!VALID_INDEX(vip_selection, k_vip_vip_selection_settings))
	{
		event(_event_warning, "game_engine:vip: invalid vip_selection setting '%d' outside range '[0, %d)'!", vip_selection, k_vip_vip_selection_settings);

		m_vip_selection = k_vip_vip_selection_settings_default;
	}
	else
	{
		m_vip_selection = vip_selection;
	}
}

e_vip_zone_movement_settings c_game_engine_vip_variant::get_zone_movement() const
{
	return m_zone_movement;
}

int32 c_game_engine_vip_variant::get_zone_movement_time_in_seconds() const
{
	//ASSERT(VALID_INDEX(m_zone_movement, k_number_of_vip_zone_movement_settings));
	ASSERT(VALID_INDEX(get_zone_movement(), k_number_of_vip_zone_movement_settings));

	return k_vip_zone_movement_values[get_zone_movement()];
}

void c_game_engine_vip_variant::set_zone_movement(e_vip_zone_movement_settings zone_movement)
{
	if (!VALID_INDEX(zone_movement, k_number_of_vip_zone_movement_settings))
	{
		event(_event_warning, "game_engine:vip: invalid zone_movement setting '%d' outside range '[0, %d)'!", zone_movement, k_number_of_vip_zone_movement_settings);

		m_zone_movement = k_vip_zone_movement_settings_default;
	}
	else
	{
		m_zone_movement = zone_movement;
	}
}

e_vip_zone_order_settings c_game_engine_vip_variant::get_zone_order() const
{
	return m_zone_order;
}

void c_game_engine_vip_variant::set_zone_order(e_vip_zone_order_settings zone_order)
{
	if (!VALID_INDEX(zone_order, k_vip_zone_order_settings))
	{
		event(_event_warning, "game_engine:vip: invalid zone_order setting '%d' outside range '[0, %d)'!", zone_order, k_vip_zone_order_settings);

		m_zone_order = k_vip_zone_order_settings_default;
	}
	else
	{
		m_zone_order = zone_order;
	}
}

int16 c_game_engine_vip_variant::get_influence_radius() const
{
	return m_influence_radius;
}

void c_game_engine_vip_variant::set_influence_radius(int16 influence_radius)
{
	if (!VALID_INDEX(influence_radius, 50))
	{
		event(_event_warning, "game_engine:vip: invalid influence_radius setting '%d' outside range '[%d, %d]'!", influence_radius, 0, 50);

		m_influence_radius = 0;
	}
	else
	{
		m_influence_radius = influence_radius;
	}
}

c_player_traits* c_game_engine_vip_variant::get_vip_team_traits_writeable()
{
	return &m_vip_team_traits;
}

const c_player_traits* c_game_engine_vip_variant::get_vip_team_traits() const
{
	return &m_vip_team_traits;
}

void c_game_engine_vip_variant::set_vip_team_traits(const c_player_traits* traits, bool force)
{
	m_vip_team_traits.set(traits, force);
}

c_player_traits* c_game_engine_vip_variant::get_vip_influence_traits_writeable()
{
	return &m_vip_influence_traits;
}

const c_player_traits* c_game_engine_vip_variant::get_vip_influence_traits() const
{
	return &m_vip_influence_traits;
}

void c_game_engine_vip_variant::set_vip_influence_traits(const c_player_traits* traits, bool force)
{
	m_vip_influence_traits.set(traits, force);
}

c_player_traits* c_game_engine_vip_variant::get_vip_traits_writeable()
{
	return &m_vip_traits;
}

const c_player_traits* c_game_engine_vip_variant::get_vip_traits() const
{
	return &m_vip_traits;
}

void c_game_engine_vip_variant::set_vip_traits(const c_player_traits* traits, bool force)
{
	m_vip_traits.set(traits, force);
}

void c_vip_engine::dump_settings(s_file_reference* file) const
{
	ASSERT(file != nullptr);

	c_game_engine::dump_settings(file);

	file_printf(file, "---------- VIP variant settings\r\n");

	file_printf(file, "single vip= %s\r\n", vip_variant->get_single_vip());
	file_printf(file, "destination zones enabled= %s\r\n", vip_variant->get_destination_zones_enabled());
	file_printf(file, "score to win round= %d\r\n", vip_variant->get_score_to_win_round());
	file_printf(file, "kill points= %d\r\n", vip_variant->get_kill_points());
	file_printf(file, "takedown points= %d\r\n", vip_variant->get_takedown_points());
	file_printf(file, "kill as vip points= %d\r\n", vip_variant->get_kill_as_vip_points());
	file_printf(file, "vip death points= %d\r\n", vip_variant->get_vip_death_points());
	file_printf(file, "destination arrival points= %d\r\n", vip_variant->get_destination_arrival_points());
	file_printf(file, "suicide points= %d\r\n", vip_variant->get_suicide_points());
	file_printf(file, "betrayal points= %d\r\n", vip_variant->get_betrayal_points());

	file_printf(file, "vip selection= ");
	switch (vip_variant->get_vip_selection())
	{
	case _vip_vip_selection_settings_random:
		file_printf(file, "RANDOM\r\n");
		break;
	case _vip_vip_selection_settings_next_respawn:
		file_printf(file, "NEXT_RESPAWN\r\n");
		break;
	case _vip_vip_selection_settings_next_kill:
		file_printf(file, "NEXT_KILL\r\n");
		break;
	case _vip_vip_selection_settings_unchanged:
		file_printf(file, "UNCHANGED\r\n");
		break;
	}

	file_printf(file, "zone movement= ");
	switch (vip_variant->get_zone_movement())
	{
	case _vip_zone_movement_settings_off:
		file_printf(file, "OFF\r\n");
		break;
	case _vip_zone_movement_settings_on_arrival:
		file_printf(file, "ON ARRIVAL\r\n");
		break;
	case _vip_zone_movement_settings_on_switch:
		file_printf(file, "ON VIP SWITCH\r\n");
		break;
	default:
		file_printf(file, "%d seconds\r\n", vip_variant->get_zone_movement_time_in_seconds());
		break;
	}

	file_printf(file, "zone order= %s\r\n", !vip_variant->get_zone_order() ? "RANDOM" : "SEQUENCE");

	dump_player_trait_settings("vip team traits", vip_variant->get_vip_team_traits(), file);
	dump_player_trait_settings("vip influence traits", vip_variant->get_vip_influence_traits(), file);
	dump_player_trait_settings("vip traits", vip_variant->get_vip_traits(), file);

	file_printf(file, "---------- END VIP variant settings\r\n");
}

