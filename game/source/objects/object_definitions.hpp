#pragma once

#include "cseries/cseries.hpp"
#include "math/function_definitions.hpp"
#include "tag_files/tag_groups.hpp"
#include "physics/collision_model_definitions.hpp"
#include "objects/multiplayer_game_objects.hpp"

enum e_object_type
{
	_object_type_biped = 0,
	_object_type_vehicle,
	_object_type_weapon,
	_object_type_equipment,
	_object_type_arg_device,
	_object_type_terminal,
	_object_type_projectile,
	_object_type_scenery,
	_object_type_machine,
	_object_type_control,
	_object_type_sound_scenery,
	_object_type_crate,
	_object_type_creature,
	_object_type_giant,
	_object_type_effect_scenery,

	k_object_type_count
};

enum e_object_source
{
	_object_source_structure = 0,
	_object_source_editor,
	_object_source_dynamic,
	_object_source_legacy,
	_object_source_sky,
	_object_source_parent,

	k_object_source_count
};

enum e_object_definition_flags
{
	_object_definition_flag_does_not_cast_shadow_bit = 0,
	_object_definition_flag_search_cardinal_direction_lightmaps_on_failure_bit,
	_object_definition_flag_preserves_initial_damage_owner_bit,
	_object_definition_flag_not_a_pathfinding_obstacle_bit,

	// object passes all function values to parent and uses parent's markers
	_object_definition_flag_extension_of_parent_bit,

	_object_definition_flag_does_not_cause_collision_damage_bit,
	_object_definition_flag_early_mover_bit,
	_object_definition_flag_early_mover_localized_physics_bit,
	_object_definition_flag_use_static_massive_lightmap_sample_bit,
	_object_definition_flag_object_scales_attachments_bit,
	_object_definition_flag_inherits_players_appearance_bit,
	_object_definition_flag_non_physical_in_map_editor_bit,

	// use this for the mac gun on spacestation
	_object_definition_flag_attach_to_clusters_by_dynamic_sphere_bit,

	_object_definition_flag_effects_created_by_this_object_do_not_spawn_objects_in_multiplayer_bit,

	// specificly the flying observer camera
	_object_definition_flag_does_not_collide_with_camera_bit,

	// AOE damage being applied to this object does not test for obstrutions.
	_object_definition_flag_damage_not_blocked_by_obstructions_bit,

	k_object_definition_flags
};

enum e_lightmap_shadow_mode
{
	_lightmap_shadow_mode_default = 0,
	_lightmap_shadow_mode_never,
	_lightmap_shadow_mode_always,
	_lightmap_shadow_mode_blur,

	k_lightmap_shadow_mode_count
};

enum e_sweetener_size
{
	_sweetener_size_default = 0,
	_sweetener_size_small,
	_sweetener_size_medium,
	_sweetener_size_large,

	k_sweetener_size_count
};

enum e_water_density_type
{
	_water_density_type_default = 0,
	_water_density_type_super_floater,
	_water_density_type_floater,
	_water_density_type_neutral,
	_water_density_type_sinker,
	_water_density_type_super_sinker,
	_water_density_type_none,

	k_water_density_type_count
};

enum e_object_definition_secondary_flags
{
	_object_definition_secondary_flag_does_not_affect_projectile_aiming_bit = 0,

	k_object_definition_secondary_flags
};

struct s_object_early_mover_obb_definition;
struct object_ai_properties;
struct s_object_function_definition;
struct object_attachment_definition;
struct object_definition_widget;
struct object_change_color_definition;
struct object_node_map_defintion;
struct s_object_health_pack_definition;
struct _object_definition
{
	short type;
	c_flags<e_object_definition_flags, word_flags, k_object_definition_flags> flags;
	real bounding_radius; // world units
	real_point3d bounding_offset;

	// marine 1.0, grunt 1.4, elite 0.9, hunter 0.5, etc.
	real acceleration_scale; // [0,+inf]

	c_enum<e_lightmap_shadow_mode, short, _lightmap_shadow_mode_default, k_lightmap_shadow_mode_count> lightmap_shadow_mode;
	c_enum<e_sweetener_size, char, _sweetener_size_default, k_sweetener_size_count> sweetener_size;
	c_enum<e_water_density_type, char, _water_density_type_default, k_water_density_type_count> water_density;
	dword_flags runtime_flags;

	// sphere to use for dynamic lights and shadows. only used if not 0
	real dynamic_light_sphere_radius;

	// only used if radius not 0
	real_point3d dynamic_light_sphere_offset;

	c_string_id default_model_variant;
	c_typed_tag_reference<MODEL_TAG> model;
	c_typed_tag_reference<CRATE_TAG> crate_object;

	// only set this tag if you want to override the default collision damage values in globals.globals
	c_typed_tag_reference<COLLISION_DAMAGE_TAG> collision_damage;

