#pragma once

#define assert_valid_real_normal3d(NORMAL) \
{ \
	if (!valid_real_normal3d(NORMAL)) \
	{ \
		ASSERT2(c_string_builder("%s: assert_valid_real_normal3d(%f, %f, %f)", \
			#NORMAL, \
			##NORMAL->i, \
			##NORMAL->j, \
			##NORMAL->k \
		).get_string()); \
	} \
}

#define assert_valid_real_vector3d_axes3(FORWARD, LEFT, UP) \
{ \
	if (!valid_real_vector3d_axes3(FORWARD, LEFT, UP)) \
	{ \
		ASSERT2(c_string_builder("%s, %s, %s: assert_valid_real_vector3d_axes3(%f, %f, %f / %f, %f, %f / %f, %f, %f)", \
			#FORWARD, \
			#LEFT, \
			#UP, \
			##FORWARD->i, \
			##FORWARD->j, \
			##FORWARD->k, \
			##LEFT->i, \
			##LEFT->j, \
			##LEFT->k, \
			##UP->i, \
			##UP->j, \
			##UP->k \
		).get_string()); \
	} \
}

#define PI 3.14159265359f
#define TWO_PI real(PI * 2) // 6.28318530718
#define RAD real(180 / PI)  // 57.2957795131
#define DEG real(PI / 180)  // 0.01745329251

typedef float real;
static_assert(sizeof(real) == sizeof(float));

typedef float real_fraction;
static_assert(sizeof(real_fraction) == sizeof(real));

struct real_decibel
{
	real db;
};
static_assert(sizeof(real_decibel) == sizeof(real));

#define DEFINE_REAL_CONSTANT(NAME) real const k##NAME = NAME

#define _pi PI
#define _test_real_epsilon  0.001f
#define _real_epsilon       0.0001f
#define _real_tiny_epsilon  0.000001f
#define _real_max           3.4028235e38f
#define _real_min          -3.4028235e38f

DEFINE_REAL_CONSTANT(_test_real_epsilon);
DEFINE_REAL_CONSTANT(_real_epsilon);
DEFINE_REAL_CONSTANT(_real_tiny_epsilon);
DEFINE_REAL_CONSTANT(_real_max);
DEFINE_REAL_CONSTANT(_real_min);

real const k_distance_max = 50000.0f;
real const k_horizontal_field_of_view_min = DEG * 1.0f;
real const k_horizontal_field_of_view_max = DEG * 150.0f;
real const k_full_circle = TWO_PI;
real const k_half_circle = k_full_circle / 2;
real const k_quarter_circle = k_full_circle / 4;

enum
{
	_x = 0,
	_y,
	_z,
	_w,

	k_faces_per_cube_count = 6,
	k_vertices_per_cube_count = 8,
	k_edges_per_cube_count = 12,
};

union real_bounds
{
	struct
	{
		real lower;
		real upper;
	};
	real n[2];
};
static_assert(sizeof(real_bounds) == sizeof(real) * 2);

union real_point2d
{
	struct
	{
		real x;
		real y;
	};
	struct
	{
		real u;
		real v;
	};
	real n[2];
};
static_assert(sizeof(real_point2d) == sizeof(real) * 2);

union real_point3d
{
	struct
	{
		real x;
		real y;
		real z;
	};
	struct
	{
		real u;
		real v;
		real w;
	};
	real n[3];
};
static_assert(sizeof(real_point3d) == sizeof(real) * 3);

union real_point4d
{
	struct
	{
		real x;
		real y;
		real z;
		real w;
	};
	real n[4];
};
static_assert(sizeof(real_point4d) == sizeof(real) * 4);

union real_vector2d
{
	struct
	{
		real i;
		real j;
	};
	real n[2];
};
static_assert(sizeof(real_vector2d) == sizeof(real) * 2);

union real_vector3d
{
	struct
	{
		real i;
		real j;
		real k;
	};
	real n[3];
};
static_assert(sizeof(real_vector3d) == sizeof(real) * 3);

union real_vector4d
{
	struct
	{
		real i;
		real j;
		real k;
		real l;
	};
	real n[4];
};
static_assert(sizeof(real_vector4d) == sizeof(real) * 4);

struct real_plane2d
{
	real_vector2d n; // normal
	real d; // distance
};
static_assert(sizeof(real_plane2d) == 0xC);

