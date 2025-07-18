#pragma once

#include "cseries/cseries.hpp"
#include "multithreading/message_queue.hpp"
#include "rasterizer/rasterizer_vertex_definitions.hpp"

struct s_game_non_bsp_zone_set;
struct s_game_cluster_bit_vectors;
class c_scenario_resource_registry;
class c_decal_system :
	public s_datum_header
{
public:
	static void __cdecl prepare_for_new_zone_set(uns32 old_active_structure_bsp_mask, uns32 new_active_structure_bsp_mask);
	static void __cdecl initialize_for_new_non_bsp_zone_set(const s_game_non_bsp_zone_set* new_non_bsp_zone_set);
	static void __cdecl dispose_from_old_non_bsp_zone_set(const s_game_non_bsp_zone_set* activating_non_bsp_zone_set);
	static void __cdecl change_pvs(const s_game_cluster_bit_vectors* old_cluster_activation, const s_game_cluster_bit_vectors* new_cluster_activation, bool local);
	static void __cdecl dispose();
	static void __cdecl dispose_from_old_map();
	static void __cdecl dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask);
	static void __cdecl initialize();
	static void __cdecl initialize_for_new_map();
	static void __cdecl initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask);
	static void __cdecl prepare_for_non_bsp_zone_set_switch(const s_game_non_bsp_zone_set* old_non_bsp_zone_set, const s_game_non_bsp_zone_set* new_non_bsp_zone_set, c_scenario_resource_registry* pending_zone_registry);
	static void __cdecl submit_all();

//private:
	c_tag_index m_definition_index;
	int32 m_first_decal_index;
	int32 m_flags;
	s_cluster_reference m_cluster_refs[4];
	real_matrix4x3 m_projection;
	real32 m_rotation;
};
static_assert(sizeof(c_decal_system) == 0x50);

struct s_decal_counts
{
	uns32 m_decal_system_count;
	uns32 m_decal_count;
	uns32 m_vertex_count;
	uns32 m_index_count;
};
static_assert(sizeof(s_decal_counts) == 0x10);

class c_decal :
	public s_datum_header
{
public:
	void __thiscall render(int32 pass);
	static void __cdecl render_all(int32 pass);

	int32 m_definition_block_index;
	int32 m_parent_system_index;
	int32 m_next_sibling_index;
	int32 m_vertex_allocation_index;
	int32 m_index_allocation_index;
	byte __data18[0xC];
	uns16 m_vertex_count;
	uns16 m_index_count;
	int32 m_flags;
	real32 m_age;
	real32 m_lifespan;
	real32 m_decay_period;
	real_vector2d m_texture_scale;
	real_point2d m_sprite_corner;
	real_vector2d m_sprite_size;
	rasterizer_vertex_world m_floating_vertices[4];
};
static_assert(sizeof(c_decal) == 0x130);

struct s_decal_message
{
	enum
	{
		k_max_per_frame = 256
	};

	enum e_type
	{
		_type_add_cluster_to_pvs = 0,
		_type_remove_cluster_from_pvs,

		k_max_type
	};

	e_type m_type;
	s_cluster_reference m_cluster_ref;
};

struct s_decal_message_queue :
	t_message_queue<s_decal_message, s_decal_message::k_max_per_frame>
{
};
static_assert(sizeof(s_decal_message_queue) == 0x824);

extern void __cdecl sub_6948C0(int32 a1);

