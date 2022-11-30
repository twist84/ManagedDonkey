#include "interface/closed_caption.hpp"

#include "cseries/cseries.hpp"

void __cdecl closed_caption_add_caption(long a1, bool a2)
{
	INVOKE(0x00A98B60, closed_caption_add_caption, a1, a2);
}

void __cdecl closed_caption_dispose()
{
	INVOKE(0x00A98C10, closed_caption_dispose);
}

void __cdecl closed_caption_dispose_from_old_map()
{
	INVOKE(0x00A98C40, closed_caption_dispose_from_old_map);
}

void __cdecl closed_caption_enable(bool enable)
{
	INVOKE(0x00A98C70, closed_caption_enable, enable);
}

bool __cdecl closed_caption_enabled()
{
	return INVOKE(0x00A98CB0, closed_caption_enabled);
}

void __cdecl closed_caption_initialize()
{
	INVOKE(0x00A98CF0, closed_caption_initialize);
}

void __cdecl closed_caption_initialize_for_new_map()
{
	INVOKE(0x00A98D20, closed_caption_initialize_for_new_map);
}

void __cdecl closed_caption_render()
{
	INVOKE(0x00A98D50, closed_caption_render);
}

void __cdecl closed_caption_reset()
{
	INVOKE(0x00A99060, closed_caption_reset);
}

void __cdecl closed_caption_scroll_one_entry()
{
	INVOKE(0x00A99090, closed_caption_scroll_one_entry);
}

void __cdecl closed_caption_update()
{
	INVOKE(0x00A990E0, closed_caption_update);
}