struct real_plane3d
{
	real_vector3d n; // normal
	real d; // distance
};
static_assert(sizeof(real_plane3d) == 0x10);

union real_rgb_color
{
	real n[3];

	struct
	{
		real red;
		real green;
		real blue;
	};
};
static_assert(sizeof(real_rgb_color) == sizeof(real) * 3);

union real_argb_color
{
	real n[4];

	struct
	{
		real alpha;
		real_rgb_color rgb;
	};

	struct
	{
		unsigned char gap0[4];
		real red;
		real green;
		real blue;
	};
};
static_assert(sizeof(real_argb_color) == sizeof(real) * 4);

union real_hsv_color
{
	real n[3];

	struct
	{
		real hue;
		real saturation;
		real value;
	};
};
static_assert(sizeof(real_hsv_color) == sizeof(real) * 3);

union real_ahsv_color
{
	real n[4];

	struct
	{
		real alpha;
		real_hsv_color hsv;
	};

	struct
	{
		unsigned char gap0[4];
		real hue;
		real saturation;
		real value;
	};
};
static_assert(sizeof(real_ahsv_color) == sizeof(real) * 4);

struct real_linear_rgb_color
{
	real red;
	real green;
	real blue;
};
static_assert(sizeof(real_linear_rgb_color) == sizeof(real) * 3);

typedef float angle;
static_assert(sizeof(angle) == sizeof(float));

union angle_bounds
{
	angle n[2];

	struct
	{
		angle lower;
		angle upper;
	};
};
static_assert(sizeof(angle_bounds) == sizeof(angle) * 2);

union real_euler_angles2d
{
	angle n[2];

	struct
	{
		angle yaw;
		angle pitch;
	};
};
static_assert(sizeof(real_euler_angles2d) == sizeof(angle) * 2);

union real_euler_angles3d
{
	angle n[3];

	struct
	{
		angle yaw;
		angle pitch;
		angle roll;
	};
};
static_assert(sizeof(real_euler_angles3d) == sizeof(angle) * 3);

struct real_matrix3x3
{
	union
	{
		real n[3][3];
		real_vector3d basis[3];

		struct
		{
			real_vector3d forward;
			real_vector3d left;
			real_vector3d up;
		};
	};
};
static_assert(sizeof(real_matrix3x3) == sizeof(real_vector3d) * 3);

struct real_matrix4x3
{
	real scale;

	union
	{
		real n[4][3];

		struct
		{
			real_matrix3x3 matrix3x3;
			real_point3d origin;
		};

		struct
		{
			real_vector3d basis[3];
			real_point3d position;
		};

		struct
		{
			real_vector3d forward;
			real_vector3d left;
			real_vector3d up;
		};
	};
};
static_assert(sizeof(real_matrix4x3) == 0x34);

union real_rectangle2d
{
	real n[4];
	real m[2][2];

	struct
	{
		real x0;
		real x1;
		real y0;
		real y1;
	};
};
static_assert(sizeof(real_rectangle2d) == sizeof(real) * 4);

union real_rectangle3d
{
	real n[6];
	real m[3][2];

	struct
	{
		real x0;
		real x1;
		real y0;
		real y1;
		real z0;
		real z1;
	};
};
static_assert(sizeof(real_rectangle3d) == sizeof(real) * 6);

struct real_quaternion
{
	real_vector3d v;
	real w;
};
static_assert(sizeof(real_quaternion) == 0x10);

struct real_orientation
{
	real_quaternion rotation;
	real_point3d translation;
	real scale;
};
static_assert(sizeof(real_orientation) == 0x20);

