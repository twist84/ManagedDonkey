#include "cache/cache_files.hpp"

#include "bitmaps/bitmap_group_tag_definition.hpp"
#include "bitmaps/bitmaps.hpp"
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
#include "interface/c_gui_screen_widget.hpp"
#include "interface/c_gui_text_widget.hpp"
#include "interface/chud/chud_globals_definitions.hpp"
#include "items/equipment_definitions.hpp"
#include "items/item_definitions.hpp"
#include "items/projectile_definitions.hpp"
#include "items/weapon_definitions.hpp"
#include "main/global_preferences.hpp"
#include "main/loading.hpp"
#include "main/main.hpp"
#include "memory/crc.hpp"
#include "memory/module.hpp"
#include "objects/object_definitions.hpp"
#include "rasterizer/rasterizer.hpp"
#include "render/camera_fx_settings.hpp"
#include "scenario/scenario.hpp"
#include "scenario/scenario_definitions.hpp"
#include "shell/shell_windows.hpp"
#include "tag_files/string_ids.hpp"
#include "text/text_group.hpp"
#include "units/biped_definitions.hpp"
#include "units/unit_definition.hpp"
#include "units/vehicle_definitions.hpp"

#include <DDS.h>
#include <malloc.h>
#include <search.h>
#include <string.h>

REFERENCE_DECLARE(0x0189CFDC, const char*, k_multiplayer_shared_scenario_tag) = "levels\\shared\\shared\\shared";
REFERENCE_DECLARE(0x0189CFE0, const char*, k_single_player_shared_scenario_tag) = "levels\\shared\\shared\\campaign";
REFERENCE_DECLARE(0x0189CFE4, const char*, k_main_menu_scenario_tag) = "levels\\ui\\mainmenu\\mainmenu";
REFERENCE_DECLARE(0x0189CFE8, const char*, k_introduction_scenario_tag) = "levels\\solo\\005_intro\\005_intro";// "levels\\atlas\\c100\\c100";
REFERENCE_DECLARE(0x0189CFEC, const char*, k_cache_strings_file) = "maps\\string_ids.dat";
REFERENCE_DECLARE(0x0189CFF0, const char*, k_cache_tags_file) = "maps\\tags.dat";
REFERENCE_DECLARE(0x0189CFF4, const char*, k_cache_tag_list_file) = "maps\\tag_list.csv";
REFERENCE_DECLARE(0x0189CFF8, const char*, k_cache_resources_file) = "maps\\resources.dat";
REFERENCE_DECLARE(0x0189CFFC, const char*, k_cache_textures_file) = "maps\\textures.dat";
REFERENCE_DECLARE(0x0189D000, const char*, k_cache_textures_b_file) = "maps\\textures_b.dat";
REFERENCE_DECLARE(0x0189D004, const char*, k_cache_audio_file) = "maps\\audio.dat";
REFERENCE_DECLARE(0x0189D008, const char*, k_cache_video_file) = "maps\\video.dat";
REFERENCE_DECLARE(0x0189D00C, const char*, k_cache_file_extension) = ".map";

void* (__cdecl* tag_get_hook)(tag group_tag, int32 tag_index) = tag_get;

REFERENCE_DECLARE(0x022AAFE8, s_cache_file_debug_globals*, g_cache_file_debug_globals);
REFERENCE_DECLARE(0x022AAFF0, s_cache_file_globals, g_cache_file_globals);

HOOK_DECLARE(0x005016D0, cache_file_blocking_read);
HOOK_DECLARE(0x00501FC0, cache_files_map_directory);
HOOK_DECLARE(0x00501FD0, cache_files_populate_resource_gestalt);
HOOK_DECLARE(0x00502210, cache_files_verify_header_rsa_signature);
HOOK_DECLARE(0x00502780, cache_file_tags_load_recursive);
HOOK_DECLARE(0x00502CE0, cache_file_tags_unload);
HOOK_DECLARE(0x005031A0, cache_file_tags_fixup_all_instances);
HOOK_DECLARE(0x00503370, tag_get_hook);
HOOK_DECLARE(0x00503470, sub_503470);

const char* k_cache_path_format = "maps\\%s.map";

uns8 const g_cache_file_creator_key[64]
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

s_tag_reference g_last_tag_accessed = { .group_tag = 0xFFFFFFFF, .index = NONE };

struct s_cache_file_tag_group_bsearch
{
public:
	static int32 __cdecl compare(tag group_tag, const s_cache_file_tag_group* group)
	{
		return group_tag - group->group_tag;
	}

