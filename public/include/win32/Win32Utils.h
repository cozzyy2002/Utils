#pragma once

#include "ComUtils.h"

#include <memory>

#if !defined(TYPEDEF_CSafeHandle)

class CSafeHandle {
public:
	CSafeHandle(HANDLE h = NULL) : m_handle(h) {}

	operator HANDLE() const { return m_handle.get(); }

	bool isValid() const { return isValid(m_handle.get()); }
	static bool isValid(HANDLE h);

	struct Deleter {
		typedef HANDLE pointer;
		void operator()(HANDLE h);
	};

protected:
	std::unique_ptr<HANDLE, Deleter> m_handle;
};

#else
struct HandleDeleter {
	typedef HANDLE pointer;
	void operator()(HANDLE h) {
		if ((h != NULL) && (h != INVALID_HANDLE_VALUE)) {
			HR_EXPECT(::CloseHandle(h), HRESULT_FROM_WIN32(::GetLastError()));
		}
	}
};

typedef std::unique_ptr<HANDLE, HandleDeleter> CSafeHandle;

#endif
