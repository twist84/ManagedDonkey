#include "networking/session/network_session_membership.hpp"

#include "networking/messages/network_messages_session_membership.hpp"

long c_network_session_membership::get_first_peer() const
{
	//return INVOKE_CLASS_MEMBER(0x0044E690, c_network_session_membership, get_first_peer);

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

long c_network_session_membership::get_first_player() const
{
	return INVOKE_CLASS_MEMBER(0x0044E6C0, c_network_session_membership, get_first_player);
}

//.text:0044E6F0 ; public: s_player_add_queue_entry const* c_network_session_membership::get_first_player_from_player_add_queue() const

long c_network_session_membership::get_next_peer(long peer_index) const
{
	//return INVOKE_CLASS_MEMBER(0x0044E710, c_network_session_membership, get_next_peer, peer_index);

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

long c_network_session_membership::get_next_player(long peer_index) const
{
	return INVOKE_CLASS_MEMBER(0x0044E750, c_network_session_membership, get_next_player, peer_index);
}

//.text:0044E790 ; public: long c_network_session_membership::update_number() const
//.text:0044E7A0 ; public: long c_network_session_membership::get_observer_channel_index(long) const

s_network_session_peer* c_network_session_membership::get_peer(long peer_index)
{
	return INVOKE_CLASS_MEMBER(0x0044E7C0, c_network_session_membership, get_peer, peer_index);
}

//.text:0044E7E0 ; public: e_network_session_peer_state c_network_session_membership::get_peer_connection_state(long) const
//.text:0044E800 ; public: long c_network_session_membership::get_peer_from_incoming_address(transport_address const*) const
//.text:0044E860 ; public: long c_network_session_membership::get_peer_from_observer_channel(long) const
//.text:0044E910 ; public: long c_network_session_membership::get_peer_from_secure_address(s_transport_secure_address const*) const
//.text:0044E9E0 ; public: long c_network_session_membership::get_peer_from_unique_identifier(s_transport_unique_identifier const*) const
//.text:0044EAE0 ; public: long c_network_session_membership::get_peer_index_of_player_in_queue(s_player_identifier const*) const
//.text:0044EB10 ; public: dword c_network_session_membership::get_peer_valid_mask() const
//.text:0044EB20 ; public: s_network_session_player const* c_network_session_membership::get_player(long) const
//.text:0044EB40 ; public: s_player_add_queue_entry const* c_network_session_membership::get_player_add_queue_entry(long) const
//.text:0044EB90 ; public: long c_network_session_membership::get_player_from_identifier(s_player_identifier const*) const
//.text:0044EC60 ; public: long c_network_session_membership::get_player_from_xuid(qword) const
//.text:0044ED30 ; public: c_flags<long, dword, 16> c_network_session_membership::get_player_incompatible_determinism_mask(long) const
//.text:0044EE00 ; public: dword c_network_session_membership::get_player_valid_mask() const
//.text:0044EE10 ; public: s_player_identifier const* c_network_session_membership::get_player_identifier(long) const
//.text:0044EE30 ; 
//.text:0044EE50 ; 
//.text:0044EE70 ; 
//.text:0044EE80 ; 
//.text:0044EE90 ; public: s_network_session_shared_membership const* c_network_session_membership::get_transmitted_membership(long) const
//.text:0044EEB0 ; private: c_network_session* c_network_session_membership::get_session()
//.text:0044EEC0 ; public: bool c_network_session_membership::handle_membership_update(s_network_message_membership_update const*)
//.text:0044F930 ; public: bool c_network_session_membership::host_exists_at_incoming_address(transport_address const*) const
//.text:0044F9B0 ; public: void c_network_session_membership::idle()
//.text:0044FA50 ; private: void c_network_session_membership::increment_update()
//.text:0044FA60 ; 
//.text:0044FA80 ; public: bool c_network_session_membership::is_peer_a_bad_client(long) const
//.text:0044FAF0 ; public: bool c_network_session_membership::is_peer_established(long) const
//.text: ; public: bool c_network_session_membership::is_peer_valid(long) const

bool c_network_session_membership::is_peer_valid(long peer_index) const
{
	//return INVOKE_CLASS_MEMBER(0x0044FB10, c_network_session_membership, is_peer_valid, peer_index);

	return m_shared_network_membership.peer_valid_mask.test(peer_index);
}

//.text:0044FB40 ; public: bool c_network_session_membership::is_player_established(long) const
//.text:0044FB70 ; public: bool c_network_session_membership::is_player_in_player_add_queue(s_player_identifier const*) const
//.text:0044FB90 ; public: void c_network_session_membership::lock_slots()

long c_network_session_membership::get_player_index_from_peer(long peer_index)
{
	return DECLFUNC(0x0052E280, long, __cdecl, dword*, long)(m_shared_network_membership.peers[peer_index].player_mask, 16);
}

long c_network_session_membership::get_observer_channel_index(long peer_index) const
{
	ASSERT(is_peer_valid(peer_index));
	return m_local_peer_state[peer_index].channel_index;
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

	if (player->configuration.host.weapon.loadouts[0].bungienet_user != player_data->host_partial.bungienet_user)
	{
		player->configuration.host.weapon.loadouts[0].bungienet_user = player_data->host_partial.bungienet_user;
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

	if (csmemcmp(&player->configuration.host.weapon.loadouts[0].consumables, &player_data->host_partial.consumables, sizeof(c_static_array<char, 4>)))
	{
		player->configuration.host.weapon.loadouts[0].consumables = player_data->host_partial.consumables;
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

