#pragma once

#include "cseries/cseries.hpp"

struct c_base_channel_settings
{
	long graph_index;
	long impulse_name;
	c_animation_id primary_animation_id;
	uint16 playback_flags;
	char graph_location;
};
static_assert(sizeof(c_base_channel_settings) == 0x10);

struct c_channel_base
{
	long m_graph_index;
	long m_impulse_name;
	c_animation_id m_animation_id;
	uint16 m_playback_flags;
	char m_graph_location;
	char m_parent_slider_index;
	uint8 m_parent_slider_type;
	char m_initialized;
};
static_assert(sizeof(c_channel_base) == 0x14);

