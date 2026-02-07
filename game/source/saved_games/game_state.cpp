#include "saved_games/game_state.hpp"

#include "cache/cache_files.hpp"
#include "cache/restricted_memory.hpp"
#include "cache/restricted_memory_regions.hpp"
#include "config/version.hpp"
#include "cseries/cseries_events.hpp"
#include "cseries/runtime_state.hpp"
#include "game/game.hpp"
#include "game/game_save.hpp"
#include "interface/user_interface_session.hpp"
#include "main/console.hpp"
#include "main/main.hpp"
#include "memory/crc.hpp"
#include "memory/module.hpp"
#include "multithreading/synchronization.hpp"
#include "saved_games/game_state_pc.hpp"
#include "saved_games/game_state_procs.hpp"
#include "saved_games/saved_film_manager.hpp"
#include "simulation/simulation.hpp"
#include "tag_files/files.hpp"

#include <string.h>

REFERENCE_DECLARE(0x0189D468, c_game_state_compressor_callback, g_game_state_compressor_optional_cache_callback);
REFERENCE_DECLARE(0x02344148, s_game_state_globals, game_state_globals);

HOOK_DECLARE(0x00510110, game_state_read_core);
HOOK_DECLARE(0x00510FB0, game_state_write_core);

_game_state_test_option s_game_state_globals::game_state_test_option = GAME_STATE_TEST_OPTION_DEFAULT;

c_gamestate_deterministic_allocation_callbacks g_gamestate_deterministic_allocation_callbacks{};
c_gamestate_nondeterministic_allocation_callbacks g_gamestate_nondeterministic_allocation_callbacks{};
c_gamestate_allocation_record_allocation_callbacks g_gamestate_allocation_record_allocation_callbacks{};

s_file_reference g_game_state_allocation_file_reference{};
bool g_game_state_allocation_file_reference_valid = false;

void __cdecl restricted_region_create_for_render_initialize(int32 index, c_restricted_section* primary_section, int32 critical_section_index, c_restricted_memory_callbacks* callbacks)
{
	ASSERT(callbacks == nullptr);
	callbacks = &g_gamestate_allocation_record_allocation_callbacks;

	restricted_region_create(index, primary_section, critical_section_index, callbacks);
}
HOOK_DECLARE_CALL(0x00A29936, restricted_region_create_for_render_initialize);

//.text:0050EEA0 ; c_game_state_compressor_callback::c_game_state_compressor_callback

//.text:0050EEB0 ; c_gamestate_deterministic_allocation_callbacks::c_gamestate_deterministic_allocation_callbacks
c_gamestate_deterministic_allocation_callbacks::c_gamestate_deterministic_allocation_callbacks() :
	c_restricted_memory_callbacks()
{
}

//.text:0050EEC0 ; c_gamestate_nondeterministic_allocation_callbacks::c_gamestate_nondeterministic_allocation_callbacks
c_gamestate_nondeterministic_allocation_callbacks::c_gamestate_nondeterministic_allocation_callbacks() :
	c_restricted_memory_callbacks()
{
}

c_gamestate_allocation_record_allocation_callbacks::c_gamestate_allocation_record_allocation_callbacks() :
	c_restricted_memory_callbacks()
{
}

//.text:0050EED0 ; c_optional_cache_user_callback::c_optional_cache_user_callback

//.text:0050EEE0 ; c_restricted_memory_callbacks::c_restricted_memory_callbacks
c_restricted_memory_callbacks::c_restricted_memory_callbacks()
{
}

//.text:0050EEF0
//.text:0050EF10
//.text:0050EF70
//.text:0050EF80
//.text:0050EFA0
//.text:0050EFC0
//.text:0050EFE0 ; c_game_state_compressor::allocate_buffer
//.text:0050F020 ; c_game_state_compressor::compress_internal

bool __cdecl create_file_from_buffer(const char* file_name, const char* file_contents)
{
	return INVOKE(0x0050F0C0, create_file_from_buffer, file_name, file_contents);
}

//.text:0050F170 ; c_game_state_compressor::deallocate_buffer
//.text:0050F1A0 ; c_game_state_compressor::dispose

//.text:0050F220 ; c_gamestate_deterministic_allocation_callbacks::filter_base_offset
int32 c_gamestate_deterministic_allocation_callbacks::filter_base_offset(int32 base_offset, int32 total_allocation_space)
{
	return base_offset;
}

