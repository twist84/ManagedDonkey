#include "interface/overhead_map.hpp"

#include "cseries/cseries.hpp"

// $TODO find the exact location of this file

void __cdecl overhead_map_dispose()
{
	INVOKE(0x00AADBE0, overhead_map_dispose);
}

void __cdecl overhead_map_dispose_from_old_map()
{
	INVOKE(0x00AADBF0, overhead_map_dispose_from_old_map);
}

void __cdecl overhead_map_initialize()
{
	INVOKE(0x00AADC00, overhead_map_initialize);
}

void __cdecl overhead_map_initialize_for_new_map()
{
	INVOKE(0x00AADC10, overhead_map_initialize_for_new_map);
}

void __cdecl overhead_map_render()
{
	INVOKE(0x00AADC30, overhead_map_render);
}

void __cdecl overhead_map_update()
{
	INVOKE(0x00AADC40, overhead_map_update);
}



