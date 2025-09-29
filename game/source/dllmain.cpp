#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "cseries/cseries.hpp"
#include "memory/module.hpp"

void process_attach(HMODULE hModule)
{
	set_donkey_module(hModule);
	DisableThreadLibraryCalls(hModule);
	SetProcessDPIAware();

	c_console::initialize("ManagedDonkey");

	apply_all_patches(false);
	apply_all_hooks(false);

	c_console::toggle_window_visibility();
}

void process_detach(HMODULE hModule)
{
	apply_all_hooks(true);
	apply_all_patches(true);

	c_console::dispose();

	FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		process_attach(hModule);
		break;
	case DLL_PROCESS_DETACH:
		process_detach(hModule);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) void null_export(void) // detours helper export
{
}

// some globals addresses
//.data:0199C000 ; bool shell_application_paused
//.data:0199C004 ; s_project_version g_project_version
//.data:0199C010 ; s_windows_params g_windows_params
//.data:0199C0A8 ; int32 const k_network_compatible_version
//.data:0199C0B0 ; s_netdebug_globals g_netdebug_globals
//.data:0199FA20 ; c_synchronized_long g_net_debug_thread_has_exited
//.data:0199FA28 ; s_transport_globals transport_globals
//.data:0199FAB0 ; s_transport_security_globals transport_security_globals
//.data:0199FC88 ; 
//.data:0199FD18 ; 
//.data:0199FD58 ; 
//.data:0199FD80 ; g_server_descriptions
//.data:0199FDA8 ; s_server_connect_info g_additional_raw_servers[1]
//.data:0199FE88 ; c_online_lsp_manager g_online_lsp_manager
//.data:019A0320 ; 
//.data:019A0321 ; 
//.data:019A0322 ; bool g_network_interface_show_network_quality
//.data:019A0323 ; bool g_network_interface_fake_network_quality
//.data:019A0328 ; s_network_session_interface_globals session_interface_globals
//.data:019A6230 ; 
//.data:019A6238 ; 
//.data:019A62B8 ; bool g_network_configuration_initialized
//.data:019A62C0 ; s_network_configuration g_network_configuration
//.data:019A7988 ; 
//.data:019A9F90 ; c_network_configuration_globals g_network_configuration_globals
//.data:019A9FA0 ; s_simulation_globals simulation_globals
//.data:019AB728 ; s_online_user_internal g_online
//.data:019AB758 ; _g_online_url g_online_url
//.data:019AB7F0 ; s_network_life_cycle_globals life_cycle_globals
//.data:019E8D28 ; s_network_bandwidth_globals network_bandwidth_globals
//.data:019E8D4C ; s_network_time_globals network_time_globals
//.data:019E8D58 ; byte message_storage[0x40000]
//.data:01A28D58 ; s_network_shared_memory_globals network_shared_memory_globals
//.data:01A28D88 ; s_network_base_memory_globals network_base_memory_globals
//.data:02179450 ; 
//.data:02179454 ; 
//.data:02179458 ; 
//.data:0217945C ; 
//.data:02179460 ; c_simulation_sandbox_engine_globals_definition g_simulation_sandbox_engine_globals_definition
//.data:02179464 ; `public: static class c_simulation_sandbox_engine_globals_definition & __cdecl c_simulation_sandbox_engine_globals_definition::instance(void)'::`2'::`local static guard'{2}'
//.data:02179468 ; bool g_online_is_connected_to_live
//.data:02179470 ; c_online_file_manager g_online_file_manager
//.data:02180B68 ; s_network_payload_fragment_256KB
//.data:021C0B70 ; s_network_payload_fragment_256KB
//.data:02200B78 ; s_network_payload_fragment_256KB
//.data:02240B80 ; s_networking_join_data g_network_join_data
//.data:02247448 ; s_online_session_manager_globals online_session_manager_globals
//.data:0224A490 ; c_network_session_parameter_type_collection* g_network_parameter_types
//.data:0224A494 ; c_network_link* g_network_link
//.data:0224A498 ; c_network_message_type_collection* g_network_message_types
//.data:0224A49C ; c_network_message_gateway* g_network_message_gateway
//.data:0224A4A0 ; c_network_message_handler* g_network_message_handler
//.data:0224A4A4 ; c_network_observer* g_network_observer
//.data:0224A4A8 ; c_network_session* g_network_sessions
//.data:0224A4AC ; c_network_session_parameter_type_collection* g_network_session_parameter_types
//.data:0224A4B0 ; c_network_session_manager* g_network_session_manager
//.data:0224A4B4 ; s_network_globals network_globals
//.data:0224A4C0 ; uns32 g_last_http_acquire_server_time
//.data:0224A4C8 ; c_network_http_request_queue g_network_http_request_queue[2]
//.data:02269678 ; c_static_string<256> g_storage_url_subdirectory
//.data:02269778 ; 
//.data:02269780 ; private: static int32 c_simulation_sandbox_engine_globals_definition::next_available_update_bit
//.data:02269788 ; s_managed_user managed_user_globals[4]
//.data:0226B508 ; s_network_leaderboard_globals network_leaderboard_globals
//.data:0226DAD0 ; bool g_network_banhammer_initialized
//.data:0226DAD8 ; s_network_banhammer_globals g_network_banhammer_globals
//.data:0228E520 ; s_network_arbitration_globals network_arbitration_globals
//.data:0228E6B8 ; s_network_broadcast_search_globals g_broadcast_search_globals
//.data:0228E6E0 ; s_network_recruiting_search_globals g_recruiting_search_globals
//.data:0229AEA8 ; s_network_search_globals g_network_search_globals
//.data:0229AEC8 ; c_online_service_record_manager g_online_service_record_manager
//.data:0229C698 ; 
//.data:0229C69C ; 
//.data:0229C6A0 ; 
//.data:0229C6A4 ; 
//.data:0229C6A8 ; 
//.data:0229C918 ; 
//.data:0229C91C ; 
//.data:0229C920 ; 
//.data:0229C958 ; 
//.data:0229C998 ; s_network_request_cache_globals g_network_request_cache_globals
//.data:0229EC20 ; s_session_tracker_globals session_tracker_globals
//.data:0229ECF0 ; c_service_client* g_service_client
//.data:0229ECF8 ; bool g_webstats_globals_initialized
//.data:0229ED00 ; s_webstats_globals g_webstats_globals
//.data:022AADA8 ; 
//.data:022AAEAC ; 
//.data:022AAEB0 ; 
//.data:022AAEB4 ; scenario* global_scenario
//.data:022AAEB8 ; s_game_globals* global_game_globals
//.data:022AAEBC ; uns32 g_active_structure_bsp_mask
//.data:022AAEC0 ; uns32 g_touched_structure_bsp_mask
//.data:022AAEC4 ; uns32 g_active_designer_zone_mask
//.data:022AAEC8 ; uns32 g_active_cinematic_zone_mask
//.data:022AAECC ; uns32 g_touched_cinematic_zone_mask
//.data:022AAED1 ; 
//.data:022AAED8 ; 
//.data:022AAEDC ; 
//.data:022AAEE0 ; 
//.data:022AAFE0 ; 
//.data:022AAFE8 ; s_cache_file_debug_globals* g_cache_file_debug_globals
//.data:022AAFF0 ; s_cache_file_globals g_cache_file_globals
//.data:022AE4F0 ; bool g_render_thread_user_setting
//.data:022AE4F2 ; bool disable_main_loop_throttle
//.data:022AE4F8 ; s_main_status_value g_status_values[32]
//.data:022B456C ; c_interlocked_long g_render_thread_waiting
//.data:022B46C8 ; c_interlocked_long g_render_thread_enabled
//.data:022B471C ; c_interlocked_long g_single_thread_request_flags
//.data:022B4738 ; _main_globals main_globals
//.data:022B47F0 ; bool x_recursion_lock
//.data:022B47F1 ; bool x_fatal_recursion_lock
//.data:022B47F8 ; real32 last_framerate_time
//.data:022B47FC ; bool display_framerate
//.data:022B4800 ; 
//.data:022B4808 ; 
//.data:022B480C ; 
//.data:022B4818 ; s_async_globals async_globals
//.data:022B7FA4 ; int32 g_secure_memory_size
//.data:022B7FA8 ; void* g_secure_memory
//.data:022B7FAC ; s_font_globals g_font_globals
//.data:022C0108 ; s_runtime_state_globals g_runtime_state_globals
//.data:022C0120 ; 
//.data:022C0128 ; s_global_preferences_internals_type g_global_preferences_internal
//.data:02344144 ; c_interlocked_long g_game_state_locked
//.data:02344148 ; s_game_state_globals game_state_globals
//.data:0238DB40 ; c_restricted_section g_restricted_section[6]
//.data:0238DBE8 ; s_input_globals input_globals
//.data:0238E858 ; s_sound_manager_globals* g_sound_manager_globals
//.data:0238E85C ; sound_channel_datum* g_sound_channels
//.data:0238E860 ; c_smart_data_array<sound_datum> g_sound_data
//.data:0238E864 ; bool debug_sound_always_update_in_idle
//.data:0238E86E ; 
//.data:0238E86F ; 
//.data:0238E871 ; bool debug_sound_force_first_person_listener
//.data:0238E874 ; int32 g_sound_manager_reference_count
//.data:0238E878 ; bool g_set_always_a_debugger_present
//.data:0238E879 ; bool g_set_never_a_debugger_present
//.data:0238E87C ; uns32 g_exception_time
//.data:0238E880 ; PEXCEPTION_POINTERS g_exception_pointers
//.data:0238E884 ; c_synchronized_long g_exception_caching_in_progress
//.data:0238E888 ; s_exception_information g_exception_information
//.data:0238EB80 ; s_thread_system_globals g_thread_globals
//.data:0238EC50 ; s_physical_memory_globals physical_memory_globals
//.data:0238ED04 ; void* resources_buffer
//.data:0238ED08 ; void* k_physical_memory_base_virtual_address
//.data:0238ED0C ; void* k_virtual_to_physical_base_offset
//.data:0238ED14 ; uns32 g_local_random_seed
//.data:0238ED18 ; 
//.data:0238ED1C ; 
//.data:0238ED20 ; int32 g_saved_game_files[14]
//.data:0238ED58 ; 
//.data:0238ED60 ; 
//.data:0238EF60 ; bool g_use_temporary_directory
//.data:0238F45C ; s_synchronization_globals g_synch_globals
//.data:0238FD80 ; 
//.data:0238FDA0 ; int32 crc_table[256]
//.data:023901A0 ; bool crc_table_initialized
//.data:023901A1 ; bool debug_globals_empty
//.data:023901A4 ; int32 g_datamine_player_update_interval
//.data:023901A8 ; int32 g_mp_player_update_interval
//.data:023901AC ; int32 g_datamine_debug_menu_interval
//.data:023901B0 ; bool g_datamine_spam_enabled
//.data:023901B8 ; s_transport_address_mapping g_transport_address_mapping[51]
//.data:02390CE8 ; LARGE_INTEGER
//.data:02390CF0 ; 
//.data:02390CF8 ; 
//.data:02390D00 ; loading_will_use_ship_loading_screen
//.data:02390D04 ; loading_globals_definition loading_globals
//.data:02390D3C ; bool global_playtest_mode
//.data:02390D44 ; 
//.data:02390D48 ; 
//.data:02390D4C ; 
//.data:02390D50 ; 
//.data:02390D54 ; 
//.data:02390D58 ; 
//.data:02390D5C ; 
//.data:02390D60 ; 
//.data:02390DA0 ; s_game_engine_global_message g_game_engine_globals.messages[128]
//.data:023915A0 ; s_synchronized_list_header g_game_engine_globals_messages.__unknown800
//.data:023915A8 ; s_synchronized_list_header g_game_engine_globals_messages.__unknown808
//.data:023915B0 ; bool g_game_engine_globals_messages.m_initialized
//.data:023915B4 ; int32 g_game_engine_globals_messages.m_last_entry_index
//.data:023915C0 ; s_game_engine_ranking_cache g_game_engine_ranking_cache[2]
//.data:023916C0 ; c_typed_allocation_data_no_destruct<c_cache_file_tag_resource_runtime_manager, 1> g_resource_runtime_manager
//.data:023916D8 ; s_main_game_globals main_game_globals
//.data:023DAE90 ; bool main_game_load_panic_recursion_lock
//.data:023DAE91 ; 
//.data:023DAE94 ; 
//.data:023DAE98 ; s_game_results_globals g_game_results_globals
//.data:023DAEA8 ; c_game_results g_current_game_results
//.data:023FF3B8 ; 
//.data:023FF3C0 ; s_game_state_proc_globals g_game_state_proc_globals
//.data:023FF428 ; 
//.data:023FF42C ; 
//.data:023FF440 ; bool debug_scripting
//.data:023FF441 ; bool debug_globals
//.data:023FF442 ; bool debug_globals_all
//.data:023FF444 ; bool debug_global_variables[512]
//.data:023FF648 ; typecasting_procedure g_typecasting_procedures[k_hs_type_count][k_hs_type_count]
//.data:0240648C ; c_game_engine_sandbox_variant* sandbox_variant
//.data:02406494 ; c_restricted_memory g_restricted_regions[5]
//.data:0240A33C ; 
//.data:0240A340 ; s_content_catalogue_globals g_content_catalogue_globals
//.data:0240AE08 ; c_storage_device
//.data:0240AE10 ; 
//.data:0240AE1A ; int16[87]
//.data:0240AEC8 ; 
//.data:0240AED0 ; s_overlapped_globals g_overlapped_globals
//.data:0240B1E0 ; bool g_cache_files_read_from_map_directory
//.data:0240B1E8 ; s_cache_file_table_of_contents cache_file_table_of_contents
//.data:0243C098 ; s_cache_file_copy_globals cache_file_copy_globals
//.data:0243F780 ; c_asynchronous_io_arena g_cache_file_io_arena
//.data:0243FB38 ; 
//.data:0243FB40 ; int32 g_io_last_error
//.data:0243FB44 ; 
//.data:0243FB60 ; bool effects_enable
//.data:0243FB64 ; c_aoe_damage_batchifier* global_batchifier_pointer