//.text:0050F230 ; c_gamestate_nondeterministic_allocation_callbacks::filter_base_offset
int32 c_gamestate_nondeterministic_allocation_callbacks::filter_base_offset(int32 base_offset)
{
	return base_offset;
}

//.text:0050F240 ; c_restricted_memory_callbacks::filter_base_offset
int32 c_restricted_memory_callbacks::filter_base_offset(int32 base_offset)
{
	return base_offset;
}

//.text:0050F250 ; c_gamestate_deterministic_allocation_callbacks::filter_size_request
unsigned int c_gamestate_deterministic_allocation_callbacks::filter_size_request(unsigned int in_size)
{
	return (in_size + 3) & ~3;
}

//.text:0050F260 ; c_gamestate_nondeterministic_allocation_callbacks::filter_size_request
unsigned int c_gamestate_nondeterministic_allocation_callbacks::filter_size_request(unsigned int in_size)
{
	return (in_size + 3) & ~3;
}

//.text:0050F270 ; c_restricted_memory_callbacks::filter_size_request
unsigned int c_restricted_memory_callbacks::filter_size_request(unsigned int in_size)
{
	return in_size;
}

void __cdecl game_state_allocation_record(int32 region_index, const char* name, const char* type, int32 allocation_size)
{
	if (g_game_state_allocation_file_reference_valid)
	{
		file_printf(&g_game_state_allocation_file_reference, "% 44s,% 24s", name, type);

		for (int32 i = 0; i < k_total_restricted_memory_regions; i++)
		{
			file_printf(&g_game_state_allocation_file_reference, ",% 40d", region_index == i ? allocation_size : 0);
		}
	}

	file_printf(&g_game_state_allocation_file_reference, "\r\n");
}

void __cdecl game_state_buffer_handle_read()
{
	INVOKE(0x0050F280, game_state_buffer_handle_read);
}

bool __cdecl game_state_can_save_to_persistent_storage()
{
	return INVOKE(0x0050F2A0, game_state_can_save_to_persistent_storage);
}

bool __cdecl game_state_compressor_lock_pending()
{
	return INVOKE(0x0050F2D0, game_state_compressor_lock_pending);
}

void __cdecl game_state_compressor_lock_update()
{
	INVOKE(0x0050F2E0, game_state_compressor_lock_update);
}

bool __cdecl game_state_debug_server_file_uploading_enabled(c_static_string<256>* reason)
{
	//return INVOKE(0x0050F330, game_state_debug_server_file_uploading_enabled, reason);

	bool upload_to_server = true;

	if (reason)
	{
		reason->clear();
	}

	if (game_state_globals.game_state_test_option == GAME_STATE_TEST_OPTION_REPRO)
	{
		if (reason)
		{
			reason->set("Build is a repro build and won't upload.");
		}

		upload_to_server = false;
	}

	if (!version_is_tracked_build() && !g_force_upload_even_if_untracked)
	{
		if (reason)
		{
			reason->set("Untracked build, won't upload.");
		}

		upload_to_server = false;
	}

	if (reason)
	{
		ASSERT(upload_to_server == (reason->length() == 0));
	}

	return upload_to_server;
}

//.text:0050F370 ; c_game_state_compressor::game_state_decompress_buffer

void __cdecl game_state_dispose()
{
	INVOKE(0x0050F5D0, game_state_dispose);
}

void __cdecl game_state_dispose_from_old_map()
{
	INVOKE(0x0050F5E0, game_state_dispose_from_old_map);
}

void __cdecl game_state_dispose_from_old_non_bsp_zone_set(const s_game_non_bsp_zone_set* old_non_bsp_zone_set)
{
	INVOKE(0x0050F5F0, game_state_dispose_from_old_non_bsp_zone_set, old_non_bsp_zone_set);
}

void __cdecl game_state_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask)
{
	INVOKE(0x0050F600, game_state_dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);
}

const void* __cdecl game_state_get_buffer_address(int32* buffer_size)
{
	return INVOKE(0x0050F610, game_state_get_buffer_address, buffer_size);
}

//.text:0050F630 ; c_game_state_compressor::game_state_get_compressed

void game_state_get_core_file_reference(const char* core_name, s_file_reference* file)
{
	ASSERT(core_name);
	ASSERT(file);

	c_static_string<1024> filename;

	if (string_is_absolute_path(core_name))
		filename.print("%s.bin", core_name);
	else
		filename.print("core\\%s.bin", core_name);

	file_reference_create_from_path(file, filename.get_string(), false);
}

