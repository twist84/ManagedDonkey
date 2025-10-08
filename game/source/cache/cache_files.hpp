#pragma once

#include "cache/cache_file_builder_tag_resource_manager.hpp"
#include "cache/cache_file_tag_resource_runtime.hpp"
#include "cseries/cseries.hpp"
#include "cseries/language.hpp"
#include "memory/secure_signature.hpp"
#include "tag_files/files.hpp"
#include "tag_files/tag_groups.hpp"
#include "tag_files/tag_resource_internals.hpp"

#define TAG_GET(GROUP, TYPE, INDEX) ((TYPE*)tag_get((GROUP), (INDEX)))
#define TAG_GET_SAFE(GROUP, TYPE, INDEX) ((INDEX) != NONE ? ((TYPE*)tag_get((GROUP), (INDEX))) : NULL)
#define TAG_BLOCK_GET_ELEMENT(BLOCK, INDEX, TYPE) ((TYPE*)tag_block_get_element_with_size((BLOCK), (INDEX), sizeof(TYPE)))
#define TAG_BLOCK_GET_ELEMENT_SAFE(BLOCK, INDEX, TYPE) ((INDEX) != NONE ? ((TYPE*)tag_block_get_element_with_size((BLOCK), (INDEX), sizeof(TYPE))) : NULL)

struct s_cache_file_section_file_bounds
{
	int32 offset;
	int32 size;
};
static_assert(sizeof(s_cache_file_section_file_bounds) == 0x8);

const int32 k_cache_file_header_signature = 'head';
const int32 k_cache_file_footer_signature = 'foot';
const int32 k_cache_file_version = 18;

#pragma pack(push, 4)
union s_cache_file_header
{
	__pragma(warning(disable : 4200)) byte base[];

	struct
	{
		tag header_signature;
		int32 version;
		int32 file_size;

		int32 compressed_file_padding;
		int32 tags_offset;
		uns32 tag_buffer_offset;
		uns32 total_tags_size;

		c_static_string<k_tag_long_string_length> path;
		c_static_string<k_tag_string_length> build_number;

		int16 scenario_type;
		int16 shared_cache_file_type;

		bool uncompressed;
		bool tracked_build;
		bool valid_shared_resource_usage;
		uns8 header_flags;

		s_file_last_modification_date slot_modification_date;

		int32 low_detail_texture_number;
		uns32 low_detail_texture_offset;
		uns32 low_detail_texture_byte_count;

		int32 string_id_count;
		int32 string_id_data_count;
		uns32 string_id_index_offset;
		uns32 string_id_data_offset;

		// bool uses_shared_map[shared_file_count];
		// c_flags<e_cache_file_shared_file_type, uns8, k_number_of_cache_file_shared_file_types> shared_file_flags;
		uns8 shared_file_flags;

		s_file_last_modification_date creation_date;
		c_static_array<s_file_last_modification_date, k_number_of_cache_file_shared_file_types> shared_creation_date;

		c_static_string<k_tag_string_length> name;
		c_enum<e_language, int32, _language_invalid, k_language_count> language;
		c_static_string<k_tag_long_string_length> tag_path;
		int32 minor_version_number;

		int32 debug_tag_name_count;
		uns32 debug_tag_name_data_offset;
		int32 debug_tag_name_data_size;
		uns32 debug_tag_name_index_offset;

		s_cache_file_section_file_bounds reports;

		byte __data2E4[0x4];

		c_static_string<k_tag_string_length> creator_name;

		byte __data308[0x10];

		uns64 signature_marker;

		s_network_http_request_hash content_hashes[1];
		s_rsa_signature rsa_signature;

		c_static_array<int32, k_number_of_cache_file_sections> section_offsets;
		c_static_array<s_cache_file_section_file_bounds, k_number_of_cache_file_sections> original_section_bounds;

		s_cache_file_shared_resource_usage shared_resource_usage;

		int32 tag_cache_offsets;
		int32 tag_count;

		e_map_id map_id;
		int32 scenario_index;
		int32 cache_file_resource_gestalt_index; // 'zone' tags don't exist in ms23

		byte padding[0x594];

		tag footer_signature;
	};
};
static_assert(sizeof(s_cache_file_header) == 0x3390);
#pragma pack(pop)

union cache_address
{
	uns32 value;
	struct
	{
		uns32 offset : 30;
		uns32 persistent : 1;
		uns32 : 1;
	};
};
static_assert(sizeof(cache_address) == 0x4);

union cache_file_tag_instance
{
	__pragma(warning(disable : 4200)) byte base[];

	struct
	{
		uns32 checksum;
		uns32 total_size;
		int16 dependency_count;
		int16 data_fixup_count;
		int16 resource_fixup_count;
		int16 : 16;

		// offset from `base`
		uns32 offset;

		s_cache_file_tag_group tag_group;

		__pragma(warning(disable : 4200)) uns32 dependencies[];
	};

