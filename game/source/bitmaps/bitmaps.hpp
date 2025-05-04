#pragma once

#include "cseries/cseries.hpp"

struct bitmap_data;

extern void* __cdecl bitmap_2d_address(bitmap_data const* bitmap, int16 x, int16 y, int16 mipmap_index);
extern void __cdecl bitmap_2d_initialize(bitmap_data* bitmap, int16 width, int16 height, int16 mipmap_count, int16 format, uns16 flags, bool a7, bool a8);
extern bitmap_data* __cdecl bitmap_2d_new(int16 width, int16 height, int16 mipmap_count, int16 format, uns16 flags);
extern void __cdecl bitmap_delete(bitmap_data* bitmap);

