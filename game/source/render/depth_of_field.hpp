#pragma once

#include "cseries/cseries.hpp"

struct s_observer_depth_of_field;
struct s_depth_of_field
{
//public:
	static void __cdecl dispose();
	static void __cdecl dispose_from_old_map();
	//static s_depth_of_field* __cdecl s_depth_of_field::get();
	//bool __cdecl s_depth_of_field::get_current_depth_of_field_parameters(s_observer_depth_of_field const*, real*, real*, real*, real*);
	//static real __cdecl s_depth_of_field::get_depth_of_field_blur_sigma(s_observer_depth_of_field const*);
	//static bool __cdecl s_depth_of_field::get_enabled();
	static void __cdecl initialize();
	static void __cdecl initialize_for_new_map();
	//void __cdecl s_depth_of_field::set_default_values();
	static void __cdecl update();

//private:
	//void __cdecl update_internal();

//protected:
	bool enable;
	bool __unknown1;

	byte __data2[0x2]; // pad?

	real __unknown4;

	// these are linked?
	real __unknown8;
	real __unknownC;
	real depth_of_field;

	real __unknown14;
	real __unknown18;
	long __unknown1C;
	long __unknown20;
	long __unknown24;
	long __unknown28;
	real __unknown2C;
	real __unknown30;

	// these are linked and set from the linked 3 above?
	real __unknown34;
	real __unknown38;
	real __unknown3C;
};
static_assert(sizeof(s_depth_of_field) == 0x40);

