#include "interface/user_interface_text.hpp"

#include "config/version.hpp"
#include "memory/module.hpp"

#include <string.h>

REFERENCE_DECLARE_ARRAY(0x01942C48, s_parse_text_entry, g_parse_text_table, 131);

HOOK_DECLARE(0x00ABC070, parse_build_number_string);

void wchar_string_sanitize_for_game(wchar_t* string, long maximum_character_count)
{
	ASSERT(string != NULL);

	wchar_t v1[3][2]
	{
		{ '<', '[' },
		{ '>', ']' },
		{ '|', '-' }
	};

	bool v2 = 0;
	do
	{
		for (long i = 0; i < maximum_character_count && string[i]; ++i)
		{
			for (long j = 0; j < 3; ++j)
			{
				if (string[i] == v1[j][0])
				{
					string[i] = v1[j][1];
					v2 = 1;
					break;
				}
			}
		}
	} while (v2);
}

void utf32_to_string(e_utf32 utf32, wchar_t(&out_string)[2])
{
	union
	{
		e_utf32 value;
		wchar_t str[2];
	} out = { .value = utf32 };

	ustrnzcpy(out_string, out.str, 2);
}

bool __cdecl parse_lobby_privacy(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00AA4B50, parse_lobby_privacy, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_coop_max_players(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B226F0, parse_lobby_coop_max_players, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_countdown_remaining(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B22710, parse_lobby_countdown_remaining, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_current_players(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B22750, parse_lobby_current_players, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_delaying_player(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B22790, parse_lobby_delaying_player, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_film_max_players(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B227E0, parse_lobby_film_max_players, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_film_party_leader_requirement(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B22800, parse_lobby_film_party_leader_requirement, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_header(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B22850, parse_lobby_header, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_max_players(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B228C0, parse_lobby_max_players, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_network(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B22910, parse_lobby_network, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_party_leader(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B22930, parse_lobby_party_leader, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_percent_loaded(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B22980, parse_lobby_percent_loaded, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_start_button_name(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B22A10, parse_lobby_start_button_name, this_ptr, buffer, buffer_length);
}

bool __cdecl parse_lobby_title(void* this_ptr, wchar_t* buffer, long buffer_length)
{
	return INVOKE(0x00B22A80, parse_lobby_title, this_ptr, buffer, buffer_length);
}

// this hook never gets hit
void __cdecl parse_build_number_string(e_controller_index controller_index, e_utf32 utf32, c_static_wchar_string<1024>* out_string)
{
	char const* build_name = version_get_build_name();
	char const* build_string = version_get_build_string();

	out_string->set(L"");

	if (strlen(build_name) > 1)
	{
		out_string->append_print(L"%hs (%hs)", build_name, build_string);
	}
	else
	{
		out_string->append_print(L"%hs", build_string);
	}
}

