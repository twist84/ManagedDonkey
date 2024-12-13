#pragma once

#include "cseries/cseries.hpp"
#include "objects/objects.hpp"

struct c_mover_definition_data;
struct object_placement_data;
struct s_game_cluster_bit_vectors;
struct object_type_definition
{
	char const* name;
	tag group_tag;
	short game_datum_size;
	short placement_tag_block_offset;
	short palette_tag_block_offset;
	short placement_tag_block_element_size;
	dword maximum_placement_count;
	long animation_mixing_board_definition_index;
	c_mover_definition_data* mover_definition;
	void(__cdecl* initialize)();
	void(__cdecl* dispose)();
	void(__cdecl* initialize_for_new_map)();
	void(__cdecl* dispose_from_old_map)();
	void(__cdecl* initialize_for_new_structure_bsp)(long object_index);
	void(__cdecl* dispose_from_old_structure_bsp)(long object_index);
	void(__cdecl* datum_adjust_placement)(object_placement_data* data);
	bool(__cdecl* datum_new)(long object_index, object_placement_data* data, bool* out_of_memory);
	void(__cdecl* datum_place)(long object_index, void const* scenario_object);
	void(__cdecl* datum_unplace)(long object_index);
	void(__cdecl* datum_create_children)(long object_index);
	void(__cdecl* datum_delete)(long object_index);
	long(__cdecl* datum_update)(long object_index);
	long(__cdecl* datum_post_update)(long object_index);
	void(__cdecl* datum_move)(long object_index);
	long(__cdecl* datum_compute_activation)(long object_index, s_game_cluster_bit_vectors const* cluster_activation, bool* out_active);
	bool(__cdecl* datum_compute_function_value)(long object_index, long function, long function_owner_tag_index, real* value, bool* active, bool deterministic);
	void(__cdecl* datum_attach_gamestate_entity)(long object_index);
	void(__cdecl* datum_detach_gamestate_entity)(long object_index);
	void(__cdecl* datum_attach_to_marker)(long parent_object_index, long parent_marker_name, long child_object_index, long child_marker_name);
	void(__cdecl* datum_attach_to_node)(long parent_object_index, long child_object_index, short node_index);
	void(__cdecl* datum_detach_from_parent)(long object_index);
	void(__cdecl* handle_deleted_object)(long object_index, long deleted_object_index);
	dword deleted_object_type_mask;
	void(__cdecl* handle_deleted_player)(long object_index, long deleted_player_index);
	void(__cdecl* handle_region_destroyed)(long object_index, short region_index, dword damage_region_flags);
	bool(__cdecl* handle_parent_destroyed)(long object_index);
	void(__cdecl* datum_fix_transform)(long object_index, real_point3d* position, real_vector3d* forward, real_vector3d* up);
	void(__cdecl* datum_fix_transform_to_physics)(long object_index, real_matrix4x3* world_matrix);
	void(__cdecl* datum_fix_transform_from_physics)(long object_index, real_matrix4x3* world_matrix);
	void(__cdecl* datum_preprocess_node_orientations)(long object_index, c_static_flags<256> const* node_mask, long node_orientation_count, real_orientation* node_orientations);
	void(__cdecl* datum_preprocess_root_node_matrix)(long object_index, real_matrix4x3* node_matrix);
	void(__cdecl* datum_postprocess_node_matrices)(long object_index, long node_matrices_count, real_matrix4x3* node_matrices);
	void(__cdecl* reset)(long object_index);
	void(__cdecl* notify_impulse_sound)(long object_index, long sound_definition_index, long impulse_sound_index);
	void(__cdecl* render_debug)(long object_index);
	object_type_definition* part_definitions[16];
	object_type_definition* next;
	dword deleted_object_notification_mask;
};
static_assert(sizeof(object_type_definition) == 0xF4);

extern bool debug_objects_player_only;
extern long debug_object_index;
extern long debug_objects_type_mask;

extern object_type_definition*(&object_type_definitions)[k_object_type_count];

extern void __cdecl object_type_adjust_placement(object_placement_data* data);
extern object_type_definition* __cdecl object_type_definition_get(e_object_type object_type);
extern char const* __cdecl object_type_get_name(e_object_type object_type);
extern void __cdecl object_type_render_debug(long object_index);

extern void render_debug_objects();
extern bool should_render_debug_object(long object_index);

