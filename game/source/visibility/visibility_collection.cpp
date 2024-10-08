#include "visibility/visibility_collection.hpp"

REFERENCE_DECLARE(0x018BF6B0, long, c_visible_items::m_marker_count);
REFERENCE_DECLARE(0x018BF6E8, s_visible_items, c_visible_items::m_items);
REFERENCE_DECLARE_ARRAY(0x024B0A58, c_visible_items::s_marker_indices, c_visible_items::m_marker_indices, k_maximum_item_markers);

void __cdecl visibility_collection_dispose()
{
	INVOKE(0x006874E0, visibility_collection_dispose);
}

void __cdecl visibility_collection_initialize()
{
	INVOKE(0x006874F0, visibility_collection_initialize);
}

