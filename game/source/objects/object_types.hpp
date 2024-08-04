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

	short datum_size;
	short placement_tag_block_offset;
	short palette_tag_block_offset;
	short placement_tag_block_size;

	long maximum_placement_count;

	// unsure of the name
	long mixing_board_type;

	c_mover_definition_data* mover_definition;

	void(__cdecl* initialize_proc)();
	void(__cdecl* dispose_proc)();
	void(__cdecl* initialize_for_new_map_proc)();
	void(__cdecl* dispose_from_old_map_proc)();
	void(__cdecl* initialize_for_new_structure_bsp)(long);
	void(__cdecl* dispose_from_old_structure_bsp_proc)(long);
	void(__cdecl* adjust_placement_proc)(void*);
	bool(__cdecl* new_proc)(long, object_placement_data*, bool*);
	void(__cdecl* place_proc)(long, long);
	void(__cdecl* unplace_proc)(long);
	void(__cdecl* create_children_proc)(long);
	void(__cdecl* delete_proc)(long);
	long(__cdecl* update_proc)(long);
	long(__cdecl* post_update_proc)(long);
	void(__cdecl* move_proc)(long);
	long(__cdecl* compute_activation_proc)(long, const s_game_cluster_bit_vectors*, bool*);
	bool(__cdecl* compute_function_value_proc)(long, long, long, real*, bool*, bool);
	void(__cdecl* attach_gamestate_entity_proc)(long);
	void(__cdecl* detach_simulation_object_glue_proc)(long);

	void* attach_to_marker_proc;
	void* attach_to_node_proc;

	void(__cdecl* detach_from_parent_proc)(long);
	void(__cdecl* handle_deleted_object_proc)(long, long);

	dword_flags flags;

	void(__cdecl* handle_deleted_player_proc)(long, long);
	void(__cdecl* handle_region_destroyed_proc)(long, short, dword);
	bool(__cdecl* handle_parent_destroyed_proc)(long);
	void(__cdecl* fix_transform_proc)(long, real_point3d*, vector3d*, vector3d*);
	void(__cdecl* fix_transform_to_physics_proc)(long, real_matrix4x3*);
	void(__cdecl* fix_transform_from_physics_proc)(long, real_matrix4x3*);
	void(__cdecl* preprocess_node_orientations_proc)(long, long, long, long);
	void(__cdecl* preprocess_root_node_matrix_proc)(long, real_matrix4x3*);
	void(__cdecl* postprocess_node_matrices_proc)(long, long, real_matrix4x3*);
	void(__cdecl* reset_proc)(long);
	void(__cdecl* notify_impulse_sound_proc)(long, long, long);
	void(__cdecl* render_debug_proc)(long);

	object_type_definition* type_definitions[16];
	object_type_definition* next;

	// wtf is this?
	long __unknownF0;
};
static_assert(sizeof(object_type_definition) == 0xF4);

extern long debug_object_index;
extern long debug_objects_type_mask;
extern bool debug_objects_player_only;

extern object_type_definition* (&object_type_definitions)[k_object_type_count];

extern void __cdecl object_type_adjust_placement(object_placement_data* data);

extern void __cdecl render_debug_objects();
extern void __cdecl object_type_render_debug(long object_index);
extern bool __cdecl should_render_debug_object(long object_index);
extern object_type_definition* __cdecl object_type_definition_get(e_object_type object_type);

