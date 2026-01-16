#pragma once

#include "cseries/cseries.hpp"

struct rasterizer_vertex_debug
{
	real_point3d position;
	uns32 color;
};
COMPILE_ASSERT(sizeof(rasterizer_vertex_debug) == 0x10);

struct rasterizer_vertex_lightmap_uv
{
	real_vector2d texcoord;
};
COMPILE_ASSERT(sizeof(rasterizer_vertex_lightmap_uv) == 0x8);

struct rasterizer_vertex_rigid
{
	real_point3d position;
	real_point2d texcoord;
	real_vector3d normal;
	real_vector3d tangent;
	real_vector3d binormal;
};
COMPILE_ASSERT(sizeof(rasterizer_vertex_rigid) == 0x38);

struct rasterizer_vertex_screen
{
	real_point2d position;
	real_vector2d texcoord;
	uns32 color;
};
COMPILE_ASSERT(sizeof(rasterizer_vertex_screen) == 0x14);

struct rasterizer_vertex_transparent
{
	real_point3d position;
	real_vector2d texcoord;
	uns32 color;
};
COMPILE_ASSERT(sizeof(rasterizer_vertex_transparent) == 0x18);

struct rasterizer_vertex_world
{
	real_point3d position;
	real_point2d texcoord;
	real_vector3d normal;   //int16 normal[4];
	real_vector3d tangent;  //int16 tangent[4];
	real_vector3d binormal; //int16 binormal[4];
};
COMPILE_ASSERT(sizeof(rasterizer_vertex_world) == 0x38);

