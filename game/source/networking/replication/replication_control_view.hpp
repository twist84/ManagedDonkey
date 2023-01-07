#pragma once

#include "cseries/cseries.hpp"
#include "networking/replication/replication_scheduler.hpp"

struct c_replication_control_view : c_replication_scheduler_client
{
	byte __data[0x13D4];
};
static_assert(sizeof(c_replication_control_view) == 0x13D8);

