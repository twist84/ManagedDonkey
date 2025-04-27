#pragma once

#include "cseries/cseries.hpp"
#include "game/materials.hpp"
#include "objects/damage_owner.hpp"
#include "objects/damage_reporting.hpp"
#include "shell/shell.hpp"

struct s_damage_globals
{
	struct s_damage_acceleration
	{
		real_point3d epicenter;
		real_vector3d acceleration;
		long object_index;
		short node_index;
		uint16 flags;
	};
	static_assert(sizeof(s_damage_acceleration) == 0x20);

	long damage_acceleration_count;
	c_static_array<s_damage_acceleration, 64> damage_accelerations;
	c_static_flags<64> damage_accelerations_evictable;
	bool damage_acceleration_queue_active;
};
static_assert(sizeof(s_damage_globals) == 0x810);

struct s_projectile_material_response_definition;
struct c_aoe_damage_batchifier;
struct s_damage_data
{
	long definition_index;
	uint32 flags;
	s_damage_owner damage_owner;
	long collision_damage_object_index;
	long damage_creating_object_index;
	uint32 melee_damage_unique_identifier;
	s_location location;
	real_point3d origin;
	real_point3d epicenter;
	real_vector3d direction;
	real_vector3d aoe_force_direction;
	real32 scale;
	real32 shake_scale;
	real32 multiplier;
	real32 area_of_effect_distance;
	real32 area_of_effect_intensity;
	real32 area_of_effect_unknown68;
	bool area_of_effect_spike;
	bool area_of_effect_emp;
	real_vector3d area_of_effect_forward;
	long area_of_effect_spike_object;
	real32 material_effect_scale;
	c_global_material_type global_material_type;
	short shot_id;
	s_projectile_material_response_definition const* material_response;
	s_damage_reporting_info damage_reporting_info;
	c_aoe_damage_batchifier* optional_batchifier;
	long secret_melee_material_index;
};
static_assert(sizeof(s_damage_data) == 0x98);

struct s_model_material;
struct s_model_damage_info;
struct s_damage_response_data
{
	s_model_material const* model_material;
	s_model_damage_info const* damage_info;
	short body_part;
	long region_index;
	long node_index;
	real32 total_damage;
	real32 available_damage;
	real32 shield_damage;
	real32 section_damage;
	real32 body_damage;
	uint16 destroyed_constraints;
	uint16 loosened_constraints;
	uint16 being_damaged_flags;
	long special_death_type;
	bool supress_damage_effects;
};
static_assert(sizeof(s_damage_response_data) == 0x38);

struct s_damage_aftermath_result_data
{
	s_damage_reporting_info damage_reporting_info;
	c_enum<e_damage_aftermath_flags, long, _damage_aftermath_body_depleted_bit, k_damage_aftermath_flags> flags;
	long damage_definition_index;
	s_damage_owner damage_owner;
	real_vector3d direction;
	bool epicenter_valid;
	real_vector3d epicenter;
	real32 shake;
	real32 shake_scale;
	long body_part;
	short node_index;
	real32 body_damage;
	real32 shield_damage;
	real32 area_of_effect_distance;
	long ping_type;
	long special_death_type;
};
static_assert(sizeof(s_damage_aftermath_result_data) == 0x58);

struct object_damage_section
{
	uint16 response_flags;
	uint8 current_damage;
	uint8 timed_response_damage_info;
	uint16 timed_response_index : 4;
	uint16 timed_response_timer : 12;
	short damage_section_recharge_ticks;
};
static_assert(sizeof(object_damage_section) == 0x8);

extern bool debug_damage_radius;
extern bool debug_damage_this_event;
extern bool debug_damage_verbose;
extern bool debug_player_damage;
extern bool debug_damage;
extern long global_debug_damage_object_index;

extern real32 __cdecl compute_total_damage(s_damage_data* damage_data, void* damage_effect_definition, void const* damage_definition, long object_index, bool* a5);
extern void __cdecl damage_acceleration_apply(s_damage_globals::s_damage_acceleration const* damage_acceleration);
extern void __cdecl damage_acceleration_queue_begin();
extern void __cdecl damage_acceleration_queue_end();
extern void __cdecl damage_data_new(s_damage_data* damage_data, long definition_index);
extern void __cdecl damage_initialize_for_new_map();
extern void __cdecl damage_update();
extern void __cdecl object_cause_damage(s_damage_data* damage_data, long object_index, short node_index, short region_index, short material_index, long predictability);
extern void __cdecl object_cause_damage_simple(s_damage_data* damage_data, long object_index, long predictability);
extern s_model_damage_info const* __cdecl object_get_damage_info(long object_index);

extern void render_debug_object_damage();

