#include "game/game_engine_player_traits.hpp"

#include "cseries/cseries_events.hpp"
#include "game/game_engine_util.hpp"
#include "memory/bitstream.hpp"
#include "memory/byte_swapping.hpp"

void c_player_traits::byteswap()
{
	m_weapon_traits.byteswap();
	m_sensor_traits.byteswap();
}

void c_player_traits::set(const c_player_traits* traits, bool force)
{
	ASSERT(traits != nullptr);

	if (traits)
	{
		get_shield_vitality_traits_writeable()->set(traits->get_shield_vitality_traits(), force);
		get_weapons_traits_writeable()->set(traits->get_weapons_traits(), force);
		get_movement_traits_writeable()->set(traits->get_movement_traits(), force);
		get_appearance_traits_writeable()->set(traits->get_appearance_traits(), force);
		get_sensor_traits_writeable()->set(traits->get_sensor_traits(), force);
	}
	else if (force)
	{
		get_shield_vitality_traits_writeable()->set_damage_resistance_percentage_setting(_damage_resistance_percentage_setting_unchanged, force);
		get_shield_vitality_traits_writeable()->set_shield_recharge_rate_percentage_setting(_shield_recharge_rate_percentage_setting_unchanged, force);
		get_shield_vitality_traits_writeable()->set_vampirism_percentage_setting(_vampirism_percentage_setting_unchanged, force);
		get_shield_vitality_traits_writeable()->set_headshot_immunity_setting(_headshot_immunity_setting_unchanged, force);
		get_shield_vitality_traits_writeable()->set_shield_multiplier_setting(_shield_multiplier_setting_unchanged, force);

		get_weapons_traits_writeable()->set_initial_grenade_count_setting(_grenade_count_setting_unchanged, force);
		get_weapons_traits_writeable()->set_initial_primary_weapon_absolute_index(int8(0xFE), force);
		get_weapons_traits_writeable()->set_initial_secondary_weapon_absolute_index(int8(0xFE), force);
		get_weapons_traits_writeable()->set_damage_modifier_percentage_setting(_damage_modifier_percentage_setting_unchanged, force);
		get_weapons_traits_writeable()->set_recharging_grenades_setting(_recharging_grenades_setting_unchanged, force);
		get_weapons_traits_writeable()->set_infinite_ammo_setting(_infinite_ammo_setting_unchanged, force);
		get_weapons_traits_writeable()->set_weapon_pickup_allowed_setting(_weapon_pickup_setting_unchanged, force);

		get_movement_traits_writeable()->set_speed_setting(_player_speed_setting_unchanged, force);
		get_movement_traits_writeable()->set_gravity_setting(_player_gravity_setting_unchanged, force);
		get_movement_traits_writeable()->set_vehicle_usage_setting(_vehicle_usage_setting_unchanged, force);

		get_appearance_traits_writeable()->set_active_camo_setting(_active_camo_setting_unchanged, force);
		get_appearance_traits_writeable()->set_waypoint_setting(_waypoint_setting_unchanged, force);
		get_appearance_traits_writeable()->set_aura_setting(_aura_setting_unchanged, force);
		get_appearance_traits_writeable()->set_forced_change_color_setting(_forced_change_color_setting_unchanged, force);

		get_sensor_traits_writeable()->set_motion_tracker_setting(_motion_tracker_setting_unchanged, force);
		get_sensor_traits_writeable()->set_motion_tracker_range_setting(_motion_tracker_range_setting_unchanged, force);
	}
}

void c_player_traits::encode_to_mcc(c_bitstream* packet) const
{
	get_shield_vitality_traits()->encode_to_mcc(packet);
	get_weapons_traits()->encode_to_mcc(packet);
	get_movement_traits()->encode_to_mcc(packet);
	get_appearance_traits()->encode_to_mcc(packet);
	get_sensor_traits()->encode_to_mcc(packet);
}

void c_player_traits::decode_from_mcc(c_bitstream* packet)
{
	get_shield_vitality_traits_writeable()->decode_from_mcc(packet);
	get_weapons_traits_writeable()->decode_from_mcc(packet);
	get_movement_traits_writeable()->decode_from_mcc(packet);
	get_appearance_traits_writeable()->decode_from_mcc(packet);
	get_sensor_traits_writeable()->decode_from_mcc(packet);
}

c_player_trait_shield_vitality* c_player_traits::get_shield_vitality_traits_writeable()
{
	return &m_shield_vitality_traits;
}

const c_player_trait_shield_vitality* c_player_traits::get_shield_vitality_traits() const
{
	return &m_shield_vitality_traits;
}

void c_player_traits::set_shield_vitality_traits(const c_player_trait_shield_vitality* traits, bool force)
{
	m_shield_vitality_traits.set(traits, force);
}

c_player_trait_weapons* c_player_traits::get_weapons_traits_writeable()
{
	return &m_weapon_traits;
}

const c_player_trait_weapons* c_player_traits::get_weapons_traits() const
{
	return &m_weapon_traits;
}

void c_player_traits::set_weapons_traits(const c_player_trait_weapons* traits, bool force)
{
	m_weapon_traits.set(traits, force);
}

c_player_trait_movement* c_player_traits::get_movement_traits_writeable()
{
	return &m_movement_traits;
}

const c_player_trait_movement* c_player_traits::get_movement_traits() const
{
	return &m_movement_traits;
}

void c_player_traits::set_movement_traits(const c_player_trait_movement* traits, bool force)
{
	m_movement_traits.set(traits, force);
}

c_player_trait_appearance* c_player_traits::get_appearance_traits_writeable()
{
	return &m_appearance_traits;
}

const c_player_trait_appearance* c_player_traits::get_appearance_traits() const
{
	return &m_appearance_traits;
}

void c_player_traits::set_appearance_traits(const c_player_trait_appearance* traits, bool force)
{
	m_appearance_traits.set(traits, force);
}

c_player_trait_sensors* c_player_traits::get_sensor_traits_writeable()
{
	return &m_sensor_traits;
}

const c_player_trait_sensors* c_player_traits::get_sensor_traits() const
{
	return &m_sensor_traits;
}

