#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "networking/online/online_marketplace.hpp"
#include "text/unicode.hpp"

class c_player_marketplace
{
public:
	enum
	{
		k_maximum_http_results_size = 0x1000,
	};

	enum e_message_type
	{
		_message_type_headquarters_button = 0,
		_message_type_headquarters_message,
		_message_type_file_share_button,
		_message_type_file_share_message,
		_message_type_file_share_help,
		_message_type_just_subscribed_message,
		_message_type_currently_subscribed_message,
		_message_type_over_quota_message,

		k_message_type_count,
	};

	enum e_marketplace_state
	{
		_marketplace_state_none = 0,
		_marketplace_state_assets_enumerate,
		_marketplace_state_asset_consume_pre_updating_website,
		_marketplace_state_asset_consume,
		_marketplace_state_asset_consume_post_updating_website,
		_marketplace_state_subscription_get_details,

		k_marketplace_state_count,
	};

	enum e_subscription_status
	{
		_subscription_status_none = 0,
		_subscription_status_never_subscribed,
		_subscription_status_subscribed,
		_subscription_status_subscribed_renew,
		_subscription_status_expired,
	};

	enum e_task_state
	{
		_task_state_none = 0,
		_task_state_pending,
		_task_state_complete,
	};

public:
	static c_player_marketplace* __cdecl get(e_controller_index controller_index);

private:
	static int32& x_active_count;
	static c_online_marketplace_purchase_task& m_purchase_task;
	static c_online_marketplace_enumerate_assets_task& m_enumerate_assets_task;
	static c_online_marketplace_consume_asset_task& m_consume_asset_task;
	static c_http_stored_buffer_downloader<k_maximum_http_results_size>& m_downloader;

protected:
	e_controller_index m_controller_index;
	bool m_needs_refresh;
	e_marketplace_state m_state;
	int32 m_subscription_details_cookie;
	int32 m_subscription_info_checked;
	bool m_subscription_info_valid;

	// Status
	// 1: NeverSubscribed
	// 2: Subscribed
	// 3: SubscribedRenew
	// 4: Expired
	e_subscription_status m_subscription_status;

	// NextOfferID
	uns64 m_subscription_next_offer_id;

	// 0: HQButton
	// 1: HQMessage
	// 2: FileShareButton
	// 3: FileShareMessage
	// 4: FileShareHelp
	// 5: JustSubscribedMessage
	// 6: CurrentlySubscribedMessage
	// 7: OverQuotaMessage
	c_static_wchar_string<256> m_subscription_message[k_message_type_count];

	// SubscriptionSecondsPast19700101
	uns64 m_subscription_expiration_time;

	// SubscriptionHash
	int32 m_subscription_hash;

	// PortalButton
	c_static_wchar_string<256> m_extras_portal;

	// PortalOfferCount
	int32 m_portal_offer_count;

	// PortalOfferID
	uns64 m_portal_offer_id;

	// PortalExecuteImageFilename
	c_static_string<256> m_portal_execute_package_filename;

	// PortalExecuteImageFilename
	c_static_string<256> m_portal_execute_image_filename;

	// PortalExecuteLaunchData
	uns64 m_portal_execute_launch_data;

	e_task_state m_consumable_assets_checked;
	bool m_asset_to_consume_valid;
	int32 m_asset_to_consume;
	int32 m_progress_dialog_start_time;
	int32 m_purchase_result; // HRESULT
	bool m_purchase_attempted_this_session;
};
COMPILE_ASSERT(sizeof(c_player_marketplace) == 0x1460);

//extern c_static_array<c_player_marketplace, 4>& g_player_marketplace;
extern c_player_marketplace(&g_player_marketplace)[4];

extern void online_marketplace_refresh();

