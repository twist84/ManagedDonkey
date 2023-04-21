#pragma once

struct hkMoppCode;
struct s_tag_data;

extern hkMoppCode* __cdecl mopp_code_data_get_mopp_code(s_tag_data* data, long offset);
extern long __cdecl mopp_code_get_tag_data_size(hkMoppCode const* mopp_code);


