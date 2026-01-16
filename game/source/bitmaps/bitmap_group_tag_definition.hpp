#pragma once

#include "cache/cache_file_tag_resource_runtime.hpp"
#include "cseries/cseries.hpp"
#include "rasterizer/rasterizer_text.hpp"
#include "tag_files/tag_groups.hpp"

struct bitmap_group_sprite
{
	int16 bitmap_index;
	int16 bitmap_pad;
	int32 unused;
	real_rectangle2d bounds;
	real_point2d registration_point;
};
COMPILE_ASSERT(sizeof(bitmap_group_sprite) == 0x20);

struct bitmap_group_sequence
{
	char name[32];
	int16 first_bitmap_index;
	int16 bitmap_count;
	int32 unused[4];
	c_typed_tag_block<bitmap_group_sprite> sprites;
};
COMPILE_ASSERT(sizeof(bitmap_group_sequence) == 0x40);

enum e_bitmap_more_flags
{
	// is this even used anymore?
	_bitmap_more_flag_delete_from_cache_file_bit = 0,
	
	// (memory spacing) DO NOT CHANGE
	_bitmap_more_flag_xbox360_pitch_bit,
	
	// DO NOT CHANGE
	_bitmap_more_flag_xbox360_byte_order_bit,
	
	// DO NOT CHANGE
	_bitmap_more_flag_xbox360_tiled_texture_bit,
	
	// (hack for bumpmaps) DO NOT CHANGE
	_bitmap_more_flag_xbox360_created_correctly_bit,
	
	// DO NOT CHANGE
	_bitmap_more_flag_xbox360_high_resolution_offset_is_valid_bit,
	
	// DO NOT CHANGE
	_bitmap_more_flag_xbox360_use_interleaved_textures_bit,
	
	// DO NOT CHANGE
	_bitmap_more_flag_xbox360_use_on_demand_only_bit,

	k_bitmap_more_flags
};

enum e_bitmap_type
{
	// 2D
	_bitmap_type_2d = 0,

	// 3D
	_bitmap_type_3d,

	// CUBE MAP
	_bitmap_type_cube_map,

	// No string found in ms23. MCC specific?
	_bitmap_type_array,

	k_bitmap_type_count
};

enum e_bitmap_format
{
	// alpha
	_bitmap_format_a8 = 0,
	
	// intensity
	_bitmap_format_y8,
	
	// combined alpha-intensity
	_bitmap_format_ay8,
	
	// separate alpha-intensity
	_bitmap_format_a8y8,
	
	_bitmap_format_unused1,
	
	_bitmap_format_unused2,
	
	// high-color
	_bitmap_format_r5g6b5,
	
	// r6g5b5
	_bitmap_format_unused3,
	
	// high-color with 1-bit alpha
	_bitmap_format_a1r5g5b5,
	
	// high-color with alpha
	_bitmap_format_a4r4g4b4,
	
	// true-color
	_bitmap_format_x8r8g8b8,
	
	// true-color with alpha
	_bitmap_format_a8r8g8b8,
	
	_bitmap_format_unused4,
	
	_bitmap_format_unused5,
	
	// compressed with color-key transparency
	_bitmap_format_dxt1,
	
	// compressed with explicit alpha
	_bitmap_format_dxt3,
	
	// compressed with interpolated alpha
	_bitmap_format_dxt5,
	
	// font format
	_bitmap_format_a4r4g4b4_font,
	
	// palettized
	_bitmap_format_unused7,
	
	_bitmap_format_unused8,
	
	// SOFTWARE 32 bit float RGB
	_bitmap_format_SOFTWARE_rgbfp32,
	
	_bitmap_format_unused9,
	
	// v8u8 signed 8-bit
	_bitmap_format_v8u8,
	
	// g8b8 unsigned 8-bit
	_bitmap_format_g8b8,
	
	// 32 bit float ABGR
	_bitmap_format_abgrfp32,
	
	// 16 bit float ABGR
	_bitmap_format_abgrfp16,
	
	// 8 bit signed 4 channel
	_bitmap_format_q8w8v8u8,
	
	// 30-bit color 2-bit alpha
	_bitmap_format_a2r10g10b10,
	