void c_player_traits::set_sensor_traits(const c_player_trait_sensors* traits, bool force)
{
	m_sensor_traits.set(traits, force);
}

void c_player_trait_shield_vitality::set(const c_player_trait_shield_vitality* traits, bool force)
{
	ASSERT(traits != nullptr);

	set_damage_resistance_percentage_setting(traits->get_damage_resistance_percentage_setting(), force);
	set_shield_recharge_rate_percentage_setting(traits->get_shield_recharge_rate_percentage_setting(), force);
	set_vampirism_percentage_setting(traits->get_vampirism_percentage_setting(), force);
	set_shield_multiplier_setting(traits->get_shield_multiplier_setting(), force);
	set_headshot_immunity_setting(traits->get_headshot_immunity_setting(), force);
}

void c_player_trait_shield_vitality::encode_to_mcc(c_bitstream* packet) const
{
	ASSERT(packet != nullptr);

	e_damage_resistance_percentage_setting damage_resistance_percentage_setting = get_damage_resistance_percentage_setting();
	e_shield_recharge_rate_percentage_setting shield_recharge_rate_percentage_setting = get_shield_recharge_rate_percentage_setting();
	e_vampirism_percentage_setting vampirism_percentage_setting = get_vampirism_percentage_setting();
	e_headshot_immunity_setting headshot_immunity_setting = get_headshot_immunity_setting();
	e_shield_multiplier_setting shield_multiplier_setting = get_shield_multiplier_setting();

	packet->write_integer("player-trait-damage-resistance", damage_resistance_percentage_setting, 4);
	packet->write_integer("player-trait-shield-recharge-rate", shield_recharge_rate_percentage_setting, 4);
	packet->write_integer("player-trait-vampirism", vampirism_percentage_setting, 3);
	packet->write_integer("player-traits-headshot-immunity", headshot_immunity_setting, 2);
	packet->write_integer("player-trait-shield-multiplier", shield_multiplier_setting, 3);
}

void c_player_trait_shield_vitality::decode_from_mcc(c_bitstream* packet)
{
	ASSERT(packet != nullptr);

	e_damage_resistance_percentage_setting damage_resistance_percentage_setting = packet->read_enum<e_damage_resistance_percentage_setting, 4>("player-trait-damage-resistance");
	e_shield_recharge_rate_percentage_setting shield_recharge_rate_percentage_setting = packet->read_enum<e_shield_recharge_rate_percentage_setting, 4>("player-trait-shield-recharge-rate");
	e_vampirism_percentage_setting vampirism_percentage_setting = packet->read_enum<e_vampirism_percentage_setting, 3>("player-trait-vampirism");
	e_headshot_immunity_setting headshot_immunity_setting = packet->read_enum<e_headshot_immunity_setting, 2>("player-traits-headshot-immunity");
	e_shield_multiplier_setting shield_multiplier_setting = packet->read_enum<e_shield_multiplier_setting, 3>("player-trait-shield-multiplier");

	set_damage_resistance_percentage_setting(damage_resistance_percentage_setting, true);
	set_shield_recharge_rate_percentage_setting(shield_recharge_rate_percentage_setting, true);
	set_vampirism_percentage_setting(vampirism_percentage_setting, true);
	set_headshot_immunity_setting(headshot_immunity_setting, true);
	set_shield_multiplier_setting(shield_multiplier_setting, true);
}

e_damage_resistance_percentage_setting c_player_trait_shield_vitality::get_damage_resistance_percentage_setting() const
{
	return m_damage_resistance_percentage_setting;
}

void c_player_trait_shield_vitality::set_damage_resistance_percentage_setting(e_damage_resistance_percentage_setting damage_resistance_percentage_setting, bool force)
{
	if (!VALID_INDEX(damage_resistance_percentage_setting, k_damage_resistance_percentage_settings))
	{
		event(_event_warning, "game_engine:player_trait:shield_vitality: invalid damage resistance value '%ld'!", damage_resistance_percentage_setting);

		if (force)
			m_shield_recharge_rate_percentage_setting = _damage_resistance_percentage_setting_unchanged;
	}
	else if (damage_resistance_percentage_setting || force)
	{
		m_damage_resistance_percentage_setting = damage_resistance_percentage_setting;
	}
}

real32 c_player_trait_shield_vitality::get_damage_resistance_percentage() const
{
	e_damage_resistance_percentage_setting setting = get_damage_resistance_percentage_setting();

	const int32 minimum_value = _damage_resistance_percentage_setting_10_percent;
	if (IN_RANGE_INCLUSIVE(setting, minimum_value, _damage_resistance_percentage_setting_invulnerable))
		return k_damage_resistance_percentage_values[setting - minimum_value];

	return k_damage_resistance_percentage_values[k_damage_resistance_percentage_setting_default - minimum_value];
}

void c_player_trait_shield_vitality::set_damage_resistance_percentage(real32 damage_resistance_percentage, bool force)
{
	const int32 minimum_value = _damage_resistance_percentage_setting_10_percent;
	for (int32 setting = minimum_value; setting < k_damage_resistance_percentage_settings; setting++)
	{
		real32 value = k_damage_resistance_percentage_values[setting - minimum_value];
		real32 distance = damage_resistance_percentage - value;
		if ((damage_resistance_percentage - value) < 0.0f)
			distance = -distance;

		if (distance > k_real_epsilon)
			continue;

		set_damage_resistance_percentage_setting(static_cast<e_damage_resistance_percentage_setting>(setting), force);
		return;
	}

	set_damage_resistance_percentage_setting(_damage_resistance_percentage_setting_unchanged, force);
}

e_shield_recharge_rate_percentage_setting c_player_trait_shield_vitality::get_shield_recharge_rate_percentage_setting() const
{
	return m_shield_recharge_rate_percentage_setting;
}

