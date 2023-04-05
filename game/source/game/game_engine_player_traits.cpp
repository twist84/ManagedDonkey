#include "game/game_engine_player_traits.hpp"

#include "cseries/console.hpp"
#include "memory/byte_swapping.hpp"

void c_player_traits::byteswap()
{
	bswap_word_inplace(m_weapon_traits.m_initial_grenade_count);
	bswap_word_inplace(m_sensor_traits.m_motion_tracker_setting);
	bswap_word_inplace(m_sensor_traits.m_motion_tracker_range_setting);
}

void c_player_traits::set(c_player_traits const* traits, bool force)
{
	if (traits)
	{
		set_shield_vitality_traits(traits->get_shield_vitality_traits(), force);
	}
	else
	{
		get_shield_vitality_traits_writeable()->set_damage_resistance_percentage_setting(_damage_resistance_percentage_setting_unchanged, force);
		get_shield_vitality_traits_writeable()->set_shield_recharge_rate_percentage_setting(_shield_recharge_rate_percentage_setting_unchanged, force);
		get_shield_vitality_traits_writeable()->set_vampirism_percentage_setting(_vampirism_percentage_setting_unchanged, force);
		get_shield_vitality_traits_writeable()->set_headshot_immunity_setting(_headshot_immunity_setting_unchanged, force);
		get_shield_vitality_traits_writeable()->set_shield_multiplier_setting(_shield_multiplier_setting_unchanged, force);
	}
}

c_player_trait_shield_vitality* c_player_traits::get_shield_vitality_traits_writeable()
{
	return &m_shield_vitality_traits;
}

c_player_trait_shield_vitality const* c_player_traits::get_shield_vitality_traits() const
{
	return &m_shield_vitality_traits;
}

void c_player_traits::set_shield_vitality_traits(c_player_trait_shield_vitality const* traits, bool force)
{
	m_shield_vitality_traits.set(traits, force);
}

c_player_trait_weapons* c_player_traits::get_weapons_traits_writeable()
{
	return &m_weapon_traits;
}

c_player_trait_weapons const* c_player_traits::get_weapons_traits() const
{
	return &m_weapon_traits;
}

void c_player_traits::set_weapons_traits(c_player_trait_weapons const* traits, bool force)
{
	//m_weapon_traits.set(traits, force);
}

c_player_trait_movement* c_player_traits::get_movement_traits_writeable()
{
	return &m_movement_traits;
}

c_player_trait_movement const* c_player_traits::get_movement_traits() const
{
	return &m_movement_traits;
}

void c_player_traits::set_movement_traits(c_player_trait_movement const* traits, bool force)
{
	//m_movement_traits.set(traits, force);
}

c_player_trait_appearance* c_player_traits::get_appearance_traits_writeable()
{
	return &m_appearance_traits;
}

c_player_trait_appearance const* c_player_traits::get_appearance_traits() const
{
	return &m_appearance_traits;
}

void c_player_traits::set_appearance_traits(c_player_trait_appearance const* traits, bool force)
{
	//m_appearance_traits.set(traits, force);
}

c_player_trait_sensors* c_player_traits::get_sensor_traits_writeable()
{
	return &m_sensor_traits;
}

c_player_trait_sensors const* c_player_traits::get_sensor_traits() const
{
	return &m_sensor_traits;
}

void c_player_traits::set_sensor_traits(c_player_trait_sensors const* traits, bool force)
{
	//m_sensor_traits.set(traits, force);
}

void c_player_trait_shield_vitality::set(c_player_trait_shield_vitality const* traits, bool force)
{
	ASSERT(traits != NULL);

	set_damage_resistance_percentage_setting(traits->get_damage_resistance_percentage_setting(), force);
	set_shield_recharge_rate_percentage_setting(traits->get_shield_recharge_rate_percentage_setting(), force);
	set_vampirism_percentage_setting(traits->get_vampirism_percentage_setting(), force);
	set_shield_multiplier_setting(traits->get_shield_multiplier_setting(), force);
	set_headshot_immunity_setting(traits->get_headshot_immunity_setting(), force);
}

