#pragma once

#include "animations/animation_interpolation.hpp"
#include "animations/mixing_board/channels/animation_channel.hpp"
#include "cseries/cseries.hpp"

struct c_model_animation_graph;
struct c_animation_manager
{
	c_model_animation_graph const* get_graph() const;
	bool valid_graph() const;
	long get_state_name() const;

	c_animation_channel __unknown0;
	c_animation_channel __unknown3C;
	c_interpolator_control __unknown78;

	long m_mixing_board_definition_index;

	byte __data88[0x4];

	long m_model_animation_graph_definition_index;

	byte __data90[0x4];

	c_string_id __unknown94;
	c_string_id __unknown98;
	c_string_id __unknown9C;
	c_string_id m_state_name;

	byte __data[0x1C];
};
static_assert(sizeof(c_animation_manager) == 0xC0);

