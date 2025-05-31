#pragma once

#include <stdarg.h>


//extern int ustrcmp(const wchar_t*, const wchar_t*);
//extern unsigned int ustrlen(const wchar_t*);

extern unsigned int ustrnlen(const wchar_t*, int32);

//extern const wchar_t* ustrchr(const wchar_t*,wchar_t);
//extern int ustrcoll(const wchar_t*,const wchar_t*);
//extern unsigned int ustrcspn(const wchar_t*,const wchar_t*);

extern wchar_t* ustrnzcat(wchar_t*, const wchar_t*, int32);

extern int ustrncmp(const wchar_t*,const wchar_t*,int32);
//extern wchar_t* ustrncpy(wchar_t*,const wchar_t*,int32);

extern wchar_t* ustrnzcpy(wchar_t*, const wchar_t*, int32);

//extern const wchar_t* ustrpbrk(const wchar_t*,const wchar_t*);
//extern const wchar_t* ustrrchr(const wchar_t*,wchar_t);
//extern unsigned int ustrspn(const wchar_t*,const wchar_t*);
//extern const wchar_t* ustrstr(const wchar_t*,const wchar_t*);
//extern wchar_t* ustrtok(wchar_t*,const wchar_t*);
//extern unsigned int ustrxfrm(wchar_t*,const wchar_t*,int32);
//extern wchar_t* ustrnlwr(wchar_t*,int32);
extern wchar_t* ustrnupr(wchar_t* string, int32 count);
extern int __cdecl ustricmp(const wchar_t*,const wchar_t*);
extern int __cdecl ustrnicmp(const wchar_t*,const wchar_t*,int32);
//extern int uisalpha(wchar_t);
//extern int uisupper(wchar_t);
//extern int uislower(wchar_t);
//extern int uisdigit(wchar_t);
//extern int uisxdigit(wchar_t);
//extern int uisspace(wchar_t);
//extern int uispunct(wchar_t);
//extern int uisalnum(wchar_t);
//extern int uisprint(wchar_t);
//extern int uisgraph(wchar_t);
//extern int uiscntrl(wchar_t);
//extern wchar_t utoupper(wchar_t);
//extern int utolower(wchar_t);
//extern wchar_t ufgetc(_iobuf*);
//extern wchar_t ufputc(wchar_t,_iobuf*);
//extern wchar_t uungetc(wchar_t,_iobuf*);
//extern wchar_t* ufgets(wchar_t*,int,_iobuf*);
//extern int ufputs(const wchar_t*,_iobuf*);
//extern wchar_t* ugets(wchar_t*);
//extern int uputs(const wchar_t*);
//extern int ufprintf(_iobuf*,const wchar_t*,...);
//extern int uprintf(const wchar_t*,...);
//extern int usnprintf(wchar_t*,int32,const wchar_t*,...);

extern int usnzprintf(wchar_t*,int32,const wchar_t*,...);

//extern int uvfprintf(_iobuf*,const wchar_t*,char*);
//extern int uvprintf(const wchar_t*,char*);

extern int uvsnzprintf(wchar_t* string, int32 size, const wchar_t* format, va_list list);
extern const wchar_t* uvsnzappend(wchar_t* string, int32 size, const wchar_t* format, va_list list);
extern const wchar_t* usnzappend(wchar_t* string, int32 size, const wchar_t* format, ...);

//extern _iobuf* ufdopen(int,const wchar_t*);
//extern _iobuf* ufopen(const wchar_t*,const wchar_t*);
//extern int ufclose(_iobuf*);
//extern _iobuf* ufreopen(const wchar_t*,const wchar_t*,_iobuf*);
//extern void uperror(const wchar_t*);
//extern int uremove(const wchar_t*);
//extern wchar_t* utmpnam(wchar_t*);
//extern int32 ustrtol(const wchar_t*,wchar_t**,int);
//extern uns32 ustrtoul(const wchar_t*,wchar_t**,int);
//extern real64 ustrtod(const wchar_t*,wchar_t**);
//extern wchar_t* ustrftime_tm(wchar_t*,int32,const wchar_t*,const tm*);
//extern wchar_t* ustrftime(wchar_t*,int32,const wchar_t*,int64);
//extern int uatoi(const wchar_t*);

extern void wchar_string_to_ascii_string(const wchar_t* source, char* destination, int32 source_length, int32* destination_length);

//extern utf32 ascii_string_to_utf32_characters(const char*,const s_escape_table*,const char**,int32*,utf32*,int32,int32*);
//extern utf32 wchar_string_to_utf32_characters(const wchar_t*,const s_escape_table*,const wchar_t**,int32*,utf32*,int32,int32*);
//extern void ascii_string_to_utf32_string(const char*,const s_escape_table*,utf32*,int32,int32*);
//extern void wchar_string_to_utf32_string(const wchar_t*,const s_escape_table*,utf32*,int32,int32*);

