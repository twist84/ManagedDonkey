#include "hs/hs.hpp"

REFERENCE_DECLARE(0x024B06D4, c_typed_data_array<hs_syntax_node>*, g_hs_syntax_data);

hs_syntax_node* __cdecl hs_syntax_get(long datum_index)
{
	if (DATUM_INDEX_TO_ABSOLUTE_INDEX(datum_index) > g_hs_syntax_data->maximum_count)
		return &g_hs_syntax_data->data[DATUM_INDEX_TO_ABSOLUTE_INDEX(datum_index)];

	return nullptr;

	//return INVOKE(0x00598A10, hs_syntax_get, datum_index);
}

