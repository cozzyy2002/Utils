#include "stdafx.h"
#include "win32/Enum.h"

using namespace testing;

class EnumTest : public Test {
public:
	ENUM(Color, RED, GREEN, BLUE);
	ENUM(WeekDay, SUN, MON, TUE, WED, THU, FRI, SAT);
};

template<typename T>
struct ValueTestTemplate {
	int intValue;
	T value;
	LPCTSTR name;
};

ValueTestTemplate<EnumTest::Color::Values> ColorValueTestData[] = {
	{ 0, EnumTest::Color::RED, _T("RED") },
	{ 1, EnumTest::Color::GREEN, _T("GREEN") },
	{ 2, EnumTest::Color::BLUE, _T("BLUE") },
};

class EnumValueTest : public TestWithParam<ValueTestTemplate<EnumTest::Color::Values>> {};

INSTANTIATE_TEST_CASE_P(Colors, EnumValueTest, ValuesIn(ColorValueTestData));

TEST_P(EnumValueTest, Colors)
{
	const ValueTestTemplate<EnumTest::Color::Values>& data = GetParam();

	EnumTest::Color color(data.value);

	EXPECT_EQ(data.intValue, data.value);
	EXPECT_TRUE(EnumTest::Color::isValid(data.value));
	EXPECT_TRUE(color.isValid());
	EXPECT_STREQ(data.name, EnumTest::Color::toString(data.value));
	EXPECT_STREQ(data.name, color.toString());
}

TEST_F(EnumTest, Color_toString)
{
	EXPECT_STREQ(_T("UNKNOWN"), ((Color)Color::COUNT).toString());
	EXPECT_STREQ(_T("UNKNOWN"), ((Color)Color::INVALID).toString());
	EXPECT_STREQ(_T("UNKNOWN"), ((Color)(Color::Values)5).toString());
}

TEST_F(EnumTest, Color_methods)
{
	Color color;

	EXPECT_FALSE(color.isValid())			<< "Default constructor";
	color = Color(Color::Values::BLUE);
	EXPECT_TRUE(color.isValid())			<< "Valid value";
	color = Color::Values::INVALID;
	EXPECT_FALSE(color.isValid())			<< "Invalid value";

	// Conpare object with Values type
	Color::Values v = Color::GREEN;
	color = v;
	EXPECT_TRUE(color == v);
	EXPECT_TRUE(color != Color::RED);
	EXPECT_TRUE(v == color);
	EXPECT_TRUE(Color::RED != color);

	// Compare Objects
	EXPECT_TRUE(color == Color(color));
}

TEST_F(EnumTest, WeekDay_multiInstance)
{
	const WeekDay d1(WeekDay::Values::FRI);
	const WeekDay d2;

	EXPECT_EQ(7, WeekDay::COUNT)			<< "Count of enum";
	EXPECT_EQ(7, d1.COUNT)					<< "Count of instance 1";
	EXPECT_EQ(7, d2.COUNT)					<< "Count of instance 2";
	EXPECT_STREQ(d1.toString(), d2.toString(WeekDay::Values::FRI))	<<"instance and static toString()";

	WeekDay d3(d1);
	EXPECT_EQ(WeekDay::Values::FRI, d3)		<< "Instance created by copy constructor";
	EXPECT_STREQ(_T("FRI"), d3.toString())		 << "Instance created by copy constructor";
}

ENUM_CLASS(Month, Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec);

TEST(EnumClassTest, 1)
{
	Month month;
	EXPECT_EQ(Month::FROM(12), MonthValue::COUNT);
	EXPECT_EQ(Month::FROM(-1), MonthValue::INVALID);
	EXPECT_FALSE(month.isValid());

	EXPECT_STREQ(_T("UNKNOWN"), month.toString());

	EXPECT_STREQ(_T("Jan"), month.toString(MonthValue::Jan));
	EXPECT_STREQ(_T("Feb"), month.toString(MonthValue::Feb));
	EXPECT_STREQ(_T("Mar"), month.toString(MonthValue::Mar));

	std::cout << "month=" << month << std::endl;
	std::cout << "month=" << Month(Month::Value::Aug) << std::endl;

	Month m(MonthValue::Apr);
	std::cout << "month=" << m << std::endl;
}
