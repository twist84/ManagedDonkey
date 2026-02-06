#pragma once

#include "cseries/cseries.hpp"
#include "cseries/location.hpp"

//#include <emmintrin.h>

enum
{
	k_subpart_bitvector_size_in_bits = 16384,

	k_maximum_region_memory_items = 512,
	k_maximum_item_markers = 6,
	k_maximum_sky_items = 6,
};

template<typename t_type, uns16 k_maximum_count>
class c_simple_list
{
public:
	c_simple_list()
	{
		m_maximum_count = k_maximum_count;
	}

	int32 add()
	{
		if (m_count >= m_maximum_count)
			return NONE;

		return m_count++;
	}

	void clear()
	{
		m_count = 0;
	}

	int32 get_count()
	{
		return m_count;
	}

	t_type* list_iterator_next(int32* iterator)
	{
		if (*iterator >= m_count - 1)
		{
			*iterator = NONE;
			return nullptr;
		}

		return data[++*iterator];
	}

	t_type* list_iterator_new(int32* iterator, int32 starting_index)
	{
		*iterator = starting_index - 1;

		return list_iterator_next(iterator);
	}

	void set_count(uns16 count)
	{
		ASSERT(count <= m_maximum_count);
		m_count = count;
	}

	bool valid(int32 index)
	{
		return index >= 0 && index < m_count;
	}

	t_type& operator[](int32 index)
	{
		ASSERT(valid(index));
		return data[index];
	}

	uns16 m_maximum_count;
	uns16 m_count;

	t_type data[k_maximum_count];
};
COMPILE_ASSERT(sizeof(c_simple_list<int32, 6>) == 0x1C);

struct s_zone_cluster
{
	s_cluster_reference cluster_reference;
	int16 zone_index;
};
COMPILE_ASSERT(sizeof(s_zone_cluster) == 0x4);

struct s_lod_transparency
{
	uns8 model_alpha;
	uns8 instance_alpha;
	uns8 shadow_alpha;
	uns8 unused_alpha;
};
COMPILE_ASSERT(sizeof(s_lod_transparency) == 0x4);

struct s_visible_object_hierarchy
{
	int16 flags;
	uns16 region_cluster_bitvector_start_index;
	int32 first_visibility_object_index;
	uns16 visibility_object_index_count;
	s_lod_transparency lod_transparency;
};
COMPILE_ASSERT(sizeof(s_visible_object_hierarchy) == 0x10);

struct s_shader_extern_info;
struct s_render_object_info
{
	int32 region_count;
	int32 render_model_index;
	int32 skinning_memory_designator;
	uns8 skinning_matrix_count;
	uns8 region_mesh_indices[16];
	uns16 region_z_sort_offset_enum_index[16];
	uns16 __unknown3E[16];
	int16 lod_index;
	int16 lightmap_object_index;
	uns32 clip_plane_masks;
	s_shader_extern_info* render_info;
};
COMPILE_ASSERT(sizeof(s_render_object_info) == 0x6C);

struct s_visible_object_render_visibility
{
	s_render_object_info info;
	int32 object_index;
	uns32* subpart_bitvector;
	uns8 flags;
};
COMPILE_ASSERT(sizeof(s_visible_object_render_visibility) == 0x78);

struct s_visible_instance_list
{
	s_zone_cluster cluster;
	uns16 instance_list_index;
	uns16 first_visible_instance_list_instance_index;
	uns16 visible_instance_list_count;
	uns16 mesh_index;
};
COMPILE_ASSERT(sizeof(s_visible_instance_list) == 0xC);

struct s_visible_instances
{
	int16 flags;
	uns16 structure_bsp_instance_index;
	int16 structure_bsp_index;
	uns16 region_cluster_bitvector_start_index;
	uns8 alpha_byte;
	uns32* part_bitvector;
};
COMPILE_ASSERT(sizeof(s_visible_instances) == 0x10);

struct s_visible_clusters
{
	int16 flags;
	s_zone_cluster cluster;
	uns16 region_cluster_index;
	uns16 mesh_index;
	uns32* part_bitvector;
};
COMPILE_ASSERT(sizeof(s_visible_clusters) == 0x10);

