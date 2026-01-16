#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

enum e_model_definition_flags
{
	_model_definition_flag_active_camo_always_on_bit = 0,
	_model_definition_flag_active_camo_never_bit,
	_model_definition_flag_has_shield_impact_effect_bit,
	_model_definition_flag_model_use_sky_lighting_bit,

	// used in magnetism and campaign saving
	_model_definition_flag_inconsequential_target_bit,

	_model_definition_flag_model_use_airprobe_lighting_bit = 0,
	_model_definition_flag_bit6,
	_model_definition_flag_bit7,
	_model_definition_flag_bit8,

	k_model_definition_flags
};

enum e_model_definition_private_flags
{
	_model_definition_private_flag_contains_runtime_nodes_bit = 0,

	k_model_definition_private_flags
};

enum e_model_self_shadow_bounces
{
	// very fast, direct light only
	_model_self_shadow_0_bounces = 0,

	// slower, pretty good
	_model_self_shadow_1_bounce,

	// molasses, very good
	_model_self_shadow_2_bounces,

	// sloth in molasses, overkill really
	_model_self_shadow_3_bounces,

	k_model_self_shadow_bounce_count
};

enum e_model_self_shadow_detail
{
	// 2 bytes per vertex
	_model_self_shadow_detail_ambient_occlusion = 0,

	// 8 bytes per vertex
	_model_self_shadow_detail_linear,

	// 18 bytes per vertex
	_model_self_shadow_detail_quadratic,

	// 32 bytes per vertex
	_model_self_shadow_detail_cubic,

	k_model_self_shadow_detail_count
};

struct s_model_variant;
class c_model_instance_group;
struct s_model_material;
struct s_model_damage_info;
struct s_model_target;
struct s_model_definition
{
	static tag const k_group_tag = MODEL_TAG;

	c_typed_tag_reference<RENDER_MODEL_TAG, INVALID_TAG> render_model;
	c_typed_tag_reference<COLLISION_MODEL_TAG, INVALID_TAG> collision_model;
	c_typed_tag_reference<MODEL_ANIMATION_GRAPH_TAG, INVALID_TAG> animation;
	c_typed_tag_reference<PHYSICS_MODEL_TAG, INVALID_TAG> physics_model;


	// level of detail

	// If a model is further away than the LOD reduction distance, it will be reduced to that LOD.
	// So the L1 reduction distance should be really long and the L5 reduction distance should be really int16.
	// This means distances should be in descending order, e.g. 5 4 3 2 1.
	// 
	// Note that if we run out of memory or too many models are on screen at once, the engine may reduce
	// models to a lower LOD BEFORE they reach the reduction distance for that LOD.
	// 
	// If a model has a begin fade distance and disappear distance, it will begin fading out at that distance,
	// reaching zero alpha and disappearing at the disappear distance. These distances should be greater than all
	// of the LOD reduction distances.

	real32 disappear_distance; // world units
	real32 begin_fade_distance; // world units
	real32 animation_lod_distance; // world units
	real32 reduce_to_L1; // world units (low)
	real32 instance_disappear_distance; // world units
	c_typed_tag_reference<RENDER_MODEL_TAG, INVALID_TAG> lod_render_model;


	c_typed_tag_block<s_model_variant> variants;
	c_typed_tag_block<c_string_id> region_sort;
	c_typed_tag_block<c_model_instance_group> instance_groups;
	c_typed_tag_block<s_model_material> materials;
	c_typed_tag_block<s_model_damage_info> new_damage_info;
	c_typed_tag_block<s_model_target> targets;
	s_tag_block runtime_regions;
	s_tag_block runtime_nodes;
	uns32 runtime_node_list_checksum;
	s_tag_block model_object_data;


	// more stuff

	// The default dialogue tag for this model (overriden by variants)
	c_typed_tag_reference<DIALOGUE_TAG, INVALID_TAG> primary_dialogue;

	// The default FEMALE dialogue tag for this model (overriden by variants)
	c_typed_tag_reference<DIALOGUE_TAG, INVALID_TAG> secondary_dialogue;

	c_flags<e_model_definition_flags, uns32, k_model_definition_flags> flags;

