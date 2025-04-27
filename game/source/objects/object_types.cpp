#include "objects/object_types.hpp"

#include "game/players.hpp"
#include "render/render_visibility.hpp"
#include "render/render_objects_static_lighting.hpp"

REFERENCE_DECLARE_ARRAY(0x01948004, object_type_definition*, object_type_definitions, k_object_type_count);

bool debug_objects_player_only = false;
int32 debug_object_index = NONE;
int32 debug_objects_type_mask = 0;

void __cdecl object_type_adjust_placement(object_placement_data* data)
{
    INVOKE(0x00B731D0, object_type_adjust_placement, data);
}

//.text:00B73240 ; void __cdecl object_type_attach_gamestate_entity(int32)
//.text:00B732B0 ; void __cdecl object_type_attach_to_marker(int32, int32, int32, int32)
//.text:00B73310 ; void __cdecl object_type_attach_to_node(int32, int32, int16)
//.text:00B73370 ; bool __cdecl object_type_compute_activation(int32, s_game_cluster_bit_vectors const*, bool*)
//.text:00B733E0 ; bool __cdecl object_type_compute_function_value(int32, int32, int32, real32*, bool*, bool*)
//.text:00B73480 ; void __cdecl object_type_create_children(int32)

object_type_definition* __cdecl object_type_definition_get(e_object_type object_type)
{
    //return INVOKE(0x00B734F0, object_type_definition_get, object_type);

    ASSERT(object_type_definitions[object_type]);
    ASSERT(object_type_definitions[object_type]->group_tag);

    return object_type_definitions[object_type];
}

//.text:00B73500 ; void __cdecl object_type_delete(int32)
//.text:00B735A0 ; void __cdecl object_type_detach_from_parent(int32)
//.text:00B73610 ; void __cdecl object_type_detach_gamestate_entity(int32)
//.text:00B73680 ; void __cdecl object_type_dispose_from_old_structure_bsp(int32)
//.text:00B736F0 ; void __cdecl object_type_fix_transform(int32, real_point3d*, real_vector3d*, real_vector3d*)
//.text:00B73770 ; void __cdecl object_type_fix_transform_from_physics(int32, real_matrix4x3*)
//.text:00B737F0 ; void __cdecl object_type_fix_transform_to_physics(int32, real_matrix4x3*)
//.text:00B73870 ; e_object_type __cdecl object_type_from_definition_index(int32)
//.text:00B738A0 ; e_object_type __cdecl object_type_from_group_tag(uint32)
//.text:00B738D0 ; int16 __cdecl object_type_get_datum_size(e_object_type)
//.text:00B738F0 ; uint32 __cdecl object_type_get_maximum_placement_count(e_object_type)

char const* __cdecl object_type_get_name(e_object_type object_type)
{
    return INVOKE(0x00B73910, object_type_get_name, object_type);
}

//.text:00B73930 ; int32 __cdecl object_type_get_palette_tag_block_offset(e_object_type)
//.text:00B73950 ; void __cdecl object_type_get_placement_tag_block(e_object_type, int32*, int32*)
//.text:00B73980 ; void __cdecl object_type_handle_deleted_object(int32, int32)
//.text:00B73A00 ; void __cdecl object_type_handle_deleted_player(int32, int32)
//.text:00B73A80 ; bool __cdecl object_type_handle_parent_destroyed(int32)
//.text:00B73B00 ; void __cdecl object_type_handle_region_destroyed(int32, int16, uint32)
//.text:00B73B90 ; void __cdecl object_type_initialize_for_new_structure_bsp(int32)
//.text:00B73C00 ; void __cdecl object_type_move(int32)
//.text:00B73C60 ; bool __cdecl object_type_new(int32, object_placement_data*, bool*)
//.text:00B73D30 ; void __cdecl object_type_notify_impulse_sound(int32, int32, int32)
//.text:00B73DC0 ; void __cdecl object_type_place(int32, s_scenario_object const*)
//.text:00B73E40 ; void __cdecl object_type_postprocess_node_matrices(int32, int32, real_matrix4x3*)
//.text:00B73ED0 ; void __cdecl object_type_preprocess_node_orientations(int32, c_static_flags<255> const*, int32, real_orientation*)
//.text:00B73F60 ; void __cdecl object_type_preprocess_root_node_matrix(int32, real_matrix4x3*)

void __cdecl object_type_render_debug(int32 object_index)
{
    //INVOKE(0x00B73FE0, object_type_render_debug, object_index);

    if (!should_render_debug_object(object_index))
        return;

    object_datum* object = object_get(object_index);
    if (!object)
        return;

    object_type_definition* definition = object_type_definition_get(object->object.object_identifier.m_type);
    if (!definition)
        return;

    for (int32 i = 0; definition->part_definitions[i]; i++)
    {
        if (definition->part_definitions[i]->render_debug && (!debug_objects_player_only || player_index_from_unit_index(object_index) != NONE))
            definition->part_definitions[i]->render_debug(object_index);
    }

    for (int32 child_object_index = object->object.first_child_object_index; child_object_index != NONE; child_object_index = object_get(child_object_index)->object.next_object_index)
        object_type_render_debug(child_object_index);
}


//.text:00B73FF0 ; void __cdecl object_type_reset(int32)
//.text:00B74070 ; e_object_type __cdecl object_type_search_from_name(char const*)
//.text:00B74080 ; void __cdecl object_type_unplace(int32)
//.text:00B740F0 ; bool __cdecl object_type_update(int32)
//.text:00B741C0 ; void __cdecl object_types_dispose()
//.text:00B741F0 ; void __cdecl object_types_dispose_from_old_map()
//.text:00B74220 ; int16 __cdecl object_types_get_maximum_datum_size()
//.text:00B742B0 ; void __cdecl object_types_initialize()
//.text:00B74480 ; void __cdecl object_types_initialize_for_new_map()

void render_debug_objects()
{
    if (!debug_objects)
        return;

    render_visible_objects_iterate(object_type_render_debug);
    render_invisible_objects_iterate(object_type_render_debug);
}

bool should_render_debug_object(int32 object_index)
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

