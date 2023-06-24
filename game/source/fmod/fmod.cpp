#include "fmod/fmod.hpp"

#include "cseries/cseries.hpp"

void __cdecl fmod_initialize()
{
	INVOKE(0x0064E190, fmod_initialize);
}

void __cdecl fmod_terminate()
{
	INVOKE(0x00652EE0, fmod_terminate);
}