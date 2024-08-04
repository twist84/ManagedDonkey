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

void __cdecl object_type_adjust_placement(object_placement_data* data)
{
    INVOKE(0x00B731D0, object_type_adjust_placement, data);
}

//.text:00B73240 ; void __cdecl object_type_attach_gamestate_entity(long)
//.text:00B732B0 ; void __cdecl object_type_attach_to_marker(long, long, long, long)
//.text:00B73310 ; void __cdecl object_type_attach_to_node(long, long, short)
//.text:00B73370 ; bool __cdecl object_type_compute_activation(long, s_game_cluster_bit_vectors const*, bool*)
//.text:00B733E0 ; bool __cdecl object_type_compute_function_value(long, long, long, real*, bool*, bool*)
//.text:00B73480 ; void __cdecl object_type_create_children(long)
//.text:00B734F0 ; object_type_definition* __cdecl object_type_definition_get(e_object_type)
//.text:00B73500 ; void __cdecl object_type_delete(long)
//.text:00B735A0 ; void __cdecl object_type_detach_from_parent(long)
//.text:00B73610 ; void __cdecl object_detach_simulation_object_glue(long, long)
//.text:00B73680 ; void __cdecl object_type_dispose_from_old_structure_bsp(long)
//.text:00B736F0 ; void __cdecl object_type_fix_transform(long, real_point3d*, vector3d*, vector3d*)
//.text:00B73770 ; void __cdecl object_type_fix_transform_from_physics(long, real_matrix4x3*)
//.text:00B737F0 ; void __cdecl object_type_fix_transform_to_physics(long, real_matrix4x3*)
//.text:00B73870 ; e_object_type __cdecl object_type_from_definition_index(long)
//.text:00B738A0 ; e_object_type __cdecl object_type_from_group_tag(dword)
//.text:00B738D0 ; short __cdecl object_type_get_datum_size(e_object_type)
//.text:00B738F0 ; dword __cdecl object_type_get_maximum_placement_count(e_object_type)
//.text:00B73910 ; char const* __cdecl object_type_get_name(e_object_type)
//.text:00B73930 ; long __cdecl object_type_get_palette_tag_block_offset(e_object_type)
//.text:00B73950 ; void __cdecl object_type_get_placement_tag_block(e_object_type, long*, long*)
//.text:00B73980 ; void __cdecl object_type_handle_deleted_object(long, long)
//.text:00B73A00 ; void __cdecl object_type_handle_deleted_player(long, long)
//.text:00B73A80 ; bool __cdecl object_type_handle_parent_destroyed(long)
//.text:00B73B00 ; void __cdecl object_type_handle_region_destroyed(long, short, dword)
//.text:00B73B90 ; void __cdecl object_type_initialize_for_new_structure_bsp(long)
//.text:00B73C00 ; void __cdecl object_type_move(long)
//.text:00B73C60 ; bool __cdecl object_type_new(long, object_placement_data*, bool*)
//.text:00B73D30 ; void __cdecl object_type_notify_impulse_sound(long, long, long)
//.text:00B73DC0 ; void __cdecl object_type_place(long, s_scenario_object const*)
//.text:00B73E40 ; void __cdecl object_type_postprocess_node_matrices(long, long, real_matrix4x3*)
//.text:00B73ED0 ; void __cdecl object_type_preprocess_node_orientations(long, c_static_flags<255> const*, long, real_orientation*)
//.text:00B73F60 ; void __cdecl object_type_preprocess_root_node_matrix(long, real_matrix4x3*)
//.text:00B73FE0 ; 
//.text:00B73FF0 ; void __cdecl object_type_reset(long)
//.text:00B74070 ; 
//.text:00B74080 ; void __cdecl object_type_unplace(long)
//.text:00B740F0 ; bool __cdecl object_type_update(long)
//.text:00B741C0 ; void __cdecl object_types_dispose()
//.text:00B741F0 ; void __cdecl object_types_dispose_from_old_map()
//.text:00B74220 ; short __cdecl object_types_get_maximum_datum_size()
//.text:00B742B0 ; void __cdecl object_types_initialize()
//.text:00B74480 ; void __cdecl object_types_initialize_for_new_map()

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

