#pragma once

#include "cseries/async_xoverlapped.hpp"

class c_online_marketplace_purchase_task :
	public c_overlapped_task
{
public:
	static unsigned int const k_max_offer_count = 4;

	e_controller_index m_controller_index;
	int32 m_entry_point;
	uns64 m_offers[k_max_offer_count]; // is this the correct name
	int32 m_offer_count;
	int32 m_purchase_result;
};
static_assert(sizeof(c_online_marketplace_purchase_task) == 0x40);

class c_online_marketplace_enumerate_assets_task :
	public c_overlapped_task
{
public:
	enum
	{
		k_maximum_number_of_assets_to_enumerate = 32,
	};

	e_controller_index m_controller_index;
	void* m_enumeration_handle;
	void* m_overlapped_results_buffer;
	int32 m_results[k_maximum_number_of_assets_to_enumerate];
	int32 m_result_count;
};
static_assert(sizeof(c_online_marketplace_enumerate_assets_task) == 0xA0);

class c_online_marketplace_consume_asset_task :
	public c_overlapped_task
{
public:
	e_controller_index m_controller_index;
	int32 m_asset; // XMARKETPLACE_ASSET
};
static_assert(sizeof(c_online_marketplace_consume_asset_task) == 0x18);