//.text:0050F6A0 ; c_game_state_compressor::game_state_get_storage
//.text:0050F6C0 ; c_game_state_compressor::game_state_get_compressed_checksum
//.text:0050F6E0 ; c_game_state_compressor::game_state_get_compressed_from_buffer
//.text:0050F710 ; c_game_state_compressor::game_state_get_compressed_size

c_game_state_compressor* __cdecl game_state_get_compressor()
{
	return INVOKE(0x0050F720, game_state_get_compressor);
}

bool __cdecl game_state_get_game_options_from_core(const char* core_name, game_options* options)
{
	return INVOKE(0x0050F740, game_state_get_game_options_from_core, core_name, options);
}

bool __cdecl game_state_get_scenario_path_from_core(const char* core_name, char* buffer, int32 buffer_length)
{
	return INVOKE(0x0050F890, game_state_get_scenario_path_from_core, core_name, buffer, buffer_length);
}

const char* const k_game_state_test_option_description[k_game_state_test_option_count]
{
	"default",
	"repro",
	"stress"
};

const char* __cdecl game_state_get_test_option_string()
{
	//return INVOKE(0x0050F9E0, game_state_get_test_option_string);

	return k_game_state_test_option_description[game_state_globals.game_state_test_option];
}

bool __cdecl game_state_header_matches(const game_state_header* current_header, const game_state_header* file_header)
{
	return INVOKE(0x0050F9F0, game_state_header_matches, current_header, file_header);
}

bool __cdecl game_state_header_matches_current_network_session_settings(const game_state_header* header)
{
	return INVOKE(0x0050FAA0, game_state_header_matches_current_network_session_settings, header);

	//e_campaign_id campaign_id = _campaign_id_none;
	//e_map_id map_id = _map_id_none;
	//if (!user_interface_session_get_map(&campaign_id, &map_id))
	//{
	//	event(_event_warning, "game_state: failed to retrieve session campaign settings");
	//	return false;
	//}
	//
	//if (campaign_id != header->options.campaign_id)
	//{
	//	event(_event_warning, "game_state: game state campaign_id does not match session campaign_id");
	//	return false;
	//}
	//
	//if (map_id != header->options.map_id)
	//{
	//	event(_event_warning, "game_state: game state map_id does not match session map_id");
	//	return false;
	//}
	//
	//if (user_interface_game_settings_get_campaign_difficulty() == header->options.campaign_difficulty)
	//{
	//	event(_event_warning, "game_state: game state difficulty does not match session difficulty");
	//	return false;
	//}
	//
	//if (user_interface_game_settings_get_campaign_insertion_point() == header->options.campaign_insertion_point)
	//{
	//	event(_event_warning, "game_state: game state insertion_point does not match session insertion_point");
	//	return false;
	//}
	//
	//if (user_interface_game_settings_get_campaign_active_primary_skulls() == header->options.campaign_active_primary_skulls)
	//{
	//	event(_event_warning, "game_state: game state primary_skulls does not match session primary_skulls");
	//	return false;
	//}
	//
	//if (user_interface_game_settings_get_campaign_active_secondary_skulls() == header->options.campaign_active_secondary_skulls)
	//{
	//	event(_event_warning, "game_state: game state secondary_skulls does not match session secondary_skulls");
	//	return false;
	//}
	//
	//event(_event_message, "game_state: game state options match session options - safe to proceed");
	//return true;
}

bool __cdecl game_state_header_prepare_to_load(game_state_header* header, bool allow_networking, bool allow_change_maps, bool* out_changed_maps)
{
	return INVOKE(0x0050FBC0, game_state_header_prepare_to_load, header, allow_networking, allow_change_maps, out_changed_maps);
}

bool __cdecl game_state_header_valid(const game_state_header* header)
{
	return INVOKE(0x0050FC70, game_state_header_valid, header);
}

bool __cdecl game_state_header_valid_for_saving(const game_state_header* header)
{
	return INVOKE(0x0050FCF0, game_state_header_valid_for_saving, header);
}

void __cdecl game_state_initialize()
{
	//INVOKE(0x0050FDB0, game_state_initialize);

	ASSERT(game_state_globals.initialized);
}

