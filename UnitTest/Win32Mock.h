#pragma once

class Win32Mock {
public:
	Win32Mock() { instance = this; }
	~Win32Mock() { instance = NULL; }

	MOCK_METHOD7(DuplicateHandle, BOOL(HANDLE, HANDLE, HANDLE, LPHANDLE, DWORD, BOOL, DWORD));
	MOCK_METHOD0(GetCurrentProcess, HANDLE());
	MOCK_METHOD1(CloseHandle, BOOL(HANDLE));
	MOCK_METHOD0(GetLastError, DWORD());

	// Used by global hook functions.
	static Win32Mock* instance;;
};

// Specify this macro in any .cpp file.
#define DECRARE_WIN32_MOCK Win32Mock* Win32Mock::instance = NULL

#define Win32HookDuplicateHandle \
	SEXYHOOK_API_HOOK_7_BEGIN("Kernel32.dll",BOOL,DuplicateHandle,HANDLE h1,HANDLE h2,HANDLE h3,LPHANDLE ph4,DWORD ac,BOOL inh,DWORD opt) \
	{ \
		return Win32Mock::instance->DuplicateHandle(h1, h2, h3, ph4, ac, inh, opt); \
	} \
	SEXYHOOK_API_END()

#define Win32HookGetCurrentProcess \
	SEXYHOOK_API_HOOK_0_BEGIN("Kernel32.dll",HANDLE,GetCurrentProcess) \
	{ \
		return Win32Mock::instance->GetCurrentProcess(); \
	} \
	SEXYHOOK_API_END()

#define Win32HookCloseHandle \
	SEXYHOOK_API_HOOK_1_BEGIN("Kernel32.dll",BOOL,CloseHandle,HANDLE h) \
	{ \
		return Win32Mock::instance->CloseHandle(h); \
	} \
	SEXYHOOK_API_END()

#define Win32HookGetLastError \
	SEXYHOOK_API_HOOK_0_BEGIN("Kernel32.dll",DWORD,GetLastError) \
	{ \
		return Win32Mock::instance->GetLastError(); \
	} \
	SEXYHOOK_API_END()
