#pragma once

enum e_utf32 : uns32;
struct s_font_character;
struct s_font_header;

extern byte k_fallback_font_data[0x1194];
extern const s_font_header* g_fallback_font_header;

extern bool fallback_font_get_character(e_utf32 utf_character, const s_font_character** out_character, const void** out_pixel_data);
extern const s_font_header* fallback_font_get_header();
extern void fallback_font_initialize();