void __cdecl game_state_initialize_for_new_map()
{
	//INVOKE(0x0050FDC0, game_state_initialize_for_new_map);

	game_state_reset_mapping(k_no_game_state_proc_flags);

	if (g_game_state_allocation_file_reference_valid)
	{
		file_close(&g_game_state_allocation_file_reference);
		g_game_state_allocation_file_reference_valid = false;
	}

	game_state_globals.allocations_locked = true;
	game_state_globals.saved_game_valid = false;
	game_state_globals.fallback_valid = false;
	game_state_globals.current_saved_game_index = 0;
	csmemset(game_state_globals.saved_game_header, 0, sizeof(game_state_globals.saved_game_header));
	game_state_globals.revert_time = NONE;

	csmemset(game_state_globals.header, 0, sizeof(game_state_header));
	scenario_initialize_game_state(&game_state_globals.header->scenario_game_state);
	game_state_globals.header->allocation_size_checksum = game_state_globals.allocation_size_checksum;
	cache_files_get_header()->tag_path.copy_to(game_state_globals.header->map_name, sizeof(game_state_globals.header->map_name));
	csstrnzcpy(game_state_globals.header->build_number, version_get_build_string(), sizeof(game_state_globals.header->build_number));
	csmemcpy(&game_state_globals.header->options, game_options_get(), sizeof(game_options));
	ASSERT(game_state_header_valid(game_state_globals.header));

	game_state_globals.loaded_from_persistent_storage = false;
}

void __cdecl game_state_initialize_for_new_non_bsp_zone_set(const s_game_non_bsp_zone_set* new_non_bsp_zone_set)
{
	//INVOKE(0x0050FEB0, game_state_initialize_for_new_non_bsp_zone_set, new_non_bsp_zone_set);

	scenario_game_state_grab_global_state(&game_state_globals.header->scenario_game_state);
}

void __cdecl game_state_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask)
{
	//INVOKE(0x0050FED0, game_state_initialize_for_new_structure_bsp, activating_structure_bsp_mask);

	scenario_game_state_grab_global_state(&game_state_globals.header->scenario_game_state);
}

bool __cdecl game_state_is_locked()
{
	return INVOKE(0x0050FF00, game_state_is_locked);
}

void __cdecl game_state_load_core(const char* name)
{
	//INVOKE(0x0050FF10, game_state_load_core, name);

	game_state_header header{};
	
	bool changed_maps = false;
	if (!game_state_read_core(name, &header, sizeof(game_state_header)))
	{
		event(_event_warning, "unable to read core '%s'!",
			name);
		return;
	}
	
	if (!game_state_header_valid(&header))
	{
		event(_event_warning, "invalid core '%s'!",
			name);
		return;
	}
	
	game_options_validate(&header.options);
	
	if (!game_state_header_prepare_to_load(&header, false, true, &changed_maps))
	{
		event(_event_warning, "unable to load game/map for core '%s'!",
			name);
		return;
	}
	
	if (!game_state_header_matches(game_state_globals.header, &header))
	{
		event(_event_warning, "unable to load core '%s' due to header mismatch!",
			name);
		return;
	}
	
	game_state_call_before_load_procs(_rebuild_players_flag | _flush_networking_flag);
	bool core_loaded = game_state_read_core(name, game_state_globals.base_address, k_game_state_file_size);
	game_options_clear_game_playback();
	game_options_game_engine_fixup();
	game_state_call_after_load_procs(_rebuild_players_flag | _flush_networking_flag);
	
	if (!core_loaded)
	{
		event(_event_critical, "core '%s' did not load into memory successfully, we are toast",
			name);
		return;
	}
	
	console_printf("loaded '%s'", name);
	saved_film_manager_notify_gamestate_load(_saved_film_game_state_load_source_core);
	if (game_state_globals.game_state_test_option == GAME_STATE_TEST_OPTION_STRESS)
	{
		game_save_immediate();
	}
}

void __cdecl game_state_lock(bool locked)
{
	INVOKE(0x00510020, game_state_lock, locked);
}

void __cdecl game_state_prepare_for_revert()
{
	INVOKE(0x00510040, game_state_prepare_for_revert);
}

void __cdecl game_state_preserve()
{
	INVOKE(0x00510100, game_state_preserve);
}

