#include "cache/cache_files.hpp"

#include "bitmaps/bitmap_group_tag_definition.hpp"
#include "cache/cache_file_builder_security.hpp"
#include "cache/cache_file_tag_resource_runtime.hpp"
#include "cache/cache_files_windows.hpp"
#include "cache/physical_memory_map.hpp"
#include "cache/security_functions.hpp"
#include "config/version.hpp"
#include "cseries/async_helpers.hpp"
#include "cseries/cseries.hpp"
#include "effects/vision_mode.hpp"
#include "game/game_globals.hpp"
#include "game/multiplayer_definitions.hpp"
#include "interface/chud/chud_globals_definitions.hpp"
#include "items/equipment_definitions.hpp"
#include "items/item_definitions.hpp"
#include "items/projectile_definitions.hpp"
#include "main/global_preferences.hpp"
#include "main/loading.hpp"
#include "main/main.hpp"
#include "memory/crc.hpp"
#include "memory/module.hpp"
#include "objects/object_definitions.hpp"
#include "rasterizer/rasterizer.hpp"
#include "scenario/scenario.hpp"
#include "scenario/scenario_definitions.hpp"
#include "tag_files/string_ids.hpp"
#include "units/biped_definitions.hpp"
#include "units/unit_definition.hpp"
#include "units/vehicle_definitions.hpp"

#include <DDS.h>
#include <malloc.h>
#include <search.h>
#include <string.h>

REFERENCE_DECLARE(0x0189CFDC, char const* const, k_multiplayer_shared_scenario_tag);
REFERENCE_DECLARE(0x0189CFE0, char const* const, k_single_player_shared_scenario_tag);
REFERENCE_DECLARE(0x0189CFE4, char const* const, k_main_menu_scenario_tag);
REFERENCE_DECLARE(0x0189CFE8, char const* const, k_introduction_scenario_tag);
REFERENCE_DECLARE(0x0189CFEC, char const*, k_cache_strings_file);
REFERENCE_DECLARE(0x0189CFF0, char const*, k_cache_tags_file);
REFERENCE_DECLARE(0x0189CFF4, char const*, k_cache_tag_list_file);
REFERENCE_DECLARE(0x0189CFF8, char const*, k_cache_resources_file);
REFERENCE_DECLARE(0x0189CFFC, char const*, k_cache_textures_file);
REFERENCE_DECLARE(0x0189D000, char const*, k_cache_textures_b_file);
REFERENCE_DECLARE(0x0189D004, char const*, k_cache_audio_file);
REFERENCE_DECLARE(0x0189D008, char const*, k_cache_video_file);
REFERENCE_DECLARE(0x0189D00C, char const*, k_cache_file_extension);

char const* k_cache_path_format = "maps\\%s.map";

byte const g_cache_file_creator_key[64]
{
	0x05, 0x11, 0x6A, 0xA3, 0xCA, 0xB5, 0x07, 0xDF,
	0x50, 0xE7, 0x5B, 0x75, 0x6B, 0x4A, 0xBB, 0xF4,
	0xE8, 0x54, 0x8F, 0xC6, 0xD6, 0xCC, 0x92, 0x15,
	0x97, 0xDC, 0xF5, 0xEE, 0xB9, 0x3C, 0x01, 0x3C,
	0x95, 0xCF, 0xB8, 0x58, 0x5A, 0x6F, 0x2E, 0xB9,
	0x30, 0x6D, 0x89, 0x31, 0x2F, 0x83, 0x6F, 0xF0,
	0x9F, 0xE8, 0x37, 0x78, 0xE4, 0xC7, 0xE2, 0x2B,
	0x19, 0x66, 0x11, 0x06, 0x77, 0x24, 0x74, 0x66
};

long g_tag_total_count_pre_external_files = 0;

void* (__cdecl* tag_get_hook)(tag group_tag, long tag_index) = tag_get;

REFERENCE_DECLARE(0x022AAFE8, s_cache_file_debug_globals*, g_cache_file_debug_globals);
REFERENCE_DECLARE(0x022AAFF0, s_cache_file_globals, g_cache_file_globals);

HOOK_DECLARE(0x00501FC0, cache_files_map_directory);
HOOK_DECLARE(0x00501FD0, cache_files_populate_resource_gestalt);
HOOK_DECLARE(0x00502210, cache_files_verify_header_rsa_signature);
HOOK_DECLARE(0x00502780, cache_file_tags_load_recursive);
HOOK_DECLARE(0x00502CE0, cache_file_tags_unload);
HOOK_DECLARE(0x005031A0, cache_file_tags_fixup_all_instances);
HOOK_DECLARE(0x00503370, tag_get_hook);
HOOK_DECLARE(0x00503470, sub_503470);

s_tag_reference g_last_tag_accessed = { .group_tag = 0xFFFFFFFF, .index = NONE };

struct s_cache_file_tag_group_bsearch
{
public:
	static long __cdecl compare(tag group_tag, s_cache_file_tag_group const* group)
	{
		return group_tag - group->group_tags[0];
	}

	static long __cdecl search(tag group_tag, s_cache_file_tag_group const* group, long count)
	{
		ASSERT(m_sorted);

		long result = -1;
		s_cache_file_tag_group const* current_group = group;

		while (count)
		{
			s_cache_file_tag_group const* next_group = &current_group[count >> 1];
			long compare_result = compare(group_tag, next_group);

			if (!compare_result)
			{
				result = next_group - group;
				break;
			}

			if (compare_result > 0)
			{
				current_group = next_group + 1;
				--count;
			}

			count >>= 1;
		}

		return result;
	}

private:
	static int __cdecl sort_func(void const* a, void const* b)
	{
		s_cache_file_tag_group const* group_a = static_cast<s_cache_file_tag_group const*>(a);
		s_cache_file_tag_group const* group_b = static_cast<s_cache_file_tag_group const*>(b);

		return group_a->group_tags[0] - group_b->group_tags[0];
	}

	static bool __cdecl sort()
	{
		qsort(global_tag_groups, global_tag_group_count, sizeof(s_cache_file_tag_group), sort_func);

		return true;
	}

protected:
	static bool m_sorted;
};

bool s_cache_file_tag_group_bsearch::m_sorted = s_cache_file_tag_group_bsearch::sort();

char const* tag_group_get_name(tag group_tag)
{
	s_cache_file_tag_group const* group = nullptr;
	long index = s_cache_file_tag_group_bsearch::search(group_tag, global_tag_groups, global_tag_group_count);
	if (index != -1)
		group = &global_tag_groups[index];

	if (group)
		return group->name.get_string();

	return "";
}

long __cdecl tag_loaded(tag group_tag, char const* tag_name)
{
	if (g_cache_file_globals.tags_loaded)
	{
		//ASSERT(global_tag_instances);

		for (long i = 0; i < g_cache_file_globals.tag_loaded_count; i++)
		{
			cache_file_tag_instance* instance = g_cache_file_globals.tag_instances[i];

			if (instance->tag_group != group_tag)
				continue;

			long tag_index = g_cache_file_globals.absolute_index_tag_mapping[i];
			char const* name = tag_get_name(tag_index);
			if (csstricmp(tag_name, name) == 0)
				return tag_index;
		}
	}

	return NONE;
}

struct s_cache_file_global_tags_definition
{
	c_typed_tag_block<s_tag_reference> references;
	dword : 32;
};
static_assert(sizeof(s_cache_file_global_tags_definition) == 0x10);

char const* tag_get_name(long tag_name_index)
{
	if (!g_cache_file_globals.header.debug_tag_name_count)
		return nullptr;

	//ASSERT(g_cache_file_globals.tags_loaded);
	ASSERT(VALID_INDEX(tag_name_index, g_cache_file_globals.header.debug_tag_name_count));

	return g_cache_file_debug_globals->debug_tag_names[tag_name_index];

	//long tag_name_offset = g_cache_file_globals.debug_tag_names->offsets[tag_name_index];
	//ASSERT(VALID_INDEX(tag_name_offset, sizeof(g_cache_file_globals.debug_tag_names->buffer)));
	//
	//return &g_cache_file_globals.debug_tag_names->buffer[tag_name_offset];
}

char const* tag_get_name_safe(long tag_name_index)
{
	//ASSERT(g_cache_file_globals.tags_loaded);

	if (!g_cache_file_globals.header.debug_tag_name_count)
		return nullptr;

	if (VALID_INDEX(tag_name_index, g_cache_file_globals.header.debug_tag_name_count))
	{
		return g_cache_file_debug_globals->debug_tag_names[tag_name_index];

		//long tag_name_offset = g_cache_file_globals.debug_tag_names->offsets[tag_name_index];
		//if (VALID_INDEX(tag_name_offset, sizeof(g_cache_file_globals.debug_tag_names->buffer)))
		//	return &g_cache_file_globals.debug_tag_names->buffer[tag_name_offset];
	}

	return nullptr;
}

long tag_name_get_index(tag group_tag, char const* name)
{
	for (long tag_index = 0; tag_index < g_cache_file_globals.header.debug_tag_name_count; tag_index++)
	{
		char const* result = g_cache_file_debug_globals->debug_tag_names[tag_index];
		if (!result)
			continue;

		long tag_absolute_index = g_cache_file_globals.tag_index_absolute_mapping[tag_index];
		if (tag_absolute_index == NONE)
			continue;

		cache_file_tag_instance* tag_instance = g_cache_file_globals.tag_instances[tag_absolute_index];
		if (!tag_instance || tag_instance->tag_group != group_tag)
			continue;

		if (csstricmp(name, result) == 0)
			return tag_index;
	}

	return NONE;
}

//bool cache_file_blocking_read(enum e_cache_file_section,long,long,void *)
bool __cdecl cache_file_blocking_read(long cache_file_section, long section_offset, long buffer_size, void* buffer)
{
	return INVOKE(0x005016D0, cache_file_blocking_read, cache_file_section, section_offset, buffer_size, buffer);
}

bool __cdecl cache_file_content_signatures_match(long signature0_size, byte const* signature0, long signature1_size, byte const* signature1, bool unused)
{
	return INVOKE(0x00501740, cache_file_content_signatures_match, signature0_size, signature0, signature1_size, signature1, unused);
}