	template<typename t_type>
	t_type* cast_to()
	{
		return static_cast<t_type*>(cache_file_tag_instance::get());
	}

	void* get();
	int32 get_tag_index();
	const char* get_tag_name();
};
static_assert(sizeof(cache_file_tag_instance) == 0x24);

struct s_file_reference_persist
{
	tag signture;
	uns16 flags;
	int16 location;
	char path[108];
	uns32 handle;
	int32 position;
};
static_assert(sizeof(s_file_reference_persist) == 0x7C);

struct s_cache_file_report
{
	char __unknown0[32];
	uns32 hash[5];
	s_file_reference_persist file_reference;
	uns32 __unknownB0[20];
	uns32 __unknown100;
	uns32 __unknown104;
	uns32 __unknown108;
	uns32 __unknown10C;
	uns32 __unknown110;
};
static_assert(sizeof(s_cache_file_report) == 0x114);

// this is an actual calculation used within the game
#define GET_REPORT_COUNT_FROM_SIZE(REPORT_SIZE) (((0x76B981DBi64 * REPORT_SIZE) >> 32) >> 7)

static_assert(GET_REPORT_COUNT_FROM_SIZE(sizeof(s_cache_file_report) * 1000) == 1000);
static_assert(GET_REPORT_COUNT_FROM_SIZE(0x0004FC80) == 0x0004FC80 / sizeof(s_cache_file_report));

struct s_cache_file_reports
{
	int32 count;
	s_cache_file_report* elements;
};
static_assert(sizeof(s_cache_file_reports) == 0x8);

union s_cache_file_section_header
{
	__pragma(warning(disable : 4200)) byte base[];

	struct
	{
		uns32 __unknown0;
		int32 file_offsets;
		int32 file_count;
		uns32 __unknownC;
		s_file_last_modification_date creation_date;
		uns32 __unknown18;
		uns32 __unknown1C;
	};
};
static_assert(sizeof(s_cache_file_section_header) == 0x20);

const int32 k_tag_cache_maximum_files_count = 60000;
const int32 k_tag_cache_maximum_size = 0x4B00000;

struct s_cache_file_debug_globals
{
	uns32 debug_tag_name_offsets[k_tag_cache_maximum_files_count];
	char debug_tag_name_buffer[k_tag_cache_maximum_files_count * k_tag_long_string_length];
	const char* debug_tag_names[k_tag_cache_maximum_files_count];
};
static_assert(sizeof(s_cache_file_debug_globals) == 0xF1B300);

struct s_cache_file_globals
{
	bool tags_loaded;

	// physical_memory_malloc_fixed(sizeof(int32) * header.tag_count)
	//c_static_array<int32, k_tag_cache_maximum_files_count>& tag_cache_offsets;
	int32* tag_cache_offsets;

	// tag_instances[absolute_index] = tag_cache_base_address[total_tags_size]
	//c_static_array<cache_file_tag_instance*, k_tag_cache_maximum_files_count>& tag_instances;
	//c_static_array<cache_file_tag_instance*, k_tag_cache_maximum_files_count>& tag_instances;
	cache_file_tag_instance** tag_instances;

	// tag_index_absolute_mapping[tag_index] = absolute_index;
	//c_static_array<int32, k_tag_cache_maximum_files_count>& tag_index_absolute_mapping;
	int32* tag_index_absolute_mapping;

	// absolute_index_tag_mapping[absolute_index] = tag_index;
	//c_static_array<int32, k_tag_cache_maximum_files_count>& absolute_index_tag_mapping;
	int32* absolute_index_tag_mapping;

	int32 tag_loaded_count;
	int32 tag_total_count;

	//byte(&tag_cache_base_address)[k_tag_cache_maximum_size];
	byte* tag_cache_base_address;
	uns32 tag_loaded_size;
	uns32 tag_cache_size; // k_tag_cache_maximum_size

	s_cache_file_header header;

	// void* tags_section;
	s_file_reference tags_section;

	s_cache_file_resource_gestalt* resource_gestalt;

	// resource_file_counts_mapping[resource_file_index] = resource_count;
	c_static_array<int32, 5> resource_file_counts_mapping;

	s_cache_file_reports reports;

	c_static_array<const char*, 5> resource_files;
	const char* map_directory;
};
static_assert(sizeof(s_cache_file_globals) == 0x3500);

extern const char*& k_multiplayer_shared_scenario_tag;
extern const char*& k_single_player_shared_scenario_tag;
extern const char*& k_main_menu_scenario_tag;
extern const char*& k_introduction_scenario_tag;
extern const char*& k_cache_strings_file;
extern const char*& k_cache_tags_file;
extern const char*& k_cache_tag_list_file;
extern const char*& k_cache_resources_file;
extern const char*& k_cache_textures_file;
extern const char*& k_cache_textures_b_file;
extern const char*& k_cache_audio_file;
extern const char*& k_cache_video_file;
extern const char*& k_cache_file_extension;
extern const char* k_cache_path_format;

