#pragma once

#include "interface/user_interface_data.hpp"

struct c_gui_roster_data : c_gui_ordered_data
{
	struct s_player_row
	{
		long __unknown0;
		long m_player_index;
		s_player_identifier m_player_identifier;
		byte __data10[0x8];
		s_player_configuration m_player_data;
		byte __data1638[0xC];
		long m_peer_index;
		long m_peer_user_index;
		byte __data164C[0x4];
		qword m_party_nonce;
		long m_leader_team_index;
		long m_controller_index;
		byte __data1660[0x8];
		bool m_is_leader;
		bool m_show_teams;
		byte __data[0xE];
	};
	static_assert(sizeof(s_player_row) == 0x1678);

protected:
	long __unknown10C;
	long __unknown110;
	byte __data114[4];
	c_gui_roster_data::s_player_row m_players[16];
	long m_player_count;
	long m_controller_index;
	bool __unknown168A0;
	byte __data168A1[0x7];
};
static_assert(sizeof(c_gui_roster_data) == 0x168A8);
