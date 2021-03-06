#include "stdafx.h"
#include "win32/Win32Utils.h"

/*static*/ bool CSafeHandle::isValid(HANDLE h)
{
	return (h != NULL) && (h != INVALID_HANDLE_VALUE);
}

void CSafeHandle::Deleter::operator()(HANDLE h)
{
	if (isValid(h)) {
		WIN32_EXPECT(::CloseHandle(h));
	}
}

CSafeEventHandle::CSafeEventHandle(BOOL manualReset /*= TRUE*/, BOOL initialState /*= FALSE*/, LPCTSTR name /*= NULL*/)
{
	m_handle.reset(CreateEvent(NULL, manualReset, initialState, name));
	WIN32_EXPECT(m_handle);
}
