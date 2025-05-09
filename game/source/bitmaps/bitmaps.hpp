#pragma once

#include "cseries/cseries.hpp"

struct bitmap_data;

extern void* __cdecl bitmap_2d_address(bitmap_data const* bitmap, int16 x, int16 y, int16 mipmap_index);
extern void __cdecl bitmap_2d_initialize(bitmap_data* bitmap, int16 width, int16 height, int16 mipmap_count_excluding_highest, int16 format, uns16 flags, bool preallocated, bool pc_bitmap);
extern bitmap_data* __cdecl bitmap_2d_new(int16 width, int16 height, int16 mipmap_count_excluding_highest, int16 format, uns16 flags);
extern void __cdecl bitmap_delete(bitmap_data* bitmap);

