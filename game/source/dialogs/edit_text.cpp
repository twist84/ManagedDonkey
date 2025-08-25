#include "dialogs/edit_text.hpp"

#include "input/input_windows.hpp"

#include <string.h>

#define valid_edit_text(edit) ((edit) && (edit)->buffer && (edit)->maximum_length > 0 && int16(strlen((edit)->buffer)) <= (edit)->maximum_length)

void __cdecl edit_text_new(edit_text* edit)
{
	ASSERT(valid_edit_text(edit));

	edit_text_selection_reset(edit);
}

void __cdecl edit_text_fix_selection(edit_text* edit)
{
	int16 text_len = int16(strlen(edit->buffer));

	if (edit->insertion_point_index <= 0)
		edit->insertion_point_index = 0;

	int16 text_len11 = text_len;
	if (edit->insertion_point_index < text_len)
		text_len11 = edit->insertion_point_index;

	uns16 selection_start_index = edit->selection_start_index;
	bool v8 = edit->selection_start_index <= NONE;
	edit->insertion_point_index = text_len;

	int16 v5 = NONE;
	if (!v8)
		v5 = selection_start_index;
	if (v5 < text_len)
	{
		text_len = NONE;
		if (selection_start_index > NONE)
			text_len = selection_start_index;
	}

	edit->selection_start_index = text_len;
	if (text_len11 == text_len)
		edit->selection_start_index = NONE;
}

void __cdecl edit_text_selection_reset(edit_text* edit)
{
	ASSERT(valid_edit_text(edit));

	edit_text_fix_selection(edit);
	edit->insertion_point_index = static_cast<uns16>(strlen(edit->buffer));
	edit->selection_start_index = NONE;
}

uns8 __cdecl edit_text_get_selection_indices(edit_text* edit, int16* start_index, int16* end_index)
{
	ASSERT(valid_edit_text(edit));

	edit_text_fix_selection(edit);

	if (edit->selection_start_index == NONE)
		return 0;

	*start_index = (edit->selection_start_index <= edit->insertion_point_index) ? edit->selection_start_index : edit->insertion_point_index;
	*end_index = (edit->selection_start_index <= edit->insertion_point_index) ? edit->insertion_point_index : edit->selection_start_index;

	return 1;
}

void __cdecl edit_text_handle_key(edit_text* edit, const s_key_state* key)
{
	ASSERT(valid_edit_text(edit));

	// $IMPLEMENT

	edit_text_fix_selection(edit);

	e_key_type key_type = (e_key_type)key->key_type;
	e_input_key_code ascii_code = (e_input_key_code)key->ascii_code;
	if (key_type == _key_type_down && ascii_code == _key_backspace)
	{
		edit->buffer[edit->insertion_point_index - 1] = 0;
	}
}

