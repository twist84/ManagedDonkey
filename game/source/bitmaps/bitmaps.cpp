#include "bitmaps/bitmaps.hpp"

void __cdecl bitmap_2d_initialize(bitmap_data_block_def* bitmap, short width, short height, short mipmap_count, short format, word flags, bool a7, bool a8)
{
	INVOKE(0x0086AAE0, bitmap_2d_initialize, bitmap, width, height, mipmap_count, format, flags, a7, a8);
}