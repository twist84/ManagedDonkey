#pragma once

#include "camera/observer_director.hpp"

struct c_saved_film_director : public c_observer_director
{
	bool __unknown158;
	bool __unknown159;

	byte pad[0x2];
	byte unused[0x4];
};
static_assert(sizeof(c_saved_film_director) == 0x160);