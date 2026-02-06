#include "networking/network_configuration.hpp"

#include "config/version.hpp"
#include "cseries/cseries_events.hpp"
#include "main/console.hpp"
#include "memory/crc.hpp"
#include "networking/logic/storage/network_storage_files.hpp"
#include "networking/online/online_url.hpp"
#include "networking/tools/network_blf.hpp"

REFERENCE_DECLARE(0x01860808, int32, hopper_directory_update_index);

REFERENCE_DECLARE(0x019A62B8, bool, g_network_configuration_initialized);
REFERENCE_DECLARE(0x019A62C0, s_network_configuration, g_network_configuration);
REFERENCE_DECLARE(0x019A7988, c_network_configuration_globals, g_network_configuration_globals);

int32 __cdecl network_configuration_calculate_next_retry_interval(int32 current_interval, int32 original_interval)
{
	return INVOKE(0x0043C820, network_configuration_calculate_next_retry_interval, current_interval, original_interval);
}

int32 __cdecl network_configuration_client_badness_rating_threshold()
{
	return INVOKE(0x0043C890, network_configuration_client_badness_rating_threshold);
}

bool __cdecl network_configuration_custom_games_enabled()
{
	return INVOKE(0x0043C8A0, network_configuration_custom_games_enabled);
}

bool __cdecl network_configuration_disable_bad_bandwidth_anticheating()
{
	return INVOKE(0x0043C8B0, network_configuration_disable_bad_bandwidth_anticheating);
}

bool __cdecl network_configuration_disable_bad_client_anticheating()
{
	return INVOKE(0x0043C8C0, network_configuration_disable_bad_client_anticheating);
}

bool __cdecl network_configuration_disable_bad_connectivity_anticheating()
{
	return INVOKE(0x0043C8D0, network_configuration_disable_bad_connectivity_anticheating);
}

void __cdecl network_configuration_dispose()
{
	INVOKE(0x0043C8E0, network_configuration_dispose);

	//g_network_configuration_initialized = false;
	//csmemset(&g_network_configuration, 0, sizeof(g_network_configuration));
}

int32 __cdecl network_configuration_get_map_index_from_map_id(e_map_id map_id)
{
	return INVOKE(0x0043C900, network_configuration_get_map_index_from_map_id, map_id);
}

//enum e_minidump_generation __cdecl network_configuration_get_minidump_generation_type()
int32 __cdecl network_configuration_get_minidump_generation_type()
{
	return INVOKE(0x0043C930, network_configuration_get_minidump_generation_type);
}

//e_network_configuration_state __cdecl network_configuration_get_state()
int32 __cdecl network_configuration_get_state()
{
	return INVOKE(0x0043C940, network_configuration_get_state);
}

void __cdecl network_configuration_initialize(bool building_for_network_file_output)
{
	INVOKE(0x0043C960, network_configuration_initialize, building_for_network_file_output);

	//network_configuration_initialize_private();
	//g_network_configuration_download_buffer.__unknown684 = building_for_network_file_output ? 0 : 2;
	//g_network_configuration_initialized = true;
}

void __cdecl network_configuration_initialize_observer_constants()
{
	INVOKE(0x0043C990, network_configuration_initialize_observer_constants);
}

void __cdecl network_configuration_initialize_private()
{
	INVOKE(0x0043D2B0, network_configuration_initialize_private);
}

void __cdecl network_configuration_initialize_simulation_constants()
{
	INVOKE(0x0043E2C0, network_configuration_initialize_simulation_constants);
}

void __cdecl network_configuration_initialize_simulation_entity_constants()
{
	INVOKE(0x0043E4D0, network_configuration_initialize_simulation_entity_constants);
}

void __cdecl network_configuration_initialize_simulation_entity_creation_constants()
{
	INVOKE(0x0043E4E0, network_configuration_initialize_simulation_entity_creation_constants);
}

void __cdecl network_configuration_initialize_simulation_entity_update_constants()
{
	INVOKE(0x0043E6A0, network_configuration_initialize_simulation_entity_update_constants);
}

void __cdecl network_configuration_initialize_simulation_event_constants()
{
	INVOKE(0x0043F040, network_configuration_initialize_simulation_event_constants);
}

bool __cdecl network_configuration_is_crash_ui_enabled()
{
	return INVOKE(0x0043F4D0, network_configuration_is_crash_ui_enabled);
}

int32 __cdecl network_configuration_maximum_multiplayer_split_screen()
{
	return INVOKE(0x0043F4E0, network_configuration_maximum_multiplayer_split_screen);
}