void c_player_trait_shield_vitality::set_shield_recharge_rate_percentage_setting(e_shield_recharge_rate_percentage_setting shield_recharge_rate_percentage_setting, bool force)
{
	if (!VALID_INDEX(shield_recharge_rate_percentage_setting, k_shield_recharge_rate_percentage_settings))
	{
		event(_event_warning, "game_engine:player_trait:shield_vitality: invalid shield recharge rate value '%ld'!", shield_recharge_rate_percentage_setting);

		if (force)
			m_shield_recharge_rate_percentage_setting = _shield_recharge_rate_percentage_setting_unchanged;
	}
	else if (shield_recharge_rate_percentage_setting || force)
	{
		m_shield_recharge_rate_percentage_setting = shield_recharge_rate_percentage_setting;
	}
}

real32 c_player_trait_shield_vitality::get_shield_recharge_rate_percentage() const
{
	e_shield_recharge_rate_percentage_setting setting = get_shield_recharge_rate_percentage_setting();

	const int32 minimum_value = _shield_recharge_rate_percentage_setting_negative_25_percent;
	if (IN_RANGE_INCLUSIVE(setting, minimum_value, _shield_recharge_rate_percentage_setting_200_percent))
		return k_shield_recharge_rate_percentage_values[setting - minimum_value];

	return k_shield_recharge_rate_percentage_values[k_shield_recharge_rate_percentage_setting_default - minimum_value];
}

void c_player_trait_shield_vitality::set_shield_recharge_rate_percentage(real32 shield_recharge_rate_percentage, bool force)
{
	const int32 minimum_value = _shield_recharge_rate_percentage_setting_negative_25_percent;
	for (int32 setting = minimum_value; setting < k_shield_recharge_rate_percentage_settings; setting++)
	{
		real32 value = k_shield_recharge_rate_percentage_values[setting - minimum_value];
		real32 distance = shield_recharge_rate_percentage - value;
		if ((shield_recharge_rate_percentage - value) < 0.0f)
			distance = -distance;

		if (distance > k_real_epsilon)
			continue;

		set_shield_recharge_rate_percentage_setting(static_cast<e_shield_recharge_rate_percentage_setting>(setting), force);
		return;
	}

	set_shield_recharge_rate_percentage_setting(_shield_recharge_rate_percentage_setting_unchanged, force);
}

e_vampirism_percentage_setting c_player_trait_shield_vitality::get_vampirism_percentage_setting() const
{
	return m_vampirism_percentage_setting;
}

void c_player_trait_shield_vitality::set_vampirism_percentage_setting(e_vampirism_percentage_setting vampirism_percentage_setting, bool force)
{
	if (!VALID_INDEX(vampirism_percentage_setting, k_vampirism_percentage_settings))
	{
		event(_event_warning, "game_engine:player_trait:shield_vitality: invalid vampirism value '%ld'!", vampirism_percentage_setting);

		if (force)
			m_vampirism_percentage_setting = _vampirism_percentage_setting_unchanged;
	}
	else if (vampirism_percentage_setting || force)
	{
		m_vampirism_percentage_setting = vampirism_percentage_setting;
	}
}

real32 c_player_trait_shield_vitality::get_vampirism_percentage() const
{
	e_vampirism_percentage_setting setting = get_vampirism_percentage_setting();

	const int32 minimum_value = _vampirism_percentage_setting_0_percent;
	if (IN_RANGE_INCLUSIVE(setting, minimum_value, _vampirism_percentage_setting_100_percent))
		return k_vampirism_percentage_values[setting - minimum_value];

	return k_shield_recharge_rate_percentage_values[k_vampirism_percentage_setting_default - minimum_value];
}

void c_player_trait_shield_vitality::set_vampirism_percentage(real32 vampirism_percentage, bool force)
{
	const int32 minimum_value = _vampirism_percentage_setting_0_percent;
	for (int32 setting = minimum_value; setting < k_vampirism_percentage_settings; setting++)
	{
		real32 value = k_vampirism_percentage_values[setting - minimum_value];
		real32 distance = vampirism_percentage - value;
		if ((vampirism_percentage - value) < 0.0f)
			distance = -distance;

		if (distance > k_real_epsilon)
			continue;

		set_vampirism_percentage_setting(static_cast<e_vampirism_percentage_setting>(setting), force);
		return;
	}

	set_vampirism_percentage_setting(_vampirism_percentage_setting_unchanged, force);
}

e_headshot_immunity_setting c_player_trait_shield_vitality::get_headshot_immunity_setting() const
{
	return m_headshot_immunity_setting;
}

