#pragma once

#include "cseries/async_xoverlapped.hpp"

struct s_content_item;
struct c_content_item_overlapped_task : public c_overlapped_task
{
	long m_task_type;
	s_content_item* m_content_item;
	byte __data18[0x8];
};
static_assert(sizeof(c_content_item_overlapped_task) == 0x20);

struct c_content_catalogue;
struct c_content_enumeration_overlapped_task : public c_overlapped_task
{
	byte __data10[0x4];
	c_content_catalogue* m_content_catalogue;
	byte __data18[0x18];
};
static_assert(sizeof(c_content_enumeration_overlapped_task) == 0x30);