	// The default dialogue tag for this model (overriden by variants)
	c_string_id default_dialogue_effect;

	uns32 render_only_node_flags[8];
	uns32 render_only_section_flags[8];
	c_flags<e_model_definition_private_flags, uns32, k_model_definition_private_flags> runtime_flags;
	s_tag_block scenario_load_parameters;


	// PRT Shadows (soft self-shadow)

	// These parameters control the generation of PRT shadows.
	// 
	// By default, the shadows on each permutation are computed using the first permutation in
	// every other region as shadow casters.  You can override this behavior below by specifying
	// which permutation to use as a shadow caster in a given region.
	// 
	//   PRT shadow detail:
	//     ambient occlusion means the shadows are static: they don't respond to
	//     changing directions of light, only the total amount of light.
	//     increasing the detail makes the shadows respond in an increasingly sharp
	//     fashion to different directions of light, capturing sharper shadows.
	//   PRT shadow bounces:
	//     the number of light bounces to use when computing the global illumination.
	//     (0 bounces gets only direct light).  Increasing the number of bounces
	//     increases the calculation time.  1 or 2 bounces should be good enough for
	//     almost all models.

	c_enum<e_model_self_shadow_detail, int8, _model_self_shadow_detail_ambient_occlusion, k_model_self_shadow_detail_count> PRT_shadow_detail;
	c_enum<e_model_self_shadow_bounces, int8, _model_self_shadow_0_bounces, k_model_self_shadow_bounce_count> PRT_shadow_bounces;
	byte NCFGBA[0x2];


	s_tag_block shadow_cast_override;
	s_tag_block shadow_receive_override;
	s_tag_block occlusion_spheres;


	// Shield impact overrides

	// Regular and 1st person shield impact effect overrides

	c_typed_tag_reference<SHIELD_IMPACT_TAG, INVALID_TAG> shield_impact_parameter_override;
	c_typed_tag_reference<SHIELD_IMPACT_TAG, INVALID_TAG> first_person_shield_impact_parameter_override;
	s_tag_reference overshield_parameter_override;
	s_tag_reference first_person_overshield_parameter_override;
};
COMPILE_ASSERT(sizeof(s_model_definition) == 0x1B4);

struct s_model_variant_region;
struct s_model_variant_object;
struct s_model_variant
{
	c_string_id name;

	// The dialogue tag for this model variant
	s_tag_reference voice;

	// The dialogue effect for this model variant
	c_string_id dialogue_effect;

	c_enum<e_odst_recon_variant, int8, _odst_recon_variant_none, k_odst_recon_variant_count> ai_character;
	byte NASUHJ[0x3];
	int8 runtime_variant_region_indices[16];
	c_typed_tag_block<s_model_variant_region> regions;
	c_typed_tag_block<s_model_variant_object> objects;

	// selects an instance group for this variant
	int32 instance_group;

	byte RHTDQLFD[0x8];
};
COMPILE_ASSERT(sizeof(s_model_variant) == 0x50);

enum e_region_sort
{
	_region_sort_no_sorting = 0,

	// closest
	_region_sort_negative_5,

	_region_sort_negative_4,
	_region_sort_negative_3,
	_region_sort_negative_2,
	_region_sort_negative_1,

	// same as model
	_region_sort_0,

	_region_sort_positive_1,
	_region_sort_positive_2,
	_region_sort_positive_3,
	_region_sort_positive_4,

	// farthest
	_region_sort_positive_5,

	k_region_sort_count
};

struct s_model_variant_permutation;
struct s_model_variant_region
{
	c_string_id region_name; // must match region name in render_model
	int8 runtime_region_index;
	int8 runtime_flags;
	int16 parent_variant;
	c_typed_tag_block<s_model_variant_permutation> permutations;

	// negative values mean closer to the camera
	c_enum<e_region_sort, int16, _region_sort_no_sorting, k_region_sort_count> sort_order;
	byte JO[0x2];
}; COMPILE_ASSERT(sizeof(s_model_variant_region) == 0x18);

enum e_model_variant_permutation_flags
{
	_model_variant_permutation_flag_copy_states_to_all_permutations_bit = 0,
	_model_variant_permutation_flag_bit1,

