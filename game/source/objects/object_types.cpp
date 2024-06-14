#include "objects/object_types.hpp"

#include "game/players.hpp"
#include "visibility/visibility_collection.hpp"

REFERENCE_DECLARE_ARRAY(0x01948004, object_type_definition*, object_type_definitions, k_object_type_count);

long debug_object_index = NONE;
long debug_objects_type_mask = 0;

bool debug_objects_player_only = false;

void __cdecl render_invisible_objects_iterate(void(*function)(long))
{
    ASSERT(function);
    ASSERT(c_visible_items::get_cluster_starting_index() == 0);

    INVOKE(0x00A7BC50, render_invisible_objects_iterate, function);
}

long __cdecl render_visible_objects_iterate(void(*function)(long))
{
    ASSERT(function);
    ASSERT(c_visible_items::get_root_objects_starting_index() == 0);

    return INVOKE(0x00A7C130, render_visible_objects_iterate, function);
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

    object_datum* object = object_get(object_index);
    if (!object)
        return;

    object_type_definition* definition = object_type_definition_get(object->object.object_identifier.m_type);
    if (!definition)
        return;


    for (long i = 0; definition->type_definitions[i]; i++)
    {
        if (definition->type_definitions[i]->render_debug_proc && (!debug_objects_player_only || player_index_from_unit_index(object_index) != NONE))
            definition->type_definitions[i]->render_debug_proc(object_index);
    }

    for (long child_object_index = object->object.first_child_object_index; child_object_index != NONE; child_object_index = object_get(child_object_index)->object.next_object_index)
        object_type_render_debug(child_object_index);
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

