#include "animations/animation_definitions.hpp"

#include "cache/cache_files.hpp"

c_model_animation_graph const* __cdecl c_model_animation_graph::get(long definition_index)
{
	//return INVOKE(0x00784BD0, c_model_animation_graph::get, definition_index);

	return (c_model_animation_graph*)tag_get(MODEL_ANIMATION_GRAPH_TAG, definition_index);
}

//.text:00784BF0 ; public: s_additional_node_data const* __cdecl c_model_animation_graph::get_additional_node_data(long) const
//.text:00784C20 ; public: c_model_animation const* __cdecl c_model_animation_graph::get_animation(c_animation_id) const
//.text:00784CA0 ; public: short __cdecl c_model_animation_graph::get_animation_count() const
//.text:00784CB0 ; public: void __cdecl c_model_animation_graph::get_animation_data(c_animation_id, c_animation_data*) const
//.text:00784D10 ; private: byte const* __cdecl c_model_animation_graph::get_animation_data_address(c_animation_id) const
//.text:00784DB0 ; private: byte const* __cdecl c_model_animation_graph::get_animation_data_address_from_local_animation(long) const
//.text:00784E10 ; public: c_model_animation const* __cdecl c_model_animation_graph::get_animation_interface(c_animation_id) const
//.text:00784E20 ; public: short __cdecl c_model_animation::get_animation_type() const
//.text:00784E30 ; public: real __cdecl c_model_animation::get_authored_duration() const
//.text:00784E60 ; public: void __cdecl c_model_animation_graph::get_axis_offsets(c_animation_id, short, real*, real*) const
//.text:007851D0 ; public: animation_aiming_screen_bounds const* __cdecl c_model_animation_graph::get_blend_screen_bounds(c_animation_id) const
//.text:00785260 ; public: s_dialogue_event const* __cdecl c_model_animation::get_dialogue_event(long) const
//.text:00785270 ; public: s_effect_event const* __cdecl c_model_animation::get_effect_event(long) const
//.text:00785290 ; public: animation_graph_effect_reference const* __cdecl c_model_animation_graph::get_effect_reference(long) const
//.text:007852C0 ; public: s_foot_tracking_member const* __cdecl c_model_animation_graph::get_foot_tracking_member(long) const
//.text:007852E0 ; public: short __cdecl c_model_animation::get_frame_count() const
//.text:007852F0 ; public: short __cdecl c_model_animation::get_frame_info_type() const
//.text:00785300 ; public: real __cdecl c_model_animation::get_frame_position_from_playback_ratio(real) const

c_model_animation_graph const* __cdecl c_model_animation_graph::get_from_object_definition(long object_definition_index)
{
	return INVOKE(0x00785360, c_model_animation_graph::get_from_object_definition, object_definition_index);
}

//.text:007853B0 ; public: void __cdecl c_model_animation_graph::get_graph_and_animation(c_animation_id, c_model_animation_graph const**, c_model_animation const**)const
//.text:00785440 ; public: c_model_animation_graph const* __cdecl c_model_animation_graph::get_inherited_graph(long) const
//.text:00785480 ; public: long __cdecl c_model_animation_graph::get_inherited_graph_count() const
//.text:00785490 ; public: s_frame_event const* __cdecl c_model_animation::get_keyframe_event(long) const
//.text:007854A0 ; public: real __cdecl c_model_animation::get_last_frame_index() const
//.text:007854C0 ; public: short __cdecl c_model_animation::get_right_foot_frame_index() const
//.text:007854F0 ; public: c_model_animation const* __cdecl c_model_animation_graph::get_local_animation(long) const
//.text:00785510 ; public: bool __cdecl c_model_animation_graph::get_local_animation_data(c_model_animation const*, c_animation_data*) const
//.text:007855B0 ; public: animation_aiming_screen_bounds const* __cdecl c_model_animation_graph::get_local_blend_screen_bounds(long) const
//.text:007855D0 ; private: c_model_animation_tag_resource_group const* __cdecl c_model_animation_graph::get_local_resource_group(long) const
//.text:00785600 ; private: c_model_animation_tag_resource_member const* __cdecl c_model_animation_graph::get_local_resource_group_member(long, long) const
//.text:00785640 ; public: c_model_animation* __cdecl c_model_animation_graph::get_local_writable_animation(long)
//.text:00785660 ; private: c_model_animation_tag_resource_group* __cdecl c_model_animation_graph::get_local_writable_resource_group(long)
//.text:00785690 ; private: c_model_animation_tag_resource_member* __cdecl c_model_animation_graph::get_local_writable_resource_group_member(long, long)
//.text:007856D0 ; public: short __cdecl c_model_animation::get_loop_frame_index() const
//.text:007856E0 ; public: char const* __cdecl c_model_animation::get_name() const

