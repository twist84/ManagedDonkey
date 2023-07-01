#pragma once

extern long __cdecl exceptions_update();
extern void __cdecl force_debugger_always_present(bool always_a_debugger_present);
extern void __cdecl force_debugger_not_present(bool never_a_debugger_present);
extern bool __cdecl is_debugger_present();
extern bool __cdecl has_cached_exception();

