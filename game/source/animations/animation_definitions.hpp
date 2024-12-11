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
	short next_sibling_node_index;
	short first_child_node_index;
	short parent_node_index;
	c_flags<e_animation_node_model_flags, byte, k_animation_node_model_flags> model_flags;
	c_flags<e_node_joint_flags, byte, k_node_joint_flags> node_joint_flags;
	real_vector3d base_vector;
	real vector_range;
	real z_pos;
};
static_assert(sizeof(s_animation_graph_node) == 0x20);

struct s_animation_inheritence
{
	c_typed_tag_reference<MODEL_ANIMATION_GRAPH_TAG> inherited_tag_reference;
	s_tag_block node_remap;
	s_tag_block node_remap_flags;
	real z_scale;
	long inheritance_flags;
};
static_assert(sizeof(s_animation_inheritence) == 0x30);

struct c_animation_graph_definitions
{
	// GRAPH DATA

	s_tag_reference parent_animation_graph;
	byte_flags inheritance_flags;
	byte_flags private_flags;
	short animation_codec_pack;
	c_typed_tag_block<s_animation_graph_node> skeleton_nodes;
	s_tag_block sound_references;
	s_tag_block effect_references;
	s_tag_block blend_screens;
	s_tag_block foot_markers;
	s_tag_block animations;
};
static_assert(sizeof(c_animation_graph_definitions) == 0x5C);

struct c_model_animation_graph_contents
{
	// MODE-n-STATE GRAPH
	s_tag_block modes;

	// SPECIAL CASE ANIMS
	s_tag_block vehicle_suspension;
	s_tag_block object_overlays;
};
static_assert(sizeof(c_model_animation_graph_contents) == 0x24);

struct c_model_animation_runtime_data
{
	// RUN-TIME DATA

	s_tag_block inheritence_block;
	s_tag_block weapon_block;
	c_static_flags<256> left_arm_bit_vector;
	c_static_flags<256> right_arm_bit_vector;
};
static_assert(sizeof(c_model_animation_runtime_data) == 0x58);

struct c_model_animation_tag_resource
{
	s_tag_block members;
};
static_assert(sizeof(c_model_animation_tag_resource) == sizeof(s_tag_block));

struct c_model_animation_tag_resource_group
{
	long reference_count;
	s_tag_resource tag_resource;
};
static_assert(sizeof(c_model_animation_tag_resource_group) == 0xC);

struct c_model_animation_tag_resource_member
{
	long UNUSED_animation_index;
	dword production_checksum;
	short frame_count;
	byte node_count;
	byte movement_data_type;
	c_animation_data_sizes data_sizes;
	s_tag_data animation_tag_data;
};
static_assert(sizeof(c_model_animation_tag_resource_member) == 0x30);

struct c_model_animation
{
	c_string_id name;
	real weight;
	short private_loop_frame_index;
	word_flags playback_flags;
	char blend_screen_index;
	char desired_compression_setting;
	char current_compression_setting;
	byte node_count;
	short frame_count;
	char animation_type;
	char frame_info_type;
	word_flags production_flags;
	word_flags internal_flags;
	dword node_list_checksum;
	dword production_checksum;
	word importer_version;
	word compressor_version;
	short runtime_parent_animation_index;
	short next_animation_index;
	short resource_group_index;
	short resource_group_member_index;
	s_tag_block frame_events;
	s_tag_block sound_events;
	s_tag_block effect_events;
	s_tag_block dialogue_events;
	s_tag_block object_space_nodes;
	s_tag_block foot_tracking_data;
	real_vector3d movement_heading;
	real average_translation_magnitude;
	real average_pivot_magnitude;
};
static_assert(sizeof(c_model_animation) == 0x88);

struct c_model_animation_graph
{
public:
	static c_model_animation_graph const* __cdecl get(long definition_index);
	static c_model_animation_graph const* __cdecl get_from_object_definition(long object_definition_index);
	s_animation_graph_node* get_node(long node_index) const;

//protected:
	c_animation_graph_definitions definitions;
	c_model_animation_graph_contents contents;
	c_model_animation_runtime_data runtime_data;

	// RESULTS OF THE LAST IMPORT

	s_tag_data error_report;
	s_tag_block additional_node_data;
	s_tag_block resource_groups;
};
static_assert(sizeof(c_model_animation_graph) == 0x104);

