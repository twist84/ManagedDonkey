#include "bitmaps/bitmaps.hpp"

#include "bitmaps/bitmap_group_tag_definition.hpp"

void* __cdecl bitmap_2d_address(bitmap_data const* bitmap, int16 x, int16 y, int16 mipmap_index)
{
	return INVOKE(0x00869DC0, bitmap_2d_address, bitmap, x, y, mipmap_index);
}

void __cdecl bitmap_2d_initialize(bitmap_data* bitmap, int16 width, int16 height, int16 mipmap_count, int16 format, uint16 flags, bool a7, bool a8)
{
	INVOKE(0x0086AAE0, bitmap_2d_initialize, bitmap, width, height, mipmap_count, format, flags, a7, a8);
}

bitmap_data* __cdecl bitmap_2d_new(int16 width, int16 height, int16 mipmap_count, int16 format, uint16 flags)
{
	return INVOKE(0x0086AC20, bitmap_2d_new, width, height, mipmap_count, format, flags);
}

void __cdecl bitmap_delete(bitmap_data* bitmap)
{
	INVOKE(0x0086B8C0, bitmap_delete, bitmap);
}

