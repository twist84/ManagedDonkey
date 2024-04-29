#pragma once

#include "cseries/cseries.hpp"
#include "geometry/geometry_definitions.hpp"
#include "physics/collision_instanced_geometry_definitions.hpp"
#include "render/render_structure.hpp"
#include "scenario/scenario_cubemap_definitions.hpp"
#include "structures/structure_physics_definitions.hpp"
#include "tag_files/tag_groups.hpp"

struct structure_atmosphere_palette_entry;
struct structure_camera_fx_palette_entry;
struct structure_weather_palette_entry;

enum e_structure_bsp_flags
{
	_structure_bsp_flag_lightmap_compressed_bit = 0,

	k_structure_bsp_flags
};

struct structure_seam_identifier;
struct structure_edge_to_seam_edge_mapping;
struct structure_collision_material;
struct structure_surface;
struct structure_surface_to_triangle_mapping;
struct cluster_portal;
struct structure_weather_palette_entry;
struct structure_cluster;
struct structure_marker;
struct structure_runtime_decal;
struct structure_instanced_geometry_instance;
struct structure_bsp
{
	static tag const k_group_tag = SCENARIO_STRUCTURE_BSP_TAG;

	long import_info_checksum;
	long import_version;
	c_string_id visible_name;
	c_flags<e_structure_bsp_flags, dword, k_structure_bsp_flags> flags;
	c_typed_tag_block<structure_seam_identifier> seam_identifiers;
	c_typed_tag_block<structure_edge_to_seam_edge_mapping> edge_to_seam_edge;
	c_typed_tag_block<structure_collision_material> collision_materials;
	c_typed_tag_block<char /* char cluster; */> leaves;
	real_rectangle3d world_bounds;
	c_typed_tag_block<structure_surface> structure_surfaces;
	c_typed_tag_block<structure_surface> large_structure_surfaces;
	c_typed_tag_block<structure_surface_to_triangle_mapping> structure_surface_to_triangle_mapping;
	c_typed_tag_block<cluster_portal> cluster_portals;
	c_typed_tag_block<structure_weather_palette_entry> weather_palette;
	s_tag_block atmosphere_palette;
	s_tag_block camera_fx_palette;
	s_tag_block weather_polyhedra;
	s_tag_block detail_objects;
	c_typed_tag_block<structure_cluster> clusters;
	s_tag_block materials;
	s_tag_block sky_owner_cluster;
	s_tag_block conveyor_surfaces;
	s_tag_block breakable_surfaces;
	s_tag_block pathfinding_data;
	s_tag_block pathfinding_edges;
	s_tag_block acoustics_palette;
	s_tag_block background_sound_palette;
	s_tag_block sound_environment_palette;
	s_tag_data sound_pas_data;
	c_typed_tag_block<structure_marker> markers;
	s_tag_block marker_light_palette;
	s_tag_block marker_light_palette_index;
	c_typed_tag_block<structure_runtime_decal> runtime_decals;
	s_tag_block environment_object_palette;
	s_tag_block environment_objects;
	byte IMISWXUG[0x4];
	s_tag_block leaf_map_leaves;
	s_tag_block leaf_map_connections;
	s_tag_block errors;
	c_typed_tag_block<structure_instanced_geometry_instance> instanced_geometry_instances;
	s_tag_block decorator_sets;
	s_render_geometry decorator_instance_buffer;
	s_tag_block acoustics_sound_clusters;
	s_tag_block ambience_sound_clusters;
	s_tag_block reverb_sound_clusters;
	s_tag_block transparent_planes;
	s_tag_block debug_info;
	s_structure_physics structure_physics;
	s_tag_block audibility;
	s_tag_block object_fake_lightprobes;
	s_render_geometry render_geometry;
	s_tag_block widget_references;
	c_structure_bsp_resource_interface resource_interface;
};
static_assert(sizeof(structure_bsp) == 0x3A0);

struct structure_seam_identifier
{
	long seam_id[4];
};
static_assert(sizeof(structure_seam_identifier) == 0x10);

struct structure_edge_to_seam_edge_mapping
{
	short seam_index;
	short seam_edge_index;
};
static_assert(sizeof(structure_edge_to_seam_edge_mapping) == 0x4);

struct structure_collision_material
{
	c_typed_tag_reference<RENDER_METHOD_TAG> render_method; // old shader
	short runtime_global_material_index;
	short conveyor_surface_index;
	short seam_mapping_index;
	byte pad[0x2];
};
static_assert(sizeof(structure_collision_material) == 0x18);

struct structure_surface
{
	long first_structure_surface_to_triangle_mapping_index;
	long structure_surface_to_triangle_mapping_count;
};
static_assert(sizeof(structure_surface) == 0x8);

struct structure_surface_to_triangle_mapping
{
	short triangle_index;
	short section_index;
};
static_assert(sizeof(structure_surface_to_triangle_mapping) == 0x4);

enum e_cluster_portal_flags
{
	_cluster_portal_ai_cant_hear_through_this_shit_bit,
	_cluster_portal_one_way_bit,
	_cluster_portal_door_bit,
	_cluster_portal_no_way_bit,
	_cluster_portal_one_way_reversed_bit,
	_cluster_portal_no_one_can_hear_through_this_bit,

