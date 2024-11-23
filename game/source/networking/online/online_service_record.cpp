#include "networking/online/online_service_record.hpp"

REFERENCE_DECLARE(0x0229AEC8, c_online_service_record_manager, g_online_service_record_manager);

void __cdecl online_service_record_manager_dispose()
{
	INVOKE(0x004E1520, online_service_record_manager_dispose);
}

void __cdecl online_service_record_manager_initialize()
{
	INVOKE(0x004E1530, online_service_record_manager_initialize);
}

void __cdecl online_service_record_manager_update()
{
	INVOKE(0x004E1540, online_service_record_manager_update);
}

