#pragma once

#include "cseries/async_xoverlapped.hpp"

struct s_content_item;

enum e_content_item_task_type
{
	_content_item_task_type_create = 0,
	_content_item_task_type_delete,
	_content_item_task_type_flush,
	_content_item_task_type_close,

	k_content_item_task_type_count,

	_content_item_task_type_none = NONE,
};

enum e_content_origin
{
	k_content_origin_first = 0,

	_content_origin_user_created = k_content_origin_first,
	_content_origin_downloaded_content,

	k_content_origin_count,
	k_content_origin_last = _content_origin_downloaded_content,

	_content_origin_invalid = NONE,
};


class c_content_item_overlapped_task :
	public c_overlapped_task
{
public:
	e_content_item_task_type m_task_type;
	s_content_item* m_content_item;
	int32 m_content_creation_flags;
	int32 m_maximum_content_size_bytes;
};
static_assert(sizeof(c_content_item_overlapped_task) == 0x20);

class c_content_catalogue;
class c_content_enumeration_overlapped_task :
	public c_overlapped_task
{
public:
	void* m_enumeration_handle;
	c_content_catalogue* m_content_catalogue;
	e_controller_index m_controller_index;
	e_content_origin m_content_origin;
	uns32 m_device_id;
	s_content_item* m_xcontent_data_array;
	int32 m_user_content_data_count;
	int32 m_dlc_content_data_count;
};
static_assert(sizeof(c_content_enumeration_overlapped_task) == 0x30);

