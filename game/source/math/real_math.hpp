#pragma once

#define assert_valid_real_normal3d(NORMAL) \
{ \
	if (!valid_real_normal3d(NORMAL)) \
	{ \
		VASSERT(c_string_builder("%s: assert_valid_real_normal3d(%f, %f, %f)", \
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
		VASSERT(c_string_builder("%s, %s, %s: assert_valid_real_vector3d_axes3(%f, %f, %f / %f, %f, %f / %f, %f, %f)", \
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
#define TWO_PI real32(PI * 2) // 6.28318530718
#define RAD real32(180 / PI)  // 57.2957795131
#define DEG real32(PI / 180)  // 0.01745329251

struct real_decibel
{
	real32 db;
};
static_assert(sizeof(real_decibel) == sizeof(real32));

#define DEFINE_REAL_CONSTANT(NAME) real32 const k##NAME = NAME

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

real32 const k_distance_max = 50000.0f;
real32 const k_horizontal_field_of_view_min = DEG * 1.0f;
real32 const k_horizontal_field_of_view_max = DEG * 150.0f;
real32 const k_full_circle = TWO_PI;
real32 const k_half_circle = k_full_circle / 2;
real32 const k_quarter_circle = k_full_circle / 4;

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
		real32 lower;
		real32 upper;
	};
	real32 n[2];
};
static_assert(sizeof(real_bounds) == sizeof(real32) * 2);

union real_point2d
{
	struct
	{
		real32 x;
		real32 y;
	};
	struct
	{
		real32 u;
		real32 v;
	};
	real32 n[2];
};
static_assert(sizeof(real_point2d) == sizeof(real32) * 2);

union real_point3d
{
	struct
	{
		real32 x;
		real32 y;
		real32 z;
	};
	struct
	{
		real32 u;
		real32 v;
		real32 w;
	};
	real32 n[3];
};
static_assert(sizeof(real_point3d) == sizeof(real32) * 3);

union real_point4d
{
	struct
	{
		real32 x;
		real32 y;
		real32 z;
		real32 w;
	};
	real32 n[4];
};
static_assert(sizeof(real_point4d) == sizeof(real32) * 4);

union real_vector2d
{
	struct
	{
		real32 i;
		real32 j;
	};
	real32 n[2];
};
static_assert(sizeof(real_vector2d) == sizeof(real32) * 2);

union real_vector3d
{
	struct
	{
		real32 i;
		real32 j;
		real32 k;
	};
	real32 n[3];
};
static_assert(sizeof(real_vector3d) == sizeof(real32) * 3);

union real_vector4d
{
	struct
	{
		real32 i;
		real32 j;
		real32 k;
		real32 l;
	};
	real32 n[4];
};
static_assert(sizeof(real_vector4d) == sizeof(real32) * 4);

struct real_plane2d
{
	real_vector2d n; // normal
	real32 d; // distance
};
static_assert(sizeof(real_plane2d) == 0xC);

struct real_plane3d
{
	real_vector3d n; // normal
	real32 d; // distance
};
static_assert(sizeof(real_plane3d) == 0x10);

union real_rgb_color
{
	real32 n[3];

	struct
	{
		real32 red;
		real32 green;
		real32 blue;
	};
};
static_assert(sizeof(real_rgb_color) == sizeof(real32) * 3);

union real_argb_color
{
	real32 n[4];

	struct
	{
		real32 alpha;
		real_rgb_color rgb;
	};

	struct
	{
		byte gap0[sizeof(real32)];
		real32 red;
		real32 green;
		real32 blue;
	};
};
static_assert(sizeof(real_argb_color) == sizeof(real32) * 4);

union real_hsv_color
{
	real32 n[3];

	struct
	{
		real32 hue;
		real32 saturation;
		real32 value;
	};
};
static_assert(sizeof(real_hsv_color) == sizeof(real32) * 3);

union real_ahsv_color
{
	real32 n[4];

	struct
	{
		real32 alpha;
		real_hsv_color hsv;
	};

	struct
	{
		byte gap0[sizeof(real32)];
		real32 hue;
		real32 saturation;
		real32 value;
	};
};
static_assert(sizeof(real_ahsv_color) == sizeof(real32) * 4);

struct real_linear_rgb_color
{
	real32 red;
	real32 green;
	real32 blue;
};
static_assert(sizeof(real_linear_rgb_color) == sizeof(real32) * 3);

union angle_bounds
{
	real32 n[2];

	struct
	{
		real32 lower;
		real32 upper;
	};
};
static_assert(sizeof(angle_bounds) == sizeof(real32) * 2);

union real_euler_angles2d
{
	real32 n[2];

	struct
	{
		real32 yaw;
		real32 pitch;
	};
};
static_assert(sizeof(real_euler_angles2d) == sizeof(real32) * 2);

union real_euler_angles3d
{
	real32 n[3];

	struct
	{
		real32 yaw;
		real32 pitch;
		real32 roll;
	};
};
static_assert(sizeof(real_euler_angles3d) == sizeof(real32) * 3);

struct real_matrix3x3
{
	union
	{
		real32 n[3][3];
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
	real32 scale;

	union
	{
		real32 n[4][3];

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
	real32 n[4];
	real32 m[2][2];

	struct
	{
		real32 x0;
		real32 x1;
		real32 y0;
		real32 y1;
	};
};
static_assert(sizeof(real_rectangle2d) == sizeof(real32) * 4);

union real_rectangle3d
{
	real32 n[6];
	real32 m[3][2];

	struct
	{
		real32 x0;
		real32 x1;
		real32 y0;
		real32 y1;
		real32 z0;
		real32 z1;
	};
};
static_assert(sizeof(real_rectangle3d) == sizeof(real32) * 6);

struct real_quaternion
{
	real_vector3d v;
	real32 w;
};
static_assert(sizeof(real_quaternion) == 0x10);

struct real_orientation
{
	real_quaternion rotation;
	real_point3d translation;
	real32 scale;
};
static_assert(sizeof(real_orientation) == 0x20);

extern const real_point2d* const& global_origin2d;
extern const real_point2d* const& global_x_axis2d;
extern const real_point2d* const& global_y_axis2d;
extern const real_point2d* const& global_negative_x_axis2d;
extern const real_point2d* const& global_negative_y_axis2d;
extern const real_vector2d* const& global_zero_vector2d;
extern const real_vector2d* const& global_forward2d;
extern const real_vector2d* const& global_left2d;
extern const real_vector2d* const& global_backward2d;
extern const real_vector2d* const& global_right2d;
extern const real_point3d* const& global_origin3d;
extern const real_vector3d* const& global_x_axis3d;
extern const real_vector3d* const& global_y_axis3d;
extern const real_vector3d* const& global_z_axis3d;
extern const real_vector3d* const& global_negative_x_axis3d;
extern const real_vector3d* const& global_negative_y_axis3d;
extern const real_vector3d* const& global_negative_z_axis3d;
extern const real_vector3d* const& global_zero_vector3d;
extern const real_vector3d* const& global_forward3d;
extern const real_vector3d* const& global_left3d;
extern const real_vector3d* const& global_up3d;
extern const real_vector3d* const& global_backward3d;
extern const real_vector3d* const& global_right3d;
extern const real_vector3d* const& global_down3d;
extern const real_euler_angles2d* const& global_zero_angles2d;
extern const real_euler_angles3d* const& global_zero_angles3d;
extern const real_vector4d* const& global_zero_vector4d;
extern const real_quaternion* const& global_identity_quaternion;
extern const real_matrix3x3* const& global_identity3x3;
extern const real_matrix4x3* const& global_identity4x3;
extern const real_matrix4x3* const& global_negative_identity4x3;
extern const real_orientation* const& global_identity_orientation;
extern const real_rectangle2d* const& global_null_rectangle2d;
extern const real_rectangle3d* const& global_null_rectangle3d;
extern const real_rectangle2d* const& global_zero_rectangle2d;
extern const real_rectangle3d* const& global_zero_rectangle3d;

extern real32 __cdecl real_max(real32 a, real32 b);
extern real32 __cdecl real_min(real32 a, real32 b);
extern real32 __cdecl real_pin(real32 value, real32 min_value, real32 max_value);
extern void __cdecl interpolate_scalar(real32* current, real32 desired, real32 maximum_speed);
extern bool __cdecl valid_real_vector2d(const real_vector2d* v);
extern bool __cdecl valid_real_vector4d(const real_vector4d* v);
extern bool __cdecl valid_real_sine_cosine(real32 sine, real32 cosine);
extern bool __cdecl valid_real_quaternion(const real_quaternion* quaternion);
extern bool __cdecl valid_real_normal2d(const real_vector2d* normal);
extern bool __cdecl valid_real_plane2d(const real_plane2d* plane);
extern bool __cdecl valid_real_plane3d(const real_plane3d* plane);
extern bool __cdecl valid_real_vector3d_right_handed_axes3(const real_vector3d* forward, const real_vector3d* left, const real_vector3d* up);
extern bool __cdecl valid_real_matrix4x3(const real_matrix4x3* matrix);
extern bool __cdecl valid_real_euler_angles3d(const real_euler_angles3d* angles);
extern real32 __cdecl interpolate_linear(real32 start_value, real32 end_value, real32 interpolation_factor);
extern bool __cdecl valid_real(const real32& value);
extern bool __cdecl valid_real_euler_angles2d(const real_euler_angles2d* angles);
extern bool __cdecl valid_real_normal3d(const real_vector3d* normal);
extern bool __cdecl valid_real_point3d(const real_point3d* point);
extern bool __cdecl valid_real_point2d(const real_point2d* point);
extern bool __cdecl valid_real_vector3d(const real_vector3d* vector);
extern bool __cdecl valid_real_vector3d_axes2(const real_vector3d* forward, const real_vector3d* up);
extern bool __cdecl valid_realcmp(real32 a, real32 b);
extern real32 __cdecl angle_between_vectors3d(const real_vector3d* a, const real_vector3d* b);
extern real_euler_angles2d* __cdecl euler_angles2d_from_vector3d(real_euler_angles2d* facing, const real_vector3d* forward);
extern real_vector3d* __cdecl generate_up_vector3d(const real_vector3d* forward, real_vector3d* up);
extern real_point3d* __cdecl project_point2d(const real_point2d* p2d, const real_plane3d* plane, int16 projection, bool sign, real_point3d* p3d);
extern void __cdecl quaternion_transform_point(const real_quaternion* q, const real_point3d* p, real_point3d* result);
extern void __cdecl real_math_dispose();
extern void __cdecl real_math_initialize();
extern void __cdecl real_math_reset_precision();
extern void __cdecl real_rectangle2d_clamp_bounds(real_rectangle2d* clampee, const real_rectangle2d* clamper);
extern bool __cdecl real_rectangle2d_compute_intersection(const real_rectangle2d* a, const real_rectangle2d* b, real_rectangle2d* result);
extern real_rectangle3d* __cdecl real_rectangle3d_enclose_point(real_rectangle3d* bounds, const real_point3d* point);
extern real_rectangle3d* __cdecl real_rectangle3d_enclose_points(real_rectangle3d* bounds, int32 point_count, const real_point3d* points);
extern real_rectangle3d* __cdecl real_rectangle3d_enclose_rectangle(real_rectangle3d* bounds, const real_rectangle3d* rectangle);
extern int32 __cdecl rectangle3d_build_edges(const real_rectangle3d* bounds, int32 maximum_edge_count, real_point3d(* const edges)[2]);
extern int32 __cdecl rectangle3d_build_faces(const real_rectangle3d* bounds, int32 maximum_face_count, real_point3d(* const faces)[4]);
extern int32 __cdecl rectangle3d_build_vertices(const real_rectangle3d* bounds, int32 maximum_vertex_count, real_point3d* const vertices);
extern real_vector3d* __cdecl vector3d_from_angle(real_vector3d* vector, real32 angle);
extern real_vector3d* __cdecl vector3d_from_euler_angles2d(real_vector3d* vector, const real_euler_angles2d* angles);
extern bool __cdecl valid_real_vector3d_axes3(const real_vector3d* forward, const real_vector3d* left, const real_vector3d* up);
extern bool __cdecl valid_polygon2d(int32 point_count, const real_point2d* const points);
extern real_vector3d* __cdecl add_vectors3d(const real_vector3d* a, const real_vector3d* b, real_vector3d* result);
extern real32 __cdecl arctangent(real32 y, real32 x);
extern real_vector3d* __cdecl cross_product3d(const real_vector3d* a, const real_vector3d* b, real_vector3d* result);
extern real32 __cdecl distance_squared2d(const real_point2d* a, const real_point2d* b);
extern real32 __cdecl distance_squared3d(const real_point3d* a, const real_point3d* b);
extern real32 __cdecl distance2d(const real_point2d* a, const real_point2d* b);
extern real32 __cdecl distance3d(const real_point3d* a, const real_point3d* b);
extern real32 __cdecl dot_product2d(const real_vector2d* a, const real_vector2d* b);
extern real32 __cdecl dot_product3d(const real_vector3d* a, const real_vector3d* b);
extern real32 __cdecl dot_product4d(const real_vector4d* a, const real_vector4d* b);
extern real32 __cdecl magnitude_squared2d(const real_vector2d* v);
extern real32 __cdecl magnitude_squared3d(const real_vector3d* v);
extern real32 __cdecl magnitude2d(const real_vector2d* v);
extern real32 __cdecl magnitude3d(const real_vector3d* v);
extern real_vector3d* __cdecl negate_vector3d(const real_vector3d* a, real_vector3d* result);
extern real_rectangle2d* __cdecl offset_real_rectangle2d(real_rectangle2d* rectangle, real32 dx, real32 dy);
extern real32 __cdecl normalize2d(real_vector2d* v);
extern real32 __cdecl normalize3d(real_vector3d* v);
extern real_vector2d* __cdecl perpendicular2d(const real_vector2d* a, real_vector2d* result);
extern real_vector3d* __cdecl perpendicular3d(const real_vector3d* a, real_vector3d* result);
extern real32 __cdecl plane3d_distance_to_point(const real_plane3d* plane, const real_point3d* point);
extern real_point2d* __cdecl point_from_line2d(const real_point2d* p, const real_vector2d* v, real32 t, real_point2d* result);
extern real_point3d* __cdecl point_from_line3d(const real_point3d* p, const real_vector3d* v, real32 t, real_point3d* result);
extern bool __cdecl point_in_sphere(const real_point3d* point, const real_point3d* center, real32 radius);
extern bool __cdecl point_intersects_rectangle2d(const real_point2d* p, const real_rectangle2d* bounds);
extern real_vector2d* __cdecl rotate_vector2d(const real_vector2d* v, real32 sine, real32 cosine, real_vector2d* result);
extern real_vector2d* __cdecl scale_vector2d(const real_vector2d* a, real32 c, real_vector2d* result);
extern real_vector3d* __cdecl scale_vector3d(const real_vector3d* a, real32 c, real_vector3d* result);
extern real_point2d* __cdecl set_real_point2d(real_point2d* p, real32 x, real32 y);
extern real_point3d* __cdecl set_real_point3d(real_point3d* p, real32 x, real32 y, real32 z);
extern real_rectangle2d* __cdecl set_real_rectangle2d(real_rectangle2d* bounds, real32 x0, real32 x1, real32 y0, real32 y1);
extern real_rectangle3d* __cdecl set_real_rectangle3d(real_rectangle3d* bounds, real32 x0, real32 x1, real32 y0, real32 y1, real32 z0, real32 z1);
extern real_vector2d* __cdecl set_real_vector2d(real_vector2d* vector, real32 i, real32 j);
extern real_vector3d* __cdecl set_real_vector3d(real_vector3d* vector, real32 i, real32 j, real32 k);
extern real_vector4d* __cdecl set_real_vector4d(real_vector4d* result, real32 i, real32 j, real32 k, real32 l);
extern real32 __cdecl square_root(real32 x);
extern real32 __cdecl triple_product3d(const real_vector3d* a, const real_vector3d* b, const real_vector3d* n);
extern real_vector2d* __cdecl vector_from_points2d(const real_point2d* a, const real_point2d* b, real_vector2d* result);
extern real_vector3d* __cdecl vector_from_points3d(const real_point3d* a, const real_point3d* b, real_vector3d* result);
extern real_vector4d* __cdecl real_rgb_color_to_real_vector4d(real_rgb_color* color, real32 alpha, real_vector4d* result);

