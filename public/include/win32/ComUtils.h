#pragma once

#include <combaseapi.h>
#include <Shlwapi.h>

#define IUNKNOWN_METHODS \
	STDMETHODIMP QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR *__RPC_FAR * ppvObject)	\
				{ return CUnknownImpl::QueryInterfaceImpl(riid, ppvObject); }						\
	STDMETHODIMP_(ULONG) AddRef(void) { return CUnknownImpl::AddRefImpl(); }						\
	STDMETHODIMP_(ULONG) Release(void) { return CUnknownImpl::ReleaseImpl(); }

class CUnknownImpl : public IUnknown {
public:
	CUnknownImpl() : m_refCount(0) {}
	virtual ~CUnknownImpl() {}

protected:
	HRESULT QueryInterfaceImpl(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject);

	ULONG AddRefImpl(void);

	ULONG ReleaseImpl(void);

protected:
	virtual const QITAB* getQITAB() const { return NULL; }
	LONG m_refCount;
};

#define HR_ASSERT(exp, hr) do { HRESULT _hr = (exp) ? S_OK : hr; if(FAILED(_hr)) return hrCheck(_hr, _T(#exp)); } while(0)
#define HR_ASSERT_OK(exp) do { HRESULT _hr = exp; if(FAILED(_hr)) return hrCheck(_hr, _T(#exp)); } while(0)
#define HR_EXPECT(exp, hr) hrCheck((exp) ? S_OK : hr, _T(#exp))
#define HR_EXPECT_OK(exp) hrCheck(exp, _T(#exp))

HRESULT hrCheck(HRESULT expre, LPCTSTR exprStr);
