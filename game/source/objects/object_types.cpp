#include "objects/object_types.hpp"

#include "game/players.hpp"
#include "visibility/visibility_collection.hpp"

REFERENCE_DECLARE_ARRAY(0x01948004, object_type_definition*, object_type_definitions, k_object_type_count);

long debug_object_index = NONE;
long debug_objects_type_mask = 0;

bool debug_objects_player_only = false;

long __cdecl render_visible_objects_iterate(void(*function)(long))
{
    ASSERT(function);
    ASSERT(c_visible_items::get_root_objects_starting_index() == 0);

    for (long root_object_index = 0;
        root_object_index < c_visible_items::m_items.root_objects.get_count();
        root_object_index++)
    {
        function(c_visible_items::m_items.objects.m_elements[root_object_index].object_index);
    }

    return c_visible_items::m_items.root_objects.get_count();
}

void __cdecl render_invisible_objects_iterate(void(*function)(long))
{
    ASSERT(function);

}

void __cdecl render_debug_objects()
{
    if (!debug_objects)
        return;

    render_visible_objects_iterate(object_type_render_debug);
    render_invisible_objects_iterate(object_type_render_debug);
}

void __cdecl object_type_render_debug(long object_index)
{
	if (!should_render_debug_object(object_index))
		return;

    byte* object = static_cast<byte*>(object_get_and_verify_type(object_index, NONE));
    REFERENCE_DECLARE(object + 0x94, c_object_identifier, object_identifier);

    object_type_definition* definition = object_type_definition_get(object_identifier.m_type);

    for (long i = 0; definition->type_definitions[i]; i++)
    {
        if (definition->type_definitions[i]->render_debug_proc && (!debug_objects_player_only || player_index_from_unit_index(object_index) != NONE))
            definition->type_definitions[i]->render_debug_proc(object_index);
    }

    REFERENCE_DECLARE(object + 0x10, long, first_child_object_index);
    for (long child_object_index = first_child_object_index; child_object_index < NONE;)
    {
        object_type_render_debug(child_object_index);

        byte* child_object = static_cast<byte*>(object_get_and_verify_type(child_object_index, NONE));
        REFERENCE_DECLARE(child_object + 0xC, long, child_next_object_index);
        child_object_index = child_next_object_index;
    }
}

bool __cdecl render_object_should_be_visible(long object_index)
{
    return INVOKE(0x00A450F0, render_object_should_be_visible, object_index);
}

bool __cdecl should_render_debug_object(long object_index)
{
    if (debug_object_index == NONE)
    {
        if (!debug_objects_type_mask || TEST_BIT(debug_objects_type_mask, object_get_type(object_index)))
            return render_object_should_be_visible(object_index);
    }
    else if (object_index == debug_object_index)
    {
        return true;
    }

    return false;
}

object_type_definition* __cdecl object_type_definition_get(e_object_type object_type)
{
    ASSERT(object_type_definitions[object_type]);
    ASSERT(object_type_definitions[object_type]->group_tag);

    return object_type_definitions[object_type];
}