struct s_visible_items
{
	c_simple_list<s_visible_object_hierarchy, 768> root_objects;    // address: 0x018BF6E8, offset: 0x00000
	c_simple_list<s_visible_object_render_visibility, 896> objects; // address: 0x018C26EC, offset: 0x30040
	c_simple_list<s_visible_instance_list, 1024> instance_list;     // address: 0x018DCAF0, offset: 0x1D408
	c_simple_list<s_visible_instances, 1152> instances;             // address: 0x018DFAF4, offset: 0x2040C
	c_simple_list<s_visible_clusters, 348> clusters;                // address: 0x018E42F8, offset: 0x24C10
	c_simple_list<int32, 50> lights;                                // address: 0x018E58BC, offset: 0x261D4
	c_simple_list<int32, 6> skies;                                  // address: 0x018E5988, offset: 0x262A0
	c_static_flags<16384> visible_subpart_bitvector;                // address: 0x018E59A4, offset: 0x262BC
	uns16 visible_subpart_bitvector_count;                          // address: 0x018E61A4, offset: 0x26ABC
	uns16 visiblity_region_cluster_bitvector_count;                 // address: 0x018E61A6, offset: 0x26ABE
	c_static_flags<65536> visibility_region_cluster_bitvector;      // address: 0x018E61A8, offset: 0x26AC0
	uns16 __unknown28AC0[2048];                                     // address: 0x018E81A8, offset: 0x28AC0
};
COMPILE_ASSERT(sizeof(s_visible_items) == 0x29AC0);

class c_visible_items
{
public:
	struct s_marker_indices
	{
		uns16 root_objects_starting_index;
		uns16 objects_starting_index;
		uns16 instance_list_starting_index;
		uns16 instances_starting_index;
		uns16 cluster_starting_index;
		uns16 light_starting_index;
		uns16 visible_subpart_bitvector_count;
		uns16 visiblity_region_cluster_bitvector_count;
		uns16 sky_starting_index;
	};

	static void __cdecl clear();
	static void __cdecl clear_all();

	static int32 __cdecl get_camera_root_objects_count();
	static uns16 __cdecl get_cluster_starting_index();
	static uns16 __cdecl get_instance_list_starting_index();
	static uns16 __cdecl get_instances_starting_index();
	static uns16 __cdecl get_light_starting_index();
	static uns16 __cdecl get_objects_starting_index();
	static uns16 __cdecl get_root_objects_starting_index();
	static uns16 __cdecl get_sky_starting_index();
	static void __cdecl pop_marker();
	static void __cdecl push_marker();

//private:
	static int32& m_marker_count;
	static s_visible_items& m_items;
	static s_marker_indices(&m_marker_indices)[k_maximum_item_markers];
};

struct visibility_volume
{
	byte vector_planes[0x10 * 6];
	//__m128 vector_planes[6];

	int16 projection_index;
	int16 region_cluster_index;
	int32 plane_index;
	int16 plane_bsp_index;
	int16 parent_volume_index;
	real_rectangle2d frustum_bounds;
	real32 portal_nearest_z;
	real_point3d world_vertices[5];
	real_rectangle3d world_bounds;
	real_plane3d world_planes[6];
	real_vector3d world_edge_vectors[4];
};
COMPILE_ASSERT(sizeof(visibility_volume) == 0x164);
COMPILE_ASSERT(0x000 == OFFSETOF(visibility_volume, vector_planes));
COMPILE_ASSERT(0x060 == OFFSETOF(visibility_volume, projection_index));
COMPILE_ASSERT(0x062 == OFFSETOF(visibility_volume, region_cluster_index));
COMPILE_ASSERT(0x064 == OFFSETOF(visibility_volume, plane_index));
COMPILE_ASSERT(0x068 == OFFSETOF(visibility_volume, plane_bsp_index));
COMPILE_ASSERT(0x06A == OFFSETOF(visibility_volume, parent_volume_index));
COMPILE_ASSERT(0x06C == OFFSETOF(visibility_volume, frustum_bounds));
COMPILE_ASSERT(0x07C == OFFSETOF(visibility_volume, portal_nearest_z));
COMPILE_ASSERT(0x080 == OFFSETOF(visibility_volume, world_vertices));
COMPILE_ASSERT(0x0BC == OFFSETOF(visibility_volume, world_bounds));
COMPILE_ASSERT(0x0D4 == OFFSETOF(visibility_volume, world_planes));
COMPILE_ASSERT(0x134 == OFFSETOF(visibility_volume, world_edge_vectors));

