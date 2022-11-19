#include "game/game_engine_candy_monitor.hpp"

bool __cdecl game_engine_object_has_been_abandoned(long object_index)
{
    return INVOKE(0x0058F9E0, game_engine_object_has_been_abandoned, object_index);
}

bool __cdecl game_engine_object_has_been_disturbed(long object_index)
{
    return INVOKE(0x0058FB30, game_engine_object_has_been_disturbed, object_index);
}

bool __cdecl game_engine_object_is_item_in_inventory(long object_index)
{
    return INVOKE(0x0058FCE0, game_engine_object_is_item_in_inventory, object_index);
}

bool __cdecl game_engine_object_is_useless_weapon(long object_index)
{
    return INVOKE(0x0058FD40, game_engine_object_is_useless_weapon, object_index);
}

bool __cdecl game_engine_player_is_looking_at_sphere(long player_index, real_point3d const* position, real radius)
{
    return INVOKE(0x0058FDB0, game_engine_player_is_looking_at_sphere, player_index, position, radius);
}

bool __cdecl game_engine_player_is_nearby(real_point3d const* position, real radius)
{
    return INVOKE(0x0058FEE0, game_engine_player_is_nearby, position, radius);
}

