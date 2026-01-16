#pragma once

#include "cache/cache_file_tag_resource_runtime.hpp"
#include "tag_files/tag_groups.hpp"

struct s_structure_bsp_resources;
class c_structure_bsp_resource_interface
{
public:
	s_structure_bsp_resources* get_resources() const;

	c_typed_tag_block<s_structure_bsp_resources> raw_resources;
	s_tag_resource tag_resources;
	s_tag_resource cache_file_resources;
	int32 use_resource_items;
};
COMPILE_ASSERT(sizeof(c_structure_bsp_resource_interface) == 0x20);

// $TODO use `s_*_bsp_types` for collision bsp logic
//struct s_small_bsp_types
//{
//	using k_collision_bsp_type = struct collision_bsp;
//	using k_bsp3d_nodes_type = struct bsp3d_node;
//	using k_planes_type = struct real_plane3d;
//	using k_leaves_type = struct collision_leaf;
//	using k_bsp2d_references_type = struct bsp2d_reference;
//	using k_bsp2d_nodes_type = struct bsp2d_node;
//	using k_surfaces_type = struct collision_surface;
//	using k_edges_type = struct collision_edge;
//	using k_vertices_type = struct collision_vertex;
//};
//
//struct s_large_bsp_types
//{
//	using k_collision_bsp_type = struct large_collision_bsp;
//	using k_bsp3d_nodes_type = struct large_bsp3d_node;
//	using k_planes_type = struct real_plane3d;
//	using k_leaves_type = struct large_collision_leaf;
//	using k_bsp2d_references_type = struct large_bsp2d_reference;
//	using k_bsp2d_nodes_type = struct large_bsp2d_node;
//	using k_surfaces_type = struct large_collision_surface;
//	using k_edges_type = struct large_collision_edge;
//	using k_vertices_type = struct large_collision_vertex;
//};

struct collision_bsp;
struct large_collision_bsp;
struct structure_instanced_geometry_definition;
struct s_structure_bsp_resources
{
	c_typed_tag_block<collision_bsp> bsp;
	c_typed_tag_block<large_collision_bsp> large_bsp;
	c_typed_tag_block<structure_instanced_geometry_definition> instanced_geometries_definitions;
	s_tag_block pathfinding_data;
};
COMPILE_ASSERT(sizeof(s_structure_bsp_resources) == 0x30);

struct bsp3d_node;
struct collision_leaf;
struct bsp2d_reference;
struct bsp2d_node;
struct collision_surface;
struct collision_edge;
struct collision_vertex;
struct collision_bsp
{
	c_typed_tag_block<bsp3d_node> bsp3d_nodes;
	c_typed_tag_block<real_plane3d> planes;
	c_typed_tag_block<collision_leaf> leaves;
	c_typed_tag_block<bsp2d_reference> bsp2d_references;
	c_typed_tag_block<bsp2d_node> bsp2d_nodes;
	c_typed_tag_block<collision_surface> surfaces;
	c_typed_tag_block<collision_edge> edges;
	c_typed_tag_block<collision_vertex> vertices;
};
COMPILE_ASSERT(sizeof(collision_bsp) == 0x60);

struct large_bsp3d_node;
struct large_collision_leaf;
struct large_bsp2d_reference;
struct large_bsp2d_node;
struct large_collision_surface;
struct large_collision_edge;
struct large_collision_vertex;
struct large_collision_bsp
{
	c_typed_tag_block<large_bsp3d_node> bsp3d_nodes;
	c_typed_tag_block<real_plane3d> planes;
	c_typed_tag_block<large_collision_leaf> leaves;
	c_typed_tag_block<large_bsp2d_reference> bsp2d_references;
	c_typed_tag_block<large_bsp2d_node> bsp2d_nodes;
	c_typed_tag_block<large_collision_surface> surfaces;
	c_typed_tag_block<large_collision_edge> edges;
	c_typed_tag_block<large_collision_vertex> vertices;
};
COMPILE_ASSERT(sizeof(large_collision_bsp) == 0x60);

struct structure_instanced_geometry_definition
{
	uns32 checksum;
	real_point3d bounding_sphere_center;
	real32 bounding_sphere_radius;
	collision_bsp collision_info;
	s_tag_block render_bsp;
	s_tag_block mopp_codes;
	s_tag_block breakable_surface_sets;
	s_tag_block surfaces;
	s_tag_block surface_to_triangle_mapping;
	int16 mesh_index;
	int16 compression_index;
	real32 global_lightmap_resolution_scale;
	s_tag_block mesh_mopps;
	void* mesh_mopp_bv_tree;
};
COMPILE_ASSERT(sizeof(structure_instanced_geometry_definition) == 0xC8);

struct bsp3d_node
{
	int64 node_data_designator;
};
COMPILE_ASSERT(sizeof(bsp3d_node) == 0x8);

struct large_bsp3d_node
{
	int32 plane;
	int32 back_child;
	int32 front_child;
};
COMPILE_ASSERT(sizeof(large_bsp3d_node) == 0xC);

enum e_leaf_flags
{
	_leaf_flag_contains_double_sided_surfaces_bit = 0,

	k_leaf_flags
};

struct collision_leaf
{
	c_flags<e_leaf_flags, uns8, k_leaf_flags> flags;
	byte pad[0x1];
	int16 bsp2d_reference_count;
	int32 first_bsp2d_reference;
};
COMPILE_ASSERT(sizeof(collision_leaf) == 0x8);

