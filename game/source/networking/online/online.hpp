#pragma once

#include "cseries/cseries.hpp"
#include "text/unicode.hpp"

enum e_online_nat_type
{
	_online_nat_type_none = 0,
	_online_nat_type_open,
	_online_nat_type_moderate,
	_online_nat_type_strict,

	k_online_nat_type_count
};

enum e_controller_index;

extern void __cdecl online_dispose();
extern void __cdecl online_dump_machine_info(uint64 game_instance);
extern e_online_nat_type __cdecl online_get_maximum_compatible_nat_type(e_online_nat_type nat_type);
extern e_online_nat_type __cdecl online_get_nat_type();
extern void __cdecl online_get_title_name_string(uint32 title_id, wchar_t* buffer, long buffer_length);
extern bool __cdecl online_has_all_online_enabled_users();
extern bool __cdecl online_has_any_silver_or_gold_live_users();
extern void __cdecl online_initialize();
extern bool __cdecl online_is_connected_to_live();
extern wchar_t const* __cdecl online_local_user_get_name(e_controller_index controller_index);
extern uint64 __cdecl online_local_user_get_player_identifier(e_controller_index controller_index);
extern uint64 __cdecl online_local_user_get_xuid(e_controller_index controller_index);
extern bool __cdecl online_local_user_is_free_live_gold_account(e_controller_index controller_index);
extern bool __cdecl online_local_user_is_friend_created_content_allowed(e_controller_index controller_index);
extern bool __cdecl online_local_user_is_online_enabled(e_controller_index controller_index);
extern bool __cdecl online_local_user_is_signed_in(e_controller_index controller_index);
extern bool __cdecl online_local_user_is_silver_or_gold_live(e_controller_index controller_index);
extern bool __cdecl online_local_user_is_user_created_content_allowed(e_controller_index controller_index);
extern void __cdecl online_process_debug_output_queue();
extern void __cdecl online_user_set_xuid(uint64 xuid);
extern void __cdecl online_set_is_connected_to_live(bool is_connected_to_live);
extern void __cdecl online_user_set_name(long user_index, wchar_t const* name);
extern void __cdecl online_update();