#include "cache/cache_files.hpp"

#include "cache/cache_file_builder_security.hpp"
#include "cache/security_functions.hpp"
#include "cseries/cseries_console.hpp"
#include "cseries/cseries_windows.hpp"
#include "game/multiplayer_definitions.hpp"
#include "memory/crc.hpp"
#include "memory/module.hpp"
#include "scenario/scenario_definitions.hpp"
#include "tag_files/string_ids.hpp"

#include <string.h>

HOOK_DECLARE(0x00502210, cache_files_verify_header_rsa_signature);
HOOK_DECLARE(0x00502780, cache_file_tags_load);
HOOK_DECLARE(0x005031A0, cache_file_tags_fixup_all_instances);
HOOK_DECLARE(0x00503370, tag_get);
HOOK_DECLARE(0x00503470, sub_503470);

REFERENCE_DECLARE(0x022AAFE8, s_cache_file_globals, g_cache_file_globals);

struct s_cache_file_global_tags_definition
{
	c_typed_tag_block<s_tag_reference> references;
	dword : 32;
};
static_assert(sizeof(s_cache_file_global_tags_definition) == 0x10);

char const* tag_get_name(long tag_name_index)
{
	ASSERT(g_cache_file_globals.tags_loaded);
	ASSERT(tag_name_index >= 0 && tag_name_index < g_cache_file_globals.header.debug_tag_name_count);

	long tag_name_offset = g_cache_file_globals.debug_tag_names->offsets[tag_name_index];
	ASSERT(tag_name_offset >= 0 && tag_name_offset < sizeof(g_cache_file_globals.debug_tag_names->buffer));

	return &g_cache_file_globals.debug_tag_names->buffer[tag_name_offset];
}

char const* tag_get_name_safe(long tag_name_index)
{
	ASSERT(g_cache_file_globals.tags_loaded);

	if (tag_name_index < g_cache_file_globals.header.debug_tag_name_count)
	{
		long tag_name_offset = g_cache_file_globals.debug_tag_names->offsets[tag_name_index];
		if (tag_name_offset >= 0 && tag_name_offset < sizeof(g_cache_file_globals.debug_tag_names->buffer))
			return &g_cache_file_globals.debug_tag_names->buffer[tag_name_offset];
	}

	return nullptr;
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

	//s_cache_file_global_tags_definition* global_tags = tag_get<s_cache_file_global_tags_definition>('cfgt', 0);
	//if (!global_tags)
	//	return -1;
	//for (long i = 0; i < global_tags->references.count; i++)
	//	if (group_tag == global_tags->references.address[i].group_tag)
	//		return global_tags->references.address[i].index;
	//return -1;
}

void __cdecl cache_file_get_path(char const* mapname, char* buffer, long buffer_size)
{
	INVOKE(0x005018C0, cache_file_get_path, mapname, buffer, buffer_size);
}

s_cache_file_security_globals* __cdecl cache_file_get_security_globals()
{
	return INVOKE(0x005018F0, cache_file_get_security_globals);
}

void const* __cdecl cache_file_globals_get_tag_cache_base_address()
{
	return INVOKE(0x00501930, cache_file_globals_get_tag_cache_base_address);
}

bool __cdecl cache_file_header_verify(s_cache_file_header const* header, char const* scenario_path, bool fail_fatally)
{
	return INVOKE(0x00501950, cache_file_header_verify, header, scenario_path, fail_fatally);
}

bool __cdecl cache_file_header_verify_and_version(s_cache_file_header const* header, char const* scenario_path, bool fail_fatally)
{
	return INVOKE(0x00501AD0, cache_file_header_verify_and_version, header, scenario_path, fail_fatally);
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
	return INVOKE(0x00501FC0, cache_files_map_directory);
}

const bool override_cache_file_header_security_validate_hash = true;
const bool override_cache_file_header_security_rsa_compute_and_verify_signature = true;

