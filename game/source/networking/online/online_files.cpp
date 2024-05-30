#include "networking/online/online_files.hpp"

#include "cseries/cseries.hpp"

void __cdecl online_files_dispose()
{
	INVOKE(0x00477910, online_files_dispose);
}

//.text:00477930 ; void __cdecl online_files_memory_dispose()
//.text:00477970 ; void __cdecl online_files_memory_initialize(e_map_memory_configuration)

void __cdecl online_files_initialize()
{
	INVOKE(0x00477920, online_files_initialize);
}

void __cdecl online_files_update()
{
	INVOKE(0x004779E0, online_files_update);
}

