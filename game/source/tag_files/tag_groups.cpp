#include "tag_files/tag_groups.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"
#include "tag_files/string_ids.hpp"

void __cdecl tag_load_missing_tags_report()
{
	FUNCTION_BEGIN(true);

	// nullsub
	INVOKE(0x0055AA70, tag_load_missing_tags_report);
}

char const* c_string_id::get_string()
{
	return string_id_get_string_const(m_value);
}