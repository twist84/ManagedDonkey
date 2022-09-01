#pragma once

#include "camera/director.hpp"

struct c_game_director : public c_director
{
	byte unused[0x14];

	void ctor(long user_index)
	{
		DECLFUNC(0x007215C0, void, __thiscall, c_director*, long)(this, user_index);
	}
};
static_assert(sizeof(c_game_director) == 0x160);