#pragma once

#include "cseries/async_xoverlapped.hpp"

struct c_online_marketplace_purchase_task :
	public c_overlapped_task
{
	static unsigned int const k_max_offer_count = 4;

	long m_controller_index;
	long m_entry_point;
	qword m_offers[k_max_offer_count]; // is this the correct name
	long m_offer_count;
	long m_purchase_result;
};
static_assert(sizeof(c_online_marketplace_purchase_task) == 0x40);

struct c_online_marketplace_enumerate_assets_task :
	public c_overlapped_task
{
	long m_controller_index;
	void* m_enumeration_handle;

	byte m_results[0x84]; // is this the correct name
	long m_result_count;
};
static_assert(sizeof(c_online_marketplace_enumerate_assets_task) == 0xA0);

struct c_online_marketplace_consume_asset_task :
	public c_overlapped_task
{
	long m_controller_index;
	void* m_marketplace_asset; // XMARKETPLACE_ASSET
};
static_assert(sizeof(c_online_marketplace_consume_asset_task) == 0x18);