struct visibility_projection
{
	bool parallel_projection_flag;
	byte pad0[0x3];
	real_matrix4x3 world_to_basis;
	real_matrix4x3 basis_to_world;
	bool near_bounded_flag;
	byte pad1[0x3];
	real32 near_distance;
	real_plane3d near_plane;
	bool far_bounded_flag;
	bool far_bounded_spherical_flag;
	byte pad2[0x2];
	real32 far_distance;
	bool volume_bounded_flag;
	byte pad3[0x3];
	visibility_volume volume;
	int32 convex_hull_point_count;
	real_point2d convex_hull_points[4];
};
COMPILE_ASSERT(sizeof(visibility_projection) == 0x218);
COMPILE_ASSERT(0x000 == OFFSETOF(visibility_projection, parallel_projection_flag));
COMPILE_ASSERT(0x001 == OFFSETOF(visibility_projection, pad0));
COMPILE_ASSERT(0x004 == OFFSETOF(visibility_projection, world_to_basis));
COMPILE_ASSERT(0x038 == OFFSETOF(visibility_projection, basis_to_world));
COMPILE_ASSERT(0x06C == OFFSETOF(visibility_projection, near_bounded_flag));
COMPILE_ASSERT(0x06D == OFFSETOF(visibility_projection, pad1));
COMPILE_ASSERT(0x070 == OFFSETOF(visibility_projection, near_distance));
COMPILE_ASSERT(0x074 == OFFSETOF(visibility_projection, near_plane));
COMPILE_ASSERT(0x084 == OFFSETOF(visibility_projection, far_bounded_flag));
COMPILE_ASSERT(0x085 == OFFSETOF(visibility_projection, far_bounded_spherical_flag));
COMPILE_ASSERT(0x086 == OFFSETOF(visibility_projection, pad2));
COMPILE_ASSERT(0x088 == OFFSETOF(visibility_projection, far_distance));
COMPILE_ASSERT(0x08C == OFFSETOF(visibility_projection, volume_bounded_flag));
COMPILE_ASSERT(0x08D == OFFSETOF(visibility_projection, pad3));
COMPILE_ASSERT(0x090 == OFFSETOF(visibility_projection, volume));
COMPILE_ASSERT(0x1F4 == OFFSETOF(visibility_projection, convex_hull_point_count));
COMPILE_ASSERT(0x1F8 == OFFSETOF(visibility_projection, convex_hull_points));

struct visibility_cluster
{
	s_cluster_reference cluster_reference;
	int16 volume_counts[6];
	int16 first_volume_indices[6];
};
COMPILE_ASSERT(sizeof(visibility_cluster) == 0x1A);
COMPILE_ASSERT(0x0 == OFFSETOF(visibility_cluster, cluster_reference));
COMPILE_ASSERT(0x2 == OFFSETOF(visibility_cluster, volume_counts));
COMPILE_ASSERT(0xE == OFFSETOF(visibility_cluster, first_volume_indices));

struct s_visibility_region
{
	int16 projection_count;
	visibility_projection projections[6];
	int16 cluster_count;
	visibility_cluster clusters[128];
	int32 cluster_remap_table[128];
	int16 volume_count;
	visibility_volume volumes[512];
};
COMPILE_ASSERT(sizeof(s_visibility_region) == 0x2E39C);
COMPILE_ASSERT(0x0000 == OFFSETOF(s_visibility_region, projection_count));
COMPILE_ASSERT(0x0004 == OFFSETOF(s_visibility_region, projections));
COMPILE_ASSERT(0x0C94 == OFFSETOF(s_visibility_region, cluster_count));
COMPILE_ASSERT(0x0C96 == OFFSETOF(s_visibility_region, clusters));
COMPILE_ASSERT(0x1998 == OFFSETOF(s_visibility_region, cluster_remap_table));
COMPILE_ASSERT(0x1B98 == OFFSETOF(s_visibility_region, volume_count));
COMPILE_ASSERT(0x1B9C == OFFSETOF(s_visibility_region, volumes));

enum e_collection_shape
{
	_visibility_collection_shape_projections = 0,
	_visibility_collection_shape_sphere,
	_visibility_collection_shape_pvs,

	k_number_collection_shape_types
};

enum e_collection_type
{
	_visibility_collection_type_camera = 0,
	_visibility_collection_type_light,
	_visibility_collection_type_lightmap_shadow,
	_visibility_collection_type_cinematic_shadow,

	k_number_collection_types
};

