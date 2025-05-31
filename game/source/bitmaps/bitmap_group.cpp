#include "bitmaps/bitmap_group.hpp"

#include "cseries/cseries.hpp"

const real_rectangle2d* __cdecl bitmap_group_get_bounding_rect_from_sequence(int32 bitmap_group_index, int16 sequence_index, int16 frame_index)
{
	return INVOKE(0x00718E60, bitmap_group_get_bounding_rect_from_sequence, bitmap_group_index, sequence_index, frame_index);
}

