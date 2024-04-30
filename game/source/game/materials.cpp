#include "game/materials.hpp"

s_global_material_definition* global_get_material(c_global_material_type material_type)
{
    return INVOKE(0x005CA640, global_get_material, material_type);
}

