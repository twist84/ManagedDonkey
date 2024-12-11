#pragma once

#include "cseries/cseries.hpp"

struct c_animation_data_sizes
{
	char m_static_node_flags;
	char m_animated_node_flags;
	short m_movement_data;
	short m_pill_offset_data;
	short m_default_data;
	long m_uncompressed_data;
	long m_compressed_data;
};
static_assert(sizeof(c_animation_data_sizes) == 0x10);

struct c_animation_data
{
	byte const* m_data;
	c_animation_data_sizes const* m_data_sizes;
	byte m_node_count;
	byte m_movement_data_type;
	short m_frame_count;
};
static_assert(sizeof(c_animation_data) == 0xC);

