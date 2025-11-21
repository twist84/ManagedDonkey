#include "networking/logic/network_banhammer.hpp"

REFERENCE_DECLARE(0x0226DAD0, bool, g_network_banhammer_initialized);
REFERENCE_DECLARE(0x0226DAD8, s_network_banhammer_globals, g_network_banhammer_globals);

//.text:004D7960 ; bool __cdecl network_banhammer_check_manifest_for_file_hash(const s_rsa_signature*, e_map_id)
//.text:004D7A30 ; void __cdecl network_banhammer_controller_caught_cheating(e_controller_index, uns32)
//.text:004D7A70 ; 
//.text:004D7B10 ; bool __cdecl network_banhammer_controller_is_xuid(e_controller_index, uns64)
//.text:004D7B70 ; void __cdecl network_banhammer_find_cheaters_in_current_session()
//.text:004D7C60 ; void __cdecl network_banhammer_get_cheating_report(s_network_banhammer_cheating_report*)
//.text:004D7CF0 ; uns32 __cdecl network_banhammer_get_controller_ban_flags(e_controller_index)
//.text:004D7D20 ; uns32 __cdecl network_banhammer_get_controller_ban_message_flags(e_controller_index)
//.text:004D7D50 ; uns32 __cdecl network_banhammer_get_controller_cheat_flags(e_controller_index)
//.text:004D7D80
//.text:004D7DB0
//.text:004D7DD0 ; s_network_banhammer_repeated_play_list* __cdecl network_banhammer_get_controller_repeated_play_list(e_controller_index)
//.text:004D7E00 ; int32 __cdecl network_banhammer_get_highest_skill(e_controller_index)
//.text:004D7E30 ; void __cdecl network_banhammer_get_local_players_for_web_event(s_network_web_event_local_player*, int32, int32*)

bool __cdecl network_banhammer_initialize()
{
	return INVOKE(0x004D7EC0, network_banhammer_initialize);
}

//.text:004D7F90 ; 
//.text:004D8020 ; e_hopper_load_status __cdecl network_banhammer_loaded_for_all_controllers()
//.text:004D80F0 ; e_network_file_load_status __cdecl network_banhammer_loaded_for_controller(e_controller_index, bool*)
//.text:004D8190 ; e_network_file_load_status __cdecl network_banhammer_loaded_for_all_controllers()
//.text:004D81C0 ; void __cdecl network_banhammer_local_box_caught_cheating(uns32)
//.text:004D8200 ; void __cdecl network_banhammer_remote_player_caught_cheating(const uns64*, const s_transport_secure_address*, const wchar_t*, const uns64*, uns32)
//.text:004D82D0 ; e_network_file_load_status __cdecl network_banhammer_repeated_play_list_loaded_for_all_controllers()
//.text:004D8330 ; void __cdecl network_banhammer_report_on_cheaters()
//.text:004D8510 ; void __cdecl network_banhammer_set_controller_experience_growth_banned(e_controller_index, bool)
//.text:004D8580 ; void __cdecl network_banhammer_set_controller_repeated_play_coefficient(e_controller_index, int32)
//.text:004D85C0 ; void __cdecl network_banhammer_set_highest_skill(e_controller_index, int32)

void __cdecl network_banhammer_update()
{
	INVOKE(0x004D85F0, network_banhammer_update);
}

//.text:004D8610 ; void __cdecl network_banhammer_update_controllers()
//.text:004D90B0 ; void __cdecl network_banhammer_update_for_new_map()
//.text:004D90E0 ; void __cdecl network_banhammer_update_live_ban()
//.text:004D91B0 ; void __cdecl network_banhammer_update_machine_network_statistics()
//.text:004D9730 ; void __cdecl network_banhammer_update_rsa_manifest()
//.text:004D9820 ; 

void __cdecl network_banhammer_upload_network_statistics()
{
	INVOKE(0x004D9840, network_banhammer_upload_network_statistics);
}

