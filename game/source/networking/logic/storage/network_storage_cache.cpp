#include "networking/logic/storage/network_storage_cache.hpp"

#include "cseries/cseries.hpp"

void __cdecl network_storage_cache_clear()
{
	INVOKE(0x004A4C30, network_storage_cache_clear);
}

void __cdecl network_storage_cache_dispose()
{
	INVOKE(0x004A4C40, network_storage_cache_dispose);
}

void __cdecl network_storage_cache_initialize()
{
	INVOKE(0x004A4C50, network_storage_cache_initialize);
}

void __cdecl network_storage_cache_update()
{
	INVOKE(0x004A4C60, network_storage_cache_update);
}

