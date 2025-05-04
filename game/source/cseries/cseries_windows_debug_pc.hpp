#pragma once

extern bool& g_set_always_a_debugger_present;
extern bool& g_set_never_a_debugger_present;

//extern bool __cdecl debug_get_binary_filename(c_static_string<260>* binary_filename);
extern char const* __cdecl exception_code_get_string(uns32 code);
extern int32 __cdecl exceptions_update();
extern void __cdecl force_debugger_always_present(bool always_a_debugger_present);
extern void __cdecl force_debugger_not_present(bool never_a_debugger_present);
extern bool __cdecl is_debugger_present();
extern bool __cdecl has_cached_exception();