struct s_visibility_input
{
	s_visibility_region region;
	s_cluster_reference cluster_reference;
	int32 user_index;
	int32 player_window_index;
	int32 flags;
	int16 projection_count;
	real_point3d sphere_center;
	real32 sphere_radius;
	e_collection_type collection_type;
	e_collection_shape collection_shape;
	int32 visible_items_marker_index;
	uns32 visible_cluster_bitvector[16][8];
	c_static_array<c_static_array<int8, 256>, 16> cluster_to_visibility_cluster_lookup;
};
COMPILE_ASSERT(sizeof(s_visibility_input) == 0x2F5CC);
COMPILE_ASSERT(0x0 == OFFSETOF(s_visibility_input, region));
COMPILE_ASSERT(0x2E39C == OFFSETOF(s_visibility_input, cluster_reference));
COMPILE_ASSERT(0x2E3A0 == OFFSETOF(s_visibility_input, user_index));
COMPILE_ASSERT(0x2E3A4 == OFFSETOF(s_visibility_input, player_window_index));
COMPILE_ASSERT(0x2E3A8 == OFFSETOF(s_visibility_input, flags));
COMPILE_ASSERT(0x2E3AC == OFFSETOF(s_visibility_input, projection_count));
COMPILE_ASSERT(0x2E3B0 == OFFSETOF(s_visibility_input, sphere_center));
COMPILE_ASSERT(0x2E3BC == OFFSETOF(s_visibility_input, sphere_radius));
COMPILE_ASSERT(0x2E3C0 == OFFSETOF(s_visibility_input, collection_type));
COMPILE_ASSERT(0x2E3C4 == OFFSETOF(s_visibility_input, collection_shape));
COMPILE_ASSERT(0x2E3C8 == OFFSETOF(s_visibility_input, visible_items_marker_index));
COMPILE_ASSERT(0x2E3CC == OFFSETOF(s_visibility_input, visible_cluster_bitvector));
COMPILE_ASSERT(0x2E5CC == OFFSETOF(s_visibility_input, cluster_to_visibility_cluster_lookup));

class c_visibility_collection
{
public:
	s_cluster_reference get_cluster_reference() const
	{
		ASSERT(m_input != nullptr);
		return m_input->cluster_reference;
	}

	e_collection_type get_collection_type()
	{
		ASSERT(m_input != nullptr);
		ASSERT(m_input->collection_type >= 0 && m_input->collection_type < k_number_collection_types);
		return m_input->collection_type;
	}

	s_visibility_input* get_input()
	{
		return m_input;
	}

	const s_visibility_region* get_region() const
	{
		ASSERT(m_input != nullptr);
		return &m_input->region;
	}

	e_collection_shape prepare_collection_for_build(int32 flags, e_collection_type collection_type, const visibility_projection* projections, int32 projection_count, s_cluster_reference initial_cluster_reference, int32 intersection_marker_index, const real_point3d* sphere_center, real32 sphere_radius, int32 user_index, int32 player_window_index);

	static int32 __cdecl add_root_object(int32 object_index, const real_point3d* object_center, real32 object_radius, int32 player_window_index, bool lit, bool shadow_casting, bool fully_contained, int32 region_cluster_memory, s_lod_transparency lod_transparency, bool calculate_lod, bool ignore_first_person_objects, int32 ignore_first_person_user_index, uns16* a13);
	static void __cdecl expand_sky_object(int32 player_window_index);
	static void __cdecl generate_objects_visible_subparts();

	s_visibility_input* m_input;

	uns16* __unknown4;
	// pointer to `s_visible_items::__unknown28AC0`
};
COMPILE_ASSERT(sizeof(c_visibility_collection) == 0x8);

struct s_visibility_globals
{
	s_visibility_input g_camera_visibility_input;
	c_visibility_collection camera_visibility;
	c_visible_items visible_items;
};
COMPILE_ASSERT(sizeof(s_visibility_globals) == 0x2F5D8);

struct __declspec(align(16)) c_visibility_globals_keeper
{
	c_visibility_globals_keeper()
	{
		m_initialized = false;
	}

	~c_visibility_globals_keeper()
	{
	}

	c_visibility_globals_keeper* get()
	{
		//ASSERT(restricted_region_locked_for_current_thread(k_game_state_render_region));
		return this;
	}

	void initialize()
	{
		m_initialized = true;
		csmemset(&m_visibility_globals, 0, sizeof(s_visibility_globals));
	}

	s_visibility_globals m_visibility_globals;
	bool m_initialized;
};
COMPILE_ASSERT(sizeof(c_visibility_globals_keeper) == 0x2F5E0);

extern bool debug_pvs;
extern bool debug_pvs_render_all;
extern bool debug_pvs_activation;
extern bool debug_portals;
extern bool visibility_debug_visible_clusters;

extern c_visibility_globals_keeper& g_visibility_globals_keeper;

extern c_visibility_collection* __cdecl get_global_camera_collection();
extern s_visible_items& __cdecl get_global_items();
extern void __cdecl visibility_collection_dispose();
extern void __cdecl visibility_collection_initialize();