bool __cdecl game_state_read_core(const char* core_name, void* buffer, uns32 buffer_length)
{
	//return INVOKE(0x00510110, game_state_read_core, core_name, buffer, buffer_length);

	if (buffer == game_state_globals.base_address)
	{
		ASSERT(game_state_proc_globals_get_state() == _game_state_proc_state_loading);
	}

	s_file_reference core_file{};
	game_state_get_core_file_reference(core_name, &core_file);

	bool success = false;

	uns32 error = 0;
	if (file_open(&core_file, FLAG(_permission_read_bit), &error))
	{
		success = file_read(&core_file, buffer_length, false, buffer);
		file_close(&core_file);

		if (buffer == game_state_globals.base_address)
		{
			success = success && game_state_security_verify_signature_insecure(nullptr);
			if (!success)
			{
				event(_event_critical, "game_state: core '%s' failed signature check", core_name);
			}

			game_state_buffer_handle_read();
		}
	}

	return success;
}

bool __cdecl game_state_read_from_persistent_storage_blocking(e_controller_index controller_index, void* buffer, uns32 buffer_size)
{
	return INVOKE(0x00510270, game_state_read_from_persistent_storage_blocking, controller_index, buffer, buffer_size);
}

//.text:00510280 ; bool __cdecl game_state_read_game_options_from_persistent_storage_blocking(e_controller_index, game_options*)

bool __cdecl game_state_read_header_from_persistent_storage_blocking(e_controller_index controller_index, game_state_header* header)
{
	return INVOKE(0x00510310, game_state_read_header_from_persistent_storage_blocking, controller_index, header);
}

void __cdecl game_state_reset_mapping(int32 game_state_proc_flags)
{
	//INVOKE(0x00510330, game_state_reset_mapping, game_state_proc_flags);

	game_state_set_buffer_protection(game_state_globals.base_address, k_game_state_file_size, game_state_globals.guard_page_size);
}

//.text:00510350 ; void __cdecl game_state_restore_internal(int32)

void __cdecl game_state_revert(uns32 flags)
{
	INVOKE(0x00510360, game_state_revert, flags);
}

bool __cdecl game_state_reverted()
{
	//return INVOKE(0x00510530, game_state_reverted);

	return game_state_globals.revert_time == game_time_get();
}

void __cdecl game_state_save()
{
	INVOKE(0x00510550, game_state_save);
}

void __cdecl game_state_save_core(const char* name)
{
	//INVOKE(0x005105F0, game_state_save_core, name);

	int32 game_state_proc_flags = k_no_game_state_proc_flags;

	if ((game_is_server() || game_is_distributed()) && !game_is_playback())
	{
		simulation_notify_core_save();
	}
	else
	{
		game_state_proc_flags = _dont_rebuild_havok_flag | _dont_cancel_pending_game_save_flag | _doesnt_effect_location_after_death_flag;
	}

	game_state_call_before_save_procs(game_state_proc_flags);

	bool success = game_state_write_core(name, game_state_globals.base_address, k_game_state_allocation_size);
	console_printf(success ? "saved '%s'" : "error writing '%s'", name);

	game_state_call_after_save_procs(game_state_proc_flags);

	//if (success)
	//{
	//	game_state_upload_to_debug_server(name);
	//}
	//bug_notify_core_creation(name, success);
}

void __cdecl game_state_save_to_persistent_storage_blocking()
{
	INVOKE(0x00510650, game_state_save_to_persistent_storage_blocking);

	//if (!game_is_campaign() || game_is_or_was_cooperative())
	//{
	//	return;
	//}
	//
	//game_state_revert(k_no_game_state_revert_flags);
	//game_state_call_before_save_procs(k_no_game_state_proc_flags);
	//
	//event(_event_status, "blocking game state save");
	//
	//game_state_write_to_persistent_storage_blocking(
	//	&game_state_globals.saved_game_header[game_state_globals.current_saved_game_index],
	//	sizeof(game_state_header),
	//	game_state_globals.base_address,
	//	k_game_state_file_size);
	//
	//game_state_call_after_save_procs(k_no_game_state_proc_flags);
}

void __cdecl game_state_security_invalidate_signature()
{
	INVOKE(0x005106B0, game_state_security_invalidate_signature);
}

bool __cdecl game_state_security_verify_signature_insecure(void* header_address)
{
	return INVOKE(0x005106F0, game_state_security_verify_signature_insecure, header_address);
}

bool __cdecl game_state_security_verify_signature_internal(void* header_address, bool insecure)
{
	return INVOKE(0x00510710, game_state_security_verify_signature_internal, header_address, insecure);
}

