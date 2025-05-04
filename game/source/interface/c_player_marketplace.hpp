#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "networking/online/online_marketplace.hpp"
#include "text/unicode.hpp"

struct c_player_marketplace
{
private:
	static int32& x_active_count;
	static c_online_marketplace_purchase_task& m_purchase_task;
	static c_online_marketplace_enumerate_assets_task& m_enumerate_assets_task;
	static c_online_marketplace_consume_asset_task& m_consume_asset_task;
	static c_http_stored_buffer_downloader<4096>& m_downloader;

protected:
	c_enum<e_controller_index, int32, _controller0, k_number_of_controllers> m_controller_index;

	bool m_refresh;
	int32 m_marketplace_state;

	int32 __unknownC;

	int32 m_subscription_info_checked;
	bool m_extras_portal_enabled;

	// 0: ?
	// 1: NeverSubscribed
	// 2: Subscribed
	// 3: SubscribedRenew
	// 4: Expired
	int32 m_status;

	byte __data1C[0x4];

	// NextOfferID
	uns64 m_next_offer_id;

	// 0: HQButton
	// 1: HQMessage
	// 2: FileShareButton
	// 3: FileShareMessage
	// 4: FileShareHelp
	// 5: JustSubscribedMessage
	// 6: CurrentlySubscribedMessage
	// 7: OverQuotaMessage
	c_static_array<c_static_wchar_string<256>, 8> m_strings;

	// SubscriptionSecondsPast19700101
	uns64 m_subscription_seconds;

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

	int32 m_consumable_assets_checked;

	bool __unknown144C;
	byte __pad144D[0x3];

	byte __data1450[0xC];

	bool __unknown145C;
};
static_assert(sizeof(c_player_marketplace) == 0x1460);

//extern c_static_array<c_player_marketplace, 4>& g_player_marketplace;
extern c_player_marketplace(&g_player_marketplace)[4];

