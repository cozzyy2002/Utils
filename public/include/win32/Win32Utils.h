#pragma once

#include "ComUtils.h"

#include <memory>

#if !defined(TYPEDEF_CSafeHandle)

class CNoneCopyable {
protected:
	CNoneCopyable() {}

private:
	CNoneCopyable(const CNoneCopyable&);
	CNoneCopyable& operator=(CNoneCopyable&);
};

class CSafeHandle : CNoneCopyable {
public:
	CSafeHandle(HANDLE h = NULL) : m_handle(h) {}

	void reset(HANDLE h = NULL) { m_handle.reset(h); }
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

class CSafeEventHandle : public CSafeHandle {
public:
	CSafeEventHandle(BOOL manualReset = TRUE, BOOL initialState = FALSE, LPCTSTR name = NULL);
};

/**
	Safe handle class that duplicat another handle.

	Copy constructor and = operator can accept CCopyableSafeHandle object created in the other process.
	This object can be used only in the process that created the object,
	except used as source of copy constructor or = operator.
*/
template<
	DWORD	dwDesiredAccess = 0,
	BOOL	bInheritHandle = FALSE,
	DWORD	dwOptions = DUPLICATE_SAME_ACCESS
>
class CCopyableSafeHandle : public CSafeHandle {
public:
	CCopyableSafeHandle(HANDLE h = NULL)
		: CSafeHandle(h)
		, m_process(GetCurrentProcess()) {}

	CCopyableSafeHandle(const CCopyableSafeHandle& other)
		: m_process(GetCurrentProcess()) { *this = other; }

	CCopyableSafeHandle& operator=(const CCopyableSafeHandle& other)
	{
		HANDLE handle;
		HRESULT hr = WIN32_EXPECT(
			DuplicateHandle(other.m_process, other, m_process, &handle,
				dwDesiredAccess, bInheritHandle, dwOptions)
		);
		if (SUCCEEDED(hr)) {
			m_handle.reset(handle);
		}

		return *this;
	}

protected:
	// Process handle in which this object is created.
	CSafeHandle m_process;
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

// Deleter to set value
template<typename T, T V>
struct ValueDeleleter { void operator()(T* p) { *p = V; } };

// Smart pointers to set bool value at destruction
template<typename T, T V>
using CSafeValue = std::unique_ptr<T, ValueDeleleter<T, V>>;
