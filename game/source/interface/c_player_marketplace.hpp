#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "networking/online/online_marketplace.hpp"
#include "shell/shell.hpp"
#include "text/unicode.hpp"

struct c_player_marketplace
{
private:
	static long& x_active_count;
	static c_online_marketplace_purchase_task& m_purchase_task;
	static c_online_marketplace_enumerate_assets_task& m_enumerate_assets_task;
	static c_online_marketplace_consume_asset_task& m_consume_asset_task;
	static c_http_stored_buffer_downloader<4096>& m_downloader;

protected:
	c_enum<e_controller_index, long, _controller_index0, k_number_of_controllers> m_controller_index;

	bool m_refresh;
	long m_marketplace_state;

	long __unknownC;

	long m_subscription_info_checked;
	bool m_extras_portal_enabled;

	// 0: ?
	// 1: NeverSubscribed
	// 2: Subscribed
	// 3: SubscribedRenew
	// 4: Expired
	long m_status;

	byte __data1C[0x4];

	// NextOfferID
	qword m_next_offer_id;

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
	qword m_subscription_seconds;

	// SubscriptionHash
	long m_subscription_hash;

	// PortalButton
	c_static_wchar_string<256> m_extras_portal;

	// PortalOfferCount
	long m_portal_offer_count;

	// PortalOfferID
	qword m_portal_offer_id;

	// PortalExecuteImageFilename
	c_static_string<256> m_portal_execute_package_filename;

	// PortalExecuteImageFilename
	c_static_string<256> m_portal_execute_image_filename;

	// PortalExecuteLaunchData
	qword m_portal_execute_launch_data;

	long m_consumable_assets_checked;

	bool __unknown144C;
	byte __pad144D[3];

	byte __data1450[12];

	bool __unknown145C;
};
static_assert(sizeof(c_player_marketplace) == 0x1460);

//extern c_static_array<c_player_marketplace, 4>& g_player_marketplace;
extern c_player_marketplace(&g_player_marketplace)[4];

