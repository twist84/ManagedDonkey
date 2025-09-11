#include "scenario/scenario_cubemap_definitions.hpp"

#include "scenario/scenario.hpp"
#include "structures/structure_bsp_definitions.hpp"

s_structure_cluster_cubemap_info* structure_cluster_try_and_get_cubemap_info(s_cluster_reference cluster_reference, int32 cubemap_index)
{
    if (cluster_reference_valid(&cluster_reference) && cubemap_index >= 0)
    {
        structure_cluster& cluster = global_structure_bsp_get(cluster_reference.bsp_index)->clusters[cluster_reference.cluster_index];
        if (VALID_INDEX(cubemap_index, cluster.cluster_cubemaps.count))
        {
            return &cluster.cluster_cubemaps[cubemap_index];
        }
    }

    return nullptr;
}

