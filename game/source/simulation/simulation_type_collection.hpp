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
	virtual float __cdecl calculate_relevance(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*, real) = 0;
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

// #TODO: find these `s_simulation_*_event_data` a home
struct s_simulation_damage_aftermath_event_data
{
	byte __data[0x50];
};
static_assert(sizeof(s_simulation_damage_aftermath_event_data) == 0x50);

struct s_simulation_damage_section_response_event_data
{
	byte __data[0xC];
};
static_assert(sizeof(s_simulation_damage_section_response_event_data) == 0xC);

struct s_simulation_breakable_surface_damage_event_data
{
	byte __data[0x18];
};
static_assert(sizeof(s_simulation_breakable_surface_damage_event_data) == 0x18);

struct s_simulation_projectile_attached_event_data
{
	byte __data[0x1C];
};
static_assert(sizeof(s_simulation_projectile_attached_event_data) == 0x1C);

struct s_simulation_projectile_detonate_event_data
{
	byte __data[0x40];
};
static_assert(sizeof(s_simulation_projectile_detonate_event_data) == 0x40);

struct s_simulation_projectile_impact_effect_event_data
{
	byte __data[0x38];
};
static_assert(sizeof(s_simulation_projectile_impact_effect_event_data) == 0x38);

struct s_simulation_projectile_object_impact_effect_event_data
{
	byte __data[0x44];
};
static_assert(sizeof(s_simulation_projectile_object_impact_effect_event_data) == 0x44);

struct s_simulation_effect_on_pos_event_data
{
	byte __data[0x10];
};
static_assert(sizeof(s_simulation_effect_on_pos_event_data) == 0x10);

struct s_simulation_game_engine_event_data
{
	byte __data[0x28];
};
static_assert(sizeof(s_simulation_game_engine_event_data) == 0x28);

struct s_simulation_unit_board_vehicle_event_data
{
	byte __data[0x4];
};
static_assert(sizeof(s_simulation_unit_board_vehicle_event_data) == 0x4);

struct s_simulation_unit_pickup_event_event_data
{
	byte __data[0xC];
};
static_assert(sizeof(s_simulation_unit_pickup_event_event_data) == 0xC);

struct s_simulation_weapon_effect_event_data
{
	byte __data[0x10];
};
static_assert(sizeof(s_simulation_weapon_effect_event_data) == 0x10);

struct s_simulation_weapon_empty_click_event_data
{
	byte __data[0x8];
};
static_assert(sizeof(s_simulation_weapon_empty_click_event_data) == 0x8);

//struct s_simulation_hit_marker_event_data
//{
//	byte __data[0x0];
//};
//static_assert(sizeof(s_simulation_hit_marker_event_data) == 0x0);

struct s_simulation_unit_exit_vehicle_event_data
{
	byte __data[0x8];
};
static_assert(sizeof(s_simulation_unit_exit_vehicle_event_data) == 0x8);

struct s_simulation_unit_assassinate_event_data
{
	byte __data[0x64];
};
static_assert(sizeof(s_simulation_unit_assassinate_event_data) == 0x64);

struct s_simulation_player_taunt_request_event_data
{
	byte __data[0x4];
};
static_assert(sizeof(s_simulation_player_taunt_request_event_data) == 0x4);

struct s_simulation_weapon_fire_event_event_data
{
	byte __data[0x354];
};
static_assert(sizeof(s_simulation_weapon_fire_event_event_data) == 0x354);

struct s_simulation_unit_equipment_use_event_data
{
	byte __data[0x8];
};
static_assert(sizeof(s_simulation_unit_equipment_use_event_data) == 0x8);

struct s_simulation_weapon_reload_event_data
{
	byte __data[0x3];
};
static_assert(sizeof(s_simulation_weapon_reload_event_data) == 0x3);

struct s_simulation_unit_throw_initiate_event_data
{
	byte __data[0x8];
};
static_assert(sizeof(s_simulation_unit_throw_initiate_event_data) == 0x8);

struct s_simulation_unit_melee_initiate_event_data
{
	byte __data[0x4];
};
static_assert(sizeof(s_simulation_unit_melee_initiate_event_data) == 0x4);

