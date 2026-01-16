#pragma once

#include "cseries/cseries.hpp"

class c_animation_data_sizes
{
public:
	int8 m_static_node_flags;
	int8 m_animated_node_flags;
	int16 m_movement_data;
	int16 m_pill_offset_data;
	int16 m_default_data;
	int32 m_uncompressed_data;
	int32 m_compressed_data;
};
COMPILE_ASSERT(sizeof(c_animation_data_sizes) == 0x10);

class c_animation_data
{
public:
	const byte* m_data;
	const c_animation_data_sizes* m_data_sizes;
	uns8 m_node_count;
	uns8 m_movement_data_type;
	int16 m_frame_count;
};
COMPILE_ASSERT(sizeof(c_animation_data) == 0xC);

