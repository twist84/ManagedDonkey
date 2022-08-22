#pragma once

extern long __cdecl network_life_cycle_get_state();
extern void __cdecl network_life_cycle_request_leave(bool disconnect);
extern void __cdecl network_life_cycle_end();