	// 48-bit color 16-bit alpha
	_bitmap_format_a16b16g16r16,
	
	// v16u16 signed 16-bit
	_bitmap_format_v16u16,
	
	// compressed 4-bit single channel
	_bitmap_format_dxt3a,
	
	// compressed interpolated single channel
	_bitmap_format_dxt5a,
	
	// compressed channel mask
	_bitmap_format_dxt3a_1111,
	
	// compressed normals: high quality
	_bitmap_format_dxn,
	
	// compressed normals: high compression
	_bitmap_format_ctx1,
	
	// compressed 4-bit alpha channel
	_bitmap_format_dxt3a_alpha,
	
	// compressed interpolated alpha channel
	_bitmap_format_dxt3a_mono,
	
	// compressed 4-bit monochrome
	_bitmap_format_dxt5a_alpha,
	
	// compressed interpolated monochrome
	_bitmap_format_dxt5a_mono,

	// compressed interpolated monochrome + alpha
	_bitmap_format_dxn_mono_alpha,

	k_bitmap_format_count
};

enum e_bitmap_flags
{
	// DO NOT CHANGE
	_bitmap_is_power_of_two_dimensions_bit = 0,
	_bitmap_is_compressed_bit,
	_bitmap_flipped_axes_bit,

	k_tag_bitmap_flags_count,

	_bitmap_free_on_delete_bit = k_tag_bitmap_flags_count,
	_bitmap_not_tiled_bit,
	_bitmap_hardware_only_bit,
	_bitmap_use_base_address_for_hardware_format_bit,
	_bitmap_unknown_bit7,
	_bitmap_hardware_format_is_tracked_externally_bit,

	k_bitmap_flags_count
};

enum e_bitmap_curve
{
	_bitmap_curve_unknown = 0,
	_bitmap_curve_xrgb_gamma2,
	_bitmap_curve_gamma2,
	_bitmap_curve_linear,
	_bitmap_curve_offset_log,
	_bitmap_curve_srgb_gamma22,

	k_bitmap_curve_count,
};

// constructors in `rasterizer_textures_xenon_header` and `font_cache`
struct bitmap_data
{
	tag signature; // should be 'bitm'

	// DO NOT CHANGE
	int16 width;

	// DO NOT CHANGE
	int16 height;

	// DO NOT CHANGE
	int8 depth;

	c_flags<e_bitmap_more_flags, uns8, k_bitmap_more_flags> more_flags;

	// DO NOT CHANGE
	c_enum<e_bitmap_type, int16, _bitmap_type_2d, k_bitmap_type_count> type;

	// DO NOT CHANGE
	c_enum<e_bitmap_format, int16, _bitmap_format_a8, k_bitmap_format_count> format;

	c_flags<e_bitmap_flags, uns16, k_bitmap_flags_count> flags;

	// the 'center' of the bitmap - i.e. for particles
	point2d registration_point;

	// DO NOT CHANGE
	int8 mipmap_count_excluding_highest;

	// how to convert from pixel value to linear
	int8 curve;

	int8 interleaved_interop;
	int8 interleaved_texture_index;

	// DO NOT CHANGE (offset of the beginning of this bitmap, into pixel data)
	int32 pixels_offset; // bytes

	// DO NOT CHANGE (total bytes used by this bitmap)
	int32 pixels_size; // bytes

	// DO NOT CHANGE
	int32 high_res_pixels_offset_offset;

	// DO NOT CHANGE
	int32 high_res_pixels_size;

	c_rasterizer_texture_ref internal_hardware_format;

	void* base_address;
};
COMPILE_ASSERT(sizeof(bitmap_data) == 0x30);

enum e_bitmap_usage_global
{
	// Diffuse Map
	_bitmap_usage_global_diffuse_map = 0,
	
	// Specular Map
	_bitmap_usage_global_specular_map,
	
	// Bump Map (from Height Map)
	_bitmap_usage_global_bump_map_from_height_map,
	
	// Detail Bump Map (from Height Map - fades out)
	_bitmap_usage_global_detail_bump_map_from_height_map_fades_out,
	
	// Detail Map
	_bitmap_usage_global_detail_map,
	
	// Self-Illum Map
	_bitmap_usage_global_self_illum_map,
	