e_damage_resistance_percentage_setting c_player_trait_shield_vitality::get_damage_resistance_percentage_setting() const
{
	return m_damage_resistance_percentage_setting;
}

void c_player_trait_shield_vitality::set_damage_resistance_percentage_setting(e_damage_resistance_percentage_setting damage_resistance_percentage_setting, bool force)
{
	if (!VALID_INDEX(damage_resistance_percentage_setting, k_damage_resistance_percentage_settings))
	{
		c_console::write_line("game_engine:player_trait:shield_vitality: invalid damage resistance value '%ld'!", damage_resistance_percentage_setting);

		if (force)
			m_shield_recharge_rate_percentage_setting = _damage_resistance_percentage_setting_unchanged;
	}
	else if (damage_resistance_percentage_setting || force)
	{
		m_damage_resistance_percentage_setting = damage_resistance_percentage_setting;
	}
}

real c_player_trait_shield_vitality::get_damage_resistance_percentage() const
{
	switch (get_damage_resistance_percentage_setting())
	{
	case _damage_resistance_percentage_setting_10_percent:
		return 0.1f;
	case _damage_resistance_percentage_setting_50_percent:
		return 0.5f;
	case _damage_resistance_percentage_setting_90_percent:
		return 0.9f;
	case _damage_resistance_percentage_setting_100_percent:
		return 1.0f;
	case _damage_resistance_percentage_setting_110_percent:
		return 1.1f;
	case _damage_resistance_percentage_setting_150_percent:
		return 1.5f;
	case _damage_resistance_percentage_setting_200_percent:
		return 2.0f;
	case _damage_resistance_percentage_setting_300_percent:
		return 3.0f;
	case _damage_resistance_percentage_setting_500_percent:
		return 5.0f;
	case _damage_resistance_percentage_setting_1000_percent:
		return 10.0f;
	case _damage_resistance_percentage_setting_2000_percent:
		return 20.0f;
	case _damage_resistance_percentage_setting_invulnerable:
		return 1000.0f;
	}
	return 1.0f;
}

void c_player_trait_shield_vitality::set_damage_resistance_percentage(real damage_resistance_percentage, bool force)
{
	// #TODO: implement
}

e_shield_recharge_rate_percentage_setting c_player_trait_shield_vitality::get_shield_recharge_rate_percentage_setting() const
{
	return m_shield_recharge_rate_percentage_setting;
}

void c_player_trait_shield_vitality::set_shield_recharge_rate_percentage_setting(e_shield_recharge_rate_percentage_setting shield_recharge_rate_percentage_setting, bool force)
{
	if (!VALID_INDEX(shield_recharge_rate_percentage_setting, k_shield_recharge_rate_percentage_settings))
	{
		c_console::write_line("game_engine:player_trait:shield_vitality: invalid shield recharge rate value '%ld'!", shield_recharge_rate_percentage_setting);

		if (force)
			m_shield_recharge_rate_percentage_setting = _shield_recharge_rate_percentage_setting_unchanged;
	}
	else if (shield_recharge_rate_percentage_setting || force)
	{
		m_shield_recharge_rate_percentage_setting = shield_recharge_rate_percentage_setting;
	}
}

real c_player_trait_shield_vitality::get_shield_recharge_rate_percentage() const
{
	switch (get_shield_recharge_rate_percentage_setting())
	{
	case _shield_recharge_rate_percentage_setting_negative_25_percent:
		return -0.25f;
	case _shield_recharge_rate_percentage_setting_negative_10_percent:
		return -0.1f;
	case _shield_recharge_rate_percentage_setting_negative_5_percent:
		return -0.05f;
	case _shield_recharge_rate_percentage_setting_0_percent:
		return 0.0f;
	case _shield_recharge_rate_percentage_setting_50_percent:
		return 0.5f;
	case _shield_recharge_rate_percentage_setting_90_percent:
		return 0.9f;
	case _shield_recharge_rate_percentage_setting_100_percent:
		return 1.0f;
	case _shield_recharge_rate_percentage_setting_110_percent:
		return 1.1f;
	case _shield_recharge_rate_percentage_setting_200_percent:
		return 2.0f;
	}
	return 1.0f;
}

