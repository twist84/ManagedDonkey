#include "networking/online/online.hpp"

#include "cseries/cseries_events.hpp"
#include "interface/user_interface_controller.hpp"
#include "memory/module.hpp"
#include "networking/network_memory.hpp"
#include "networking/online/online_arbitration_windows.hpp"
#include "text/unicode.hpp"
#include "xbox/xnet.hpp"

#include <winsock.h>

//REFERENCE_DECLARE(0x019AB728, s_online_user_globals_old, g_online_user);
REFERENCE_DECLARE(0x02179468, bool, g_online_is_connected_to_live);

HOOK_DECLARE(0x004429C0, online_dispose);
HOOK_DECLARE(0x004429D0, online_dump_machine_info);
HOOK_DECLARE(0x004429F0, online_get_maximum_compatible_nat_type);
HOOK_DECLARE(0x00442A00, online_get_nat_type);
HOOK_DECLARE(0x00442A20, online_get_title_name_string);
HOOK_DECLARE(0x00442A50, online_has_all_online_enabled_users);
HOOK_DECLARE(0x00442A60, online_has_any_silver_or_gold_live_users);
HOOK_DECLARE(0x00442A70, online_initialize);
HOOK_DECLARE(0x00442A90, online_is_connected_to_live);
HOOK_DECLARE(0x00442AA0, online_local_user_get_name);
HOOK_DECLARE(0x00442AE0, online_local_user_get_xuid);
HOOK_DECLARE(0x00442AB0, online_local_user_get_player_identifier);
HOOK_DECLARE(0x00442B00, online_local_user_is_free_live_gold_account);
HOOK_DECLARE(0x00442B10, online_local_user_is_friend_created_content_allowed);
HOOK_DECLARE(0x00442B20, online_local_user_is_online_enabled);
HOOK_DECLARE(0x00442B40, online_local_user_is_signed_in);
HOOK_DECLARE(0x00442B60, online_local_user_is_silver_or_gold_live);
HOOK_DECLARE(0x00442B70, online_local_user_is_user_created_content_allowed);
HOOK_DECLARE(0x00442B80, online_process_debug_output_queue);
HOOK_DECLARE(0x00442B90, online_user_set_xuid);
HOOK_DECLARE(0x00442BB0, online_set_is_connected_to_live);
HOOK_DECLARE(0x00442BC0, online_user_set_name);
HOOK_DECLARE(0x00442BF0, online_update);

struct s_online_user_globals
{
	bool is_signed_in;
	bool is_silver_or_gold_xbox_live;
	bool is_online_enabled;
	bool is_free_live_gold;
	bool is_user_created_content_allowed;
	bool is_friend_created_content_allowed;
	int32 guest_number;
	uns64 player_identifier;
	uns64 local_xuid;
	uns64 online_xuid;
	wchar_t* online_xuid_string;
	wchar_t* gamertag;
	wchar_t* display_name;
};
static_assert(sizeof(s_online_user_globals) == 0x38);

struct s_online_globals
{
	s_online_user_globals users[4];
};
static_assert(sizeof(s_online_globals) == sizeof(s_online_user_globals) * 4);

s_online_globals online_globals{};

int32 g_nat_type_override = _online_nat_type_open;

uns64 hash64(uns64 h, uns64 s = 0x3B9F1629F7D0609Eu)
{
	// murmur64
	// https://lemire.me/blog/2018/08/15/fast-strongly-universal-64-bit-hashing-everywhere/
	h ^= s;
	h ^= h >> 33u;
	h *= 0XFF51AFD7ED558CCDu;
	h ^= h >> 33u;
	h *= 0xC4CEB9FE1A85EC53u;
	h ^= h >> 33u;
	return h;
}

constexpr bool xuid_is_guest(uns64 xuid)
{
	uns64 guest_bits = xuid >> 48;
	return (guest_bits & 15) == 9 && (guest_bits & 192) != 0;
}

uns64 xuid_make_online(uns64 xuid)
{
	uns64 online_xuid = xuid;
	if (xuid_is_guest(online_xuid))
		online_xuid &= ~0b11001001000000000000000000000000000000000000000000000000;
	return online_xuid;
}

