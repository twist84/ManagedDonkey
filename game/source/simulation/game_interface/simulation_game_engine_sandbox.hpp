#pragma once

#include "simulation/game_interface/simulation_game_interface.hpp"

struct c_simulation_sandbox_engine_globals_definition :
	public c_simulation_game_engine_globals_definition
{
public:
	//virtual e_simulation_entity_type entity_type();
	//virtual char const* entity_type_name();
	//virtual long state_data_size();
	//virtual long creation_data_size();
	//virtual long update_flag_count();
	//virtual uint64& initial_update_mask(uint64& mask);
	//virtual bool entity_replication_required_for_view_activation(s_simulation_entity const*);
	//virtual bool entity_type_is_gameworld_object();
	//virtual bool gameworld_attachment_valid(long);
	//virtual bool entity_can_be_created(s_simulation_entity const*, s_simulation_view_telemetry_data const*);
	//virtual void creation_minimum_required_bits(s_simulation_entity const*, s_simulation_view_telemetry_data const*, long*);
	//virtual void write_creation_description_to_string(s_simulation_entity const*, s_simulation_view_telemetry_data const*, long, char*);
	//virtual void update_minimum_required_bits(s_simulation_entity const*, s_entity_update_data const*, long*);
	//virtual void entity_creation_encode(long, void const*, s_simulation_view_telemetry_data const*, c_bitstream*, bool);
	//virtual bool entity_creation_decode(long, void*, c_bitstream*, bool);
	//virtual bool entity_update_encode(bool, uint32, uint32*, long, void const*, s_simulation_view_telemetry_data const*, c_bitstream*, long, bool);
	//virtual bool entity_update_decode(bool, uint32*, long, void*, c_bitstream*, bool);
	//virtual bool entity_state_lossy_compare(void*, void*, long);
	//virtual bool entity_creation_lossy_compare(void*, void*, long);
	//virtual void build_creation_data(long, long, void*);
	//virtual bool build_baseline_state_data(long, void const*, long, void*);
	//virtual bool build_updated_state_data(s_simulation_entity const*, uint32*, long, void*);
	//virtual void rotate_entity_indices(s_simulation_entity const*);
	//virtual void prepare_creation_data_for_gameworld(long, void*);
	//virtual void prepare_state_data_for_gameworld(uint32, long, void*);
	//virtual bool create_game_entity(long, long, void const*, uint32, long, void const*);
	//virtual bool update_game_entity(long, uint32, long, void const*);
	//virtual bool delete_game_entity(long);
	//virtual bool promote_game_entity_to_authority(long);
	//virtual void write_update_description_to_string(s_simulation_entity const*, s_entity_update_data const*, long, char*);

	static c_simulation_sandbox_engine_globals_definition& instance();
};

