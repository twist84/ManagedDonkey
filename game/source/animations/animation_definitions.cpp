#include "animations/animation_definitions.hpp"

#include "cache/cache_files.hpp"

const c_model_animation_graph* __cdecl c_model_animation_graph::get(int32 definition_index)
{
	//return INVOKE(0x00784BD0, c_model_animation_graph::get, definition_index);

	const c_model_animation_graph* result = TAG_GET(MODEL_ANIMATION_GRAPH_TAG, c_model_animation_graph const, definition_index);

	return result;
}

//.text:00784BF0 ; public: const s_additional_node_data* __cdecl c_model_animation_graph::get_additional_node_data(int32) const
//.text:00784C20 ; public: const c_model_animation* __cdecl c_model_animation_graph::get_animation(c_animation_id) const
//.text:00784CA0 ; public: int16 __cdecl c_model_animation_graph::get_animation_count() const
//.text:00784CB0 ; public: void __cdecl c_model_animation_graph::get_animation_data(c_animation_id, c_animation_data*) const
//.text:00784D10 ; private: const byte* __cdecl c_model_animation_graph::get_animation_data_address(c_animation_id) const
//.text:00784DB0 ; private: const byte* __cdecl c_model_animation_graph::get_animation_data_address_from_local_animation(int32) const
//.text:00784E10 ; public: const c_model_animation* __cdecl c_model_animation_graph::get_animation_interface(c_animation_id) const
//.text:00784E20 ; public: int16 __cdecl c_model_animation::get_animation_type() const
//.text:00784E30 ; public: real32 __cdecl c_model_animation::get_authored_duration() const
//.text:00784E60 ; public: void __cdecl c_model_animation_graph::get_axis_offsets(c_animation_id, int16, real32*, real32*) const
//.text:007851D0 ; public: const animation_aiming_screen_bounds* __cdecl c_model_animation_graph::get_blend_screen_bounds(c_animation_id) const
//.text:00785260 ; public: const s_dialogue_event* __cdecl c_model_animation::get_dialogue_event(int32) const
//.text:00785270 ; public: const s_effect_event* __cdecl c_model_animation::get_effect_event(int32) const
//.text:00785290 ; public: const animation_graph_effect_reference* __cdecl c_model_animation_graph::get_effect_reference(int32) const
//.text:007852C0 ; public: const s_foot_tracking_member* __cdecl c_model_animation_graph::get_foot_tracking_member(int32) const
//.text:007852E0 ; public: int16 __cdecl c_model_animation::get_frame_count() const
//.text:007852F0 ; public: int16 __cdecl c_model_animation::get_frame_info_type() const
//.text:00785300 ; public: real32 __cdecl c_model_animation::get_frame_position_from_playback_ratio(real32) const

const c_model_animation_graph* __cdecl c_model_animation_graph::get_from_object_definition(int32 object_definition_index)
{
	return INVOKE(0x00785360, c_model_animation_graph::get_from_object_definition, object_definition_index);
}

//.text:007853B0 ; public: void __cdecl c_model_animation_graph::get_graph_and_animation(c_animation_id, const c_model_animation_graph**, const c_model_animation**)const
//.text:00785440 ; public: const c_model_animation_graph* __cdecl c_model_animation_graph::get_inherited_graph(int32) const
//.text:00785480 ; public: int32 __cdecl c_model_animation_graph::get_inherited_graph_count() const
//.text:00785490 ; public: const s_frame_event* __cdecl c_model_animation::get_keyframe_event(int32) const
//.text:007854A0 ; public: real32 __cdecl c_model_animation::get_last_frame_index() const
//.text:007854C0 ; public: int16 __cdecl c_model_animation::get_right_foot_frame_index() const
//.text:007854F0 ; public: const c_model_animation* __cdecl c_model_animation_graph::get_local_animation(int32) const
//.text:00785510 ; public: bool __cdecl c_model_animation_graph::get_local_animation_data(const c_model_animation*, c_animation_data*) const
//.text:007855B0 ; public: const animation_aiming_screen_bounds* __cdecl c_model_animation_graph::get_local_blend_screen_bounds(int32) const
//.text:007855D0 ; private: const c_model_animation_tag_resource_group* __cdecl c_model_animation_graph::get_local_resource_group(int32) const
//.text:00785600 ; private: const c_model_animation_tag_resource_member* __cdecl c_model_animation_graph::get_local_resource_group_member(int32, int32) const
//.text:00785640 ; public: c_model_animation* __cdecl c_model_animation_graph::get_local_writable_animation(int32)
//.text:00785660 ; private: c_model_animation_tag_resource_group* __cdecl c_model_animation_graph::get_local_writable_resource_group(int32)
//.text:00785690 ; private: c_model_animation_tag_resource_member* __cdecl c_model_animation_graph::get_local_writable_resource_group_member(int32, int32)
//.text:007856D0 ; public: int16 __cdecl c_model_animation::get_loop_frame_index() const
//.text:007856E0 ; public: const char* __cdecl c_model_animation::get_name() const

