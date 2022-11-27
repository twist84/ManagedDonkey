#include "tag_files/tag_groups.hpp"

#include "cache/cache_files.hpp"
#include "cseries/console.hpp"
#include "cseries/cseries.hpp"

void __cdecl tag_load_missing_tags_report()
{
	// nullsub
	INVOKE(0x0055AA70, tag_load_missing_tags_report);
}

void* tag_reference::get_definition()
{
	return tag_get(group_tag, index);
}
