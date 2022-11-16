#pragma once

#include "hs/hs_scenario_definitions.hpp"
#include "memory/data.hpp"

extern c_typed_data_array<hs_syntax_node>*& g_hs_syntax_data;

extern hs_syntax_node* __cdecl hs_syntax_get(long datum_index);

