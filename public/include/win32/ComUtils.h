#pragma once

#include <combaseapi.h>
#include <Shlwapi.h>

#define IUNKNOWN_METHODS \
	STDMETHODIMP QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR *__RPC_FAR * ppvObject)	\
				{ return CUnknownImpl::QueryInterfaceImpl(this, riid, ppvObject); }					\
	STDMETHODIMP_(ULONG) AddRef(void) { return CUnknownImpl::AddRefImpl(); }						\
	STDMETHODIMP_(ULONG) Release(void) { return CUnknownImpl::ReleaseImpl(); }

class CUnknownImpl {
public:
	CUnknownImpl() : m_refCount(0) {}
	virtual ~CUnknownImpl() {}

protected:
	template<class T>
	HRESULT QueryInterfaceImpl(
		T* _this,
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		static const QITAB qitab[] = { QITABENT(T, IUnknown),{ 0 } };
		const QITAB* pqitab = getQITAB();
		HRESULT hr = QISearch(_this, pqitab ? pqitab : qitab, riid, ppvObject);

		return hr;
	}

	ULONG AddRefImpl(void);

	ULONG ReleaseImpl(void);

protected:
	virtual const QITAB* getQITAB() const { return NULL; }
	LONG m_refCount;
};

#define HR_ASSERT(exp, hr) do { HRESULT _hr = HR_EXPECT(exp, hr); if(FAILED(_hr)) return _hr; } while(0)
#define HR_ASSERT_OK(exp) do { HRESULT _hr = HR_EXPECT_OK(exp); if(FAILED(_hr)) return _hr; } while(0)
#define HR_EXPECT(exp, hr) hrCheck((exp) ? S_OK : hr, _T(#exp), _T(__FILE__), __LINE__)
#define HR_EXPECT_OK(exp) hrCheck(exp, _T(#exp), _T(__FILE__), __LINE__)

#define WIN32_ASSERT(exp) HR_ASSERT(exp, HRESULT_FROM_WIN32(GetLastError()))
#define WIN32_EXPECT(exp) HR_EXPECT(exp, HRESULT_FROM_WIN32(GetLastError()))

#define HR_FAIL(exp, hr) return hrCheck(hr, _T(#exp), _T(__FILE__), __LINE__)
#define WIN32_FAIL(exp, error) return hrCheck(HRESULT_FROM_WIN32(error), _T(#exp), _T(__FILE__), __LINE__)

HRESULT hrCheck(HRESULT expre, LPCTSTR exprStr, LPCTSTR src, int line);