extern real_point2d const* const& global_origin2d;
extern real_point2d const* const& global_x_axis2d;
extern real_point2d const* const& global_y_axis2d;
extern real_point2d const* const& global_negative_x_axis2d;
extern real_point2d const* const& global_negative_y_axis2d;
extern real_vector2d const* const& global_zero_vector2d;
extern real_vector2d const* const& global_forward2d;
extern real_vector2d const* const& global_left2d;
extern real_vector2d const* const& global_backward2d;
extern real_vector2d const* const& global_right2d;
extern real_point3d const* const& global_origin3d;
extern real_vector3d const* const& global_x_axis3d;
extern real_vector3d const* const& global_y_axis3d;
extern real_vector3d const* const& global_z_axis3d;
extern real_vector3d const* const& global_negative_x_axis3d;
extern real_vector3d const* const& global_negative_y_axis3d;
extern real_vector3d const* const& global_negative_z_axis3d;
extern real_vector3d const* const& global_zero_vector3d;
extern real_vector3d const* const& global_forward3d;
extern real_vector3d const* const& global_left3d;
extern real_vector3d const* const& global_up3d;
extern real_vector3d const* const& global_backward3d;
extern real_vector3d const* const& global_right3d;
extern real_vector3d const* const& global_down3d;
extern real_euler_angles2d const* const& global_zero_angles2d;
extern real_euler_angles3d const* const& global_zero_angles3d;
extern real_vector4d const* const& global_zero_vector4d;
extern real_quaternion const* const& global_identity_quaternion;
extern real_matrix3x3 const* const& global_identity3x3;
extern real_matrix4x3 const* const& global_identity4x3;
extern real_matrix4x3 const* const& global_negative_identity4x3;
extern real_orientation const* const& global_identity_orientation;
extern real_rectangle2d const* const& global_null_rectangle2d;
extern real_rectangle3d const* const& global_null_rectangle3d;
extern real_rectangle2d const* const& global_zero_rectangle2d;
extern real_rectangle3d const* const& global_zero_rectangle3d;

extern bool __cdecl valid_real_vector2d(real_vector2d const* vector);
extern bool __cdecl valid_real_vector4d(real_vector4d const* vector);
extern bool __cdecl valid_real_sine_cosine(real sine, real cosine);
extern bool __cdecl valid_real_quaternion(real_quaternion const* quaternion);
extern bool __cdecl valid_real_normal2d(real_vector2d const* normal);
extern bool __cdecl valid_real_plane2d(real_plane2d const* plane);
extern bool __cdecl valid_real_plane3d(real_plane3d const* plane);
extern bool __cdecl valid_real_vector3d_right_handed_axes3(real_vector3d const* forward, real_vector3d const* left, real_vector3d const* up);
extern bool __cdecl valid_real_matrix4x3(real_matrix4x3 const* matrix);
extern bool __cdecl valid_real_euler_angles3d(real_euler_angles3d const* angles);

