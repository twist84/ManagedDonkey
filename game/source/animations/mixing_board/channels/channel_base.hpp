#pragma once

#include "cseries/cseries.hpp"

class c_base_channel_settings
{
public:
	int32 graph_index;
	int32 impulse_name;
	c_animation_id primary_animation_id;
	uns16 playback_flags;
	int8 graph_location;
};
static_assert(sizeof(c_base_channel_settings) == 0x10);

class c_channel_base
{
public:
	int32 m_graph_index;
	int32 m_impulse_name;
	c_animation_id m_animation_id;
	uns16 m_playback_flags;
	int8 m_graph_location;
	int8 m_parent_slider_index;
	uns8 m_parent_slider_type;
	int8 m_initialized;
};
static_assert(sizeof(c_channel_base) == 0x14);

