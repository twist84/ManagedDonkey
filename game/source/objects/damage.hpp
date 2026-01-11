#pragma once

#include "cseries/cseries.hpp"
#include "cseries/location.hpp"
#include "game/materials.hpp"
#include "objects/damage_owner.hpp"
#include "objects/damage_reporting.hpp"

struct s_damage_globals
{
	struct s_damage_acceleration
	{
		real_point3d epicenter;
		real_vector3d acceleration;
		int32 object_index;
		int16 node_index;
		uns16 flags;
	};
	static_assert(sizeof(s_damage_acceleration) == 0x20);

	int32 damage_acceleration_count;
	c_static_array<s_damage_acceleration, 64> damage_accelerations;
	c_static_flags<64> damage_accelerations_evictable;
	bool damage_acceleration_queue_active;
};
static_assert(sizeof(s_damage_globals) == 0x810);

struct s_projectile_material_response_definition;
class c_aoe_damage_batchifier;
struct s_damage_data
{
	int32 definition_index;
	uns32 flags;
	s_damage_owner damage_owner;
	int32 collision_damage_object_index;
	int32 damage_creating_object_index;
	uns32 melee_damage_unique_identifier;
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
	int32 area_of_effect_spike_object;
	real32 material_effect_scale;
	c_global_material_type global_material_type;
	int16 shot_id;
	const s_projectile_material_response_definition* material_response;
	s_damage_reporting_info damage_reporting_info;
	c_aoe_damage_batchifier* optional_batchifier;
	int32 secret_melee_material_index;
};
static_assert(sizeof(s_damage_data) == 0x98);

struct s_model_material;
struct s_model_damage_info;
struct s_damage_response_data
{
	const s_model_material* model_material;
	const s_model_damage_info* damage_info;
	int16 body_part;
	int32 region_index;
	int32 node_index;
	real32 total_damage;
	real32 available_damage;
	real32 shield_damage;
	real32 section_damage;
	real32 body_damage;
	uns16 destroyed_constraints;
	uns16 loosened_constraints;
	uns16 being_damaged_flags;
	int32 special_death_type;
	bool supress_damage_effects;
};
static_assert(sizeof(s_damage_response_data) == 0x38);

struct s_damage_aftermath_result_data
{
	s_damage_reporting_info damage_reporting_info;
	c_enum<e_damage_aftermath_flags, int32, _damage_aftermath_body_depleted_bit, k_damage_aftermath_flags> flags;
	int32 damage_definition_index;
	s_damage_owner damage_owner;
	real_vector3d direction;
	bool epicenter_valid;
	real_vector3d epicenter;
	real32 shake;
	real32 shake_scale;
	int32 body_part;
	int16 node_index;
	real32 body_damage;
	real32 shield_damage;
	real32 area_of_effect_distance;
	int32 ping_type;
	int32 special_death_type;
};
static_assert(sizeof(s_damage_aftermath_result_data) == 0x58);

struct object_damage_section
{
	uns16 response_flags;
	uns8 current_damage;
	uns8 timed_response_damage_info;
	uns16 timed_response_index : 4;
	uns16 timed_response_timer : 12;
	int16 damage_section_recharge_ticks;
};
static_assert(sizeof(object_damage_section) == 0x8);

extern bool debug_damage_radius;
extern bool debug_damage_this_event;
extern bool debug_damage_verbose;
extern bool debug_player_damage;
extern bool debug_damage;
extern int32 global_debug_damage_object_index;

extern int32 __cdecl area_of_effect_cause_damage(s_damage_data* damage_data, int32 ignore_object_index, int32 predictability);
extern real32 __cdecl compute_total_damage(s_damage_data* damage_data, void* damage_effect_definition, const void* damage_definition, int32 object_index, bool* a5);
extern void __cdecl damage_acceleration_apply(const s_damage_globals::s_damage_acceleration* damage_acceleration);
extern void __cdecl damage_acceleration_queue_begin();
extern void __cdecl damage_acceleration_queue_end();
extern void __cdecl damage_data_new(s_damage_data* damage_data, int32 definition_index);
extern void __cdecl damage_initialize_for_new_map();
extern void __cdecl damage_update();
extern void __cdecl object_damage_damage_section(int32 object_index, int32 damage_section_string_id, real32 damage);
extern bool __cdecl object_model_target_destroyed(int32 object_index, int32 model_target_name_id);
extern int16 __cdecl object_model_targets_destroyed(int32 object_index, int32 model_target_name_id);
extern void __cdecl object_scripting_cannot_die(int32 object_index, bool cannot_die);
extern bool __cdecl object_scripting_vitality_pinned(int32 object_index);
extern void __cdecl object_can_take_damage(int32 object_list_index);
extern void __cdecl object_cannot_take_damage(int32 object_list_index);
extern void __cdecl object_cause_damage(s_damage_data* damage_data, int32 object_index, int16 node_index, int16 region_index, int16 material_index, int32 predictability);
extern void __cdecl object_cause_damage_simple(s_damage_data* damage_data, int32 object_index, int32 predictability);
extern void __cdecl object_deplete_body(int32 object_index, const s_damage_owner* depleter, const s_damage_reporting_info& damage_reporting_info);
extern const s_model_damage_info* __cdecl object_get_damage_info(int32 object_index);
extern void __cdecl object_kill_instantly(int32 object_index, bool silent, bool no_statistics, bool bypass_cheats, const s_damage_owner* damage_owner, const s_damage_reporting_info& damage_reporting_info);

extern void render_debug_object_damage();