	k_cluster_portal_flags
};

struct cluster_portal
{
	short back_cluster;
	short front_cluster;
	long plane_index;
	real_point3d centroid;
	real bounding_radius;
	c_flags<e_cluster_portal_flags, dword, k_cluster_portal_flags> flags;
	c_typed_tag_block<real_point3d> vertices;
};
static_assert(sizeof(cluster_portal) == 0x28);

struct structure_weather_palette_entry
{
	char name[32];
	byte SHTA[2];
	byte FI[2];
	byte EJMGHGBY[32];
	vector3d wind_direction;
	real wind_magnitude;
	byte XQLJZUE[2];
	short runtime_wind_global_scenario_function_index;
	char wind_scale_function[32];
};
static_assert(sizeof(structure_weather_palette_entry) == 0x78);

enum e_structure_cluster_flags
{
	_structure_cluster_flag_one_way_portal_bit = 0,
	_structure_cluster_flag_door_portal_bit,
	_structure_cluster_flag_postprocessed_geometry_bit,
	_structure_cluster_flag_is_the_sky_bit,
	_structure_cluster_flag_decorators_are_lit_bit,

	k_structure_cluster_flags
};

struct structure_cluster
{
	// CLUSTER INFO

	real_rectangle3d bounds;
	char scenario_sky_index;
	char atmosphere_index;
	char camera_fx_index;
	byte ERERRER[0x1];
	short acoustics;
	short acoustics_sound_cluster_index;
	short background_sound;
	short sound_environment;
	short weather;
	short background_sound_sound_cluster_index;
	short reverb_sound_cluster_index;
	short runtime_first_decal_index;
	short runtime_decal_count; // `runtime decal cound` is misspelled is the tag definition, thanks Bungie
	c_flags<e_structure_cluster_flags, word, k_structure_cluster_flags> flags;
	s_tag_block predicted_resources;
	s_tag_block portals;
	s_collision_instanced_geometry_definition collision_instanced_geometry;
	short mesh_index;
	byte FERAIDF[0x2];
	s_tag_block seam_indices;
	s_tag_block decorator_groups;
	s_tag_block pvs_bound_object_identifiers;
	s_tag_block pvs_bound_object_references;
	c_typed_tag_block<s_structure_cluster_cubemap_info> cluster_cubemaps;
};
static_assert(sizeof(structure_cluster) == 0xDC);

struct structure_marker
{
	c_static_string<k_tag_string_length> name;
	real_quaternion rotation;
	real_point3d position;
};
static_assert(sizeof(structure_marker) == 0x3C);

struct structure_runtime_decal
{
	short decal_palette_index;
	byte padding[2];

	real_quaternion rotation;
	real_point3d position;
	real scale;
};
static_assert(sizeof(structure_runtime_decal) == 0x24);

enum e_instanced_geometry_flags
{
	_instanced_geometry_not_in_lightprobes_bit = 0,
	_instanced_geometry_render_only_bit,
	_instanced_geometry_does_not_block_aoe_damage_bit,
	_instanced_geometry_collidable_bit,
	_instanced_geometry_decal_spacing_bit,
	_instanced_geometry_bit5,
	_instanced_geometry_bit6,

	k_instanced_geometry_flags
};

enum e_instanced_geometry_pathfinding_policy
{
	_instanced_geometry_pathfinding_policy_cut_out = 0,
	_instanced_geometry_pathfinding_policy_static,
	_instanced_geometry_pathfinding_policy_none,

	k_instanced_geometry_pathfinding_policy_count,
};

enum e_instanced_geometry_lightmapping_policy
{
	_instanced_geometry_lightmapping_policy_per_pixel_seperate = 0,
	_instanced_geometry_lightmapping_policy_per_vertex,
	_instanced_geometry_lightmapping_policy_single_probe,
	_instanced_geometry_lightmapping_policy_per_pixel_shared,

	k_instanced_geometry_lightmapping_policy_count
};

struct structure_instanced_geometry_instance
{
	real_matrix4x3 matrix;
	short instance_definition;
	c_flags<e_instanced_geometry_flags, word, k_instanced_geometry_flags> flags;
	short lightmap_texcoord_block_index;
	byte BJOLSRJV[0x2];
	short runtime_subcluster_render_first_bitvector_index;
	short runtime_magic_render_number;
	real_point3d world_bounding_sphere_center;
	real world_bounding_sphere_radius;
	dword checksum;
	string_id name;
	c_enum<e_instanced_geometry_pathfinding_policy, short, _instanced_geometry_pathfinding_policy_cut_out, k_instanced_geometry_pathfinding_policy_count> pathfinding_policy;
	c_enum<e_instanced_geometry_lightmapping_policy, short, _instanced_geometry_lightmapping_policy_per_pixel_seperate, k_instanced_geometry_lightmapping_policy_count> lightmapping_policy;
	real lightmap_resolution_scale;
	s_tag_block bsp_physics;
	short fade_pixels_start;
	short fade_pixels_end;
	short cubemap0_bitmap_index;
	short cubemap1_bitmap_index;
};
static_assert(sizeof(structure_instanced_geometry_instance) == 0x74);

