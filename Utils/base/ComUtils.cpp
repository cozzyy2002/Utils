#include "stdafx.h"
#include "win32/ComUtils.h"
#include <stdio.h>

#pragma comment(lib, "Shlwapi.lib")

static log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("Utils.ComUtils"));

HRESULT CUnknownImpl::QueryInterfaceImpl(REFIID riid, void ** ppvObject)
{
	static const QITAB qitab[] = { {0} };
	const QITAB* pqitab = getQITAB();
	return QISearch(this, pqitab ? pqitab : qitab, riid, ppvObject);
}

ULONG CUnknownImpl::AddRefImpl(void)
{
	return InterlockedIncrement(&m_refCount);
}

ULONG CUnknownImpl::ReleaseImpl(void)
{
	LONG refCount = InterlockedDecrement(&m_refCount);
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