	static int32 __cdecl search(tag group_tag, const s_cache_file_tag_group* group, int32 count)
	{
		ASSERT(m_sorted);

		int32 result = -1;
		const s_cache_file_tag_group* current_group = group;

		while (count)
		{
			const s_cache_file_tag_group* next_group = &current_group[count >> 1];
			int32 compare_result = compare(group_tag, next_group);

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
	static int __cdecl sort_func(const void* a, const void* b)
	{
		const s_cache_file_tag_group* group_a = static_cast<const s_cache_file_tag_group*>(a);
		const s_cache_file_tag_group* group_b = static_cast<const s_cache_file_tag_group*>(b);

		return group_a->group_tag - group_b->group_tag;
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

void* cache_file_tag_instance::get()
{
	return base + offset;
}

int32 cache_file_tag_instance::get_tag_index()
{
	int32 tag_index = *reinterpret_cast<int32*>(offset_pointer(base, total_size));
	return tag_index;
}

const char* cache_file_tag_instance::get_tag_name()
{
	int32 tag_index = cache_file_tag_instance::get_tag_index();
	const char* tag_name = tag_get_name_safe(tag_index);
	return tag_name;
}

const char* tag_group_get_name(tag group_tag)
{
	const s_cache_file_tag_group* group = nullptr;
	int32 index = s_cache_file_tag_group_bsearch::search(group_tag, global_tag_groups, global_tag_group_count);
	if (index != -1)
	{
		group = &global_tag_groups[index];
	}

	if (group)
	{
		return group->name.get_string();
	}

	return "";
}

int32 __cdecl tag_loaded(tag group_tag, const char* tag_name)
{
	if (g_cache_file_globals.tags_loaded)
	{
		//ASSERT(global_tag_instances);

		for (int32 i = 0; i < g_cache_file_globals.tag_loaded_count; i++)
		{
			cache_file_tag_instance* instance = g_cache_file_globals.tag_instances[i];

			if (instance->tag_group != group_tag)
			{
				continue;
			}

			int32 tag_index = g_cache_file_globals.absolute_index_tag_mapping[i];
			const char* name = tag_get_name(tag_index);
			if (csstricmp(tag_name, name) == 0)
			{
				return tag_index;
			}
		}
	}

	return NONE;
}

struct s_cache_file_global_tags_definition
{
	c_typed_tag_block<s_tag_reference> references;
	uns32 : 32;
};
COMPILE_ASSERT(sizeof(s_cache_file_global_tags_definition) == 0x10);

const char* tag_get_name(int32 tag_name_index)
{
	if (!g_cache_file_globals.header.debug_tag_name_count)
	{
		return "";
	}

	//ASSERT(g_cache_file_globals.tags_loaded);
	ASSERT(VALID_INDEX(tag_name_index, g_cache_file_globals.header.debug_tag_name_count));

	if (const char* name = g_cache_file_debug_globals->debug_tag_names[tag_name_index])
	{
		return name;
	}

	return "";

	//int32 tag_name_offset = g_cache_file_globals.debug_tag_names->offsets[tag_name_index];
	//ASSERT(VALID_INDEX(tag_name_offset, sizeof(g_cache_file_globals.debug_tag_names->buffer)));
	//
	//return &g_cache_file_globals.debug_tag_names->buffer[tag_name_offset];
}

const char* tag_get_name_safe(int32 tag_name_index)
{
	//ASSERT(g_cache_file_globals.tags_loaded);

	if (!g_cache_file_globals.header.debug_tag_name_count)
	{
		return "";
	}

	if (VALID_INDEX(tag_name_index, g_cache_file_globals.header.debug_tag_name_count))
	{
		if (const char* name = g_cache_file_debug_globals->debug_tag_names[tag_name_index])
		{
			return name;
		}

		return "";

		//int32 tag_name_offset = g_cache_file_globals.debug_tag_names->offsets[tag_name_index];
		//if (VALID_INDEX(tag_name_offset, sizeof(g_cache_file_globals.debug_tag_names->buffer)))
		//	return &g_cache_file_globals.debug_tag_names->buffer[tag_name_offset];
	}

	return "";
}

int32 tag_name_get_index(tag group_tag, const char* name)
{
	for (int32 tag_index = 0; tag_index < g_cache_file_globals.header.debug_tag_name_count; tag_index++)
	{
		const char* result = g_cache_file_debug_globals->debug_tag_names[tag_index];
		if (!result)
		{
			continue;
		}

		int32 tag_absolute_index = g_cache_file_globals.tag_index_absolute_mapping[tag_index];
		if (tag_absolute_index == NONE)
		{
			continue;
		}

		cache_file_tag_instance* tag_instance = g_cache_file_globals.tag_instances[tag_absolute_index];
		if (!tag_instance || tag_instance->tag_group != group_tag)
		{
			continue;
		}

		if (csstricmp(name, result) == 0)
		{
			return tag_index;
		}
	}

	return NONE;
}

//bool cache_file_blocking_read(enum e_cache_file_section,int32,int32,void *)
bool __cdecl cache_file_blocking_read(int32 cache_file_section, int32 section_offset, int32 buffer_size, void* buffer)
{
	//return INVOKE(0x005016D0, cache_file_blocking_read, cache_file_section, section_offset, buffer_size, buffer);

	c_synchronized_long done{};
	c_synchronized_long size{};
	cache_file_read_ex(cache_file_section, section_offset, buffer_size, buffer, &size, &done, _async_category_cache_file_misc, _async_priority_blocking_generic);
	internal_async_yield_until_done_with_networking(&done, false, true, __FILE__, __LINE__);

	return size.peek() == buffer_size;
}

bool __cdecl cache_file_content_signatures_match(int32 signature_a_size, const byte* signature_a, int32 signature_b_size, const byte* signature_b, bool use_full_language_dependent_signature)
{
	return INVOKE(0x00501740, cache_file_content_signatures_match, signature_a_size, signature_a, signature_b_size, signature_b, use_full_language_dependent_signature);

	//bool signatures_match = false;
	//if (signature_a_size == sizeof(c_cache_file_content_hashes_array) && signature_b_size == sizeof(c_cache_file_content_hashes_array))
	//{
	//	const c_cache_file_content_hashes_array* hashes_a = (const c_cache_file_content_hashes_array*)signature_a;
	//	const c_cache_file_content_hashes_array* hashes_b = (const c_cache_file_content_hashes_array*)signature_b;
	//
	//	if (use_full_language_dependent_signature)
	//	{
	//		signatures_match = csmemcmp(hashes_a, hashes_b, sizeof(c_cache_file_content_hashes_array)) == 0;
	//	}
	//}
	//return signatures_match;
}

bool __cdecl cache_file_get_content_signature(int32* out_signature_size, const byte** out_signature)
{
	return INVOKE(0x00501780, cache_file_get_content_signature, out_signature_size, out_signature);
}

int32 __cdecl cache_file_get_global_tag_index(tag group_tag)
{
	//return INVOKE(0x005017E0, cache_file_get_global_tag_index, group_tag);

	s_cache_file_global_tags_definition* global_tags = TAG_GET(CACHE_FILE_GLOBAL_TAGS_TAG, s_cache_file_global_tags_definition, 0L);

	int32 global_tag_index = NONE;
	if (global_tags && global_tags->references.count > 0)
	{
		for (int32 tag_reference_index = 0; tag_reference_index < global_tags->references.count; tag_reference_index++)
		{
			s_tag_reference* tag_reference = TAG_BLOCK_GET_ELEMENT(&global_tags->references, tag_reference_index, s_tag_reference);
			if (tag_reference && group_tag == tag_reference->group_tag)
			{
				global_tag_index = tag_reference->index;
				break;
			}
		}
	}

	return global_tag_index;
}

//.text:00501850 ; 
//.text:00501870 ; c_static_string<256>* __cdecl cache_file_get_path(c_static_string<256>* result, const char* scenario_name)

void __cdecl cache_file_get_path(const char* scenario_name, char* cache_file_path, int32 cache_file_path_size)
{
	INVOKE(0x005018C0, cache_file_get_path, scenario_name, cache_file_path, cache_file_path_size);

	//csnzprintf(cache_file_path, cache_file_path_size, "%s%s%s", cache_files_map_directory(), scenario_name, k_cache_file_extension);
}

struct __declspec(align(8)) s_cache_file_security_globals
{
	s_cache_file_header clean_header;
	bool hashes_valid;
	c_static_array<int32, 1> hash_sizes;
	c_static_array<const void*, 1> hash_addresses;
	c_static_array<s_network_http_request_hash, 1> hashes;
	byte hash_working_memory[0x400];
	s_network_http_request_hash hash_of_hashes;
	s_rsa_signature rsa_signature;
};
COMPILE_ASSERT(sizeof(s_cache_file_security_globals) == 0x38C8);
COMPILE_ASSERT(0x00000000 == OFFSETOF(s_cache_file_security_globals, clean_header));
COMPILE_ASSERT(0x00003390 == OFFSETOF(s_cache_file_security_globals, hashes_valid));
COMPILE_ASSERT(0x00003394 == OFFSETOF(s_cache_file_security_globals, hash_sizes));
COMPILE_ASSERT(0x00003398 == OFFSETOF(s_cache_file_security_globals, hash_addresses));
COMPILE_ASSERT(0x0000339C == OFFSETOF(s_cache_file_security_globals, hashes));
COMPILE_ASSERT(0x000037C4 == OFFSETOF(s_cache_file_security_globals, rsa_signature));

s_cache_file_security_globals* __cdecl cache_file_get_security_globals()
{
	return INVOKE(0x005018F0, cache_file_get_security_globals);
}

const void* __cdecl cache_file_globals_get_tag_cache_base_address()
{
	//return INVOKE(0x00501930, cache_file_globals_get_tag_cache_base_address);

	if (!g_cache_file_globals.tags_loaded)
	{
		return nullptr;
	}

	return g_cache_file_globals.tag_cache_base_address;
}

bool __cdecl cache_file_header_verify(const s_cache_file_header* header, const char* scenario_path, bool fail_fatally)
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
	//if (strlen_debug(header->name) >= sizeof(header->name))
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

bool __cdecl cache_file_header_verify_and_version(const s_cache_file_header* header, const char* scenario_path, bool fail_fatally)
{
	return INVOKE(0x00501AD0, cache_file_header_verify_and_version, header, scenario_path, fail_fatally);
}

void __cdecl cache_file_invalidate_signature()
{
	INVOKE(0x00501B20, cache_file_invalidate_signature);
}

//real32 cache_file_map_progress_estimated_megabytes_remaining(enum e_scenario_type,const char*)
real32 __cdecl cache_file_map_progress_estimated_megabytes_remaining(int32 scenario_type, const char* scenario_path)
{
	return INVOKE(0x00501B90, cache_file_map_progress_estimated_megabytes_remaining, scenario_type, scenario_path);
}

//int32 cache_file_map_progress_estimated_miliseconds_remaining(enum e_scenario_type,const char*)
int32 __cdecl cache_file_map_progress_estimated_miliseconds_remaining(int32 scenario_type, const char* scenario_path)
{
	return INVOKE(0x00501BB0, cache_file_map_progress_estimated_miliseconds_remaining, scenario_type, scenario_path);
}

//real32 cache_file_map_progress_helper(enum e_scenario_type, const char*, enum e_cache_file_progress_type)
real32 __cdecl cache_file_map_progress_helper(int32 scenario_type, const char* scenario_path, int32 progress_type)
{
	return INVOKE(0x00501BF0, cache_file_map_progress_helper, scenario_type, scenario_path, progress_type);
}

uns32 __cdecl cache_files_get_checksum()
{
	return INVOKE(0x00501F40, cache_files_get_checksum);
}

const s_cache_file_header* __cdecl cache_files_get_header()
{
	//return INVOKE(0x00501F90, cache_files_get_header);

	ASSERT(g_cache_file_globals.header.header_signature == k_cache_file_header_signature);
	ASSERT(g_cache_file_globals.header.version == k_cache_file_version);
	ASSERT(g_cache_file_globals.header.footer_signature == k_cache_file_footer_signature);

	return &g_cache_file_globals.header;
}

const s_rsa_signature* __cdecl cache_files_get_rsa_signature()
{
	return INVOKE(0x00501FA0, cache_files_get_rsa_signature);
}

int32 __cdecl cache_files_get_total_tags_size()
{
	return INVOKE(0x00501FB0, cache_files_get_total_tags_size);
}

const char* __cdecl cache_files_map_directory()
{
	//return INVOKE(0x00501FC0, cache_files_map_directory);

	if (!g_cache_file_globals.map_directory)
	{
		g_cache_file_globals.map_directory = "maps\\";
	}

	return g_cache_file_globals.map_directory;
}

void __cdecl cache_file_tags_fixup_all_resources(c_wrapped_array<uns32>& resource_offsets, s_cache_file_resource_gestalt* resource_gestalt)
{
	tag_block_set_elements(&resource_gestalt->resources, resource_gestalt + 1);
	g_cache_file_globals.resource_gestalt = resource_gestalt;

	s_cache_file_tag_resource_data** resource_blocks = resource_gestalt->resources.begin();

	int32 resource_count = 0;
	uns32 resources_size = 0;
	uns32 shared_files_available = 0;

	for (int32 i = 0; i < g_cache_file_globals.tag_loaded_count; i++)
	{
		cache_file_tag_instance* instance = g_cache_file_globals.tag_instances[i];
		cache_address* resource_fixups = reinterpret_cast<cache_address*>(&instance->dependencies[instance->dependency_count + instance->data_fixup_count]);

		for (int32 resource_fixup_index = 0; resource_fixup_index < instance->resource_fixup_count; resource_fixup_index++)
		{
			cache_address resource_fixup = resource_fixups[resource_fixup_index];

			if (!resource_fixup.value)
			{
				continue;
			}

			if (resource_fixup.persistent)
			{
				resource_fixup.persistent = false;
			}
			resource_fixup.value += (uns32)instance;

			s_tag_resource* resource = (s_tag_resource*)resource_fixup.value;
			if (!resource)
			{
				continue;
			}

			if (!resource->resource_data)
			{
				resource->resource_handle = NONE;
				continue;
			}

			s_cache_file_tag_resource_data* resource_data = resource->resource_data;
			resource_blocks[resource_count] = resource_data;

			uns8 flags = resource_data->file_location.flags.get_unsafe();

			if (!TEST_MASK(flags, k_cache_file_tag_resource_location_mask))
			{
				shared_files_available = 0;
				resource->resource_handle = resource_count++;
				resources_size += resource_data->file_location.size;
				continue;
			}

			if (resource_data->file_location.shared_file_location_index == NONE)
			{
				resource_data->file_location.file_offset = 0;

				shared_files_available = 1;
				resource->resource_handle = resource_count++;
				resources_size += resource_data->file_location.size;
				continue;
			}

			int32 resource_index = 0;
			for (int32 map_file_index = 0; map_file_index < k_cached_map_file_shared_count - 1; map_file_index++)
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

			shared_files_available = 1;
			resource->resource_handle = resource_count++;
			resources_size += resource_data->file_location.size;
		}
	}

	tag_block_set_element_count(&resource_gestalt->resources, resource_count);
	resource_gestalt->resources_size = resources_size;
	resource_gestalt->shared_files_available = shared_files_available;
}

s_cache_file_resource_gestalt* __cdecl cache_files_populate_resource_gestalt()
{
	c_wrapped_array<uns32> resource_offsets;
	cache_files_populate_resource_offsets(&resource_offsets);

	int32 total_resource_fixup_count = 0;
	for (int32 i = 0; i < g_cache_file_globals.tag_loaded_count; i++)
	{
		total_resource_fixup_count += g_cache_file_globals.tag_instances[i]->resource_fixup_count & 0xFFFF;
	}

	int32 resource_fixup_size = sizeof(s_cache_file_tag_resource_data*) * total_resource_fixup_count;
	int32 resource_gestalt_size = resource_fixup_size + sizeof(s_cache_file_resource_gestalt);

	s_cache_file_resource_gestalt* resource_gestalt = (s_cache_file_resource_gestalt*)_physical_memory_malloc(_memory_stage_level_initialize, nullptr, resource_gestalt_size, 0);
	csmemset(resource_gestalt, 0, resource_gestalt_size);

	cache_file_tags_fixup_all_resources(resource_offsets, resource_gestalt);

	if (resource_offsets.m_elements)
	{
		physical_memory_free(resource_offsets.m_elements);
	}

	return resource_gestalt;
}

//.text:005021B0 ; 

bool __cdecl cache_files_verify_header_rsa_signature(s_cache_file_header* header)
{
	//return cache_files_verify_header_rsa_signature(header);

	ASSERT(header);

	bool success = false;
	if (header->header_signature == k_cache_file_header_signature && header->footer_signature == k_cache_file_footer_signature)
	{
		s_cache_file_header clean_header = *header;
		cache_file_builder_security_clean_header(&clean_header);

		if (security_validate_hash(&clean_header, sizeof(s_cache_file_header), true, header->content_hashes, nullptr))
		{
			s_network_http_request_hash hash_of_hashes;
			security_calculate_hash(header->content_hashes, sizeof(s_network_http_request_hash), true, &hash_of_hashes);

			s_rsa_signature rsa_signature = header->rsa_signature;
			if (security_rsa_compute_and_verify_signature(&hash_of_hashes, &rsa_signature))
			{
				success = true;
			}
			else
			{
				event(_event_warning, "cache_files:header failed RSA signature verification - possible cheating?");
			}

			if (!success)
			{
				static char hash_buffer[4096]{};
				security_print_hash(&hash_of_hashes, hash_buffer, sizeof(hash_buffer));
				event(_event_warning, "cache_files:header: failed hash verification - copying new validated values, %s", hash_buffer);
				csmemcpy(header->content_hashes, &hash_of_hashes, sizeof(s_network_http_request_hash));
			}
		}
		else
		{
			event(_event_warning, "cache_files:header failed hash verification - possible cheating?");
		}
	}
	return success;
}

uns32 __cdecl compute_realtime_checksum(char* buffer, int len)
{
	return INVOKE(0x00502300, compute_realtime_checksum, buffer, len);
}

//.text:00502360 ; 
//.text:00502370 ; 
//.text:00502380 ; 
//.text:00502390 ; 
//.text:005023A0 ; 
//.text:005023B0 ; 
//.text:005023D0 ; 
//.text:005023E0 ; 
//.text:005023F0 ; 
//.text:00502400 ; 
//.text:00502410 ; 
//.text:00502420 ; 
//.text:00502430 ; 
//.text:00502440 ; 
//.text:00502450 ; 
//.text:00502460 ; 
//.text:00502490 ; 
//.text:005024B0 ; 
//.text:005024D0 ; 
//.text:005024E0 ; 
//.text:005024F0 ; 

void __cdecl cache_file_load_reports(s_cache_file_reports* reports, s_cache_file_header* header)
{
	DECLFUNC(0x00502500, void, __thiscall, s_cache_file_reports*, s_cache_file_header*)(reports, header);

	//int32 reports_buffer_size = cache_file_round_up_read_size(header->reports.size);
	//void* reports_buffer = _physical_memory_malloc(_memory_stage_level_initialize, nullptr, reports_buffer_size, 0);
	//
	//cache_file_blocking_read(_cache_file_debug_section, header->reports.offset, reports_buffer_size, reports_buffer);
	//
	//reports->elements = static_cast<s_cache_file_report*>(reports_buffer);
	//reports->count = GET_REPORT_COUNT_FROM_SIZE(header->reports.size);
}

void __cdecl cache_files_populate_resource_offsets(c_wrapped_array<uns32>* resource_offsets)
{
	//INVOKE(0x00502550, cache_files_populate_resource_offsets, resource_offsets);

	bool success = true;

	// shared map files - ui
	s_file_handle section_handles[k_cached_map_file_shared_count - 1]{};
	s_cache_file_section_header section_headers[k_cached_map_file_shared_count - 1]{};

	int32 file_count = 0;
	for (int32 i = 0; i < NUMBEROF(section_handles); i++)
	{
		e_map_file_index map_file_index = e_map_file_index(i);
		e_map_file_index next_map_file_index = e_map_file_index(i + 1);

		if (!cached_map_file_is_shared(map_file_index))
		{
			continue;
		}

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

		int32 section_file_count = section_headers[i].file_count;
		file_count += section_file_count;

		g_cache_file_globals.resource_file_counts_mapping[i] = section_file_count;
	}

	if (!success)
	{
		resource_offsets->set_elements(nullptr, 0);
		return;
	}

	uns32* file_offsets = (uns32*)_physical_memory_malloc(_memory_stage_level_initialize, nullptr, sizeof(uns32) * file_count, 0);

	int32 offset_index_offset = 0;
	for (int32 i = 0; i < NUMBEROF(section_handles); i++)
	{
		e_map_file_index map_file_index = e_map_file_index(i);
		if (!cached_map_file_is_shared(map_file_index))
		{
			continue;
		}

		void* offsets = (byte*)file_offsets + offset_index_offset;
		int32 offsets_size = sizeof(int32) * section_headers[i].file_count;

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
		resource_offsets->set_elements(nullptr, 0);
		return;
	}

	resource_offsets->set_elements(file_offsets, file_count);
}

//.text:00502750 ; 
//.text:00502760 ; bool __cdecl scenario_structure_bsp_load(int32, int16, s_tag_reference*)
//.text:00502770 ; void __cdecl scenario_structure_bsp_unload(int32, int16, scenario_structure_bsp_reference*)

//#define EXPERIMENTAL_USE_SYSTEM_ALLOCATION_FOR_TAG_CACHE

enum
{
#if !defined(EXPERIMENTAL_USE_SYSTEM_ALLOCATION_FOR_TAG_CACHE)
	//  75 * 1024 * 1024 or 75MiB
	k_tag_cache_size = k_physical_memory_tag_cache_size,
#else
	// 100 * 1024 * 1024 or 100MiB
	k_tag_cache_size = k_physical_memory_tag_cache_size_new
#endif
};

bool __cdecl cache_file_tags_load_recursive(int32 tag_index)
{
	//return INVOKE(0x00502780, cache_file_tags_load_recursive, tag_index);

	VASSERT(g_cache_file_globals.tag_cache_size > g_cache_file_globals.tag_loaded_size, 
		"tag cache insufficient memory allocation size, either decrease the size of the loaded tags for this scenario or try enabling the 'EXPERIMENTAL_USE_SYSTEM_ALLOCATION_FOR_TAG_CACHE' define if this persists");

	cache_file_tag_instance* instance = reinterpret_cast<cache_file_tag_instance*>(g_cache_file_globals.tag_cache_base_address + g_cache_file_globals.tag_loaded_size);

	if (tag_index == NONE)
	{
		return false;
	}

	if (g_cache_file_globals.tag_index_absolute_mapping[tag_index] != NONE)
	{
		return true;
	}

	if (!cache_file_tags_section_read(g_cache_file_globals.tag_cache_offsets[tag_index], cache_file_round_up_read_size(sizeof(cache_file_tag_instance)), instance))
	{
		return false;
	}

	g_cache_file_globals.tag_loaded_size += instance->total_size;
	g_cache_file_globals.tag_instances[g_cache_file_globals.tag_loaded_count] = instance;
	g_cache_file_globals.tag_index_absolute_mapping[tag_index] = g_cache_file_globals.tag_loaded_count;
	g_cache_file_globals.absolute_index_tag_mapping[g_cache_file_globals.tag_loaded_count] = tag_index;

	if (!cache_file_tags_section_read(g_cache_file_globals.tag_cache_offsets[tag_index], instance->total_size, instance->base))
	{
		return false;
	}

	if (crc_checksum_buffer_adler32(adler_new(), instance->base + sizeof(instance->checksum), instance->total_size - sizeof(instance->checksum)) != instance->checksum)
	{
		return false;
	}

	// not needed
	//sub_503470(&g_cache_file_globals.reports, tag_instance, tag_index);

	g_cache_file_globals.tag_loaded_size += sizeof(int32);
	*reinterpret_cast<int32*>(offset_pointer(instance, instance->total_size)) = tag_index;

	g_cache_file_globals.tag_loaded_count++;

	tag_instance_modification_apply(instance, _instance_modification_stage_post_tag_load);

	if (instance->dependency_count <= 0)
	{
		return true;
	}

	int16 dependency_index = 0;
	while (cache_file_tags_load_recursive(instance->dependencies[dependency_index]))
	{
		if (++dependency_index >= instance->dependency_count)
		{
			return true;
		}
	}

	return false;
}

// to mitigate a lot of file read operations we load all of `tags.dat` into this buffer
// I don't like having this as a global but for right now its fine
static void* tags_section = nullptr;

void __cdecl cache_file_load_tags_section()
{
	//INVOKE(0x005028C0, cache_file_load_tags_section);

	if (!TEST_BIT(g_cache_file_globals.header.shared_file_flags, 1))
	{
		return;
	}

	if (g_cache_file_globals.tags_section.path[0] == 0)
	{
		file_reference_create_from_path(&g_cache_file_globals.tags_section, k_cache_tags_file, false);
	}

	uns32 error = 0;
	if (file_open(&g_cache_file_globals.tags_section, FLAG(_permission_read_bit), &error))
	{
		uns32 file_size = 0;
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
HOOK_DECLARE_CALL(0x00502E87, cache_file_load_tags_section);

void __cdecl cache_file_close_tags_section()
{
	//INVOKE(0x00502900, cache_file_close_tags_section);

	if (!TEST_BIT(g_cache_file_globals.header.shared_file_flags, 1))
	{
		return;
	}

	file_close(&g_cache_file_globals.tags_section);

	if (tags_section)
	{
		free(tags_section);
		tags_section = nullptr;
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

	int32 tag_index = instance->get_tag_index();
	const char* tag_name = instance->get_tag_name();
	const char* group_tag_name = instance->tag_group.name.get_string();

	cache_address* data_fixups = reinterpret_cast<cache_address*>(instance->dependencies + instance->dependency_count);
	for (int16 data_fixup_index = 0; data_fixup_index < instance->data_fixup_count; data_fixup_index++)
	{
		cache_address& data_fixup = *reinterpret_cast<cache_address*>(instance->base + data_fixups[data_fixup_index].offset);

		// 0.4.11.2 tags messed up `ui\halox\pregame_lobby\switch_lobby\lobbies.gui_datasource_definition` data fixups
		if (!data_fixup.value)
		{
			event(_event_warning, "tags: bad data_fixups[%d].value == 0 for tag [0x%08X, '%s.%s']",
				data_fixup_index,
				tag_index,
				tag_name,
				group_tag_name);
			continue;
		}

		// 0.4.11.2 tags messed up `levels\multi\s3d_avalanche\s3d_avalanche.scenario` data fixups
		if (!data_fixup.persistent)
		{
			event(_event_warning, "tags: data_fixups[%d].persistent == false for tag [0x%08X, '%s.%s']",
				data_fixup_index,
				tag_index,
				tag_name,
				group_tag_name);
		}

		//ASSERT(data_fixup.persistent == true);

		if (!IN_RANGE_INCLUSIVE(data_fixup.value, (uns32)instance, (uns32)instance + instance->total_size))
		{
			data_fixup.persistent = false;
			ASSERT(data_fixup.persistent == false);

			data_fixup.value += (uns32)instance;
			//ASSERT(data_fixup.value == data_fixup.offset);
		}
	}

	tag_instance_modification_apply(instance, _instance_modification_stage_post_tag_fixup);

	return true;
}

bool __cdecl cache_file_debug_tag_names_load()
{
	//return INVOKE(0x00502970, cache_file_debug_tag_names_load);

	// header
	decltype(g_cache_file_globals.header.debug_tag_name_count)& debug_tag_name_count = g_cache_file_globals.header.debug_tag_name_count;
	decltype(g_cache_file_globals.header.debug_tag_name_data_offset)& debug_tag_name_data_offset = g_cache_file_globals.header.debug_tag_name_data_offset;
	decltype(g_cache_file_globals.header.debug_tag_name_data_size)& debug_tag_name_data_size = g_cache_file_globals.header.debug_tag_name_data_size;
	decltype(g_cache_file_globals.header.debug_tag_name_index_offset)& debug_tag_name_index_offset = g_cache_file_globals.header.debug_tag_name_index_offset;

	// debug_tag_names
	decltype(g_cache_file_debug_globals->debug_tag_name_offsets)& offsets = g_cache_file_debug_globals->debug_tag_name_offsets;
	constexpr uns32 offsets_size = sizeof(g_cache_file_debug_globals->debug_tag_name_offsets);
	decltype(g_cache_file_debug_globals->debug_tag_name_buffer)& buffer = g_cache_file_debug_globals->debug_tag_name_buffer;
	constexpr uns32 buffer_size = sizeof(g_cache_file_debug_globals->debug_tag_name_buffer);
	decltype(g_cache_file_debug_globals->debug_tag_names)& storage = g_cache_file_debug_globals->debug_tag_names;
	constexpr uns32 storage_size = sizeof(g_cache_file_debug_globals->debug_tag_names);

	//if ((g_cache_file_globals.header.shared_file_flags & 0x3F) == 0) // `shared_file_flags` is 0x3E
	//if (!TEST_BIT(g_cache_file_globals.header.shared_file_flags, 0))
	if (debug_tag_name_count)
	{
		if (!cache_file_blocking_read(_cache_file_debug_section, debug_tag_name_data_offset, cache_file_round_up_read_size(debug_tag_name_data_size), buffer))
		{
			return false;
		}

		if (!cache_file_blocking_read(_cache_file_debug_section, debug_tag_name_index_offset, cache_file_round_up_read_size(debug_tag_name_count * sizeof(int32)), offsets))
		{
			return false;
		}
	}
	else
	{
		s_file_reference tag_list_file;
		file_reference_create_from_path(&tag_list_file, k_cache_tag_list_file, false);
		if (!file_exists(&tag_list_file))
		{
			event(_event_warning, "cache: load tag names, '%s' file doesn't exist", k_cache_tag_list_file);
			return true;
		}

		uns32 tag_list_size = 0;
		file_get_size(&tag_list_file, &tag_list_size);
		if (!file_read_into_buffer(&tag_list_file, buffer, buffer_size))
		{
			event(_event_error, "cache: load tag names, '%s' file read failed", k_cache_tag_list_file);
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

			int32 debug_tag_name_index = NONE;
			if (sscanf_s(line, "0x%X,", &debug_tag_name_index))
			{
				while (debug_tag_name_count < debug_tag_name_index)
				{
					debug_tag_name_count++;
				}
			}

			offsets[debug_tag_name_count++] = comma_pos - (char*)offsets - offsets_size;

			if ((line_end + 1 - (char*)offsets - offsets_size) >= tag_list_size)
			{
				break;
			}

			line = line_end + 1;
		}
	}

	csmemset(storage, 0, storage_size);

	for (int32 tag_names_index = 0; tag_names_index < NUMBEROF(offsets); ++tag_names_index)
	{
		if (offsets[tag_names_index] >= buffer_size)
		{
			break;
		}

		if (offsets[tag_names_index] != 0)
		{
			storage[tag_names_index] = &buffer[offsets[tag_names_index]];
		}
	}

	return true;
}

bool __cdecl cache_file_tags_load_allocate()
{
	//return INVOKE(0x00502B40, cache_file_tags_load_allocate);

	bool result = true;
	int32 tag_offsets_size = 0;

	//if (!TEST_BIT(g_cache_file_globals.header.shared_file_flags, 1))
	if (g_cache_file_globals.header.tag_count)
	{
		tag_offsets_size = sizeof(int32) * g_cache_file_globals.header.tag_count;
		g_cache_file_globals.tag_total_count = g_cache_file_globals.header.tag_count;
		g_cache_file_globals.tag_cache_offsets = (int32*)_physical_memory_malloc(_memory_stage_level_initialize, nullptr, cache_file_round_up_read_size(tag_offsets_size), 0);

		result = cache_file_blocking_read(_cache_file_tag_section, g_cache_file_globals.header.tag_cache_offsets, cache_file_round_up_read_size(tag_offsets_size), g_cache_file_globals.tag_cache_offsets);
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
			{
				return false;
			}
		}

		tag_offsets_size = sizeof(int32) * tags_header.file_count;
		g_cache_file_globals.tag_total_count = tags_header.file_count;
		g_cache_file_globals.tag_cache_offsets = (int32*)_physical_memory_malloc(_memory_stage_level_initialize, nullptr, tag_offsets_size, 0);
		int32* tag_offsets = (int32*)_physical_memory_malloc(_memory_stage_level_initialize, nullptr, tag_offsets_size, 0);

		if (!cache_file_tags_section_read(tags_header.file_offsets, tag_offsets_size, tag_offsets))
		{
			return false;
		}

		int32 tag_offset = 0;
		for (int32 tag_index = 0; tag_index < tags_header.file_count; g_cache_file_globals.tag_cache_offsets[tag_index - 1] = tag_offset)
		{
			tag_offset = tag_offsets[tag_index++];
		}

		physical_memory_free(tag_offsets);
	}

	g_cache_file_globals.tag_index_absolute_mapping = (int32*)_physical_memory_malloc(_memory_stage_level_initialize, nullptr, tag_offsets_size, 0);
	g_cache_file_globals.absolute_index_tag_mapping = (int32*)_physical_memory_malloc(_memory_stage_level_initialize, nullptr, tag_offsets_size, 0);
	csmemset(g_cache_file_globals.tag_index_absolute_mapping, NONE, tag_offsets_size);
	csmemset(g_cache_file_globals.absolute_index_tag_mapping, NONE, tag_offsets_size);

	return result;
}
HOOK_DECLARE_CALL(0x00502E9B, cache_file_tags_load_allocate);

bool __cdecl cache_file_tags_section_read(int32 offset, int32 size, void* buffer)
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
		{
			return file_read(&g_cache_file_globals.tags_section, size, false, buffer);
		}

		return result;
	}

	return cache_file_blocking_read(_cache_file_tag_section, offset, size, buffer);
}

void __cdecl cache_file_tags_unload()
{
	//INVOKE(0x00502CE0, cache_file_tags_unload);

	if (g_cache_file_globals.tag_cache_base_address)
	{
		physical_memory_free(g_cache_file_globals.tag_cache_base_address);

		g_cache_file_globals.tag_cache_base_address = nullptr;
		g_cache_file_globals.tag_loaded_size = 0;
	}

	if (g_cache_file_globals.tag_instances)
	{
		physical_memory_free(g_cache_file_globals.tag_instances);
		g_cache_file_globals.tag_instances = nullptr;
	}

	if (g_cache_file_globals.tag_cache_offsets)
	{
		physical_memory_free(g_cache_file_globals.tag_cache_offsets);
		g_cache_file_globals.tag_cache_offsets = nullptr;
	}

	if (g_cache_file_globals.tag_index_absolute_mapping)
	{
		physical_memory_free(g_cache_file_globals.tag_index_absolute_mapping);
		g_cache_file_globals.tag_index_absolute_mapping = nullptr;
	}

	if (g_cache_file_globals.absolute_index_tag_mapping)
	{
		physical_memory_free(g_cache_file_globals.absolute_index_tag_mapping);
		g_cache_file_globals.absolute_index_tag_mapping = nullptr;
	}

	if (g_cache_file_globals.resource_gestalt)
	{
		physical_memory_free(g_cache_file_globals.resource_gestalt);
		g_cache_file_globals.resource_gestalt = nullptr;
	}

	if (g_cache_file_globals.reports.elements)
	{
		physical_memory_free(g_cache_file_globals.reports.elements);
		g_cache_file_globals.reports.elements = nullptr;
		g_cache_file_globals.reports.count = 0;
	}
}

void cache_file_transform_creator_string(c_wrapped_array<char> in_out_creator_string)
{
	if (!in_out_creator_string[0])
	{
		return;
	}

	ASSERT(NUMBEROF(g_cache_file_creator_key) >= in_out_creator_string.count());

	for (int32 i = 0; i < in_out_creator_string.count(); i++)
	{
		in_out_creator_string[i] ^= g_cache_file_creator_key[i];
	}
}

void cache_files_update_main_status()
{
	c_static_string<32> creator_name = g_cache_file_globals.header.creator_name;
	c_wrapped_array<char> creator_string = c_wrapped_array<char>(creator_name.get());
	cache_file_transform_creator_string(creator_string);
	creator_name.null_terminate_buffer();
	main_status("map created by", "%s", creator_name.get_string());
	c_console::write_line("map created by %s", creator_name.get_string());
}

bool __cdecl scenario_tags_load(const char* scenario_path)
{
	//bool result = INVOKE(0x00502DC0, scenario_tags_load, scenario_path);
	//ASSERT(cache_file_debug_tag_names_load());
	//return result;

	int32 tag_index = NONE;
	bool success = false;

	event(_event_message, "cache: scenario load tags, name=%s", scenario_path);

	cache_file_invalidate_signature();

	if (cache_file_open(scenario_path, &g_cache_file_globals.header) && cache_file_header_verify_and_version(&g_cache_file_globals.header, scenario_path, false))
	{
		cache_files_update_main_status();

		s_cache_file_header header_copy{};
		csmemcpy(&header_copy, &g_cache_file_globals.header, sizeof(s_cache_file_header));
		loading_basic_progress_phase_begin(_basic_loading_phase_tag_load, 1);

		{
			c_static_string<260> tags_filepath = {};

			// `scenario_path` usually consists of map file without the `.map` extension
			tags_filepath.print("%s.dat", scenario_path);

			s_file_reference tags_file{};
			file_reference_create_from_path(&tags_file, tags_filepath.get_string(), false);

			g_cache_file_globals.tags_section = {};
			if (file_exists(&tags_file))
			{
				g_cache_file_globals.tags_section = tags_file;
			}
		}

		cache_file_load_tags_section();

		// only needed if `sub_503470` is called in `cache_file_tags_load_recursive`
		//cache_file_load_reports(&g_cache_file_globals.reports, &g_cache_file_globals.header);

		cache_file_tags_load_allocate();

		uns32 total_instance_size = sizeof(cache_file_tag_instance*) * g_cache_file_globals.tag_total_count;
		g_cache_file_globals.tag_instances = (cache_file_tag_instance**)_physical_memory_malloc(_memory_stage_level_initialize, nullptr, total_instance_size, 0);
		csmemset(g_cache_file_globals.tag_instances, 0, total_instance_size);

		g_cache_file_globals.tag_loaded_count = 0;

		//g_cache_file_globals.tag_cache_size = g_cache_file_globals.header.total_tags_size;
		g_cache_file_globals.tag_cache_size = k_tag_cache_size;
		g_cache_file_globals.tag_cache_base_address = (byte*)_physical_memory_malloc(_memory_stage_level_initialize, "tag cache", g_cache_file_globals.tag_cache_size, 0);
		csmemset(g_cache_file_globals.tag_cache_base_address, 0, g_cache_file_globals.tag_cache_size);

		g_cache_file_globals.tag_loaded_size = 0;

		success = g_cache_file_globals.tag_cache_base_address != nullptr;
		if (!success)
		{
			event(_event_critical, "failed to allocate the physical memory for the tags");
		}

		if (success)
		{
			success = cache_file_debug_tag_names_load();
		}

		if (!success)
		{
			event(_event_critical, "failed to load debug tag names");
		}

		if (bool cache_file_global_tags_loaded = cache_file_tags_load_recursive(0))
		{
			success = true;
			cache_file_tags_single_tag_instance_fixup(g_cache_file_globals.tag_instances[0]);

			// if no global snenario reference was found in cache file global tags we fallback to the cache file header
			if (cache_file_get_global_tag_index(SCENARIO_TAG) == NONE)
			{
				success = cache_file_tags_load_recursive(g_cache_file_globals.header.scenario_index);
			}
		}

		cache_file_close_tags_section();
		loading_basic_progress_phase_end();

		if (!success)
		{
			global_preferences_invalidate_maps();
			event(_event_critical, "failed to read the tag data section");
		}

		// $TODO security stuff
		//	calculate hash
		//	calculate hash signature
		//	compare hash signatures
		s_cache_file_security_globals* security_globals = cache_file_get_security_globals();

		//if (success)
		//{
		//	main_loop_pregame();
		//
		//	s_cache_file_security_globals* security_state = security_globals;
		//	security_globals->hashes_valid = false;
		//
		//	csmemset(&security_globals->hashes[0], 0xBB, sizeof(s_network_http_request_hash));
		//	csmemset(&security_globals->hash_of_hashes, 0xCC, sizeof(s_network_http_request_hash));
		//	csmemset(&security_globals->rsa_signature, 0xDD, sizeof(s_rsa_signature));
		//
		//	security_state->clean_header = header_copy;
		//	cache_file_builder_security_clean_header(&security_state->clean_header);
		//
		//	security_state->hash_sizes[0] = sizeof(s_cache_file_header);
		//	security_state->hash_addresses[0] = security_state;
		//	byte* hash_working_memory = security_state->hash_working_memory;
		//
		//	int32 v7 = sizeof(s_cache_file_header);
		//	if (security_incremental_hash_begin(hash_working_memory, 0x400, true))
		//	{
		//		do
		//		{
		//			int32 v8 = v7;
		//			if (v7 > 0x100000)
		//			{
		//				v8 = 0x100000;
		//			}
		//
		//			security_incremental_hash_update(hash_working_memory, 0x400, security_state, v8);
		//			main_loop_pregame();
		//
		//			v7 -= v8;
		//			security_state = (s_cache_file_security_globals*)offset_pointer(security_state, v8);
		//
		//		} while (v7 > 0);
		//		security_incremental_hash_finish(hash_working_memory, 0x400, &security_state->hashes[0]);
		//	}
		//
		//	security_calculate_hash(&security_globals->hashes, sizeof(s_network_http_request_hash), 1, &security_globals->hash_of_hashes);
		//	security_globals->rsa_signature = g_cache_file_globals.header.rsa_signature;
		//
		//	main_loop_pregame();
		//
		//	success = security_rsa_compute_and_verify_signature(&security_globals->hash_of_hashes, &security_globals->rsa_signature);
		//}

		if (success)
		{
			//success = string_id_load_strings(&g_cache_file_globals.header);
			if (!success)
			{
				event(_event_error, "networking:failed to load the string ids [%s]", scenario_path);
			}
		}

		if (success)
		{
			security_globals->hashes_valid = true;

			cache_file_tags_fixup_all_instances();

			tag_index = cache_file_get_global_tag_index(SCENARIO_TAG);

			// if no global scenario reference was found in cache file global tags we fallback to the cache file header
			if (tag_index == NONE)
			{
				tag_index = g_cache_file_globals.header.scenario_index;
			}

			g_cache_file_globals.tags_loaded = true;
		}
	}

	if (!success)
	{
		event(_event_critical, "failed to load tags for cache file");
		cache_file_tags_unload();
		cache_file_close();
		ASSERT(tag_index == NONE);
	}

	global_scenario_index = tag_index;
	if (tag_index != NONE)
	{
		global_scenario_game_globals_index = cache_file_get_global_tag_index(GLOBALS_TAG);
		global_scenario = TAG_GET(SCENARIO_TAG, struct scenario, global_scenario_index);
		global_game_globals = TAG_GET(GLOBALS_TAG, s_game_globals, global_scenario_game_globals_index);

		c_rasterizer_globals* rasterizer_globals = global_game_globals->rasterizer_globals_ref.cast_to<c_rasterizer_globals>();
		if (rasterizer_globals)
		{
			c_rasterizer::g_max_vs_gprs = rasterizer_globals->get_max_vs_gprs();
			c_rasterizer::g_max_ps_gprs = rasterizer_globals->get_max_ps_gprs();
		}

		success = true;
	}

	event(_event_message, "cache: scenario load tags, success=%s", success ? "true" : "false");

	return success;
}

void __cdecl scenario_tags_load_finished()
{
	// nullsub
	INVOKE(0x00503190, scenario_tags_load_finished);

	for (int32 i = 0; i < g_cache_file_globals.tag_loaded_count; i++)
	{
		int32 tag_index = g_cache_file_globals.absolute_index_tag_mapping[i];

		cache_file_tag_instance* instance = g_cache_file_globals.tag_instances[i];
		tag_instance_modification_apply(instance, _instance_modification_stage_post_scenario_tags_load);
	}
}

void __cdecl cache_file_tags_fixup_all_instances()
{
	//INVOKE(0x005031A0, cache_file_tags_fixup_all_instances);

	for (int32 i = 0; i < g_cache_file_globals.tag_loaded_count; i++)
	{
		int32 tag_index = g_cache_file_globals.absolute_index_tag_mapping[i];

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

//.text:00503270 ; 
//.text:00503290 ; 
//.text:005032B0 ; 
//.text:005032D0 ; 
//.text:005032E0 ; 
//.text:005032F0 ; 

void __cdecl tag_files_close()
{
	string_id_dispose();

	INVOKE(0x00503300, tag_files_close);

	//cache_file_tag_resources_dispose();
	//physical_memory_free(g_cache_file_globals.debug_tag_names);
	//g_cache_file_globals.debug_tag_names = nullptr;
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

void __cdecl tag_files_initialize_from_main()
{
	INVOKE(0x00503330, tag_files_initialize_from_main);
}

void __cdecl tag_files_open()
{
	//INVOKE(0x00503340, tag_files_open);

	cache_files_initialize();

	ASSERT(g_cache_file_debug_globals == nullptr);
	g_cache_file_debug_globals = (s_cache_file_debug_globals*)_physical_memory_malloc(_memory_stage_game_initialize, "cache file debug globals", sizeof(s_cache_file_debug_globals), 1);

	string_id_initialize();

	cache_file_tag_resources_initialize();
}

void* __cdecl tag_get(tag group_tag, int32 tag_index)
{
	g_last_tag_accessed = { .group_tag = group_tag, .index = tag_index };

	if (tag_index == NONE)
	{
		return nullptr;
	}

	int32 tag_absolute_index = g_cache_file_globals.tag_index_absolute_mapping[tag_index];
	if (tag_absolute_index == NONE)
	{
		return nullptr;
	}

	cache_file_tag_instance* tag_instance = g_cache_file_globals.tag_instances[tag_absolute_index];
	if (!tag_instance)
	{
		return nullptr;
	}

	void* data = tag_instance->base + tag_instance->offset;

	// due to bad caches the tag address needs checking
	if (!IN_RANGE_INCLUSIVE((uns32)data, (uns32)g_cache_file_globals.tag_cache_base_address, (uns32)g_cache_file_globals.tag_cache_base_address + g_cache_file_globals.tag_loaded_size))
	{
		return nullptr;
	}

	return data;
}

void* __cdecl tag_get(tag group_tag, const char* tag_name)
{
	tag_iterator iterator{};
	tag_iterator_new(&iterator, group_tag);
	for (int32 tag_index = tag_iterator_next(&iterator); tag_index != NONE; tag_index = tag_iterator_next(&iterator))
	{
		const char* _tag_name = tag_get_name_safe(tag_index);
		if (_tag_name && csstricmp(_tag_name, tag_name) == 0)
		{
			return tag_get(group_tag, tag_index);
		}
	}

	return nullptr;
}

uns32 __cdecl tag_get_group_tag(int32 tag_index)
{
	return INVOKE(0x005033A0, tag_get_group_tag, tag_index);
}

//.text:005033C0 ; uns32 __cdecl tag_group_get_parent_group_tag(uns32)
//.text:005033D0 ; int32 __cdecl cache_files_get_loaded_tags_count()

void __cdecl tag_iterator_new(tag_iterator* iterator, tag group_tag)
{
	INVOKE(0x005033E0, tag_iterator_new, iterator, group_tag);
}

int32 __cdecl tag_iterator_next(tag_iterator* iterator)
{
	return INVOKE(0x00503400, tag_iterator_next, iterator);
}

// __thiscall
void __fastcall sub_503470(s_cache_file_reports* reports, void* unused, cache_file_tag_instance* instance, int32 tag_index)
{
	ASSERT(instance);

	if (instance->dependency_count || instance->data_fixup_count || instance->resource_fixup_count)
	{
		return;
	}

	c_console::write_line("0x%08X.%s", tag_index, instance->tag_group.name.get_string());
	static char tag_instance_byte_string[sizeof(cache_file_tag_instance) * 3]{};
	type_as_byte_string(instance, tag_instance_byte_string);
	c_console::write_line(tag_instance_byte_string);
}

//.text:00503510 ; void* __cdecl tag_try_and_get_unsafe(uns32, int32)

// $TODO create some sort of tag modification manager
void apply_globals_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != GLOBALS_TAG)
	{
		return;
	}

	s_game_globals* game_globals = instance->cast_to<s_game_globals>();
	const char* tag_name = instance->get_tag_name();
	const char* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_post_tag_load:
	{
		game_globals->input_globals.index = NONE;
	}
	break;
	case _instance_modification_stage_post_tag_fixup:
	{
		ASSERT(game_globals->input_globals.index == NONE);

		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(game_globals);
	}
	break;
	case _instance_modification_stage_post_scenario_tags_load:
	{
		UPDATE_STATIC_RUNTIME_DATA(game_globals);
	}
	break;
	}
}

// $TODO create some sort of tag modification manager
void apply_multiplayer_globals_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != MULTIPLAYER_GLOBALS_TAG)
	{
		return;
	}

	//// if the very first offset is not 0x20 there is a very high likelihood that this is an ElDewrito tag set
	//bool is_base_cache = g_cache_file_globals.tag_cache_offsets[0] == 0x20;

	s_multiplayer_globals_definition* multiplayer_globals = instance->cast_to<s_multiplayer_globals_definition>();
	const char* tag_name = instance->get_tag_name();
	const char* group_tag_name = instance->tag_group.name.get_string();

	// Add back missing weapon selections
	switch (stage)
	{
	case _instance_modification_stage_post_tag_load:
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
		//	if (const c_map_variant* map_variant = network_squad_session_get_map_variant()) for (const s_variant_quota& quota : map_variant->m_quotas)
		//	{
		//		if (quota.object_definition_index == NONE)
		//		{
		//			continue;
		//		}
		//
		//		cache_file_tags_load_recursive(quota.object_definition_index);
		//	}
		//}
	}
	break;
	case _instance_modification_stage_post_tag_fixup:
	{
		//if (is_base_cache)
		//{
		//	if (multiplayer_globals == nullptr || multiplayer_globals->universal.count <= 0 || multiplayer_globals->universal[0].weapon_selections.count <= 0)
		//	{
		//		return;
		//	}
		//
		//	for (s_multiplayer_weapon_selection& weapon_selection : multiplayer_globals->universal[0].weapon_selections)
		//	{
		//		if (weapon_selection.weapon_tag.index != NONE)
		//		{
		//			continue;
		//		}
		//
		//		switch (weapon_selection.name.get_value())
		//		{
		//		case STRING_ID(global, spike_rifle):
		//		{
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\rifle\\spike_rifle\\spike_rifle");
		//		}
		//		break;
		//		case STRING_ID(global, sword):
		//		case STRING_ID(global, energy_blade):
		//		{
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\melee\\energy_blade\\energy_blade");
		//		}
		//		break;
		//		case STRING_ID(global, needler):
		//		{
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\pistol\\needler\\needler");
		//		}
		//		break;
		//		case STRING_ID(global, rocket_launcher):
		//		{
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\support_high\\rocket_launcher\\rocket_launcher");
		//		}
		//		break;
		//		case STRING_ID(global, shotgun):
		//		{
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\rifle\\shotgun\\shotgun");
		//		}
		//		break;
		//		case STRING_ID(global, sniper_rifle):
		//		{
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\rifle\\sniper_rifle\\sniper_rifle");
		//		}
		//		break;
		//		case STRING_ID(global, brute_shot):
		//		{
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\support_low\\brute_shot\\brute_shot");
		//		}
		//		break;
		//		case STRING_ID(global, beam_rifle):
		//		{
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\rifle\\beam_rifle\\beam_rifle");
		//		}
		//		break;
		//		case STRING_ID(global, spartan_laser):
		//		{
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\support_high\\spartan_laser\\spartan_laser");
		//		}
		//		break;
		//		case STRING_ID(global, gravity_hammer):
		//		{
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\melee\\gravity_hammer\\gravity_hammer");
		//		}
		//		break;
		//		case STRING_ID(global, flame_thrower):
		//		{
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\turret\\flamethrower\\flamethrower");
		//		}
		//		break;
		//		case STRING_ID(global, missile_launcher):
		//		{
		//			tag_reference_set(&weapon_selection.weapon_tag, WEAPON_TAG, "objects\\weapons\\turret\\missile_pod\\missile_pod");
		//		}
		//		break;
		//		}
		//	}
		//}

		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(multiplayer_globals);
	}
	break;
	case _instance_modification_stage_post_scenario_tags_load:
	{
	}
	break;
	}
}

// $TODO create some sort of tag modification manager
void apply_rasterizer_globals_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != RASTERIZER_GLOBALS_TAG)
	{
		return;
	}

	c_rasterizer_globals* rasterizer_globals = instance->cast_to<c_rasterizer_globals>();
	const char* tag_name = instance->get_tag_name();
	const char* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_post_tag_load:
	{
	}
	break;
	case _instance_modification_stage_post_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(rasterizer_globals);
	}
	break;
	case _instance_modification_stage_post_scenario_tags_load:
	{
	}
	break;
	}
}

