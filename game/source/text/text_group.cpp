#include "text/text_group.hpp"

bool __cdecl multilingual_unicode_string_list_test_string(int32 tag_index, string_id string)
{
	return INVOKE(0x0051DFC0, multilingual_unicode_string_list_test_string, tag_index, string);
}

bool __cdecl string_list_get_interface_string(int32 tag_index, int32 string, c_static_wchar_string<1024>* buffer)
{
	return INVOKE(0x0051E040, string_list_get_interface_string, tag_index, string, buffer);
}

bool __cdecl string_list_get_normal_string(int32 tag_index, int32 string, c_static_wchar_string<256>* buffer)
{
	return INVOKE(0x0051E120, string_list_get_normal_string, tag_index, string, buffer);
}