struct s_simulation_weapon_pickup_event_data
{
	byte __data[0x8];
};
static_assert(sizeof(s_simulation_weapon_pickup_event_data) == 0x8);

struct s_simulation_weapon_put_away_event_data
{
	byte __data[0x8];
};
static_assert(sizeof(s_simulation_weapon_put_away_event_data) == 0x8);

struct s_simulation_weapon_drop_event_data
{
	byte __data[0x8];
};
static_assert(sizeof(s_simulation_weapon_drop_event_data) == 0x8);

//struct s_simulation_vehicle_flip_event_data
//{
//	byte __data[0x0];
//};
//static_assert(sizeof(s_simulation_vehicle_flip_event_data) == 0x0);

struct s_simulation_vehicle_trick_event_data
{
	byte __data[0x4];
};
static_assert(sizeof(s_simulation_vehicle_trick_event_data) == 0x4);

//struct s_simulation_device_touch_event_data
//{
//	byte __data[0x0];
//};
//static_assert(sizeof(s_simulation_device_touch_event_data) == 0x0);

struct s_simulation_unit_throw_release_event_data
{
	byte __data[0x40];
};
static_assert(sizeof(s_simulation_unit_throw_release_event_data) == 0x40);

struct s_simulation_unit_melee_damage_event_data
{
	byte __data[0x3C];
};
static_assert(sizeof(s_simulation_unit_melee_damage_event_data) == 0x3C);

struct s_simulation_unit_melee_clang_event_data
{
	byte __data[0x1C];
};
static_assert(sizeof(s_simulation_unit_melee_clang_event_data) == 0x1C);

struct s_simulation_unit_enter_vehicle_event_data
{
	byte __data[0x4];
};
static_assert(sizeof(s_simulation_unit_enter_vehicle_event_data) == 0x4);

struct s_simulation_game_engine_request_boot_player_event_data
{
	byte __data[0x8];
};
static_assert(sizeof(s_simulation_game_engine_request_boot_player_event_data) == 0x8);

struct s_simulation_player_respawn_request_event_data
{
	byte __data[0x4];
};
static_assert(sizeof(s_simulation_player_respawn_request_event_data) == 0x4);

struct s_simulation_player_force_base_respawn_event_data
{
	byte __data[0x2];
};
static_assert(sizeof(s_simulation_player_force_base_respawn_event_data) == 0x2);

struct s_simulation_unit_equipment_pickup_event_data
{
	byte __data[0x2];
};
static_assert(sizeof(s_simulation_unit_equipment_pickup_event_data) == 0x2);

struct s_simulation_projectile_supercombine_request_event_data
{
	byte __data[0x4];
};
static_assert(sizeof(s_simulation_projectile_supercombine_request_event_data) == 0x4);

struct s_simulation_object_refresh_event_data
{
	byte __data[0x4];
};
static_assert(sizeof(s_simulation_object_refresh_event_data) == 0x4);

struct s_simulation_player_editor_request_event_data
{
	byte __data[0x78];
};
static_assert(sizeof(s_simulation_player_editor_request_event_data) == 0x78);

//struct s_simulation_hs_script_wake_event_data
//{
//	char script_name[32];
//	long script_index;
//};
//static_assert(sizeof(s_simulation_hs_script_wake_event_data) == 0x24);

