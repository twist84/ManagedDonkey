#include "networking/session/network_session_membership.hpp"
#include <networking/messages/network_messages_session_membership.hpp>

long c_network_session_membership::get_player_index_from_peer(long peer_index)
{
	return DECLFUNC(0x0052E280, long, __cdecl, dword*, long)(m_shared_network_membership.peers[peer_index].player_mask, 16);
}

long c_network_session_membership::get_first_peer() const
{
	long first_peer = NONE;
	for (long i = 0; i < 17; i++)
	{
		if (is_peer_valid(i))
		{
			first_peer = i;
			break;
		}
	}
	return first_peer;
}

long c_network_session_membership::get_next_peer(long peer_index) const
{
	ASSERT(peer_index != NONE);

	long next_peer = NONE;
	for (long i = peer_index + 1; i < 17; i++)
	{
		if (is_peer_valid(i))
		{
			next_peer = i;
			break;
		}
	}
	return next_peer;
}

long c_network_session_membership::get_observer_channel_index(long peer_index) const
{
	ASSERT(is_peer_valid(peer_index));
	return m_local_peers[peer_index].channel_index;
}

long c_network_session_membership::get_peer_from_observer_channel(long observer_channel_index) const
{
	long peer_index = NONE;
	if (observer_channel_index != NONE)
	{
		for (peer_index = get_first_peer();
			peer_index != NONE && get_observer_channel_index(peer_index) != observer_channel_index;
			peer_index = get_next_peer(peer_index))
		{
			;
		}
	}
	return peer_index;
}

void c_network_session_membership::set_player_properties(long player_index, long player_update_number, long controller_index, void const* player_data_, long player_voice)
{
	s_network_session_player* player = get_player(player_index);

	bool update_increment = false;
	if (player->desired_configuration_version != player_update_number)
	{
		player->desired_configuration_version = player_update_number;
		update_increment = true;
	}

	if (player->controller_index != controller_index)
	{
		player->controller_index = controller_index;
		update_increment = true;
	}

	s_player_configuration_for_player_properties const* player_data = static_cast<s_player_configuration_for_player_properties const*>(player_data_);
	if (csmemcmp(&player->configuration.client, &player_data->client, sizeof(s_player_configuration_from_client)))
	{
		player->configuration.client = player_data->client;
		update_increment = true;
	}

	if (csmemcmp(&player->configuration.host.appearance.service_tag, &player_data->host_partial.service_tag, sizeof(c_static_wchar_string<5>)))
	{
		player->configuration.host.appearance.service_tag = player_data->host_partial.service_tag;
		update_increment = true;
	}

	if (csmemcmp(&player->configuration.host.armor.loadouts[0].colors, &player_data->host_partial.colors, sizeof(c_static_array<rgb_color, k_color_type_count>)))
	{
		player->configuration.host.armor.loadouts[0].colors = player_data->host_partial.colors;
		update_increment = true;
	}

	if (csmemcmp(&player->configuration.host.armor.loadouts[0].armors, &player_data->host_partial.armors, sizeof(c_static_array<byte, k_armor_type_count>)))
	{
		player->configuration.host.armor.loadouts[0].armors = player_data->host_partial.armors;
		update_increment = true;
	}

	if (player->voice_settings != player_voice)
	{
		player->voice_settings = player_voice;
		update_increment = true;
	}

	if (update_increment)
		increment_update();
}

