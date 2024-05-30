#include "interface/gui_screens/matchmaking/gui_screen_matchmaking_searching.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"
#include "text/unicode.hpp"

//HOOK_DECLARE(0x00B0EF90, matchmaking_searching_parse_current_players);
//HOOK_DECLARE(0x00B0F150, matchmaking_searching_parse_hopper_name);

bool __cdecl matchmaking_searching_parse_current_players(void* this_ptr, wchar_t* buffer, long buffer_size)
{
	ustrnzcpy(buffer, L"16205740", buffer_size);

	return true;
}

bool __cdecl matchmaking_searching_parse_hopper_name(void* this_ptr, wchar_t* buffer, long buffer_size)
{
	ustrnzcpy(buffer, L"TEST HOPPER NAME", buffer_size);

	return true;
}