	// Change Color Map
	_bitmap_usage_global_change_color_map,
	
	// Cube Map (Reflection Map)
	_bitmap_usage_global_cube_map_reflection_map,
	
	// Sprite (Additive, Black Background)
	_bitmap_usage_global_sprite_additive_black_background,
	
	// Sprite (Blend, White Background)
	_bitmap_usage_global_sprite_blend_white_background,
	
	// Sprite (Double Multiply, Gray Background)
	_bitmap_usage_global_sprite_double_multiply_gray_background,
	
	// Interface Bitmap
	_bitmap_usage_global_interface_bitmap,
	
	// Warp Map (EMBM)
	_bitmap_usage_global_warp_map_embm,
	
	// Vector Map
	_bitmap_usage_global_vector_map,
	
	// 3D Texture
	_bitmap_usage_global_3_d_texture,
	
	// Float Map (WARNING: HUGE)
	_bitmap_usage_global_float_map_warning_huge,
	
	// Half float Map (HALF HUGE)
	_bitmap_usage_global_half_float_map_half_huge,
	
	// Height Map (for Parallax)
	_bitmap_usage_global_height_map_for_parallax,
	
	// ZBrush Bump Map (from Bump Map)
	_bitmap_usage_global_z_brush_bump_map_from_bump_map,
	
	// Normal Map (aka zbump)
	_bitmap_usage_global_normal_map_aka_zbump,
	
	// Detail ZBrush Bump Map
	_bitmap_usage_global_detail_z_brush_bump_map,
	
	// Detail Normal Map
	_bitmap_usage_global_detail_normal_map,
	
	// Blend Map (linear for terrains)
	_bitmap_usage_global_blend_map_linear_for_terrains,
	
	// Palettized --- effects only
	_bitmap_usage_global_palettized_effects_only,
	
	// CHUD related bitmap
	_bitmap_usage_global_chud_related_bitmap,
	
	// Lightmap Array
	_bitmap_usage_global_lightmap_array,
	
	// Water Array
	_bitmap_usage_global_water_array,
	
	// Interface Sprite
	_bitmap_usage_global_interface_sprite,
	
	// Interface Gradient
	_bitmap_usage_global_interface_gradient,
	
	// Material Map
	_bitmap_usage_global_material_map,
	
	// Smoke Warp
	_bitmap_usage_global_smoke_warp,
	
	// Mux Material Blend Map
	_bitmap_usage_global_mux_material_blend_map,
	
	// Cubemap Gel
	_bitmap_usage_global_cubemap_gel,
	
	// Lens Flare gamma 2.2 -- effects only
	_bitmap_usage_global_lens_flare_gamma_2_2_effects_only,
	
	// Signed Noise
	_bitmap_usage_global_signed_noise,
	
	// Roughness Map (auto
	_bitmap_usage_global_roughness_map_auto,

	k_bitmap_usage_global_count
};

enum e_bitmap_group_flags
{
	// affects how height maps are converted to bump maps
	_bitmap_group_flag_bitmap_is_tiled_bit = 0,

	// uses a sharper(and noisier) method of calculating bump maps from height maps
	_bitmap_group_flag_use_less_blurry_bump_map_bit,

	// lets the compressor use dithering
	_bitmap_group_flag_dither_when_compressing_bit,

	// repopulates the manual sequences with random sprites upon reimport
	_bitmap_group_flag_generate_random_sprites_bit,

	// FOR INTERNAL USE ONLY - DO NOT MODIFY
	_bitmap_group_flag_using_tag_interopand_tag_resource_bit,

	// if your alpha channel represents transparency(alpha blend or alpha - test only), set this bit to stop color bleeding on edges
	_bitmap_group_flag_alpha_channel_stores_transparency_bit,

	// prefer to use alpha value as a weight to build mipmap
	// this will artificially thicken the alpha channel in mip maps, which can keep your bitmap from disappearing in the distance when you are using alpha test
	_bitmap_group_flag_preserve_alpha_channel_in_mipmaps_for_alpha_test_bit,

	// this bitmap will always be demand loaded, only supported by UI
	_bitmap_group_flag_only_use_on_demand_and_ui_on_demand_bitmap_bit,