// $TODO create some sort of tag modification manager
void apply_scenario_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != SCENARIO_TAG)
	{
		return;
	}

	struct scenario* scenario = instance->cast_to<struct scenario>();
	const char* tag_name = instance->get_tag_name();
	const char* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_post_tag_load:
	{
	}
	break;
	case _instance_modification_stage_post_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(scenario);
	}
	break;
	case _instance_modification_stage_post_scenario_tags_load:
	{
	}
	break;
	}
}

// $TODO create some sort of tag modification manager
void apply_chud_globals_definition_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != CHUD_GLOBALS_DEFINITION_TAG)
	{
		return;
	}

	s_chud_globals_definition* chud_globals_definition = instance->cast_to<s_chud_globals_definition>();
	const char* tag_name = instance->get_tag_name();
	const char* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_post_tag_load:
	{
	}
	break;
	case _instance_modification_stage_post_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(chud_globals_definition);
	}
	break;
	case _instance_modification_stage_post_scenario_tags_load:
	{
		if (chud_globals_definition->sprint_fov_multiplier == 0.0f)
		{
			chud_globals_definition->sprint_fov_multiplier = 1.0f;
		}
	}
	break;
	}
}

// $TODO create some sort of tag modification manager
void apply_vision_mode_definition_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != VISION_MODE_TAG)
	{
		return;
	}

	s_vision_mode_definition* vision_mode_definition = instance->cast_to<s_vision_mode_definition>();
	const char* tag_name = instance->get_tag_name();
	const char* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_post_tag_load:
	{
	}
	break;
	case _instance_modification_stage_post_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(vision_mode_definition);
	}
	break;
	case _instance_modification_stage_post_scenario_tags_load:
	{
	}
	break;
	}
}

