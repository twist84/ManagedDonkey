#include "cache/cache_file_builder_tag_resource_manager.hpp"

char const* const k_cache_file_resource_type_names[k_cache_file_resource_type_count]
{
	"structure_bsp_cache_file_tag_resources",
	"bitmap_texture_interop_resource",
	"bitmap_texture_interleaved_interop_resource",
	"sound_resource_definition",
	"model_animation_tag_resource",
	"render_geometry_api_resource_definition",
	"bink_resource",
	"structure_bsp_tag_resources"
};

char const* cache_file_resource_type_get_name(long resource_type)
{
	if (resource_type < _cache_file_resource_type_structure_bsp_cache_file_tag_resources || resource_type >= k_cache_file_resource_type_count)
		return "<invalid 'cache_file_resource_type'>";

	return k_cache_file_resource_type_names[resource_type];
}