bool __cdecl cache_file_get_content_signature(long* out_signature_size, byte const** out_signature)
{
	return INVOKE(0x00501780, cache_file_get_content_signature, out_signature_size, out_signature);
}

long __cdecl cache_file_get_global_tag_index(tag group_tag)
{
	return INVOKE(0x005017E0, cache_file_get_global_tag_index, group_tag);

	//s_cache_file_global_tags_definition* global_tags = (s_cache_file_global_tags_definition*)tag_get(CACHE_FILE_GLOBAL_TAGS_TAG, static_cast<long>(0));
	//if (!global_tags)
	//	return NONE;
	//
	//for (s_tag_reference& tag_reference : global_tags->references)
	//{
	//	if (group_tag == tag_reference.group_tag)
	//		return tag_reference.index;
	//}
	//
	//return NONE;
}

void __cdecl cache_file_get_path(char const* mapname, char* buffer, long buffer_size)
{
	INVOKE(0x005018C0, cache_file_get_path, mapname, buffer, buffer_size);

	//csnzprintf(buffer, buffer_size, "%s%s%s", cache_files_map_directory(), mapname, k_cache_file_extension);
}

#pragma pack(push, 4)
struct s_cache_file_security_globals
{
	s_cache_file_header header;
	bool valid_content_signature;
	c_static_array<dword, 1> hash_sizes;
	c_static_array<void const*, 1> hash_addresses;
	c_static_array<s_network_http_request_hash, 1> hashes;
	struct
	{
		dword count;
		dword state[5];
		byte buffer[64];
		dword sha[234];
	}
	state;

	s_network_http_request_hash hash;
	s_rsa_signature rsa_signature;
	byte __data38C4[4];
};
static_assert(0x000038C8 == sizeof(s_cache_file_security_globals));
static_assert(0x00000000 == offsetof(s_cache_file_security_globals, header));
static_assert(0x00003390 == offsetof(s_cache_file_security_globals, valid_content_signature));
static_assert(0x00003394 == offsetof(s_cache_file_security_globals, hash_sizes));
static_assert(0x00003398 == offsetof(s_cache_file_security_globals, hash_addresses));
static_assert(0x0000339C == offsetof(s_cache_file_security_globals, hashes));
static_assert(0x000033B0 == offsetof(s_cache_file_security_globals, state));
static_assert(0x000037B0 == offsetof(s_cache_file_security_globals, hash));
static_assert(0x000037C4 == offsetof(s_cache_file_security_globals, rsa_signature));
static_assert(0x000038C4 == offsetof(s_cache_file_security_globals, __data38C4));
#pragma pack(pop)

s_cache_file_security_globals* __cdecl cache_file_get_security_globals()
{
	return INVOKE(0x005018F0, cache_file_get_security_globals);
}

void const* __cdecl cache_file_globals_get_tag_cache_base_address()
{
	//return INVOKE(0x00501930, cache_file_globals_get_tag_cache_base_address);

	if (!g_cache_file_globals.tags_loaded)
		return nullptr;

	return g_cache_file_globals.tag_cache_base_address;
}

bool __cdecl cache_file_header_verify(s_cache_file_header const* header, char const* scenario_path, bool fail_fatally)
{
	return INVOKE(0x00501950, cache_file_header_verify, header, scenario_path, fail_fatally);

	//c_static_string<k_tag_long_string_length> error;
	//s_file_last_modification_date file_time{};
	//
	//get_current_file_time(&file_time);
	//
	//bool error_occurred = false;
	//if (header->version != k_cache_file_version)
	//{
	//	error.print("is an old version (%d, should be %d)",
	//		header->version,
	//		k_cache_file_version);
	//	error_occurred = true;
	//}
	//
	//if (header->header_signature != k_cache_file_header_signature)
	//{
	//	error.print("does not have a valid header signature (is %08x, should be %08x)",
	//		header->header_signature,
	//		k_cache_file_header_signature);
	//	error_occurred = true;
	//}
	//
	//if (header->footer_signature != k_cache_file_footer_signature)
	//{
	//	error.print("does not have a valid footer signature (is %08x, should be %08x)",
	//		header->footer_signature,
	//		k_cache_file_footer_signature);
	//	error_occurred = true;
	//}
	//
	//if (header->size < 0 || header->size > LONG_MAX /*cache_file_get_absolute_maximum_size()*/)
	//{
	//	error.print("had a size out of range for any cache file (%d, should be within [0, %d])",
	//		header->size,
	//		LONG_MAX /*cache_file_get_absolute_maximum_size()*/);
	//	error_occurred = true;
	//}
	//
	//if (header->scenario_type < 0 || header->scenario_type >= 5)
	//{
	//	error.print(
	//		"had an invalid scenario type (%d, should be within [0, %d))",
	//		header->scenario_type,
	//		5);
	//	error_occurred = true;
	//}
	//
	//if (strlen(header->name) >= sizeof(header->name))
	//{
	//	error.print("had a corrupt name");
	//	error_occurred = true;
	//}
	//
	//if (!cache_files_verify_header_rsa_signature(const_cast<s_cache_file_header*>(header)))
	//{
	//	error.print("failed actual validation");
	//	error_occurred = true;
	//}
	//
	//if (error_occurred)
	//	c_console::write_line("the cache file '%s' won't load: %s", scenario_path, error.get_string());
	//
	//if (fail_fatally && error_occurred)
	//	ASSERT_EXCEPTION("cache_file_header_verify() fatally failed, see error output!", true);
	//
	//return !error_occurred;
}

bool __cdecl cache_file_header_verify_and_version(s_cache_file_header const* header, char const* scenario_path, bool fail_fatally)
{
	return INVOKE(0x00501AD0, cache_file_header_verify_and_version, header, scenario_path, fail_fatally);
}

void __cdecl cache_file_invalidate_signature()
{
	INVOKE(0x00501B20, cache_file_invalidate_signature);
}

//float cache_file_map_progress_estimated_megabytes_remaining(enum e_scenario_type,char const *)
real __cdecl cache_file_map_progress_estimated_megabytes_remaining(long scenario_type, char const* scenario_path)
{
	return INVOKE(0x00501B90, cache_file_map_progress_estimated_megabytes_remaining, scenario_type, scenario_path);
}

//long cache_file_map_progress_estimated_miliseconds_remaining(enum e_scenario_type,char const *)
long __cdecl cache_file_map_progress_estimated_miliseconds_remaining(long scenario_type, char const* scenario_path)
{
	return INVOKE(0x00501BB0, cache_file_map_progress_estimated_miliseconds_remaining, scenario_type, scenario_path);
}

//float cache_file_map_progress_helper(enum e_scenario_type, char const*, enum e_cache_file_progress_type)
real __cdecl cache_file_map_progress_helper(long scenario_type, char const* scenario_path, long progress_type)
{
	return INVOKE(0x00501BF0, cache_file_map_progress_helper, scenario_type, scenario_path, progress_type);
}

dword __cdecl cache_files_get_checksum()
{
	return INVOKE(0x00501F40, cache_files_get_checksum);
}

s_cache_file_header const* __cdecl cache_files_get_header()
{
	//return INVOKE(0x00501F90, cache_files_get_header);

	ASSERT(g_cache_file_globals.header.header_signature == k_cache_file_header_signature);
	ASSERT(g_cache_file_globals.header.version == k_cache_file_version);
	ASSERT(g_cache_file_globals.header.footer_signature == k_cache_file_footer_signature);

	return &g_cache_file_globals.header;
}

s_rsa_signature const* __cdecl cache_files_get_rsa_signature()
{
	return INVOKE(0x00501FA0, cache_files_get_rsa_signature);
}

long __cdecl cache_files_get_total_tags_size()
{
	return INVOKE(0x00501FB0, cache_files_get_total_tags_size);
}

char const* __cdecl cache_files_map_directory()
{
	//return INVOKE(0x00501FC0, cache_files_map_directory);

	if (!g_cache_file_globals.map_directory)
		g_cache_file_globals.map_directory = "maps\\";

	return g_cache_file_globals.map_directory;
}

void __cdecl cache_file_tags_fixup_all_resources(c_wrapped_array<dword>& resource_offsets, s_cache_file_resource_gestalt* resource_gestalt)
{
	tag_block_set_elements(&resource_gestalt->resources, resource_gestalt + 1);
	g_cache_file_globals.resource_gestalt = resource_gestalt;

	s_cache_file_tag_resource_data** resource_blocks = resource_gestalt->resources.begin();

	long resource_count = 0;
	dword resources_size = 0;
	dword resources_available = 0;

	for (long i = 0; i < g_cache_file_globals.tag_loaded_count; i++)
	{
		cache_file_tag_instance* instance = g_cache_file_globals.tag_instances[i];
		cache_address* resource_fixups = reinterpret_cast<cache_address*>(&instance->dependencies[instance->dependency_count + instance->data_fixup_count]);

		for (long resource_fixup_index = 0; resource_fixup_index < instance->resource_fixup_count; resource_fixup_index++)
		{
			cache_address resource_fixup = resource_fixups[resource_fixup_index];

			if (!resource_fixup.value)
				continue;

			if (resource_fixup.persistent)
				resource_fixup.persistent = false;
			resource_fixup.value += (dword)instance;

			s_tag_resource* resource = (s_tag_resource*)resource_fixup.value;
			if (!resource)
				continue;

			if (!resource->resource_data)
			{
				resource->resource_handle = NONE;
				continue;
			}

			resource_blocks[resource_count] = resource->resource_data;
			s_cache_file_tag_resource_data* resource_data = resource->resource_data;

			byte_flags flags = resource_data->file_location.flags.get_unsafe();

			if (!TEST_MASK(flags, k_cache_file_tag_resource_location_mask))
			{
				resources_available = 0;
				resource->resource_handle = resource_count++;
				resources_size += resource_data->file_location.size;
				continue;
			}

			if (resource_data->file_location.shared_file_location_index == NONE)
			{
				resource_data->file_location.file_offset = 0;

				resources_available = 1;
				resource->resource_handle = resource_count++;
				resources_size += resource_data->file_location.size;
				continue;
			}

			long resource_index = 0;
			for (long map_file_index = 0; map_file_index < k_cached_map_file_shared_count - 1; map_file_index++)
			{
				if (!TEST_BIT(flags, map_file_index + 1))
				{
					resource_index += g_cache_file_globals.resource_file_counts_mapping[map_file_index];
					continue;
				}

				if (!cached_map_file_is_shared(e_map_file_index(map_file_index)))
				{
					resource_data->file_location.file_offset = 0;
					continue;
				}

				ASSERT(resource_index + resource_data->file_location.shared_file_location_index < resource_offsets.count());
				resource_data->file_location.file_offset = resource_offsets[resource_index + resource_data->file_location.shared_file_location_index];
				break;
			}

			resources_available = 1;
			resource->resource_handle = resource_count++;
			resources_size += resource_data->file_location.size;
		}
	}

	tag_block_set_element_count(&resource_gestalt->resources, resource_count);
	resource_gestalt->resources_size = resources_size;
	resource_gestalt->resources_available = resources_available;
}