extern real __cdecl interpolate_linear(real start_value, real end_value, real interpolation_factor);
extern bool __cdecl valid_real(real const& value);
extern bool __cdecl valid_real_euler_angles2d(real_euler_angles2d const* angles);
extern bool __cdecl valid_real_normal3d(real_vector3d const* normal);
extern bool __cdecl valid_real_point3d(real_point3d const* point);
extern bool __cdecl valid_real_point2d(real_point2d const* point);
extern bool __cdecl valid_real_vector3d(real_vector3d const* vector);
extern bool __cdecl valid_real_vector3d_axes2(real_vector3d const* forward, real_vector3d const* up);
extern bool __cdecl valid_realcmp(real a, real b);
extern real __cdecl angle_between_vectors3d(real_vector3d const* a, real_vector3d const* b);
extern real_vector3d* __cdecl generate_up_vector3d(real_vector3d const* forward, real_vector3d* up);
extern void __cdecl real_math_dispose();
extern void __cdecl real_math_initialize();
extern void __cdecl real_math_reset_precision();
extern real_euler_angles2d* __cdecl euler_angles2d_from_vector3d(real_euler_angles2d* facing, real_vector3d const* forward);
extern bool __cdecl valid_real_vector3d_axes3(real_vector3d const* forward, real_vector3d const* left, real_vector3d const* up);
extern real_matrix3x3* __cdecl matrix3x3_rotation_from_quaternion(real_matrix3x3* matrix, real_quaternion const* quaternion);
extern void __cdecl matrix4x3_from_point_and_quaternion(real_matrix4x3* matrix, real_point3d const* point, real_quaternion const* quaternion);
extern bool __cdecl valid_polygon2d(long point_count, real_point2d const* const points);
extern real_vector3d* __cdecl cross_product3d(real_vector3d const* a, real_vector3d const* b, real_vector3d* out);
extern real __cdecl dot_product3d(real_vector3d const* a, real_vector3d const* b);
extern real __cdecl triple_product3d(real_vector3d const* a, real_vector3d const* b, real_vector3d const* c);
extern real_vector3d* __cdecl vector_from_points3d(real_point3d const* a, real_point3d const* b, real_vector3d* out);
extern real_point3d* __cdecl point_from_line3d(real_point3d const* in_point, real_vector3d const* in_vector, real scale, real_point3d* out_point);
extern real __cdecl magnitude_squared3d(real_vector3d const* a);
extern real __cdecl distance_squared3d(real_point3d const* a, real_point3d const* b);
extern real __cdecl distance3d(real_point3d const* a, real_point3d const* b);
extern real __cdecl square_root(real value);
extern real __cdecl magnitude3d(real_vector3d const* vector);
extern real __cdecl arctangent(real y, real x);
extern real_vector3d* __cdecl perpendicular3d(real_vector3d const* vector, real_vector3d* out_vector);
extern real __cdecl normalize3d(real_vector3d* vector);
extern real_vector2d* __cdecl rotate_vector2d(real_vector2d const* vector, real a2, real a3, real_vector2d* out_vector);
extern real __cdecl magnitude_squared2d(real_vector2d const* a);
extern real_vector3d* __cdecl scale_vector3d(real_vector3d const* in_vector, real scale, real_vector3d* out_vector);
extern real_vector3d* __cdecl add_vectors3d(real_vector3d const* in_vector_a, real_vector3d const* in_vector_b, real_vector3d* out_vector);
extern real_vector3d* __cdecl negate_vector3d(real_vector3d const* in_vector, real_vector3d* out_vector);
extern real_point2d* __cdecl set_real_point2d(real_point2d* point, real x, real y);
extern real_point3d* __cdecl set_real_point3d(real_point3d* point, real x, real y, real z);
extern real_point2d* __cdecl point_from_line2d(real_point2d const* point, real_vector2d const* vector, real scale, real_point2d* out_point);
extern real_vector2d* __cdecl vector_from_points2d(real_point2d const* point0, real_point2d const* point1, real_vector2d* out_vector);
extern real_point3d* __cdecl project_point2d(real_point2d const* point, real_plane3d const* plane, short projection, bool projection_sign, real_point3d* out_point);
extern real __cdecl plane3d_distance_to_point(real_plane3d const* plane, real_point3d const* point);
extern long __cdecl rectangle3d_build_vertices(real_rectangle3d const* bounds, long maximum_vertex_count, real_point3d* const vertices);
extern long __cdecl rectangle3d_build_faces(real_rectangle3d const* bounds, long maximum_face_count, real_point3d(* const faces)[4]);
extern long __cdecl rectangle3d_build_edges(real_rectangle3d const* bounds, long maximum_edge_count, real_point3d(* const edges)[2]);
extern real_rectangle2d* __cdecl set_real_rectangle2d(real_rectangle2d* rect, real x_lower, real x_upper, real y_lower, real y_upper);
extern real_rectangle3d* __cdecl set_real_rectangle3d(real_rectangle3d* rect, real x_lower, real x_upper, real y_lower, real y_upper, real z_lower, real z_upper);
extern void __cdecl quaternion_transform_point(real_quaternion const* in_quaternion, real_point3d const* in_point, real_point3d* point);
extern real_rectangle3d* __cdecl real_rectangle3d_enclose_point(real_rectangle3d* enclosed_rect, real_point3d const* point);
extern real_rectangle3d* __cdecl real_rectangle3d_enclose_points(real_rectangle3d* enclosed_rect, long point_count, real_point3d const* points);
extern real_rectangle3d* __cdecl real_rectangle3d_enclose_rectangle(real_rectangle3d* enclosed_rect, real_rectangle3d const* rect);
extern real_vector3d* __cdecl vector3d_from_angle(real_vector3d* vector, real angle);
extern real_vector3d* __cdecl vector3d_from_euler_angles2d(real_vector3d* vector, real_euler_angles2d const* angles);
extern real_vector2d* __cdecl set_real_vector2d(real_vector2d* vector, real i, real j);
extern real_vector3d* __cdecl set_real_vector3d(real_vector3d* vector, real i, real j, real k);
extern real_vector4d* __cdecl set_real_vector4d(real_vector4d* result, real i, real j, real k, real l);

extern bool point_intersects_rectangle2d(real_point2d const* point, real_rectangle2d const* rect);
extern bool point_in_sphere(real_point3d const* point, real_point3d const* center, real radius);