bool __cdecl game_state_security_verify_signature_secure(void* header_address)
{
	return INVOKE(0x00510850, game_state_security_verify_signature_secure, header_address);
}

void __cdecl game_state_security_write_signature(bool insecure, void* optional_base_address)
{
	INVOKE(0x00510870, game_state_security_write_signature, insecure, optional_base_address);
}

//.text:00510940 ; c_game_state_compressor::game_state_set_compressed_size

void __cdecl game_state_set_header_address(void* header_address)
{
	INVOKE(0x00510950, game_state_set_header_address, header_address);

	//game_state_globals.header = static_cast<s_game_state_header*>(header_address);
}

void __cdecl game_state_set_revert_time()
{
	INVOKE(0x00510960, game_state_prepare_for_revert);
}

void __cdecl game_state_set_test_options(const char* test_type)
{
	//INVOKE(0x00510980, game_state_set_test_options, test_type);

	main_status("test-type", "%s", test_type);

	if (!test_type)
	{
		return;
	}

	for (int32 test_option = 0; test_option < k_game_state_test_option_count; test_option++)
	{
		const char* description_a = k_game_state_test_option_description[test_option];
		if (!description_a)
		{
			continue;
		}

		const char* description_b = k_game_state_test_option_description[test_option];
		if (strncmp_debug(test_type, description_a, description_b - description_a - 1) == 0)
		{
			game_state_globals.game_state_test_option = (_game_state_test_option)test_option;
			console_printf("game state test option set to %s\n", description_a);
			break;
		}
	}
}

void __cdecl game_state_shell_dispose()
{
	INVOKE(0x005109A0, game_state_shell_dispose);
}

void __cdecl game_state_shell_gobble_first_physical_allocation()
{
	INVOKE(0x00510A10, game_state_shell_gobble_first_physical_allocation);
}

void __cdecl game_state_shell_initialize()
{
	//INVOKE(0x00510A50, game_state_shell_initialize);

	ASSERT(!game_state_globals.initialized);

	game_state_proc_globals_initialize();
	game_state_globals.allocation_size_checksum = crc_new();

	ASSERT(game_state_globals.base_address);

	game_state_globals.game_state_compressor.initialize();
	game_state_initialize_storage();

	game_state_globals.runtime_saved_game_storage_count = game_state_get_storage_count();
	ASSERT(IN_RANGE_INCLUSIVE(game_state_globals.runtime_saved_game_storage_count, 1, k_saved_game_storage_max_count));

	uns32 available_memory = k_game_state_file_size;
	uns8* starting_address = static_cast<uns8*>(game_state_globals.base_address);

	initialize_game_state_section(k_game_state_header_region, k_game_state_header_region_size, &starting_address, &available_memory, k_crit_section_header_subsection);
	initialize_game_state_section(k_game_state_update_region, k_game_state_update_region_size, &starting_address, &available_memory, k_crit_section_update_subsection);
	initialize_game_state_section(k_game_state_render_region, k_game_state_render_region_size, &starting_address, &available_memory, k_crit_section_render_subsection);
	initialize_game_state_section(k_game_state_shared_region, k_game_state_shared_region_size, &starting_address, &available_memory, k_crit_section_shared_subsection);
	initialize_game_state_section(k_game_state_shared_region_mirror0, k_game_state_shared_region_size, &starting_address, &available_memory, k_crit_section_shared_subsection_mirror0);
	initialize_game_state_section(k_game_state_shared_region_mirror1, k_game_state_shared_region_size, &starting_address, &available_memory, k_crit_section_shared_subsection_mirror1);

	restricted_region_create(k_game_state_header_region, &g_restricted_section[k_game_state_header_region], k_crit_section_header_region, &g_gamestate_deterministic_allocation_callbacks);
	restricted_region_create(k_game_state_update_region, &g_restricted_section[k_game_state_update_region], k_crit_section_update_region, &g_gamestate_deterministic_allocation_callbacks);
	restricted_region_create(k_game_state_render_region, &g_restricted_section[k_game_state_render_region], k_crit_section_render_region, &g_gamestate_nondeterministic_allocation_callbacks);
	restricted_region_create(k_game_state_shared_region, &g_restricted_section[k_game_state_shared_region], k_crit_section_shared_region, &g_gamestate_deterministic_allocation_callbacks);
	restricted_region_add_mirror(k_game_state_shared_region, &g_restricted_section[k_game_state_shared_region_mirror0]);
	restricted_region_add_mirror(k_game_state_shared_region, &g_restricted_section[k_game_state_shared_region_mirror1]);

	restricted_region_lock_primary(k_game_state_header_region);
	restricted_region_lock_primary(k_game_state_update_region);
	restricted_region_lock_primary(k_game_state_render_region);
	restricted_region_lock_primary(k_game_state_shared_region);

	int32 game_state_header_member_index = restricted_region_add_member(k_game_state_header_region, "header", "game_state_header", sizeof(game_state_header), 0, game_state_set_header_address, nullptr, nullptr);
	game_state_globals.header = (game_state_header*)restricted_region_get_member_address(k_game_state_header_region, game_state_header_member_index);
	ASSERT(game_state_globals.header == (game_state_header*)game_state_globals.base_address);

	uns32 error = 0;
	create_report_file_reference(&g_game_state_allocation_file_reference, "gamestate.txt", true);
	if (file_create(&g_game_state_allocation_file_reference) && file_open(&g_game_state_allocation_file_reference, FLAG(_permission_write_bit), &error))
	{
		file_printf(&g_game_state_allocation_file_reference, "game state allocations from: %s\r\n", version_get_full_string());
		file_printf(&g_game_state_allocation_file_reference, "memory total, %u\r\n", k_game_state_file_size);
		file_printf(&g_game_state_allocation_file_reference, "memory available, %u\r\n", available_memory);
		file_printf(&g_game_state_allocation_file_reference, "% 44s,% 24s", "name", "type");

		for (int32 i = 0; i < k_total_restricted_memory_regions; i++)
		{
			file_printf(&g_game_state_allocation_file_reference, ",% 40s", restricted_region_get_name(i));
		}

		file_printf(&g_game_state_allocation_file_reference, "\r\n");
		g_game_state_allocation_file_reference_valid = true;

		game_state_allocation_record(k_game_state_header_region, "header", "s_game_state_header", sizeof(game_state_header));
	}

	game_state_globals.initialized = true;
}

