#pragma once

#include "cseries/cseries.hpp"

extern void __cdecl main_loop_body_main_part();
extern void __cdecl main_loop_body_begin();
extern void __cdecl main_loop_body_end();
extern dword __cdecl _internal_halt_render_thread_and_lock_resources(char const* file, long line);
extern void __cdecl unlock_resources_and_resume_render_thread(dword flags);
extern void __cdecl main_loop_pregame_show_progress_screen();

