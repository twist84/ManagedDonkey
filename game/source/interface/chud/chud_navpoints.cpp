#include "interface/chud/chud_navpoints.hpp"

real32 __cdecl chud_get_string_width(int32 font_id, wchar_t* s)
{
	return INVOKE(0x00AAF0A0, chud_get_string_width, font_id, s);
}

