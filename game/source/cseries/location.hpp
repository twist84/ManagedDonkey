#pragma once

struct s_portal_reference
{
	int16 bsp_index : 6;
	int16 portal_index : 10;
};
COMPILE_ASSERT(sizeof(s_portal_reference) == sizeof(int16));

struct s_cluster_reference
{
	int8 bsp_index;
	uns8 cluster_index;
};
COMPILE_ASSERT(sizeof(s_cluster_reference) == 0x2);

struct s_location
{
	s_cluster_reference cluster_reference;
	//uns16 leaf_index;
};
COMPILE_ASSERT(sizeof(s_location) == sizeof(s_cluster_reference));
//COMPILE_ASSERT(sizeof(s_location) == 0x4);