void __cdecl game_state_save_for_game_state_try_and_load_from_persistent_storage()
{
	saved_film_manager_notify_gamestate_load(_saved_film_game_state_load_source_storage);

	game_state_save();
}
HOOK_DECLARE_CALL(0x00510E71, game_state_save_for_game_state_try_and_load_from_persistent_storage);

void __cdecl game_state_try_and_load_from_persistent_storage(e_controller_index controller_index)
{
	INVOKE(0x00510C40, game_state_try_and_load_from_persistent_storage, controller_index);
}

bool __cdecl game_state_validate_and_prepare_to_load_header(game_state_header* header)
{
	return INVOKE(0x00510EF0, game_state_validate_and_prepare_to_load_header, header);
}

const void* __cdecl game_state_with_mirrors_get_buffer_address(int32* buffer_size)
{
	return INVOKE(0x00510F90, game_state_with_mirrors_get_buffer_address, buffer_size);

	//ASSERT(buffer_size);
	//
	//*buffer_size = k_game_state_allocation_size;
	//return game_state_globals.base_address;
}

bool __cdecl game_state_write_core(const char* core_name, const void* buffer, uns32 buffer_length)
{
	//return INVOKE(0x00510FB0, game_state_write_core, core_name, buffer, buffer_length);

	s_file_reference file{};
	game_state_get_core_file_reference(core_name, &file);
	file_create_parent_directories_if_not_present(&file);

	if (!file_exists(&file))
	{
		file_create(&file);
	}

	uns32 error = 0;
	if (!file_open(&file, FLAG(_permission_write_bit), &error))
	{
		return false;
	}

	bool result = file_write(&file, buffer_length, buffer);
	file_close(&file);
	return result;
}

void __cdecl game_state_write_to_persistent_storage_blocking(const game_state_header* header, int32 header_size, const void* buffer, int32 buffer_length)
{
	return INVOKE(0x00511030, game_state_write_to_persistent_storage_blocking, header, header_size, buffer, buffer_length);
}

//.text:00511040 ; c_game_state_compressor_callback::get_memory_configuration

