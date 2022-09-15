#include "main/levels.hpp"

#include "cseries/cseries.hpp"

bool __cdecl levels_map_id_is_fake(long map_id)
{
    //return INVOKE(0x0054C320, levels_map_id_is_fake, map_id);

    return map_id == -2;
}
