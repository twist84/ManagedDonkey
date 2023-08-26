#pragma once

struct collision_bsp;
struct large_collision_bsp;
struct c_bsp3d_reference
{
public:
	c_bsp3d_reference();

protected:
	collision_bsp* m_bsp;
	large_collision_bsp* m_large_bsp;
};

struct structure_bsp;
struct s_structure_bsp_resources;
struct c_collision_bsp_reference :
	public c_bsp3d_reference
{
public:
	c_collision_bsp_reference();
	c_collision_bsp_reference(structure_bsp const* bsp);

	void initialize(s_structure_bsp_resources const* bsp_resources);
	bool valid() const;
	bool is_small() const;
	collision_bsp const* get_small_bsp() const;
	large_collision_bsp const* get_large_bsp() const;
	long get_leaf_count() const;
	long get_bsp2d_reference_count() const;
	long get_bsp2d_node_count() const;
	long get_surface_count() const;
	long get_edge_count() const;
	long get_vertex_count() const;
};

