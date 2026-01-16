#pragma once

#include "cseries/cseries.hpp"

struct edit_text
{
	char* buffer;
	int16 maximum_length;
	int16 insertion_point_index;
	int16 selection_start_index;
};
COMPILE_ASSERT(sizeof(edit_text) == 0xC);

struct s_key_state;

void __cdecl edit_text_new(edit_text* edit);
void __cdecl edit_text_selection_reset(edit_text* edit);
void __cdecl edit_text_fix_selection(edit_text* edit);
uns8 __cdecl edit_text_get_selection_indices(edit_text* edit, int16* out_selection_index0, int16* out_selection_index1);
void __cdecl edit_text_handle_key(edit_text* edit, const s_key_state* key);