s_animation_graph_node* c_model_animation_graph::get_node(long node_index) const
{
	//return INVOKE_CLASS_MEMBER(0x007856F0, c_model_animation_graph, get_node, node_index);

	return &definitions.skeleton_nodes[node_index];
}

//.text:00785700 ; public: dword __cdecl c_model_animation::get_node_checksum() const
//.text:00785710 ; public: short __cdecl c_model_animation::get_node_count() const
//.text:00785720 ; public: short __cdecl c_model_animation_graph::get_node_count() const
//.text:00785730 ; public: s_animation_inheritance const* __cdecl c_model_animation_graph::get_node_remap_data_for_animation(long, c_animation_id) const
//.text:00785770 ; public: c_model_animation_graph const* __cdecl c_model_animation_graph::get_parent_graph() const
//.text:00785790 ; public: real __cdecl c_model_animation::get_playback_ratio_from_frame_position(real) const
//.text:00785800 ; public: short __cdecl c_model_animation::get_primary_key_frame_index() const
//.text:00785830 ; public: c_model_animation_tag_resource_member const* __cdecl c_model_animation_graph::get_resource_group_member(c_animation_id) const
//.text:007858A0 ; 
//.text:007858D0 ; 
//.text:00785900 ; 
//.text:00785910 ; 
//.text:00785930 ; public: animation_graph_sound_reference const* __cdecl c_model_animation_graph::get_sound_reference(long) const
//.text:00785960 ; 
//.text:00785970 ; 
//.text:00785980 ; 
//.text:00785990 ; 
//.text:007859A0 ; public: long __cdecl c_model_animation_graph::get_total_foot_tracking_members() const
//.text:007859B0 ; public: long __cdecl c_model_animation::get_total_keyframe_events() const
//.text:007859C0 ; 
//.text:007859D0 ; 
//.text:007859F0 ; 
//.text:00785A70 ; 
//.text:00785A90 ; 
//.text:00785B20 ; 
//.text:00785B60 ; 
//.text:00785BD0 ; 
//.text:00785BE0 ; private: void __cdecl c_model_animation_graph::internal_node_matrices_from_orientations(long, long, real_matrix4x3 restrict*, real_orientation const restrict*, real_matrix4x3 const restrict*) const
//.text:00785C60 ; 
//.text:00785CD0 ; public: bool __cdecl c_model_animation_graph::is_inherited_from_graph(c_model_animation_graph const*) const
//.text:00785D40 ; 
//.text:00785DA0 ; 
//.text:00785DE0 ; 
//.text:00785E00 ; 
//.text:00785E40 ; public: void __cdecl c_model_animation_graph::node_matrices_from_orientations(real_matrix4x3 restrict*, real_orientation const restrict*, real_matrix4x3 const restrict*) const
//.text:00785E60 ; public: void __cdecl c_model_animation_graph::node_matrices_from_orientations_with_gun_hand_swap(real_matrix4x3 restrict*, real_orientation const restrict*, real_matrix4x3 const restrict*, long, long) const
//.text:007860D0 ; 
//.text:00786100 ; 
//.text:00786110 ; 

