#pragma once

#include <vector>
#include <string>

#ifdef _UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

#define ENUM(T, ...) \
	class T : public CEnum<T> {		\
	public:							\
		typedef enum {				\
					__VA_ARGS__,	/* Values specified in parameters of ENUM() macro	*/	\
					COUNT,			/* Values count which can be used as literal number	*/	\
					INVALID = -1	/* Invalid value which is not equal to other value	*/	\
		} Values;					\
	\
		T(Values value = INVALID) : CEnum(value) { CEnum_init(m_names, _T(#__VA_ARGS__)); }	\
		T(const T& other) : CEnum(other.m_value) {}											\
		T& operator=(Values value) { m_value = value; return *this; }						\
	}

template<class T, typename V = int>
class CEnum {
public:
	CEnum(typename V value) : m_value(value) {}

	static LPCTSTR toString(typename V value) { return isValid(value) ? m_names[value].c_str() : _T("UNKNOWN"); }
	static bool isValid(typename V value) { return (0 <= value) && (value < (V)count()); }
	static size_t count() { return m_names.size(); }

	LPCTSTR toString() const { return toString(m_value); }
	bool isValid() const { return isValid(m_value); }
	operator V() const { return m_value; }

protected:
	V m_value;
	static std::vector<tstring> m_names;
};

template<class T, typename V>
/*static*/ std::vector<tstring> CEnum<T, V>::m_names;

extern void CEnum_init(std::vector<tstring>& names, LPCTSTR values);
