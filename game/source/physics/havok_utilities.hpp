#pragma once

struct hkMoppCode;
struct s_tag_data;

extern hkMoppCode* __cdecl mopp_code_data_get_mopp_code(s_tag_data* data, int32 offset);
extern int32 __cdecl mopp_code_get_tag_data_size(const hkMoppCode* mopp_code);