	c_typed_tag_block<s_object_early_mover_obb_definition> early_mover_obb;
	c_typed_tag_reference<EFFECT_TAG> creation_effect;
	c_typed_tag_reference<MATERIAL_EFFECTS_TAG> material_effects;
	s_tag_reference armor_sounds;

	// this is the sound that is made when I am meleed.
	// This overrides the sweetener sound of my material.
	c_typed_tag_reference<SOUND_TAG> melee_sound;

	c_typed_tag_block<object_ai_properties> ai_properties;
	c_typed_tag_block<s_object_function_definition> functions;
	short hud_text_message_index;
	c_flags<e_object_definition_secondary_flags, word_flags, k_object_definition_secondary_flags> secondary_flags;
	c_typed_tag_block<object_attachment_definition> attachments;
	c_typed_tag_block<object_definition_widget> widgets;
	c_typed_tag_block<object_change_color_definition> change_colors;
	c_typed_tag_block<object_node_map_defintion> node_maps;
	c_typed_tag_block<s_multiplayer_object_properties_definition> multiplayer_object;
	c_typed_tag_block<s_object_health_pack_definition> health_packs;
	c_typed_tag_block<collision_model_pathfinding_sphere> pathfinding_spheres;

	void update_reference_names();
};
static_assert(sizeof(_object_definition) == 0x120);

struct object_definition
{
	static tag const k_group_tag = OBJECT_TAG;

	_object_definition object;

	void update_reference_names();
};
static_assert(sizeof(object_definition) == sizeof(_object_definition));

struct s_object_early_mover_obb_definition
{
	c_string_id node_name;
	real_rectangle3d bounds;
	euler_angles3d angles;
};
static_assert(sizeof(s_object_early_mover_obb_definition) == 0x28);

enum e_ai_properties_flags
{
	_ai_properties_flag_detroyable_cover_bit = 0,
	_ai_properties_flag_pathfinding_ignore_when_dead_bit,
	_ai_properties_flag_dynamic_cover_bit,
	_ai_properties_flag_non_flight_blocking_bit,
	_ai_properties_flag_dynamic_cover_from_centre_bit,
	_ai_properties_flag_has_corner_markers_bit,
	_ai_properties_flag_inspectable_bit,
	_ai_properties_flag_idle_when_flying_bit,

	k_ai_properties_flags
};

enum e_ai_size
{
	_ai_size_default = 0,
	_ai_size_tiny,
	_ai_size_small,
	_ai_size_medium,
	_ai_size_large,
	_ai_size_huge,
	_ai_size_immobile,

	k_ai_size_count
};

enum e_global_ai_jump_height
{
	_global_ai_jump_height_none = 0,
	_global_ai_jump_height_down,
	_global_ai_jump_height_step,
	_global_ai_jump_height_crouch,
	_global_ai_jump_height_stand,
	_global_ai_jump_height_storey,
	_global_ai_jump_height_tower,
	_global_ai_jump_height_infinite,

	k_global_ai_jump_height_count
};

struct object_ai_properties
{
	c_flags<e_ai_properties_flags, dword_flags, k_ai_properties_flags> ai_flags;
	c_string_id ai_type_name;
	c_enum<e_ai_size, short, _ai_size_default, k_ai_size_count> ai_size;
	c_enum<e_global_ai_jump_height, short, _global_ai_jump_height_none, k_global_ai_jump_height_count> leap_jump_speed;
};
static_assert(sizeof(object_ai_properties) == 0xC);

enum e_object_function_flags
{
	// result of function is one minus actual result
	_object_function_flag_invert_bit = 0,

	// the curve mapping can make the function active/inactive
	_object_function_flag_mapping_does_not_controls_active_bit,

	// function does not deactivate when at or below lower bound
	_object_function_flag_always_active_bit,

	// function offsets periodic function input by random value between 0 and 1
	_object_function_flag_random_time_offset_bit,

	// when this function is deactivated, it still exports its value
	_object_function_flag_always_exports_value_bit,

	// the function will be turned off if the value of the turns_off_with function is 0
	_object_function_flag_turn_off_with_uses_magnitude_bit,

	k_object_function_flags
};

struct s_object_function_definition
{
	c_flags<e_object_function_flags, dword_flags, k_object_function_flags> flags;
	c_string_id import_name;
	c_string_id export_name;
	c_string_id turn_off_with;
	real min_value;

	// default function		
	mapping_function default_function;
	c_string_id scale_by;
};
static_assert(sizeof(s_object_function_definition) == 0x2C);

enum e_object_attachment_flags
{
	// Tchou shader per scenario HACK
	_object_attachment_flag_tchou_shader_per_scenario_hack_bit = 0,
	
	// saved film Tchou shader per scenario HACK
	_object_attachment_flag_saved_film_tchou_shader_per_scenario_hack_bit,

	k_object_attachment_flags
};

enum e_global_object_change_color
{
	_global_object_change_color_none = 0,
	_global_object_change_color_primary,
	_global_object_change_color_secondary,
	_global_object_change_color_tertiary,
	_global_object_change_color_quaternary,

