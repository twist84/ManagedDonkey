#include "networking/session/network_session_membership.hpp"

#include "networking/messages/network_messages_session_membership.hpp"

int32 c_network_session_membership::get_first_peer() const
{
	//return INVOKE_CLASS_MEMBER(0x0044E690, c_network_session_membership, get_first_peer);

	int32 first_peer = NONE;
	for (int32 i = 0; i < k_maximum_machines; i++)
	{
		if (is_peer_valid(i))
		{
			first_peer = i;
			break;
		}
	}
	return first_peer;
}

int32 c_network_session_membership::get_first_player() const
{
	return INVOKE_CLASS_MEMBER(0x0044E6C0, c_network_session_membership, get_first_player);
}

//.text:0044E6F0 ; public: const s_player_add_queue_entry* c_network_session_membership::get_first_player_from_player_add_queue() const

const s_network_session_peer* c_network_session_membership::get_local_peer() const
{
	ASSERT(is_peer_valid(m_local_peer_index));
	return &m_shared_network_membership.peers[m_local_peer_index];
}

s_network_session_peer* c_network_session_membership::get_local_peer()
{
	ASSERT(is_peer_valid(m_local_peer_index));
	return &m_shared_network_membership.peers[m_local_peer_index];
}

int32 c_network_session_membership::get_next_peer(int32 peer_index) const
{
	//return INVOKE_CLASS_MEMBER(0x0044E710, c_network_session_membership, get_next_peer, peer_index);

	ASSERT(peer_index != NONE);

	int32 next_peer = NONE;
	for (int32 i = peer_index + 1; i < 17; i++)
	{
		if (is_peer_valid(i))
		{
			next_peer = i;
			break;
		}
	}
	return next_peer;
}

int32 c_network_session_membership::get_next_player(int32 peer_index) const
{
	return INVOKE_CLASS_MEMBER(0x0044E750, c_network_session_membership, get_next_player, peer_index);
}

//.text:0044E790 ; public: int32 c_network_session_membership::update_number() const
//.text:0044E7A0 ; public: int32 c_network_session_membership::get_observer_channel_index(int32) const

const s_network_session_peer* c_network_session_membership::get_host_peer() const
{
	ASSERT(is_peer_valid(host_peer_index()));

	return &m_shared_network_membership.peers[host_peer_index()];
}

s_network_session_peer* c_network_session_membership::get_host_peer()
{
	ASSERT(is_peer_valid(host_peer_index()));

	return &m_shared_network_membership.peers[host_peer_index()];
}

s_network_session_peer const* c_network_session_membership::get_peer(int32 peer_index) const
{
	ASSERT(is_peer_valid(peer_index));

	return &m_shared_network_membership.peers[peer_index];
}

s_network_session_peer* c_network_session_membership::get_peer(int32 peer_index)
{
	//return INVOKE_CLASS_MEMBER(0x0044E7C0, c_network_session_membership, get_peer, peer_index);

	ASSERT(is_peer_valid(peer_index));

	return &m_shared_network_membership.peers[peer_index];
}

e_network_session_peer_state c_network_session_membership::get_peer_connection_state(int32 peer_index) const
{
	return INVOKE_CLASS_MEMBER(0x0044E7E0, c_network_session_membership, get_peer_connection_state, peer_index);
}

//.text:0044E800 ; public: int32 c_network_session_membership::get_peer_from_incoming_address(const transport_address*) const
//.text:0044E860 ; public: int32 c_network_session_membership::get_peer_from_observer_channel(int32) const
//.text:0044E910 ; public: int32 c_network_session_membership::get_peer_from_secure_address(const s_transport_secure_address*) const
//.text:0044E9E0 ; public: int32 c_network_session_membership::get_peer_from_unique_identifier(const s_transport_unique_identifier*) const
//.text:0044EAE0 ; public: int32 c_network_session_membership::get_peer_index_of_player_in_queue(const s_player_identifier*) const
//.text:0044EB10 ; public: uns32 c_network_session_membership::get_peer_valid_mask() const

const s_network_session_player* c_network_session_membership::get_player(int32 player_index) const
{
	//return INVOKE_CLASS_MEMBER(0x0044EB20, c_network_session_membership, get_player, player_index);

	ASSERT(is_player_valid(player_index));

	return &m_shared_network_membership.players[player_index];
}

s_network_session_player* c_network_session_membership::get_player(int32 player_index)
{
	ASSERT(is_player_valid(player_index));

	return &m_shared_network_membership.players[player_index];
}

//.text:0044EB40 ; public: const s_player_add_queue_entry* c_network_session_membership::get_player_add_queue_entry(int32) const
//.text:0044EB90 ; public: int32 c_network_session_membership::get_player_from_identifier(const s_player_identifier*) const
//.text:0044EC60 ; public: int32 c_network_session_membership::get_player_from_xuid(uns64) const
//.text:0044ED30 ; public: c_flags<int32, uns32, 16> c_network_session_membership::get_player_incompatible_determinism_mask(int32) const
//.text:0044EE00 ; public: uns32 c_network_session_membership::get_player_valid_mask() const
//.text:0044EE10 ; public: const s_player_identifier* c_network_session_membership::get_player_identifier(int32) const
//.text:0044EE30 ; 
//.text:0044EE50 ; 
//.text:0044EE70 ; 
//.text:0044EE80 ; 
//.text:0044EE90 ; public: const s_network_session_shared_membership* c_network_session_membership::get_transmitted_membership(int32) const
//.text:0044EEB0 ; private: c_network_session* c_network_session_membership::get_session()
//.text:0044EEC0 ; public: bool c_network_session_membership::handle_membership_update(const s_network_message_membership_update*)

