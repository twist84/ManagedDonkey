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

const long k_cache_file_header_signature = 'head';
const long k_cache_file_footer_signature = 'foot';
const long k_cache_file_version = 18;

#pragma pack(push, 4)
union s_cache_file_header
{
#pragma warning(push)
#pragma warning(disable : 4200)
	byte base[];
#pragma warning(pop)

	struct
	{
		dword header_signature;
		long version;
		long size;

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
		s_file_last_modification_date modification_date;

		byte __data14C[0xC];

		long string_id_index_buffer_length;
		long string_id_string_storage_length;
		long string_id_index_buffer;
		long string_id_string_storage;
		dword_flags shared_file_flags; // bool uses_shared_map[shared_file_count];
		s_file_last_modification_date creation_time;
		s_file_last_modification_date shared_file_times[6];
		string name;
		dword game_language;
		long_string relative_path;
		long minor_version;
		long debug_tag_name_count;
		long debug_tag_name_buffer;
		long debug_tag_name_buffer_length;
		long debug_tag_name_offsets;
		s_cache_file_section_file_bounds reports;

		byte __data2E4[0x3C];

		s_network_http_request_hash hash;
		s_rsa_signature rsa_signature;
		c_static_array<long, 4> section_offsets;
		c_static_array<s_cache_file_section_file_bounds, 4> original_section_bounds;
		s_cache_file_shared_resource_usage shared_resource_usage;
		long insertion_point_resource_usage_count; // `has_insertion_points`
		c_static_array<s_cache_file_insertion_point_resource_usage, 9> insertion_point_resource_usage;
		long tag_cache_offsets;
		long tag_count;
		long map_id;
		long scenario_index;
		long cache_file_resource_gestalt_index; // 'zone' tags don't exist in ms23

		byte __data2DF8[0x594];

		dword footer_signature;
	};
};
static_assert(sizeof(s_cache_file_header) == 0x3390);
#pragma pack(pop)

union cache_file_tag_instance
{
#pragma warning(push)
#pragma warning(disable : 4200)
	byte base[];
#pragma warning(pop)

	struct
	{
		dword checksum;
		dword total_size;
		short dependency_count;
		short data_fixup_count;
		short resource_fixup_count;
		short : 16;

		// offset from `base`
		dword offset;

		tag group_tags[3];
		string_id group_name;
	};

	bool is_group(tag group_tag)
	{
		return group_tag != group_tags[0] && group_tag != group_tags[1] && group_tag != group_tags[2];
	}

	byte* get()
	{
		return base + offset;
	}
};
static_assert(sizeof(cache_file_tag_instance) == 0x24);

struct s_file_reference_persist
{
	tag signture;
	word_flags flags;
	short location;
	char path[108];
	dword handle;
	long position;
};
static_assert(sizeof(s_file_reference_persist) == 0x7C);

struct s_cache_file_report
{
	char __unknown0[32];
	dword hash[5];
	s_file_reference_persist file_reference;
	dword __unknownB0[20];
	dword __unknown100;
	dword __unknown104;
	dword __unknown108;
	dword __unknown10C;
	dword __unknown110;
};
static_assert(sizeof(s_cache_file_report) == 0x114);

struct s_cache_file_reports
{
	long count;
	s_cache_file_report* elements;
};
static_assert(sizeof(s_cache_file_reports) == 0x8);

union s_cache_file_tags_header
{
#pragma warning(push)
#pragma warning(disable : 4200)
	byte base[];
#pragma warning(pop)

	struct
	{
		dword __unknown0;
		long tag_cache_offsets;
		long tag_count;
		dword __unknownC;
		s_file_last_modification_date creation_date;
		dword __unknown18;
		dword __unknown1C;
	};
};
static_assert(sizeof(s_cache_file_tags_header) == 0x20);

const long k_tag_cache_maximum_files_count = 60000;
const long k_tag_cache_maximum_size = 0x4B00000;

struct s_cache_file_tag_name_collection
{
	long offsets[k_tag_cache_maximum_files_count];
	char buffer[k_tag_cache_maximum_files_count * 256];
	const char* storage[k_tag_cache_maximum_files_count];
};
static_assert(sizeof(s_cache_file_tag_name_collection) == 0xF1B300);

struct s_cache_file_globals
{
	s_cache_file_tag_name_collection* debug_tag_names;

