#pragma once

#include "cseries/cseries.hpp"
#include "text/unicode.hpp"

int32 const k_caption_count = 5;

struct s_caption_entry
{
	enum e_caption_entry_flags
	{
		_caption_valid_bit = 0,
		_caption_is_dialog_bit,
		_caption_fading_out_bit,

		k_number_of_caption_entry_flags,
	};

	uns16 flags;
	int16 vertical_position;
	int32 sound_definition_index;
	uns32 entry_time_milliseconds;
};
static_assert(sizeof(s_caption_entry) == 0xC);

struct s_closed_caption_globals // $TODO find the actual name of this
{
	int32 flags;
	e_font_id font;
	e_text_justification justification;
	s_caption_entry entries[k_caption_count];
};
static_assert(sizeof(s_closed_caption_globals) == 0x48);

extern s_closed_caption_globals& g_closed_caption_globals;
extern wchar_t(&caption_text)[128];

extern void __cdecl closed_caption_add_caption(int32 a1, bool a2);
extern void __cdecl closed_caption_dispose();
extern void __cdecl closed_caption_dispose_from_old_map();
extern void __cdecl closed_caption_enable(bool enable);
extern bool __cdecl closed_caption_enabled();
extern void __cdecl closed_caption_initialize();
extern void __cdecl closed_caption_initialize_for_new_map();
extern void __cdecl closed_caption_render();
extern void __cdecl closed_caption_reset();
extern void __cdecl closed_caption_scroll_one_entry();
extern void __cdecl closed_caption_test_add_line(bool sound_is_dialog);
extern void __cdecl closed_caption_update();

