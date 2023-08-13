#pragma once

extern bool debug_trigger_volumes;

extern bool __cdecl hs_evaluate(long thread_index, long expression_index, long destination_pointer, long* out_cast);
extern long __cdecl hs_runtime_script_begin(short script_index, short script_type, char thread_type);
extern long __cdecl hs_thread_new(char thread_index, long script_index, bool deterministic);
extern void __cdecl render_debug_trigger_volumes();

