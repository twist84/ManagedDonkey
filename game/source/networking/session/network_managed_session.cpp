#include "networking/session/network_managed_session.hpp"

REFERENCE_DECLARE(0x02247448, s_online_session_manager_globals, online_session_manager_globals);

//.text:00442C00 ; void __cdecl c_managed_session_overlapped_task::process_add_players(long, void(__cdecl*)(long, bool, dword), s_online_session*, qword const*, bool const*, bool const*, long);
//.text:00442C10 ; bool __cdecl c_managed_session_overlapped_task::process_add_players_immediately(s_online_session*, qword const*, bool const*, bool const*, long)
//.text:00442C20 ; void __cdecl c_managed_session_overlapped_task::process_create(long, void(__cdecl*)(long, bool, dword), s_online_session*, c_flags<e_online_session_flags, word, 9>);
//.text:00442C30 ; void __cdecl c_managed_session_overlapped_task::process_delete(long, void(__cdecl*)(long, bool, dword), s_online_session*);
//.text:00442C40 ; void __cdecl c_managed_session_overlapped_task::process_game_end(long, void(__cdecl*)(long, bool, dword), s_online_session*);
//.text:00442C50 ; void __cdecl c_managed_session_overlapped_task::process_game_start(long, void(__cdecl*)(long, bool, dword), s_online_session*);
//.text:00442C60 ; void __cdecl c_managed_session_overlapped_task::process_modify(long, void(__cdecl*)(long, bool, dword), s_online_session*, s_online_session*, s_online_session*);
//.text:00442C70 ; static bool __cdecl c_managed_session_overlapped_task::process_modify_immediately(s_online_session*, s_online_session*);
//.text:00442C80 ; void __cdecl c_managed_session_overlapped_task::process_remove_players(long, void(__cdecl*)(long, bool, dword), s_online_session*, qword const*, bool const*, long);
//.text:00442C90 ; bool __cdecl c_managed_session_overlapped_task::process_remove_players_immediately(s_online_session*, qword const*, bool const*, long);
//.text:00442CA0 ; void __cdecl c_managed_session_overlapped_task::process_session_host_migrate(long, void(__cdecl*)(long, bool, dword), s_online_session*, bool, s_transport_session_description*);
//.text:00442CB0 ; virtual dword __cdecl c_managed_session_overlapped_task::start(void*);

//.text:00480200 ; c_managed_session_overlapped_task* __cdecl c_managed_session_overlapped_task::c_managed_session_overlapped_task(char const*, long);
//.text:00480220 ; c_managed_session_overlapped_task* __cdecl c_managed_session_overlapped_task::c_managed_session_overlapped_task();
// 
//.text:00480240 ; s_online_managed_session* __cdecl s_online_managed_session::s_online_managed_session();
//.text:004802A0 ; s_online_managed_session* __cdecl s_online_managed_session::~s_online_managed_session();
// 
//.text:00480350 ; void __cdecl add_to_player_list(s_online_session_player*, long, qword const*, bool const*, bool const*, long);
// 
//.text:00480420 ; virtual void __cdecl c_managed_session_overlapped_task::complete();
//.text:00480440 ; virtual void __cdecl c_managed_session_overlapped_task::failure(dword, dword, dword);
//.text:004804B0 ; virtual char const* __cdecl c_managed_session_overlapped_task::get_context_string() const;

//.text:00480630 ; void __cdecl managed_session_add_players(long, qword const*, bool const*, bool const*, long);
//.text:00480680 ; long __cdecl managed_session_allocate();
//.text:00480790 ; bool __cdecl managed_session_build_add_xuid_list(long);
//.text:00480840 ; bool __cdecl managed_session_build_removal_xuid_list(long);
//.text:00480900 ; bool __cdecl managed_session_can_create_session(e_network_session_class);
//.text:00480950 ; void __cdecl managed_session_cancel_new_host(long);
//.text:00480D10 ; void __cdecl managed_session_commit_new_host(long);
//.text:00480DD0 ; bool __cdecl managed_session_compare_id(long, s_transport_secure_identifier const*);
//.text:00480E60 ; void __cdecl managed_session_completely_reset_session_on_error(long);
//.text:00480EF0 ; void __cdecl managed_session_connect_client_to_new_host(long, s_transport_session_description const*, e_transport_platform);
//.text:00481070 ; long __cdecl managed_session_count_master_sessions();
//.text:00481110 ; long __cdecl managed_session_create_client(e_network_session_class, c_flags<e_managed_session_creation_flags, byte, 4>, e_transport_platform, long, long, s_transport_secure_identifier const*, s_transport_secure_address const*, s_transport_secure_key const*, s_online_context const*, long);
//.text:00481270 ; long __cdecl managed_session_create_host(e_network_session_class, c_flags<e_managed_session_creation_flags, byte, 4>, e_transport_platform, long, long, s_online_context const*, long);
//.text:00481370 ; void __cdecl managed_session_create_host_migration_internal(long, s_online_managed_session*);
//.text:00481550 ; void __cdecl managed_session_create_session_internal(long, s_online_managed_session*);
//.text:00481930 ; void __cdecl managed_session_creation_complete(long, bool, dword);
//.text:00481A80 ; void __cdecl managed_session_delete(long);
//.text:00481B10 ; void __cdecl managed_session_delete_host_migration_internal(long, s_online_managed_session*);
//.text:00481C10 ; void __cdecl managed_session_delete_session_internal(long, s_online_managed_session*);
//.text:00481D30 ; void __cdecl managed_session_deletion_complete(long, bool, dword);
//.text:00481DE0 ; void __cdecl managed_session_free(long);
//.text:00481E10 ; void __cdecl managed_session_game_end(long);
//.text:00481EB0 ; void __cdecl managed_session_game_end_complete(long, bool, dword);
//.text:00481F10 ; void __cdecl managed_session_game_start(long);
//.text:00481F80 ; void __cdecl managed_session_game_start_complete(long, bool, dword);
//.text:00481FE0 ; s_online_managed_session* __cdecl managed_session_get(long);
//.text:00482000 ; bool __cdecl managed_session_get_handle(long, void**);

