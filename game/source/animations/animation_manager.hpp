#pragma once

#include "animations/animation_interpolation.hpp"
#include "animations/mixing_board/channels/animation_channel.hpp"
#include "cseries/cseries.hpp"

struct s_animation_goal_settings
{
	string_id mode;
	string_id weapon_class;
	string_id weapon_type;
	string_id state;
};
static_assert(sizeof(s_animation_goal_settings) == 0x10);

struct c_model_animation_graph;
struct c_animation_manager
{
	c_model_animation_graph const* get_graph() const;
	bool valid_graph() const;
	long get_state_name() const;

	c_animation_channel m_state_channel;
	c_animation_channel m_transition_channel;
	c_interpolator_control m_animation_node_interpolator;
	long m_mixing_board_definition_index;
	long m_object_index;
	long m_graph_tag_index;
	uint16 m_flags;
	uint16 m_internal_flags;
	s_animation_goal_settings m_goal_settings;
	s_animation_goal_settings m_previous_goal;
	real_vector3d m_transition_delta;
};
static_assert(sizeof(c_animation_manager) == 0xC0);