/*

bool __cdecl hs_wake_by_name(char const* script_name)
{
	return INVOKE(0x00599250, hs_wake_by_name, script_name);
}

long __cdecl hs_find_thread_by_name(char const* script_name)
{
	return INVOKE(0x00596070, hs_find_thread_by_name, script_name);
}

hs_thread* __cdecl hs_thread_get(long thread_index)
{
	TLS_DATA_GET_VALUE_REFERENCE(hs_thread_deterministic_data);

	hs_thread* result = static_cast<hs_thread*>(datum_try_and_get(hs_thread_deterministic_data, thread_index));
	return result;
}

void __cdecl hs_find_dormant_script(char const* dormant_script_name, long* script_index_out)
{
	ASSERT(dormant_script_name);
	ASSERT(script_index_out);

	*script_index_out = NONE;

	long thread_index = hs_find_thread_by_name(dormant_script_name);
	if (thread_index == NONE)
		return;

	hs_thread* thread = hs_thread_get(thread_index);

	if (global_scenario_get()->scripts[thread->script_index].script_type == _hs_script_dormant)
		*script_index_out = thread->script_index;
}

struct c_simulation_hs_script_wake_event_definition :
	public c_simulation_event_definition
{
	virtual e_simulation_event_type __cdecl event_type() override
	{
		return _event_simulation_type_hs_script_wake;
	}

	virtual char const* __cdecl event_type_name() override
	{
		return "hs_script_wake";
	}

	virtual long __cdecl payload_size() override
	{
		return sizeof(s_simulation_hs_script_wake_event_data);
	}

	virtual long __cdecl number_of_entity_references() override
	{
		return 0;
	}

	virtual bool __cdecl reference_delays_entity_deletion() override
	{
		return 0;
	}

	virtual bool __cdecl event_can_be_transmitted(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*) override
	{
		return true;
	}

	virtual long __cdecl minimum_required_bits(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*, long*) override
	{
		return 42;
	}

	virtual real __cdecl calculate_relevance(c_replication_outgoing_event const*, s_simulation_view_telemetry_data const*, real) override
	{
		return 0.0f;
	}

	virtual void __cdecl write_description_to_string(c_replication_outgoing_event const* a1, s_simulation_view_telemetry_data const* a2, real a3, long buffer_count, char* buffer) override
	{
		csnzprintf(buffer, buffer_count, "hs-script-wake : relevance=%5.3f", calculate_relevance(a1, a2, a3));
	}

	// TODO: figure it out
	virtual void* __cdecl __func9(dword* out_flags, void*) override
	{
		*out_flags = 0x5500;
	}

	virtual void __cdecl event_payload_encode(long payload_size, void const* payload, c_bitstream* packet) override
	{
		ASSERT(payload_size == c_simulation_hs_script_wake_event_definition::payload_size());
		ASSERT(payload);
		ASSERT(packet);

		s_simulation_hs_script_wake_event_definition const* definition = static_cast<s_simulation_hs_script_wake_event_definition const*>(payload);

		packet->write_integer("script-index", definition->script_index, 10);
		packet->write_string("script-name", definition->script_name, 32);
	}

	virtual bool __cdecl event_payload_decode(long payload_size, void* payload, c_bitstream* packet) override
	{
		ASSERT(payload_size == c_simulation_hs_script_wake_event_definition::payload_size());
		ASSERT(payload);
		ASSERT(packet);

		s_simulation_hs_script_wake_event_definition* definition = static_cast<s_simulation_hs_script_wake_event_definition*>(payload);

		definition->script_index = packet->read_integer("script-index", 10);
		packet->read_string("script-name", definition->script_name, 32);

		return definition->script_index != NONE && strlen(definition->script_name);
	}

	virtual void __cdecl prepare_event_data_for_gameworld(long, void*) override
	{
	}

	virtual bool __cdecl apply_game_event(long entity_reference_count, long const*, long payload_size, void const* payload) override
	{
		ASSERT(entity_reference_count == number_of_entity_references());
		ASSERT(payload_size == c_simulation_hs_script_wake_event_definition::payload_size());
		ASSERT(payload);

		s_simulation_hs_script_wake_event_definition const* definition = static_cast<s_simulation_hs_script_wake_event_definition const*>(payload);

		if (definition->script_index == NONE)
		{
			c_console::write_line("networking:simulation:hs: invalid script index in hs wake event");
			return;
		}

		long script_index = NONE;
		hs_find_dormant_script(definition->script_name, &script_index);
		if (script_index != definition->script_index)
		{
			c_console::write_line("networking:simulation:hs: script index mismatch in hs wake event [0x%08X != 0x%08X]", script_index, definition->script_index);
		}

		hs_wake_by_name(definition->script_name);
	}
};

*/

struct c_simulation_type_collection
{
	long m_entity_type_count;
	c_static_array<c_simulation_entity_definition*, k_simulation_entity_type_maximum_count> m_entity_definitions;

	long m_event_type_count;
	c_static_array<c_simulation_event_definition*, k_simulation_event_type_maximum_count> m_event_definitions;
};

