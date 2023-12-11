#include "saved_games/game_state.hpp"

#include "cache/restricted_memory.hpp"
#include "cache/restricted_memory_regions.hpp"
#include "cseries/runtime_state.hpp"
#include "memory/crc.hpp"
#include "memory/module.hpp"
#include "multithreading/synchronization.hpp"
#include "saved_games/game_state_pc.hpp"
#include "saved_games/game_state_procs.hpp"

REFERENCE_DECLARE(0x0189D468, c_game_state_compressor_callback, g_game_state_compressor_optional_cache_callback);
REFERENCE_DECLARE(0x0189D46C, c_gamestate_deterministic_allocation_callbacks, g_gamestate_deterministic_allocation_callbacks);
REFERENCE_DECLARE(0x0189D470, c_gamestate_nondeterministic_allocation_callbacks, g_gamestate_nondeterministic_allocation_callbacks);
REFERENCE_DECLARE(0x02344148, s_game_state_globals, game_state_globals);

//HOOK_DECLARE(0x00510A50, game_state_shell_initialize);

//.text:0050EEA0 ; c_game_state_compressor_callback::c_game_state_compressor_callback
//.text:0050EEB0 ; c_gamestate_deterministic_allocation_callbacks::c_gamestate_deterministic_allocation_callbacks
//.text:0050EEC0 ; c_gamestate_nondeterministic_allocation_callbacks::c_gamestate_nondeterministic_allocation_callbacks
//.text:0050EED0 ; c_optional_cache_user_callback::c_optional_cache_user_callback
//.text:0050EEE0 ; c_restricted_memory_callbacks::c_restricted_memory_callbacks
//.text:0050EEF0
//.text:0050EF10
//.text:0050EF70
//.text:0050EF80
//.text:0050EFA0
//.text:0050EFC0
//.text:0050EFE0 ; c_game_state_compressor::allocate_buffer
//.text:0050F020 ; c_game_state_compressor::compress_internal

bool __cdecl create_file_from_buffer(char const* file_name, char const* file_contents)
{
	return INVOKE(0x0050F0C0, create_file_from_buffer, file_name, file_contents);
}

//.text:0050F170 ; c_game_state_compressor::deallocate_buffer
//.text:0050F1A0 ; c_game_state_compressor::dispose
//.text:0050F220 ; c_gamestate_deterministic_allocation_callbacks::filter_base_offset
//.text:0050F230 ; c_gamestate_nondeterministic_allocation_callbacks::filter_base_offset
//.text:0050F240 ; c_restricted_memory_callbacks::filter_base_offset
//.text:0050F250 ; c_gamestate_deterministic_allocation_callbacks::filter_size_request
//.text:0050F260 ; c_gamestate_nondeterministic_allocation_callbacks::filter_size_request
//.text:0050F270 ; c_restricted_memory_callbacks::filter_size_request

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
	return INVOKE(0x0050F330, game_state_debug_server_file_uploading_enabled, reason);
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

void __cdecl game_state_dispose_from_old_designer_zone_set(dword a1)
{
	INVOKE(0x0050F5F0, game_state_dispose_from_old_designer_zone_set, a1);
}

void __cdecl game_state_dispose_from_old_structure_bsp(dword a1)
{
	INVOKE(0x0050F600, game_state_dispose_from_old_structure_bsp, a1);
}

void const* __cdecl game_state_get_buffer_address(long* buffer_size)
{
	return INVOKE(0x0050F610, game_state_get_buffer_address, buffer_size);
}

//.text:0050F630 ; c_game_state_compressor::game_state_get_compressed
//.text:0050F6A0 ; c_game_state_compressor::game_state_get_storage
//.text:0050F6C0 ; c_game_state_compressor::game_state_get_compressed_checksum
//.text:0050F6E0 ; c_game_state_compressor::game_state_get_compressed_from_buffer
//.text:0050F710 ; c_game_state_compressor::game_state_get_compressed_size

c_game_state_compressor* __cdecl game_state_get_compressor()
{
	return INVOKE(0x0050F720, game_state_get_compressor);
}

bool __cdecl game_state_get_game_options_from_core(char const* core_name, game_options* options)
{
	return INVOKE(0x0050F740, game_state_get_game_options_from_core, core_name, options);
}

bool __cdecl game_state_get_scenario_path_from_core(char const* core_name, char* buffer, long buffer_length)
{
	return INVOKE(0x0050F890, game_state_get_scenario_path_from_core, core_name, buffer, buffer_length);
}

char const* __cdecl game_state_get_test_option_string()
{
	return INVOKE(0x0050F9E0, game_state_get_test_option_string);
}

bool __cdecl game_state_header_matches(s_game_state_header const* header_a, s_game_state_header const* header_b)
{
	return INVOKE(0x0050F9F0, game_state_header_matches, header_a, header_b);
}

bool __cdecl game_state_header_matches_current_network_session_settings(s_game_state_header const* header)
{
	return INVOKE(0x0050FAA0, game_state_header_matches_current_network_session_settings, header);
}

