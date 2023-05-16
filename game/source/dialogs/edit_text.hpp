#pragma once

#include "cseries/cseries.hpp"

struct edit_text
{
	char const* text;
	word text_length;
	word selection_index6;
	word selection_index8;
};
static_assert(sizeof(edit_text) == 0xC);

struct s_key_state;

void __cdecl edit_text_new(edit_text* edit);
void __cdecl edit_text_selection_reset(edit_text* edit);
void __cdecl edit_text_fix_selection(edit_text* edit);
byte __cdecl edit_text_get_selection_indices(edit_text* edit, short* out_selection_index0, short* out_selection_index1);
void __cdecl edit_text_handle_key(edit_text* edit, s_key_state const* key);