	// generate a polygonal bounding box around the non - empty pixels to save fill rate cost
	_bitmap_group_flag_generate_tight_bounds_bit,

	// unchecked, tight bounds are generated from the color channel
	_bitmap_group_flag_tight_bounds_from_alpha_channel_bit,

	k_bitmap_group_flags
};

enum e_bitmap_curve_override
{
	// will choose FAST if your bitmap is bright
	_bitmap_curve_override_choose_best,

	// forces FAST mode, but causes banding in dark areas
	_bitmap_curve_override_force_fast,

	// chooses the best looking curve, probably slower
	_bitmap_curve_override_force_pretty,

	k_bitmap_curve_override_count
};

enum e_bitmap_usage_format
{
	// Use Default (defined by usage)
	_bitmap_usage_format_use_default_defined_by_usage = 0,

	// Best Compressed Color Format
	_bitmap_usage_format_best_compressed_color_format,

	// Best Uncompressed Color Format
	_bitmap_usage_format_best_uncompressed_color_format,

	// Best Compressed Bump Format
	_bitmap_usage_format_best_compressed_bump_format,

	// Best Uncompressed Bump Format
	_bitmap_usage_format_best_uncompressed_bump_format,

	// Best Compressed Monochrome Format
	_bitmap_usage_format_best_compressed_monochrome_format,

	// Best Uncompressed Monochrome Format
	_bitmap_usage_format_best_uncompressed_monochrome_format,

	// unused2!
	_bitmap_usage_format_unused_2,

	// unused3!
	_bitmap_usage_format_unused_3,

	// unused4!
	_bitmap_usage_format_unused_4,

	// unused5!
	_bitmap_usage_format_unused_5,

	// unused6!
	_bitmap_usage_format_unused_6,

	// --- Color and Alpha formats ---
	_bitmap_usage_format_color_and_alpha_formats,

	// DXT1 (Compressed Color + Color-Key Alpha)
	_bitmap_usage_format_dxt1_compressed_color_and_color_key_alpha,

	// DXT3 (Compressed Color + 4-bit Alpha)
	_bitmap_usage_format_dxt3_compressed_color_and_4bit_alpha,

	// DXT5 (Compressed Color + Compressed 8-bit Alpha)
	_bitmap_usage_format_dxt5_compressed_color_and_compressed_8bit_alpha,

	// 24-bit Color + 8-bit Alpha
	_bitmap_usage_format_24bit_color_and_8bit_alpha,

	// 8-bit Monochrome + 8-bit Alpha
	_bitmap_usage_format_8bit_monochrome_and_8bit_alpha,

	//    Channel Mask (3-bit Color + 1-bit Alpha)
	_bitmap_usage_format_channel_mask_3bit_color_and_1bit_alpha,

	//    30-bit Color + 2-bit Alpha
	_bitmap_usage_format_30bit_color_and_2bit_alpha,

	//    48-bit Color + 16-bit Alpha
	_bitmap_usage_format_48bit_color_and_16bit_alpha,

	//    HALF Color + Alpha
	_bitmap_usage_format_half_color_and_alpha,

	//    FLOAT Color + Alpha
	_bitmap_usage_format_float_color_and_alpha,

	// AY8 (8-bit Intensity replicated to ARGB)
	_bitmap_usage_format_ay8_8bit_intensity_replicated_to_argb,

	// DXT3A (4-bit Intensity replicated to ARGB)
	_bitmap_usage_format_dxt3a_4bit_intensity_replicated_to_argb,

	// DXT5A (DXT-compressed Intensity replicated to ARGB)
	_bitmap_usage_format_dxt5a_dxt_compressed_intensity_replicated_to_argb,

	// Compressed Monochrome + Alpha
	_bitmap_usage_format_compressed_monochrome_alpha,

	// A4R4G4B4 (12-bit color + 4-bit alpha)
	_bitmap_usage_format_a4r4g4b4_12bit_color_and_4bit_alpha,

	// --- Color only formats ---
	_bitmap_usage_format_color_only_formats,

	// 8-bit Monochrome
	_bitmap_usage_format_8_bit_monochrome,

	//    Compressed 24-bit Color
	_bitmap_usage_format_compressed_24_bit_color,

