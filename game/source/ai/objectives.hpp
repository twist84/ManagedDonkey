#pragma once

#include "cseries/cseries.hpp"
#include "memory/data.hpp"

struct s_objective :
	s_datum_header
{
	dword __unknown4;
	dword __unknown8;
};
static_assert(sizeof(s_objective) == 0xC);

struct s_task_record
{
	byte __data[0x61A80];
};
static_assert(sizeof(s_task_record) == 0x61A80);

struct s_objective;