// $TODO create some sort of tag modification manager
void apply_object_definition_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != OBJECT_TAG)
	{
		return;
	}

	struct object_definition* object_definition = instance->cast_to<struct object_definition>();
	const char* tag_name = instance->get_tag_name();
	const char* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_post_tag_load:
	{
	}
	break;
	case _instance_modification_stage_post_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(object_definition);
	}
	break;
	case _instance_modification_stage_post_scenario_tags_load:
	{
	}
	break;
	}
}

// $TODO create some sort of tag modification manager
void apply_unit_definition_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != UNIT_TAG)
	{
		return;
	}

	struct unit_definition* unit_definition = instance->cast_to<struct unit_definition>();
	const char* tag_name = instance->get_tag_name();
	const char* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_post_tag_load:
	{
	}
	break;
	case _instance_modification_stage_post_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(unit_definition);
	}
	break;
	case _instance_modification_stage_post_scenario_tags_load:
	{
	}
	break;
	}
}

// $TODO create some sort of tag modification manager
void apply_biped_definition_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != BIPED_TAG)
	{
		return;
	}

	struct biped_definition* biped_definition = instance->cast_to<struct biped_definition>();
	const char* tag_name = instance->get_tag_name();
	const char* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_post_tag_load:
	{
	}
	break;
	case _instance_modification_stage_post_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(biped_definition);
	}
	break;
	case _instance_modification_stage_post_scenario_tags_load:
	{
		// "edge drop" fix
		biped_definition->biped.physics.ground.scale_ground_adhesion_velocity = 30.0f / 60;

		//void __cdecl biped_initialize_character_physics_update_input(int32, s_character_physics_update_input_datum* physics_input, bool, bool, real32, bool, bool)
		//{
		//	if (biped_definition->biped.physics.ground.scale_ground_adhesion_velocity > 0.0f)
		//	{
		//		physics_input->ground.adhesion_velocity_scale = biped_definition->biped.physics.ground.scale_ground_adhesion_velocity;
		//	}
		//}
	}
	break;
	}
}

