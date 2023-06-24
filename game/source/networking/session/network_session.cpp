#include "networking/session/network_session.hpp"

#include "interface/c_controller.hpp"
#include "hf2p/hf2p.hpp"
#include "memory/module.hpp"
#include "networking/delivery/network_channel.hpp"
#include "networking/messages/network_message_type_collection.hpp"
#include "networking/messages/network_messages_session_membership.hpp"
#include "networking/session/network_managed_session.hpp"
#include "networking/session/network_observer.hpp"

c_network_session_membership const* c_network_session::get_session_membership() const
{
	ASSERT(established());
	ASSERT(m_session_membership.has_membership());
	ASSERT(m_session_membership.is_peer_valid(m_session_membership.local_peer_index()));
	ASSERT(m_session_membership.is_peer_valid(m_session_membership.host_peer_index()));

	return &m_session_membership;
}

c_network_session_membership* c_network_session::get_session_membership_for_update()
{
	ASSERT(established());
	ASSERT(is_host());
	ASSERT(m_session_membership.has_membership());
	ASSERT(m_session_membership.is_peer_valid(m_session_membership.local_peer_index()));
	ASSERT(m_session_membership.is_peer_valid(m_session_membership.host_peer_index()));

	return &m_session_membership;
}

c_network_session_membership const* c_network_session::get_session_membership_unsafe() const
{
	if (disconnected() || !m_session_membership.has_membership())
		return nullptr;

	ASSERT(m_session_membership.is_peer_valid(m_session_membership.local_peer_index()));
	ASSERT(m_session_membership.is_peer_valid(m_session_membership.host_peer_index()));

	return &m_session_membership;
}

c_network_session_parameters const* c_network_session::get_session_parameters() const
{
	return &m_session_parameters;
}

c_network_session_parameters* c_network_session::get_session_parameters()
{
	//return DECLFUNC(0x0045C2A0, c_network_session_parameters*, __thiscall, c_network_session*)(this);

	return &m_session_parameters;
}

bool c_network_session::join_abort(transport_address const* incoming_address, qword join_nonce)
{
	return DECLFUNC(0x0045CC80, bool, __thiscall, c_network_session*, transport_address const*, qword)(this, incoming_address, join_nonce);
}

long c_network_session::current_local_state() const
{
	return m_local_state;
}

bool c_network_session::disconnected() const
{
	return current_local_state() == _network_session_state_none;
}

bool c_network_session::established() const
{
	return current_local_state() >= _network_session_state_peer_established;
}

bool c_network_session::is_host() const
{
	return current_local_state() == _network_session_state_host_established || current_local_state() == _network_session_state_host_disband;
}

bool c_network_session::is_leader()
{
	return m_session_membership.m_local_peer_index == m_session_membership.m_shared_network_membership.leader_peer_index;
}

s_network_session_player* c_network_session::get_player(long player_index)
{
	ASSERT(!disconnected());

	return &m_session_membership.m_shared_network_membership.players[player_index];
}

e_network_session_mode c_network_session::session_mode() const
{
	return m_session_parameters.m_parameters_internal.session_mode.get();
}

bool __cdecl c_network_session::handle_player_properties(c_network_channel* channel, s_network_message_player_properties const* message)
{
	//return DECLFUNC(0x004DAEC0, bool, __thiscall, c_network_session*, c_network_channel*, s_network_message_player_properties const*)(this, channel, message);

	if (established() && is_host())
	{
		long observer_channel_index = m_observer->observer_channel_find_by_network_channel(m_session_index, channel);
		long peer_index = m_session_membership.get_peer_from_observer_channel(observer_channel_index);
		if (peer_index == NONE || peer_index == m_session_membership.local_peer_index())
		{
			c_console::write_line("networking:session:membership: [%s] player-properties received from invalid peer [#%d]",
				managed_session_get_id_string(m_managed_session_index),
				peer_index);
		}
		else
		{
			long player_index = m_session_membership.get_player_index_from_peer(peer_index);
			if (player_index == NONE)
			{
				c_console::write_line("networking:session:membership: [%s] player-properties received but no player associated with peer [#%d]",
					managed_session_get_id_string(m_managed_session_index),
					peer_index);
			}
			else
			{
				m_session_membership.set_player_properties(player_index,
					message->player_update_number,
					message->controller_index,
					&message->player_data,
					message->player_voice);

				c_console::write_line("networking:session:membership: [%s] player-properties accepted for peer/player [#%d]/[#%d]",
					managed_session_get_id_string(m_managed_session_index),
					peer_index,
					peer_index);
			}

			return true;
		}
	}
	else
	{
		c_console::write_line("networking:session:membership: [%s] player-properties received but not host, can't update players",
			managed_session_get_id_string(m_managed_session_index));
	}

	return false;
}

