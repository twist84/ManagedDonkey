#include "cache/cache_files.hpp"

s_cache_file_globals& g_cache_file_globals = *reinterpret_cast<s_cache_file_globals*>(0x022AAFE8);

struct s_cache_file_global_tags_definition
{
	struct
	{
		long count;
		struct
		{
			tag group_tag;
			long name_offset;
			long name_length;
			long index;
		}*address;
		void* definition;
	} references;
};

long __cdecl cache_file_get_global_tag_index(tag group_tag)
{
	//s_cache_file_global_tags_definition* global_tags = tag_get<s_cache_file_global_tags_definition>('cfgt', 0);
	//if (!global_tags)
	//	return -1;
	//for (long i = 0; i < global_tags->references.count; i++)
	//	if (group_tag == global_tags->references.address[i].group_tag)
	//		return global_tags->references.address[i].index;
	//return -1;

	return DECLTHUNK(0x005017E0, cache_file_get_global_tag_index, group_tag);
}