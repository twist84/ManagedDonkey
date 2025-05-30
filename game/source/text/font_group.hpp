#pragma once

#include "cseries/cseries.hpp"

struct s_font_character;
struct s_font_header;
struct s_kerning_pair;

extern bool __cdecl font_character_validate(const s_font_character* character);
extern int16 __cdecl font_get_kerning_pair_offset(const s_font_header* header, uns32 first_character, uns32 second_character);
extern int32 __cdecl font_get_line_height(const s_font_header* header);
extern void __cdecl font_header_byteswap(s_font_header* header);
extern bool __cdecl font_header_validate(const s_font_header* header);
extern void __cdecl font_kerning_pairs_byteswap(s_kerning_pair* kerning_pairs, int32 kerning_pair_count);