bool c_network_session_membership::has_membership() const
{
	return (m_shared_network_membership.update_number + 1) == 0;
}

//.text:0044F930 ; public: bool c_network_session_membership::host_exists_at_incoming_address(const transport_address*) const

int32 c_network_session_membership::host_peer_index() const
{
	return m_shared_network_membership.host_peer_index;
}

//.text:0044F9B0 ; public: void c_network_session_membership::idle()

void c_network_session_membership::increment_update()
{
	//INVOKE_CLASS_MEMBER(0x0044FA50, c_network_session_membership, increment_update);

	m_shared_network_membership.update_number++;
	m_local_membership_update_number++;
}

bool c_network_session_membership::is_leader() const
{
	return m_shared_network_membership.update_number != NONE
		&& m_local_peer_index == m_shared_network_membership.leader_peer_index;
}

//.text:0044FA60 ; 
//.text:0044FA80 ; public: bool c_network_session_membership::is_peer_a_bad_client(int32) const
//.text:0044FAF0 ; public: bool c_network_session_membership::is_peer_established(int32) const

bool c_network_session_membership::is_peer_valid(int32 peer_index) const
{
	//return INVOKE_CLASS_MEMBER(0x0044FB10, c_network_session_membership, is_peer_valid, peer_index);

	return m_shared_network_membership.peer_valid_mask.test(peer_index);
}

bool c_network_session_membership::is_player_valid(int32 player_index) const
{
	return m_shared_network_membership.player_valid_mask.test(player_index);
}

//.text:0044FB40 ; public: bool c_network_session_membership::is_player_established(int32) const
//.text:0044FB70 ; public: bool c_network_session_membership::is_player_in_player_add_queue(const s_player_identifier*) const
//.text:0044FB90 ; public: void c_network_session_membership::lock_slots()

int32 c_network_session_membership::get_player_index_from_peer(int32 peer_index)
{
	//return DECLFUNC(0x0052E280, int32, __cdecl, uns32*, int32)(m_shared_network_membership.peers[peer_index].player_mask, 16);

	return bit_vector_lowest_bit_set(m_shared_network_membership.peers[peer_index].player_mask, 16);
}

int32 c_network_session_membership::get_observer_channel_index(int32 peer_index) const
{
	ASSERT(is_peer_valid(peer_index));
	return m_local_peer_state[peer_index].channel_index;
}

int32 c_network_session_membership::get_peer_from_observer_channel(int32 observer_channel_index) const
{
	int32 peer_index = NONE;
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

int32 c_network_session_membership::local_peer_index() const
{
	return m_local_peer_index;
}

void c_network_session_membership::set_player_properties(int32 player_index, int32 player_update_number, e_controller_index controller_index, const void* player_data_from_client, int32 player_voice_settings)
{
	s_network_session_player* player = get_player(player_index);

	bool update_increment = false;
	if (player->player_update_number != player_update_number)
	{
		player->player_update_number = player_update_number;
		update_increment = true;
	}

	if (player->controller_index != controller_index)
	{
		player->controller_index = controller_index;
		update_increment = true;
	}

	const s_player_configuration_for_player_properties* player_data = (const s_player_configuration_for_player_properties*)player_data_from_client;
	if (csmemcmp(&player->player_data.client, &player_data->client, sizeof(s_player_configuration_from_client)))
	{
		player->player_data.client = player_data->client;
		update_increment = true;
	}

	if (csmemcmp(&player->player_data.host.appearance.service_tag, &player_data->host_partial.service_tag, sizeof(c_static_wchar_string<5>)))
	{
		player->player_data.host.appearance.service_tag = player_data->host_partial.service_tag;
		update_increment = true;
	}

	if (player->player_data.host.weapon.loadouts[0].bungienet_user != player_data->host_partial.bungienet_user)
	{
		player->player_data.host.weapon.loadouts[0].bungienet_user = player_data->host_partial.bungienet_user;
		update_increment = true;
	}

	if (csmemcmp(&player->player_data.host.armor.loadouts[0].colors, &player_data->host_partial.colors, sizeof(c_static_array<rgb_color, k_color_type_count>)))
	{
		player->player_data.host.armor.loadouts[0].colors = player_data->host_partial.colors;
		update_increment = true;
	}

	if (csmemcmp(&player->player_data.host.armor.loadouts[0].armors, &player_data->host_partial.armors, sizeof(c_static_array<uns8, k_armor_type_count>)))
	{
		player->player_data.host.armor.loadouts[0].armors = player_data->host_partial.armors;
		update_increment = true;
	}

	if (csmemcmp(&player->player_data.host.weapon.loadouts[0].consumables, &player_data->host_partial.consumables, sizeof(c_static_array<int8, 4>)))
	{
		player->player_data.host.weapon.loadouts[0].consumables = player_data->host_partial.consumables;
		update_increment = true;
	}

	if (csmemcmp(&player->player_data.host.appearance.emblem_info, &player_data->host_partial.emblem, sizeof(s_emblem_info)))
	{
		player->player_data.host.appearance.emblem_info = player_data->host_partial.emblem;
		update_increment = true;
	}

	if (player->player_voice_settings != player_voice_settings)
	{
		player->player_voice_settings = player_voice_settings;
		update_increment = true;
	}

	if (update_increment)
	{
		increment_update();
	}
}

