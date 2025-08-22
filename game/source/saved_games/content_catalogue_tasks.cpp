#include "saved_games/content_catalogue_tasks.hpp"

c_content_enumeration_overlapped_task::c_content_enumeration_overlapped_task(const char* file, int32 line) :
	c_overlapped_task(file, line),
	m_enumeration_handle(NULL),
	m_content_catalogue(NULL),
	m_controller_index(k_no_controller),
	m_content_origin(_content_origin_invalid),
	m_device_id(0xCACACACA),
	m_xcontent_data_array(NULL),
	m_user_content_data_count(0),
	m_dlc_content_data_count(0)
{
	//DECLFUNC(0x0074C5E0, void, __thiscall, c_content_enumeration_overlapped_task*, const char*, int32)(this, file, line);
}

c_content_item_overlapped_task::c_content_item_overlapped_task(const char* file, int32 line) :
	c_overlapped_task(file, line),
	m_task_type(_content_item_task_type_none),
	m_content_item(NULL),
	m_content_creation_flags(0),
	m_maximum_content_size_bytes(0)
{
	//DECLFUNC(0x0074C640, void, __thiscall, c_content_item_overlapped_task*, const char*, int32)(this, file, line);
}

//.text:0074C680 ; 
//.text:0074C690 ; public: virtual void* c_content_enumeration_overlapped_task::`vector deleting destructor'(unsigned int)
//.text:0074C6C0 ; public: virtual void* c_content_item_overlapped_task::`vector deleting destructor'(unsigned int)

void c_content_enumeration_overlapped_task::complete()
{
	INVOKE_CLASS_MEMBER(0x0074C6F0, c_content_enumeration_overlapped_task, complete);
}

void c_content_item_overlapped_task::complete()
{
	INVOKE_CLASS_MEMBER(0x0074C730, c_content_item_overlapped_task, complete);
}

void c_content_enumeration_overlapped_task::failure(uns32 calling_result, uns32 overlapped_error, uns32 overlapped_extended_error)
{
	INVOKE_CLASS_MEMBER(0x0074C770, c_content_enumeration_overlapped_task, failure, calling_result, overlapped_error, overlapped_extended_error);
}

void c_content_item_overlapped_task::failure(uns32 calling_result, uns32 overlapped_error, uns32 overlapped_extended_error)
{
	INVOKE_CLASS_MEMBER(0x0074C7A0, c_content_item_overlapped_task, failure, calling_result, overlapped_error, overlapped_extended_error);
}

const char* c_content_enumeration_overlapped_task::get_context_string() const
{
	return INVOKE_CLASS_MEMBER(0x0074C7F0, c_content_enumeration_overlapped_task, get_context_string);

	//return "XContentCreateEnumerator";
}

const char* c_content_item_overlapped_task::get_context_string() const
{
	return INVOKE_CLASS_MEMBER(0x0074C800, c_content_item_overlapped_task, get_context_string);

	//const char* result = "<unknown content item task type>";
	//switch (m_task_type)
	//{
	//case _content_item_task_type_create:
	//{
	//	result = "XContentCreateEx";
	//}
	//break;
	//case _content_item_task_type_delete:
	//{
	//	result = "XContentDelete";
	//}
	//break;
	//case _content_item_task_type_flush:
	//{
	//	result = "XContentFlush";
	//}
	//break;
	//case _content_item_task_type_close:
	//{
	//	result = "XContentClose";
	//}
	//break;
	//}
	//
	//return result;
}

//.text:0074C840 ; 
//.text:0074C850 ; 

bool c_content_enumeration_overlapped_task::is_result_successful(uns32 calling_result, uns32 overlapped_error, uns32 overlapped_extended_error)
{
	return INVOKE_CLASS_MEMBER(0x0074C880, c_content_enumeration_overlapped_task, is_result_successful, calling_result, overlapped_error, overlapped_extended_error);
}

//.text:0074C8C0 ; 
//.text:0074C8D0 ; 
//.text:0074C8F0 ; 
//.text:0074C910 ; 
//.text:0074C920 ; 

uns32 c_content_enumeration_overlapped_task::start(void* overlapped)
{
	return INVOKE_CLASS_MEMBER(0x0074C940, c_content_enumeration_overlapped_task, start, overlapped);
}

uns32 c_content_item_overlapped_task::start(void* overlapped)
{
	return INVOKE_CLASS_MEMBER(0x0074C950, c_content_item_overlapped_task, start, overlapped);
}

//.text:0074C9B0 ; 
//.text:0074C9C0 ; 
//.text:0074C9D0 ; 
//.text:0074C9E0 ; 

void c_content_enumeration_overlapped_task::success(uns32 return_result)
{
	INVOKE_CLASS_MEMBER(0x0074C9F0, c_content_enumeration_overlapped_task, success, return_result);
}

void c_content_item_overlapped_task::success(uns32 return_result)
{
	INVOKE_CLASS_MEMBER(0x0074CA90, c_content_item_overlapped_task, success, return_result);
}