s_cache_file_resource_gestalt* __cdecl cache_files_populate_resource_gestalt()
{
	c_wrapped_array<dword> resource_offsets;
	cache_files_populate_resource_offsets(&resource_offsets);

	long total_resource_fixup_count = 0;
	for (long i = 0; i < g_cache_file_globals.tag_loaded_count; i++)
		total_resource_fixup_count += g_cache_file_globals.tag_instances[i]->resource_fixup_count;

	long resource_fixup_size = sizeof(s_cache_file_tag_resource_data*) * total_resource_fixup_count;
	long resource_gestalt_size = resource_fixup_size + sizeof(s_cache_file_resource_gestalt);

	s_cache_file_resource_gestalt* resource_gestalt = (s_cache_file_resource_gestalt*)_physical_memory_malloc_fixed(_memory_stage_level_initialize, NULL, resource_gestalt_size, 0);
	csmemset(resource_gestalt, 0, resource_gestalt_size);

	cache_file_tags_fixup_all_resources(resource_offsets, resource_gestalt);

	if (resource_offsets.m_elements)
		physical_memory_free(resource_offsets.m_elements);

	return resource_gestalt;
}

//.text:005021B0 ; 

const bool override_cache_file_header_security_validate_hash = true;
const bool override_cache_file_header_security_rsa_compute_and_verify_signature = true;

bool __cdecl cache_files_verify_header_rsa_signature(s_cache_file_header* header)
{
	//bool result = false;
	//HOOK_INVOKE(result =, cache_files_verify_header_rsa_signature, header);
	//return result;

	if (header->header_signature != k_cache_file_header_signature || header->footer_signature != k_cache_file_footer_signature)
	{
		display_debug_string("cache_files:header is invalid, header/footer signatures are bad");
		return false;
	}

	s_cache_file_header clean_header{};
	memmove(&clean_header, header, sizeof(s_cache_file_header));
	cache_file_builder_security_clean_header(&clean_header);

	static char hash_string[4096]{};
	s_network_http_request_hash hash{};
	if (!security_validate_hash(&clean_header, sizeof(s_cache_file_header), true, &header->hash, nullptr))
	{
		if (!override_cache_file_header_security_validate_hash)
		{
			display_debug_string("cache_files:header failed hash verification - possible cheating?");
			return false;
		}

		type_as_byte_string(&hash, hash_string);
		display_debug_string("cache_files:header: failed hash verification - copying new validated values, %s", hash_string);

		csmemcpy(&header->hash, &hash, sizeof(s_network_http_request_hash));
	}

	//type_as_byte_string(&hash, hash_string);

	security_calculate_hash(&header->hash, sizeof(s_network_http_request_hash), true, &hash);

	s_rsa_signature rsa_signature{};
	csmemcpy(&rsa_signature, &header->rsa_signature, sizeof(s_rsa_signature));

	bool result = security_rsa_compute_and_verify_signature(&hash, &rsa_signature);
	if (!result)
	{
		if (!override_cache_file_header_security_rsa_compute_and_verify_signature)
		{
			display_debug_string("cache_files:header failed RSA signature verification - possible cheating?");
			return false;
		}
	}
	return true;
}

dword __cdecl compute_realtime_checksum(char* a1, int a2)
{
	return INVOKE(0x00502300, compute_realtime_checksum, a1, a2);
}

void __cdecl cache_file_load_reports(s_cache_file_reports* reports, s_cache_file_header* header)
{
	DECLFUNC(0x00502500, void, __thiscall, s_cache_file_reports*, s_cache_file_header*)(reports, header);

	//long reports_buffer_size = cache_file_round_up_read_size(header->reports.size);
	//void* reports_buffer = _physical_memory_malloc_fixed(_memory_stage_level_initialize, NULL, reports_buffer_size, 0);
	//
	//cache_file_blocking_read(_cache_file_section_debug, header->reports.offset, reports_buffer_size, reports_buffer);
	//
	//reports->elements = static_cast<s_cache_file_report*>(reports_buffer);
	//reports->count = GET_REPORT_COUNT_FROM_SIZE(header->reports.size);
}

void __cdecl cache_files_populate_resource_offsets(c_wrapped_array<dword>* resource_offsets)
{
	//INVOKE(0x00502550, cache_files_populate_resource_offsets, resource_offsets);

	bool success = true;

	// shared map files - ui
	s_file_handle section_handles[k_cached_map_file_shared_count - 1]{};
	s_cache_file_section_header section_headers[k_cached_map_file_shared_count - 1]{};

	long file_count = 0;
	for (long i = 0; i < NUMBEROF(section_handles); i++)
	{
		e_map_file_index map_file_index = e_map_file_index(i);
		e_map_file_index next_map_file_index = e_map_file_index(i + 1);

		if (!cached_map_file_is_shared(map_file_index))
			continue;

		if (!cache_file_get_async_file_handle_from_index(next_map_file_index, &section_handles[map_file_index]))
		{
			success = false;
			break;
		}

		c_synchronized_long done = 0;
		c_synchronized_long size = 0;

		async_read_position(
			section_handles[i],
			&section_headers[i],
			sizeof(s_cache_file_section_header),
			0,
			_async_category_none,
			_async_priority_blocking_generic,
			&size,
			&done);

		internal_async_yield_until_done(&done, false, false, __FILE__, __LINE__);

		if (size.peek() != sizeof(s_cache_file_section_header))
		{
			success = false;
			break;
		}

		long section_file_count = section_headers[i].file_count;
		file_count += section_file_count;

		g_cache_file_globals.resource_file_counts_mapping[i] = section_file_count;
	}

	if (!success)
	{
		resource_offsets->set_elements(NULL, 0);
		return;
	}

	dword* file_offsets = (dword*)_physical_memory_malloc_fixed(_memory_stage_level_initialize, NULL, sizeof(dword) * file_count, 0);

	long offset_index_offset = 0;
	for (long i = 0; i < NUMBEROF(section_handles); i++)
	{
		e_map_file_index map_file_index = e_map_file_index(i);
		if (!cached_map_file_is_shared(map_file_index))
			continue;

		void* offsets = (byte*)file_offsets + offset_index_offset;
		long offsets_size = sizeof(long) * section_headers[i].file_count;

		c_synchronized_long done = 0;
		c_synchronized_long size = 0;

		async_read_position(
			section_handles[i],
			offsets,
			offsets_size,
			section_headers[i].file_offsets,
			_async_category_none,
			_async_priority_blocking_generic,
			&size,
			&done);

		internal_async_yield_until_done(&done, false, false, __FILE__, __LINE__);

		if (size.peek() != offsets_size)
		{
			success = false;
			break;
		}

		offset_index_offset += offsets_size;
	}

	if (!success)
	{
		physical_memory_free(file_offsets);
		resource_offsets->set_elements(NULL, 0);
		return;
	}

	resource_offsets->set_elements(file_offsets, file_count);
}

bool __cdecl cache_file_tags_load_recursive(long tag_index)
{
	//return INVOKE(0x00502780, cache_file_tags_load_recursive, tag_index);

	if (g_cache_file_globals.tag_cache_size <= g_cache_file_globals.tag_loaded_size)
		ASSERT2("tag cache insufficient memory allocation size, either decrease the size of the loaded tags for this scenario or try enabling the 'EXPERIMENTAL_TAG_CACHE_ALLOCATION' define if this persists");

	cache_file_tag_instance* instance = reinterpret_cast<cache_file_tag_instance*>(g_cache_file_globals.tag_cache_base_address + g_cache_file_globals.tag_loaded_size);

	long& tag_loaded_count = g_cache_file_globals.tag_loaded_count;
	long tag_cache_offset = g_cache_file_globals.tag_cache_offsets[tag_index];

	if (g_cache_file_globals.tag_index_absolute_mapping[tag_index] != NONE)
		return true;

	if (!cache_file_tags_section_read(tag_cache_offset, cache_file_round_up_read_size(sizeof(cache_file_tag_instance)), instance))
		return false;

	g_cache_file_globals.tag_loaded_size += instance->total_size;
	g_cache_file_globals.tag_instances[tag_loaded_count] = instance;
	g_cache_file_globals.tag_index_absolute_mapping[tag_index] = tag_loaded_count;
	g_cache_file_globals.absolute_index_tag_mapping[tag_loaded_count] = tag_index;

	REFERENCE_DECLARE(instance->base + instance->total_size, c_static_string<k_tag_long_string_length>, tag_name);
	tag_name.print("%s", tag_get_name(tag_index));
	g_cache_file_globals.tag_loaded_size += cache_file_round_up_read_size(cache_file_round_up_read_size(tag_name.length()));

	if (!cache_file_tags_section_read(tag_cache_offset, instance->total_size, instance->base))
		return false;

	if (crc_checksum_buffer_adler32(adler_new(), instance->base + sizeof(instance->checksum), instance->total_size - sizeof(instance->checksum)) != instance->checksum)
		return false;

	// not needed
	//sub_503470(&g_cache_file_globals.reports, tag_instance, tag_index);

	tag_loaded_count++;

	tag_instance_modification_apply(instance, _instance_modification_stage_tag_load);

	if (instance->dependency_count <= 0)
		return true;

	short dependency_index = 0;
	while (cache_file_tags_load_recursive(instance->dependencies[dependency_index]))
	{
		if (++dependency_index >= instance->dependency_count)
			return true;
	}

	return false;
}

