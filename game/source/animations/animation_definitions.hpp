#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"
#include "animations/animation_data.hpp"

enum e_animation_node_model_flags
{
	_animation_node_model_primary_bit = 0,
	_animation_node_model_secondary_bit,
	_animation_node_model_local_root_bit,
	_animation_node_model_left_hand_bit,
	_animation_node_model_right_hand_bit,
	_animation_node_model_left_arm_member_bit,

	k_animation_node_model_flags
};

enum e_node_joint_flags
{
	_node_joint_ball_socket_bit = 0,
	_node_joint_hinge_bit,
	_node_joint_no_movement_bit,

	k_node_joint_flags
};

struct s_animation_graph_node
{
	c_string_id name;
	int16 next_sibling_node_index;
	int16 first_child_node_index;
	int16 parent_node_index;
	c_flags<e_animation_node_model_flags, uns8, k_animation_node_model_flags> model_flags;
	c_flags<e_node_joint_flags, uns8, k_node_joint_flags> node_joint_flags;
	real_vector3d base_vector;
	real32 vector_range;
	real32 z_pos;
};
COMPILE_ASSERT(sizeof(s_animation_graph_node) == 0x20);

struct s_animation_inheritence
{
	c_typed_tag_reference<MODEL_ANIMATION_GRAPH_TAG> inherited_tag_reference;
	s_tag_block node_remap;
	s_tag_block node_remap_flags;
	real32 z_scale;
	int32 inheritance_flags;
};
COMPILE_ASSERT(sizeof(s_animation_inheritence) == 0x30);

class c_animation_graph_definitions
{
public:
	// GRAPH DATA

	s_tag_reference parent_animation_graph;
	uns8 inheritance_flags;
	uns8 private_flags;
	int16 animation_codec_pack;
	c_typed_tag_block<s_animation_graph_node> skeleton_nodes;
	s_tag_block sound_references;
	s_tag_block effect_references;
	s_tag_block blend_screens;
	s_tag_block foot_markers;
	s_tag_block animations;
};
COMPILE_ASSERT(sizeof(c_animation_graph_definitions) == 0x5C);

class c_model_animation_graph_contents
{
public:
	// MODE-n-STATE GRAPH
	s_tag_block modes;

	// SPECIAL CASE ANIMS
	s_tag_block vehicle_suspension;
	s_tag_block object_overlays;
};
COMPILE_ASSERT(sizeof(c_model_animation_graph_contents) == 0x24);

class c_model_animation_runtime_data
{
public:
	// RUN-TIME DATA

	s_tag_block inheritence_block;
	s_tag_block weapon_block;
	c_static_flags<256> left_arm_bit_vector;
	c_static_flags<256> right_arm_bit_vector;
};
COMPILE_ASSERT(sizeof(c_model_animation_runtime_data) == 0x58);

class c_model_animation_tag_resource
{
public:
	s_tag_block members;
};
COMPILE_ASSERT(sizeof(c_model_animation_tag_resource) == sizeof(s_tag_block));

class c_model_animation_tag_resource_group
{
public:
	int32 reference_count;
	s_tag_resource tag_resource;
};
COMPILE_ASSERT(sizeof(c_model_animation_tag_resource_group) == 0xC);

class c_model_animation_tag_resource_member
{
public:
	int32 UNUSED_animation_index;
	uns32 production_checksum;
	int16 frame_count;
	uns8 node_count;
	uns8 movement_data_type;
	c_animation_data_sizes data_sizes;
	s_tag_data animation_tag_data;
};
COMPILE_ASSERT(sizeof(c_model_animation_tag_resource_member) == 0x30);

class c_model_animation
{
public:
	c_string_id name;
	real32 weight;
	int16 private_loop_frame_index;
	uns16 playback_flags;
	int8 blend_screen_index;
	int8 desired_compression_setting;
	int8 current_compression_setting;
	uns8 node_count;
	int16 frame_count;
	int8 animation_type;
	int8 frame_info_type;
	uns16 production_flags;
	uns16 internal_flags;
	uns32 node_list_checksum;
	uns32 production_checksum;
	uns16 importer_version;
	uns16 compressor_version;
	int16 runtime_parent_animation_index;
	int16 next_animation_index;
	int16 resource_group_index;
	int16 resource_group_member_index;
	s_tag_block frame_events;
	s_tag_block sound_events;
	s_tag_block effect_events;
	s_tag_block dialogue_events;
	s_tag_block object_space_nodes;
	s_tag_block foot_tracking_data;
	real_vector3d movement_heading;
	real32 average_translation_magnitude;
	real32 average_pivot_magnitude;
};
COMPILE_ASSERT(sizeof(c_model_animation) == 0x88);

class c_model_animation_graph
{
public:
	static const c_model_animation_graph* __cdecl get(int32 definition_index);
	static const c_model_animation_graph* __cdecl get_from_object_definition(int32 object_definition_index);
	s_animation_graph_node* get_node(int32 node_index) const;

//protected:
	c_animation_graph_definitions definitions;
	c_model_animation_graph_contents contents;
	c_model_animation_runtime_data runtime_data;

	// RESULTS OF THE LAST IMPORT

	s_tag_data error_report;
	s_tag_block additional_node_data;
	s_tag_block resource_groups;
};
COMPILE_ASSERT(sizeof(c_model_animation_graph) == 0x104);