void update_player_data(s_player_configuration_for_player_properties* player_data)
{
	// #TODO: save `c_player_profile_interface` from a config file
	c_player_profile_interface& player_profile = controller_get(_controller_index0)->m_player_profile;
	s_s3d_player_armor_configuration_loadout& armor_loadout = get_armor_loadout();
	s_s3d_player_weapon_configuration_loadout& weapon_loadout = get_weapon_loadout();

	player_data->host_partial.service_tag = player_profile.desired_service_tag;
	player_data->host_partial.bungienet_user = weapon_loadout.bungienet_user;
	player_data->host_partial.colors = armor_loadout.colors;
	player_data->host_partial.armors = armor_loadout.armors;
	player_data->host_partial.consumables = weapon_loadout.consumables;
}

bool c_network_session::peer_request_player_desired_properties_update(long player_update_number, e_controller_index controller_index, s_player_configuration_from_client const* player_data_from_client, dword player_voice)
{
	ASSERT(controller_index >= 0 && controller_index < k_number_of_controllers);
	ASSERT(player_data_from_client);

	if (!established())
		return false;

	s_player_configuration_for_player_properties player_data = { .client = *player_data_from_client };
	update_player_data(&player_data);

	if (is_host())
	{
		ASSERT(m_session_membership.local_peer_index() >= 0 && m_session_membership.local_peer_index() < k_network_maximum_machines_per_session);

		long player_index = m_session_membership.get_player_index_from_peer(m_session_membership.local_peer_index());
		if (player_index == NONE)
		{
			c_console::write_line("networking:session:membership: [%s] local host requested player-properties does not exist",
				managed_session_get_id_string(m_managed_session_index));

			return false;
		}

		s_network_session_player* player = get_player(player_index);

		c_console::write_line("networking:session:membership: [%s] local host applying player-properties for player [#%d]",
			managed_session_get_id_string(m_managed_session_index),
			player_index);

		ASSERT(player->peer_index == m_session_membership.local_peer_index());
		m_session_membership.set_player_properties(
			player_index,
			player_update_number,
			controller_index,
			&player_data,
			player_voice);
	}
	else
	{
		c_console::write_line("networking:session:membership: [%s] sending player-properties request",
			managed_session_get_id_string(m_managed_session_index));

		s_network_message_player_properties message{};
		csmemset(&message, 0, sizeof(s_network_message_player_properties));

		managed_session_get_id(m_managed_session_index, &message.session_id);
		message.player_update_number = player_update_number;
		message.controller_index = controller_index;
		message.player_data = player_data;
		message.player_voice = player_voice;

		long observer_channel_index = m_session_membership.m_local_peers[m_session_membership.host_peer_index()].channel_index;
		m_observer->observer_channel_send_message(m_session_index, observer_channel_index, false, _network_message_player_properties, sizeof(message), &message);
	}

	return true;
}

bool __fastcall network_session_peer_request_player_desired_properties_update(c_network_session* _this, void* usused, long player_update_number, e_controller_index controller_index, s_player_configuration_from_client const* player_data_from_client, dword player_voice)
{
	return _this->peer_request_player_desired_properties_update(player_update_number, controller_index, player_data_from_client, player_voice);
}
HOOK_DECLARE_CALL(0x00437C8C, network_session_peer_request_player_desired_properties_update);