void c_player_trait_shield_vitality::set_shield_recharge_rate_percentage(real shield_recharge_rate_percentage, bool force)
{
	// #TODO: implement
}

e_vampirism_percentage_setting c_player_trait_shield_vitality::get_vampirism_percentage_setting() const
{
	return m_vampirism_percentage_setting;
}

void c_player_trait_shield_vitality::set_vampirism_percentage_setting(e_vampirism_percentage_setting vampirism_percentage_setting, bool force)
{
	if (!VALID_INDEX(vampirism_percentage_setting, k_vampirism_percentage_settings))
	{
		c_console::write_line("game_engine:player_trait:shield_vitality: invalid vampirism value '%ld'!", vampirism_percentage_setting);

		if (force)
			m_vampirism_percentage_setting = _vampirism_percentage_setting_unchanged;
	}
	else if (vampirism_percentage_setting || force)
	{
		m_vampirism_percentage_setting = vampirism_percentage_setting;
	}
}

real c_player_trait_shield_vitality::get_vampirism_percentage() const
{
	switch (get_vampirism_percentage_setting())
	{
	case _vampirism_percentage_setting_0_percent:
		return 0.0f;
	case _vampirism_percentage_setting_10_percent:
		return 0.1f;
	case _vampirism_percentage_setting_25_percent:
		return 0.25f;
	case _vampirism_percentage_setting_50_percent:
		return 0.5f;
	case _vampirism_percentage_setting_100_percent:
		return 1.0f;
	}
	return 0.0f;
}

void c_player_trait_shield_vitality::set_vampirism_percentage(real vampirism_percentage, bool force)
{
	// #TODO: implement
}

e_headshot_immunity_setting c_player_trait_shield_vitality::get_headshot_immunity_setting() const
{
	return m_headshot_immunity_setting;
}

void c_player_trait_shield_vitality::set_headshot_immunity_setting(e_headshot_immunity_setting headshot_immunity_setting, bool force)
{
	if (!VALID_INDEX(headshot_immunity_setting, k_headshot_immunity_settings))
	{
		c_console::write_line("game_engine:player_trait:shield_vitality: invalid headshot immunity value '%ld'!", headshot_immunity_setting);

		if (force)
			m_headshot_immunity_setting = _headshot_immunity_setting_unchanged;
	}
	else if (headshot_immunity_setting || force)
	{
		m_headshot_immunity_setting = headshot_immunity_setting;
	}
}

bool c_player_trait_shield_vitality::get_headshot_immunity() const
{
	return get_headshot_immunity_setting() == _headshot_immunity_setting_immune_to_headshots;
}

void c_player_trait_shield_vitality::set_headshot_immunity(bool headshot_immunity, bool force)
{
	// #TODO: implement
}

e_shield_multiplier_setting c_player_trait_shield_vitality::get_shield_multiplier_setting() const
{
	return m_shield_multiplier_setting;
}

void c_player_trait_shield_vitality::set_shield_multiplier_setting(e_shield_multiplier_setting shield_multiplier_setting, bool force)
{
	if (!VALID_INDEX(shield_multiplier_setting, k_shield_multiplier_settings))
	{
		c_console::write_line("game_engine:player_trait:shield_vitality: invalid shield multiplier value '%ld'!", shield_multiplier_setting);

		if (force)
			m_shield_multiplier_setting = _shield_multiplier_setting_unchanged;
	}
	else if (shield_multiplier_setting || force)
	{
		m_shield_multiplier_setting = shield_multiplier_setting;
	}
}

long c_player_trait_shield_vitality::get_shield_multiplier() const
{
	switch (get_shield_multiplier_setting())
	{
	case _shield_multiplier_setting_0x:
		return 0;
	case _shield_multiplier_setting_1x:
		return 1;
	case _shield_multiplier_setting_2x:
		return 2;
	case _shield_multiplier_setting_3x:
		return 3;
	case _shield_multiplier_setting_4x:
		return 4;
	}
	return 1;
}

void c_player_trait_shield_vitality::set_shield_multiplier(long shield_multiplier, bool force)
{
	// #TODO: implement
}