// $TODO create some sort of tag modification manager
void apply_vehicle_definition_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != VEHICLE_TAG)
	{
		return;
	}

	struct vehicle_definition* vehicle_definition = instance->cast_to<struct vehicle_definition>();
	const char* tag_name = instance->get_tag_name();
	const char* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_post_tag_load:
	{
	}
	break;
	case _instance_modification_stage_post_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(vehicle_definition);
	}
	break;
	case _instance_modification_stage_post_scenario_tags_load:
	{
	}
	break;
	}
}

// $TODO create some sort of tag modification manager
void apply_item_definition_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != ITEM_TAG)
	{
		return;
	}

	struct item_definition* item_definition = instance->cast_to<struct item_definition>();
	const char* tag_name = instance->get_tag_name();
	const char* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_post_tag_load:
	{
	}
	break;
	case _instance_modification_stage_post_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(item_definition);
	}
	break;
	case _instance_modification_stage_post_scenario_tags_load:
	{
	}
	break;
	}
}

// $TODO create some sort of tag modification manager
void apply_equipment_definition_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != EQUIPMENT_TAG)
	{
		return;
	}

	struct equipment_definition* equipment_definition = instance->cast_to<struct equipment_definition>();
	const char* tag_name = instance->get_tag_name();
	const char* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_post_tag_load:
	{
	}
	break;
	case _instance_modification_stage_post_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(equipment_definition);
	}
	break;
	case _instance_modification_stage_post_scenario_tags_load:
	{
	}
	break;
	}
}

