#pragma once

#include <windows.h>

// ==============================================================================================

// from `https://github.com/pnill/cartographer/blob/f280be10d04975fd48dda115c2eab3a239ffca4d/xlive/xliveless.cpp#L69-L111`

// Xbox-specific Overlapped

typedef struct _XOVERLAPPED             XOVERLAPPED, * PXOVERLAPPED;

typedef
VOID
(WINAPI* PXOVERLAPPED_COMPLETION_ROUTINE)(
	DWORD dwErrorCode,
	DWORD dwNumberOfBytesTransfered,
	DWORD pOverlapped
	);


typedef struct _XOVERLAPPED {
	ULONG_PTR                           InternalLow;
	ULONG_PTR                           InternalHigh;
	ULONG_PTR                           InternalContext;
	HANDLE                              hEvent;
	PXOVERLAPPED_COMPLETION_ROUTINE     pCompletionRoutine;
	DWORD_PTR                           dwCompletionContext;
	DWORD                               dwExtendedError;
} XOVERLAPPED, * PXOVERLAPPED;

extern DWORD WINAPI XGetOverlappedExtendedError(PXOVERLAPPED pOverlapped);
extern DWORD WINAPI XGetOverlappedResult(PXOVERLAPPED pOverlapped, LPDWORD pResult, BOOL bWait);

// ==============================================================================================

extern bool __cdecl get_clipboard_as_text(char* buf, long len);
extern bool __cdecl set_clipboard_as_text(char* buf, long len);

extern long XShowKeyboardUI(
	long controller_index,
	unsigned long character_flags,
	wchar_t const* default_text,
	wchar_t const* title_text,
	wchar_t const* description_text,
	wchar_t* result_text,
	unsigned long maximum_character_count,
	void* overlapped
);

extern void display_notification(wchar_t const* tip = L"Notification Tip", wchar_t const* info = L"Notification Message", wchar_t const* info_title = L"Notification Title");

