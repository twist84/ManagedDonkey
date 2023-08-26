#pragma once

#include "cseries/cseries.hpp"
#include "physics/collision_bsp_references.hpp"

struct c_collision_edge_reference :
	public c_collision_bsp_reference
{
public:
	c_collision_edge_reference();
	c_collision_edge_reference(c_collision_bsp_reference bsp_reference, long index);

	long get_edge_index() const;
	long get_vertex_index(long index) const;
	long get_edge_index(long index) const;
	long get_surface_index(long index) const;

protected:
	long m_edge_index;
};

struct c_collision_vertex_reference :
	public c_collision_bsp_reference
{
public:
	c_collision_vertex_reference();
	c_collision_vertex_reference(c_collision_bsp_reference bsp_reference, long index);

	long get_vertex_index() const;
	real_point3d* get_position() const;
	long get_first_edge_index() const;
	long get_sink() const;

protected:
	long m_vertex_index;
};

struct c_collision_surface_reference :
	public c_collision_bsp_reference
{
public:
	c_collision_surface_reference();

	c_collision_surface_reference(c_collision_bsp_reference bsp_reference, long index);
	byte_flags get_flags() const;
	plane3d const* get_plane(plane3d* plane) const;

protected:
	long m_surface_index;
};