// $TODO create some sort of tag modification manager
void apply_weapon_definition_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != WEAPON_TAG)
	{
		return;
	}

	struct weapon_definition* weapon_definition = instance->cast_to<struct weapon_definition>();
	const char* tag_name = instance->get_tag_name();
	const char* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_post_tag_load:
	{
	}
	break;
	case _instance_modification_stage_post_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(weapon_definition);
	}
	break;
	case _instance_modification_stage_post_scenario_tags_load:
	{
	}
	break;
	}
}

// $TODO create some sort of tag modification manager
void apply_projectile_definition_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != PROJECTILE_TAG)
	{
		return;
	}

	struct projectile_definition* projectile_definition = instance->cast_to<struct projectile_definition>();
	const char* tag_name = instance->get_tag_name();
	const char* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_post_tag_load:
	{
	}
	break;
	case _instance_modification_stage_post_tag_fixup:
	{
		if (print_reference_updates) c_console::write_line("%s.%s", tag_name, group_tag_name);
		UPDATE_STRUCT_POINTER_REFERENCE_NAMES(projectile_definition);
	}
	break;
	case _instance_modification_stage_post_scenario_tags_load:
	{
	}
	break;
	}
}

// $TODO create some sort of tag modification manager
void apply_multilingual_unicode_string_list_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != MULTILINGUAL_UNICODE_STRING_LIST_TAG)
	{
		return;
	}

	s_multilingual_unicode_string_list_group_header* multilingual_unicode_string_list = instance->cast_to<s_multilingual_unicode_string_list_group_header>();
	const char* tag_name = instance->get_tag_name();
	const char* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_post_tag_load:
	{
	}
	break;
	case _instance_modification_stage_post_tag_fixup:
	{
		//if (csstrcmp("ui\\halox\\main_menu\\strings", tag_name) == 0)
		{
			for (int32 block_index = 0; block_index < multilingual_unicode_string_list->string_references.count; block_index++)
			{
				s_multilingual_unicode_string_reference& string_reference = multilingual_unicode_string_list->string_references[block_index];
				if (string_reference.id == STRING_ID(global, leave_game))
				{
					static char x_english_string[] = "QUIT TO DESKTOP";
					char* string = (char*)tag_data_get_pointer(&multilingual_unicode_string_list->text_data, string_reference.offset[_language_english], sizeof(x_english_string));
					uns32 string_length = csstrnlen(string, sizeof(x_english_string));
					if (string_length + 1 >= sizeof(x_english_string))
					{
						csstrnzcpy(string, x_english_string, string_length + 1);
					}
					break;
				}
			}
		}
	}
	break;
	case _instance_modification_stage_post_scenario_tags_load:
	{
	}
	break;
	}
}

