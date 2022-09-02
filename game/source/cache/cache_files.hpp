#pragma once

#include "cache/cache_file_tag_resource_runtime.hpp"
#include "cseries/cseries.hpp"
#include "memory/secure_signature.hpp"
#include "tag_files/files.hpp"

struct s_cache_file_section_file_bounds
{
	long offset;
	long size;
};
static_assert(sizeof(s_cache_file_section_file_bounds) == 0x8);

struct s_cache_file_header
{
	dword header_signature;
	long file_version;
	long file_size;

	dword __unknownC;
	dword __unknown10;
	dword __unknown14;
	dword __unknown18;

	long_string source_file;
	string build;

	short scenario_type;
	short scenario_load_type;

	bool __unknown140;
	bool tracked_build;
	bool has_insertion_points;
	byte_flags header_flags;

	s_file_last_modification_date last_modification_date;

	byte __data14C[0xC];

	long string_id_index_buffer_count;
	long string_id_string_storage_size;
	long string_id_index_buffer_offset;
	long string_id_string_storage_offset;

	// bool uses_shared_map[shared_file_count];
	dword_flags shared_file_flags;

	s_file_last_modification_date creation_time;
	s_file_last_modification_date shared_file_times[6];

	string name;
	long game_language;
	long_string relative_path;
	long minor_version;

	long debug_tag_name_count;
	long debug_tag_name_buffer;
	long debug_tag_name_buffer_length;
	long debug_tag_name_offsets;

	s_cache_file_section_file_bounds reports;

	long __data2E4[0x3C/4];

	dword hash[5];
	dword rsa_signature[64];

	long section_offsets[4];
	s_cache_file_section_file_bounds original_section_bounds[4];

	s_cache_file_shared_resource_usage shared_resource_usage;

	// has_insertion_points
	long insertion_point_resource_usage_count;
	c_static_array<s_cache_file_insertion_point_resource_usage, 9> insertion_point_resource_usage;
	
	long tag_cache_offsets;
	long tag_count;
	long map_id;
	long scenario_index;

	// 'zone' tags don't exist in ms23
	long cache_file_resource_gestalt_index;

	byte __data2DF8[0x594];

	dword footer_signature;
};
static_assert(sizeof(s_cache_file_header) == 0x3390);

struct cache_file_tag_instance
{
	dword checksum;
	long total_size;
	short dependencies_count;
	short data_fixup_count;
	short resource_fixup_count;
	short : 16;
	dword address;
	tag group_tags[3];
	string_id group_name;
};
static_assert(sizeof(cache_file_tag_instance) == 0x24);

struct s_cache_file_tags_header
{
	dword __unknown0;
	long tag_cache_offsets;
	long tag_count;
	dword __unknownC;
	s_file_last_modification_date creation_date;
	dword __unknown18;
	dword __unknown1C;
};
static_assert(sizeof(s_cache_file_tags_header) == 0x20);

struct s_cache_file_tag_name_collection
{
	long offsets[60000];
	char buffer[60000 * 256];
	const char* storage[60000];
};
static_assert(sizeof(s_cache_file_tag_name_collection) == 0xF1B300);

struct s_cache_file_globals
{
	s_cache_file_tag_name_collection* debug_tag_names;

	// padding?
	long __unknown4;

	bool tags_loaded;

	// physical_memory_malloc_fixed(sizeof(long) * header.tag_count)
	long(*tag_cache_offsets)[60000];

	// tag_instances[absolute_index] = tag_cache_base_address[total_tags_size]
	cache_file_tag_instance*(*tag_instances)[60000];

	// tag_index_absolute_mapping[tag_index] = absolute_index;
	long(*tag_index_absolute_mapping)[60000];
	// absolute_index_tag_mapping[absolute_index] = tag_index;
	long(*absolute_index_tag_mapping)[60000];

	long tag_loaded_count;
	long tag_total_count;

	byte(*tag_cache_base_address)[0x4B00000];
	long tag_loaded_size;
	long tag_cache_size;

	s_cache_file_header header;

	// s_cache_file_tags_header* tags_header;
	s_file_reference tags_header;

	void* __unknown34C8;
	long __unknown34CC[5 /* `resource_files` count? */];

	long report_count;
	struct s_cache_file_report* reports;

	const char* resource_files[5];
	const char* map_directory;
};
static_assert(sizeof(s_cache_file_globals) == 0x3508);

extern s_cache_file_globals& g_cache_file_globals;