#pragma once

#include "camera/director.hpp"

struct c_editor_director : public c_director
{
	long __unknown168;
	bool m_scripted;
	bool __unknown16D;

	byte pad[0x2];
	byte unused[0xC];

	void ctor(long user_index)
	{
		DECLFUNC(0x00727EA0, void, __thiscall, c_director*, long)(this, user_index);
	}
};
static_assert(sizeof(c_editor_director) == 0x160);