#pragma once

#include "cseries/cseries.hpp"

#pragma pack(push, 1)
struct s_motd_data
{
	static int32 const k_chunk_type = 'motd';
	static int32 const k_chunk_major_version = 1;
	//static int32 const k_chunk_minor_version = 0;

	int32 message_length;
	c_static_string<511> message;
	byte __data[2721 - sizeof(message_length) - sizeof(message)];
};
COMPILE_ASSERT(sizeof(s_motd_data) == 2721);
#pragma pack(pop)
