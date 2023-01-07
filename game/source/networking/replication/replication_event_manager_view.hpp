#pragma once

#include "cseries/cseries.hpp"
#include "networking/replication/replication_scheduler.hpp"

struct c_replication_event_manager_view : c_replication_scheduler_client
{
	byte __data[0x24];
};
static_assert(sizeof(c_replication_event_manager_view) == 0x28);

