#pragma once

#include "cseries/async_xoverlapped.hpp"

class c_content_catalogue;
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

class c_content_enumeration_overlapped_task :
	public c_overlapped_task
{
public:
	virtual ~c_content_enumeration_overlapped_task() = default;
	virtual const char* get_context_string() const override;
	virtual uns32 start(void* overlapped) override;
	virtual void success(uns32 return_result) override;
	virtual void failure(uns32 calling_result, uns32 overlapped_error, uns32 overlapped_extended_error) override;
	virtual void complete() override;
	virtual bool is_result_successful(uns32 calling_result, uns32 overlapped_error, uns32 overlapped_extended_error) override;

public:
	c_content_enumeration_overlapped_task(const char* file, int32 line);

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
COMPILE_ASSERT(sizeof(c_content_enumeration_overlapped_task) == 0x30);

class c_content_item_overlapped_task :
	public c_overlapped_task
{
public:
	virtual ~c_content_item_overlapped_task() = default;
	virtual const char* get_context_string() const override;
	virtual uns32 start(void* overlapped) override;
	virtual void success(uns32 return_result) override;
	virtual void failure(uns32 calling_result, uns32 overlapped_error, uns32 overlapped_extended_error) override;
	virtual void complete() override;

public:
	c_content_item_overlapped_task(const char* file, int32 line);

public:
	e_content_item_task_type m_task_type;
	s_content_item* m_content_item;
	int32 m_content_creation_flags;
	int32 m_maximum_content_size_bytes;
};
COMPILE_ASSERT(sizeof(c_content_item_overlapped_task) == 0x20);