bool __cdecl cache_files_verify_header_rsa_signature(s_cache_file_header* header)
{
	//bool result = false;
	//HOOK_INVOKE(result =, cache_files_verify_header_rsa_signature, header);
	//return result;

	if (header->header_signature != 'head' || header->footer_signature != 'foot')
	{
		display_debug_string("cache_files:header is invalid, header/footer signatures are bad");
		return false;
	}

	s_cache_file_header clean_header{};
	memmove(&clean_header, header, sizeof(s_cache_file_header));
	cache_file_builder_security_clean_header(&clean_header);

	static char hash_string[4096]{};
	s_network_http_request_hash hash{};
	if (!security_validate_hash(&clean_header, sizeof(s_cache_file_header), true, &header->hash, &hash))
	{
		if (!override_cache_file_header_security_validate_hash)
		{
			display_debug_string("cache_files:header failed hash verification - possible cheating?");
			return false;
		}

		type_as_byte_string(&hash, hash_string);
		display_debug_string("cache_files:header: failed hash verification - copying new validated values, %s", hash_string);

		memcpy(&header->hash, &hash, sizeof(s_network_http_request_hash));
	}

	//type_as_byte_string(&hash, hash_string);

	security_calculate_hash(&header->hash, sizeof(s_network_http_request_hash), true, &hash);

	s_rsa_signature rsa_signature{};
	memcpy(&rsa_signature, &header->rsa_signature, sizeof(s_rsa_signature));

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

bool __cdecl scenario_tags_load(char const* scenario_path)
{
	return INVOKE(0x00502DC0, scenario_tags_load, scenario_path);
}

void __cdecl scenario_tags_load_finished()
{
	// nullsub
	INVOKE(0x00503190, scenario_tags_load_finished);
}

void __cdecl scenario_tags_unload()
{
	INVOKE(0x00503200, scenario_tags_unload);
}

void __cdecl tag_files_close()
{
	INVOKE(0x00503300, tag_files_close);
}

void __cdecl tag_files_open()
{
	INVOKE(0x00503340, tag_files_open);
}

void* __cdecl tag_get(tag group_tag, long tag_index)
{
	long tag_absolute_index = g_cache_file_globals.tag_index_absolute_mapping[tag_index];
	if (tag_absolute_index == -1)
		return nullptr;

	void* data = g_cache_file_globals.tag_instances[tag_absolute_index]->base + g_cache_file_globals.tag_instances[tag_absolute_index]->offset;

	return data;
}

// 005033A0 //dword tag_get_group_tag(long)

void __cdecl tag_iterator_new(tag_iterator* iterator, tag group_tag)
{
	INVOKE(0x005033E0, tag_iterator_new, iterator, group_tag);
}

long __cdecl tag_iterator_next(tag_iterator* iterator)
{
	return INVOKE(0x00503400, tag_iterator_next, iterator);
}

void add_missing_weapon_selections(cache_file_tag_instance* instance, bool load_tag)
{
	if (instance == nullptr || !instance->is_group('mulg'))
		return;

	// Add back missing weapon selections
	if (load_tag)
	{
		cache_file_tags_load(0x00001500); // spike_rifle
		cache_file_tags_load(0x0000159E); // sword
		cache_file_tags_load(0x000014F8); // needler
		cache_file_tags_load(0x000015B3); // rocket_launcher
		cache_file_tags_load(0x00001A45); // shotgun
		cache_file_tags_load(0x000015B1); // sniper_rifle
		cache_file_tags_load(0x000014FF); // brute_shot
		cache_file_tags_load(0x00001509); // beam_rifle
		cache_file_tags_load(0x000015B2); // spartan_laser
		cache_file_tags_load(0x0000150C); // gravity_hammer
		cache_file_tags_load(0x00001A55); // flame_thrower
		cache_file_tags_load(0x00001A54); // missile_launcher
	}
	else
	{
		s_multiplayer_globals_definition* multiplayer_globals = reinterpret_cast<s_multiplayer_globals_definition*>(instance->base + instance->offset);
		if (multiplayer_globals == nullptr || multiplayer_globals->universal.count() <= 0 || multiplayer_globals->universal[0].weapon_selections.count() <= 0)
			return;

		for (s_multiplayer_weapon_selection& weapon_selection : multiplayer_globals->universal[0].weapon_selections)
		{
			if (weapon_selection.weapon_tag.index != 0xFFFFFFFF)
				continue;

			switch (weapon_selection.name.get_value())
			{
			case STRING_ID(global, spike_rifle):
				weapon_selection.weapon_tag.group_tag = 'weap';
				weapon_selection.weapon_tag.index = 0x00001500;
				break;
			case STRING_ID(global, sword):
				weapon_selection.weapon_tag.group_tag = 'weap';
				weapon_selection.weapon_tag.index = 0x0000159E;
				break;
			case STRING_ID(global, needler):
				weapon_selection.weapon_tag.group_tag = 'weap';
				weapon_selection.weapon_tag.index = 0x000014F8;
				break;
			case STRING_ID(global, rocket_launcher):
				weapon_selection.weapon_tag.group_tag = 'weap';
				weapon_selection.weapon_tag.index = 0x000015B3;
				break;
			case STRING_ID(global, shotgun):
				weapon_selection.weapon_tag.group_tag = 'weap';
				weapon_selection.weapon_tag.index = 0x00001A45;
				break;
			case STRING_ID(global, sniper_rifle):
				weapon_selection.weapon_tag.group_tag = 'weap';
				weapon_selection.weapon_tag.index = 0x000015B1;
				break;
			case STRING_ID(global, brute_shot):
				weapon_selection.weapon_tag.group_tag = 'weap';
				weapon_selection.weapon_tag.index = 0x000014FF;
				break;
			case STRING_ID(global, beam_rifle):
				weapon_selection.weapon_tag.group_tag = 'weap';
				weapon_selection.weapon_tag.index = 0x00001509;
				break;
			case STRING_ID(global, spartan_laser):
				weapon_selection.weapon_tag.group_tag = 'weap';
				weapon_selection.weapon_tag.index = 0x000015B2;
				break;
			case STRING_ID(global, gravity_hammer):
				weapon_selection.weapon_tag.group_tag = 'weap';
				weapon_selection.weapon_tag.index = 0x0000150C;
				break;
			case STRING_ID(global, flame_thrower):
				weapon_selection.weapon_tag.group_tag = 'weap';
				weapon_selection.weapon_tag.index = 0x00001A55;
				break;
			case STRING_ID(global, missile_launcher):
				weapon_selection.weapon_tag.group_tag = 'weap';
				weapon_selection.weapon_tag.index = 0x00001A54;
				break;
			}
		}
	}
}

bool __cdecl cache_file_tags_load(dword tag_index)
{
	cache_file_tag_instance* instance = reinterpret_cast<cache_file_tag_instance*>(g_cache_file_globals.tag_cache_base_address + g_cache_file_globals.tag_loaded_size);
	dword& absolute_index = g_cache_file_globals.tag_loaded_count;
	dword tag_cache_offset = g_cache_file_globals.tag_cache_offsets[tag_index];

	if (g_cache_file_globals.tag_index_absolute_mapping[tag_index] != -1)
		return true;

	if (!file_read_from_position(&g_cache_file_globals.tags_header, tag_cache_offset, sizeof(cache_file_tag_instance), false, instance))
		return false;

	g_cache_file_globals.tag_loaded_size += instance->total_size;
	g_cache_file_globals.tag_instances[absolute_index] = instance;
	g_cache_file_globals.tag_index_absolute_mapping[tag_index] = absolute_index;
	g_cache_file_globals.absolute_index_tag_mapping[absolute_index] = tag_index;

	if (!file_read_from_position(&g_cache_file_globals.tags_header, tag_cache_offset, instance->total_size, false, instance))
		return false;

	if (crc_checksum_buffer_adler32(adler_new(), instance->base + sizeof(instance->checksum), instance->total_size - sizeof(instance->checksum)) != instance->checksum)
		return false;

	g_cache_file_globals.tag_loaded_count++;

	if (instance->dependency_count <= 0)
		return true;

	if (instance->is_group('mulg'))
		add_missing_weapon_selections(instance, true);
	
	short dependency_index = 0;
	while (cache_file_tags_load(instance->dependencies[dependency_index]))
	{
		if (++dependency_index >= instance->dependency_count)
			return true;
	}

	return false;
}

void __cdecl cache_file_tags_single_tag_instance_fixup(cache_file_tag_instance* instance)
{
	ASSERT(instance);

	cache_address* data_fixups = reinterpret_cast<cache_address*>(instance->dependencies + instance->dependency_count);
	for (short data_fixup_index = 0; data_fixup_index < instance->data_fixup_count; data_fixup_index++)
	{
		cache_address& data_fixup = *reinterpret_cast<cache_address*>(instance->base + data_fixups[data_fixup_index].offset);
		ASSERT(data_fixup.persistent == true);

		data_fixup.offset += (dword)instance->base;
		data_fixup.persistent = false;
		ASSERT(data_fixup.value == data_fixup.offset);
	}

	if (instance->is_group('mulg'))
		add_missing_weapon_selections(instance, false);
}

void __cdecl cache_file_tags_fixup_all_instances()
{
	for (dword i = 0; i < g_cache_file_globals.tag_loaded_count; i++)
	{
		cache_file_tag_instance* instance = g_cache_file_globals.tag_instances[i];
		cache_file_tags_single_tag_instance_fixup(instance);
	}
}

// __thiscall
void __fastcall sub_503470(s_cache_file_reports* reports, void* unused, cache_file_tag_instance* instance, dword tag_index)
{
	ASSERT(instance);

	if (instance->dependency_count || instance->data_fixup_count || instance->resource_fixup_count)
		return;

	c_console::write_line("0x%08X.%s", tag_index, instance->group_name.get_string());
	static char tag_instance_byte_string[sizeof(cache_file_tag_instance) * 3]{};
	type_as_byte_string(instance, tag_instance_byte_string);
	c_console::write_line(tag_instance_byte_string);
}

bool cache_file_tags_single_tag_file_load(s_file_reference* file, cache_file_tag_instance** out_instance)
{
	cache_file_tag_instance* instance = reinterpret_cast<cache_file_tag_instance*>(g_cache_file_globals.tag_cache_base_address + g_cache_file_globals.tag_loaded_size);
	dword& absolute_index = g_cache_file_globals.tag_loaded_count;
	dword tag_cache_offset = 0;

	if (out_instance)
		*out_instance = instance;

	dword tag_index = 0;
	while (g_cache_file_globals.tag_index_absolute_mapping[tag_index] != -1)
		tag_index++;

	if (!file_read_from_position(&g_cache_file_globals.tags_header, tag_cache_offset, sizeof(cache_file_tag_instance), false, instance))
		return false;

	g_cache_file_globals.tag_loaded_size += instance->total_size;
	g_cache_file_globals.tag_instances[absolute_index] = instance;
	g_cache_file_globals.tag_index_absolute_mapping[tag_index] = absolute_index;
	g_cache_file_globals.absolute_index_tag_mapping[absolute_index] = tag_index;

	if (!file_read_from_position(&g_cache_file_globals.tags_header, tag_cache_offset, instance->total_size, false, instance))
		return false;

	if (crc_checksum_buffer_adler32(adler_new(), instance->base + sizeof(instance->checksum), instance->total_size - sizeof(instance->checksum)) != instance->checksum)
		return false;

	g_cache_file_globals.tag_loaded_count++;

	if (instance->dependency_count <= 0)
		return true;

	short dependency_index = 0;
	while (cache_file_tags_load(instance->dependencies[dependency_index]))
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

	file_reference_create_from_path(&file, file_name, false);
	if (file_open(&file, FLAG(_file_open_flag_desired_access_read), &error)/* && error == 0*/)
	{
		cache_file_tag_instance* instance = nullptr;
		if (cache_file_tags_single_tag_file_load(&file, &instance) && instance)
			cache_file_tags_single_tag_instance_fixup(instance);
	}
	file_close(&file);
}

