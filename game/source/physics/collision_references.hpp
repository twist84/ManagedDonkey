#pragma once

#include "cseries/cseries.hpp"
#include "physics/collision_bsp_references.hpp"

class c_collision_edge_reference :
	public c_collision_bsp_reference
{
public:
	c_collision_edge_reference();
	c_collision_edge_reference(c_collision_bsp_reference bsp_reference, int32 index);

	int32 get_edge_index() const;
	int32 get_vertex_index(int32 index) const;
	int32 get_edge_index(int32 index) const;
	int32 get_surface_index(int32 index) const;

protected:
	int32 m_edge_index;
};

class c_collision_vertex_reference :
	public c_collision_bsp_reference
{
public:
	c_collision_vertex_reference();
	c_collision_vertex_reference(c_collision_bsp_reference bsp_reference, int32 index);

	int32 get_vertex_index() const;
	real_point3d* get_position() const;
	int32 get_first_edge_index() const;
	int32 get_sink() const;

protected:
	int32 m_vertex_index;
};

class c_collision_surface_reference :
	public c_collision_bsp_reference
{
public:
	c_collision_surface_reference();
	c_collision_surface_reference(c_collision_bsp_reference bsp_reference, int32 index);

	uns8 get_best_plane_calculation_vertex_index() const;
	int32 get_breakable_surface_set_index() const;
	int32 get_breakable_surface_index() const;
	int32 get_first_edge_index() const;
	uns8 get_flags() const;
	int32 get_material_index() const;
	const real_plane3d* get_plane(real_plane3d* plane) const;
	const real_plane3d* get_plane(bool* is_negated) const;
	int32 get_plane_index() const;
	bool is_plane_negated() const;

protected:
	int32 m_surface_index;
};