// to mitigate a lot of file read operations we load all of `tags.dat` into this buffer
// I don't like having this as a global but for right now its fine
static void* tags_section = nullptr;

void __cdecl cache_file_load_tags_section()
{
	//INVOKE(0x005028C0, cache_file_load_tags_section);

	if (TEST_BIT(g_cache_file_globals.header.shared_file_flags, 1))
	{
		if (g_cache_file_globals.tags_section.path.is_empty())
			file_reference_create_from_path(&g_cache_file_globals.tags_section, k_cache_tags_file, false);

		dword error = 0;
		if (file_open(&g_cache_file_globals.tags_section, FLAG(_file_open_flag_desired_access_read), &error))
		{
			dword file_size = 0;
			file_get_size(&g_cache_file_globals.tags_section, &file_size);

			if (tags_section)
			{
				free(tags_section);
				tags_section = nullptr;
			}

			if (tags_section = malloc(file_size))
			{
				csmemset(tags_section, 0, file_size);
				file_read(&g_cache_file_globals.tags_section, file_size, false, tags_section);
			}
		}
	}
}
HOOK_DECLARE_CALL(0x00502E87, cache_file_load_tags_section);

void __cdecl cache_file_close_tags_section()
{
	//INVOKE(0x00502900, cache_file_close_tags_section);

	if (TEST_BIT(g_cache_file_globals.header.shared_file_flags, 1))
	{
		file_close(&g_cache_file_globals.tags_section);

		if (tags_section)
		{
			free(tags_section);
			tags_section = nullptr;
		}
	}
}
//HOOK_DECLARE(0x00502900, cache_file_close_tags_section);

bool __cdecl tags_section_file_close(s_file_reference* reference)
{
	cache_file_close_tags_section();

	return true;
}
HOOK_DECLARE_CALL(0x00502F2E, tags_section_file_close);

bool __cdecl cache_file_tags_single_tag_instance_fixup(cache_file_tag_instance* instance)
{
	ASSERT(instance);

	cache_address* data_fixups = reinterpret_cast<cache_address*>(instance->dependencies + instance->dependency_count);
	for (short data_fixup_index = 0; data_fixup_index < instance->data_fixup_count; data_fixup_index++)
	{
		cache_address& data_fixup = *reinterpret_cast<cache_address*>(instance->base + data_fixups[data_fixup_index].offset);

		// 0.4.11.2 tags messed up `ui\halox\pregame_lobby\switch_lobby\lobbies.gui_datasource_definition` data fixups
		if (!data_fixup.value)
			continue;

		// 0.4.11.2 tags messed up `levels\multi\s3d_avalanche\s3d_avalanche.scenario` data fixups
		if (!data_fixup.persistent)
			continue;

		ASSERT(data_fixup.persistent == true);
		data_fixup.persistent = false;
		data_fixup.value += (dword)instance;
		//ASSERT(data_fixup.value == data_fixup.offset);
	}

	tag_instance_modification_apply(instance, _instance_modification_stage_tag_fixup);

	return true;
}

bool __cdecl cache_file_debug_tag_names_load()
{
	//return INVOKE(0x00502970, cache_file_debug_tag_names_load);

	// header
	decltype(g_cache_file_globals.header.debug_tag_name_count)& debug_tag_name_count = g_cache_file_globals.header.debug_tag_name_count;
	decltype(g_cache_file_globals.header.debug_tag_name_buffer)& debug_tag_name_buffer = g_cache_file_globals.header.debug_tag_name_buffer;
	decltype(g_cache_file_globals.header.debug_tag_name_buffer_length)& debug_tag_name_buffer_length = g_cache_file_globals.header.debug_tag_name_buffer_length;
	decltype(g_cache_file_globals.header.debug_tag_name_offsets)& debug_tag_name_offsets = g_cache_file_globals.header.debug_tag_name_offsets;

	// debug_tag_names
	decltype(g_cache_file_debug_globals->debug_tag_name_offsets)& offsets = g_cache_file_debug_globals->debug_tag_name_offsets;
	constexpr dword offsets_size = sizeof(g_cache_file_debug_globals->debug_tag_name_offsets);
	decltype(g_cache_file_debug_globals->debug_tag_name_buffer)& buffer = g_cache_file_debug_globals->debug_tag_name_buffer;
	constexpr dword buffer_size = sizeof(g_cache_file_debug_globals->debug_tag_name_buffer);
	decltype(g_cache_file_debug_globals->debug_tag_names)& storage = g_cache_file_debug_globals->debug_tag_names;
	constexpr dword storage_size = sizeof(g_cache_file_debug_globals->debug_tag_names);

	//if ((g_cache_file_globals.header.shared_file_flags & 0x3F) == 0) // `shared_file_flags` is 0x3E
	//if (!TEST_BIT(g_cache_file_globals.header.shared_file_flags, 0))
	if (debug_tag_name_count)
	{
		if (!cache_file_blocking_read(_cache_file_section_debug, debug_tag_name_buffer, cache_file_round_up_read_size(debug_tag_name_buffer_length), buffer))
			return false;

		if (!cache_file_blocking_read(_cache_file_section_debug, debug_tag_name_offsets, cache_file_round_up_read_size(debug_tag_name_count * sizeof(long)), offsets))
			return false;
	}
	else
	{
		s_file_reference tag_list_file;
		file_reference_create_from_path(&tag_list_file, k_cache_tag_list_file, false);
		if (!file_exists(&tag_list_file))
		{
			c_console::write_line("cache: load tag names, '%s' file doesn't exist", k_cache_tag_list_file);
			return true;
		}

		dword tag_list_size = 0;
		file_get_size(&tag_list_file, &tag_list_size);
		if (!file_read_into_buffer(&tag_list_file, buffer, buffer_size))
		{
			c_console::write_line("cache: load tag names, '%s' file read failed", k_cache_tag_list_file);
			return false;
		}

		// edge case if the file does not end with a newline
		char* tag_list_end = buffer + tag_list_size;
		if (*(tag_list_end - 1) != '\n' || *(tag_list_end - 1) != '\r')
		{
			*(tag_list_end + 0) = '\r';
			*(tag_list_end + 1) = '\n';
		}

		char* line = buffer;
		char* line_end = 0;
		debug_tag_name_count = 0;
		for (char* position = strchr(buffer, ','); position; position = strchr(line_end + 1, ','))
		{
			char* comma_pos = position + 1;
			if (char* nl = strchr(comma_pos, '\n'))
			{
				*nl = 0;
				nl++;
				line_end = nl;
			}

			if (char* cr = strchr(comma_pos, '\r'))
			{
				*cr = 0;
				cr++;
				line_end = cr;
			}

			long debug_tag_name_index = NONE;
			if (sscanf_s(line, "0x%X,", &debug_tag_name_index))
			{
				while (debug_tag_name_count < debug_tag_name_index)
					debug_tag_name_count++;
			}

			offsets[debug_tag_name_count++] = comma_pos - (char*)offsets - offsets_size;

			if ((line_end + 1 - (char*)offsets - offsets_size) >= tag_list_size)
				break;

			line = line_end + 1;
		}
	}

	csmemset(storage, 0, storage_size);

	for (long tag_names_index = 0; tag_names_index < NUMBEROF(offsets); ++tag_names_index)
	{
		if (offsets[tag_names_index] >= buffer_size)
			break;

		if (offsets[tag_names_index] != 0)
			storage[tag_names_index] = &buffer[offsets[tag_names_index]];
	}

	return true;
}

bool __cdecl cache_file_tags_load_allocate()
{
	//return INVOKE(0x00502B40, cache_file_tags_load_allocate);

	bool result = true;
	long tag_offsets_size = 0;

	//if (!TEST_BIT(g_cache_file_globals.header.shared_file_flags, 1))
	if (g_cache_file_globals.header.tag_count)
	{
		tag_offsets_size = sizeof(long) * g_cache_file_globals.header.tag_count;
		g_cache_file_globals.tag_total_count = g_cache_file_globals.header.tag_count;
		g_cache_file_globals.tag_cache_offsets = (long*)_physical_memory_malloc_fixed(_memory_stage_level_initialize, NULL, cache_file_round_up_read_size(tag_offsets_size), 0);

		result = cache_file_blocking_read(_cache_file_section_tag, g_cache_file_globals.header.tag_cache_offsets, cache_file_round_up_read_size(tag_offsets_size), g_cache_file_globals.tag_cache_offsets);
	}
	else
	{
		s_cache_file_section_header tags_header{};
		if (tags_section)
		{
			tags_header = *static_cast<s_cache_file_section_header*>(tags_section);
		}
		else
		{
			if (!file_read(&g_cache_file_globals.tags_section, sizeof(s_cache_file_section_header), false, &tags_header))
				return false;
		}

		tag_offsets_size = sizeof(long) * tags_header.file_count;
		g_cache_file_globals.tag_total_count = tags_header.file_count;
		g_cache_file_globals.tag_cache_offsets = (long*)_physical_memory_malloc_fixed(_memory_stage_level_initialize, NULL, tag_offsets_size, 0);
		long* tag_offsets = (long*)_physical_memory_malloc_fixed(_memory_stage_level_initialize, NULL, tag_offsets_size, 0);

		if (!cache_file_tags_section_read(tags_header.file_offsets, tag_offsets_size, tag_offsets))
			return false;

		long tag_offset = 0;
		for (long tag_index = 0; tag_index < tags_header.file_count; g_cache_file_globals.tag_cache_offsets[tag_index - 1] = tag_offset)
			tag_offset = tag_offsets[tag_index++];

		physical_memory_free(tag_offsets);
	}

	g_cache_file_globals.tag_index_absolute_mapping = (long*)_physical_memory_malloc_fixed(_memory_stage_level_initialize, NULL, tag_offsets_size, 0);
	g_cache_file_globals.absolute_index_tag_mapping = (long*)_physical_memory_malloc_fixed(_memory_stage_level_initialize, NULL, tag_offsets_size, 0);
	csmemset(g_cache_file_globals.tag_index_absolute_mapping, NONE, tag_offsets_size);
	csmemset(g_cache_file_globals.absolute_index_tag_mapping, NONE, tag_offsets_size);

	return result;
}
HOOK_DECLARE_CALL(0x00502E9B, cache_file_tags_load_allocate);

