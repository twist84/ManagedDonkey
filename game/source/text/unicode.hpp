#pragma once

#include <stdarg.h>


//extern int ustrcmp(wchar_t const*, wchar_t const*);
//extern unsigned int ustrlen(wchar_t const *);

extern unsigned int ustrnlen(wchar_t const*, int32);

//extern wchar_t const * ustrchr(wchar_t const *,wchar_t);
//extern int ustrcoll(wchar_t const *,wchar_t const *);
//extern unsigned int ustrcspn(wchar_t const *,wchar_t const *);

extern wchar_t* ustrnzcat(wchar_t*, wchar_t const*, int32);

extern int ustrncmp(wchar_t const *,wchar_t const *,int32);
//extern wchar_t * ustrncpy(wchar_t *,wchar_t const *,int32);

extern wchar_t* ustrnzcpy(wchar_t*, wchar_t const*, int32);

//extern wchar_t const * ustrpbrk(wchar_t const *,wchar_t const *);
//extern wchar_t const * ustrrchr(wchar_t const *,wchar_t);
//extern unsigned int ustrspn(wchar_t const *,wchar_t const *);
//extern wchar_t const * ustrstr(wchar_t const *,wchar_t const *);
//extern wchar_t * ustrtok(wchar_t *,wchar_t const *);
//extern unsigned int ustrxfrm(wchar_t *,wchar_t const *,int32);
//extern wchar_t * ustrnlwr(wchar_t *,int32);
//extern wchar_t * ustrnupr(wchar_t *,int32);
extern int __cdecl ustricmp(wchar_t const *,wchar_t const *);
extern int __cdecl ustrnicmp(wchar_t const *,wchar_t const *,int32);
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
//extern wchar_t ufgetc(struct _iobuf *);
//extern wchar_t ufputc(wchar_t,struct _iobuf *);
//extern wchar_t uungetc(wchar_t,struct _iobuf *);
//extern wchar_t * ufgets(wchar_t *,int,struct _iobuf *);
//extern int ufputs(wchar_t const *,struct _iobuf *);
//extern wchar_t * ugets(wchar_t *);
//extern int uputs(wchar_t const *);
//extern int ufprintf(struct _iobuf *,wchar_t const *,...);
//extern int uprintf(wchar_t const *,...);
//extern int usnprintf(wchar_t *,int32,wchar_t const *,...);

extern int usnzprintf(wchar_t *,int32,wchar_t const *,...);

//extern int uvfprintf(struct _iobuf *,wchar_t const *,char *);
//extern int uvprintf(wchar_t const *,char *);

extern int uvsnzprintf(wchar_t* string, int32 size, wchar_t const* format, va_list list);
extern wchar_t const* uvsnzappend(wchar_t* string, int32 size, wchar_t const* format, va_list list);
extern wchar_t const* usnzappend(wchar_t* string, int32 size, wchar_t const* format, ...);

//extern struct _iobuf * ufdopen(int,wchar_t const *);
//extern struct _iobuf * ufopen(wchar_t const *,wchar_t const *);
//extern int ufclose(struct _iobuf *);
//extern struct _iobuf * ufreopen(wchar_t const *,wchar_t const *,struct _iobuf *);
//extern void uperror(wchar_t const *);
//extern int uremove(wchar_t const *);
//extern wchar_t * utmpnam(wchar_t *);
//extern int32 ustrtol(wchar_t const *,wchar_t * *,int);
//extern uns32 ustrtoul(wchar_t const *,wchar_t * *,int);
//extern real64 ustrtod(wchar_t const *,wchar_t * *);
//extern wchar_t * ustrftime_tm(wchar_t *,int32,wchar_t const *,struct tm const *);
//extern wchar_t * ustrftime(wchar_t *,int32,wchar_t const *,int64);
//extern int uatoi(wchar_t const *);

extern void wchar_string_to_ascii_string(wchar_t const* source, char* destination, int32 source_length, int32* destination_length);

//extern struct utf32 ascii_string_to_utf32_characters(char const *,struct s_escape_table const *,char const * *,int32 *,struct utf32 *,int32,int32 *);
//extern struct utf32 wchar_string_to_utf32_characters(wchar_t const *,struct s_escape_table const *,wchar_t const * *,int32 *,struct utf32 *,int32,int32 *);
//extern void ascii_string_to_utf32_string(char const *,struct s_escape_table const *,struct utf32 *,int32,int32 *);
//extern void wchar_string_to_utf32_string(wchar_t const *,struct s_escape_table const *,struct utf32 *,int32,int32 *);