// $TODO create some sort of tag modification manager
void apply_gui_datasource_definition_tag_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != GUI_DATASOURCE_DEFINITION_TAG)
	{
		return;
	}

	s_datasource_definition* datasource_definition = instance->cast_to<s_datasource_definition>();
	const char* tag_name = instance->get_tag_name();
	const char* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_post_tag_load:
	{
	}
	break;
	case _instance_modification_stage_post_tag_fixup:
	{
		//if (csstrcmp("ui\\halox\\director\\saved_film_control_buttons", tag_name) == 0)
		//if (csstrcmp("ui\\halox\\director\\screenshot_option_list", tag_name) == 0)
		{
			for (int32 block_index = 0; block_index < datasource_definition->elements.count; block_index++)
			{
				s_datasource_block& datasource = datasource_definition->elements[block_index];
				for (int32 string_id_nugget_index = 0; string_id_nugget_index < datasource.string_id_nuggets.count; string_id_nugget_index++)
				{
					s_string_id_data_nugget& string_id_nugget = datasource.string_id_nuggets[string_id_nugget_index];

					// somewhere in the dev cycle this was changed/broken, let's change it back
					if (string_id_nugget.name == STRING_ID(global, item))
					{
						string_id_nugget.name = STRING_ID(gui, gui_item);
					}
				}
			}
		}
	}
	break;
	case _instance_modification_stage_post_scenario_tags_load:
	{
	}
	break;
	}
}

