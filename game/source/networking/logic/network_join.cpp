#include "networking/logic/network_join.hpp"

REFERENCE_DECLARE(0x02240B80, s_networking_join_data, g_network_join_data);

//.text:0047DC70 ; void __cdecl network_join_add_join_to_queue(c_network_session*, const transport_address*, const s_network_session_join_request*)
//.text:0047DF10 ; void __cdecl network_join_calculate_full_queue_desirability(c_network_session_membership* membership)
//.text:0047E040 ; void __cdecl network_join_clear_group_join_result()
//.text:0047E050 ; void __cdecl network_join_clear_squad_join_result()
//.text:0047E0D0 ; void __cdecl network_join_disable_outgoing_joins(bool)
//.text:0047E0E0 ; void __cdecl network_join_flush_join_queue()
//.text:0047E160 ; void __cdecl network_join_flush_our_join_from_join_queue(const s_transport_secure_address*, uns64)
//.text:0047E270 ; e_life_cycle_join_result __cdecl network_join_get_group_join_result()
//.text:0047E280 ; e_network_join_queue_mode __cdecl network_join_get_join_queue_mode()
//.text:0047E290 ; e_life_cycle_join_result __cdecl network_join_get_squad_join_result()
//.text:0047E340 ; void __cdecl network_join_group_to_remote_group(const s_transport_secure_identifier*, const s_transport_secure_key*, const s_transport_secure_address*, const s_group_session_join_request_payload*)
//.text:0047E400 ; bool __cdecl network_join_is_any_join_in_progress()
//.text:0047E460 ; bool __cdecl network_join_is_any_join_in_progress_or_join_queue_active()
//.text:0047E490 ; bool __cdecl network_join_is_join_to_squad_in_progress()
//.text:0047E4B0 ; bool __cdecl network_join_is_leaving_group()
//.text:0047E510 ; void __cdecl network_join_leave_group_session_and_swap_if_necessary()
//.text:0047E540 ; void __cdecl network_join_local_squad_to_local_group()
//.text:0047E580 ; void __cdecl network_join_local_squad_to_remote_group(const s_transport_secure_identifier*, const s_transport_secure_key*, const s_transport_secure_address*, const s_group_session_join_request_payload*)
//.text:0047E640 ; void __cdecl network_join_local_squad_to_remote_target_group(const s_transport_secure_identifier*, const s_transport_secure_key*, const s_transport_secure_address*, const s_group_session_join_request_payload*)
//.text:0047E700 ; e_life_cycle_join_result __cdecl network_join_map_refuse_reason_to_join_result(e_network_join_refuse_reason, bool, const c_network_session*)
//.text:0047E8A0 ; int32 __cdecl network_join_number_of_joins_in_queue()
//.text:0047E8B0 ; bool __cdecl network_join_process_joins_from_queue()	
//.text:0047EA50 ; void __cdecl network_join_queue_update()
//.text:0047EBB0 ; void __cdecl network_join_remove_join_from_queue(uns64)
//.text:0047EC40 ; void __cdecl network_join_remove_join_from_queue_internal(s_networking_join_queue_entry*)

void __cdecl network_join_set_join_queue_mode(e_network_join_queue_mode join_queue_mode)
{
	INVOKE(0x0047EC70, network_join_set_join_queue_mode, join_queue_mode);
}

//.text:0047ECD0 ; void __cdecl network_join_squad_join_abort()
//.text:0047ECF0 ; bool __cdecl network_join_to_remote_squad(e_network_join_squad_join_source, bool, bool, bool, e_network_session_class, const s_transport_secure_identifier*, const s_transport_secure_key*, const s_transport_secure_address*)

void __cdecl network_join_update()
{
	INVOKE(0x0047EEA0, network_join_update);
}

