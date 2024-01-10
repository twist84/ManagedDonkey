#pragma once

extern void __cdecl display_debug_string(const char* format, ...);
extern unsigned long __cdecl system_milliseconds();
extern unsigned long __cdecl system_seconds();
extern void __cdecl system_abort();
extern void __cdecl system_exit();
extern void __cdecl system_set_dll_directory();

