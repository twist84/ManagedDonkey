#pragma once

#include "cseries/cseries.hpp"
#include "objects/objects.hpp"

class c_mover_definition_data;
struct object_placement_data;
struct s_game_cluster_bit_vectors;
struct object_type_definition
{
	const char* name;
	tag group_tag;
	int16 game_datum_size;
	int16 placement_tag_block_offset;
	int16 palette_tag_block_offset;
	int16 placement_tag_block_element_size;
	uns32 maximum_placement_count;
	int32 animation_mixing_board_definition_index;
	c_mover_definition_data* mover_definition;
	void(__cdecl* initialize)();
	void(__cdecl* dispose)();
	void(__cdecl* initialize_for_new_map)();
	void(__cdecl* dispose_from_old_map)();
	void(__cdecl* initialize_for_new_structure_bsp)(int32 object_index);
	void(__cdecl* dispose_from_old_structure_bsp)(int32 object_index);
	void(__cdecl* datum_adjust_placement)(object_placement_data* data);
	bool(__cdecl* datum_new)(int32 object_index, object_placement_data* data, bool* out_of_memory);
	void(__cdecl* datum_place)(int32 object_index, const void* scenario_object);
	void(__cdecl* datum_unplace)(int32 object_index);
	void(__cdecl* datum_create_children)(int32 object_index);
	void(__cdecl* datum_delete)(int32 object_index);
	int32(__cdecl* datum_update)(int32 object_index);
	int32(__cdecl* datum_post_update)(int32 object_index);
	void(__cdecl* datum_move)(int32 object_index);
	int32(__cdecl* datum_compute_activation)(int32 object_index, const s_game_cluster_bit_vectors* cluster_activation, bool* out_active);
	bool(__cdecl* datum_compute_function_value)(int32 object_index, int32 function, int32 function_owner_tag_index, real32* value, bool* active, bool deterministic);
	void(__cdecl* datum_attach_gamestate_entity)(int32 object_index);
	void(__cdecl* datum_detach_gamestate_entity)(int32 object_index);
	void(__cdecl* datum_attach_to_marker)(int32 parent_object_index, int32 parent_marker_name, int32 child_object_index, int32 child_marker_name);
	void(__cdecl* datum_attach_to_node)(int32 parent_object_index, int32 child_object_index, int16 node_index);
	void(__cdecl* datum_detach_from_parent)(int32 object_index);
	void(__cdecl* handle_deleted_object)(int32 object_index, int32 deleted_object_index);
	uns32 deleted_object_type_mask;
	void(__cdecl* handle_deleted_player)(int32 object_index, int32 deleted_player_index);
	void(__cdecl* handle_region_destroyed)(int32 object_index, int16 region_index, uns32 damage_region_flags);
	bool(__cdecl* handle_parent_destroyed)(int32 object_index);
	void(__cdecl* datum_fix_transform)(int32 object_index, real_point3d* position, real_vector3d* forward, real_vector3d* up);
	void(__cdecl* datum_fix_transform_to_physics)(int32 object_index, real_matrix4x3* world_matrix);
	void(__cdecl* datum_fix_transform_from_physics)(int32 object_index, real_matrix4x3* world_matrix);
	void(__cdecl* datum_preprocess_node_orientations)(int32 object_index, const c_static_flags<256>* node_mask, int32 node_orientation_count, real_orientation* node_orientations);
	void(__cdecl* datum_preprocess_root_node_matrix)(int32 object_index, real_matrix4x3* node_matrix);
	void(__cdecl* datum_postprocess_node_matrices)(int32 object_index, int32 node_matrices_count, real_matrix4x3* node_matrices);
	void(__cdecl* reset)(int32 object_index);
	void(__cdecl* notify_impulse_sound)(int32 object_index, int32 sound_definition_index, int32 impulse_sound_index);
	void(__cdecl* render_debug)(int32 object_index);
	object_type_definition* part_definitions[16];
	object_type_definition* next;
	uns32 deleted_object_notification_mask;
};
COMPILE_ASSERT(sizeof(object_type_definition) == 0xF4);

extern bool debug_objects_player_only;
extern int32 debug_object_index;
extern int32 debug_objects_type_mask;

extern object_type_definition*(&object_type_definitions)[k_object_type_count];

extern void __cdecl object_type_adjust_placement(object_placement_data* data);
extern object_type_definition* __cdecl object_type_definition_get(e_object_type object_type);
extern const char* __cdecl object_type_get_name(e_object_type object_type);
extern void __cdecl object_type_render_debug(int32 object_index);

extern void render_debug_objects();
extern bool should_render_debug_object(int32 object_index);

