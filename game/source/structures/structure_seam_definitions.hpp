#pragma once

#include "cseries/cseries.hpp"
#include "tag_files/tag_groups.hpp"

struct error_report_category;
struct s_structure_seam;
struct s_structure_seams
{
	static tag const k_group_tag = STRUCTURE_SEAMS_TAG;

	// This tag defines the geometry between seam connected structures bsp.
	long version;

	c_typed_tag_block<error_report_category> errors;
	c_typed_tag_block<s_structure_seam> seams;
};
static_assert(sizeof(s_structure_seams) == 0x1C);

enum e_error_report_types
{
	_error_report_type_silent = 0,
	_error_report_type_comment,
	_error_report_type_warning,
	_error_report_type_error,

	k_error_report_type_count
};

enum e_error_report_flags
{
	_error_report_rendered_bit = 0,
	_error_report_tangent_space_bit,
	_error_report_non_critical_bit,
	_error_report_lightmap_light_bit,
	_error_report_report_key_is_valid_bit,

	k_error_report_flags
};

struct error_report_category
{
	char name[k_tag_long_string_length];
	c_enum<e_error_report_types, short, _error_report_type_silent, k_error_report_type_count> report_type;
	c_flags<e_error_report_flags, word, k_error_report_flags> flags;
	word_flags runtime_generation_flags;

	// pad
	byte WVTP[0x2];

	long runtime_something;

	s_tag_block reports;
};
static_assert(sizeof(s_structure_seams) == 0x1C);

struct s_structure_seam_identifier
{
	dword part[4];
};
static_assert(sizeof(s_structure_seam_identifier) == 0x10);

struct s_structure_seam_original_vertex
{
	real_point3d original_vertex;
	long final_point_index;
};
static_assert(sizeof(s_structure_seam_original_vertex) == 0x10);

struct s_structure_seam_original_geometry
{
	c_typed_tag_block<s_structure_seam_original_vertex> original_vertices;
};
static_assert(sizeof(s_structure_seam_original_geometry) == sizeof(s_tag_block));

struct s_structure_seam_final_plane
{
	real_plane3d plane;
};
static_assert(sizeof(s_structure_seam_final_plane) == sizeof(real_plane3d));

struct s_structure_seam_final_point
{
	real_point3d point;
};
static_assert(sizeof(s_structure_seam_final_point) == sizeof(real_point3d));

struct s_structure_seam_final_triangle
{
	long final_plane;
	short final_points[3]; // s_structure_seam_final_point

	// PAD
	byte pad0[0x2];
};
static_assert(sizeof(s_structure_seam_final_triangle) == 0xC);

struct s_structure_seam_final_edge
{
	long final_plane;
	short final_points[2]; // s_structure_seam_final_point
};
static_assert(sizeof(s_structure_seam_final_edge) == 0x8);

struct s_structure_seam_final_geometry
{
	c_typed_tag_block<s_structure_seam_final_plane> planes;
	c_typed_tag_block<s_structure_seam_final_point> points;
	c_typed_tag_block<s_structure_seam_final_triangle> triangles;
	c_typed_tag_block<s_structure_seam_final_edge> edges;
};
static_assert(sizeof(s_structure_seam_final_geometry) == sizeof(s_tag_block) * 4);

struct s_structure_seam
{
	s_structure_seam_identifier identifier;
	s_structure_seam_original_geometry original;
	s_structure_seam_final_geometry final;
};
static_assert(sizeof(s_structure_seam) == 0x4C);