void __cdecl online_dispose()
{
	for (e_controller_index controller_index = _controller0; controller_index < k_number_of_controllers; controller_index++)
	{
		s_online_user_globals& user = online_globals.users[controller_index];

		if (user.online_xuid_string)
		{
			free(user.online_xuid_string);
			user.online_xuid_string = NULL;
		}
		if (user.gamertag)
		{
			free(user.gamertag);
			user.gamertag = NULL;
		}
		if (user.display_name)
		{
			free(user.display_name);
			user.display_name = NULL;
		}
	}

	online_arbitration_dispose();
	//online_statistics_dispose();
	//online_session_search_dispose();
}

void __cdecl online_dump_machine_info(uns64 game_instance)
{
	event(_event_message, "networking:online:machine_info: game instance-%0I64X", game_instance);
}

e_online_nat_type __cdecl online_get_maximum_compatible_nat_type(e_online_nat_type nat_type)
{
	if (nat_type >= k_online_nat_type_count)
	{
		event(_event_error, "online: unexpected nat type %d passed to online_get_maximum_compatible_nat_type()", nat_type);
		return _online_nat_type_open;
	}

	switch (nat_type)
	{
	case _online_nat_type_unknown:  return _online_nat_type_unknown;
	case _online_nat_type_open:     return _online_nat_type_open;
	case _online_nat_type_moderate: return _online_nat_type_moderate;
	case _online_nat_type_strict:   return _online_nat_type_strict;
	}

	return _online_nat_type_unknown;
}

e_online_nat_type __cdecl online_get_nat_type()
{
	int32 result = _online_nat_type_unknown;
	//XONLINE_NAT_TYPE xnat_type XOnlineGetNatType()

	if (!online_is_connected_to_live() /* || xnat_type == XONLINE_NAT_OPEN*/)
	{
		result = _online_nat_type_open;
	}
	//else if (xnat_type == XONLINE_NAT_MODERATE)
	//{
	//	result = _online_nat_type_moderate;
	//}
	//else if (xnat_type == XONLINE_NAT_STRICT)
	//{
	//	result = _online_nat_type_strict;
	//}
	//else
	//{
	//	event(_event_error, "online: XOnlineGetNatType returned unknown NAT type 0x%08lX", xnat_type);
	//}

	if (g_nat_type_override)
	{
		if (g_nat_type_override < _online_nat_type_open || g_nat_type_override >= k_online_nat_type_count)
		{
			event(_event_error, "online: invalid NAT override %d, resetting", g_nat_type_override);
			g_nat_type_override = _online_nat_type_unknown;
		}
		else
		{
			result = g_nat_type_override;
		}
	}

	return static_cast<e_online_nat_type>(result);
}

void __cdecl online_get_title_name_string(uns32 title_id, wchar_t* buffer, int32 buffer_length)
{
	usnzprintf(buffer, buffer_length, L"%08lX", title_id);
}

bool __cdecl online_has_all_online_enabled_users()
{
	for (e_controller_index controller_index = _controller0; controller_index < k_number_of_controllers; controller_index++)
	{
		if (online_local_user_is_signed_in(controller_index) && !online_local_user_is_online_enabled(controller_index))
		{
			return false;
		}
	}
	return true;
}

bool __cdecl online_has_any_silver_or_gold_live_users()
{
	for (e_controller_index controller_index = _controller0; controller_index < k_number_of_controllers; controller_index++)
	{
		if (!online_local_user_is_silver_or_gold_live(controller_index))
			return true;
	}
	return false;
}

