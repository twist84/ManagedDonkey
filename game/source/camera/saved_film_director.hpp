#pragma once

#include "camera/observer_director.hpp"

struct c_saved_film_director :
	public c_observer_director
{
	bool __unknown158;
	bool __unknown159;

	uint8 pad[0x2];
	uint8 unused[0x4];

	void constructor(long user_index)
	{
		INVOKE_CLASS_MEMBER(0x007276C0, c_saved_film_director, constructor, user_index);
	}
};
static_assert(sizeof(c_saved_film_director) == 0x160);

