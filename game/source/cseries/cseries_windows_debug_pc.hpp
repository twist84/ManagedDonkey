#pragma once

extern bool g_set_always_a_debugger_present;
extern bool g_set_never_a_debugger_present;

extern bool is_debugger_present();
extern long __cdecl exceptions_update();
extern void debug_stack_print(void(*console_write_line)(wchar_t const* format, ...), bool include_offset);

