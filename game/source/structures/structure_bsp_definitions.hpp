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

struct leaf_map
{
	int32 unused;
	s_tag_block leaves;
	s_tag_block connections;
};
static_assert(sizeof(leaf_map) == 0x1C);

struct structure_seam_identifier;
struct structure_edge_to_seam_edge_mapping;
struct structure_collision_material;
struct structure_surface;
struct structure_surface_to_triangle_mapping;
struct cluster_portal;
struct structure_weather_palette_entry;
struct structure_cluster;
struct structure_conveyor_surface;
struct structure_marker;
struct structure_runtime_decal;
struct structure_instanced_geometry_instance;
struct structure_bsp
{
	static tag const k_group_tag = SCENARIO_STRUCTURE_BSP_TAG;

	int32 import_info_checksum;
	int32 import_version;
	c_string_id visible_name;
	c_flags<e_structure_bsp_flags, uns32, k_structure_bsp_flags> flags;
	c_typed_tag_block<structure_seam_identifier> seam_identifiers;
	c_typed_tag_block<structure_edge_to_seam_edge_mapping> edge_to_seam_edge;
	c_typed_tag_block<structure_collision_material> collision_materials;
	c_typed_tag_block<int8 /* int8 cluster; */> leaves;
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
	c_typed_tag_block<structure_conveyor_surface> conveyor_surfaces;
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
	leaf_map leaf_map;
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
	int32 seam_id[4];
};
static_assert(sizeof(structure_seam_identifier) == 0x10);

struct structure_edge_to_seam_edge_mapping
{
	int16 seam_index;
	int16 seam_edge_index;
};
static_assert(sizeof(structure_edge_to_seam_edge_mapping) == 0x4);

struct structure_collision_material
{
	c_typed_tag_reference<RENDER_METHOD_TAG, INVALID_TAG> render_method; // old shader
	int16 runtime_global_material_index;
	int16 conveyor_surface_index;
	int16 seam_mapping_index;
	byte pad[0x2];
};
static_assert(sizeof(structure_collision_material) == 0x18);

struct structure_surface
{
	int32 first_structure_surface_to_triangle_mapping_index;
	int32 structure_surface_to_triangle_mapping_count;
};
static_assert(sizeof(structure_surface) == 0x8);

struct structure_surface_to_triangle_mapping
{
	int16 triangle_index;
	int16 section_index;
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
	int16 back_cluster;
	int16 front_cluster;
	int32 plane_index;
	real_point3d centroid;
	real32 bounding_radius;
	c_flags<e_cluster_portal_flags, uns32, k_cluster_portal_flags> flags;
	c_typed_tag_block<real_point3d> vertices;
};
static_assert(sizeof(cluster_portal) == 0x28);

struct structure_weather_palette_entry
{
	char name[32];
	uns8 SHTA[2];
	uns8 FI[2];
	uns8 EJMGHGBY[32];
	real_vector3d wind_direction;
	real32 wind_magnitude;
	uns8 XQLJZUE[2];
	int16 runtime_wind_global_scenario_function_index;
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
	int8 scenario_sky_index;
	int8 atmosphere_index;
	int8 camera_fx_index;
	byte ERERRER[0x1];
	int16 acoustics;
	int16 acoustics_sound_cluster_index;
	int16 background_sound;
	int16 sound_environment;
	int16 weather;
	int16 background_sound_sound_cluster_index;
	int16 reverb_sound_cluster_index;
	int16 runtime_first_decal_index;
	int16 runtime_decal_count; // `runtime decal cound` is misspelled is the tag definition, thanks Bungie
	c_flags<e_structure_cluster_flags, uns16, k_structure_cluster_flags> flags;
	s_tag_block predicted_resources;
	s_tag_block portals;
	s_collision_instanced_geometry_definition collision_instanced_geometry;
	int16 mesh_index;
	byte FERAIDF[0x2];
	s_tag_block seam_indices;
	s_tag_block decorator_groups;
	s_tag_block pvs_bound_object_identifiers;
	s_tag_block pvs_bound_object_references;
	c_typed_tag_block<s_structure_cluster_cubemap_info> cluster_cubemaps;
};
static_assert(sizeof(structure_cluster) == 0xDC);

struct structure_conveyor_surface
{
	real_vector3d u;
	real_vector3d v;
};
static_assert(sizeof(structure_conveyor_surface) == 0x18);

struct structure_marker
{
	c_static_string<k_tag_string_length> name;
	real_quaternion rotation;
	real_point3d position;
};
static_assert(sizeof(structure_marker) == 0x3C);

struct structure_runtime_decal
{
	int16 decal_palette_index;
	byte padding[2];

	real_quaternion rotation;
	real_point3d position;
	real32 scale;
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
	real_matrix4x3 world_transform;
	int16 definition_block_index;
	c_flags<e_instanced_geometry_flags, uns16, k_instanced_geometry_flags> flags;
	int16 lightmap_texcoord_block_index;
	uns16 pad;
	int16 runtime_subcluster_render_first_bitvector_index;
	int16 runtime_magic_render_number;
	real_point3d world_bounding_sphere_center;
	real32 world_bounding_sphere_radius;
	int32 transform_checksum;
	c_string_id name;
	c_enum<e_instanced_geometry_pathfinding_policy, int16, _instanced_geometry_pathfinding_policy_cut_out, k_instanced_geometry_pathfinding_policy_count> pathfinding_policy;
	c_enum<e_instanced_geometry_lightmapping_policy, int16, _instanced_geometry_lightmapping_policy_per_pixel_seperate, k_instanced_geometry_lightmapping_policy_count> lightmapping_policy;
	real32 lightmap_resolution_scale;
	s_tag_block bsp_physics;
	int16 fade_pixels_start;
	int16 fade_pixels_end;
	int16 cubemap_0_bitmap_index;
	int16 cubemap_1_bitmap_index;
};
static_assert(sizeof(structure_instanced_geometry_instance) == 0x74);

extern void __cdecl structure_bsp_compute_cluster_active_pvs(s_cluster_reference cluster_reference, s_game_cluster_bit_vectors* destination_pvs);

