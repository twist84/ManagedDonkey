#pragma once

extern char const fatal_error_message[];

extern bool& g_show_damaged_media;

extern void __cdecl damaged_media_exception();
extern void __cdecl damaged_media_update();