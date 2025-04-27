#include "interface/c_player_marketplace.hpp"

REFERENCE_DECLARE(0x0526A178, int32, x_active_count);
REFERENCE_DECLARE(0x0526A180, c_online_marketplace_purchase_task, m_purchase_task);
REFERENCE_DECLARE(0x0526A1C0, c_online_marketplace_enumerate_assets_task, m_enumerate_assets_task);

//REFERENCE_DECLARE_STATIC_ARRAY(0x0526A260, c_player_marketplace, 4, g_player_marketplace);
REFERENCE_DECLARE_ARRAY(0x0526A260, c_player_marketplace, g_player_marketplace, 4);

REFERENCE_DECLARE(0x0526F3E0, c_online_marketplace_consume_asset_task, m_consume_asset_task);
REFERENCE_DECLARE(0x0526F3F8, c_http_stored_buffer_downloader<4096>, c_player_marketplace::m_downloader);

