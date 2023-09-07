#pragma once

struct cluster_partition
{
	void* first_data;
	void* data_reference;
	void* cluster_first_data_references;
};
static_assert(sizeof(cluster_partition) == 0xC);