void __cdecl network_configuration_update()
{
	INVOKE(0x0043F4F0, network_configuration_update);

	//if (url_key != g_online_url_hopper_directory_key)
	//{
	//	url_key = g_online_url_hopper_directory_key;
	//
	//	c_url_string url{};
	//	online_url_make_network_configuration(&url);
	//	g_network_configuration_globals.downloader.set_url(&url);
	//}
	//
	//g_network_configuration_globals.downloader.update();
	//if (g_network_configuration_globals.get_attempt_index() != g_network_configuration_globals.downloader.get_attempt_index())
	//{
	//	g_network_configuration_globals.downloader.update();
	//
	//	s_network_configuration* network_configuration = nullptr;
	//	int32 network_configuration_size = 0;
	//
	//	switch (g_network_configuration_globals.get_data(&network_configuration, &network_configuration_size))
	//	{
	//	case _http_download_status_none:
	//	case _http_download_status_failed:
	//	case _http_download_status_failed_file_not_found:
	//	{
	//		g_network_configuration_globals.last_download_failed = true;
	//		g_network_configuration_globals.attempt_index = g_network_configuration_globals.downloader.get_attempt_index();
	//	}
	//	break;
	//	case _http_download_status_succeeded:
	//	{
	//		if (network_configuration)
	//		{
	//			g_network_configuration = *network_configuration;
	//			g_network_configuration_globals.last_download_failed = false;
	//			g_network_configuration_globals.has_downloaded_network_configuration = true;
	//			g_network_configuration_globals.attempt_index = g_network_configuration_globals.downloader.get_attempt_index();
	//		}
	//	}
	//	break;
	//	}
	//}
}

struct s_network_configuration_file
{
	s_blf_chunk_start_of_file start_of_file_chunk;
	s_blf_chunk_author author_chunk;
	s_blf_header memory_header;
	s_network_configuration memory;
	s_blf_chunk_end_of_file_with_crc end_of_file_chunk;
	byte __pad3719[0x3];
};
COMPILE_ASSERT(sizeof(s_network_configuration_file) == 0x1768);

void network_build_network_configuration()
{
	s_network_configuration_file network_configuration_file;
	csmemset(&network_configuration_file, 0, sizeof(network_configuration_file));

	network_configuration_file.start_of_file_chunk.initialize();
	network_configuration_file.author_chunk.initialize();

	csstrnzcpy(network_configuration_file.start_of_file_chunk.file_type, "halo3 net config", sizeof(network_configuration_file.start_of_file_chunk.file_type));
	csstrnzcpy(network_configuration_file.author_chunk.program_name, version_get_build_name(), sizeof(network_configuration_file.author_chunk.program_name));
	network_configuration_file.author_chunk.program_build_number = version_get_build_number_identifier();
	csstrnzcpy(network_configuration_file.author_chunk.program_build_string, version_get_build_string(), sizeof(network_configuration_file.author_chunk.program_build_string));
	csmemset(network_configuration_file.author_chunk.user, 0, sizeof(network_configuration_file.author_chunk.user));

	network_configuration_file.memory_header.setup('netc', sizeof(s_blf_header) + sizeof(s_network_configuration), 142, 0);
	network_configuration_file.memory = g_network_configuration;

	network_configuration_file.end_of_file_chunk.header.setup(s_blf_chunk_end_of_file::k_chunk_type, OFFSETOF(s_network_configuration_file, end_of_file_chunk), s_blf_chunk_end_of_file::k_chunk_major_version, s_blf_chunk_end_of_file::k_chunk_minor_version);
	network_configuration_file.end_of_file_chunk.authentication_type = _blf_file_authentication_type_crc;
	network_configuration_file.end_of_file_chunk.checksum = crc_checksum_buffer(crc_new(), (byte*)&g_network_configuration, sizeof(s_network_configuration));

	c_static_string<256> configuration_filename;
	configuration_filename.print("%snetwork_configuration_%03u.bin", "bin\\", 142);
	if (create_configuration_file(configuration_filename.get_string(), &network_configuration_file, sizeof(s_network_configuration_file)))
	{
		event(_event_message, "net_build_network_config: succeeded in writing %s", configuration_filename.get_string());
		console_printf("net_build_network_config: succeeded in writing %s", configuration_filename.get_string());
	}
	else
	{
		event(_event_critical, "creation of network configuration file failed. Do you have it checked out?");
	}
}

