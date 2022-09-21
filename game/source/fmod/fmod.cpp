#include "fmod/fmod.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"

void __cdecl fmod_initialize()
{
	FUNCTION_BEGIN(true);

	INVOKE(0x0064E190, fmod_initialize);
}

void __cdecl fmod_terminate()
{
	FUNCTION_BEGIN(true);

	INVOKE(0x00652EE0, fmod_terminate);
}