bool __cdecl cache_file_tags_section_read(long offset, long size, void* buffer)
{
	//return INVOKE(0x00502C90, cache_file_tags_section_read, offset, size, buffer);

	if (TEST_BIT(g_cache_file_globals.header.shared_file_flags, 1))
	{
		if (tags_section)
		{
			csmemcpy(buffer, static_cast<byte*>(tags_section) + offset, size);
			return true;
		}

		bool result = file_set_position(&g_cache_file_globals.tags_section, offset, 0);
		if (result)
			return file_read(&g_cache_file_globals.tags_section, size, false, buffer);

		return result;
	}

	return cache_file_blocking_read(_cache_file_section_tag, offset, size, buffer);
}

//#define EXPERIMENTAL_TAG_CACHE_ALLOCATION

void __cdecl cache_file_tags_unload()
{
	//INVOKE(0x00502CE0, cache_file_tags_unload);

	if (g_cache_file_globals.tag_cache_base_address)
	{
#if defined(EXPERIMENTAL_TAG_CACHE_ALLOCATION)
		physical_memory_system_free(g_cache_file_globals.tag_cache_base_address);
#else
		physical_memory_free(g_cache_file_globals.tag_cache_base_address);
#endif

		g_cache_file_globals.tag_cache_base_address = NULL;
		g_cache_file_globals.tag_loaded_size = 0;
	}

	if (g_cache_file_globals.tag_instances)
	{
		physical_memory_free(g_cache_file_globals.tag_instances);
		g_cache_file_globals.tag_instances = NULL;
	}

	if (g_cache_file_globals.tag_cache_offsets)
	{
		physical_memory_free(g_cache_file_globals.tag_cache_offsets);
		g_cache_file_globals.tag_cache_offsets = NULL;
	}

	if (g_cache_file_globals.tag_index_absolute_mapping)
	{
		physical_memory_free(g_cache_file_globals.tag_index_absolute_mapping);
		g_cache_file_globals.tag_index_absolute_mapping = NULL;
	}

	if (g_cache_file_globals.absolute_index_tag_mapping)
	{
		physical_memory_free(g_cache_file_globals.absolute_index_tag_mapping);
		g_cache_file_globals.absolute_index_tag_mapping = NULL;
	}

	if (g_cache_file_globals.resource_gestalt)
	{
		physical_memory_free(g_cache_file_globals.resource_gestalt);
		g_cache_file_globals.resource_gestalt = NULL;
	}

	if (g_cache_file_globals.reports.elements)
	{
		physical_memory_free(g_cache_file_globals.reports.elements);
		g_cache_file_globals.reports.elements = NULL;
		g_cache_file_globals.reports.count = 0;
	}
}

void load_external_files();
bool __cdecl scenario_tags_load(char const* scenario_path)
{
	//bool result = INVOKE(0x00502DC0, scenario_tags_load, scenario_path);
	//ASSERT(cache_file_debug_tag_names_load());
	//
	//load_external_files();
	//
	//return result;

	long tag_index = NONE;
	bool success = false;

	generate_event(_event_level_message, "cache: scenario load tags, name=%s", scenario_path);

	cache_file_invalidate_signature();

	if (cache_file_open(scenario_path, &g_cache_file_globals.header) && cache_file_header_verify_and_version(&g_cache_file_globals.header, scenario_path, false))
	{
		cache_files_update_main_status();

		s_cache_file_header header_copy{};
		csmemcpy(&header_copy, &g_cache_file_globals.header, sizeof(s_cache_file_header));
		loading_basic_progress_phase_begin(1, 1);

		{
			c_static_string<260> tags_filepath = {};

			// `scenario_path` usually consists of map file without the `.map` extension
			tags_filepath.print("%s.dat", scenario_path);

			s_file_reference tags_file{};
			file_reference_create_from_path(&tags_file, tags_filepath.get_string(), false);

			g_cache_file_globals.tags_section = {};
			if (file_exists(&tags_file))
				g_cache_file_globals.tags_section = tags_file;
		}

		cache_file_load_tags_section();

		// only needed if `sub_503470` is called in `cache_file_tags_load_recursive`
		cache_file_load_reports(&g_cache_file_globals.reports, &g_cache_file_globals.header);

		cache_file_tags_load_allocate();

		dword total_instance_size = sizeof(cache_file_tag_instance*) * g_cache_file_globals.tag_total_count;
		g_cache_file_globals.tag_instances = (cache_file_tag_instance**)_physical_memory_malloc_fixed(_memory_stage_level_initialize, NULL, total_instance_size, 0);
		csmemset(g_cache_file_globals.tag_instances, 0, total_instance_size);

		g_cache_file_globals.tag_loaded_count = 0;

#if defined(EXPERIMENTAL_TAG_CACHE_ALLOCATION)
		//g_cache_file_globals.tag_cache_size = g_cache_file_globals.header.total_tags_size;
		g_cache_file_globals.tag_cache_size = 0x6400000; // 100 * 1024 * 1024 or 100MiB
		g_cache_file_globals.tag_cache_base_address = (byte*)physical_memory_system_malloc(g_cache_file_globals.tag_cache_size, NULL);
#else
		g_cache_file_globals.tag_cache_size = 0x4B00000; //  75 * 1024 * 1024 or 75MiB
		g_cache_file_globals.tag_cache_base_address = (byte*)_physical_memory_malloc_fixed(_memory_stage_level_initialize, "tag cache", g_cache_file_globals.tag_cache_size, 0);
#endif

		g_cache_file_globals.tag_loaded_size = 0;

		success = g_cache_file_globals.tag_cache_base_address != NULL;
		if (!success)
		{
			generate_event(_event_level_critical, "failed to allocate the physical memory for the tags");
		}

		if (success)
			success = cache_file_debug_tag_names_load();

		if (!success)
		{
			generate_event(_event_level_critical, "failed to load debug tag names");
		}

		if (bool cache_file_global_tags_loaded = cache_file_tags_load_recursive(0))
		{
			success = true;
			cache_file_tags_single_tag_instance_fixup(g_cache_file_globals.tag_instances[0]);

			// if no global snenario reference was found in cache file global tags we fallback to the cache file header
			if (cache_file_get_global_tag_index(SCENARIO_TAG) == NONE)
				success = cache_file_tags_load_recursive(g_cache_file_globals.header.scenario_index);
		}

		cache_file_close_tags_section();
		loading_basic_progress_phase_end();

		if (!success)
		{
			global_preferences_invalidate_maps();
			generate_event(_event_level_critical, "failed to read the tag data section");
		}

		// #TODO: security stuff
		//	calculate hash
		//	calculate hash signature
		//	compare hash signatures
		s_cache_file_security_globals* security_globals = cache_file_get_security_globals();

		//if (success)
		//{
		//	main_loop_pregame();
		//
		//	security_globals->valid_content_signature = false;
		//
		//	csmemset(&security_globals->hashes[0], 0xBB, sizeof(s_network_http_request_hash));
		//	csmemset(&security_globals->hash, 0xCC, sizeof(s_network_http_request_hash));
		//	csmemset(&security_globals->rsa_signature, 0xDD, sizeof(s_rsa_signature));
		//
		//	s_cache_file_security_globals* v6 = security_globals;
		//	v6->header = header_copy;
		//	cache_file_builder_security_clean_header(&v6->header);
		//
		//	v6->hash_sizes[0] = sizeof(s_cache_file_header);
		//	v6->hash_addresses[0] = v6;
		//
		//	long v7 = sizeof(s_cache_file_header);
		//	decltype(v6->state)* hash_state = &v6->state;
		//	if (security_incremental_hash_begin(&v6->state, sizeof(*hash_state), 1))
		//	{
		//		do
		//		{
		//			long v8 = v7;
		//			if (v7 > 0x100000)
		//				v8 = 0x100000;
		//
		//			security_incremental_hash_update(hash_state, sizeof(*hash_state), v6, v8);
		//			main_loop_pregame();
		//
		//			v7 -= v8;
		//			v6 = (s_cache_file_security_globals*)offset_pointer(v6, v8);
		//
		//		} while (v7 > 0);
		//		security_incremental_hash_finish(hash_state, sizeof(*hash_state), &v6->hashes[0]);
		//	}
		//
		//	security_calculate_hash(&security_globals->hashes, sizeof(s_network_http_request_hash), 1, &security_globals->hash);
		//	security_globals->rsa_signature = g_cache_file_globals.header.rsa_signature;
		//
		//	main_loop_pregame();
		//
		//	success = security_rsa_compute_and_verify_signature(&security_globals->hash, &security_globals->rsa_signature);
		//}

		if (success)
		{
			//success = string_id_load_strings(&g_cache_file_globals.header);
			if (!success)
			{
				generate_event(_event_level_error, "networking:failed to load the string ids [%s]", scenario_path);
			}
		}

		if (success)
		{
			security_globals->valid_content_signature = true;

			cache_file_tags_fixup_all_instances();

			tag_index = cache_file_get_global_tag_index(SCENARIO_TAG);

			// if no global scenario reference was found in cache file global tags we fallback to the cache file header
			if (tag_index == NONE)
				tag_index = g_cache_file_globals.header.scenario_index;

			g_cache_file_globals.tags_loaded = true;

			g_tag_total_count_pre_external_files = g_cache_file_globals.tag_total_count;
			load_external_files();
		}
	}

	if (!success)
	{
		generate_event(_event_level_critical, "failed to load tags for cache file");
		cache_file_tags_unload();
		cache_file_close();
		ASSERT(tag_index == NONE);
	}

	global_scenario_index = tag_index;
	if (tag_index != NONE)
	{
		global_scenario_game_globals_index = cache_file_get_global_tag_index(GLOBALS_TAG);
		global_scenario = static_cast<s_scenario*>(tag_get(SCENARIO_TAG, global_scenario_index));
		global_game_globals = static_cast<s_game_globals*>(tag_get(GLOBALS_TAG, global_scenario_game_globals_index));

		c_rasterizer_globals* rasterizer_globals = global_game_globals->rasterizer_globals_ref.cast_to<c_rasterizer_globals>();
		if (rasterizer_globals)
		{
			c_rasterizer::g_max_vs_gprs = rasterizer_globals->get_max_vs_gprs();
			c_rasterizer::g_max_ps_gprs = rasterizer_globals->get_max_ps_gprs();
		}

		success = true;
	}

	generate_event(_event_level_message, "cache: scenario load tags, success=%s", success ? "true" : "false");

	return success;
}