	k_model_variant_permutation_flags
};

struct s_model_variant_state;
struct s_model_variant_permutation
{
	c_string_id permutation_name;
	int8 runtime_permutation_index;
	c_flags<e_model_variant_permutation_flags, uns8, k_model_variant_permutation_flags> flags;
	byte KSJOSR[0x2];
	real32 probability; // (0,+inf)
	c_typed_tag_block<s_model_variant_state> states;
	int8 runtime_state_permutation_indices[5];
	byte LOOEL[0x7];
};
COMPILE_ASSERT(sizeof(s_model_variant_permutation) == 0x24);

enum e_model_state_property_flags
{
	_model_state_property_flag_blurred_bit = 0,
	_model_state_property_flag_hella_blurred_bit,
	_model_state_property_flag_unshielded_bit, // shielded
	_model_state_property_flag_battery_depleted_bit,

	k_model_state_property_flags
};

struct s_model_variant_state
{
	c_string_id permutation_name;
	int8 runtime_permutation_index;
	c_flags<e_model_state_property_flags, uns8, k_model_state_property_flags> property_flags;
	c_enum<e_model_state, int16, _model_state_standard, k_number_of_model_states> state;

	// played while the model is in this state
	c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> looping_effect;

	c_string_id looping_effect_marker_name;
	real32 initial_probability;
};
COMPILE_ASSERT(sizeof(s_model_variant_state) == 0x20);

struct s_model_variant_object
{
	c_string_id parent_marker;
	c_string_id child_marker;

	// optional
	c_string_id child_variant_name;

	c_typed_tag_reference<OBJECT_TAG, INVALID_TAG> child_object;
};
COMPILE_ASSERT(sizeof(s_model_variant_object) == 0x1C);

class c_model_instance_group_member
{
public:
	// if this member is chosen, this subgroup will be chosen as well
	int32 subgroup;

	// instance name, a partial name will choose all matching instances, leave blank for NONE
	c_string_id instances;

	// higher numbers make it more likely
	real32 probability; // > 0.0

	int32 instance_placement_mask[4];
};
COMPILE_ASSERT(sizeof(c_model_instance_group_member) == 0x1C);

enum e_model_instance_group_choice
{
	_model_instance_group_choice_choose_one_member = 0,
	_model_instance_group_choice_choose_all_members,

	k_model_instance_group_choice_count
};

class c_model_instance_group
{
public:
	// name of this instance group
	c_string_id name;

	// how to choose members
	c_enum<e_model_instance_group_choice, int32, _model_instance_group_choice_choose_one_member, k_model_instance_group_choice_count> choice;

	c_typed_tag_block<c_model_instance_group_member> member_list;
	real32 total_probability;
};
COMPILE_ASSERT(sizeof(c_model_instance_group) == 0x18);

struct s_model_material
{
	c_string_id material_name;
	byte unused_flags[0x2];
	int16 damage_section;
	int16 runtime_collision_material_index;
	int16 runtime_damager_material_index;
	c_string_id global_material_name;
	int16 runtime_global_material_index;
	byte SEWETKHRE[0x2];
};
COMPILE_ASSERT(sizeof(s_model_material) == 0x14);

struct s_model_damage_section
{
	c_string_id name;

	// * absorbs body damage: damage to this section does not count against body vitality
	// * headshottable: takes extra headshot damage when shot
	// * ignores shields: damage to this section bypasses shields
	uns32 flags;

	// percentage of total object vitality
	real32 vitality_percentage; // [0,1]

	s_tag_block instant_responses;
	s_tag_block unused0;
	s_tag_block unused1;
	real32 stun_time; // seconds
	real32 recharge_time; // seconds
	real32 runtime_recharge_velocity;
	string_id resurrection_restored_region_name;
	int16 runtime_resurrection_restored_region_index;

	// pad
	byte AG[0x2];
};
COMPILE_ASSERT(sizeof(s_model_damage_section) == 0x44);

struct s_model_damage_info
{
	uns32 flags;

	// absorbes AOE or child damage
	c_string_id global_indirect_material_name;

	// absorbes AOE or child damage
	int16 indirect_damage_section; // short_block_index_custom_search

