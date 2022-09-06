#include "main/loading.hpp"

#include "cseries/cseries.hpp"

bool force_load_map_failed = false;

bool __cdecl main_load_map(char* scenario_path, long map_load_type)
{
    if (force_load_map_failed)
        return false;

    return DECLTHUNK(0x0052F180, main_load_map, scenario_path, map_load_type);
}