void __cdecl scenario_tags_load_finished()
{
	// nullsub
	INVOKE(0x00503190, scenario_tags_load_finished);

	for (long i = 0; i < g_cache_file_globals.tag_loaded_count; i++)
	{
		long tag_index = g_cache_file_globals.absolute_index_tag_mapping[i];

		cache_file_tag_instance* instance = g_cache_file_globals.tag_instances[i];
		tag_instance_modification_apply(instance, _instance_modification_stage_after_scenario_tags_loaded);
	}
}

void __cdecl cache_file_tags_fixup_all_instances()
{
	//INVOKE(0x005031A0, cache_file_tags_fixup_all_instances);

	for (long i = 0; i < g_cache_file_globals.tag_loaded_count; i++)
	{
		long tag_index = g_cache_file_globals.absolute_index_tag_mapping[i];

		cache_file_tag_instance* instance = g_cache_file_globals.tag_instances[i];
		cache_file_tags_single_tag_instance_fixup(instance);
	}
}

void __cdecl scenario_tags_unload()
{
	//INVOKE(0x00503200, scenario_tags_unload);

	cache_file_close();
	cache_file_invalidate_signature();
	g_cache_file_globals.tags_loaded = 0;
	cache_file_tags_unload();
	csmemset(&g_cache_file_globals.header, 0, sizeof(g_cache_file_globals.header));
}

void cache_file_transform_creator_string(c_wrapped_array<char> in_out_creator_string)
{
	if (in_out_creator_string[0])
	{
		ASSERT(NUMBEROF(g_cache_file_creator_key) >= in_out_creator_string.count());

		for (long i = 0; i < in_out_creator_string.count(); i++)
			in_out_creator_string[i] ^= g_cache_file_creator_key[i];
	}
}

void cache_files_update_main_status()
{
	c_static_string<32> author = g_cache_file_globals.header.author;
	c_wrapped_array<char> creator_string = c_wrapped_array<char>(author.get());
	cache_file_transform_creator_string(creator_string);
	author.null_terminate_buffer();
	main_status("map created by", "%s", author.get_string());
	c_console::write_line("map created by %s", author.get_string());
}

void __cdecl tag_files_close()
{
	string_id_dispose();

	INVOKE(0x00503300, tag_files_close);

	//cache_file_tag_resources_dispose();
	//physical_memory_free(g_cache_file_globals.debug_tag_names);
	//g_cache_file_globals.debug_tag_names = NULL;
	//optional_cache_unregister_user((e_optional_cache_user)0, &g_cache_file_copy_optional_cache_callback);

	//tag build
	//tag_groups_dispose();
	//ASSERT(g_tag_file_globals.open);
	//g_tag_file_globals.some_count++;
	//ASSERT(g_tag_file_globals.open);
	//ASSERT(g_tag_file_globals.shim);
	//g_tag_file_globals.shim->tag_files_new_sync_initialize();
	//g_tag_file_globals.shim->tag_files_new_single_file_close();
}

void __cdecl tag_files_open()
{
	//INVOKE(0x00503340, tag_files_open);

	cache_files_initialize();

	ASSERT(g_cache_file_debug_globals == NULL);
	g_cache_file_debug_globals = (s_cache_file_debug_globals*)_physical_memory_malloc_fixed(_memory_stage_game_initialize, "cache file debug globals", sizeof(s_cache_file_debug_globals), 1);

	string_id_initialize();

	cache_file_tag_resources_initialize();
}

void* __cdecl tag_get(tag group_tag, long tag_index)
{
	g_last_tag_accessed = { .group_tag = group_tag, .index = tag_index };

	long tag_absolute_index = g_cache_file_globals.tag_index_absolute_mapping[tag_index];
	if (tag_absolute_index == NONE)
		return nullptr;

	void* data = g_cache_file_globals.tag_instances[tag_absolute_index]->base + g_cache_file_globals.tag_instances[tag_absolute_index]->offset;

	return data;
}

void* __cdecl tag_get(tag group_tag, char const* tag_name)
{
	tag_iterator iterator{};
	tag_iterator_new(&iterator, group_tag);
	for (long tag_index = tag_iterator_next(&iterator); tag_index != NONE; tag_index = tag_iterator_next(&iterator))
	{
		char const* _tag_name = tag_get_name_safe(tag_index);
		if (_tag_name && csstricmp(_tag_name, tag_name) == 0)
			return tag_get(group_tag, tag_index);
	}

	return nullptr;
}

dword __cdecl tag_get_group_tag(long tag_index)
{
	return INVOKE(0x005033A0, tag_get_group_tag, tag_index);
}

void __cdecl tag_iterator_new(tag_iterator* iterator, tag group_tag)
{
	INVOKE(0x005033E0, tag_iterator_new, iterator, group_tag);
}

long __cdecl tag_iterator_next(tag_iterator* iterator)
{
	return INVOKE(0x00503400, tag_iterator_next, iterator);
}

// __thiscall
void __fastcall sub_503470(s_cache_file_reports* reports, void* unused, cache_file_tag_instance* instance, long tag_index)
{
	ASSERT(instance);

	if (instance->dependency_count || instance->data_fixup_count || instance->resource_fixup_count)
		return;

	c_console::write_line("0x%08X.%s", tag_index, instance->tag_group.name.get_string());
	static char tag_instance_byte_string[sizeof(cache_file_tag_instance) * 3]{};
	type_as_byte_string(instance, tag_instance_byte_string);
	c_console::write_line(tag_instance_byte_string);
}

bool cache_file_tags_single_tag_file_load(s_file_reference* file, long* out_tag_index, cache_file_tag_instance** out_instance)
{
	cache_file_tag_instance* instance = reinterpret_cast<cache_file_tag_instance*>(g_cache_file_globals.tag_cache_base_address + g_cache_file_globals.tag_loaded_size);
	long& tag_loaded_count = g_cache_file_globals.tag_loaded_count;

	if (out_instance)
		*out_instance = instance;

	if (!file_read_from_position(file, 0, sizeof(cache_file_tag_instance), false, instance))
		return false;

	dword file_size = 0;
	file_get_size(file, &file_size);

	g_cache_file_globals.tag_loaded_size += file_size;
	g_cache_file_globals.tag_instances[tag_loaded_count] = instance;
	g_cache_file_globals.tag_index_absolute_mapping[g_cache_file_globals.tag_total_count] = tag_loaded_count;
	g_cache_file_globals.absolute_index_tag_mapping[tag_loaded_count] = g_cache_file_globals.tag_total_count;

	if (*out_tag_index)
		*out_tag_index = g_cache_file_globals.tag_total_count++;

	if (!file_read_from_position(file, 0, file_size, false, instance))
		return false;

	if (crc_checksum_buffer_adler32(adler_new(), instance->base + sizeof(instance->checksum), instance->total_size - sizeof(instance->checksum)) != instance->checksum)
		return false;

	tag_loaded_count++;
	g_cache_file_globals.tag_total_count++;

	if (instance->dependency_count <= 0)
		return true;

	short dependency_index = 0;
	while (cache_file_tags_load_recursive(instance->dependencies[dependency_index]))
	{
		if (++dependency_index >= instance->dependency_count)
			return true;
	}

	if (out_instance)
		*out_instance = nullptr;

	return false;
}

void cache_file_tags_load_single_tag_file_test(char const* file_name)
{
	s_file_reference file;
	dword error = 0;

	file_reference_create_from_path(&file, "tags\\", false);
	file.path.append(file_name);
	if (file_open(&file, FLAG(_file_open_flag_desired_access_read), &error)/* && error == 0*/)
	{
		long tag_index = NONE;
		cache_file_tag_instance* instance = nullptr;
		if (cache_file_tags_single_tag_file_load(&file, &tag_index, &instance) && tag_index != NONE && instance)
		{
			cache_file_tags_single_tag_instance_fixup(instance);

			dword file_size = 0;
			file_get_size(&file, &file_size);

			if (instance->tag_group == BITMAP_TAG)
			{
				bitmap_group* bitmap_instance = instance->cast_to<bitmap_group>();
				ASSERT(bitmap_instance->bitmaps.count() == bitmap_instance->hardware_textures.count());

				bitmap_data& bitmap = bitmap_instance->bitmaps[0];
				s_cache_file_tag_resource_data* resource_data = bitmap_instance->hardware_textures[0].get();

				if (!resource_data->runtime_data.control_data.base)
					return;

				s_render_texture_descriptor& texture_descriptor = resource_data->runtime_data.control_data.get()->render_texture;

				// disable any checksums
				resource_data->file_location.flags.set(_cache_file_tag_resource_location_flags_valid_checksum, false);
				resource_data->file_location.checksum = 0;

				// set the sizes to tag index and resource offset
				resource_data->file_location.file_size = tag_index;
				resource_data->file_location.size = file_size - instance->total_size;

				// set the codec index to that of our custom decompressor
				resource_data->file_location.codec_index = _cache_file_compression_codec_runtime_tag_resource;

				// update the owner tag index
				resource_data->runtime_data.owner_tag.index = tag_index;

				texture_descriptor.base_pixel_data.size = bitmap.pixels_size;
			}
		}
	}
	file_close(&file);
}

// #TODO: create some sort of tag modification manager
void apply_globals_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != GLOBALS_TAG)
		return;

	s_game_globals* game_globals = instance->cast_to<s_game_globals>();
	char const* tag_name = instance->get_name();
	char const* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_tag_load:
	{
		game_globals->input_globals.index = NONE;
	}
	break;
	case _instance_modification_stage_tag_fixup:
	{
		ASSERT(game_globals->input_globals.index == NONE);

		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(game_globals);
	}
	break;
	case _instance_modification_stage_after_scenario_tags_loaded:
	{
	}
	break;
	}
}

