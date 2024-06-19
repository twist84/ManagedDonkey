#pragma once

#include "cseries/cseries.hpp"

struct s_font_character;
struct s_font_header;
struct s_kerning_pair;

extern bool __cdecl font_character_validate(s_font_character const* character);
extern short __cdecl font_get_kerning_pair_offset(s_font_header const* header, dword a2, dword a3);
extern long __cdecl font_get_line_height(s_font_header const* header);
extern void __cdecl font_header_byteswap(s_font_header* header);
extern bool __cdecl font_header_validate(s_font_header const* header);
extern void __cdecl font_kerning_pairs_byteswap(s_kerning_pair* kerning_pairs, long kerning_pair_count);

