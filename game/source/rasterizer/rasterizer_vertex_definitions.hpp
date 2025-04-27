#pragma once

#include "cseries/cseries.hpp"

struct rasterizer_vertex_debug
{
	real_point3d position;
	uint32 color;
};
static_assert(sizeof(rasterizer_vertex_debug) == 0x10);

struct rasterizer_vertex_lightmap_uv
{
	real_vector2d texcoord;
};
static_assert(sizeof(rasterizer_vertex_lightmap_uv) == 0x8);

struct rasterizer_vertex_rigid
{
	real_point3d position;
	real_point2d texcoord;
	real_vector3d normal;
	real_vector3d tangent;
	real_vector3d binormal;
};
static_assert(sizeof(rasterizer_vertex_rigid) == 0x38);

struct rasterizer_vertex_screen
{
	real_point2d position;
	real_vector2d texcoord;
	uint32 color;
};
static_assert(sizeof(rasterizer_vertex_screen) == 0x14);

struct rasterizer_vertex_transparent
{
	real_point3d position;
	real_vector2d texcoord;
	uint32 color;
};
static_assert(sizeof(rasterizer_vertex_transparent) == 0x18);

struct rasterizer_vertex_world
{
	real_point3d position;
	real_point2d texcoord;
	real_vector3d normal;   //short normal[4];
	real_vector3d tangent;  //short tangent[4];
	real_vector3d binormal; //short binormal[4];
};
static_assert(sizeof(rasterizer_vertex_world) == 0x38);