// $TODO create some sort of tag modification manager
void apply_gui_skin_definition_tag_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != GUI_SKIN_DEFINITION_TAG)
	{
		return;
	}

	s_gui_skin_definition* gui_skin_definition = instance->cast_to<s_gui_skin_definition>();
	const char* tag_name = instance->get_tag_name();
	const char* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_post_tag_load:
	{
	}
	break;
	case _instance_modification_stage_post_tag_fixup:
	{
		//if (csstrcmp("ui\\halox\\director\\screenshot_options_list", tag_name) == 0)
		{
			for (int32 block_index = 0; block_index < gui_skin_definition->text_blocks.count; block_index++)
			{
				s_text_widget_block& text_block = gui_skin_definition->text_blocks[block_index];

				// somewhere in the dev cycle this was changed/broken, let's change it back
				if (text_block.override_definition.widget_identifier == STRING_ID(global, item))
				{
					text_block.override_definition.widget_identifier = STRING_ID(gui, gui_item);
				}

				if (text_block.override_definition.value_identifier == STRING_ID(global, item))
				{
					text_block.override_definition.value_identifier = STRING_ID(gui, gui_item);
				}
			}
		}
	}
	break;
	case _instance_modification_stage_post_scenario_tags_load:
	{
	}
	break;
	}
}

// $TODO create some sort of tag modification manager
void apply_camera_fx_settings_instance_modification(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	ASSERT(instance != nullptr);

	if (instance->tag_group != CAMERA_FX_SETTINGS_TAG)
	{
		return;
	}

	c_camera_fx_settings* camera_fx_settings = instance->cast_to<c_camera_fx_settings>();
	const char* tag_name = instance->get_tag_name();
	const char* group_tag_name = instance->tag_group.name.get_string();

	switch (stage)
	{
	case _instance_modification_stage_post_tag_load:
	{
	}
	break;
	case _instance_modification_stage_post_tag_fixup:
	{
		if (csstrstr(shell_get_command_line(), "-disable-auto-exposure") != 0)
		{
			SET_BIT(camera_fx_settings->m_exposure.m_flags, c_camera_fx_settings::_parameter_auto_bit, false);
		}
	}
	break;
	case _instance_modification_stage_post_scenario_tags_load:
	{
	}
	break;
	}
}

// $TODO create some sort of tag modification manager
void tag_instance_modification_apply(cache_file_tag_instance* instance, e_instance_modification_stage stage)
{
	if (instance == nullptr)
	{
		return;
	}

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
	APPLY_INSTANCE_MODIFICATION(weapon_definition);
	APPLY_INSTANCE_MODIFICATION(projectile_definition);
	APPLY_INSTANCE_MODIFICATION(multilingual_unicode_string_list);
	APPLY_INSTANCE_MODIFICATION(gui_datasource_definition_tag);
	APPLY_INSTANCE_MODIFICATION(gui_skin_definition_tag);
	APPLY_INSTANCE_MODIFICATION(camera_fx_settings);

#undef APPLY_INSTANCE_MODIFICATION
}

