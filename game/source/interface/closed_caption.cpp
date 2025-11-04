#include "interface/closed_caption.hpp"

#include "interface/user_interface.hpp"

REFERENCE_DECLARE(0x052605C0, s_closed_caption_globals, g_closed_caption_globals);
REFERENCE_DECLARE_ARRAY(0x05260608, wchar_t, caption_text, 128);

void __cdecl closed_caption_add_caption(int32 sound_definition_index, bool sound_is_dialog)
{
	INVOKE(0x00A98B60, closed_caption_add_caption, sound_definition_index, sound_is_dialog);

	//uns32 milliseconds = user_interface_milliseconds();
	//
	//s_caption_entry* caption = NULL;
	//for (int32 caption_index = k_caption_count - 1; caption_index >= 0; caption_index--)
	//{
	//	if (TEST_BIT(g_closed_caption_globals.entries[caption_index].flags, 0))
	//	{
	//		caption = &g_closed_caption_globals.entries[caption_index];
	//	}
	//}
	//
	//if (!caption)
	//{
	//	closed_caption_scroll_one_entry();
	//	caption = &g_closed_caption_globals.entries[k_caption_count - 1];
	//}
	//
	//SET_BIT(caption->flags, 0, true);
	//caption->entry_time_milliseconds = milliseconds;
	//SET_BIT(caption->flags, 1, sound_is_dialog);
	//caption->sound_definition_index = sound_definition_index;
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
	//SET_BIT(g_closed_caption_globals.flags, 0, enable);
}

bool __cdecl closed_caption_enabled()
{
	return INVOKE(0x00A98CB0, closed_caption_enabled);

	//return TEST_BIT(g_closed_caption_globals.flags, 0);
}

const wchar_t* __cdecl closed_caption_get_text(const s_caption_entry* entry)
{
	return INVOKE(0x00A98CC0, closed_caption_get_text, entry);

	//return g_closed_caption_globals.text.print(L"test caption line [ %ld ]", entry->milliseconds);
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

	//g_closed_caption_globals.font = _subtitle_font;
	//g_closed_caption_globals.justification = _text_justification_left;
	//g_closed_caption_globals.captions.clear();
}

void __cdecl closed_caption_scroll_one_entry()
{
	INVOKE(0x00A99090, closed_caption_scroll_one_entry);
}

void __cdecl closed_caption_test_add_line(bool sound_is_dialog)
{
	closed_caption_add_caption(NONE, sound_is_dialog);
}

void __cdecl closed_caption_update()
{
	INVOKE(0x00A990E0, closed_caption_update);

	//uns32 milliseconds = user_interface_milliseconds();
	//for (int32 caption_index = 0; caption_index < k_caption_count; caption_index++)
	//{
	//	s_caption_entry* caption = &g_closed_caption_globals.entries[caption_index];
	//	if (TEST_BIT(caption->flags, 0) && (milliseconds - caption->entry_time_milliseconds) >= 5000)
	//	{
	//		*caption = {};
	//	}
	//}
}