s_animation_graph_node* c_model_animation_graph::get_node(int32 node_index) const
{
	//return INVOKE_CLASS_MEMBER(0x007856F0, c_model_animation_graph, get_node, node_index);

	return &definitions.skeleton_nodes[node_index];
}

//.text:00785700 ; public: uns32 __cdecl c_model_animation::get_node_checksum() const
//.text:00785710 ; public: int16 __cdecl c_model_animation::get_node_count() const
//.text:00785720 ; public: int16 __cdecl c_model_animation_graph::get_node_count() const
//.text:00785730 ; public: const s_animation_inheritance* __cdecl c_model_animation_graph::get_node_remap_data_for_animation(int32, c_animation_id) const
//.text:00785770 ; public: const c_model_animation_graph* __cdecl c_model_animation_graph::get_parent_graph() const
//.text:00785790 ; public: real32 __cdecl c_model_animation::get_playback_ratio_from_frame_position(real32) const
//.text:00785800 ; public: int16 __cdecl c_model_animation::get_primary_key_frame_index() const
//.text:00785830 ; public: const c_model_animation_tag_resource_member* __cdecl c_model_animation_graph::get_resource_group_member(c_animation_id) const
//.text:007858A0 ; 
//.text:007858D0 ; 
//.text:00785900 ; 
//.text:00785910 ; 
//.text:00785930 ; public: const animation_graph_sound_reference* __cdecl c_model_animation_graph::get_sound_reference(int32) const
//.text:00785960 ; 
//.text:00785970 ; 
//.text:00785980 ; 
//.text:00785990 ; 
//.text:007859A0 ; public: int32 __cdecl c_model_animation_graph::get_total_foot_tracking_members() const
//.text:007859B0 ; public: int32 __cdecl c_model_animation::get_total_keyframe_events() const
//.text:007859C0 ; 
//.text:007859D0 ; 
//.text:007859F0 ; 
//.text:00785A70 ; 
//.text:00785A90 ; 
//.text:00785B20 ; 
//.text:00785B60 ; 
//.text:00785BD0 ; 
//.text:00785BE0 ; private: void __cdecl c_model_animation_graph::internal_node_matrices_from_orientations(int32, int32, real_matrix4x3 restrict*, real_orientation const restrict*, real_matrix4x3 const restrict*) const
//.text:00785C60 ; 
//.text:00785CD0 ; public: bool __cdecl c_model_animation_graph::is_inherited_from_graph(const c_model_animation_graph*) const
//.text:00785D40 ; 
//.text:00785DA0 ; 
//.text:00785DE0 ; 
//.text:00785E00 ; 
//.text:00785E40 ; public: void __cdecl c_model_animation_graph::node_matrices_from_orientations(real_matrix4x3 restrict*, real_orientation const restrict*, real_matrix4x3 const restrict*) const
//.text:00785E60 ; public: void __cdecl c_model_animation_graph::node_matrices_from_orientations_with_gun_hand_swap(real_matrix4x3 restrict*, real_orientation const restrict*, real_matrix4x3 const restrict*, int32, int32) const
//.text:007860D0 ; 
//.text:00786100 ; 
//.text:00786110 ; 

