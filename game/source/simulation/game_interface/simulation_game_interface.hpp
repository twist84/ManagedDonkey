#pragma once

#include "simulation/simulation_type_collection.hpp"

struct c_simulation_game_engine_globals_definition :
	public c_simulation_entity_definition
{
	////virtual e_simulation_entity_type __cdecl entity_type();
	////virtual char const* __cdecl entity_type_name();
	////virtual long __cdecl state_data_size();
	//virtual long __cdecl creation_data_size();
	////virtual long __cdecl update_flag_count();
	////virtual dword __cdecl initial_update_mask();
	//virtual bool __cdecl entity_replication_required_for_view_activation(s_simulation_entity const*);
	//virtual bool __cdecl entity_type_is_gameworld_object();
	//virtual bool __cdecl gameworld_attachment_valid(long);
	//virtual bool __cdecl entity_can_be_created(s_simulation_entity const*, s_simulation_view_telemetry_data const*);
	//virtual void __cdecl creation_minimum_required_bits(s_simulation_entity const*, s_simulation_view_telemetry_data const*, long*);
	//virtual void __cdecl write_creation_description_to_string(s_simulation_entity const*, s_simulation_view_telemetry_data const*, long, char*);
	////virtual void __cdecl update_minimum_required_bits(s_simulation_entity const*, s_entity_update_data const*, long*);
	//virtual void __cdecl entity_creation_encode(long, void const*, s_simulation_view_telemetry_data const*, c_bitstream*, bool);
	//virtual bool __cdecl entity_creation_decode(long, void*, c_bitstream*, bool);
	////virtual bool __cdecl entity_update_encode(bool, dword, dword*, long, void const*, s_simulation_view_telemetry_data const*, c_bitstream*, long, bool);
	////virtual bool __cdecl entity_update_decode(bool, dword*, long, void*, c_bitstream*, bool);
	////virtual bool __cdecl entity_state_lossy_compare(void*, void*, long);
	//virtual bool __cdecl entity_creation_lossy_compare(void*, void*, long);
	//virtual void __cdecl build_creation_data(long, long, void*);
	//virtual bool __cdecl build_baseline_state_data(long, void const*, long, void*);
	//virtual bool __cdecl build_updated_state_data(s_simulation_entity const*, dword*, long, void*);
	//virtual void __cdecl rotate_entity_indices(s_simulation_entity const*);
	//virtual void __cdecl prepare_creation_data_for_gameworld(long, void*);
	//virtual void __cdecl prepare_state_data_for_gameworld(dword, long, void*);
	//virtual bool __cdecl create_game_entity(long, long, void const*, dword, long, void const*);
	//virtual bool __cdecl update_game_entity(long, dword, long, void const*);
	//virtual bool __cdecl delete_game_entity(long);
	//virtual bool __cdecl promote_game_entity_to_authority(long);
	////virtual void __cdecl write_update_description_to_string(s_simulation_entity const*, s_entity_update_data const*, long, char*);
};

extern void __cdecl simulation_game_register_types(c_simulation_type_collection* type_collection, long* entity_type_count, long* event_type_count);
extern void __cdecl simulation_game_register_additionnal_types(c_simulation_type_collection* type_collection, long* entity_type_count, long* event_type_count);

