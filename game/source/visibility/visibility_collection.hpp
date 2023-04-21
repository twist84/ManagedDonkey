#pragma once

#include "cseries/cseries.hpp"

long const k_maximum_item_markers = 6;

struct s_visible_object_hierarchy
{
	byte __data[0x10];
};
static_assert(sizeof(s_visible_object_hierarchy) == 0x10);

struct s_visible_object_render_visibility
{
	byte __data[0x78];
};
static_assert(sizeof(s_visible_object_render_visibility) == 0x78);

struct s_visible_instance_list
{
	byte __data[0xC];
};
static_assert(sizeof(s_visible_instance_list) == 0xC);

struct s_visible_instances
{
	word_flags flags;
	char structure_bsp_index;
	byte cluster_index;
	byte __data4[0x2];
	word region_cluster_bitvector_start_index;
	byte __data8[0x4];
	dword* part_bitvector_space;
};
static_assert(sizeof(s_visible_instances) == 0x10);

struct s_visible_clusters
{
	byte __data[0x10];
};
static_assert(sizeof(s_visible_clusters) == 0x10);

template<typename t_type, word k_maximum_count>
struct c_simple_list
{
	//c_simple_list()

	//long get_count()
	//void set_count(word)

	//bool valid(long)

	//long add()
	//void clear()

	//t_type& operator[](long)

	//t_type* list_iterator_next(long*)
	//t_type* list_iterator_new(long*, long)

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
};
static_assert(sizeof(s_visible_items) == 0x28AC0);

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

//private:
	static s_visible_items& m_items;
	static s_marker_indices(&m_markers)[k_maximum_item_markers];
};

