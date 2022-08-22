#pragma once

#include "camera/director.hpp"

struct c_editor_director : public c_director
{
	long __unknown168;
	bool m_scripted;
	bool __unknown16D;

	byte pad[0x2];
	byte unused[0xC];
};
static_assert(sizeof(c_editor_director) == 0x160);