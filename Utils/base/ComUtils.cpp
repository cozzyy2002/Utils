#include "stdafx.h"
#include "win32/ComUtils.h"
#include <stdio.h>

#pragma comment(lib, "Shlwapi.lib")

static log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("Utils.ComUtils"));

ULONG CUnknownImpl::AddRefImpl(void)
{
	LONG refCount = InterlockedIncrement(&m_refCount);
	LOG4CPLUS_DEBUG(logger, typeid(*this).name() << "::AddRef(). ref counf=" << refCount);
	return refCount;
}

ULONG CUnknownImpl::ReleaseImpl(void)
{
	LONG refCount = InterlockedDecrement(&m_refCount);
	LOG4CPLUS_DEBUG(logger, typeid(*this).name() << "::Release(). ref counf=" << refCount);
	if (refCount == 0) delete this;
	return refCount;
}

HRESULT hrCheck(HRESULT exp, LPCTSTR expStr, LPCTSTR src, int line)
{
	if (FAILED(exp)) {
		LOG4CPLUS_ERROR(logger, 
			expStr << _T(" failed. HRESULT=0x") << std::hex << exp
			<< _T(" at:\n") << src << _T("(") << std::dec << line << _T(")"));
		//_ftprintf_p(stderr, _T("%s faild: 0x%08lx at:\n%s(%d)\n"), expStr, exp, src, line);
	}
	return exp;
}
