#include "interface/gui_screens/matchmaking/gui_screen_matchmaking_match_found.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"
#include "text/unicode.hpp"

//HOOK_DECLARE(0x00B11350, matchmaking_match_found_parse_playlist_name);

bool __cdecl matchmaking_match_found_parse_playlist_name(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	ustrnzcpy(buffer, L"TEST PLAYLIST NAME", buffer_length);

	return true;
}