	//    32-bit Color (R11G11B10)
	_bitmap_usage_format_32bit_color_r11g11b10,

	//    16-bit Monochrome
	_bitmap_usage_format_16bit_monochrome,

	//    16-bit Red + Green Only
	_bitmap_usage_format_16bit_red_and_green_only,

	//    HALF Red Only
	_bitmap_usage_format_half_red_only,

	//    FLOAT Red Only
	_bitmap_usage_format_float_red_only,

	//    HALF Red + Green Only
	_bitmap_usage_format_half_red_and_green_only,

	//    FLOAT Red + Green Only
	_bitmap_usage_format_float_red_and_green_only,

	// Compressed 4-bit Monochrome
	_bitmap_usage_format_compressed_4bit_monochrome,

	// Compressed Interpolated Monochrome
	_bitmap_usage_format_compressed_interpolated_monochrome,

	// unused12!
	_bitmap_usage_format_unused12,

	// --- Alpha only formats ---
	_bitmap_usage_format_alpha_only_formats,

	// DXT3A (4-bit Alpha)
	_bitmap_usage_format_dxt3a_4bit_alpha,

	// DXT5A (8-bit Compressed Alpha)
	_bitmap_usage_format_dxt5a_8bit_compressed_alpha,

	// 8-bit Alpha
	_bitmap_usage_format_8bit_alpha,

	// unused13!
	_bitmap_usage_format_unused13,

	// unused14!
	_bitmap_usage_format_unused14,

	// unused15!
	_bitmap_usage_format_unused15,

	// --- Normal map formats ---
	_bitmap_usage_format_normal_map_formats,

	// DXN Compressed Normals (better)
	_bitmap_usage_format_dxn_compressed_normals_better,

	// CTX1 Compressed Normals (smaller)
	_bitmap_usage_format_ctx1_compressed_normals_smaller,

	// 16-bit Normals
	_bitmap_usage_format_16bit_normals,

	// 32-bit Normal
	_bitmap_usage_format_32bit_normals,

	k_bitmap_usage_format_count
};

struct bitmap_group
{
	static tag const k_group_tag = BITMAP_TAG;

	// choose how you are using this bitmap
	c_enum<e_bitmap_usage_global, int32, _bitmap_usage_global_diffuse_map, k_bitmap_usage_global_count> usage;

	c_flags<e_bitmap_group_flags, uns16, k_bitmap_group_flags> flags;

	// number of pixels between adjacent sprites (0 uses default, negative numbers set no spacing)
	int16 sprite_spacing;

	// the apparent height of the bump map above the triangle it is textured onto, in texture repeats (i.e., 1.0 would be as high as the texture is wide)
	real32 bump_map_height; // repeats

	// used by detail maps and illum maps.  0 means fade by last mipmap, 1 means fade by first mipmap
	real32 fade_factor; // [0,1]

	// automatic chooses FAST if your bitmap is bright, and PRETTY if your bitmap has dark bits
	c_enum<e_bitmap_curve_override, int8, _bitmap_curve_override_choose_best, k_bitmap_curve_override_count> curve_mode;

	// 0 = use default defined by usage
	int8 max_mipmap_level;

	// 0 = do not downsample source image
	int16 max_resolution;
	uns8 post_max_resolution[2];

	// overrides the format defined by usage
	c_enum<e_bitmap_usage_format, int16, _bitmap_usage_format_use_default_defined_by_usage, k_bitmap_usage_format_count> force_bitmap_format;

	s_tag_block tight_bounds;
	s_tag_block usage_override;
	s_tag_block manual_sequences;

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
	s_tag_data source_data;

	s_tag_data processed_pixel_data;
	c_typed_tag_block<bitmap_group_sequence> sequences;
	c_typed_tag_block<bitmap_data> bitmaps;
	s_tag_data xenon_processed_pixel_data;
	c_typed_tag_block<bitmap_data> xenon_bitmaps;
	c_typed_tag_block<c_typed_tag_resource<struct s_cache_file_tag_resource_data>> hardware_textures;
	s_tag_block interleaved_hardware_textures;
	int32 __unknownB4;
};
COMPILE_ASSERT(sizeof(bitmap_group) == 0xB8);