extern uns8 const g_cache_file_creator_key[64];

extern s_tag_reference g_last_tag_accessed;
extern s_cache_file_globals& g_cache_file_globals;

int32 __cdecl tag_loaded(tag group_tag, const char* tag_name);
const char* __cdecl tag_group_get_name(tag group_tag);

extern int32 __cdecl cache_file_get_global_tag_index(tag group_tag);
extern const s_cache_file_header* cache_files_get_header();

extern const char* tag_get_name(int32 tag_name_index);
extern const char* tag_get_name_safe(int32 tag_name_index);
extern int32 tag_name_get_index(tag group_tag, const char* name);

struct s_cache_file_security_globals;

extern bool __cdecl cache_file_blocking_read(int32 cache_file_section, int32 section_offset, int32 buffer_size, void* buffer);
extern bool __cdecl cache_file_content_signatures_match(int32 signature0_size, const byte* signature0, int32 signature1_size, const byte* signature1, bool unused);
extern bool __cdecl cache_file_get_content_signature(int32* out_signature_size, const byte** out_signature);
extern int32 __cdecl cache_file_get_global_tag_index(tag group_tag);
extern void __cdecl cache_file_get_path(const char* mapname, char* buffer, int32 buffer_size);
extern s_cache_file_security_globals* __cdecl cache_file_get_security_globals();
extern const void* __cdecl cache_file_globals_get_tag_cache_base_address();
extern bool __cdecl cache_file_header_verify(const s_cache_file_header* header, const char* scenario_path, bool fail_fatally);
extern bool __cdecl cache_file_header_verify_and_version(const s_cache_file_header* header, const char* scenario_path, bool fail_fatally);
extern void __cdecl cache_file_invalidate_signature();
extern real32 __cdecl cache_file_map_progress_estimated_megabytes_remaining(int32 scenario_type, const char* scenario_path);
extern int32 __cdecl cache_file_map_progress_estimated_miliseconds_remaining(int32 scenario_type, const char* scenario_path);
extern real32 __cdecl cache_file_map_progress_helper(int32 scenario_type, const char* scenario_path, int32 progress_type);
extern uns32 __cdecl cache_files_get_checksum();
extern const s_cache_file_header* __cdecl cache_files_get_header();
extern const s_rsa_signature* __cdecl cache_files_get_rsa_signature();
extern int32 __cdecl cache_files_get_total_tags_size();
extern const char* __cdecl cache_files_map_directory();
extern s_cache_file_resource_gestalt* __cdecl cache_files_populate_resource_gestalt();
extern bool __cdecl cache_files_verify_header_rsa_signature(s_cache_file_header* header);
extern uns32 __cdecl compute_realtime_checksum(char* buffer, int len);
extern void __cdecl cache_files_populate_resource_offsets(c_wrapped_array<uns32>* resource_offsets);
extern bool __cdecl cache_file_tags_section_read(int32 offset, int32 size, void* buffer);
extern void __cdecl cache_file_tags_unload();
extern bool __cdecl scenario_tags_load(const char* scenario_path);
extern void __cdecl scenario_tags_load_finished();
extern void __cdecl scenario_tags_unload();

extern void cache_file_transform_creator_string(c_wrapped_array<char> in_out_creator_string);
extern void cache_files_update_main_status();

struct s_tag_instance_datum : 
	s_datum_header
{
	uns16 flags;
	const char* name;
	s_cache_file_tag_group tag_group;
	int32 tag_index;
	void* instance;
	bool loaded;
};

struct tag_iterator
{
	c_data_iterator<void/*s_tag_instance_datum*/> iterator;
	tag key_group_tag;
};
static_assert(sizeof(tag_iterator) == 0x14);

extern void __cdecl tag_files_close();
extern void __cdecl tag_files_initialize_from_main();
extern void __cdecl tag_files_open();
extern void __cdecl tag_iterator_new(tag_iterator* iterator, tag group_tag);
extern int32 __cdecl tag_iterator_next(tag_iterator* iterator);

extern bool __cdecl cache_file_tags_load_recursive(int32 tag_index);
extern void __cdecl cache_file_tags_fixup_all_instances();
extern void* __cdecl tag_get(tag group_tag, int32 tag_index);
extern void* __cdecl tag_get(tag group_tag, const char* tag_name);
extern uns32 __cdecl tag_get_group_tag(int32 tag_index);
extern void __fastcall sub_503470(s_cache_file_reports* reports, void* unused, cache_file_tag_instance* tag_instance, int32 tag_index);

enum e_instance_modification_stage
{
	_instance_modification_stage_post_tag_load = 0,
	_instance_modification_stage_post_tag_fixup,
	_instance_modification_stage_post_scenario_tags_load,

	k_instance_modification_stages
};

extern void tag_instance_modification_apply(cache_file_tag_instance* instance, e_instance_modification_stage stage);

