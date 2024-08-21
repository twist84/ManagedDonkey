#pragma once

#include "cseries/cseries.hpp"
#include "simulation/game_interface/simulation_game_entities.hpp"
#include "simulation/game_interface/simulation_game_events.hpp"

struct c_bitstream;
struct s_entity_update_data;
struct c_replication_outgoing_event;
struct s_simulation_entity;
struct s_simulation_view_telemetry_data;

struct c_simulation_entity_definition
{
public:
	virtual e_simulation_entity_type __cdecl entity_type() = 0;
	virtual char const* __cdecl entity_type_name() = 0;
	virtual long __cdecl state_data_size() = 0;
	virtual long __cdecl creation_data_size() = 0;
	virtual long __cdecl update_flag_count() = 0;
	virtual dword __cdecl initial_update_mask() = 0;
	virtual bool __cdecl entity_replication_required_for_view_activation(s_simulation_entity const*) = 0;
	virtual bool __cdecl entity_type_is_gameworld_object() = 0;
	virtual bool __cdecl gameworld_attachment_valid(long) = 0;
	virtual bool __cdecl entity_can_be_created(s_simulation_entity const*, s_simulation_view_telemetry_data const*) = 0;
	virtual void __cdecl creation_minimum_required_bits(s_simulation_entity const*, s_simulation_view_telemetry_data const*, long*) = 0;
	virtual void __cdecl write_creation_description_to_string(s_simulation_entity const*, s_simulation_view_telemetry_data const*, long, char*) = 0;
	virtual void __cdecl update_minimum_required_bits(s_simulation_entity const*, s_entity_update_data const*, long*) = 0;
	virtual void __cdecl entity_creation_encode(long, void const*, s_simulation_view_telemetry_data const*, c_bitstream*, bool) = 0;
	virtual bool __cdecl entity_creation_decode(long, void*, c_bitstream*, bool) = 0;
	virtual bool __cdecl entity_update_encode(bool, dword, dword*, long, void const*, s_simulation_view_telemetry_data const*, c_bitstream*, long, bool) = 0;
	virtual bool __cdecl entity_update_decode(bool, dword*, long, void*, c_bitstream*, bool) = 0;
	virtual bool __cdecl entity_state_lossy_compare(void*, void*, long) = 0;
	virtual bool __cdecl entity_creation_lossy_compare(void*, void*, long) = 0;
	virtual void __cdecl build_creation_data(long, long, void*) = 0;
	virtual bool __cdecl build_baseline_state_data(long, void const*, long, void*) = 0;
	virtual bool __cdecl build_updated_state_data(s_simulation_entity const*, dword*, long, void*) = 0;
	virtual void __cdecl rotate_entity_indices(s_simulation_entity const*) = 0;
	virtual void __cdecl prepare_creation_data_for_gameworld(long, void*) = 0;
	virtual void __cdecl prepare_state_data_for_gameworld(dword, long, void*) = 0;
	virtual bool __cdecl create_game_entity(long, long, void const*, dword, long, void const*) = 0;
	virtual bool __cdecl update_game_entity(long, dword, long, void const*) = 0;
	virtual bool __cdecl delete_game_entity(long) = 0;
	virtual bool __cdecl promote_game_entity_to_authority(long) = 0;
	virtual void __cdecl write_update_description_to_string(s_simulation_entity const*, s_entity_update_data const*, long, char*) = 0;
};

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
	virtual bool event_payload_decode(long, void*, c_bitstream*) = 0;
	virtual void prepare_event_data_for_gameworld(long, void*) = 0;
	virtual bool apply_game_event(long, long const*, long, void const*) = 0;

	c_simulation_event_definition()
	{
	};
};

struct c_simulation_type_collection
{
public:
	c_simulation_type_collection();
	~c_simulation_type_collection();

	void clear_types();
	void finish_types(long entity_type_count, long event_type_count);

	long get_entity_definition_count() const;
	c_simulation_entity_definition* get_entity_definition(e_simulation_entity_type) const;
	char const* get_entity_type_name(e_simulation_entity_type) const;
	void register_entity_definition(e_simulation_entity_type, c_simulation_entity_definition*);

	long get_event_definition_count() const;
	c_simulation_event_definition* get_event_definition(e_simulation_event_type) const;
	char const* get_event_type_name(e_simulation_event_type) const;
	void register_event_definition(e_simulation_event_type, c_simulation_event_definition*);

protected:
	long m_entity_type_count;
	c_static_array<c_simulation_entity_definition*, k_simulation_entity_type_maximum_count> m_entity_definitions;

	long m_event_type_count;
	c_static_array<c_simulation_event_definition*, k_simulation_event_type_maximum_count> m_event_definitions;
};
long const k_simulation_entity_type_maximum_size = sizeof(long) + (sizeof(c_simulation_entity_definition*) * k_simulation_entity_type_maximum_count);
long const k_simulation_event_type_maximum_size = sizeof(long) + (sizeof(c_simulation_event_definition*) * k_simulation_event_type_maximum_count);
static_assert(sizeof(c_simulation_type_collection) == k_simulation_entity_type_maximum_size + k_simulation_event_type_maximum_size);

