#pragma once

extern void __cdecl render_report_render_debug(int32 user_index, bool draw_environment_error);
extern bool __cdecl render_debug_drawing_active();
extern void __cdecl error_report_begin_frame();
extern void __cdecl error_report_end_frame();
extern void __cdecl render_report_draw_environment_error_report();

