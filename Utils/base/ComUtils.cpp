#include "stdafx.h"
#include "win32/ComUtils.h"
#include <stdio.h>

#pragma comment(lib, "Shlwapi.lib")

static log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("Utils.ComUtils"));

ULONG CUnknownImpl::AddRefImpl(void)
{
	LONG refCount = InterlockedIncrement(&m_refCount);
	LOG4CPLUS_DEBUG_FMT(logger, _T("%s::AddRef(). ref counf=%d"), getName(), refCount);
	return refCount;
}

ULONG CUnknownImpl::ReleaseImpl(void)
{
	LONG refCount = InterlockedDecrement(&m_refCount);
	LOG4CPLUS_DEBUG_FMT(logger, _T("%s::Release(). ref counf=%d"), getName(), refCount);
	if (refCount == 0) delete this;
	return refCount;
}

LPCTSTR CUnknownImpl::getName()
{
	if (!m_name) {
		m_name.reset(new CA2T(typeid(*this).name()));
	}
	return (LPCTSTR)*m_name;
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