	// padding?
	long __unknown4;

	bool tags_loaded;

	// physical_memory_malloc_fixed(sizeof(long) * header.tag_count)
	long(&tag_cache_offsets)[k_tag_cache_maximum_files_count];

	// tag_instances[absolute_index] = tag_cache_base_address[total_tags_size]
	cache_file_tag_instance*(&tag_instances)[k_tag_cache_maximum_files_count];

	// tag_index_absolute_mapping[tag_index] = absolute_index;
	long(&tag_index_absolute_mapping)[k_tag_cache_maximum_files_count];

	// absolute_index_tag_mapping[absolute_index] = tag_index;
	long(&absolute_index_tag_mapping)[k_tag_cache_maximum_files_count];

	long tag_loaded_count;
	long tag_total_count;

	byte(&tag_cache_base_address)[k_tag_cache_maximum_size];
	long tag_loaded_size;
	long tag_cache_size; // k_tag_cache_maximum_size

	s_cache_file_header header;

	// s_cache_file_tags_header* tags_header;
	s_file_reference tags_header;

	void* __unknown34C8;
	long __unknown34CC[5 /* `resource_files` count? */];

	long report_count;
	s_cache_file_report* reports;

	const char* resource_files[5];
	const char* map_directory;
};
static_assert(sizeof(s_cache_file_globals) == 0x3508);

extern s_cache_file_globals& g_cache_file_globals;

extern long __cdecl cache_file_get_global_tag_index(tag group_tag);
extern s_cache_file_header const* cache_files_get_header();

template<typename t_type = byte>
t_type* tag_get(tag group_tag, long tag_index)
{
	long tag_absolute_index = g_cache_file_globals.tag_index_absolute_mapping[tag_index];
	if (tag_absolute_index == -1)
		return nullptr;

	cache_file_tag_instance* instance = g_cache_file_globals.tag_instances[tag_absolute_index];
	if (!instance)
		return nullptr;

	if (instance->is_group(group_tag))
		return nullptr;

	return reinterpret_cast<t_type*>(instance->get());
}

extern char const* tag_get_name(long tag_name_index);
extern char const* tag_get_name_safe(long tag_name_index);

struct s_cache_file_security_globals;

bool __cdecl cache_file_blocking_read(long cache_file_section, long section_offset, long buffer_size, void* buffer);
bool __cdecl cache_file_content_signatures_match(long signature0_size, byte const* signature0, long signature1_size, byte const* signature1, bool unused);
bool __cdecl cache_file_get_content_signature(long* out_signature_size, byte const** out_signature);
long __cdecl cache_file_get_global_tag_index(tag group_tag);
void __cdecl cache_file_get_path(char const* mapname, char* buffer, long buffer_size);
s_cache_file_security_globals* __cdecl cache_file_get_security_globals();
void const* __cdecl cache_file_globals_get_tag_cache_base_address();
bool __cdecl cache_file_header_verify(s_cache_file_header const* header, char const* scenario_path, bool fail_fatally);
bool __cdecl cache_file_header_verify_and_version(s_cache_file_header const* header, char const* scenario_path, bool fail_fatally);
real __cdecl cache_file_map_progress_estimated_megabytes_remaining(long scenario_type, char const* scenario_path);
long __cdecl cache_file_map_progress_estimated_miliseconds_remaining(long scenario_type, char const* scenario_path);
real __cdecl cache_file_map_progress_helper(long scenario_type, char const* scenario_path, long progress_type);
dword __cdecl cache_files_get_checksum();
s_cache_file_header const* __cdecl cache_files_get_header();
s_rsa_signature const* __cdecl cache_files_get_rsa_signature();
long __cdecl cache_files_get_total_tags_size();
char const* __cdecl cache_files_map_directory();
bool __cdecl cache_files_verify_header_rsa_signature(s_cache_file_header* header);
dword __cdecl compute_realtime_checksum(char* a1, int a2);
bool __cdecl scenario_tags_load(char const* scenario_path);
void __cdecl scenario_tags_load_finished();
void __cdecl scenario_tags_unload();

struct tag_iterator;

void __cdecl tag_files_close();
void __cdecl tag_iterator_new(tag_iterator* iterator, tag group_tag);
long __cdecl tag_iterator_next(tag_iterator* iterator);
