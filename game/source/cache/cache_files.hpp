#pragma once

#include "cache/cache_file_builder_tag_resource_manager.hpp"
#include "cache/cache_file_tag_resource_runtime.hpp"
#include "cseries/cseries.hpp"
#include "cseries/language.hpp"
#include "memory/secure_signature.hpp"
#include "tag_files/files.hpp"
#include "tag_files/tag_groups.hpp"
#include "tag_files/tag_resource_internals.hpp"

#define TAG_GET(GROUP, TYPE, INDEX) ((TYPE*)tag_get(GROUP, INDEX))

enum e_map_id;

enum e_cache_file_shared_file_type
{
	_cache_file_shared_file_type_ui = 0,
	_cache_file_shared_file_type_resources,
	_cache_file_shared_file_type_textures,
	_cache_file_shared_file_type_textures_b,
	_cache_file_shared_file_type_audio,
	_cache_file_shared_file_type_video,

	k_number_of_cache_file_shared_file_types
};

enum e_cache_file_section
{
	_cache_file_debug_section = 0,
	_cache_file_resource_section,
	_cache_file_tag_section,
	_cache_file_language_pack_section,

	k_number_of_cache_file_sections
};

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
	__pragma(warning(disable : 4200)) byte base[];

	struct
	{
		tag header_signature;
		long version;
		long file_size;

		long compressed_file_padding;
		long tags_offset;
		dword tag_buffer_offset;
		dword total_tags_size;

		c_static_string<k_tag_long_string_length> path;
		c_static_string<k_tag_string_length> build_number;

		short scenario_type;
		short shared_cache_file_type;

		bool uncompressed;
		bool tracked_build;
		bool valid_shared_resource_usage;
		byte_flags header_flags;

		s_file_last_modification_date slot_modification_date;

		long low_detail_texture_number;
		dword low_detail_texture_offset;
		dword low_detail_texture_byte_count;

		long string_id_count;
		long string_id_data_count;
		dword string_id_index_offset;
		dword string_id_data_offset;

		// bool uses_shared_map[shared_file_count];
		// c_flags<e_cache_file_shared_file_type, byte, k_number_of_cache_file_shared_file_types> shared_file_flags;
		byte_flags shared_file_flags;

		s_file_last_modification_date creation_date;
		c_static_array<s_file_last_modification_date, k_number_of_cache_file_shared_file_types> shared_creation_date;

		c_static_string<k_tag_string_length> name;
		c_enum<e_language, long, _language_invalid, k_language_count> language;
		c_static_string<k_tag_long_string_length> tag_path;
		long minor_version_number;

		long debug_tag_name_count;
		dword debug_tag_name_data_offset;
		long debug_tag_name_data_size;
		dword debug_tag_name_index_offset;

		s_cache_file_section_file_bounds reports;

		byte __data2E4[0x4];

		c_static_string<k_tag_string_length> creator_name;

		byte __data308[0x10];

		qword signature_marker;

		s_network_http_request_hash content_hashes[1];
		s_rsa_signature rsa_signature;

		c_static_array<long, k_number_of_cache_file_sections> section_offsets;
		c_static_array<s_cache_file_section_file_bounds, k_number_of_cache_file_sections> original_section_bounds;

		s_cache_file_shared_resource_usage shared_resource_usage;

		long tag_cache_offsets;
		long tag_count;

		e_map_id map_id;
		long scenario_index;
		long cache_file_resource_gestalt_index; // 'zone' tags don't exist in ms23

		byte padding[0x594];

		tag footer_signature;
	};
};
static_assert(sizeof(s_cache_file_header) == 0x3390);
#pragma pack(pop)

union cache_address
{
	dword value;
	struct
	{
		dword offset : 30;
		dword persistent : 1;
		dword : 1;
	};
};
static_assert(sizeof(cache_address) == 0x4);

union cache_file_tag_instance
{
	__pragma(warning(disable : 4200)) byte base[];

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

		s_cache_file_tag_group tag_group;

