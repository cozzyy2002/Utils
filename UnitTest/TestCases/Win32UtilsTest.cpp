#include "stdafx.h"

#include "win32/Win32Utils.h"
#include "Win32Mock.h"

#include <memory>

using namespace ::testing;

DECRARE_WIN32_MOCK;

class CSafeHandleTest : public Test {
public:
	Win32Mock win32Mock;
};

/**
	@test CloseHandle() succeeded in CSafeHandle destructor
*/
TEST_F(CSafeHandleTest, CSafeHandle_Normal)
{
	const HANDLE handle = (HANDLE)10;
	EXPECT_CALL(win32Mock, CloseHandle(handle)).WillOnce(Return(TRUE));
	EXPECT_CALL(win32Mock, GetLastError()).Times(0);
	{
		Win32HookCloseHandle;
		Win32HookGetLastError;

		CSafeHandle h(handle);
		EXPECT_EQ(handle, h);
	}
}

/**
	@test CloseHandle() failed in CSafeHandle destructor
*/
TEST_F(CSafeHandleTest, CSafeHandle_Error)
{
	const HANDLE handle = (HANDLE)10;
	EXPECT_CALL(win32Mock, CloseHandle(handle)).WillOnce(Return(FALSE));
	EXPECT_CALL(win32Mock, GetLastError()).WillOnce(Return(5));
	{
		Win32HookCloseHandle;
		Win32HookGetLastError;

		CSafeHandle h(handle);
		EXPECT_EQ(handle, h);
	}
}

/**
	@test CloseHandle() default constructor(HANDLE = NULL)
*/
TEST_F(CSafeHandleTest, CSafeHandle_NULL)
{
	EXPECT_CALL(win32Mock, CloseHandle(_)).Times(0);
	{
		Win32HookCloseHandle;

		CSafeHandle h;
	}
}

/**
	@test CloseHandle() constructor assigned INVALID_HANDLE_VALUE
*/
TEST_F(CSafeHandleTest, CSafeHandle_INVALID)
{
	EXPECT_CALL(win32Mock, CloseHandle(_)).Times(0);
	{
		Win32HookCloseHandle;

		CSafeHandle h(INVALID_HANDLE_VALUE);
	}
}

class CCopyableSafeHandleTest : public CSafeHandleTest {
public:
	typedef CCopyableSafeHandle<> CSafeHandleEx;
};

TEST_F(CCopyableSafeHandleTest, operator_assign)
{
	// Handle values assigned to m_handle or m_process, and they should be closed.
	for (int h = 1; h < 6; h++) {
		EXPECT_CALL(win32Mock, CloseHandle((HANDLE)h)).WillOnce(Return(TRUE));
	}

	EXPECT_CALL(win32Mock, DuplicateHandle((HANDLE)3, (HANDLE)5, (HANDLE)2, _, _, _, _)).WillOnce(DoAll(
		SetArgPointee<3>((HANDLE)1),
		Return(TRUE)
	));
	EXPECT_CALL(win32Mock, GetCurrentProcess())
		.WillOnce(Return((HANDLE)2))
		.WillOnce(Return((HANDLE)3));
	{
		Win32HookDuplicateHandle;
		Win32HookGetCurrentProcess;
		Win32HookCloseHandle;

		CSafeHandleEx a((HANDLE)4);
		CSafeHandleEx b((HANDLE)5);
		HANDLE h = a = b;
		EXPECT_EQ((HANDLE)1, a);
		EXPECT_EQ((HANDLE)5, b);
		EXPECT_EQ(h, a);
	}
}

TEST_F(CCopyableSafeHandleTest, duplicate_error)
{
	// Handle values assigned to m_handle or m_process, and they should be closed.
	for (int h = 1; h < 5; h++) {
		EXPECT_CALL(win32Mock, CloseHandle((HANDLE)h)).WillOnce(Return(TRUE));
	}

	EXPECT_CALL(win32Mock, DuplicateHandle(_, _, _, _, _, _, _))
		.WillOnce(Return(FALSE));
	EXPECT_CALL(win32Mock, GetCurrentProcess())
		.WillOnce(Return((HANDLE)1))
		.WillOnce(Return((HANDLE)2));
	EXPECT_CALL(win32Mock, GetLastError()).WillOnce(Return(6));
	{
		Win32HookDuplicateHandle;
		Win32HookGetCurrentProcess;
		Win32HookCloseHandle;
		Win32HookGetLastError;

		CSafeHandleEx a((HANDLE)3);
		CSafeHandleEx b((HANDLE)4);
		HANDLE h = a = b;
		EXPECT_EQ((HANDLE)3, a);
		EXPECT_EQ((HANDLE)4, b);
		EXPECT_EQ(h, a);
	}
}
