#pragma once

#include "ComUtils.h"

#include <memory>

struct HandleDeleter {
	typedef HANDLE pointer;
	void operator()(HANDLE h) {
		if ((h != NULL) && (h != INVALID_HANDLE_VALUE)) {
			HR_EXPECT(::CloseHandle(h), HRESULT_FROM_WIN32(::GetLastError()));
		}
	}
};

typedef std::unique_ptr<HANDLE, HandleDeleter> CSafeHandle;
