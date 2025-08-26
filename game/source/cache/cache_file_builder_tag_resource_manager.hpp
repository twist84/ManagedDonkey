#pragma once

#include "cache/cache_file_tag_resource_location_table.hpp"
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

	k_cache_file_tag_resource_location_flags,

	k_cache_file_tag_resource_location_mask
	= FLAG(_cache_file_tag_resource_location_flags_resources)
	| FLAG(_cache_file_tag_resource_location_flags_textures)
	| FLAG(_cache_file_tag_resource_location_flags_textures_b)
	| FLAG(_cache_file_tag_resource_location_flags_audio)
	| FLAG(_cache_file_tag_resource_location_flags_video)
};

struct s_cache_file_resource_checksum
{
	uns32 crc_value;
};

struct s_cache_file_resource_file_location
{
	// runtime
	int16 resource_handle_salt;

	c_flags<e_cache_file_tag_resource_location_flags, uns8, k_cache_file_tag_resource_location_flags> flags;
	int8 codec_index; // e_cache_file_compression_codec

	union
	{
		// persistent file index, dependant on what location flag is active
		int32 shared_file_location_index;

		// runtime combined offset, at runtime all resource files are a combined buffer
		uns32 file_offset;
	};

	uns32 file_size;
	uns32 size;
	s_cache_file_resource_checksum checksum;
	s_cache_file_resource_streaming_sublocation_table streaming_sublocation_table;
};
static_assert(sizeof(s_cache_file_resource_file_location) == 0x24);

struct s_render_texture_format
{
	int16 width;
	int16 height;
	int8 depth;
	int8 mipmap_count_including_highest;
	int8 bitmap_type;
	int8 is_high_res_bitmap;
	uns32 xenon_d3d_format;
	int8 bm_format;
	int8 bm_curve;
	uns16 bm_flags;
};
static_assert(sizeof(s_render_texture_format) == 0x10);

struct s_render_texture_descriptor
{
	s_tag_data base_pixel_data;
	s_tag_data high_res_pixel_data;
	s_render_texture_format texture;
};
static_assert(sizeof(s_render_texture_descriptor) == 0x38);

// $TODO add other control data structures
union control_data_t
{
	s_render_texture_descriptor render_texture;
};

struct s_cache_file_resource_fixup_location
{
	int32 encoded_fixup_location;
	int32 encoded_fixup_value;
};
static_assert(sizeof(s_cache_file_resource_fixup_location) == 0x8);

struct s_tag_resource_interop_location
{
	c_tag_resource_fixup encoded_interop_location;
	int32 interop_type_index; // long_block_index
};
static_assert(sizeof(s_cache_file_resource_fixup_location) == 0x8);

extern const char* const k_cache_file_resource_type_names[k_cache_file_resource_type_count];

struct s_cache_file_resource_runtime_data_new
{
	s_tag_reference owner_tag;
	int16 resource_salt;
	c_enum<e_cache_file_resource_type, int8, _cache_file_resource_type_structure_bsp_cache_file_tag_resources, k_cache_file_resource_type_count> resource_type;
	int8 control_alignment_bits;
	c_typed_tag_data<control_data_t> control_data;
	c_tag_resource_fixup root_fixup;
	c_typed_tag_block<s_cache_file_resource_fixup_location> control_fixups;
	c_typed_tag_block<s_tag_resource_interop_location> interop_locations;
	c_flags<e_cache_file_resource_data_flags, uns32, k_cache_file_resource_data_flags> flags;
};
static_assert(sizeof(s_cache_file_resource_runtime_data_new) == 0x48);

struct s_cache_file_tag_resource_data
{
	s_cache_file_resource_file_location file_location;
	s_cache_file_resource_runtime_data_new runtime_data;
};
static_assert(sizeof(s_cache_file_tag_resource_data) == 0x6C);

struct s_cache_file_tag_parentage
{
	s_tag_reference child_tag;
	uns16 flags;
	int16 resource_owner_index;
	c_typed_tag_block<int32> parent_indices; // s_cache_file_tag_parentage block indices
	c_typed_tag_block<int32> child_indices;  // s_cache_file_tag_parentage block indices
};
static_assert(sizeof(s_cache_file_tag_parentage) == 0x2C);

struct s_tag_resource_prediction_quantum;
struct s_tag_resource_prediction_atom;
struct s_tag_resource_prediction_molecule;
struct s_tag_resource_prediction_molecule_key;

struct s_tag_resource_cache_prediction_table
{
	c_typed_tag_block<s_tag_resource_prediction_quantum> quanta;
	c_typed_tag_block<s_tag_resource_prediction_atom> atoms;
	c_typed_tag_block<int32> molecule_atoms;
	c_typed_tag_block<s_tag_resource_prediction_molecule> molecules;
	c_typed_tag_block<s_tag_resource_prediction_molecule_key> molecule_keys;
};
static_assert(sizeof(s_tag_resource_cache_prediction_table) == 0x3C);

struct s_cache_file_zone_manifest;

struct s_cache_file_resource_gestalt
{
	c_typed_tag_block<s_cache_file_tag_resource_data*> resources;
	uns32 resources_size;
	uns32 resources_available;

	c_typed_tag_block<s_cache_file_zone_manifest> designer_zone_manifests;
	c_typed_tag_block<s_cache_file_zone_manifest> global_zone_manifests;
	c_typed_tag_block<s_cache_file_zone_manifest> unattached_designer_zone_manifests;
	c_typed_tag_block<s_cache_file_zone_manifest> dvd_forbidden_zone_manifests;
	c_typed_tag_block<s_cache_file_zone_manifest> dvd_always_streaming_zone_manifests;
	c_typed_tag_block<s_cache_file_zone_manifest> default_bsp_zone_manifests;
	c_typed_tag_block<s_cache_file_zone_manifest> static_bsp_zone_manifests;
	c_typed_tag_block<s_cache_file_zone_manifest> dynamic_bsp_zone_manifests;
	c_typed_tag_block<s_cache_file_zone_manifest> cinematic_zone_manifests;
	c_typed_tag_block<s_cache_file_zone_manifest> zones_only_zone_set_manifests;
	c_typed_tag_block<s_cache_file_zone_manifest> expected_zone_set_manifests;
	c_typed_tag_block<s_cache_file_zone_manifest> fully_populated_zone_set_manifests;
	s_tag_data naive_control_data;
	uns32 minimum_complete_pageable_data_size;
	uns32 minimum_required_pageable_data_size;
	uns32 minimum_required_dvd_data_size;
	uns32 global_pageable_data_size;
	uns32 optional_control_data_size;
	s_tag_block global_resource_usage;
	s_tag_block bsp_game_attachments;
	s_tag_block debug_resources_per_model_variant;
	s_tag_block debug_resources_per_combat_dialogue;
	s_tag_block debug_resources_per_tag;
	s_tag_block resource_debug_definitions;
	s_tag_block resource_layouts;
	s_tag_block resource_properties;

	c_typed_tag_block<s_cache_file_tag_parentage> parentages;
	s_tag_resource_cache_prediction_table prediction_table;

	// Mat is in a really bad mood
	e_campaign_id campaign_id;

	// Next time we don't put things that the game depends on outside of tool, guerilla, or sapien
	e_map_id map_id;
};
static_assert(sizeof(s_cache_file_resource_gestalt) == 0x17C);

extern const char* cache_file_resource_type_get_name(int32 resource_type);

