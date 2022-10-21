#pragma once

#include "cseries/cseries.hpp"
#include "text/unicode.hpp"

extern void wchar_string_sanitize_for_game(wchar_t* string, long maximum_character_count);

enum e_controller_index;
enum e_utf32;

extern void __cdecl parse_build_number_string(e_controller_index controller_index, e_utf32 utf32, c_static_wchar_string<1024>* out_string);