// #TODO: create some sort of tag modification manager
void apply_multiplayer_globals_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != MULTIPLAYER_GLOBALS_TAG)
		return;

	//// if the very first offset is not 0x20 there is a very high likelihood that this is an ElDewrito tag set
	//bool is_base_cache = g_cache_file_globals.tag_cache_offsets[0] == 0x20;

	s_multiplayer_globals_definition* multiplayer_globals = instance->cast_to<s_multiplayer_globals_definition>();
	char const* tag_name = instance->get_name();
	char const* group_tag_name = instance->tag_group.name.get_string();

	// Add back missing weapon selections
	switch (stage)
	{
	case _instance_modification_stage_tag_load:
	{
		//if (is_base_cache)
		//{
		//	// load weapons
		//	cache_file_tags_load_recursive(0x00001500); // objects\weapons\rifle\spike_rifle\spike_rifle
		//	cache_file_tags_load_recursive(0x0000159E); // objects\weapons\melee\energy_blade\energy_blade
		//	cache_file_tags_load_recursive(0x000014F8); // objects\weapons\pistol\needler\needler
		//	cache_file_tags_load_recursive(0x000015B3); // objects\weapons\support_high\rocket_launcher\rocket_launcher
		//	cache_file_tags_load_recursive(0x00001A45); // objects\weapons\rifle\shotgun\shotgun
		//	cache_file_tags_load_recursive(0x000015B1); // objects\weapons\rifle\sniper_rifle\sniper_rifle
		//	cache_file_tags_load_recursive(0x000014FF); // objects\weapons\support_low\brute_shot\brute_shot
		//	cache_file_tags_load_recursive(0x00001509); // objects\weapons\rifle\beam_rifle\beam_rifle
		//	cache_file_tags_load_recursive(0x000015B2); // objects\weapons\support_high\spartan_laser\spartan_laser
		//	cache_file_tags_load_recursive(0x0000150C); // objects\weapons\melee\gravity_hammer\gravity_hammer
		//	cache_file_tags_load_recursive(0x00001A55); // objects\weapons\turret\flamethrower\flamethrower
		//	cache_file_tags_load_recursive(0x00001A54); // objects\weapons\turret\missile_pod\missile_pod
		//
		//	// load bipeds
		//	cache_file_tags_load_recursive(0x000027D6); // objects\characters\odst\odst
		//	cache_file_tags_load_recursive(0x000027D7); // objects\characters\marine\marine
		//
		//	// load vehicles
		//	cache_file_tags_load_recursive(0x00001599); // objects\vehicles\warthog\warthog_snow
		//
		//	if (c_map_variant const* map_variant = network_squad_session_get_map_variant()) for (s_variant_quota const& quota : map_variant->m_quotas)
		//	{
		//		if (quota.object_definition_index == NONE)
		//			continue;
		//
		//		cache_file_tags_load_recursive(quota.object_definition_index);
		//	}
		//}
	}
	break;
	case _instance_modification_stage_tag_fixup:
	{
		//if (is_base_cache)
		//{
		//	if (multiplayer_globals == nullptr || multiplayer_globals->universal.count() <= 0 || multiplayer_globals->universal[0].weapon_selections.count() <= 0)
		//		return;
		//
		//	for (s_multiplayer_weapon_selection& weapon_selection : multiplayer_globals->universal[0].weapon_selections)
		//	{
		//		if (weapon_selection.weapon_tag.index != NONE)
		//			continue;
		//
		//		switch (weapon_selection.name.get_value())
		//		{
		//		case STRING_ID(global, spike_rifle):
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\rifle\\spike_rifle\\spike_rifle");
		//			break;
		//		case STRING_ID(global, sword):
		//		case STRING_ID(global, energy_blade):
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\melee\\energy_blade\\energy_blade");
		//			break;
		//		case STRING_ID(global, needler):
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\pistol\\needler\\needler");
		//			break;
		//		case STRING_ID(global, rocket_launcher):
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\support_high\\rocket_launcher\\rocket_launcher");
		//			break;
		//		case STRING_ID(global, shotgun):
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\rifle\\shotgun\\shotgun");
		//			break;
		//		case STRING_ID(global, sniper_rifle):
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\rifle\\sniper_rifle\\sniper_rifle");
		//			break;
		//		case STRING_ID(global, brute_shot):
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\support_low\\brute_shot\\brute_shot");
		//			break;
		//		case STRING_ID(global, beam_rifle):
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\rifle\\beam_rifle\\beam_rifle");
		//			break;
		//		case STRING_ID(global, spartan_laser):
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\support_high\\spartan_laser\\spartan_laser");
		//			break;
		//		case STRING_ID(global, gravity_hammer):
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\melee\\gravity_hammer\\gravity_hammer");
		//			break;
		//		case STRING_ID(global, flame_thrower):
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\turret\\flamethrower\\flamethrower");
		//			break;
		//		case STRING_ID(global, missile_launcher):
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\turret\\missile_pod\\missile_pod");
		//			break;
		//		}
		//	}
		//}

		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(multiplayer_globals);
	}
	break;
	case _instance_modification_stage_after_scenario_tags_loaded:
	{
	}
	break;
	}
}

// #TODO: create some sort of tag modification manager
void apply_rasterizer_globals_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != RASTERIZER_GLOBALS_TAG)
		return;

	c_rasterizer_globals* rasterizer_globals = instance->cast_to<c_rasterizer_globals>();
	char const* tag_name = instance->get_name();
	char const* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_tag_load:
	{
	}
	break;
	case _instance_modification_stage_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(rasterizer_globals);
	}
	break;
	case _instance_modification_stage_after_scenario_tags_loaded:
	{
	}
	break;
	}
}

// #TODO: create some sort of tag modification manager
void apply_scenario_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != SCENARIO_TAG)
		return;

	s_scenario* scenario = instance->cast_to<s_scenario>();
	char const* tag_name = instance->get_name();
	char const* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_tag_load:
	{
	}
	break;
	case _instance_modification_stage_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(scenario);
	}
	break;
	case _instance_modification_stage_after_scenario_tags_loaded:
	{
	}
	break;
	}
}

// #TODO: create some sort of tag modification manager
void apply_chud_globals_definition_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != CHUD_GLOBALS_DEFINITION_TAG)
		return;

	s_chud_globals_definition* chud_globals_definition = instance->cast_to<s_chud_globals_definition>();
	char const* tag_name = instance->get_name();
	char const* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_tag_load:
	{
	}
	break;
	case _instance_modification_stage_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(chud_globals_definition);
	}
	break;
	case _instance_modification_stage_after_scenario_tags_loaded:
	{
		if (chud_globals_definition->sprint_fov_multiplier == 0.0f)
			chud_globals_definition->sprint_fov_multiplier = 1.0f;
	}
	break;
	}
}

// #TODO: create some sort of tag modification manager
void apply_vision_mode_definition_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != VISION_MODE_TAG)
		return;

	s_vision_mode_definition* vision_mode_definition = instance->cast_to<s_vision_mode_definition>();
	char const* tag_name = instance->get_name();
	char const* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_tag_load:
	{
	}
	break;
	case _instance_modification_stage_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(vision_mode_definition);
	}
	break;
	case _instance_modification_stage_after_scenario_tags_loaded:
	{
	}
	break;
	}
}

// #TODO: create some sort of tag modification manager
void apply_object_definition_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != OBJECT_TAG)
		return;

	struct object_definition* object_definition = instance->cast_to<struct object_definition>();
	char const* tag_name = instance->get_name();
	char const* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_tag_load:
	{
	}
	break;
	case _instance_modification_stage_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(object_definition);
	}
	break;
	case _instance_modification_stage_after_scenario_tags_loaded:
	{
	}
	break;
	}
}

// #TODO: create some sort of tag modification manager
void apply_unit_definition_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != UNIT_TAG)
		return;

	struct unit_definition* unit_definition = instance->cast_to<struct unit_definition>();
	char const* tag_name = instance->get_name();
	char const* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_tag_load:
	{
	}
	break;
	case _instance_modification_stage_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(unit_definition);
	}
	break;
	case _instance_modification_stage_after_scenario_tags_loaded:
	{
	}
	break;
	}
}

// #TODO: create some sort of tag modification manager
void apply_biped_definition_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != BIPED_TAG)
		return;

	struct biped_definition* biped_definition = instance->cast_to<struct biped_definition>();
	char const* tag_name = instance->get_name();
	char const* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_tag_load:
	{
	}
	break;
	case _instance_modification_stage_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(biped_definition);
	}
	break;
	case _instance_modification_stage_after_scenario_tags_loaded:
	{
		// "edge drop" fix
		biped_definition->biped.physics.ground_physics.scale_ground_adhesion_velocity = 30.0f / 60;

		//void __cdecl biped_initialize_character_physics_update_input(long, s_character_physics_update_input_datum* physics_input, bool, bool, real, bool, bool)
		//{
		//	if (biped_definition->biped.physics.ground_physics.scale_ground_adhesion_velocity > 0.0f)
		//		physics_input->m_ground_adhesion_velocity_scale = biped_definition->biped.physics.ground_physics.scale_ground_adhesion_velocity;
		//}
	}
	break;
	}
}

// #TODO: create some sort of tag modification manager
void apply_vehicle_definition_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != VEHICLE_TAG)
		return;

	struct vehicle_definition* vehicle_definition = instance->cast_to<struct vehicle_definition>();
	char const* tag_name = instance->get_name();
	char const* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_tag_load:
	{
	}
	break;
	case _instance_modification_stage_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(vehicle_definition);
	}
	break;
	case _instance_modification_stage_after_scenario_tags_loaded:
	{
	}
	break;
	}
}

