#pragma once

#include "cache/cache_file_tag_resource_runtime.hpp"
#include "tag_files/tag_groups.hpp"

struct s_structure_bsp_resources;
struct c_structure_bsp_resource_interface
{
	c_typed_tag_block<s_structure_bsp_resources> raw_resources;
	s_tag_resource tag_resources;
	s_tag_resource cache_file_resources;
	long use_resource_items;

	s_structure_bsp_resources* get_resources() const;
};
static_assert(sizeof(c_structure_bsp_resource_interface) == 0x20);

// #TODO: use `s_*_bsp_types` for collision bsp logic
//struct s_small_bsp_types
//{
//	using k_collision_bsp_type = struct collision_bsp;
//	using k_bsp3d_nodes_type = struct bsp3d_node;
//	using k_planes_type = struct plane3d;
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
//	using k_planes_type = struct plane3d;
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
static_assert(sizeof(s_structure_bsp_resources) == 0x30);

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
	c_typed_tag_block<plane3d> planes;
	c_typed_tag_block<collision_leaf> leaves;
	c_typed_tag_block<bsp2d_reference> bsp2d_references;
	c_typed_tag_block<bsp2d_node> bsp2d_nodes;
	c_typed_tag_block<collision_surface> surfaces;
	c_typed_tag_block<collision_edge> edges;
	c_typed_tag_block<collision_vertex> vertices;
};
static_assert(sizeof(collision_bsp) == 0x60);

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
	c_typed_tag_block<plane3d> planes;
	c_typed_tag_block<large_collision_leaf> leaves;
	c_typed_tag_block<large_bsp2d_reference> bsp2d_references;
	c_typed_tag_block<large_bsp2d_node> bsp2d_nodes;
	c_typed_tag_block<large_collision_surface> surfaces;
	c_typed_tag_block<large_collision_edge> edges;
	c_typed_tag_block<large_collision_vertex> vertices;
};
static_assert(sizeof(large_collision_bsp) == 0x60);

struct structure_instanced_geometry_definition
{
	dword checksum;
	real_point3d bounding_sphere_center;
	real bounding_sphere_radius;
	collision_bsp collision_info;
	s_tag_block render_bsp;
	s_tag_block mopp_codes;
	s_tag_block breakable_surface_sets;
	s_tag_block surfaces;
	s_tag_block surface_to_triangle_mapping;
	short mesh_index;
	short compression_index;
	real global_lightmap_resolution_scale;
	s_tag_block mesh_mopps;
	void* mesh_mopp_bv_tree;
};
static_assert(sizeof(structure_instanced_geometry_definition) == 0xC8);

struct bsp3d_node
{
	long long node_data_designator;
};
static_assert(sizeof(bsp3d_node) == 0x8);

struct large_bsp3d_node
{
	long plane;
	long back_child;
	long front_child;
};
static_assert(sizeof(large_bsp3d_node) == 0xC);

enum e_leaf_flags
{
	_leaf_flag_contains_double_sided_surfaces_bit = 0,

	k_leaf_flags
};

struct collision_leaf
{
	c_flags<e_leaf_flags, byte, k_leaf_flags> flags;
	byte pad[0x1];
	short bsp2d_reference_count;
	long first_bsp2d_reference;
};
static_assert(sizeof(collision_leaf) == 0x8);

struct large_collision_leaf
{
	c_flags<e_leaf_flags, word, k_leaf_flags> flags;
	short bsp2d_reference_count;
	long first_bsp2d_reference;
};
static_assert(sizeof(large_collision_leaf) == 0x8);

struct bsp2d_reference
{
	short plane;
	short bsp2d_node;
};
static_assert(sizeof(bsp2d_reference) == 0x4);

struct large_bsp2d_reference
{
	long plane;
	long bsp2d_node;
};
static_assert(sizeof(large_bsp2d_reference) == 0x8);

struct bsp2d_node
{
	plane2d plane;
	short left_child;
	short right_child;
};
static_assert(sizeof(bsp2d_node) == 0x10);

struct large_bsp2d_node
{
	plane2d plane;
	long left_child;
	long right_child;
};
static_assert(sizeof(large_bsp2d_node) == 0x14);

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
	short plane;
	short first_edge;
	short material;
	short breakable_surface_set;
	short breakable_surface;
	c_flags<e_surface_flags, byte, k_collision_surface_flags> flags;
	byte best_plane_calculation_vertex_index;
};
static_assert(sizeof(collision_surface) == 0xC);

struct large_collision_surface
{
	long plane;
	long first_edge;
	short material;
	short breakable_surface_set;
	short breakable_surface;
	c_flags<e_surface_flags, byte, k_collision_surface_flags> flags;
	byte best_plane_calculation_vertex_index;
};
static_assert(sizeof(large_collision_surface) == 0x10);

struct collision_edge
{
	short vertex_indices[2];
	short edge_indices[2];
	short surface_indices[2];
};
static_assert(sizeof(collision_edge) == 0xC);

struct large_collision_edge
{
	long vertex_indices[2];
	long edge_indices[2];
	long surface_indices[2];
};
static_assert(sizeof(large_collision_edge) == 0x18);

struct collision_vertex
{
	real_point3d point;
	short first_edge;
	short sink;
};
static_assert(sizeof(collision_vertex) == 0x10);

struct large_collision_vertex
{
	real_point3d point;
	long first_edge;
	long sink;
};
static_assert(sizeof(large_collision_vertex) == 0x14);

struct s_render_cluster_part
{
	dword_flags flags;
	s_cluster_reference cluster_reference;
	word mesh_index;
	word part_index;
};
static_assert(sizeof(s_render_cluster_part) == 0xC);

struct render_instance_mesh
{
	dword_flags flags;
	short structure_bsp_index;
	word lightmap_instance_index;
	word __unknown8;
	word part_index;
	byte __unknownC;
};
static_assert(sizeof(render_instance_mesh) == 0x10);

struct scenario_lightmap_bsp_data_definition;
struct s_render_geometry;
struct render_structure_globals
{
	struct
	{
		dword_flags flags;
		c_static_array<long, 16> lightmap_bsp_type;
		c_static_array<scenario_lightmap_bsp_data_definition*, 16> lightmap_bsp_data;
		c_static_array<s_render_geometry*, 16> render_geometry;
	} cached;

	c_static_sized_dynamic_array<s_render_cluster_part, 2048> render_cluster_parts;
	c_static_sized_dynamic_array<render_instance_mesh, 3072> render_instance_meshes;

	long marker_index;

	long render_cluster_part_markers[6];
	long render_instance_mesh_markers[6];

	long scenario_sbsp_index;
	long lightmap_cluster_reference;
	long lightmap_instance_index;
};
static_assert(sizeof(render_structure_globals) == 0x1210C);

extern render_structure_globals& g_render_structure_globals;

