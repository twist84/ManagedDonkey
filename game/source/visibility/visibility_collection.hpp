#pragma once

#include "cseries/cseries.hpp"
#include "effects/effects.hpp"

long const k_maximum_item_markers = 6;

#pragma pack(push, 1)
struct s_visible_object_hierarchy
{
	word_flags flags;
	word __unknown2;
	dword __unknown4;
	word __unknown8;
	dword __unknownA;
	byte __data[0x2];
};
static_assert(sizeof(s_visible_object_hierarchy) == 0x10);
#pragma pack(pop)

struct c_dynamic_cubemap_sample
{
	word __unknown0;
	word __unknown2;
	word __unknown4;
	word __unknown6;
	word __unknown8;
	word __unknownA;
	real __unknownC;
};
static_assert(sizeof(c_dynamic_cubemap_sample) == 0x10);

struct s_shader_extern_info
{
	dword __unknown0;
	dword __unknown4;
	dword __unknown8;
	dword __unknownC;
	vector3d negative_up;
	s_geometry_sample lightprobe_sample;
	c_dynamic_cubemap_sample cubemap_sample;
	dword __unknown224;
	byte __data228[0x34];
};
static_assert(sizeof(s_shader_extern_info) == 0x25C);

struct s_visible_object_render_visibility
{
	dword __unknown0;
	dword model_tag_index;
	dword __unknown8;
	byte __dataC[0x5C];
	s_shader_extern_info* shader_extern_info;
	long object_index;
	dword __unknown70;
	byte_flags flags;
};
static_assert(sizeof(s_visible_object_render_visibility) == 0x78);

struct s_visible_instance_list
{
	char structure_bsp_index;
	byte __data[0x9];
	word __unknownA;
};
static_assert(sizeof(s_visible_instance_list) == 0xC);

struct s_visible_instances
{
	word_flags flags;
	word structure_bsp_instanced_geometry_instances_index; // structure_bsp.instanced_geometry_instances[instanced_geometry_instances_index]
	short structure_bsp_index; // global_structure_bsp_get(structure_bsp_index)
	word region_cluster_bitvector_start_index;
	byte __data8[4];
	dword* part_bitvector_space;
};
static_assert(sizeof(s_visible_instances) == 0x10);

struct s_visible_clusters
{
	word_flags flags;
	s_cluster_reference cluster_reference;
	byte __data4[0x4];
	union
	{
		word render_geometry_mesh_index;
		word render_geometry_per_mesh_mopp_index;
	};
	byte __dataA[0x2];
	dword* part_visibility_bitvector;
};
static_assert(sizeof(s_visible_clusters) == 0x10);

template<typename t_type, word k_maximum_count>
struct c_simple_list
{
	//c_simple_list()

	long get_count()
	{
		return m_count;
	}

	//void set_count(word)

	//bool valid(long)

	//long add()
	//void clear()

	t_type& operator[](long index)
	{
		return m_elements[index];
	}

	t_type* list_iterator_next(long* a1)
	{
		if (*a1 >= m_count - 1)
		{
			*a1 = NONE;
			return nullptr;
		}

		return m_elements[++*a1];
	}

	t_type* list_iterator_new(long* a1, long a2)
	{
		*a1 = a2 - 1;

		return list_iterator_next(a1);
	}

	word m_maximum_count = k_maximum_count;
	word m_count;

	t_type m_elements[k_maximum_count];
};
static_assert(sizeof(c_simple_list<long, 6>) == 0x1C);

struct s_visible_items
{
	c_simple_list<s_visible_object_hierarchy, 768> root_objects;    // address: 0x018BF6E8, offset: 0x00000
	c_simple_list<s_visible_object_render_visibility, 896> objects; // address: 0x018C26EC, offset: 0x30040
	c_simple_list<s_visible_instance_list, 1024> instance_list;     // address: 0x018DCAF0, offset: 0x1D408
	c_simple_list<s_visible_instances, 1152> instances;             // address: 0x018DFAF4, offset: 0x2040C
	c_simple_list<s_visible_clusters, 348> clusters;                // address: 0x018E42F8, offset: 0x24C10
	c_simple_list<long, 50> lights;                                 // address: 0x018E58BC, offset: 0x261D4
	c_simple_list<long, 6> skies;                                   // address: 0x018E5988, offset: 0x262A0
	c_static_flags<16384> visible_subpart_bitvector;                // address: 0x018E59A4, offset: 0x262BC
	word visible_subpart_bitvector_count;                           // address: 0x018E61A4, offset: 0x26ABC
	word visiblity_region_cluster_bitvector_count;                  // address: 0x018E61A6, offset: 0x26ABE
	c_static_flags<65536> visiblity_region_cluster_bitvector;       // address: 0x018E61A8, offset: 0x26AC0
	word __unknown28AC0[2048];                                      // address: 0x018E81A8, offset: 0x28AC0
};
static_assert(sizeof(s_visible_items) == 0x29AC0);

struct c_visible_items
{
	struct s_marker_indices
	{
		word root_objects_starting_index;
		word objects_starting_index;
		word instance_list_starting_index;
		word instances_starting_index;
		word cluster_starting_index;
		word light_starting_index;
		word visible_subpart_bitvector_count;
		word visiblity_region_cluster_bitvector_count;
		word sky_starting_index;
	};

	static word __cdecl get_root_objects_starting_index()
	{
		return m_marker_indices[m_marker_count].root_objects_starting_index;
	}

	static word __cdecl get_objects_starting_index()
	{
		return m_marker_indices[m_marker_count].objects_starting_index;
	}

	static word __cdecl get_instance_list_starting_index()
	{
		return m_marker_indices[m_marker_count].instance_list_starting_index;
	}

	static word __cdecl get_instances_starting_index()
	{
		return m_marker_indices[m_marker_count].instances_starting_index;
	}

	static word __cdecl get_cluster_starting_index()
	{
		return m_marker_indices[m_marker_count].cluster_starting_index;
	}

	static word __cdecl get_light_starting_index()
	{
		return m_marker_indices[m_marker_count].light_starting_index;
	}

	static word __cdecl get_sky_starting_index()
	{
		return m_marker_indices[m_marker_count].sky_starting_index;
	}

//private:
	static long& m_marker_count;
	static s_visible_items& m_items;
	static s_marker_indices(&m_marker_indices)[k_maximum_item_markers];
};