// #TODO: create some sort of tag modification manager
void apply_item_definition_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != ITEM_TAG)
		return;

	struct item_definition* item_definition = instance->cast_to<struct item_definition>();
	char const* tag_name = instance->get_name();
	char const* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_tag_load:
	{
	}
	break;
	case _instance_modification_stage_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(item_definition);
	}
	break;
	case _instance_modification_stage_after_scenario_tags_loaded:
	{
	}
	break;
	}
}

// #TODO: create some sort of tag modification manager
void apply_equipment_definition_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != EQUIPMENT_TAG)
		return;

	struct equipment_definition* equipment_definition = instance->cast_to<struct equipment_definition>();
	char const* tag_name = instance->get_name();
	char const* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_tag_load:
	{
	}
	break;
	case _instance_modification_stage_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(equipment_definition);
	}
	break;
	case _instance_modification_stage_after_scenario_tags_loaded:
	{
	}
	break;
	}
}

// #TODO: create some sort of tag modification manager
void apply_projectile_definition_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != PROJECTILE_TAG)
		return;

	struct projectile_definition* projectile_definition = instance->cast_to<struct projectile_definition>();
	char const* tag_name = instance->get_name();
	char const* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_tag_load:
	{
	}
	break;
	case _instance_modification_stage_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(projectile_definition);
	}
	break;
	case _instance_modification_stage_after_scenario_tags_loaded:
	{
	}
	break;
	}
}

// #TODO: create some sort of tag modification manager
void tag_instance_modification_apply(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	if (instance == nullptr)
		return;

#define APPLY_INSTANCE_MODIFICATION(GROUP_NAME) apply_##GROUP_NAME##_instance_modification(instance, stage)

	APPLY_INSTANCE_MODIFICATION(globals);
	APPLY_INSTANCE_MODIFICATION(multiplayer_globals);
	APPLY_INSTANCE_MODIFICATION(rasterizer_globals);
	APPLY_INSTANCE_MODIFICATION(scenario);
	APPLY_INSTANCE_MODIFICATION(chud_globals_definition);
	APPLY_INSTANCE_MODIFICATION(vision_mode_definition);
	APPLY_INSTANCE_MODIFICATION(object_definition);
	APPLY_INSTANCE_MODIFICATION(unit_definition);
	APPLY_INSTANCE_MODIFICATION(biped_definition);
	APPLY_INSTANCE_MODIFICATION(vehicle_definition);
	APPLY_INSTANCE_MODIFICATION(item_definition);
	APPLY_INSTANCE_MODIFICATION(equipment_definition);
	APPLY_INSTANCE_MODIFICATION(projectile_definition);

#undef APPLY_INSTANCE_MODIFICATION
}

//#define ISEXPERIMENTAL

#if defined(ISEXPERIMENTAL)

void bitmap_fixup(cache_file_tag_instance* instance, s_resource_file_header const* file_header)
{
	DirectX::DDS_FILE_HEADER const* dds_file = reinterpret_cast<DirectX::DDS_FILE_HEADER const*>(file_header + 1);
	if (!dds_file)
		return;

	bitmap_group* bitmap_instance = instance->cast_to<bitmap_group>();
	ASSERT(bitmap_instance->bitmaps.count() == bitmap_instance->hardware_textures.count());

	bitmap_data& bitmap = bitmap_instance->bitmaps[file_header->resource_index];
	s_cache_file_tag_resource_data* resource_data = bitmap_instance->hardware_textures[file_header->resource_index].get();

	if (!resource_data->runtime_data.control_data.base)
		return;

	s_render_texture_descriptor& texture_descriptor = resource_data->runtime_data.control_data.get()->render_texture;

	// update the width and height to that of the dds file
	bitmap.width = static_cast<short>(dds_file->header.width);
	bitmap.height = static_cast<short>(dds_file->header.height);

	// update the pixels offset and size base on the linear size and resource index
	bitmap.pixels_offset = (dds_file->header.pitchOrLinearSize * file_header->resource_index);
	bitmap.pixels_size = dds_file->header.pitchOrLinearSize;

	// disable any checksums
	resource_data->file_location.flags.set(_cache_file_tag_resource_location_flags_valid_checksum, false);
	resource_data->file_location.checksum = 0;

	// set the sizes to tag index and resource index plus one
	resource_data->file_location.file_size = file_header->tag_index;
	resource_data->file_location.size = file_header->resource_index + 1;

	// set the codec index to that of our custom decompressor
	resource_data->file_location.codec_index = _cache_file_compression_codec_runtime_resource;

	texture_descriptor.base_pixel_data.size = bitmap.pixels_size;
	texture_descriptor.texture.width = bitmap.width;
	texture_descriptor.texture.height = bitmap.height;
}

void external_tag_fixup(s_file_reference* file, long tag_index, cache_file_tag_instance* instance)
{
	dword file_size = 0;
	file_get_size(file, &file_size);

	if (instance->tag_group == BITMAP_TAG)
	{
		bitmap_group* bitmap_instance = instance->cast_to<bitmap_group>();
		ASSERT(bitmap_instance->bitmaps.count() == bitmap_instance->hardware_textures.count());

		bitmap_data& bitmap = bitmap_instance->bitmaps[0];
		s_cache_file_tag_resource_data* resource_data = bitmap_instance->hardware_textures[0].get();

		if (!resource_data->runtime_data.control_data.base)
			return;

		s_render_texture_descriptor& texture_descriptor = resource_data->runtime_data.control_data.get()->render_texture;

		// disable any checksums
		resource_data->file_location.flags.set(_cache_file_tag_resource_location_flags_valid_checksum, false);
		resource_data->file_location.checksum = 0;

		// set the sizes to tag index and resource offset
		resource_data->file_location.file_size = tag_index;
		resource_data->file_location.size = file_size - instance->total_size;

		// set the codec index to that of our custom decompressor
		resource_data->file_location.codec_index = _cache_file_compression_codec_runtime_tag_resource;

		// update the owner tag index
		resource_data->runtime_data.owner_tag.index = tag_index;

		texture_descriptor.base_pixel_data.size = bitmap.pixels_size;
	}
}

bool check_for_specific_scenario(s_file_reference* file)
{
	char directory[256]{};
	file_reference_get_name(file, FLAG(_name_directory_bit), directory, sizeof(directory));
	csstrnzcat(directory, "\\specific_scenario.txt", sizeof(directory));

	s_file_reference specific_scenario_file{};
	file_reference_create_from_path(&specific_scenario_file, directory, false);
	if (!file_exists(&specific_scenario_file))
		return false;

	dword error = 0;
	if (!file_open(&specific_scenario_file, FLAG(_file_open_flag_desired_access_read), &error))
		return false;

	dword file_size = 0;
	if (!file_get_size(&specific_scenario_file, &file_size))
	{
		file_close(&specific_scenario_file);
		return false;
	}

	char* const file_buffer = new char[file_size + 1] {};

	if (!file_read(&specific_scenario_file, file_size, false, file_buffer))
	{
		file_close(&specific_scenario_file);
		return false;
	}
	file_close(&specific_scenario_file);

	char const* scenario_name = tag_name_strip_path(file_buffer);

	return csstricmp(g_cache_file_globals.header.name.get_string(), scenario_name) != 0;
}

bool load_external_tag(s_file_reference* file)
{
	if (!file->path.ends_with("bitmap"))
		return false;

	if (check_for_specific_scenario(file))
		return false;

	long tag_index = NONE;
	cache_file_tag_instance* instance = nullptr;
	if (!cache_file_tags_single_tag_file_load(file, &tag_index, &instance))
		return false;

	if (tag_index == NONE || !instance)
		return false;

	if (!cache_file_tags_single_tag_instance_fixup(instance))
		return false;

	external_tag_fixup(file, tag_index, instance);

	return true;
}

void external_resource_fixup(long tag_index, cache_file_tag_instance* instance)
{
	for (long i = 0; i < g_resource_file_headers.count(); i++)
	{
		s_resource_file_header const* file_header = g_resource_file_headers[i];

		if (instance->tag_group == file_header->group_tag)
		{
			if (tag_index != file_header->tag_index)
				continue;

			bitmap_fixup(instance, file_header);
		}
	}
}

bool load_external_resource(s_file_reference* file)
{
	long valid_extension = 0;
	for (long i = 0; i < k_cache_file_resource_type_count; i++)
		valid_extension += file->path.ends_with(cache_file_resource_type_get_name(i));

	if (valid_extension <= 0)
		return false;

	dword file_size = 0;
	if (!file_get_size(file, &file_size))
		return false;

	if (file_size <= sizeof(s_resource_file_header))
		return false;

	void* buffer = new byte[file_size]{};
	ASSERT(buffer != nullptr);

	if (!file_read(file, file_size, false, buffer))
	{
		delete[] buffer;
		return false;
	}

	s_resource_file_header* file_header = static_cast<s_resource_file_header*>(buffer);
	if (file_header->file_size != file_size)
	{
		delete[] buffer;
		return false;
	}

	bool add_new_resource = true;
	for (long i = 0; i < g_resource_file_headers.count(); i++)
	{
		if (csmemcmp(g_resource_file_headers[i], file_header, sizeof(s_resource_file_header)) == 0)
			add_new_resource = false;
	}

	if (!add_new_resource)
	{
		delete[] buffer;
		return false;
	}

	g_resource_file_headers[g_resource_file_headers.new_element_index()] = file_header;

	return true;
}
#endif // ISEXPERIMENTAL

void load_external_files()
{
#if defined(ISEXPERIMENTAL)

	s_file_reference search_directory{};

	// Add tags at runtime starting at 0x00004441
	file_reference_create_from_path(&search_directory, "tags\\", true);
	find_files_recursive(&search_directory, FLAG(_file_open_flag_desired_access_read), load_external_tag);

	file_reference_create_from_path(&search_directory, "data\\", true);
	find_files_recursive(&search_directory, FLAG(_file_open_flag_desired_access_read), load_external_resource);

	for (long i = 0; i < g_cache_file_globals.tag_loaded_count; i++)
	{
		long tag_index = g_cache_file_globals.absolute_index_tag_mapping[i];
		cache_file_tag_instance* instance = g_cache_file_globals.tag_instances[i];

		external_resource_fixup(tag_index, instance);
	}

#endif // ISEXPERIMENTAL
}