void __cdecl online_initialize()
{
	online_arbitration_initialize();
	//online_statistics_initialize();
	//online_session_search_initialize();

	for (e_controller_index controller_index = _controller0; controller_index < k_number_of_controllers; controller_index++)
	{
		uns64 user_id = hash64(controller_index);

		wchar_t computer_name[MAX_COMPUTERNAME_LENGTH + 1];
		uns32 computer_name_length = MAX_COMPUTERNAME_LENGTH + 1;
		if (!GetComputerNameExW(ComputerNamePhysicalDnsHostname, computer_name, &computer_name_length))
		{
			ustrnzcpy(computer_name, L"User", MAX_COMPUTERNAME_LENGTH);
		}

		computer_name_length = ustrnlen(computer_name, MAX_COMPUTERNAME_LENGTH);
		if (controller_index > 0)
		{
			if (computer_name_length >= MAX_COMPUTERNAME_LENGTH - 3)
			{
				computer_name[computer_name_length - 3] = 0;
			}

			usnzappend(computer_name, MAX_COMPUTERNAME_LENGTH + 1, L"(%li)", controller_index);
			computer_name_length = ustrnlen(computer_name, MAX_COMPUTERNAME_LENGTH);
		}
		for (uns32 name_hash_index = 0; name_hash_index < computer_name_length; name_hash_index++)
		{
			user_id = hash64(computer_name[controller_index], user_id);
		}

		s_online_user_globals& user = online_globals.users[controller_index];

		transport_secure_random(sizeof(user.player_identifier), (byte*)&user.player_identifier);
		if (!user.player_identifier)
		{
			user.player_identifier = 3LL;
		}

		user.is_signed_in = controller_index < 1;
		user.is_silver_or_gold_xbox_live = user.is_signed_in;
		user.is_online_enabled = user.is_signed_in;
		user.is_free_live_gold = false;
		user.is_user_created_content_allowed = user.is_signed_in;
		user.is_friend_created_content_allowed = user.is_signed_in;
		user.guest_number = NONE;
		user.local_xuid = user_id;
		user.online_xuid = xuid_make_online(user_id);
		user.online_xuid_string = _wcsdup(computer_name);
		user.gamertag = _wcsdup(computer_name);
		user.display_name = _wcsdup(computer_name);

		//user_interface_controller_set_user_index(controller_index, (int32)controller_index);
	}
}

bool __cdecl online_is_connected_to_live()
{
	return g_online_is_connected_to_live;
}

const wchar_t* __cdecl online_local_user_get_name(e_controller_index controller_index)
{
	const wchar_t* result = online_globals.users[controller_index].display_name;
	return result;
}

uns64 __cdecl online_local_user_get_player_identifier(e_controller_index controller_index)
{
	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	return online_globals.users[controller_index].player_identifier;
}

uns64 __cdecl online_local_user_get_xuid(e_controller_index controller_index)
{
	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	if (online_globals.users[controller_index].online_xuid)
	{
		return online_globals.users[controller_index].online_xuid;
	}
	return online_globals.users[controller_index].local_xuid;
}

bool __cdecl online_local_user_is_free_live_gold_account(e_controller_index controller_index)
{
	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	return online_globals.users[controller_index].is_free_live_gold;
}

bool __cdecl online_local_user_is_friend_created_content_allowed(e_controller_index controller_index)
{
	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	return online_globals.users[controller_index].is_friend_created_content_allowed;
}

bool __cdecl online_local_user_is_online_enabled(e_controller_index controller_index)
{
	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	return online_globals.users[controller_index].is_online_enabled;
}

bool __cdecl online_local_user_is_signed_in(e_controller_index controller_index)
{
	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	return online_globals.users[controller_index].is_signed_in;
}

bool __cdecl online_local_user_is_silver_or_gold_live(e_controller_index controller_index)
{
	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	return online_globals.users[controller_index].is_silver_or_gold_xbox_live;
}

bool __cdecl online_local_user_is_user_created_content_allowed(e_controller_index controller_index)
{
	ASSERT(VALID_INDEX(controller_index, k_number_of_controllers));
	return online_globals.users[controller_index].is_user_created_content_allowed;
}

void __cdecl online_process_debug_output_queue()
{
}

void __cdecl online_user_set_xuid(uns64 xuid)
{
}

void __cdecl online_set_is_connected_to_live(bool is_connected_to_live)
{
	g_online_is_connected_to_live = is_connected_to_live;
}

void __cdecl online_user_set_name(int32 user_index, const wchar_t* name)
{
	s_online_user_globals& user = online_globals.users[user_index];

	if (user.online_xuid_string)
	{
		free(user.online_xuid_string);
		user.online_xuid_string = NULL;
	}
	if (user.gamertag)
	{
		free(user.gamertag);
		user.gamertag = NULL;
	}
	if (user.display_name)
	{
		free(user.display_name);
		user.display_name = NULL;
	}

	user.online_xuid_string = _wcsdup(name);
	user.gamertag = _wcsdup(name);
	user.display_name = _wcsdup(name);
}

void __cdecl online_update()
{
}
