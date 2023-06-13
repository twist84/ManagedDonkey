#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"
#include "tag_files/tag_resource_internals.hpp"

enum e_cache_file_tag_resource_location_flags
{
	_cache_file_tag_resource_location_flags_valid_checksum = 0,
	_cache_file_tag_resource_location_flags_resources,
	_cache_file_tag_resource_location_flags_textures,
	_cache_file_tag_resource_location_flags_textures_b,
	_cache_file_tag_resource_location_flags_audio,
	_cache_file_tag_resource_location_flags_video,
	_cache_file_tag_resource_location_flags_unused,
	_cache_file_tag_resource_location_flags_only_full_valid_checksum,

	k_cache_file_tag_resource_location_flags_count
};

enum e_cache_file_compression_codec
{
	_cache_file_compression_codec_none = NONE,
	_cache_file_compression_codec_lz,

	// custom compression codecs
	_cache_file_compression_codec_bitmap_texture_interop_resource,

	k_cache_file_compression_codec_count
};

struct s_cache_file_resource_file_location
{
	// runtime
	short resource_handle_salt;

	c_flags<e_cache_file_tag_resource_location_flags, byte, k_cache_file_tag_resource_location_flags_count> flags;
	c_enum<e_cache_file_compression_codec, char, _cache_file_compression_codec_none, k_cache_file_compression_codec_count> codec_index;

	union
	{
		// persistent file index, dependant on what location flag is active
		short shared_file_location_index;

		// runtime combined offset, at runtime all resource files are a combined buffer
		dword file_offset;
	};

	dword file_size;
	dword size;
	dword checksum;
	short resource_reference_count;
	short streaming_sublocation_table; // short_block_index
	dword __unknown18;
	dword __unknown1C;
	dword __unknown20;
};
static_assert(sizeof(s_cache_file_resource_file_location) == 0x24);

struct s_render_texture_format
{
	short width;
	short height;
	char depth;
	char total_mipmap_count;
	char bitmap_type;
	char is_high_res_bitmap;
	long xenon_d3d_format;
	char bm_format;
	char bm_curve;
	short bm_flags;
};
static_assert(sizeof(s_render_texture_format) == 0x10);

struct s_render_texture_descriptor
{
	s_tag_data base_pixel_data;
	s_tag_data high_res_pixel_data;
	s_render_texture_format texture;
};
static_assert(sizeof(s_render_texture_descriptor) == 0x38);

// #TODO: add other control data structures
union control_data_t
{
	s_render_texture_descriptor render_texture;
};

struct s_cache_file_resource_fixup_location
{
	long encoded_fixup_location;
	long encoded_fixup_value;
};
static_assert(sizeof(s_cache_file_resource_fixup_location) == 0x8);

struct s_tag_resource_interop_location
{
	c_tag_resource_fixup encoded_interop_location;
	long interop_type_index; // long_block_index
};
static_assert(sizeof(s_cache_file_resource_fixup_location) == 0x8);

enum e_cache_file_resource_type
{
	_cache_file_resource_type_structure_bsp_cache_file_tag_resources = 0,
	_cache_file_resource_type_bitmap_texture_interop_resource,
	_cache_file_resource_type_bitmap_texture_interleaved_interop_resource,
	_cache_file_resource_type_sound_resource_definition,
	_cache_file_resource_type_model_animation_tag_resource,
	_cache_file_resource_type_render_geometry_api_resource_definition,
	_cache_file_resource_type_bink_resource,
	_cache_file_resource_type_structure_bsp_tag_resources,

	k_cache_file_resource_type_count
};

extern char const* const k_cache_file_resource_type_names[k_cache_file_resource_type_count];

enum e_cache_file_resource_data_flags
{
	_cache_file_resource_data_flags_has_pageable_data = 0,
	_cache_file_resource_data_flags_has_optional_data,

	k_cache_file_resource_data_flags
};

struct s_cache_file_resource_runtime_data_new
{
	s_tag_reference owner_tag;
	short resource_salt;
	c_enum<e_cache_file_resource_type, char, _cache_file_resource_type_structure_bsp_cache_file_tag_resources, k_cache_file_resource_type_count> resource_type;
	char control_alignment_bits;
	c_typed_tag_data<control_data_t> control_data;
	c_tag_resource_fixup root_fixup;
	c_typed_tag_block<s_cache_file_resource_fixup_location> control_fixups;
	c_typed_tag_block<s_tag_resource_interop_location> interop_locations;
	c_flags<e_cache_file_resource_data_flags, dword, k_cache_file_resource_data_flags> flags;
};
static_assert(sizeof(s_cache_file_resource_runtime_data_new) == 0x48);

struct s_cache_file_tag_resource_data
{
	s_cache_file_resource_file_location file_location;
	s_cache_file_resource_runtime_data_new runtime_data;
};
static_assert(sizeof(s_cache_file_tag_resource_data) == 0x6C);

struct s_tag_resource_prediction_quantum;
struct s_tag_resource_prediction_atom;
struct s_tag_resource_prediction_molecule;
struct s_tag_resource_prediction_molecule_key;

struct s_tag_resource_cache_prediction_table
{
	c_typed_tag_block<s_tag_resource_prediction_quantum> prediction_quanta;
	c_typed_tag_block<s_tag_resource_prediction_atom> prediction_atoms;
	c_typed_tag_block<long> prediction_molecule_atoms;
	c_typed_tag_block<s_tag_resource_prediction_molecule> prediction_molecules;
	c_typed_tag_block<s_tag_resource_prediction_molecule_key> prediction_molecule_keys;
};
static_assert(sizeof(s_tag_resource_cache_prediction_table) == 0x3C);

struct s_cache_file_resource_gestalt
{
	union
	{
		struct
		{
			long loaded_resource_count;
			s_cache_file_tag_resource_data*(*loaded_resources)[60000];
			long : 32;
		};
	
		c_typed_tag_block<s_cache_file_tag_resource_data*> resources;
	};

	dword resource_loaded_size;
	dword __unknown10;
	s_tag_block designer_zone_manifests;
	s_tag_block global_zone_manifest;
	s_tag_block unattached_designer_zone_manifest;
	s_tag_block dvd_forbidden_zone_manifest;
	s_tag_block dvd_always_streaming_zone_manifest;
	s_tag_block default_bsp_zone_manifests;
	s_tag_block static_bsp_zone_manifests;
	s_tag_block dynamic_bsp_zone_manifests;
	s_tag_block cinematic_zone_manifests;
	s_tag_block zone_only_zone_set_manifests;
	s_tag_block expected_zone_set_manifests;
	s_tag_block fully_populated_zone_set_manifests;
	s_tag_block __unknownA4;
	s_tag_block __unknownB0;
	dword __unknownBC;
	s_tag_block __unknownC0;
	s_tag_block __unknownCC;
	s_tag_block __unknownD8;
	s_tag_block __unknownE4;
	s_tag_block __unknownF0;
	s_tag_block __unknownFC;
	s_tag_block __unknown108;
	s_tag_block __unknown114;
	s_tag_block __unknown120;
	s_tag_block __unknown12C;

	s_tag_resource_cache_prediction_table prediction_table;

	// Mat is in a really bad mood
	long campaign_id;

	// Next time we don't put things that the game depends on outside of tool, guerilla, or sapien
	long map_id;
};
static_assert(sizeof(s_cache_file_resource_gestalt) == 0x17C);

extern char const* cache_file_resource_type_get_name(long resource_type);

