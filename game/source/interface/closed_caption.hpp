#pragma once

#include "cseries/cseries.hpp"
#include "text/unicode.hpp"

long const k_caption_count = 5;

struct s_caption_entry
{
	word_flags flags;
	short __unknown2;
	long __unknown4;
	dword milliseconds;
};
static_assert(sizeof(s_caption_entry) == 0xC);

struct s_caption_globals // $TODO: find the actual name of this
{
	dword_flags flags;
	long font;
	long justification;
	c_static_array<s_caption_entry, k_caption_count> captions;
	c_static_wchar_string<128> text;
};
static_assert(sizeof(s_caption_globals) == 0x148);

extern s_caption_globals& g_caption_globals;

extern void __cdecl closed_caption_add_caption(long a1, bool a2);
extern void __cdecl closed_caption_dispose();
extern void __cdecl closed_caption_dispose_from_old_map();
extern void __cdecl closed_caption_enable(bool enable);
extern bool __cdecl closed_caption_enabled();
extern void __cdecl closed_caption_initialize();
extern void __cdecl closed_caption_initialize_for_new_map();
extern void __cdecl closed_caption_render();
extern void __cdecl closed_caption_reset();
extern void __cdecl closed_caption_scroll_one_entry();
extern void __cdecl closed_caption_update();

