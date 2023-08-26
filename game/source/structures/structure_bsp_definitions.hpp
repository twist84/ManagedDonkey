#pragma once

#include "cseries/cseries.hpp"
#include "geometry/geometry_definitions.hpp"
#include "physics/collision_instanced_geometry_definitions.hpp"
#include "render/render_structure.hpp"
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

struct structure_cluster;
struct structure_marker;
struct structure_bsp
{
	static tag const k_group_tag = SCENARIO_STRUCTURE_BSP_TAG;

	long import_info_checksum;
	long import_version;
	c_string_id visible_name;
	c_flags<e_structure_bsp_flags, dword, k_structure_bsp_flags> flags;
	s_tag_block seam_identifiers;
	s_tag_block edge_to_seam_edge;
	s_tag_block collision_materials;
	s_tag_block leaves;
	real_rectangle3d world_bounds;
	s_tag_block structure_surfaces;
	s_tag_block large_structure_surfaces;
	s_tag_block structure_surface_to_triangle_mapping;
	s_tag_block cluster_portals;
	s_tag_block weather_palette;
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
	s_tag_block acoustics_palette;
	s_tag_block background_sound_palette;
	s_tag_block sound_environment_palette;
	s_tag_data sound_pas_data;
	c_typed_tag_block<structure_marker> markers;
	s_tag_block marker_light_palette;
	s_tag_block marker_light_palette_index;
	s_tag_block runtime_decals;
	s_tag_block environment_object_palette;
	s_tag_block environment_objects;
	byte IMISWXUG[0x4];
	s_tag_block leaf_map_leaves;
	s_tag_block leaf_map_connections;
	s_tag_block errors;
	s_tag_block instanced_geometry_instances;
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
	s_tag_block leaf_systems;
	s_tag_block widget_references;
	c_structure_bsp_resource_interface resource_interface;
};
static_assert(sizeof(structure_bsp) == 0x3A0);

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
	short runtime_decal_cound;
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
	s_tag_block cluster_cubemaps;
};
static_assert(sizeof(structure_cluster) == 0xDC);

struct structure_marker
{
	c_static_string<32> name;
	real_quaternion rotation;
	real_point3d position;
};
static_assert(sizeof(structure_marker) == 0x3C);

