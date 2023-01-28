#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

enum e_bitmap_format
{
	_bitmap_format_a8 = 0,
	_bitmap_format_y8,
	_bitmap_format_ay8,
	_bitmap_format_a8y8,
	_bitmap_format_r8,
	_bitmap_format_unused2,
	_bitmap_format_r5g6b5,
	_bitmap_format_unused3,
	_bitmap_format_a1r5g5b5,
	_bitmap_format_a4r4g4b4,
	_bitmap_format_x8r8g8b8,
	_bitmap_format_a8r8g8b8,
	_bitmap_format_unused4,
	_bitmap_format_unused5,
	_bitmap_format_dxt1,
	_bitmap_format_dxt3,
	_bitmap_format_dxt5,
	_bitmap_format_a4r4g4b4_font,
	_bitmap_format_unused7,
	_bitmap_format_unused8,
	_bitmap_format_software_rgbfp32,
	_bitmap_format_unused9,
	_bitmap_format_v8u8,
	_bitmap_format_g8b8,
	_bitmap_format_abgrfp32,
	_bitmap_format_abgrfp16,
	_bitmap_format_q8w8v8u8,
	_bitmap_format_a2r10g10b10,
	_bitmap_format_a16b16g16r16,
	_bitmap_format_v16u16,
	_bitmap_format_dxt3a,
	_bitmap_format_dxt5a,
	_bitmap_format_dxt3a_1111,
	_bitmap_format_dxn,
	_bitmap_format_ctx1,
	_bitmap_format_dxt3a_alpha,
	_bitmap_format_dxt3a_mono,
	_bitmap_format_dxt5a_alpha,
	_bitmap_format_dxt5a_mono,
	_bitmap_format_dxn_mono_alpha,

	k_bitmap_format_count
};

// constructors in `rasterizer_textures_xenon_header` and `font_cache`
struct bitmap_data_block_def
{
	tag signature;
	short width;
	short height;
	char depth;
	byte_flags more_flags;
	short type;
	short format;
	word_flags flags;
	int16_point2d registration_point;
	char mipmap_count;
	char curve;
	char interleaved_interop;
	char interleaved_texture_index;
	long pixels_offset;
	long pixels_size;
	long high_res_pixels_offset_offset;
	long high_res_pixels_size;
	long hardware_format;
	dword base_address;
};
static_assert(sizeof(bitmap_data_block_def) == 0x30);

struct bitmap_group
{
	// choose how you are using this bitmap
	long usage;

	word_flags flags;

	// number of pixels between adjacent sprites (0 uses default, negative numbers set no spacing)
	short sprite_spacing;

	// the apparent height of the bump map above the triangle it is textured onto, in texture repeats (i.e., 1.0 would be as high as the texture is wide)
	real bump_map_height; // repeats

	// used by detail maps and illum maps.  0 means fade by last mipmap, 1 means fade by first mipmap
	real_fraction fade_factor; // [0,1]

	// automatic chooses FAST if your bitmap is bright, and PRETTY if your bitmap has dark bits
	char curve_mode;

	// 0 = use default defined by usage
	char max_mipmap_level;

	// 0 = do not downsample source image
	short max_resolution;
	byte post_max_resolution[2];

	// overrides the format defined by usage
	short force_bitmap_format;

	tag_block tight_bounds;
	tag_block usage_override;
	tag_block manual_sequences;

	// IMPORT DATA
	//
	// ***************************************************************************************************************************
	//
	// Everything below this line is bitmap data.  It is updated when you reimport the bitmap.
	//
	// Any changes you make below will be lost in the next reimport, and may even cause 'bad things' to happen.
	//
	// ***************************************************************************************************************************
	//
	tag_data source_data;

	tag_data processed_pixel_data;
	tag_block sequences;
	c_typed_tag_block<bitmap_data_block_def> bitmaps;
	tag_data xenon_processed_pixel_data;
	c_typed_tag_block<bitmap_data_block_def> xenon_bitmaps;
	tag_block hardware_textures;
	tag_block interleaved_hardware_textures;
	long __unknownB4;
};
static_assert(sizeof(bitmap_group) == 0xB8);

