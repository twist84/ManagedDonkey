#include "objects/objects.hpp"

#include "cseries/console.hpp"
#include "physics/havok.hpp"

void __cdecl object_delete(long object_index)
{
    INVOKE(0x00B2CD10, object_delete, object_index);
}

long __cdecl object_get_ultimate_parent(long object_index)
{
    FUNCTION_BEGIN(true);

    return INVOKE(0x00B2EAB0, object_get_ultimate_parent, object_index);
}

long __cdecl object_new(object_placement_data* placement_data)
{
    return INVOKE(0x00B30440, object_new, placement_data);
}

void __cdecl object_placement_data_new(object_placement_data* placement_data, long object_definition_index, long object_datum_index, s_damage_owner const* damage_owner)
{
    INVOKE(0x00B31590, object_placement_data_new, placement_data, object_definition_index, object_datum_index, damage_owner);
}

bool __cdecl object_set_base_change_color_by_index(long object_index, long color_index, real_rgb_color const* color)
{
    return INVOKE(0x00B328F0, object_set_base_change_color_by_index, object_index, color_index, color);
}

void __cdecl object_set_in_limbo(long object_index, bool deactivate)
{
    INVOKE(0x00B32E20, object_set_in_limbo, object_index, deactivate);
}

void __cdecl object_set_position(long object_index, real_point3d const* position, real_vector3d const* up, real_vector3d const* forward, s_location const* location)
{
    FUNCTION_BEGIN(true);

    INVOKE(0x00B33530, object_set_position, object_index, position, up, forward, location);
}

void* __cdecl object_try_and_get_and_verify_type(long object_index, dword object_type)
{
    FUNCTION_BEGIN(true);

    return INVOKE(0x00B34490, object_try_and_get_and_verify_type, object_index, object_type);
}

void __cdecl object_debug_teleport(long object_index, real_point3d const* position)
{
    FUNCTION_BEGIN(true);

    void* object = object_try_and_get_and_verify_type(object_index, -1);
    if (object)
    {
        dword_flags flags = reinterpret_cast<dword_flags*>(object)[1];

        havok_can_modify_state_allow();
        if (TEST_BIT(flags, 7))
            object_set_in_limbo(object_index, false);
        object_set_position(object_index, position, nullptr, nullptr, nullptr);
        havok_can_modify_state_disallow();
    }
    else
    {
        c_console::write_line("Failed to get a valid object in %s.", __FUNCTION__);
    }
}