extern void __cdecl ascii_string_to_wchar_string(char const* src, wchar_t* dest, int32 src_len, int32* out_dest_len);
extern wchar_t* __cdecl unicode_byte_swap_wchar_string(wchar_t* string, int32 maximum_count, int32 byte_order);

//extern int32 utf32_character_to_utf16_string(struct utf32,struct utf16 *,int32);
//extern struct utf32 utf16_string_to_utf32_character(struct utf16 const *,struct utf16 const * *);
//extern bool utf32_in_list(struct utf32,struct s_utf32_range const *,int32);
//extern bool utf32_isspace(struct utf32);
//extern bool utf32_is_id_start(struct utf32);
//extern bool utf32_is_id_continue(struct utf32);
//extern bool utf32_isalpha(struct utf32);
//extern bool utf32_isprint(struct utf32);
//extern bool utf32_ismonochrome(struct utf32);
//extern bool utf32_can_line_break(struct utf32,struct utf32);
//extern unsigned int utf32_strlen(struct utf32 const *);
//extern bool is_private_use_character(wchar_t);
//extern void utf8_string_to_wchar_string(struct utf8 const *,wchar_t *,int32,int32 *);
//extern void wchar_string_to_utf8_string(wchar_t const *,struct utf8 *,int32,int32 *);
//extern void string_to_utf32_string<char,8>(char const *,struct s_escape_table const *,struct utf32 *,struct utf32 (*)(char const *,struct s_escape_table const *,char const * *,int32 *,struct utf32 *,int32,int32 *),int32,int32 *);
//extern void string_to_utf32_string<wchar_t,8>(wchar_t const *,struct s_escape_table const *,struct utf32 *,struct utf32 (*)(wchar_t const *,struct s_escape_table const *,wchar_t const * *,int32 *,struct utf32 *,int32,int32 *),int32,int32 *);

template<int32 k_buffer_size>
struct c_static_wchar_string
{
public:
	c_static_wchar_string() :
		m_string{}
	{
		clear();
	}

	c_static_wchar_string(wchar_t const* s) :
		c_static_wchar_string()
	{
		set(s);
	}

	void set(wchar_t const* s)
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

	wchar_t const* get_string() const
	{
		return m_string;
	}

	wchar_t const* get_offset(int32 offset) const
	{
		if (VALID_INDEX(offset, length()))
			return &m_string[offset];

		return L"";
	}

	int32 length() const
	{
		return ustrnlen(m_string, k_buffer_size);
	}

	void append(wchar_t const* s)
	{
		ustrnzcat(m_string, s, k_buffer_size);
	}

	void append_line(wchar_t const* s = nullptr)
	{
		if (s != nullptr)
			ustrnzcat(m_string, s, k_buffer_size);
		ustrnzcat(m_string, L"\r\n", k_buffer_size);
	}

	wchar_t const* print(wchar_t const* format, ...)
	{
		va_list list;
		va_start(list, format);

		print_va(format, list);

		va_end(list);

		return m_string;
	}

	wchar_t const* print_line(wchar_t const* format, ...)
	{
		va_list list;
		va_start(list, format);

		print_va(format, list);
		append_line();

		va_end(list);

		return m_string;
	}

	wchar_t const* print_va(wchar_t const* format, va_list list)
	{
		uvsnzprintf(m_string, k_buffer_size, format, list);

		return m_string;
	}

	wchar_t const* append_print(wchar_t const* format, ...)
	{
		va_list list;
		va_start(list, format);

		append_print_va(format, list);
			
		va_end(list);
		return m_string;
	}
	
	wchar_t const* append_print_line(wchar_t const* format, ...)
	{
		va_list list;
		va_start(list, format);

		wchar_t const* result = append_print_va(format, list);
		append_line();

		va_end(list);
		return result;
	}

	wchar_t const* append_print_va(wchar_t const* format, va_list list)
	{
		int32 current_length = length();

		//ASSERT(format);
		//ASSERT(current_length >= 0 && current_length < k_buffer_size);

		uvsnzprintf(m_string + current_length, k_buffer_size - current_length, format, list);

		return m_string;
	}

	bool is_equal(wchar_t const* s, bool case_sensitive) const
	{
		if (case_sensitive)
			return ustrncmp(m_string, s, k_buffer_size) == 0;

		return ustrnicmp(m_string, s, k_buffer_size) == 0;
	}

	bool is_equal(wchar_t const* s) const
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
