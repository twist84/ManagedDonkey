#pragma once

struct hs_script;
extern int32 start_script(hs_script const* script, int32 index);
extern int32 user_interface_start_hs_script_by_name(char const* name);