#pragma once

struct collision_bsp;
struct large_collision_bsp;
class c_bsp3d_reference
{
public:
	c_bsp3d_reference();

protected:
	collision_bsp* m_bsp;
	large_collision_bsp* m_large_bsp;
};

struct structure_bsp;
struct s_structure_bsp_resources;
class c_collision_bsp_reference :
	public c_bsp3d_reference
{
public:
	c_collision_bsp_reference();
	c_collision_bsp_reference(const structure_bsp* bsp);

	void initialize(const s_structure_bsp_resources* bsp_resources);
	bool valid() const;
	bool is_small() const;
	const collision_bsp* get_small_bsp() const;
	const large_collision_bsp* get_large_bsp() const;
	int32 get_leaf_count() const;
	int32 get_bsp2d_reference_count() const;
	int32 get_bsp2d_node_count() const;
	int32 get_surface_count() const;
	int32 get_edge_count() const;
	int32 get_vertex_count() const;
};

