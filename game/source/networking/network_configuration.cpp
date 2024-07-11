#include "networking/network_configuration.hpp"

#include "config/version.hpp"
#include "cseries/cseries_events.hpp"
#include "main/console.hpp"
#include "memory/crc.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "networking/logic/storage/network_storage_files.hpp"
#include "networking/online/online_url.hpp"
#include "networking/tools/network_blf.hpp"

using t_network_configuration_downloader = c_http_blf_simple_downloader<s_network_configuration>;

REFERENCE_DECLARE(0x01860808, long, hopper_directory_update_index);

REFERENCE_DECLARE(0x019A62B8, bool, g_network_configuration_initialized);
REFERENCE_DECLARE(0x019A62C0, s_network_configuration, g_network_configuration);
REFERENCE_DECLARE(0x019A7988, t_network_configuration_downloader, g_network_configuration_download_buffer);
REFERENCE_DECLARE(0x019A9F90, c_network_configuration_globals, g_network_configuration_globals);

//.text:0043C890 ; 

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

//long __cdecl network_configuration_get_map_index_from_map_id(e_map_id)
long __cdecl network_configuration_get_map_index_from_map_id(long map_id)
{
	return INVOKE(0x0043C900, network_configuration_get_map_index_from_map_id, map_id);
}

//.text:0043C930 ; 

//e_network_configuration_state __cdecl network_configuration_get_state()
long __cdecl network_configuration_get_state()
{
	return INVOKE(0x0043C940, network_configuration_get_state);
}

void __cdecl network_configuration_initialize(bool a1)
{
	INVOKE(0x0043C960, network_configuration_initialize, a1);

	//network_configuration_initialize_private();
	//g_network_configuration_download_buffer.__unknown684 = a1 ? 0 : 2;
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

//.text:0043F4D0 ; 

long __cdecl network_configuration_maximum_multiplayer_split_screen()
{
	return INVOKE(0x0043F4E0, network_configuration_maximum_multiplayer_split_screen);
}

void __cdecl network_configuration_update()
{
	INVOKE(0x0043F4F0, network_configuration_update);

	//if (g_network_configuration_globals.__unknown8 != hopper_directory_update_index)
	//{
	//	g_network_configuration_globals.__unknown8 = hopper_directory_update_index;
	//
	//	c_url_string url_string{};
	//	online_url_make_network_configuration(&url_string);
	//	g_network_configuration_download_buffer.set_url(&url_string);
	//}
	//
	//g_network_configuration_download_buffer.update();
	//if (g_network_configuration_globals.__unknown4 != g_network_configuration_download_buffer.get_attempt_index())
	//{
	//	g_network_configuration_download_buffer.update();
	//
	//	s_network_configuration* network_configuration = NULL;
	//	long network_configuration_size = 0;
	//
	//	switch (g_network_configuration_download_buffer.get_data(&network_configuration, &network_configuration_size))
	//	{
	//	case _http_download_status_none:
	//	case _http_download_status_failed:
	//	case _http_download_status_failed_file_not_found:
	//	{
	//		g_network_configuration_globals.__unknown0 = true;
	//		g_network_configuration_globals.__unknown4 = g_network_configuration_download_buffer.get_attempt_index();
	//	}
	//	break;
	//	case _http_download_status_succeeded:
	//	{
	//		if (network_configuration)
	//		{
	//			g_network_configuration = *network_configuration;
	//			g_network_configuration_globals.__unknown0 = false;
	//			g_network_configuration_globals.__unknown1 = true;
	//			g_network_configuration_globals.__unknown4 = g_network_configuration_download_buffer.get_attempt_index();
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
static_assert(sizeof(s_network_configuration_file) == 0x1768);

void network_build_network_configuration()
{
	s_network_configuration_file network_configuration_file;
	csmemset(&network_configuration_file, 0, sizeof(network_configuration_file));

	network_configuration_file.start_of_file_chunk.initialize();
	network_configuration_file.author_chunk.initialize();

	network_configuration_file.start_of_file_chunk.name.set("halo3 net config");
	network_configuration_file.author_chunk.build_name.set(version_get_build_name());
	network_configuration_file.author_chunk.build_identifier = version_get_build_number_identifier();
	network_configuration_file.author_chunk.build_string.set(version_get_build_string());
	network_configuration_file.author_chunk.author_name.clear();

	network_configuration_file.memory_header.setup('netc', sizeof(s_blf_header) + sizeof(s_network_configuration), 142, 0);
	network_configuration_file.memory = g_network_configuration;

	network_configuration_file.end_of_file_chunk.header.setup(s_blf_chunk_end_of_file::k_chunk_type, OFFSETOF(s_network_configuration_file, end_of_file_chunk), s_blf_chunk_end_of_file::k_version_major, s_blf_chunk_end_of_file::k_version_minor);
	network_configuration_file.end_of_file_chunk.authentication_type = _blf_file_authentication_type_crc;
	network_configuration_file.end_of_file_chunk.checksum = crc_checksum_buffer(crc_new(), (byte*)&g_network_configuration, sizeof(s_network_configuration));

	c_static_string<256> configuration_filename;
	configuration_filename.print("%snetwork_configuration_%03u.bin", "bin\\", 142);
	if (create_configuration_file(configuration_filename.get_string(), &network_configuration_file, sizeof(s_network_configuration_file)))
	{
		generate_event(_event_level_message, "net_build_network_config: succeeded in writing %s", configuration_filename.get_string());
		console_printf("net_build_network_config: succeeded in writing %s", configuration_filename.get_string());
	}
	else
	{
		generate_event(_event_level_critical, "creation of network configuration file failed. Do you have it checked out?");
	}
}