bool __cdecl game_state_header_prepare_to_load(s_game_state_header* header, bool a2, bool a3, bool* a4)
{
	return INVOKE(0x0050FBC0, game_state_header_prepare_to_load, header, a2, a3, a4);
}

bool __cdecl game_state_header_valid(s_game_state_header const* header)
{
	return INVOKE(0x0050FC70, game_state_header_valid, header);
}

bool __cdecl game_state_header_valid_for_saving(s_game_state_header const* header)
{
	return INVOKE(0x0050FCF0, game_state_header_valid_for_saving, header);
}

void __cdecl game_state_initialize()
{
	INVOKE(0x0050FDB0, game_state_initialize);
}

void __cdecl game_state_initialize_for_new_map()
{
	INVOKE(0x0050FDC0, game_state_initialize_for_new_map);
}

void __cdecl game_state_initialize_for_new_designer_zone_set(dword a1)
{
	INVOKE(0x0050FEB0, game_state_initialize_for_new_designer_zone_set, a1);
}

void __cdecl game_state_initialize_for_new_structure_bsp(dword a1)
{
	INVOKE(0x0050FED0, game_state_initialize_for_new_structure_bsp, a1);
}

bool __cdecl game_state_is_locked()
{
	return INVOKE(0x0050FF00, game_state_is_locked);
}

void __cdecl game_state_load_core(char const* core_name)
{
	INVOKE(0x0050FF10, game_state_load_core, core_name);
}

void __cdecl game_state_lock(bool a1)
{
	INVOKE(0x00510020, game_state_lock, a1);
}

void __cdecl game_state_prepare_for_revert()
{
	INVOKE(0x00510040, game_state_prepare_for_revert);
}

void __cdecl game_state_preserve()
{
	INVOKE(0x00510100, game_state_preserve);
}

bool __cdecl game_state_read_core(char const* core_name, void* buffer, dword buffer_length)
{
	return INVOKE(0x00510110, game_state_read_core, core_name, buffer, buffer_length);
}

bool __cdecl game_state_read_from_persistent_storage_blocking(e_controller_index controller_index, void* buffer, dword buffer_size)
{
	return INVOKE(0x00510270, game_state_read_from_persistent_storage_blocking, controller_index, buffer, buffer_size);
}

//.text:00510280

bool __cdecl game_state_read_header_from_persistent_storage_blocking(e_controller_index controller_index, s_game_state_header* header)
{
	return INVOKE(0x00510310, game_state_read_header_from_persistent_storage_blocking, controller_index, header);
}

void __cdecl game_state_reset_mapping(long a1)
{
	INVOKE(0x00510330, game_state_reset_mapping, a1);
}

//.text:00510350

void __cdecl game_state_revert(dword flags)
{
	INVOKE(0x005105F0, game_state_revert, flags);
}

bool __cdecl game_state_reverted()
{
	return INVOKE(0x00510530, game_state_reverted);
}

void __cdecl game_state_save()
{
	INVOKE(0x00510550, game_state_save);
}

void __cdecl game_state_save_core(char* core_name)
{
	INVOKE(0x005105F0, game_state_save_core, core_name);
}

