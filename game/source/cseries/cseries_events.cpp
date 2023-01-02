#include "cseries/cseries_events.hpp"

#include "cseries/console.hpp"
#include "memory/module.hpp"

#include <string.h>

HOOK_DECLARE(0x000D858D0, network_debug_print);

// used inplace of `c_event::generate`
// net::REMOTE_BINLOGGER
void network_debug_print(const char* format, ...)
{
	long format_address = (long)format;

	// no print switch
	switch (format_address)
	{
	case 0x01610560: // "MP/NET/SESSION,PARAMS: c_generic_network_session_parameter<enum e_network_rough_quality>::set: [%s] parameter type %d [%s] being set"
	case 0x01611EE8: // "MP/NET/LINK,RCV: c_network_link::decode_packet: Received a game-data-only packet that was too big (%d > %d bytes)."
	case 0x01612028: // "MP/NET/LINK,RCV: c_network_link::read_data_immediate: Read %d-byte packet from invalid address '%s'."
		return;
	}

	// format replacement switch
	// occurrences of `%LX` need to be replaced with `%llX`
	// occurrences of `%Lx` need to be replaced with `%llx`
	switch (format_address)
	{
	case 0x0160E6F0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: network_session_interface_set_local_user_xuid: local user %d set player xuid=%LX"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: network_session_interface_set_local_user_xuid: local user %d set player xuid=%llX";
		break;
	case 0x01616488: // "MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::set_peer_properties: [%s] peer-properties accepted for peer #%d [%s], name=%S, session name=%S, map=%d, map status=%s, map progress=%d, game instance=0x%LX, ready hopper=%d, start error=%s, hdd=%d, upstream=%d bps (reliable %d), downstream=%d bps, nat=%d, ratings (conn, host, client)=%d, %d, %d, connectivity=%04X (complete=%04X), flags=%X"
		format = "MP/NET/SESSION,MEMBERSHIP: c_network_session_membership::set_peer_properties: [%s] peer-properties accepted for peer #%d [%s], name=%S, session name=%S, map=%d, map status=%s, map progress=%d, game instance=0x%llX, ready hopper=%d, start error=%s, hdd=%d, upstream=%d bps (reliable %d), downstream=%d bps, nat=%d, ratings (conn, host, client)=%d, %d, %d, connectivity=%04X (complete=%04X), flags=%X";
		break;
	case 0x016216A8: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_online_files_deleter::update: Deleted item from share %Lx, slot %d"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_online_files_deleter::update: Deleted item from share %llx, slot %d";
		break;
	case 0x01621718: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_online_files_deleter::update: Could not delete item from share %Lx, slot %d"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_online_files_deleter::update: Could not delete item from share %llx, slot %d";
		break;
	case 0x01625120: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: remove_from_player_list: unable to find player [0x%Lx] in the list to remove from the session"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: remove_from_player_list: unable to find player [0x%llx] in the list to remove from the session";
		break;
	case 0x01627DD8: // "MP/NET/SESSION,PARAMS: c_network_session_parameters::check_to_send_updates: [%s] sending session parameter update [cleared 0x%LX updated 0x%LX initial:%s]"
		format = "MP/NET/SESSION,PARAMS: c_network_session_parameters::check_to_send_updates: [%s] sending session parameter update [cleared 0x%llX updated 0x%llX initial:%s]";
		break;
	case 0x01628578: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameters::handle_update: [%s] handling session parameter update [cleared 0x%LX updated 0x%LX initial:%s]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameters::handle_update: [%s] handling session parameter update [cleared 0x%llX updated 0x%llX initial:%s]";
		break;
	case 0x016338F0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_life_cycle_state_handler_matchmaking_arbitration::session_membership_matches_arbitration_registration_results: registrant %d machine %Lx xuid %Lx trust %d"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_life_cycle_state_handler_matchmaking_arbitration::session_membership_matches_arbitration_registration_results: registrant %d machine %llx xuid %llx trust %d";
		break;
	case 0x0163D7D8: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_replication_entity_manager::process_creation: remote entity creation %lx type %d"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_replication_entity_manager::process_creation: remote entity creation %lx type %d";
		break;
	case 0x0163E5C8: // "MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::delete_local_entity: local entity marked for deletion %lx"
		format = "MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::delete_local_entity: local entity marked for deletion %lx";
		break;
	case 0x0163E640: // "MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::notify_delete_remote_entity: remote entity deleted %lx"
		format = "MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::notify_delete_remote_entity: remote entity deleted %lx";
		break;
	case 0x0163E6B0: // "MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::acknowledge_entity_deletion: local entity deleted %lx after view %d acknowledge"
		format = "MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::acknowledge_entity_deletion: local entity deleted %lx after view %d acknowledge";
		break;
	case 0x0163E820: // "MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::delete_entity_internal: entity deleted %lx"
		format = "MP/NET/REPLICATION,ENTITY: c_replication_entity_manager::delete_entity_internal: entity deleted %lx";
		break;
	case 0x01647B28: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: network_banhammer_update_controllers: user file for controller %d / %L / %S contains auto download for user %LX slot %d server id %LX"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: network_banhammer_update_controllers: user file for controller %d / %L / %S contains auto download for user %llX slot %d server id %llX";
		break;
	}

	// format replacement switch
	// occurrences of `%8X` need to be replaced with `%08X`
	// this isn't a necessary replacement, but it makes logs nicer to read
	switch (format_address)
	{
	case 0x01623838: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_host: create host to session 0x%8X"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_host: create host to session 0x%08X";
		break;
	case 0x016243C8: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_process_pending_operations: modify session 0x%8X [slots pub %d priv %d] [flags 0x%08X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_process_pending_operations: modify session 0x%08X [slots pub %d priv %d] [flags 0x%08X]";
		break;
	case 0x016244E0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_session_internal: deleting session 0x%8X that we haven't created yet"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_session_internal: deleting session 0x%08X that we haven't created yet";
		break;
	case 0x01624568: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_session_internal: deleting offline session 0x%8X"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_session_internal: deleting offline session 0x%08X";
		break;
	case 0x016245D8: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_session_internal: deleting system-link session 0x%8X [%s]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_session_internal: deleting system-link session 0x%08X [%s]";
		break;
	case 0x01624650: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_session_internal: async deleting session 0x%8X [%s]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_session_internal: async deleting session 0x%08X [%s]";
		break;
	case 0x01624948: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_session_internal: creating %s offline session 0x%8X [controller %d] [slots pub %d priv %d] [flags 0x%08X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_session_internal: creating %s offline session 0x%08X [controller %d] [slots pub %d priv %d] [flags 0x%08X]";
		break;
	case 0x016249F0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_session_internal: creating %s keys (system link) 0x%8X [controller %d] [slots pub %d priv %d] [flags 0x%08X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_session_internal: creating %s keys (system link) 0x%08X [controller %d] [slots pub %d priv %d] [flags 0x%08X]";
		break;
	case 0x01624AA0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_session_internal: async creating %s session 0x%8X [controller %d] [slots pub %d priv %d] [flags 0x%08X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_session_internal: async creating %s session 0x%08X [controller %d] [slots pub %d priv %d] [flags 0x%08X]";
		break;
	case 0x01624BC0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_host_migration_internal: creating offline host-migration session 0x%8X [controller %d] [slots pub %d priv %d] [flags 0x%08X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_host_migration_internal: creating offline host-migration session 0x%08X [controller %d] [slots pub %d priv %d] [flags 0x%08X]";
		break;
	case 0x01624C80: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_host_migration_internal: creating host-migration keys (system link) 0x%8X [controller %d] [slots pub %d priv %d] [flags 0x%08X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_host_migration_internal: creating host-migration keys (system link) 0x%08X [controller %d] [slots pub %d priv %d] [flags 0x%08X]";
		break;
	case 0x01624D40: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_host_migration_internal: async doing host migration as %s, session 0x%8X [controller %d] [slots pub %d priv %d] [flags 0x%08X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_create_host_migration_internal: async doing host migration as %s, session 0x%08X [controller %d] [slots pub %d priv %d] [flags 0x%08X]";
		break;
	case 0x01624E00: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_host_migration_internal: deleting host migration description that we haven't created yet 0x%8X"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_host_migration_internal: deleting host migration description that we haven't created yet 0x%08X";
		break;
	case 0x01624EA0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_host_migration_internal: deleting offline host-migration session 0x%8X"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_host_migration_internal: deleting offline host-migration session 0x%08X";
		break;
	case 0x01624F28: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_host_migration_internal: deleting system-link host migration description 0x%8X"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_host_migration_internal: deleting system-link host migration description 0x%08X";
		break;
	case 0x01624FB8: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_host_migration_internal: async deleting host migration 0x%8X [%s]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_delete_host_migration_internal: async deleting host migration 0x%08X [%s]";
		break;
	case 0x01625038: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_build_removal_xuid_list: session 0x%8X, operating on %d xuids"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_build_removal_xuid_list: session 0x%08X, operating on %d xuids";
		break;
	case 0x016250B0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_build_add_xuid_list: session 0x%8X, operating on %d xuids"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_build_add_xuid_list: session 0x%08X, operating on %d xuids";
		break;
	case 0x01625308: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_creation_complete: session 0x%8X, async session create failed"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_creation_complete: session 0x%08X, async session create failed";
		break;
	case 0x01625380: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_creation_complete: session 0x%8X, async session create completely successfully"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_creation_complete: session 0x%08X, async session create completely successfully";
		break;
	case 0x01625408: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_deletion_complete: session 0x%8X, async session delete failed, forcing a successful completion"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_deletion_complete: session 0x%08X, async session delete failed, forcing a successful completion";
		break;
	case 0x016254A0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_deletion_complete: session 0x%8X, async session delete completed successfully"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_deletion_complete: session 0x%08X, async session delete completed successfully";
		break;
	case 0x01625590: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_host_migration_creation_complete: session 0x%8X, active migration session is '%s'"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_host_migration_creation_complete: session 0x%08X, active migration session is '%s'";
		break;
	case 0x016256D8: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_host_migration_complete: session 0x%8X, async host-migration wound up with a zero session descripton, bailing..."
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_host_migration_complete: session 0x%08X, async host-migration wound up with a zero session descripton, bailing...";
		break;
	case 0x01625780: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_host_migration_complete: session 0x%8X, async host-migration create completed successfully [%s]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_host_migration_complete: session 0x%08X, async host-migration create completed successfully [%s]";
		break;
	case 0x01625818: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_host_migration_complete: session 0x%8X, host migration failed (%08X), reset"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_host_migration_complete: session 0x%08X, host migration failed (%08X), reset";
		break;
	case 0x016258A0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_modify_complete: session 0x%8X, async session modify completed successfully"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_modify_complete: session 0x%08X, async session modify completed successfully";
		break;
	case 0x01625930: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_modify_complete: session 0x%8X, async session modify failed"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_modify_complete: session 0x%08X, async session modify failed";
		break;
	case 0x016259A8: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_players_add_complete: session 0x%8X, player add completed successfully"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_players_add_complete: session 0x%08X, player add completed successfully";
		break;
	case 0x01625A30: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_players_add_complete: session 0x%8X, operating xuid operation (%d) complete"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_players_add_complete: session 0x%08X, operating xuid operation (%d) complete";
		break;
	case 0x01625AC0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_players_add_complete: session 0x%8X, player add failed"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_players_add_complete: session 0x%08X, player add failed";
		break;
	case 0x01625B30: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_players_add_complete: session 0x%8X, operating xuid operation (%d) complete"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_players_add_complete: session 0x%08X, operating xuid operation (%d) complete";
		break;
	case 0x01625BB8: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_players_remove_complete: session 0x%8X, player remove completed successfully"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_players_remove_complete: session 0x%08X, player remove completed successfully";
		break;
	case 0x01625C48: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_players_remove_complete: session 0x%8X, operating xuid operation (%d) complete"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_players_remove_complete: session 0x%08X, operating xuid operation (%d) complete";
		break;
	case 0x01625CD8: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_players_remove_complete: session 0x%8X, player remove failed"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_players_remove_complete: session 0x%08X, player remove failed";
		break;
	case 0x01625D50: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_players_remove_complete: session 0x%8X, operating xuid operation (%d) complete"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_players_remove_complete: session 0x%08X, operating xuid operation (%d) complete";
		break;
	case 0x01625DD8: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_game_start_complete: session 0x%8X, game start completed successfully"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_game_start_complete: session 0x%08X, game start completed successfully";
		break;
	case 0x01625E60: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_game_start_complete: session 0x%8X, game start failed"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_game_start_complete: session 0x%08X, game start failed";
		break;
	case 0x01625ED0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_game_end_complete: session 0x%8X, game end completed successfully"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_successful_game_end_complete: session 0x%08X, game end completed successfully";
		break;
	case 0x01625F58: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_game_end_complete: session 0x%8X, game end failed"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_game_end_complete: session 0x%08X, game end failed";
		break;
	case 0x01628730: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameters::handle_update: [%s] failed to handle update (params 0x%8X/cleared 0x%08X/initial %s)"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameters::handle_update: [%s] failed to handle update (params 0x%08X/cleared 0x%08X/initial %s)";
		break;
	case 0x01628A98: // "MP/NET/SESSION,PARAMS: c_network_session_parameters::handle_change_request: [%s] parameter %d [%s] change request size out of bounds of payload size [0x%8X 0x%8X 0x%8X]"
		format = "MP/NET/SESSION,PARAMS: c_network_session_parameters::handle_change_request: [%s] parameter %d [%s] change request size out of bounds of payload size [0x%08X 0x%08X 0x%08X]";
		break;
	case 0x01628D88: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_game_variant,6144>::get_update_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%8X 0x%8X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_game_variant,6144>::get_update_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%08X 0x%08X]";
		break;
	case 0x01629270: // "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_game_variant,6144>::handle_update_payload: [%s] parameter %d [%s] failed to verify checksum update for apply [%0x%8X != %0x%8X]"
		format = "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_game_variant,6144>::handle_update_payload: [%s] parameter %d [%s] failed to verify checksum update for apply [%0x%08X != %0x%08X]";
		break;
	case 0x016294F0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_game_variant,6144>::get_change_request_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%8X 0x%8X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_game_variant,6144>::get_change_request_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%08X 0x%08X]";
		break;
	case 0x016299F0: // "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_game_variant,6144>::handle_change_request_payload: [%s] parameter %d [%s] checksum mismatch on apply [%0x%8X != %0x%8X]"
		format = "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_game_variant,6144>::handle_change_request_payload: [%s] parameter %d [%s] checksum mismatch on apply [%0x%08X != %0x%08X]";
		break;
	case 0x01629E28: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_map_variant,28672>::get_update_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%8X 0x%8X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_map_variant,28672>::get_update_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%08X 0x%08X]";
		break;
	case 0x0162A310: // "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_map_variant,28672>::handle_update_payload: [%s] parameter %d [%s] failed to verify checksum update for apply [%0x%8X != %0x%8X]"
		format = "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_map_variant,28672>::handle_update_payload: [%s] parameter %d [%s] failed to verify checksum update for apply [%0x%08X != %0x%08X]";
		break;
	case 0x0162A590: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_map_variant,28672>::get_change_request_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%8X 0x%8X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_map_variant,28672>::get_change_request_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%08X 0x%08X]";
		break;
	case 0x0162AA90: // "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_map_variant,28672>::handle_change_request_payload: [%s] parameter %d [%s] checksum mismatch on apply [%0x%8X != %0x%8X]"
		format = "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_map_variant,28672>::handle_change_request_payload: [%s] parameter %d [%s] checksum mismatch on apply [%0x%08X != %0x%08X]";
		break;
	case 0x0162AEE0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_saved_film_game_options,34816>::get_update_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%8X 0x%8X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_saved_film_game_options,34816>::get_update_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%08X 0x%08X]";
		break;
	case 0x0162B410: // "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_saved_film_game_options,34816>::handle_update_payload: [%s] parameter %d [%s] failed to verify checksum update for apply [%0x%8X != %0x%8X]"
		format = "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_saved_film_game_options,34816>::handle_update_payload: [%s] parameter %d [%s] failed to verify checksum update for apply [%0x%08X != %0x%08X]";
		break;
	case 0x0162B6B0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_saved_film_game_options,34816>::get_change_request_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%8X 0x%8X]"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session_parameter_chunked<struct s_network_session_parameter_saved_film_game_options,34816>::get_change_request_payload: [%s] type %d [%s] bad chunk count/encoded size? [0x%08X 0x%08X]";
		break;
	case 0x0162BC00: // "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_saved_film_game_options,34816>::handle_change_request_payload: [%s] parameter %d [%s] checksum mismatch on apply [%0x%8X != %0x%8X]"
		format = "MP/NET/SESSION,PARAMS: c_network_session_parameter_chunked<struct s_network_session_parameter_saved_film_game_options,34816>::handle_change_request_payload: [%s] parameter %d [%s] checksum mismatch on apply [%0x%08X != %0x%08X]";
		break;
	case 0x01639450: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::idle_peer_creating: creation failed for managed session [client] 0x%8X"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::idle_peer_creating: creation failed for managed session [client] 0x%08X";
		break;
	case 0x016394D0: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::idle_peer_creating: failed to get security information from managed session 0x%8X"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::idle_peer_creating: failed to get security information from managed session 0x%08X";
		break;
	case 0x01639788: // "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::idle_peer_creating: managed session creation neither succeeded nor failed...but is no longer happening [client] 0x%8X"
		format = "MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_session::idle_peer_creating: managed session creation neither succeeded nor failed...but is no longer happening [client] 0x%08X";
		break;
	}

	va_list list;
	va_start(list, format);
	c_console::write_line(format, list);
	va_end(list);
}

