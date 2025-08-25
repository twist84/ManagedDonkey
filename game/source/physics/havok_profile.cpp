#include "physics/havok_profile.hpp"

#include "cseries/cseries.hpp"

void havok_profile_display(char* buffer, int32 buffer_size)
{
	csnzappendf(buffer, buffer_size, "%d/%d/%d/%d(active/inactive/fixed/total) entities (%d/%d islands(active/total))|n",
		0,  // $TODO entities active
		0,  // $TODO entities inactive
		0,  // $TODO entities fixed
		0,  // $TODO entities total
		0,  // $TODO islands active
		0); // $TODO islands total
}

