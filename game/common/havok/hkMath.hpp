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
COMPILE_ASSERT(sizeof(hkVector4) == 0x10);

struct hkQuaternion
{
	hkVector4 vector;
};
COMPILE_ASSERT(sizeof(hkQuaternion) == 0x10);

struct hkMatrix3
{
	hkVector4 forward;
	hkVector4 left;
	hkVector4 up;
};
COMPILE_ASSERT(sizeof(hkMatrix3) == 0x30);

struct hkRotation
{
	hkMatrix3 matrix;
};
COMPILE_ASSERT(sizeof(hkRotation) == 0x30);

struct hkTransform
{
	hkRotation rotation;
	hkVector4 translation;
};
COMPILE_ASSERT(sizeof(hkTransform) == 0x40);

