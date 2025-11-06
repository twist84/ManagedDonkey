#include "interface/user_interface_global_text_parsers.hpp"

#include "config/version.hpp"
#include "main/global_preferences.hpp"
#include "memory/module.hpp"
#include "text/unicode.hpp"

#include <string.h>

HOOK_DECLARE(0x00AD86B0, parse_build_number);

bool parse_build_number(void* this_ptr, wchar_t* buffer, int32 buffer_length)
{
	ASSERT(buffer != NULL);
	ASSERT(buffer_length > 0);

	static const char* build_name = version_get_build_name();
	static const char* build_string = version_get_build_string();

	if (strlen_debug(build_name) > 1)
		usnzprintf(buffer, buffer_length, L"build number: %hs (%hs)", build_string, build_name);
	else
		usnzprintf(buffer, buffer_length, L"build number: %hs", build_string);

	static bool once = true;
	if (once)
	{
		once = false;

		global_preferences_set_hide_watermark(true);
	}

	return true;
}
