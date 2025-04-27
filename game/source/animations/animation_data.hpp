#pragma once

#include "cseries/cseries.hpp"

struct c_animation_data_sizes
{
	int8 m_static_node_flags;
	int8 m_animated_node_flags;
	int16 m_movement_data;
	int16 m_pill_offset_data;
	int16 m_default_data;
	int32 m_uncompressed_data;
	int32 m_compressed_data;
};
static_assert(sizeof(c_animation_data_sizes) == 0x10);

struct c_animation_data
{
	uint8 const* m_data;
	c_animation_data_sizes const* m_data_sizes;
	uint8 m_node_count;
	uint8 m_movement_data_type;
	int16 m_frame_count;
};
static_assert(sizeof(c_animation_data) == 0xC);

