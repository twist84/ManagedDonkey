#pragma once

#include "cseries/cseries.hpp"

struct bitmap_data;

extern void* __cdecl bitmap_2d_address(bitmap_data const* bitmap, short x, short y, short mipmap_index);
extern void __cdecl bitmap_2d_initialize(bitmap_data* bitmap, short width, short height, short mipmap_count, short format, word flags, bool a7, bool a8);
extern bitmap_data* __cdecl bitmap_2d_new(short width, short height, short mipmap_count, short format, word flags);
extern void __cdecl bitmap_delete(bitmap_data* bitmap);

