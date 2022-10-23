#pragma once

#include "cseries/cseries.hpp"

// constructors in and `rasterizer_textures_xenon_header` `font_cache`
struct bitmap_data
{
	tag signature;
	short width;
	short height;
	char depth;
	byte_flags more_flags;
	short type;
	short format;
	word_flags flags;
	point2d registration_point;
	char mipmap_count;
	char curve;
	char interleaved_interop;
	char interleaved_texture_index;
	long pixels_offset;
	long pixels_size;
	long high_res_pixels_offset_offset;
	long high_res_pixels_size;
	long hardware_format;
	dword runtime_tag_base_address;
};
static_assert(sizeof(bitmap_data) == 0x30);