#include "bitmaps/bitmaps.hpp"

#include "bitmaps/bitmap_group_tag_definition.hpp"

void* __cdecl bitmap_2d_address(bitmap_data const* bitmap, short x, short y, short mipmap_index)
{
	return INVOKE(0x00869DC0, bitmap_2d_address, bitmap, x, y, mipmap_index);
}

void __cdecl bitmap_2d_initialize(bitmap_data* bitmap, short width, short height, short mipmap_count, short format, word flags, bool a7, bool a8)
{
	INVOKE(0x0086AAE0, bitmap_2d_initialize, bitmap, width, height, mipmap_count, format, flags, a7, a8);
}

bitmap_data* __cdecl bitmap_2d_new(short width, short height, short mipmap_count, short format, word flags)
{
	return INVOKE(0x0086AC20, bitmap_2d_new, width, height, mipmap_count, format, flags);
}

