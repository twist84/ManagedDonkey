#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"
#include "multithreading/message_queue.hpp"

struct c_decal_system :
	s_datum_header
{
	long tag_index;
	long __unknown8;
	long __unknownC;
	long __unknown10;
	long __unknown14;
	long __unknown18;
	long __unknown1C;
	long __unknown20;
	long __unknown24;
	long __unknown28;
	long __unknown2C;
	long __unknown30;
	long __unknown34;
	long __unknown38;
	long __unknown3C;
	long __unknown40;
	long __unknown44;
	long __unknown48;
	long __unknown4C;
	byte __unknown50[0x308];
};
static_assert(sizeof(c_decal_system) == 0x358);

struct s_decal_counts
{
	byte __data[0x20];
};
static_assert(sizeof(s_decal_counts) == 0x20);

struct c_decal :
	s_datum_header
{
	byte __data[0x12E];
};
static_assert(sizeof(c_decal) == 0x130);

union c_decal_message_union
{
	byte __data[0x8];
};

struct c_decal_message_queue
{
	t_message_queue<c_decal_message_union, 256> m_queue;
};
static_assert(sizeof(c_decal_message_queue) == 0x824);

extern void __cdecl sub_6948C0(long a1);