extern void __cdecl ascii_string_to_wchar_string(const char* src, wchar_t* dest, int32 src_len, int32* out_dest_len);
extern wchar_t* __cdecl unicode_byte_swap_wchar_string(wchar_t* string, int32 maximum_count, int32 byte_order);

//extern int32 utf32_character_to_utf16_string(utf32,utf16*,int32);
//extern utf32 utf16_string_to_utf32_character(const utf16*,const utf16**);
//extern bool utf32_in_list(utf32,const s_utf32_range*,int32);
//extern bool utf32_isspace(utf32);
//extern bool utf32_is_id_start(utf32);
//extern bool utf32_is_id_continue(utf32);
//extern bool utf32_isalpha(utf32);
//extern bool utf32_isprint(utf32);
//extern bool utf32_ismonochrome(utf32);
//extern bool utf32_can_line_break(utf32,utf32);
//extern unsigned int utf32_strlen(const utf32*);
//extern bool is_private_use_character(wchar_t);
//extern void utf8_string_to_wchar_string(const utf8*,wchar_t*,int32,int32*);
//extern void wchar_string_to_utf8_string(const wchar_t*,utf8*,int32,int32*);
//extern void string_to_utf32_string<char,8>(const char*,const s_escape_table*,utf32*,utf32 (*)(const char*,const s_escape_table*,const char**,int32*,utf32*,int32,int32*),int32,int32*);
//extern void string_to_utf32_string<wchar_t,8>(const wchar_t*,const s_escape_table*,utf32*,utf32 (*)(const wchar_t*,const s_escape_table*,const wchar_t**,int32*,utf32*,int32,int32*),int32,int32*);

template<int32 k_buffer_size>
struct c_static_wchar_string
{
public:
	c_static_wchar_string() :
		m_string{}
	{
		clear();
	}

	c_static_wchar_string(const wchar_t* s) :
		c_static_wchar_string()
	{
		set(s);
	}

	void set(const wchar_t* s)
	{
		ustrnzcpy(m_string, s, k_buffer_size);
	}

	void clear()
	{
		*m_string = 0;
	}

	bool is_empty()
	{
		return !m_string[0];
	}

	const wchar_t* get_string() const
	{
		return m_string;
	}

	const wchar_t* get_offset(int32 offset) const
	{
		if (VALID_INDEX(offset, length()))
			return &m_string[offset];

		return L"";
	}

	int32 length() const
	{
		return ustrnlen(m_string, k_buffer_size);
	}

	void append(const wchar_t* s)
	{
		ustrnzcat(m_string, s, k_buffer_size);
	}

	void append_line(const wchar_t* s = nullptr)
	{
		if (s != nullptr)
			ustrnzcat(m_string, s, k_buffer_size);
		ustrnzcat(m_string, L"\r\n", k_buffer_size);
	}

	const wchar_t* print(const wchar_t* format, ...)
	{
		va_list list;
		va_start(list, format);

		print_va(format, list);

		va_end(list);

		return m_string;
	}

	const wchar_t* print_line(const wchar_t* format, ...)
	{
		va_list list;
		va_start(list, format);

		print_va(format, list);
		append_line();

		va_end(list);

		return m_string;
	}

	const wchar_t* print_va(const wchar_t* format, va_list list)
	{
		uvsnzprintf(m_string, k_buffer_size, format, list);

		return m_string;
	}

	const wchar_t* append_print(const wchar_t* format, ...)
	{
		va_list list;
		va_start(list, format);

		append_print_va(format, list);
			
		va_end(list);
		return m_string;
	}
	
	const wchar_t* append_print_line(const wchar_t* format, ...)
	{
		va_list list;
		va_start(list, format);

		const wchar_t* result = append_print_va(format, list);
		append_line();

		va_end(list);
		return result;
	}

	const wchar_t* append_print_va(const wchar_t* format, va_list list)
	{
		int32 current_length = length();

		//ASSERT(format);
		//ASSERT(current_length >= 0 && current_length < k_buffer_size);

		uvsnzprintf(m_string + current_length, k_buffer_size - current_length, format, list);

		return m_string;
	}

	bool is_equal(const wchar_t* s, bool case_sensitive) const
	{
		if (case_sensitive)
			return ustrncmp(m_string, s, k_buffer_size) == 0;

		return ustrnicmp(m_string, s, k_buffer_size) == 0;
	}

	bool is_equal(const wchar_t* s) const
	{
		return is_equal(s, true);
	}

	wchar_t* copy_to(wchar_t* s, unsigned int size) const
	{
		if (size > k_buffer_size)
			size = k_buffer_size;

		return ustrnzcpy(s, m_string, size);
	}

protected:
	wchar_t m_string[k_buffer_size];
};
