#pragma once

#include "cseries/cseries.hpp"

union network_message_converter_t
{
	void const* message_storage;
	struct s_network_message_ping const* message_ping;
	struct s_network_message_pong const* message_pong;
	struct s_network_message_broadcast_search const* message_broadcast_search;
	struct s_network_message_broadcast_reply const* message_broadcast_reply;
	struct s_network_message_connect_request const* message_connect_request;
	struct s_network_message_connect_refuse const* message_connect_refuse;
	struct s_network_message_connect_establish const* message_connect_establish;
	struct s_network_message_connect_closed const* message_connect_closed;
	struct s_network_message_join_request const* message_join_request;
	struct s_network_message_peer_connect const* message_peer_connect;
	struct s_network_message_join_abort const* message_join_abort;
	struct s_network_message_join_refuse const* message_join_refuse;
	struct s_network_message_leave_session const* message_leave_session;
	struct s_network_message_leave_acknowledge const* message_leave_acknowledge;
	struct s_network_message_session_disband const* message_session_disband;
	struct s_network_message_session_boot const* message_session_boot;
	struct s_network_message_host_decline const* message_host_decline;
	struct s_network_message_peer_establish const* message_peer_establish;
	struct s_network_message_time_synchronize const* message_time_synchronize;
	struct s_network_message_membership_update const* message_membership_update;
	struct s_network_message_peer_properties const* message_peer_properties;
	struct s_network_message_delegate_leadership const* message_delegate_leadership;
	struct s_network_message_boot_machine const* message_boot_machine;
	struct s_network_message_player_add const* message_player_add;
	struct s_network_message_player_refuse const* message_player_refuse;
	struct s_network_message_player_remove const* message_player_remove;
	struct s_network_message_player_properties const* message_player_properties;
	struct s_network_message_parameters_update const* message_parameters_update;
	struct s_network_message_parameters_request const* message_parameters_request;
	struct s_network_message_view_establishment const* message_view_establishment;
	struct s_network_message_player_acknowledge const* message_player_acknowledge;
	struct s_network_message_synchronous_update const* message_synchronous_update;
	struct s_network_message_synchronous_playback_control const* message_synchronous_playback_control;
	struct s_network_message_synchronous_actions const* message_synchronous_actions;
	struct s_network_message_synchronous_acknowledge const* message_synchronous_acknowledge;
	struct s_network_message_synchronous_gamestate const* message_synchronous_gamestate;
	struct s_network_message_distributed_game_results const* message_distributed_game_results;
	struct s_network_message_test const* message_test;
	struct s_network_message_text_chat const* message_text_chat;
};

struct c_network_channel;
struct c_network_link;
struct c_network_message_type_collection;
struct c_network_message_gateway;
struct c_network_observer;
struct c_network_session_manager;
struct transport_address;
enum e_network_message_type;
struct c_network_message_handler
{
public:
	void __cdecl handle_out_of_band_message(transport_address const* address, e_network_message_type message_type, long message_storage_size, void const* message_storage);
	//void __cdecl handle_channel_message(c_network_channel* channel, e_network_message_type message_type, long, void const* message);

	void __cdecl handle_ping(transport_address const* address, s_network_message_ping const* message);
	void __cdecl handle_pong(transport_address const* address, s_network_message_pong const* message);
	void __cdecl handle_broadcast_search(transport_address const* address, s_network_message_broadcast_search const* message);
	void __cdecl handle_broadcast_reply(transport_address const* address, s_network_message_broadcast_reply const* message);
	void __cdecl handle_connect_request(transport_address const* address, s_network_message_connect_request const* message);
	void __cdecl handle_connect_refuse(c_network_channel* channel, s_network_message_connect_refuse const* message);
	void __cdecl handle_connect_establish(c_network_channel* channel, s_network_message_connect_establish const* message);
	void __cdecl handle_connect_closed(c_network_channel* channel, s_network_message_connect_closed const* message);
	void __cdecl handle_join_request(transport_address const* address, s_network_message_join_request const* message);
	void __cdecl handle_peer_connect(transport_address const* address, s_network_message_peer_connect const* message);
	void __cdecl handle_join_abort(transport_address const* address, s_network_message_join_abort const* message);
	void __cdecl handle_join_refuse(transport_address const* address, s_network_message_join_refuse const* message);
	void __cdecl handle_leave_session(transport_address const* address, s_network_message_leave_session const* message);
	void __cdecl handle_leave_acknowledge(transport_address const* address, s_network_message_leave_acknowledge const* message);
	void __cdecl handle_session_disband(transport_address const* address, s_network_message_session_disband const* message);
	void __cdecl handle_session_boot(transport_address const* address, s_network_message_session_boot const* message);
	void __cdecl handle_host_decline(c_network_channel* channel, s_network_message_host_decline const* message);
	void __cdecl handle_peer_establish(c_network_channel* channel, s_network_message_peer_establish const* message);
	void __cdecl handle_time_synchronize(transport_address const* address, s_network_message_time_synchronize const* message);
	void __cdecl handle_membership_update(c_network_channel* channel, s_network_message_membership_update const* message);
	void __cdecl handle_peer_properties(c_network_channel* channel, s_network_message_peer_properties const* message);
	void __cdecl handle_delegate_leadership(c_network_channel* channel, s_network_message_delegate_leadership const* message);
	void __cdecl handle_boot_machine(c_network_channel* channel, s_network_message_boot_machine const* message);
	void __cdecl handle_player_add(c_network_channel* channel, s_network_message_player_add const* message);
	void __cdecl handle_player_refuse(c_network_channel* channel, s_network_message_player_refuse const* message);
	void __cdecl handle_player_remove(c_network_channel* channel, s_network_message_player_remove const* message);
	void __cdecl handle_player_properties(c_network_channel* channel, s_network_message_player_properties const* message);
	void __cdecl handle_parameters_update(c_network_channel* channel, s_network_message_parameters_update const* message);
	void __cdecl handle_parameters_request(c_network_channel* channel, s_network_message_parameters_request const* message);
	void __cdecl handle_view_establishment(c_network_channel* channel, dword a2, s_network_message_view_establishment const* message);
	void __cdecl handle_player_acknowledge(c_network_channel* channel, s_network_message_player_acknowledge const* message);
	void __cdecl handle_synchronous_update(c_network_channel* channel, s_network_message_synchronous_update const* message);
	void __cdecl handle_synchronous_playback_control(c_network_channel* channel, dword a2, s_network_message_synchronous_playback_control const* message);
	void __cdecl handle_synchronous_actions(c_network_channel* channel, s_network_message_synchronous_actions const* message);
	void __cdecl handle_synchronous_acknowledge(c_network_channel* channel, dword a2, s_network_message_synchronous_acknowledge const* message);
	void __cdecl handle_synchronous_gamestate(c_network_channel* channel, s_network_message_synchronous_gamestate const* message, long a3, void const* a4);
	void __cdecl handle_distributed_game_results(c_network_channel* channel, s_network_message_distributed_game_results const* message);
	void __cdecl handle_text_chat(transport_address const* address, s_network_message_text_chat const* message);

//protected:
	bool m_initialized;
	c_network_link* m_link;
	c_network_message_type_collection* m_message_types;
	c_network_message_gateway* m_message_gateway;
	c_network_observer* m_observer;
	c_network_session_manager* m_session_manager;
};
static_assert(sizeof(c_network_message_handler) == 0x18);

