#include "text/text_group.hpp"

bool __cdecl string_list_get_normal_string(int32 definition_index, int32 id, c_static_wchar_string<256>* buffer)
{
	return INVOKE(0x0051E120, string_list_get_normal_string, definition_index, id, buffer);
}

