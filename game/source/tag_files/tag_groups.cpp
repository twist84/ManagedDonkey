#include "tag_files/tag_groups.hpp"

#include "cache/cache_files.hpp"
#include "cseries/console.hpp"
#include "cseries/cseries.hpp"

#include <string.h>

void __cdecl tag_load_missing_tags_report()
{
	// nullsub
	INVOKE(0x0055AA70, tag_load_missing_tags_report);
}

char const* __cdecl tag_name_strip_path(char const* path)
{
	//return INVOKE(0x0055AA90, tag_name_strip_path, path);

	char const* name = strrchr(path, '\\');
	if (name)
		return name + 1;
	else
		return path;
}

wchar_t const* __cdecl tag_name_strip_path(wchar_t const* path)
{
	wchar_t const* name = wcsrchr(path, '\\');
	if (name)
		return name + 1;
	else
		return path;
}

void* s_tag_reference::get_definition()
{
	return tag_get(group_tag, index);
}