bool __cdecl managed_session_get_id(long index, s_transport_secure_identifier* session_id)
{
    return INVOKE(0x00482040, managed_session_get_id, index, session_id);
}

char const* __cdecl managed_session_get_id_string(long index)
{
    //INVOKE(0x004820A0, managed_session_get_id_string, index);

    if (index == -1)
        return "00:00:00:00:00:00:00:00";
    else
        return transport_secure_identifier_get_string(&online_session_manager_globals.managed_sessions[index].actual_online_session_state.description.id);
}

//.text:004820D0 ; void __cdecl managed_session_get_new_host_information(long, s_transport_session_description*, e_transport_platform*);
//.text:00482140 ; bool __cdecl managed_session_get_nonce(long, qword*);
//.text:00482170 ; bool __cdecl managed_session_get_security_information(long, bool, s_transport_session_description*, e_transport_platform*);
//.text:00482280 ; c_flags<e_managed_session_status_flags, word, 14> __cdecl managed_session_get_status(long);
//.text:004823C0 ; bool __cdecl managed_session_handle_valid(void*);
//.text:00482410 ; void __cdecl managed_session_host_migration_complete(long, bool, dword);
//.text:00482500 ; bool __cdecl managed_session_is_host(long);
//.text:00482530 ; bool __cdecl managed_session_is_live(long);
//.text:00482560 ; bool __cdecl managed_session_is_master_session(long);
//.text:00482590 ; void __cdecl managed_session_mark_session_to_be_created_offline(long);
//.text:004825B0 ; void __cdecl managed_session_modify_complete(long, bool, dword);
//.text:00482650 ; void __cdecl managed_session_modify_slot_counts(long, long, long, bool, bool);
//.text:00482750 ; void __cdecl managed_session_players_add_complete(long, bool, dword);
//.text:004827F0 ; void __cdecl managed_session_players_remove_complete(long, bool, dword);
//.text:00482880 ; void __cdecl managed_session_process_pending_operations(long);
//.text:00482D60 ; void __cdecl managed_session_remove_players(long, qword const*, long);
//.text:00482DB0 ; void __cdecl managed_session_reset_players_add_status(long);
//.text:00482DD0 ; void __cdecl managed_session_reset_session(long, bool);
//.text:00482E70 ; long __cdecl managed_session_retrieve_security_information(transport_address const*, e_transport_platform, s_transport_session_description*, s_transport_secure_address*);
//.text:00482FC0 ; void __cdecl managed_session_start_host_migration(long);
//.text:00483040 ; void __cdecl managed_session_successful_creation_complete(long);
//.text:004831F0 ; void __cdecl managed_session_successful_delete_complete(long);
//.text:00483290 ; void __cdecl managed_session_successful_delete_host_migration_complete(long);
//.text:004832D0 ; void __cdecl managed_session_successful_game_end_complete(long);
//.text:00483300 ; void __cdecl managed_session_successful_game_start_complete(long);
//.text:00483330 ; void __cdecl managed_session_successful_host_migration_creation_complete(long);
//.text:004833F0 ; void __cdecl managed_session_successful_modify_complete(long);
//.text:00483440 ; void __cdecl managed_session_successful_players_add_complete(long);
//.text:004834D0 ; void __cdecl managed_session_successful_players_remove_complete(long);
//.text:00483540 ; bool __cdecl managed_session_synchronize_to_player_list(long, c_network_session_membership*);
//.text:004837D0 ; void __cdecl managed_session_use_new_host(long);
//.text:004839C0 ; long __cdecl managed_session_who_has_this_session_id(s_transport_secure_identifier const*);

//.text:00483AF0 ; void __cdecl online_session_manager_dispose();
//.text:00483B10 ; void __cdecl online_session_manager_initialize();
//.text:00483B20 ; void __cdecl online_session_manager_update();

//.text:00483B50 ; void __cdecl remove_from_player_list(s_online_session_player*, long, qword const*, long);

//.text:00483CB0 ; virtual void __cdecl c_managed_session_overlapped_task::success(dword)

