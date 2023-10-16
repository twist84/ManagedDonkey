#pragma once

#include "cseries/cseries.hpp"

enum e_simulation_entity_type
{
	_simulation_entity_type_slayer = 0,
	_simulation_entity_type_ctf,
	_simulation_entity_type_assault,
	_simulation_entity_type_oddball,
	_simulation_entity_type_king,
	_simulation_entity_type_territories,
	_simulation_entity_type_juggernaut,
	_simulation_entity_type_sandbox,
	_simulation_entity_type_infection,
	_simulation_entity_type_vip,
	_simulation_entity_type_game_engine_player,
	_simulation_entity_type_game_statborg,
	_simulation_entity_type_breakable_surface_group,
	_simulation_entity_type_map_variant,
	_simulation_entity_type_unit,
	_simulation_entity_type_item,
	_simulation_entity_type_generic,
	_simulation_entity_type_generic_garbage,
	_simulation_entity_type_vehicle,
	_simulation_entity_type_projectile,
	_simulation_entity_type_weapon,
	_simulation_entity_type_device,

	k_simulation_entity_type_count,

	k_simulation_entity_type_none = -1,
	k_simulation_entity_type_maximum_count = 32
};

enum e_simulation_event_type
{
	_event_simulation_type_damage_aftermath = 0,
	_event_simulation_type_damage_section_response,
	_event_simulation_type_breakable_surface_damage,
	_event_simulation_type_projectile_attached,
	_event_simulation_type_projectile_detonate,
	_event_simulation_type_projectile_impact_effect,
	_event_simulation_type_projectile_object_impact_effect,
	_event_simulation_type_effect_on_pos,
	_event_simulation_type_game_engine,
	_event_simulation_type_unit_board_vehicle,
	_event_simulation_type_unit_pickup,
	_event_simulation_type_weapon_effect,
	_event_simulation_type_weapon_empty_click,
	_event_simulation_type_hit_marker,
	_event_simulation_type_unit_exit_vehicle,
	_event_simulation_type_unit_assassinate,
	_event_simulation_type_player_taunt_request,
	_event_simulation_type_weapon_fire,
	_event_simulation_type_unit_equipment_use,
	_event_simulation_type_weapon_reload,
	_event_simulation_type_unit_throw_initiate,
	_event_simulation_type_unit_melee_initiate,
	_event_simulation_type_weapon_pickup,
	_event_simulation_type_weapon_put_away,
	_event_simulation_type_weapon_drop,
	_event_simulation_type_vehicle_flip,
	_event_simulation_type_vehicle_trick,
	_event_simulation_type_device_touch,
	_event_simulation_type_unit_throw_release,
	_event_simulation_type_unit_melee_damage,
	_event_simulation_type_unit_melee_clang,
	_event_simulation_type_unit_enter_vehicle,
	_event_simulation_type_game_engine_request_boot_player,
	_event_simulation_type_player_respawn_request,
	_event_simulation_type_player_force_base_respawn,
	_event_simulation_type_unit_equipment_pickup,
	_event_simulation_type_projectile_supercombine_request,
	_event_simulation_type_object_refresh,
	_event_simulation_type_player_editor_request,

	//_event_simulation_type_hs_script_wake,

	k_simulation_event_type_count,

	k_simulation_event_type_none = -1,
	k_simulation_event_type_maximum_count = 64
};

struct c_bitstream;
struct s_entity_update_data;
struct c_replication_outgoing_event;
struct s_simulation_entity;
struct s_simulation_view_telemetry_data;

struct c_simulation_entity_definition
{
public:
	virtual e_simulation_entity_type __cdecl entity_type();
	virtual char const* __cdecl entity_type_name();
	virtual long __cdecl state_data_size();
	virtual long __cdecl creation_data_size();
	virtual long __cdecl update_flags();
	virtual dword __cdecl initial_update_mask();
	virtual bool __cdecl entity_replication_required_for_view_activation(s_simulation_entity const*);
	virtual bool __cdecl entity_type_is_gameworld_object();
	virtual bool __cdecl gameworld_attachment_valid(long);
	virtual bool __cdecl entity_can_be_created(s_simulation_entity const*, s_simulation_view_telemetry_data const*);
	virtual void __cdecl creation_minimum_required_bits(s_simulation_entity const*, s_simulation_view_telemetry_data const*, long*);
	virtual void __cdecl write_creation_description_to_string(s_simulation_entity const*, s_simulation_view_telemetry_data const*, long, char*);
	virtual void __cdecl update_minimum_required_bits(s_simulation_entity const*, s_entity_update_data const*, long*);
	virtual void __cdecl entity_creation_encode(long, void const*, s_simulation_view_telemetry_data const*, c_bitstream*, bool);
	virtual bool __cdecl entity_creation_decode(long, void*, c_bitstream*, bool);
	virtual bool __cdecl entity_update_encode(bool, dword, dword*, long, void const*, s_simulation_view_telemetry_data const*, c_bitstream*, long, bool);
	virtual bool __cdecl entity_update_decode(bool, dword*, long, void*, c_bitstream*, bool);
	virtual bool __cdecl entity_state_lossy_compare(void*, void*, long);
	virtual bool __cdecl entity_creation_lossy_compare(void*, void*, long);
	virtual void __cdecl build_creation_data(long, long, void*);
	virtual bool __cdecl build_baseline_state_data(long, void const*, long, void*);
	virtual bool __cdecl build_updated_state_data(s_simulation_entity const*, dword*, long, void*);
	virtual void __cdecl rotate_entity_indices(s_simulation_entity const*);
	virtual void __cdecl prepare_creation_data_for_gameworld(long, void*);
	virtual void __cdecl prepare_state_data_for_gameworld(dword, long, void*);
	virtual bool __cdecl create_game_entity(long, long, void const*, dword, long, void const*);
	virtual bool __cdecl update_game_entity(long, dword, long, void const*);
	virtual bool __cdecl delete_game_entity(long);
	virtual bool __cdecl promote_game_entity_to_authority(long);
	virtual void __cdecl write_update_description_to_string(s_simulation_entity const*, s_entity_update_data const*, long, char*);

protected:
	// TODO: figure out any data
};

struct c_simulation_event_definition
{
public:
	virtual e_simulation_event_type __cdecl event_type() = 0;
	virtual char const* __cdecl event_type_name() = 0;
	virtual long __cdecl payload_size() = 0;
	virtual long __cdecl number_of_entity_references() = 0;
	virtual bool __cdecl reference_delays_entity_deletion() = 0;
	virtual bool __cdecl event_can_be_transmitted(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*) = 0;
	virtual long __cdecl minimum_required_bits(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*, long*) = 0;
	virtual real __cdecl calculate_relevance(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*, real) = 0;
	virtual void __cdecl write_description_to_string(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*, real, long, char*) = 0;

	// TODO: figure it out
	virtual void* __cdecl __func9(dword* out_flags, void* event) = 0;

	virtual void __cdecl event_payload_encode(long, void const*, c_bitstream*) = 0;
	virtual bool __cdecl event_payload_decode(long, void*, c_bitstream*) = 0;
	virtual void __cdecl prepare_event_data_for_gameworld(long, void*) = 0;
	virtual bool __cdecl apply_game_event(long, long const*, long, void const*) = 0;

protected:
	// TODO: figure out any data
};

struct c_simulation_type_collection
{
	long m_entity_type_count;
	c_static_array<c_simulation_entity_definition*, k_simulation_entity_type_maximum_count> m_entity_definitions;

	long m_event_type_count;
	c_static_array<c_simulation_event_definition*, k_simulation_event_type_maximum_count> m_event_definitions;
};