void c_player_trait_shield_vitality::set_headshot_immunity_setting(e_headshot_immunity_setting headshot_immunity_setting, bool force)
{
	if (!VALID_INDEX(headshot_immunity_setting, k_headshot_immunity_settings))
	{
		event(_event_warning, "game_engine:player_trait:shield_vitality: invalid headshot immunity value '%ld'!", headshot_immunity_setting);

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
	set_headshot_immunity_setting(static_cast<e_headshot_immunity_setting>((headshot_immunity ^ 1) + 1), force);
}

e_shield_multiplier_setting c_player_trait_shield_vitality::get_shield_multiplier_setting() const
{
	return m_shield_multiplier_setting;
}

void c_player_trait_shield_vitality::set_shield_multiplier_setting(e_shield_multiplier_setting shield_multiplier_setting, bool force)
{
	if (!VALID_INDEX(shield_multiplier_setting, k_shield_multiplier_settings))
	{
		event(_event_warning, "game_engine:player_trait:shield_vitality: invalid shield multiplier value '%ld'!", shield_multiplier_setting);

		if (force)
			m_shield_multiplier_setting = _shield_multiplier_setting_unchanged;
	}
	else if (shield_multiplier_setting || force)
	{
		m_shield_multiplier_setting = shield_multiplier_setting;
	}
}

int32 c_player_trait_shield_vitality::get_shield_multiplier() const
{
	e_shield_multiplier_setting setting = get_shield_multiplier_setting();

	const int32 minimum_value = _shield_multiplier_setting_0x;
	if (IN_RANGE_INCLUSIVE(setting, minimum_value, _shield_multiplier_setting_4x))
		return k_shield_multiplier_values[setting - minimum_value];

	return k_shield_multiplier_values[k_shield_multiplier_setting_default - minimum_value];
}

void c_player_trait_shield_vitality::set_shield_multiplier(int32 shield_multiplier, bool force)
{
	const int32 minimum_value = _shield_multiplier_setting_0x;
	for (int32 setting = minimum_value; setting < k_shield_multiplier_settings; setting++)
	{
		int32 value = k_shield_multiplier_values[setting - minimum_value];
		int32 distance = shield_multiplier - value;
		if ((shield_multiplier - value) < 0.0f)
			distance = -distance;

		if (distance > k_real_epsilon)
			continue;

		set_shield_multiplier_setting(static_cast<e_shield_multiplier_setting>(setting), force);
		return;
	}

	set_shield_multiplier_setting(_shield_multiplier_setting_unchanged, force);
}

real32 c_player_trait_shield_vitality::get_maximum_shield_vitality()
{
	return static_cast<real32>(get_shield_multiplier());
}

void c_player_trait_weapons::byteswap()
{
	bswap_uns16_inplace(m_initial_grenade_count_setting);
}

void c_player_trait_weapons::set(const c_player_trait_weapons* traits, bool force)
{
	ASSERT(traits != nullptr);

	set_initial_grenade_count_setting(traits->get_initial_grenade_count_setting(), force);
	set_initial_primary_weapon_absolute_index(traits->get_initial_primary_weapon_absolute_index(), force);
	set_initial_secondary_weapon_absolute_index(traits->get_initial_secondary_weapon_absolute_index(), force);
	set_damage_modifier_percentage_setting(traits->get_damage_modifier_percentage_setting(), force);
	set_recharging_grenades_setting(traits->get_recharging_grenades_setting(), force);
	set_infinite_ammo_setting(traits->get_infinite_ammo_setting(), force);
	set_weapon_pickup_allowed_setting(traits->get_weapon_pickup_setting(), force);
}

void c_player_trait_weapons::encode_to_mcc(c_bitstream* packet) const
{
	ASSERT(packet != nullptr);

	e_damage_modifier_percentage_setting damage_modifier_percentage_setting = get_damage_modifier_percentage_setting();
	int8 initial_primary_weapon_absolute_index = get_initial_primary_weapon_absolute_index();
	int8 initial_secondary_weapon_absolute_index = get_initial_secondary_weapon_absolute_index();
	e_grenade_count_setting initial_grenade_count_setting = get_initial_grenade_count_setting();
	e_infinite_ammo_setting infinite_ammo_setting = get_infinite_ammo_setting();
	e_recharging_grenades_setting recharging_grenades_setting = get_recharging_grenades_setting();
	e_weapon_pickup_setting weapon_pickup_setting = get_weapon_pickup_setting();

	packet->write_integer("player-trait-damage-modifier", initial_grenade_count_setting, 4);
	packet->write_signed_integer("player-trait-initial-primary-weapon", initial_primary_weapon_absolute_index, 8);
	packet->write_signed_integer("player-trait-initial-secondary-weapon", initial_secondary_weapon_absolute_index, 8);
	packet->write_integer("player-trait-initial-grenade-count", damage_modifier_percentage_setting, 2);
	packet->write_integer("player-traits-infinite-ammo-setting", recharging_grenades_setting, 2);
	packet->write_integer("player-traits-recharging-grenades", infinite_ammo_setting, 2);
	packet->write_integer("player-traits-weapon-pickup-allowed", weapon_pickup_setting, 2);
}

void c_player_trait_weapons::decode_from_mcc(c_bitstream* packet)
{
	ASSERT(packet != nullptr);

	e_damage_modifier_percentage_setting damage_modifier_percentage_setting = packet->read_enum<e_damage_modifier_percentage_setting, 4>("player-trait-damage-modifier");
	int8 initial_primary_weapon_absolute_index = static_cast<int8>(packet->read_signed_integer("player-trait-initial-primary-weapon", 8));
	int8 initial_secondary_weapon_absolute_index = static_cast<int8>(packet->read_signed_integer("player-trait-initial-secondary-weapon", 8));
	e_grenade_count_setting initial_grenade_count_setting = packet->read_enum<e_grenade_count_setting, 2>("player-trait-initial-grenade-count");
	e_infinite_ammo_setting infinite_ammo_setting = packet->read_enum<e_infinite_ammo_setting, 2>("player-traits-infinite-ammo-setting");
	e_recharging_grenades_setting recharging_grenades_setting = packet->read_enum<e_recharging_grenades_setting, 2>("player-traits-recharging-grenades");
	e_weapon_pickup_setting weapon_pickup_setting = packet->read_enum<e_weapon_pickup_setting, 2>("player-traits-weapon-pickup-allowed");

	// override enabled including firebombs
	if (recharging_grenades_setting == 3)
		recharging_grenades_setting = _recharging_grenades_setting_enabled;

	set_damage_modifier_percentage_setting(damage_modifier_percentage_setting, true);
	set_initial_primary_weapon_absolute_index(initial_primary_weapon_absolute_index, true);
	set_initial_secondary_weapon_absolute_index(initial_secondary_weapon_absolute_index, true);
	set_initial_grenade_count_setting(initial_grenade_count_setting, true);
	set_infinite_ammo_setting(infinite_ammo_setting, true);
	set_recharging_grenades_setting(recharging_grenades_setting, true);
	set_weapon_pickup_allowed_setting(weapon_pickup_setting, true);
}

e_grenade_count_setting c_player_trait_weapons::get_initial_grenade_count_setting() const
{
	return m_initial_grenade_count_setting;
}

void c_player_trait_weapons::set_initial_grenade_count_setting(e_grenade_count_setting initial_grenade_count_setting, bool force)
{
	if (!VALID_INDEX(initial_grenade_count_setting, k_grenade_count_settings))
	{
		event(_event_warning, "game_engine:player_trait:weapons: invalid initial grenade count '%d'", initial_grenade_count_setting);

		if (force)
			m_initial_grenade_count_setting = _grenade_count_setting_unchanged;
	}
	else if (initial_grenade_count_setting || force)
	{
		m_initial_grenade_count_setting = initial_grenade_count_setting;
	}
}

int8 c_player_trait_weapons::get_initial_grenade_count() const
{
	e_grenade_count_setting setting = get_initial_grenade_count_setting();

	const int32 minimum_value = _grenade_count_setting_none;
	if (IN_RANGE_INCLUSIVE(setting, minimum_value, _grenade_count_setting_two))
		return k_grenade_count_values[setting - minimum_value];

	return k_grenade_count_values[k_grenade_count_setting_default - minimum_value];
}

void c_player_trait_weapons::set_initial_grenade_count(int32 initial_grenade_count, bool force)
{
	const int32 minimum_value = _grenade_count_setting_none;
	for (int32 setting = minimum_value; setting < k_grenade_count_settings; setting++)
	{
		real32 value = k_grenade_count_values[setting - minimum_value];
		real32 distance = initial_grenade_count - value;
		if ((initial_grenade_count - value) < 0.0f)
			distance = -distance;

		if (distance > k_real_epsilon)
			continue;

		set_initial_grenade_count_setting(static_cast<e_grenade_count_setting>(setting), force);
		return;
	}

	set_initial_grenade_count_setting(_grenade_count_setting_unchanged, force);
}

int8 c_player_trait_weapons::get_initial_primary_weapon_absolute_index() const
{
	return m_initial_primary_weapon_absolute_index;
}

void c_player_trait_weapons::set_initial_primary_weapon_absolute_index(int8 initial_primary_weapon_absolute_index, bool force)
{
	if (initial_primary_weapon_absolute_index >= 0 || initial_primary_weapon_absolute_index == int8(0xFF) || initial_primary_weapon_absolute_index == int8(0xFD) || initial_primary_weapon_absolute_index == int8(0xFE))
	{
		if (force || initial_primary_weapon_absolute_index != int8(0xFE))
		{
			m_initial_primary_weapon_absolute_index = initial_primary_weapon_absolute_index;
		}
		else
		{
			event(_event_warning, "game_engine:player_trait:weapons: ignoring initial primary weapon '%d'", int8(0xFE));
		}
	}
	else if (force)
	{
		event(_event_warning, "game_engine:player_trait:weapons: invalid initial primary weapon '%d'", initial_primary_weapon_absolute_index);
		m_initial_primary_weapon_absolute_index = int8(0xFE);
	}
}

int32 c_player_trait_weapons::get_initial_primary_weapon() const
{
	return game_engine_get_multiplayer_weapon_selection_name(get_initial_primary_weapon_absolute_index());
}

void c_player_trait_weapons::set_initial_primary_weapon(int32 initial_primary_weapon, bool force)
{
	set_initial_primary_weapon_absolute_index(static_cast<int8>(game_engine_get_multiplayer_weapon_selection_absolute_index(initial_primary_weapon)), force);
}

int8 c_player_trait_weapons::get_initial_secondary_weapon_absolute_index() const
{
	return m_initial_secondary_weapon_absolute_index;
}

void c_player_trait_weapons::set_initial_secondary_weapon_absolute_index(int8 initial_secondary_weapon_absolute_index, bool force)
{
	if (initial_secondary_weapon_absolute_index >= 0 || initial_secondary_weapon_absolute_index == int8(0xFF) || initial_secondary_weapon_absolute_index == int8(0xFD) || initial_secondary_weapon_absolute_index == int8(0xFE))
	{
		if (force || initial_secondary_weapon_absolute_index != int8(0xFE))
		{
			m_initial_secondary_weapon_absolute_index = initial_secondary_weapon_absolute_index;
		}
		else
		{
			event(_event_warning, "game_engine:player_trait:weapons: ignoring initial secondary weapon '%d'", int8(0xFE));
		}
	}
	else if (force)
	{
		event(_event_warning, "game_engine:player_trait:weapons: invalid initial secondary weapon '%d'", initial_secondary_weapon_absolute_index);
		m_initial_secondary_weapon_absolute_index = int8(0xFE);
	}
}

int32 c_player_trait_weapons::get_initial_secondary_weapon() const
{
	return game_engine_get_multiplayer_weapon_selection_name(get_initial_secondary_weapon_absolute_index());
}

void c_player_trait_weapons::set_initial_secondary_weapon(int32 initial_secondary_weapon, bool force)
{
	set_initial_secondary_weapon_absolute_index(static_cast<int8>(game_engine_get_multiplayer_weapon_selection_absolute_index(initial_secondary_weapon)), force);
}

e_damage_modifier_percentage_setting c_player_trait_weapons::get_damage_modifier_percentage_setting() const
{
	return m_damage_modifier_percentage_setting;
}

void c_player_trait_weapons::set_damage_modifier_percentage_setting(e_damage_modifier_percentage_setting damage_modifier_percentage_setting, bool force)
{
	if (!VALID_INDEX(damage_modifier_percentage_setting, k_damage_modifier_percentage_settings))
	{
		event(_event_warning, "game_engine:player_trait:weapons: invalid damage modifier value '%ld'!", damage_modifier_percentage_setting);

		if (force)
			m_damage_modifier_percentage_setting = _damage_modifier_percentage_setting_unchanged;
	}
	else if (damage_modifier_percentage_setting || force)
	{
		m_damage_modifier_percentage_setting = damage_modifier_percentage_setting;
	}
}

real32 c_player_trait_weapons::get_damage_modifier_percentage() const
{
	e_damage_modifier_percentage_setting setting = get_damage_modifier_percentage_setting();

	const int32 minimum_value = _damage_modifier_percentage_setting_0_percent;
	if (IN_RANGE_INCLUSIVE(setting, minimum_value, _damage_modifier_percentage_setting_fatality))
		return k_damage_modifier_percentage_values[setting - minimum_value];

	return k_damage_modifier_percentage_values[k_damage_modifier_percentage_setting_default - minimum_value];
}

void c_player_trait_weapons::set_damage_modifier_percentage(real32 damage_modifier_percentage, bool force)
{
	const int32 minimum_value = _damage_modifier_percentage_setting_0_percent;
	for (int32 setting = minimum_value; setting < k_damage_modifier_percentage_settings; setting++)
	{
		real32 value = k_damage_modifier_percentage_values[setting - minimum_value];
		real32 distance = damage_modifier_percentage - value;
		if ((damage_modifier_percentage - value) < 0.0f)
			distance = -distance;

		if (distance > k_real_epsilon)
			continue;

		set_damage_modifier_percentage_setting(static_cast<e_damage_modifier_percentage_setting>(setting), force);
		return;
	}

	set_damage_modifier_percentage_setting(_damage_modifier_percentage_setting_unchanged, force);
}

e_recharging_grenades_setting c_player_trait_weapons::get_recharging_grenades_setting() const
{
	return m_recharging_grenades_setting;
}

void c_player_trait_weapons::set_recharging_grenades_setting(e_recharging_grenades_setting recharging_grenades_setting, bool force)
{
	if (!VALID_INDEX(recharging_grenades_setting, k_recharging_grenades_settings))
	{
		event(_event_warning, "game_engine:player_trait:weapons: invalid recharging grenade setting '%d'", recharging_grenades_setting);

		if (force)
			m_recharging_grenades_setting = _recharging_grenades_setting_unchanged;
	}
	else if (recharging_grenades_setting || force)
	{
		m_recharging_grenades_setting = recharging_grenades_setting;
	}
}

bool c_player_trait_weapons::get_recharging_grenades() const
{
	// MCC
	//return m_recharging_grenades_setting == _recharging_grenades_setting_enabled || m_recharging_grenades_setting == _recharging_grenades_setting_enabled_including_firebombs;
	return m_recharging_grenades_setting == _recharging_grenades_setting_enabled;
}

void c_player_trait_weapons::set_recharging_grenades(bool recharging_grenades, bool force)
{
	set_recharging_grenades_setting(static_cast<e_recharging_grenades_setting>((recharging_grenades ^ 1) + 1), force);
}

e_infinite_ammo_setting c_player_trait_weapons::get_infinite_ammo_setting() const
{
	return m_infinite_ammo_setting;
}

void c_player_trait_weapons::set_infinite_ammo_setting(e_infinite_ammo_setting infinite_ammo_setting, bool force)
{
	if (!VALID_INDEX(infinite_ammo_setting, k_infinite_ammo_settings))
	{
		event(_event_warning, "game_engine:player_trait:weapons: invalid infinite ammo setting '%d'", infinite_ammo_setting);

		if (force)
			m_infinite_ammo_setting = _infinite_ammo_setting_unchanged;
	}
	else if (infinite_ammo_setting || force)
	{
		m_infinite_ammo_setting = infinite_ammo_setting;
	}
}

bool c_player_trait_weapons::infinite_ammo_enabled() const
{
	// MCC
	//return m_infinite_ammo_setting >= _infinite_ammo_setting_enabled && m_infinite_ammo_setting <= _infinite_ammo_setting_bottomless_clip;
	return m_infinite_ammo_setting == _infinite_ammo_setting_enabled;
}

void c_player_trait_weapons::set_infinite_ammo(bool infinite_ammo, bool force)
{
	set_infinite_ammo_setting(static_cast<e_infinite_ammo_setting>(infinite_ammo + 1), force);
}

e_weapon_pickup_setting c_player_trait_weapons::get_weapon_pickup_setting() const
{
	return m_weapon_pickup_setting;
}

void c_player_trait_weapons::set_weapon_pickup_allowed_setting(e_weapon_pickup_setting weapon_pickup_setting, bool force)
{
	if (!VALID_INDEX(weapon_pickup_setting, k_weapon_pickup_settings))
	{
		event(_event_warning, "game_engine:player_trait:weapons: invalid weapon pickup setting '%d'", weapon_pickup_setting);

		if (force)
			m_weapon_pickup_setting = _weapon_pickup_setting_unchanged;
	}
	else if (weapon_pickup_setting || force)
	{
		m_weapon_pickup_setting = weapon_pickup_setting;
	}
}

bool c_player_trait_weapons::get_weapon_pickup_allowed() const
{
	return m_weapon_pickup_setting != _weapon_pickup_setting_disallowed;
}

void c_player_trait_weapons::set_weapon_pickup_allowed(bool weapon_pickup_allowed, bool force)
{
	set_weapon_pickup_allowed_setting(static_cast<e_weapon_pickup_setting>((weapon_pickup_allowed ^ 1) + 1), force);
}

void c_player_trait_movement::set(const c_player_trait_movement* traits, bool force)
{
	ASSERT(traits != nullptr);

	set_speed_setting(traits->get_speed_setting(), force);
	set_gravity_setting(traits->get_gravity_setting(), force);
	set_vehicle_usage_setting(traits->get_vehicle_usage_setting(), force);
}

void c_player_trait_movement::encode_to_mcc(c_bitstream* packet) const
{
	ASSERT(packet != nullptr);

	e_player_speed_setting speed_setting = get_speed_setting();
	e_player_gravity_setting gravity_setting = get_gravity_setting();
	e_vehicle_usage_setting vehicle_usage_setting = get_vehicle_usage_setting();

	packet->write_integer("player-traits-movement-speed", speed_setting, 4);
	packet->write_integer("player-traits-movement-gravity", gravity_setting, 3);
	packet->write_integer("player-traits-movement-vehicle-usage", vehicle_usage_setting, 2);
}

void c_player_trait_movement::decode_from_mcc(c_bitstream* packet)
{
	ASSERT(packet != nullptr);

	e_player_speed_setting speed_setting = packet->read_enum<e_player_speed_setting, 4>("player-traits-movement-speed");
	e_player_gravity_setting gravity_setting = packet->read_enum<e_player_gravity_setting, 3>("player-traits-movement-gravity");
	e_vehicle_usage_setting vehicle_usage_setting = packet->read_enum<e_vehicle_usage_setting, 2>("player-traits-movement-vehicle-usage");

	set_speed_setting(speed_setting, true);
	set_gravity_setting(gravity_setting, true);
	set_vehicle_usage_setting(vehicle_usage_setting, true);
}

e_player_speed_setting c_player_trait_movement::get_speed_setting() const
{
	return m_speed_setting;
}

void c_player_trait_movement::set_speed_setting(e_player_speed_setting speed_setting, bool force)
{
	if (!VALID_INDEX(speed_setting, k_player_speed_settings))
	{
		event(_event_warning, "game_engine:player_trait:movement: invalid walking speed percentage '%ld'", speed_setting);

		if (force)
			m_speed_setting = _player_speed_setting_unchanged;
	}
	else if (speed_setting || force)
	{
		m_speed_setting = speed_setting;
	}
}

real32 c_player_trait_movement::get_speed() const
{
	e_player_speed_setting setting = get_speed_setting();

	const int32 minimum_value = _player_speed_setting_25_percent;
	if (IN_RANGE_INCLUSIVE(setting, minimum_value, _player_speed_setting_300_percent))
		return k_player_speed_values[setting - minimum_value];

	return k_player_speed_values[k_player_speed_setting_default - minimum_value];
}

void c_player_trait_movement::set_speed(real32 speed, bool force)
{
	const int32 minimum_value = _player_speed_setting_25_percent;
	for (int32 setting = minimum_value; setting < k_player_speed_settings; setting++)
	{
		real32 value = k_player_speed_values[setting - minimum_value];
		real32 distance = speed - value;
		if ((speed - value) < 0.0f)
			distance = -distance;

		if (distance > k_real_epsilon)
			continue;

		set_speed_setting(static_cast<e_player_speed_setting>(setting), force);
		return;
	}

	set_speed_setting(_player_speed_setting_unchanged, force);
}

e_player_gravity_setting c_player_trait_movement::get_gravity_setting() const
{
	return m_gravity_setting;
}

void c_player_trait_movement::set_gravity_setting(e_player_gravity_setting gravity_setting, bool force)
{
	if (!VALID_INDEX(gravity_setting, k_player_gravity_settings))
	{
		event(_event_warning, "game_engine:player_trait:movement: invalid personal gravity '%ld'", gravity_setting);

		if (force)
			m_gravity_setting = _player_gravity_setting_unchanged;
	}
	else if (gravity_setting || force)
	{
		m_gravity_setting = gravity_setting;
	}
}

real32 c_player_trait_movement::get_gravity() const
{
	e_player_gravity_setting setting = get_gravity_setting();

	const int32 minimum_value = _player_gravity_setting_50_percent;
	if (IN_RANGE_INCLUSIVE(setting, minimum_value, _player_gravity_setting_200_percent))
		return k_player_gravity_values[setting - minimum_value];

	return k_player_gravity_values[k_player_gravity_setting_default - minimum_value];
}

void c_player_trait_movement::set_gravity(real32 gravity, bool force)
{
	const int32 minimum_value = _player_gravity_setting_50_percent;
	for (int32 setting = minimum_value; setting < k_player_gravity_settings; setting++)
	{
		real32 value = k_player_gravity_values[setting - minimum_value];
		real32 distance = gravity - value;
		if ((gravity - value) < 0.0f)
			distance = -distance;

		if (distance > k_real_epsilon)
			continue;

		set_gravity_setting(static_cast<e_player_gravity_setting>(setting), force);
		return;
	}

	set_gravity_setting(_player_gravity_setting_unchanged, force);
}

e_vehicle_usage_setting c_player_trait_movement::get_vehicle_usage_setting() const
{
	return m_vehicle_usage_setting;
}

void c_player_trait_movement::set_vehicle_usage_setting(e_vehicle_usage_setting vehicle_usage_setting, bool force)
{
	if (!VALID_INDEX(vehicle_usage_setting, k_vehicle_usage_settings))
	{
		event(_event_warning, "game_engine:player_trait:movement: invalid vehicle usage '%ld'", vehicle_usage_setting);

		if (force)
			m_vehicle_usage_setting = _vehicle_usage_setting_unchanged;
	}
	else if (vehicle_usage_setting || force)
	{
		m_vehicle_usage_setting = vehicle_usage_setting;
	}
}

void c_player_trait_appearance::set(const c_player_trait_appearance* traits, bool force)
{
	ASSERT(traits != nullptr);

	set_active_camo_setting(traits->get_active_camo_setting(), force);
	set_waypoint_setting(traits->get_waypoint_setting(), force);
	set_aura_setting(traits->get_aura_setting(), force);
	set_forced_change_color_setting(traits->get_forced_change_color_setting(), force);
}

void c_player_trait_appearance::encode_to_mcc(c_bitstream* packet) const
{
	ASSERT(packet != nullptr);

	e_active_camo_setting active_camo_setting = get_active_camo_setting();
	e_waypoint_setting waypoint_setting = get_waypoint_setting();
	e_aura_setting aura_setting = get_aura_setting();
	e_forced_change_color_setting forced_change_color_setting = get_forced_change_color_setting();

	packet->write_integer("player-traits-appearance-active-camo", active_camo_setting, 3);
	packet->write_integer("player-traits-appearance-waypoint", waypoint_setting, 2);
	packet->write_integer("player-traits-appearance-aura", aura_setting, 3);
	packet->write_integer("player-traits-appearance-forced-change-color", forced_change_color_setting, 4);
}

void c_player_trait_appearance::decode_from_mcc(c_bitstream* packet)
{
	ASSERT(packet != nullptr);

	e_active_camo_setting active_camo_setting = packet->read_enum<e_active_camo_setting, 3>("player-traits-appearance-active-camo");
	e_waypoint_setting waypoint_setting = packet->read_enum<e_waypoint_setting, 2>("player-traits-appearance-waypoint");
	e_aura_setting aura_setting = packet->read_enum<e_aura_setting, 3>("player-traits-appearance-aura");
	e_forced_change_color_setting forced_change_color_setting = packet->read_enum<e_forced_change_color_setting, 4>("player-traits-appearance-forced-change-color");

	set_active_camo_setting(active_camo_setting, true);
	set_waypoint_setting(waypoint_setting, true);
	set_aura_setting(aura_setting, true);
	set_forced_change_color_setting(forced_change_color_setting, true);
}

e_active_camo_setting c_player_trait_appearance::get_active_camo_setting() const
{
	return m_active_camo_setting;
}

void c_player_trait_appearance::set_active_camo_setting(e_active_camo_setting active_camo_setting, bool force)
{
	if (!VALID_INDEX(active_camo_setting, k_active_camo_settings))
	{
		event(_event_warning, "game_engine:player_trait:appearance: invalid active camo setting '%ld'", active_camo_setting);

		if (force)
			m_active_camo_setting = _active_camo_setting_unchanged;
	}
	else if (active_camo_setting || force)
	{
		m_active_camo_setting = active_camo_setting;
	}
}

e_waypoint_setting c_player_trait_appearance::get_waypoint_setting() const
{
	return m_waypoint_setting;
}

void c_player_trait_appearance::set_waypoint_setting(e_waypoint_setting waypoint_setting, bool force)
{
	if (!VALID_INDEX(waypoint_setting, k_waypoint_settings))
	{
		event(_event_warning, "game_engine:player_trait:appearance: invalid active camo setting '%ld'", waypoint_setting);

		if (force)
			m_waypoint_setting = _waypoint_setting_unchanged;
	}
	else if (waypoint_setting || force)
	{
		m_waypoint_setting = waypoint_setting;
	}
}

e_aura_setting c_player_trait_appearance::get_aura_setting() const
{
	return m_aura_setting;
}

void c_player_trait_appearance::set_aura_setting(e_aura_setting aura_setting, bool force)
{
	if (!VALID_INDEX(aura_setting, k_aura_settings))
	{
		event(_event_warning, "game_engine:player_trait:appearance: invalid aura setting '%ld'", aura_setting);

		if (force)
			m_aura_setting = _aura_setting_unchanged;
	}
	else if (aura_setting || force)
	{
		m_aura_setting = aura_setting;
	}
}

e_forced_change_color_setting c_player_trait_appearance::get_forced_change_color_setting() const
{
	return m_forced_change_color_setting;
}

void c_player_trait_appearance::set_forced_change_color_setting(e_forced_change_color_setting forced_change_color_setting, bool force)
{
	if (!VALID_INDEX(forced_change_color_setting, k_forced_change_color_settings))
	{
		event(_event_warning, "game_engine:player_trait:appearance: invalid forced_change_color setting '%ld'", forced_change_color_setting);

		if (force)
			m_forced_change_color_setting = _forced_change_color_setting_unchanged;
	}
	else if (forced_change_color_setting || force)
	{
		m_forced_change_color_setting = forced_change_color_setting;
	}
}

void c_player_trait_sensors::byteswap()
{
	bswap_uns16_inplace(m_motion_tracker_setting);
	bswap_uns16_inplace(m_motion_tracker_range_setting);
}

void c_player_trait_sensors::set(const c_player_trait_sensors* traits, bool force)
{
	ASSERT(traits != nullptr);

	set_motion_tracker_setting(traits->get_motion_tracker_setting(), force);
	set_motion_tracker_range_setting(traits->get_motion_tracker_range_setting(), force);
}

void c_player_trait_sensors::encode_to_mcc(c_bitstream* packet) const
{
	ASSERT(packet != nullptr);

	e_motion_tracker_setting motion_tracker_setting = get_motion_tracker_setting();
	e_motion_tracker_range_setting motion_tracker_range_setting = get_motion_tracker_range_setting();

	packet->write_integer("player-traits-sensors-motion-tracker", motion_tracker_setting, 3);
	packet->write_integer("player-traits-sensors-motion-tracker-range", motion_tracker_range_setting, 3);
}

void c_player_trait_sensors::decode_from_mcc(c_bitstream* packet)
{
	ASSERT(packet != nullptr);

	e_motion_tracker_setting motion_tracker_setting = packet->read_enum<e_motion_tracker_setting, 3>("player-traits-sensors-motion-tracker");
	e_motion_tracker_range_setting motion_tracker_range_setting = packet->read_enum<e_motion_tracker_range_setting, 3>("player-traits-sensors-motion-tracker-range");

	set_motion_tracker_setting(motion_tracker_setting, true);
	set_motion_tracker_range_setting(motion_tracker_range_setting, true);
}

e_motion_tracker_setting c_player_trait_sensors::get_motion_tracker_setting() const
{
	return m_motion_tracker_setting;
}

void c_player_trait_sensors::set_motion_tracker_setting(e_motion_tracker_setting motion_tracker_setting, bool force)
{
	if (!VALID_INDEX(motion_tracker_setting, k_motion_tracker_settings))
	{
		event(_event_warning, "game_engine:player_trait:sensors: invalid motion_tracker setting '%ld'", motion_tracker_setting);

		if (force)
			m_motion_tracker_setting = _motion_tracker_setting_unchanged;
	}
	else if (motion_tracker_setting || force)
	{
		m_motion_tracker_setting = motion_tracker_setting;
	}
}

e_motion_tracker_range_setting c_player_trait_sensors::get_motion_tracker_range_setting() const
{
	return m_motion_tracker_range_setting;
}

void c_player_trait_sensors::set_motion_tracker_range_setting(e_motion_tracker_range_setting motion_tracker_range_setting, bool force)
{
	if (!VALID_INDEX(motion_tracker_range_setting, k_motion_tracker_range_settings))
	{
		event(_event_warning, "game_engine:player_trait:sensors: invalid motion_tracker_range setting '%ld'", motion_tracker_range_setting);

		if (force)
			m_motion_tracker_range_setting = _motion_tracker_range_setting_unchanged;
	}
	else if (motion_tracker_range_setting || force)
	{
		m_motion_tracker_range_setting = motion_tracker_range_setting;
	}
}

real32 c_player_trait_sensors::get_motion_tracker_range_meters() const
{
	e_motion_tracker_range_setting setting = get_motion_tracker_range_setting();

	const int32 minimum_value = _motion_tracker_range_setting_10_meters;
	if (IN_RANGE_INCLUSIVE(setting, minimum_value, _motion_tracker_range_setting_150_meters))
		return k_motion_tracker_range_values[setting - minimum_value];

	//return k_motion_tracker_range_values[k_motion_tracker_range_setting_default - minimum_value];
	return 0.0f;
}