void __cdecl game_state_save_to_persistent_storage_blocking()
{
	INVOKE(0x00510650, game_state_save_to_persistent_storage_blocking);
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

void __cdecl game_state_security_write_signature(bool a1, void* a2)
{
	INVOKE(0x00510870, game_state_security_write_signature, a1, a2);
}

//.text:00510940 ; c_game_state_compressor::game_state_set_compressed_size

void __cdecl game_state_set_header_address(void* header_address)
{
	INVOKE(0x00510950, game_state_set_header_address, header_address);
}

void __cdecl game_state_set_revert_time()
{
	INVOKE(0x00510960, game_state_prepare_for_revert);
}

void __cdecl game_state_set_test_options(char const* test_type)
{
	INVOKE(0x00510980, game_state_set_test_options, test_type);
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
	INVOKE(0x00510A50, game_state_shell_initialize);

	//ASSERT(!game_state_globals.initialized)
	//
	//game_state_proc_globals_initialize();
	//game_state_globals.checksum = crc_new();
	//
	//ASSERT(game_state_globals.base_address)
	//
	//game_state_globals.compressor.initialize();
	//game_state_initialize_storage();
	//
	//game_state_globals.runtime_saved_game_storage_count = game_state_get_storage_count();
	//ASSERT(IN_RANGE_INCLUSIVE(game_state_globals.runtime_saved_game_storage_count, 1, k_saved_game_storage_max_count));
	//
	//dword available_memory = 0x1280000; // cpu_size = 0x1280000, persist_size = 0x980000
	//byte* starting_address = static_cast<byte*>(game_state_globals.base_address);
	//
	//initialize_game_state_section(k_game_state_header_region, k_game_state_header_region_size, &starting_address, &available_memory, _critical_section_header_section);
	//initialize_game_state_section(k_game_state_update_region, k_game_state_update_region_size, &starting_address, &available_memory, _critical_section_update_section);
	//initialize_game_state_section(k_game_state_render_region, k_game_state_render_region_size, &starting_address, &available_memory, _critical_section_render_section);
	//initialize_game_state_section(k_game_state_shared_region, k_game_state_shared_region_size, &starting_address, &available_memory, _critical_section_shared_section);
	//initialize_game_state_section(k_global_render_data_region, k_game_state_shared_region_size, &starting_address, &available_memory, _critical_section_shared_mirror0);
	//initialize_game_state_section(k_global_render_data_region + 1, k_game_state_shared_region_size, &starting_address, &available_memory, _critical_section_shared_mirror1);
	//
	//restricted_region_create(k_game_state_header_region, &g_restricted_section[k_game_state_header_region], _critical_section_header_region, &g_gamestate_deterministic_allocation_callbacks);
	//restricted_region_create(k_game_state_update_region, &g_restricted_section[k_game_state_update_region], _critical_section_update_region, &g_gamestate_deterministic_allocation_callbacks);
	//restricted_region_create(k_game_state_render_region, &g_restricted_section[k_game_state_render_region], _critical_section_render_region, &g_gamestate_nondeterministic_allocation_callbacks);
	//restricted_region_create(k_game_state_shared_region, &g_restricted_section[k_game_state_shared_region], _critical_section_shared_region, &g_gamestate_deterministic_allocation_callbacks);
	//
	//restricted_region_lock_primary(k_game_state_header_region);
	//restricted_region_lock_primary(k_game_state_update_region);
	//restricted_region_lock_primary(k_game_state_render_region);
	//restricted_region_lock_primary(k_game_state_shared_region);
	//
	//long game_state_header_member_index = restricted_region_add_member(k_game_state_header_region, "header", "game_state_header", sizeof(s_game_state_header), 0, game_state_set_header_address, NULL, NULL);
	//game_state_globals.header = (s_game_state_header*)restricted_region_get_member_address(k_game_state_header_region, game_state_header_member_index);
	//ASSERT(game_state_globals.header == (s_game_state_header*)game_state_globals.base_address);
	//
	//game_state_globals.initialized = true;
}

void __cdecl game_state_try_and_load_from_persistent_storage(e_controller_index controller_index)
{
	INVOKE(0x00510C40, game_state_try_and_load_from_persistent_storage, controller_index);
}

bool __cdecl game_state_validate_and_prepare_to_load_header(s_game_state_header* header)
{
	return INVOKE(0x00510EF0, game_state_validate_and_prepare_to_load_header, header);
}

//.text:00510F90

bool __cdecl game_state_write_core(char const* core_name, void const* buffer, dword buffer_length)
{
	return INVOKE(0x00510FB0, game_state_write_core, core_name, buffer, buffer_length);
}

void __cdecl game_state_write_to_persistent_storage_blocking(s_game_state_header const* header, long header_size, void const* buffer, long buffer_length)
{
	return INVOKE(0x00511030, game_state_write_to_persistent_storage_blocking, header, header_size, buffer, buffer_length);
}

//.text:00511040 ; c_game_state_compressor_callback::get_memory_configuration
//.text:00511070 ; c_gamestate_deterministic_allocation_callbacks::handle_allocation
//.text:00511090 ; c_gamestate_nondeterministic_allocation_callbacks::handle_allocation
//.text:005110B0 ; c_restricted_memory_callbacks::handle_allocation
//.text:005110C0 ; c_gamestate_deterministic_allocation_callbacks::handle_release
//.text:005110D0 ; c_gamestate_nondeterministic_allocation_callbacks::handle_release
//.text:005110E0 ; c_restricted_memory_callbacks::handle_release
//.text:005110F0 ; c_game_state_compressor_callback::idle
//.text:00511100 ; c_optional_cache_user_callback::idle

void c_game_state_compressor::initialize()
{
	DECLFUNC(0x00511110, void, __thiscall, c_game_state_compressor*)(this);

	//m_scratch_buffer = (char*)g_runtime_state_allocation->allocate(0x4B000, "game-state-compression-scratch");
	//m_scratch_buffer_size = 0x4B000;
	//optional_cache_register_user((e_optional_cache_user)5, &g_game_state_compressor_optional_cache_callback);
	//
	//m_compressed_game_state_buffer = NULL;
	//m_compressed_game_state_buffer_size = 0;
	//
	//m_locked = false;
	//m_lock_pending = false;
	//m_status = 0;
	//m_compressed_game_state_size = 0;
}


void __cdecl initialize_game_state_section(long section_index, unsigned int size, byte** starting_address, dword* available_memory, e_critical_sections critical_section_id)
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

bool __cdecl player_identifier_exists_in_game_header(s_player_identifier const* player_identifier, s_game_state_header const* header)
{
	return INVOKE(0x00511290, player_identifier_exists_in_game_header, player_identifier, header);
}

//.text:00511310 ; c_game_state_compressor::set_lock_pending
//.text:00511320 ; c_game_state_compressor_callback::terminate
//.text:00511370 ; c_game_state_compressor::unlock

