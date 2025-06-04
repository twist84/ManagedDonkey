#pragma once

#include "camera/observer_director.hpp"

class c_saved_film_director :
	public c_observer_director
{
public:
	void constructor(int32 user_index)
	{
		INVOKE_CLASS_MEMBER(0x007276C0, c_saved_film_director, constructor, user_index);
	}

//protected:
	bool __unknown158;
	bool __unknown159;

	byte pad[0x2];
	byte unused[0x4];
};
static_assert(sizeof(c_saved_film_director) == 0x160);

