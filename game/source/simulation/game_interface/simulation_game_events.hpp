#pragma once

#include "cseries/cseries.hpp"

enum e_simulation_event_type
{
	_simulation_event_type_damage_aftermath = 0,
	_simulation_event_type_damage_section_response,
	_simulation_event_type_breakable_surface_damage,
	_simulation_event_type_projectile_attached,
	_simulation_event_type_projectile_detonate,
	_simulation_event_type_projectile_impact_effect,
	_simulation_event_type_projectile_object_impact_effect,
	_simulation_event_type_effect_on_pos,
	_simulation_event_type_game_engine,
	_simulation_event_type_unit_board_vehicle,
	_simulation_event_type_unit_pickup,
	_simulation_event_type_weapon_effect,
	_simulation_event_type_weapon_empty_click,
	_simulation_event_type_hit_marker,
	_simulation_event_type_unit_exit_vehicle,
	_simulation_event_type_unit_assassinate,
	_simulation_event_type_player_taunt_request,
	_simulation_event_type_weapon_fire,
	_simulation_event_type_unit_equipment_use,
	_simulation_event_type_weapon_reload,
	_simulation_event_type_unit_throw_initiate,
	_simulation_event_type_unit_melee_initiate,
	_simulation_event_type_weapon_pickup,
	_simulation_event_type_weapon_put_away,
	_simulation_event_type_weapon_drop,
	_simulation_event_type_vehicle_flip,
	_simulation_event_type_vehicle_trick,
	_simulation_event_type_device_touch,
	_simulation_event_type_unit_throw_release,
	_simulation_event_type_unit_melee_damage,
	_simulation_event_type_unit_melee_clang,
	_simulation_event_type_unit_enter_vehicle,
	_simulation_event_type_game_engine_request_boot_player,
	_simulation_event_type_player_respawn_request,
	_simulation_event_type_player_force_base_respawn,
	_simulation_event_type_unit_equipment_pickup,
	_simulation_event_type_projectile_supercombine_request,
	_simulation_event_type_object_refresh,
	_simulation_event_type_player_editor_request,

	k_simulation_event_type_count,

	_simulation_event_type_hs_script_wake = k_simulation_event_type_count,

	k_simulation_event_type_count_plus_additional,

	k_simulation_event_type_none = -1,
	k_simulation_event_type_maximum_count = 64
};

struct c_replication_outgoing_event;
struct s_simulation_view_telemetry_data;
struct c_bitstream;
struct c_simulation_event_definition
{
public:
	virtual e_simulation_event_type event_type() = 0;
	virtual char const* event_type_name() = 0;
	virtual long payload_size() = 0;
	virtual long number_of_entity_references() = 0;
	virtual bool reference_delays_entity_deletion() = 0;
	virtual bool event_can_be_transmitted(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*) = 0;
	virtual long minimum_required_bits(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*, long*) = 0;
	virtual real calculate_relevance(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*, real) = 0;
	virtual void write_description_to_string(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*, real, long, char*) = 0;
	virtual long* maximum_required_bits(long* out_bits, c_replication_outgoing_event*, long) = 0; // unsure of the name, pc only?
	virtual void event_payload_encode(long, void const*, c_bitstream*) = 0;
	virtual bool event_payload_decode(long, void*, c_bitstream*, bool a4) = 0;
	virtual void prepare_event_data_for_gameworld(long, void*) = 0;
	virtual bool apply_game_event(long, long const*, long, void const*) = 0;

	c_simulation_event_definition()
	{
	};
};

