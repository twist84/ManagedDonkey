#pragma once

#include "cseries/cseries.hpp"

struct s_model_skinning_matrix
{
	real32 forward_i;
	real32 left_i;
	real32 up_i;
	real32 center_x;

	real32 forward_j;
	real32 left_j;
	real32 up_j;
	real32 center_y;

	real32 forward_k;
	real32 left_k;
	real32 up_k;
	real32 center_z;
};
COMPILE_ASSERT(sizeof(s_model_skinning_matrix) == sizeof(real32) * 12);

