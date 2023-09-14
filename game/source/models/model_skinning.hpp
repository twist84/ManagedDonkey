#pragma once

#include "cseries/cseries.hpp"

struct s_model_skinning_matrix
{
	real forward_i;
	real left_i;
	real up_i;
	real center_x;

	real forward_j;
	real left_j;
	real up_j;
	real center_y;

	real forward_k;
	real left_k;
	real up_k;
	real center_z;
};
static_assert(sizeof(s_model_skinning_matrix) == sizeof(real) * 12);