		__pragma(warning(disable : 4200)) dword dependencies[];
	};

	char const* get_name()
	{
		REFERENCE_DECLARE(base + total_size, c_static_string<k_tag_long_string_length>, tag_name);
		return tag_name.get_string();
	}

	void* get()
	{
		return base + offset;
	}

	template<typename t_type>
	t_type* cast_to()
	{
		return static_cast<t_type*>(get());
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

// this is an actual calculation used within the game
#define GET_REPORT_COUNT_FROM_SIZE(REPORT_SIZE) (((0x76B981DBi64 * REPORT_SIZE) >> 32) >> 7)

static_assert(GET_REPORT_COUNT_FROM_SIZE(sizeof(s_cache_file_report) * 1000) == 1000);
static_assert(GET_REPORT_COUNT_FROM_SIZE(0x0004FC80) == 0x0004FC80 / sizeof(s_cache_file_report));

struct s_cache_file_reports
{
	long count;
	s_cache_file_report* elements;
};
static_assert(sizeof(s_cache_file_reports) == 0x8);

union s_cache_file_section_header
{
	__pragma(warning(disable : 4200)) byte base[];

	struct
	{
		dword __unknown0;
		long file_offsets;
		long file_count;
		dword __unknownC;
		s_file_last_modification_date creation_date;
		dword __unknown18;
		dword __unknown1C;
	};
};
static_assert(sizeof(s_cache_file_section_header) == 0x20);

const long k_tag_cache_maximum_files_count = 60000;
const long k_tag_cache_maximum_size = 0x4B00000;

struct s_cache_file_debug_globals
{
	dword debug_tag_name_offsets[k_tag_cache_maximum_files_count];
	char debug_tag_name_buffer[k_tag_cache_maximum_files_count * k_tag_long_string_length];
	char const* debug_tag_names[k_tag_cache_maximum_files_count];
};
static_assert(sizeof(s_cache_file_debug_globals) == 0xF1B300);

struct s_cache_file_globals
{
	bool tags_loaded;

	// physical_memory_malloc_fixed(sizeof(long) * header.tag_count)
	//c_static_array<long, k_tag_cache_maximum_files_count>& tag_cache_offsets;
	long* tag_cache_offsets;

	// tag_instances[absolute_index] = tag_cache_base_address[total_tags_size]
	//c_static_array<cache_file_tag_instance*, k_tag_cache_maximum_files_count>& tag_instances;
	//c_static_array<cache_file_tag_instance*, k_tag_cache_maximum_files_count>& tag_instances;
	cache_file_tag_instance** tag_instances;

	// tag_index_absolute_mapping[tag_index] = absolute_index;
	//c_static_array<long, k_tag_cache_maximum_files_count>& tag_index_absolute_mapping;
	long* tag_index_absolute_mapping;

	// absolute_index_tag_mapping[absolute_index] = tag_index;
	//c_static_array<long, k_tag_cache_maximum_files_count>& absolute_index_tag_mapping;
	long* absolute_index_tag_mapping;

	long tag_loaded_count;
	long tag_total_count;

	//byte(&tag_cache_base_address)[k_tag_cache_maximum_size];
	byte* tag_cache_base_address;
	dword tag_loaded_size;
	dword tag_cache_size; // k_tag_cache_maximum_size

	s_cache_file_header header;

	// void* tags_section;
	s_file_reference tags_section;

	s_cache_file_resource_gestalt* resource_gestalt;

	// resource_file_counts_mapping[resource_file_index] = resource_count;
	c_static_array<long, 5> resource_file_counts_mapping;

	s_cache_file_reports reports;

	c_static_array<char const*, 5> resource_files;
	char const* map_directory;
};
static_assert(sizeof(s_cache_file_globals) == 0x3500);

extern char const* const& k_multiplayer_shared_scenario_tag;
extern char const* const& k_single_player_shared_scenario_tag;
extern char const* const& k_main_menu_scenario_tag;
extern char const* const& k_introduction_scenario_tag;
extern char const*& k_cache_strings_file;
extern char const*& k_cache_tags_file;
extern char const*& k_cache_tag_list_file;
extern char const*& k_cache_resources_file;
extern char const*& k_cache_textures_file;
extern char const*& k_cache_textures_b_file;
extern char const*& k_cache_audio_file;
extern char const*& k_cache_video_file;
extern char const*& k_cache_file_extension;
extern char const* k_cache_path_format;

extern byte const g_cache_file_creator_key[64];
extern long g_tag_total_count_pre_external_files;

extern s_tag_reference g_last_tag_accessed;
extern s_cache_file_globals& g_cache_file_globals;

long __cdecl tag_loaded(tag group_tag, char const* tag_name);
char const* __cdecl tag_group_get_name(tag group_tag);

extern long __cdecl cache_file_get_global_tag_index(tag group_tag);
extern s_cache_file_header const* cache_files_get_header();

extern char const* tag_get_name(long tag_name_index);
extern char const* tag_get_name_safe(long tag_name_index);
extern long tag_name_get_index(tag group_tag, char const* name);

struct s_cache_file_security_globals;

extern bool __cdecl cache_file_blocking_read(long cache_file_section, long section_offset, long buffer_size, void* buffer);
extern bool __cdecl cache_file_content_signatures_match(long signature0_size, byte const* signature0, long signature1_size, byte const* signature1, bool unused);
extern bool __cdecl cache_file_get_content_signature(long* out_signature_size, byte const** out_signature);
extern long __cdecl cache_file_get_global_tag_index(tag group_tag);
extern void __cdecl cache_file_get_path(char const* mapname, char* buffer, long buffer_size);
extern s_cache_file_security_globals* __cdecl cache_file_get_security_globals();
extern void const* __cdecl cache_file_globals_get_tag_cache_base_address();
extern bool __cdecl cache_file_header_verify(s_cache_file_header const* header, char const* scenario_path, bool fail_fatally);
extern bool __cdecl cache_file_header_verify_and_version(s_cache_file_header const* header, char const* scenario_path, bool fail_fatally);
extern void __cdecl cache_file_invalidate_signature();
extern real __cdecl cache_file_map_progress_estimated_megabytes_remaining(long scenario_type, char const* scenario_path);
extern long __cdecl cache_file_map_progress_estimated_miliseconds_remaining(long scenario_type, char const* scenario_path);
extern real __cdecl cache_file_map_progress_helper(long scenario_type, char const* scenario_path, long progress_type);
extern dword __cdecl cache_files_get_checksum();
extern s_cache_file_header const* __cdecl cache_files_get_header();
extern s_rsa_signature const* __cdecl cache_files_get_rsa_signature();
extern long __cdecl cache_files_get_total_tags_size();
extern char const* __cdecl cache_files_map_directory();
extern s_cache_file_resource_gestalt* __cdecl cache_files_populate_resource_gestalt();
extern bool __cdecl cache_files_verify_header_rsa_signature(s_cache_file_header* header);
extern dword __cdecl compute_realtime_checksum(char* buffer, int len);
extern void __cdecl cache_files_populate_resource_offsets(c_wrapped_array<dword>* resource_offsets);
extern bool __cdecl cache_file_tags_section_read(long offset, long size, void* buffer);
extern void __cdecl cache_file_tags_unload();
extern bool __cdecl scenario_tags_load(char const* scenario_path);
extern void __cdecl scenario_tags_load_finished();
extern void __cdecl scenario_tags_unload();

extern void cache_file_transform_creator_string(c_wrapped_array<char> in_out_creator_string);
extern void cache_files_update_main_status();

struct tag_iterator
{
	dword __unknown0;
	dword __unknown4;
	long datum_index;
	long absolute_tag_index;
	tag group_tag;
};
static_assert(sizeof(tag_iterator) == 0x14);

extern void __cdecl tag_files_close();
extern void __cdecl tag_files_initialize_from_main();
extern void __cdecl tag_files_open();
extern void __cdecl tag_iterator_new(tag_iterator* iterator, tag group_tag);
extern long __cdecl tag_iterator_next(tag_iterator* iterator);

extern bool __cdecl cache_file_tags_load_recursive(long tag_index);
extern void __cdecl cache_file_tags_fixup_all_instances();
extern void* __cdecl tag_get(tag group_tag, long tag_index);
extern void* __cdecl tag_get(tag group_tag, char const* tag_name);
extern dword __cdecl tag_get_group_tag(long tag_index);
extern void __fastcall sub_503470(s_cache_file_reports* reports, void* unused, cache_file_tag_instance* tag_instance, long tag_index);
extern void cache_file_tags_load_single_tag_file_test(char const* file_name);

enum e_instance_modification_stage
{
	_instance_modification_stage_post_tag_load = 0,
	_instance_modification_stage_post_tag_fixup,
	_instance_modification_stage_post_scenario_tags_load,

	k_instance_modification_stages
};

extern void tag_instance_modification_apply(cache_file_tag_instance* instance, e_instance_modification_stage stage);

