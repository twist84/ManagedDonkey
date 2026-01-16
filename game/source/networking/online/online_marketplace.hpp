#pragma once

#include "cseries/async_xoverlapped.hpp"

class c_online_marketplace_consume_asset_task :
	public c_overlapped_task
{
public:
	virtual ~c_online_marketplace_consume_asset_task() = default;
	virtual const char* get_context_string() const override;
	virtual uns32 start(void* overlapped) override;

public:
	e_controller_index m_controller_index;
	int32 m_asset; // XMARKETPLACE_ASSET
};
COMPILE_ASSERT(sizeof(c_online_marketplace_consume_asset_task) == 0x18);
COMPILE_ASSERT(sizeof(c_online_marketplace_consume_asset_task) == sizeof(c_overlapped_task) + 0x8);

class c_online_marketplace_enumerate_assets_task :
	public c_overlapped_task
{
public:
	enum
	{
		k_maximum_number_of_assets_to_enumerate = 32,
	};

public:
	virtual ~c_online_marketplace_enumerate_assets_task() = default;
	virtual const char* get_context_string() const override;
	virtual uns32 start(void* overlapped) override;
	virtual void success(uns32 return_result) override;
	virtual void complete() override;
	virtual void reset() override;
	virtual bool is_result_successful(uns32 calling_result, uns32 overlapped_error, uns32 overlapped_extended_error) override;

protected:
	e_controller_index m_controller_index;
	void* m_enumeration_handle;
	void* m_overlapped_results_buffer;
	int32 m_results[k_maximum_number_of_assets_to_enumerate];
	int32 m_result_count;
};
COMPILE_ASSERT(sizeof(c_online_marketplace_enumerate_assets_task) == 0xA0);
COMPILE_ASSERT(sizeof(c_online_marketplace_enumerate_assets_task) == sizeof(c_overlapped_task) + 0x90);

class c_online_marketplace_enumerate_offers_task :
	public c_overlapped_task
{
public:
	enum
	{
		k_maximum_number_of_offers_to_enumerate = 32,
	};

	enum e_offer_filter_flags
	{
		_offer_filter_bnet_pro_subscription_bit = 0,
	};

	struct s_enumerated_offer
	{
		uns64 offer_id;
	};
	COMPILE_ASSERT(sizeof(s_enumerated_offer) == 0x8);

public:
	virtual ~c_online_marketplace_enumerate_offers_task() = default;
	virtual const char* get_context_string() const override;
	virtual uns32 start(void* overlapped) override;
	virtual void success(uns32 return_result) override;
	virtual void complete() override;
	virtual void reset() override;
	virtual bool is_result_successful(uns32 calling_result, uns32 overlapped_error, uns32 overlapped_extended_error) override;


protected:
	e_controller_index m_controller_index;
	uns32 m_offer_filter_flags;
	void* m_enumeration_handle;
	void* m_overlapped_results_buffer;
	s_enumerated_offer m_results[k_maximum_number_of_offers_to_enumerate];
	int32 m_result_count;
};
COMPILE_ASSERT(sizeof(c_online_marketplace_enumerate_offers_task) == 0x128);
COMPILE_ASSERT(sizeof(c_online_marketplace_enumerate_offers_task) == sizeof(c_overlapped_task) + 0x118);

class c_online_marketplace_purchase_task :
	public c_overlapped_task
{
public:
	enum
	{
		k_maximum_number_of_offers_to_purchase = 4,
	};

	enum e_entry_point
	{
		// $TODO map me
	};

public:
	virtual ~c_online_marketplace_purchase_task() = default;
	virtual const char* get_context_string() const override;
	virtual uns32 start(void* overlapped) override;
	virtual void success(uns32 return_result) override;

	e_controller_index m_controller_index;
	e_entry_point m_entry_point;
	uns64 m_offers[k_maximum_number_of_offers_to_purchase]; // is this the correct name
	int32 m_offer_count;
	int32 m_purchase_result;
};
COMPILE_ASSERT(sizeof(c_online_marketplace_purchase_task) == 0x40);
COMPILE_ASSERT(sizeof(c_online_marketplace_purchase_task) == sizeof(c_overlapped_task) + 0x30);