//.text:00511070 ; c_gamestate_deterministic_allocation_callbacks::handle_allocation
void __thiscall c_gamestate_deterministic_allocation_callbacks::handle_allocation(const c_restricted_memory* manager, const char* name, const char* type_name, int32 member_index, void* primary_address, unsigned int size)
{
	ASSERT(!game_state_globals.allocations_locked);
	game_state_allocation_record(manager->m_region_index, name, type_name, size);
	game_state_globals.allocation_size_checksum = crc_checksum_buffer(game_state_globals.allocation_size_checksum, (byte*)&size, 4);
	//determinism_debug_manager_register_game_state_allocation(name, primary_address, size);
}

//.text:00511090 ; c_gamestate_nondeterministic_allocation_callbacks::handle_allocation
void __thiscall c_gamestate_nondeterministic_allocation_callbacks::handle_allocation(const c_restricted_memory* manager, const char* name, const char* type_name, int32 member_index, void* primary_address, unsigned int size)
{
	ASSERT(!game_state_globals.allocations_locked);
	game_state_allocation_record(manager->m_region_index, name, type_name, size);
	game_state_globals.allocation_size_checksum = crc_checksum_buffer(game_state_globals.allocation_size_checksum, (byte*)&size, 4);
}

void c_gamestate_allocation_record_allocation_callbacks::handle_allocation(const c_restricted_memory* manager, const char* name, const char* type_name, int32 member_index, void* primary_address, unsigned int size)
{
	ASSERT(!game_state_globals.allocations_locked);
	game_state_allocation_record(manager->m_region_index, name, type_name, size);
}

//.text:005110B0 ; c_restricted_memory_callbacks::handle_allocation
void c_restricted_memory_callbacks::handle_allocation(const c_restricted_memory* manager, const char* name, const char* type_name, int32 member_index, void* primary_address, unsigned int size)
{
}

//.text:005110C0 ; c_gamestate_deterministic_allocation_callbacks::handle_release
void c_gamestate_deterministic_allocation_callbacks::handle_release(const c_restricted_memory* memory, int32 member_index, void* base_address, unsigned int allocation_size)
{
}

//.text:005110D0 ; c_gamestate_nondeterministic_allocation_callbacks::handle_release
void c_gamestate_nondeterministic_allocation_callbacks::handle_release(const c_restricted_memory* memory, int32 member_index, void* base_address, unsigned int allocation_size)
{
}

//.text:005110E0 ; c_restricted_memory_callbacks::handle_release
void c_restricted_memory_callbacks::handle_release(const c_restricted_memory* memory, int32 member_index, void* base_address, unsigned int allocation_size)
{
}

//.text:005110F0 ; c_game_state_compressor_callback::idle
//.text:00511100 ; c_optional_cache_user_callback::idle

void c_game_state_compressor::initialize()
{
	INVOKE_CLASS_MEMBER(0x00511110, c_game_state_compressor, initialize);

	//m_scratch_buffer = (char*)g_runtime_state_allocation->allocate(0x4B000, "game-state-compression-scratch");
	//m_scratch_buffer_size = 0x4B000;
	//optional_cache_register_user((e_optional_cache_user)5, &g_game_state_compressor_optional_cache_callback);
	//
	//m_compressed_game_state_buffer = nullptr;
	//m_compressed_game_state_buffer_size = 0;
	//
	//m_locked = false;
	//m_lock_pending = false;
	//m_status = 0;
	//m_compressed_game_state_size = 0;
}

void __cdecl initialize_game_state_section(int32 section_index, unsigned int size, uns8** starting_address, uns32* available_memory, e_critical_sections critical_section_id)
{
	return INVOKE(0x00511170, initialize_game_state_section, section_index, size, starting_address, available_memory, critical_section_id);

	//g_restricted_section[section_index].initialize(*starting_address, size, critical_section_id);
	//*available_memory -= size;
	//*starting_address += size;
}

//.text:005111A0 ; c_game_state_compressor::is_lock_pending
//.text:005111B0 ; c_game_state_compressor::is_locked
//.text:005111C0 ; c_game_state_compressor::lock
//.text:00511200 ; c_game_state_compressor::locked_get_compressed_chunk
//.text:00511240 ; c_game_state_compressor::locked_write_compressed_chunk

bool __cdecl player_identifier_exists_in_game_header(const s_player_identifier* player_identifier, const game_state_header* header)
{
	return INVOKE(0x00511290, player_identifier_exists_in_game_header, player_identifier, header);
}

//.text:00511310 ; c_game_state_compressor::set_lock_pending
//.text:00511320 ; c_game_state_compressor_callback::terminate
//.text:00511370 ; c_game_state_compressor::unlock

