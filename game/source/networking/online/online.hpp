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

union u_online_user_id
{
	qword value;
	byte bytes[sizeof(qword)];
};

struct s_online_user
{
	bool initialized;
	qword player_xuid;
	c_static_wchar_string<16> player_name;
};
static_assert(sizeof(s_online_user) == 0x30);

extern s_online_user& g_online_user;

extern void __cdecl online_dispose();
extern void __cdecl online_dump_machine_info(qword game_instance);
extern e_online_nat_type __cdecl online_get_maximum_compatible_nat_type(e_online_nat_type nat_type);
extern e_online_nat_type __cdecl online_get_nat_type();
extern void __cdecl online_get_title_name_string(dword title_id, wchar_t* buffer, long buffer_length);
extern bool __cdecl online_has_all_online_enabled_users();
extern bool __cdecl online_has_any_silver_or_gold_live_users();
extern void __cdecl online_initialize();
extern bool __cdecl online_is_connected_to_live();
extern wchar_t const* __cdecl online_user_get_name(long controller_index);
extern qword __cdecl online_user_get_player_identifier(long controller_index);
extern qword __cdecl online_user_get_xuid(long controller_index);
extern bool __cdecl sub_442B00(long controller_index);
extern bool __cdecl online_local_xuid_is_online_enabled(long controller_index);
extern bool __cdecl online_has_signed_in_user(long controller_index);
extern bool __cdecl online_local_xuid_is_silver_or_gold_live(long controller_index);
extern bool __cdecl sub_442B70();
extern void __cdecl online_process_debug_output_queue();
extern void __cdecl online_user_set_xuid(qword xuid);
extern void __cdecl online_set_is_connected_to_live(bool is_connected_to_live);
extern void __cdecl online_user_set_name(wchar_t const* name);
extern void __cdecl online_update();