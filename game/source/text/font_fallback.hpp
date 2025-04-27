#pragma once

enum e_utf32 : uint32;
struct s_font_character;
struct s_font_header;

extern unsigned char k_fallback_font_data[0x1194];
extern s_font_header const* g_fallback_font_header;

extern bool fallback_font_get_character(e_utf32 utf_character, s_font_character const** out_character, void const** out_pixel_data);
extern s_font_header const* fallback_font_get_header();
extern void fallback_font_initialize();

