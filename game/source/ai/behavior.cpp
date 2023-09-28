#include "ai/behavior.hpp"

#include "cseries/cseries.hpp"

short __cdecl behavior_index_by_name(char* name)
{
	return INVOKE(0x014745C0, behavior_index_by_name, name);
}

