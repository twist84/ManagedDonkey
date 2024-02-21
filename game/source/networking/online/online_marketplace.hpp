#pragma once

#include "cseries/async_xoverlapped.hpp"

struct c_online_marketplace_purchase_task :
	c_overlapped_task
{
	byte __data10[0x30];
};
static_assert(sizeof(c_online_marketplace_purchase_task) == 0x40);

struct c_online_marketplace_enumerate_assets_task :
	c_overlapped_task
{
	byte __data10[0x90];
};
static_assert(sizeof(c_online_marketplace_enumerate_assets_task) == 0xA0);

struct c_online_marketplace_consume_asset_task :
	c_overlapped_task
{
	byte __data10[0x8];
};
static_assert(sizeof(c_online_marketplace_consume_asset_task) == 0x18);

