#pragma once

struct hs_script;
extern long start_script(hs_script const* script, long index);
extern long user_interface_start_hs_script_by_name(char const* name);