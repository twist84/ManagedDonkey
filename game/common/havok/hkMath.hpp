#pragma once

#include <xmmintrin.h>

struct hkHalf
{
	int16 m_value;
};

struct hkVector4
{
	__m128 m_quad;
};
static_assert(sizeof(hkVector4) == 0x10);

struct hkQuaternion
{
	hkVector4 vector;
};
static_assert(sizeof(hkQuaternion) == 0x10);

struct hkMatrix3
{
	hkVector4 forward;
	hkVector4 left;
	hkVector4 up;
};
static_assert(sizeof(hkMatrix3) == 0x30);

struct hkRotation
{
	hkMatrix3 matrix;
};
static_assert(sizeof(hkRotation) == 0x30);

struct hkTransform
{
	hkRotation rotation;
	hkVector4 translation;
};
static_assert(sizeof(hkTransform) == 0x40);

