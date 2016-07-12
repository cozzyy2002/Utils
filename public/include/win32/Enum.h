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

#ifndef ENUM_VALUE_NAME
#define ENUM_VALUE_NAME(x) x##Value
#endif

#define ENUM_CLASS(T, ...) \
	enum class ENUM_VALUE_NAME(T) { __VA_ARGS__, COUNT, INVALID = -1 }; \
	typedef CEnumClass<ENUM_VALUE_NAME(T)> T; \
	CEnumClass<ENUM_VALUE_NAME(T)>::CEnumClass(ENUM_VALUE_NAME(T) value) : m_value(value) { CEnum_init(m_names, _T(#__VA_ARGS__)); } \
	std::vector<tstring> CEnumClass<ENUM_VALUE_NAME(T)>::m_names

template<typename E>
class CEnumClass {
public:
	CEnumClass(E value = E::INVALID);
	typedef E Value;
	template<typename T>
	static constexpr T TO(Value value) { return static_cast<T>(value); }
	template<typename T>
	static constexpr Value FROM(typename T value) { return static_cast<Value>(value); }

	static LPCTSTR toString(Value value) { return isValid(value) ? m_names[TO<int>(value)].c_str() : _T("UNKNOWN"); }
	static bool isValid(Value value) { size_t i = TO<size_t>(value); return (0 <= i) && (i < count()); }
	static size_t count() { return m_names.size(); }

	Value value() const { return m_value; }
	LPCTSTR toString() const { return toString(m_value); }
	bool isValid() const { return isValid(m_value); }
	operator Value() const { return m_value; }

protected:
	Value m_value;
	static std::vector<tstring> m_names;
};

namespace std {
	template<typename E>
	ostream& operator<<(ostream& stream, const CEnumClass<E>& value) {
		return stream << value.toString() << ":" << (int)value.value();
	}
}
