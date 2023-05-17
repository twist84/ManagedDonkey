#include "dialogs/edit_text.hpp"

#include <string.h>

#define valid_edit_text(edit) ((edit) && (edit)->text && (edit)->text_length > 0 && strlen((edit)->text) <= (edit)->text_length)

void __cdecl edit_text_new(edit_text* edit)
{
	ASSERT(valid_edit_text(edit));

	edit_text_selection_reset(edit);
}

void __cdecl edit_text_fix_selection(edit_text* edit)
{
	short text_len = static_cast<short>(strlen(edit->text));

	if (edit->selection_index6 <= 0)
		edit->selection_index6 = 0;

	short text_len11 = text_len;
	if (edit->selection_index6 < text_len)
		text_len11 = edit->selection_index6;

	word selection_index8 = edit->selection_index8;
	bool v8 = edit->selection_index8 <= 0xFFFF;
	edit->selection_index6 = text_len;

	short v5 = -1;
	if (!v8)
		v5 = selection_index8;
	if (v5 < text_len)
	{
		text_len = -1;
		if (selection_index8 > -1)
			text_len = selection_index8;
	}

	edit->selection_index8 = text_len;
	if (text_len11 == text_len)
		edit->selection_index8 = -1;
}

void __cdecl edit_text_selection_reset(edit_text* edit)
{
	ASSERT(valid_edit_text(edit));

	edit_text_fix_selection(edit);
	edit->selection_index6 = static_cast<word>(strlen(edit->text));
	edit->selection_index8 = -1;
}

byte __cdecl edit_text_get_selection_indices(edit_text* edit, short* out_selection_index0, short* out_selection_index1)
{
	ASSERT(valid_edit_text(edit));

	edit_text_fix_selection(edit);

	if (edit->selection_index8 == -1)
		return 0;

	*out_selection_index0 = (edit->selection_index8 <= edit->selection_index6) ? edit->selection_index8 : edit->selection_index6;
	*out_selection_index0 = (edit->selection_index8 <= edit->selection_index6) ? edit->selection_index6 : edit->selection_index8;

	return 1;
}

void __cdecl edit_text_handle_key(edit_text* edit, s_key_state const* key)
{
	ASSERT(valid_edit_text(edit));

	// #TODO: implement
}