	// pad
	byte XN[0x2];

	// pad
	byte LPVYKO[0x4];

	c_enum<e_damage_reporting_type, int8, _damage_reporting_type_unknown, k_damage_reporting_type_count> collision_damage_reporting_type;
	c_enum<e_damage_reporting_type, int8, _damage_reporting_type_unknown, k_damage_reporting_type_count> response_damage_reporting_type;

	// pad
	byte MQ[0x2];

	// pad
	byte MYON[0x14];

	// body
	struct
	{
		real32 maximum_vitality;

		// the minimum damage required to stun this object's health
		real32 minimum_stun_damage;

		// the length of time the health stay stunned (do not recharge) after taking damage
		real32 stun_time; // seconds

		// the length of time it would take for the shields to fully recharge after being completely depleted
		real32 recharge_time; // seconds

		// 0 defaults to 1 - to what maximum level the body health will be allowed to recharge
		real32 recharge_fraction;

	} body;

	// pad
	byte IKEIDYSCX[0x40];

	struct
	{
		// the default initial and maximum shield vitality of this object
		real32 maximum_shield_vitality;

		c_string_id global_shield_material_name;

		// the minimum damage required to stun this object's shields
		real32 minimum_stun_damage;

		// the length of time the shields stay stunned (do not recharge) after taking damage
		real32 stun_time; // seconds
		
		// the length of time it would take for the shields to fully recharge after being completely depleted
		real32 recharge_time; // seconds

		real32 shield_damaged_threshold;

		c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> shield_damaged_effect;
		c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> shield_depleted_effect;
		c_typed_tag_reference<EFFECT_TAG, INVALID_TAG> shield_recharging_effect;

	} shield;

	c_typed_tag_block<s_model_damage_section> damage_sections;
	s_tag_block nodes;

	int16 runtime_shield_material_type;
	int16 runtime_indirect_material_type;

	real32 runtime_shield_recharge_velocity;
	real32 runtime_health_recharge_velocity;

	s_tag_block damage_seats;
	s_tag_block damage_constraints;
};
COMPILE_ASSERT(sizeof(s_model_damage_info) == 0x100);

enum e_model_target_lock_on_flags
{
	_model_target_lock_on_flag_locked_by_human_tracking_bit = 0,
	_model_target_lock_on_flag_locked_by_plasma_tracking_bit,
	_model_target_lock_on_flag_headshot_bit,
	_model_target_lock_on_flag_vulnerable_bit,
	_model_target_lock_on_flag_alwas_locked_by_plasma_tracking_bit,
	_model_target_lock_on_flag_ignored_on_local_physics_bit,
	_model_target_lock_on_flag_use_for_network_lead_vector_only_bit,

	k_model_target_lock_on_flags
};

struct s_model_target_lock_on_data
{
	// lock-on fields

	c_flags<e_model_target_lock_on_flags, uns32, k_model_target_lock_on_flags> flags;
	real32 lock_on_distance;

	// a weapon can track/lock on this target if this string is in the weapon's tracking block
	c_string_id tracking_type;
};
COMPILE_ASSERT(sizeof(s_model_target_lock_on_data) == 0xC);

enum e_model_target_flags
{
	_model_target_flag_aoe_top_level_bit = 0,
	_model_target_flag_aoe_test_obstruction_bit,

	// use this model targets center for displaying the targetting reticle
	_model_target_flag_shows_tracking_reticle_bit,

	k_model_target_flags
};

struct s_model_target
{
	c_flags<e_model_target_flags, uns8, k_model_target_flags> flags;
	byte MTBNP1[0x3];

	// multiple markers become multiple spheres of the same radius
	c_string_id marker_name;

	// sphere radius
	real32 size;

	// the target is only visible when viewed within this angle of the marker's x axis
	real32 cone_angle;

	// the target is associated with this damage section
	int16 damage_section;

	// the target will only appear with this variant
	int16 variant;

	// higher relevances turn into stronger magnetisms
	real32 targeting_relevance;

	// ignored if zero
	real32 aoe_exclusion_radius;

	s_model_target_lock_on_data lock_on_data;
};
COMPILE_ASSERT(sizeof(s_model_target) == 0x28);