struct large_collision_leaf
{
	c_flags<e_leaf_flags, uns16, k_leaf_flags> flags;
	int16 bsp2d_reference_count;
	int32 first_bsp2d_reference;
};
COMPILE_ASSERT(sizeof(large_collision_leaf) == 0x8);

struct bsp2d_reference
{
	int16 plane;
	int16 bsp2d_node;
};
COMPILE_ASSERT(sizeof(bsp2d_reference) == 0x4);

struct large_bsp2d_reference
{
	int32 plane;
	int32 bsp2d_node;
};
COMPILE_ASSERT(sizeof(large_bsp2d_reference) == 0x8);

struct bsp2d_node
{
	real_plane2d plane;
	int16 left_child;
	int16 right_child;
};
COMPILE_ASSERT(sizeof(bsp2d_node) == 0x10);

struct large_bsp2d_node
{
	real_plane2d plane;
	int32 left_child;
	int32 right_child;
};
COMPILE_ASSERT(sizeof(large_bsp2d_node) == 0x14);

enum e_surface_flags
{
	_collision_surface_two_sided_bit = 0,
	_collision_surface_invisible_bit,
	_collision_surface_climbable_bit,
	_collision_surface_breakable_bit,
	_collision_surface_invalid_bit,
	_collision_surface_conveyor_bit,
	_collision_surface_slip_bit,

	k_collision_surface_flags
};

struct collision_surface
{
	int16 plane;
	int16 first_edge;
	int16 material;
	int16 breakable_surface_set;
	int16 breakable_surface;
	c_flags<e_surface_flags, uns8, k_collision_surface_flags> flags;
	uns8 best_plane_calculation_vertex_index;
};
COMPILE_ASSERT(sizeof(collision_surface) == 0xC);

struct large_collision_surface
{
	int32 plane;
	int32 first_edge;
	int16 material;
	int16 breakable_surface_set;
	int16 breakable_surface;
	c_flags<e_surface_flags, uns8, k_collision_surface_flags> flags;
	uns8 best_plane_calculation_vertex_index;
};
COMPILE_ASSERT(sizeof(large_collision_surface) == 0x10);

struct collision_edge
{
	int16 vertex_indices[2];
	int16 edge_indices[2];
	int16 surface_indices[2];
};
COMPILE_ASSERT(sizeof(collision_edge) == 0xC);

struct large_collision_edge
{
	int32 vertex_indices[2];
	int32 edge_indices[2];
	int32 surface_indices[2];
};
COMPILE_ASSERT(sizeof(large_collision_edge) == 0x18);

struct collision_vertex
{
	real_point3d point;
	int16 first_edge;
	int16 sink;
};
COMPILE_ASSERT(sizeof(collision_vertex) == 0x10);

struct large_collision_vertex
{
	real_point3d point;
	int32 first_edge;
	int32 sink;
};
COMPILE_ASSERT(sizeof(large_collision_vertex) == 0x14);

struct s_render_cluster_part
{
	int32 flags;
	s_cluster_reference cluster_reference;
	uns16 mesh_index;
	uns16 part_index;
};
COMPILE_ASSERT(sizeof(s_render_cluster_part) == 0xC);

struct s_render_instance_part
{
	int32 flags;
	int16 structure_bsp_index;
	uns16 structure_instance_index;
	uns16 mesh_index;
	uns16 part_index;
	uns8 alpha_byte;
};
COMPILE_ASSERT(sizeof(s_render_instance_part) == 0x10);

enum
{
	k_maximum_rendered_cluster_parts = 2048,
	k_maximum_rendered_instance_parts = 3072,
	k_maximum_rendered_instance_lists = 1024,
	k_maximum_render_structure_markers = 6,
};

struct s_scenario_lightmap_bsp_data;
struct s_render_geometry;
struct render_structure_globals
{
	bool lightmaps_available_on_all_active_bsps;
	int32 lightmap_bsp_type[16];
	const s_scenario_lightmap_bsp_data* lightmap_bsp_data[16];
	const s_render_geometry* render_geometry[16];
	s_render_cluster_part render_cluster_parts[k_maximum_rendered_cluster_parts];
	int32 render_cluster_part_count;
	s_render_instance_part render_instance_parts[k_maximum_rendered_instance_parts];
	int32 render_instance_part_count;
	int32 marker_index;
	int32 render_cluster_part_starting_index[k_maximum_render_structure_markers];
	int32 render_instance_part_starting_index[k_maximum_render_structure_markers];
	int32 last_structure_bsp_index;
	int32 last_instance_definition_index;
	int32 last_structure_instance_index;
};
COMPILE_ASSERT(sizeof(render_structure_globals) == 0x1210C);

class c_structure_renderer
{
public:
	static void __cdecl dispose();
	static void __cdecl dispose_from_old_map();
	static void __cdecl dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask);
	static void __cdecl initialize();
	static void __cdecl initialize_for_new_map();
	static void __cdecl initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);
	static bool __cdecl lightmaps_available(int32 structure_bsp_index);
	static bool __cdecl lightmaps_available_on_all_active_bsps();
	static void __cdecl pop_marker();
	static void __cdecl push_marker();
	static void __cdecl render_albedo();
	static void __cdecl render_decorators();
};

extern bool& g_render_decorators;
extern render_structure_globals& g_render_structure_globals;

