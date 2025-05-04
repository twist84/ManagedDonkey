#include "interface/closed_caption.hpp"

#include "interface/user_interface.hpp"

void __cdecl closed_caption_add_caption(int32 a1, bool a2)
{
	INVOKE(0x00A98B60, closed_caption_add_caption, a1, a2);

	//uns32 milliseconds = user_interface_milliseconds();
	//
	//s_caption_entry* caption = NULL;
	//for (int32 caption_index = k_caption_count - 1; caption_index >= 0; caption_index--)
	//{
	//	if (TEST_BIT(g_caption_globals.captions[caption_index].flags, 0))
	//		caption = &g_caption_globals.captions[caption_index];
	//}
	//
	//if (!caption)
	//{
	//	closed_caption_scroll_one_entry();
	//	caption = &g_caption_globals.captions[k_caption_count - 1];
	//}
	//
	//SET_BIT(caption->flags, 0, true);
	//caption->milliseconds = milliseconds;
	//SET_BIT(caption->flags, 1, a2);
	//caption->__unknown4 = a1;
}

void __cdecl closed_caption_dispose()
{
	INVOKE(0x00A98C10, closed_caption_dispose);

	//closed_caption_reset();
}

void __cdecl closed_caption_dispose_from_old_map()
{
	INVOKE(0x00A98C40, closed_caption_dispose_from_old_map);

	//closed_caption_reset();
}

void __cdecl closed_caption_enable(bool enable)
{
	INVOKE(0x00A98C70, closed_caption_enable, enable);

	//if (!enable)
	//	closed_caption_reset();
	//
	//SET_BIT(g_caption_globals.flags, 0, enable);
}

bool __cdecl closed_caption_enabled()
{
	return INVOKE(0x00A98CB0, closed_caption_enabled);

	//return TEST_BIT(g_caption_globals.flags, 0);
}

wchar_t const* __cdecl closed_caption_get_text(s_caption_entry const* entry)
{
	return INVOKE(0x00A98CC0, closed_caption_get_text, entry);

	//return g_caption_globals.text.print(L"test caption line [ %ld ]", entry->milliseconds);
}

void __cdecl closed_caption_initialize()
{
	INVOKE(0x00A98CF0, closed_caption_initialize);

	//closed_caption_reset();
}

void __cdecl closed_caption_initialize_for_new_map()
{
	INVOKE(0x00A98D20, closed_caption_initialize_for_new_map);

	//closed_caption_reset();
}

void __cdecl closed_caption_render()
{
	INVOKE(0x00A98D50, closed_caption_render);
}

void __cdecl closed_caption_reset()
{
	INVOKE(0x00A99060, closed_caption_reset);

	//g_caption_globals.font = _subtitle_font;
	//g_caption_globals.justification = _text_justification_left;
	//g_caption_globals.captions.clear();
}

void __cdecl closed_caption_scroll_one_entry()
{
	INVOKE(0x00A99090, closed_caption_scroll_one_entry);
}

void __cdecl closed_caption_update()
{
	INVOKE(0x00A990E0, closed_caption_update);

	//uns32 milliseconds = user_interface_milliseconds();
	//for (int32 caption_index = 0; caption_index < k_caption_count; caption_index++)
	//{
	//	s_caption_entry& caption = g_caption_globals.captions[caption_index];
	//	if (TEST_BIT(caption.flags, 0) && (milliseconds - caption.milliseconds) >= 5000)
	//		caption = {};
	//}
}

