#pragma once

extern bool __cdecl network_get_machine_name(wchar_t* machine_name, long machine_name_len);
extern void __cdecl network_remote_reporting_dispose();
extern void __cdecl network_remote_reporting_initialize();

extern void bandwidth_profiler_render();