	k_global_object_change_color_count
};

struct object_attachment_definition
{
	c_flags<e_object_attachment_flags, int32_t, k_object_attachment_flags> flags;
	c_typed_tag_reference<LIGHT_TAG, EFFECT_TAG, SOUND_LOOPING_TAG, LENS_FLARE_TAG> type;
	c_string_id marker; // old string id

	c_enum<e_global_object_change_color, short, _global_object_change_color_none, k_global_object_change_color_count> change_color;
	byte DPKP[0x2]; // pad
	c_string_id primary_scale;
	c_string_id secondary_scale;

	void update_reference_names();
};
static_assert(sizeof(object_attachment_definition) == 0x24);

struct object_definition_widget
{
	c_typed_tag_reference<ANTENNA_TAG, CELLULAR_AUTOMATA_TAG, CELLULAR_AUTOMATA2D_TAG, STEREO_SYSTEM_TAG> type;

	void update_reference_names();
};
static_assert(sizeof(object_definition_widget) == sizeof(s_tag_reference));

struct object_change_color_initial_permutation;
struct object_change_color_function;
struct object_change_color_definition
{
	c_typed_tag_block<object_change_color_initial_permutation> initial_permutations_block;
	c_typed_tag_block<object_change_color_function> functions_block;
};
static_assert(sizeof(object_change_color_definition) == 0x18);

struct object_change_color_initial_permutation
{
	real weight;
	real_rgb_color color_lower_bound;
	real_rgb_color color_upper_bound;
	c_string_id variant_name;
};
static_assert(sizeof(object_change_color_initial_permutation) == 0x20);

enum e_global_rgb_interpolation_flags
{
	// blends colors in hsv rather than rgb space
	_global_rgb_interpolation_flag_blend_in_hsv_bit = 0,

	// blends colors through more hues (goes the long way around the color wheel)
	_global_rgb_interpolation_flag_more_colors_bit,

	k_global_rgb_interpolation_flags
};

struct object_change_color_function
{
	byte TJJWBYNU[0x4]; // pad
	c_flags<e_global_rgb_interpolation_flags, int32_t, k_global_rgb_interpolation_flags> scale_flags;
	real_rgb_color color_lower_bound;
	real_rgb_color color_upper_bound;
	c_string_id darken_by;
	c_string_id scale_by;
};
static_assert(sizeof(object_change_color_function) == 0x28);

struct object_node_map_defintion
{
	char target_node;
};
static_assert(sizeof(object_node_map_defintion) == 0x1);

struct s_object_health_pack_definition
{
	c_typed_tag_reference<EQUIPMENT_TAG> health_pack_equipment;

	void update_reference_names();
};
static_assert(sizeof(s_object_health_pack_definition) == sizeof(s_tag_reference));

struct s_scenario_multiplayer_scenario_object_parent
{
	byte der[2];

	// if an object with this name exists, we attach to it as a child
	short parent_object; // short_block_index

	c_string_id parent_marker;
	c_string_id connection_marker;
};
static_assert(sizeof(s_scenario_multiplayer_scenario_object_parent) == 0xC);

enum e_multiplayer_object_placement_spawn_flags
{
	_multiplayer_object_placement_spawn_flag_unique_spawn_bit = 0,
	_multiplayer_object_placement_spawn_flag_not_initially_placed_bit,

	k_multiplayer_object_placement_spawn_flags
};

struct s_scenario_multiplayer_object_properties
{
	// Multiplayer Data
	// object data for multiplayer game use

	long_enum game_engine_symmetric_placement;
	c_flags<e_global_game_engine_type_flags, word_flags, k_global_game_engine_type_flags> game_engine_flags;
	short_enum owner_team;
	char spawn_order; // -1 for random
	char quota_minimum;
	char quota_maximum; // <=0 for unlimited

	c_flags<e_multiplayer_object_placement_spawn_flags, byte_flags, k_multiplayer_object_placement_spawn_flags> spawn_flags;
	short spawn_time; // seconds
	short abandonment_time; // seconds

	char_enum remapping_policy;
	char_enum boundary_shape;
	char_enum teleporter_channel;
	byte blah[1];

	s_scenario_multiplayer_scenario_object_parent map_variant_parent;

	union { real boundary_width; real boundary_radius; };
	real boundary_box_length;
	real boundary_positive_height;
	real boundary_negative_height;

	// Player Respawn Weight
	// This is valid only for objects which are used as player respawn locations
	real natural_respawn_weight;
};
static_assert(sizeof(s_scenario_multiplayer_object_properties) == 0x34);

struct c_object_identifier
{
	e_object_type get_type() const;

	tag m_unique_id; // 'obj#'

	// scenario_structure_bsp_reference
	short m_origin_bsp_index;

	c_enum<e_object_type, char, _object_type_biped, k_object_type_count> m_type;
	c_enum<e_object_source, char, _object_source_structure, k_object_source_count> m_source;
};
static_assert(sizeof(c_object_identifier) == 0x8);

