#include "networking/online/online_marketplace.hpp"

#include "cseries/cseries_events.hpp"
#include "networking/online/online_error.hpp"

#include <winerror.h>

void c_online_marketplace_enumerate_assets_task::complete()
{
	INVOKE_CLASS_MEMBER(0x014E2DE0, c_online_marketplace_enumerate_assets_task, complete);
}

void c_online_marketplace_enumerate_offers_task::complete()
{
	INVOKE_CLASS_MEMBER(0x014E2E20, c_online_marketplace_enumerate_offers_task, complete);
}

const char* c_online_marketplace_consume_asset_task::get_context_string() const
{
	//return INVOKE_CLASS_MEMBER(0x014E2E60, c_online_marketplace_consume_asset_task, get_context_string);

	return "XMarketplaceConsumeAssets";
}

const char* c_online_marketplace_enumerate_assets_task::get_context_string() const
{
	//return INVOKE_CLASS_MEMBER(0x014E2E70, c_online_marketplace_enumerate_assets_task, get_context_string);

	return "XMarketplaceCreateAssetEnumerator";
}

const char* c_online_marketplace_enumerate_offers_task::get_context_string() const
{
	//return INVOKE_CLASS_MEMBER(0x014E2E80, c_online_marketplace_enumerate_offers_task, get_context_string);

	return "XMarketplaceCreateOfferEnumerator";
}

const char* c_online_marketplace_purchase_task::get_context_string() const
{
	//return INVOKE_CLASS_MEMBER(0x014E2E90, c_online_marketplace_purchase_task, get_context_string);

	return "XShowMarketplaceDownloadItemsUI";
}

//.text:014E2EA0 ; public: void c_online_marketplace_consume_asset_task::initialize(const char* file, int32 line, e_controller_index controller_index, const int32* asset)
//.text:014E2EC0 ; public: void c_online_marketplace_enumerate_assets_task::initialize(const char* file, int32 line, e_controller_index controller_index)
//.text:014E2EE0 ; public: void c_online_marketplace_enumerate_offers_task::initialize(const char* file, int32 line, e_controller_index controller_index, uns32 offer_filter_flags)
//.text:014E2F10 ; public: void c_online_marketplace_purchase_task::initialize(const char* file, int32 line, e_controller_index controller_index, c_online_marketplace_purchase_task::e_entry_point entry_point, const uns64* offer_ids, int32 offer_count)

bool c_online_marketplace_enumerate_assets_task::is_result_successful(uns32 calling_result, uns32 overlapped_error, uns32 overlapped_extended_error)
{
	return INVOKE_CLASS_MEMBER(0x014E2F70, c_online_marketplace_enumerate_assets_task, is_result_successful, calling_result, overlapped_error, overlapped_extended_error);
}

bool c_online_marketplace_enumerate_offers_task::is_result_successful(uns32 calling_result, uns32 overlapped_error, uns32 overlapped_extended_error)
{
	return INVOKE_CLASS_MEMBER(0x014E2FB0, c_online_marketplace_enumerate_offers_task, is_result_successful, calling_result, overlapped_error, overlapped_extended_error);
}

void c_online_marketplace_enumerate_assets_task::reset()
{
	INVOKE_CLASS_MEMBER(0x014E2FF0, c_online_marketplace_enumerate_assets_task, reset);
}

void c_online_marketplace_enumerate_offers_task::reset()
{
	INVOKE_CLASS_MEMBER(0x014E3020, c_online_marketplace_enumerate_offers_task, reset);
}

uns32 c_online_marketplace_consume_asset_task::start(void* overlapped)
{
	//return INVOKE_CLASS_MEMBER(0x014E3050, c_online_marketplace_consume_asset_task, start, overlapped);

	return E_FAIL;
}

uns32 c_online_marketplace_enumerate_assets_task::start(void* overlapped)
{
	//return INVOKE_CLASS_MEMBER(0x014E3060, c_online_marketplace_enumerate_assets_task, start, overlapped);

	m_result_count = 0;
	return E_FAIL;
}

uns32 c_online_marketplace_enumerate_offers_task::start(void* overlapped)
{
	//return INVOKE_CLASS_MEMBER(0x014E3080, c_online_marketplace_enumerate_offers_task, start, overlapped);

	m_result_count = 0;
	return E_FAIL;
}

uns32 c_online_marketplace_purchase_task::start(void* overlapped)
{
	//return INVOKE_CLASS_MEMBER(0x014E30A0, c_online_marketplace_purchase_task, start, overlapped);

	return E_FAIL;
}

void c_online_marketplace_enumerate_assets_task::success(uns32 return_result)
{
	INVOKE_CLASS_MEMBER(0x014E30B0, c_online_marketplace_enumerate_assets_task, success, return_result);
}

void c_online_marketplace_enumerate_offers_task::success(uns32 return_result)
{
	INVOKE_CLASS_MEMBER(0x014E30D0, c_online_marketplace_enumerate_offers_task, success, return_result);
}

void c_online_marketplace_purchase_task::success(uns32 return_result)
{
	//INVOKE_CLASS_MEMBER(0x014E30F0, c_online_marketplace_purchase_task, success, return_result);

	event(_event_message, "networking:online:marketplace: XShowMarketplaceDownloadItemsUI result: %s",
		online_error_get_string(m_purchase_result).get_string());
}

