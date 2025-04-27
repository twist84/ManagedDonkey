#include "ai/character_definitions.hpp"

character_perception_properties* __cdecl actor_perception_properties_get(int32 actor_index)
{
    return INVOKE(0x01455830, actor_perception_properties_get, actor_index);
}

