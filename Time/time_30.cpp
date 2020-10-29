#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <sstream>
#include <algorithm>

class Time;
std::pair<Time, Time> minmax(const Time& time1, const Time& time2);

class Time {
private:
	unsigned int hour, minute, second;
	std::string adjustOneDigitNumber(unsigned int number) const
	{
		return "0" + std::to_string(number);
	}
	std::string getNumberRepresentation(unsigned int number) const
	{
		if (number < 10)
			return adjustOneDigitNumber(number);
		return std::to_string(number);
	}
public:
	Time(unsigned int hour, unsigned int minute, unsigned int second) : hour(hour), minute(minute), second(second)
	{

	}
	unsigned int toSeconds() const
	{
		return hour * 3600 + minute * 60 + second;
	}
	unsigned int distanceFrom(const Time& time) const
	{
		auto tpair = minmax(*this, time);
		return tpair.second.toSeconds() - tpair.first.toSeconds();
	}
	unsigned int distanceFromNoon() const
	{
		return distanceFrom(Time(12, 0, 0));
	}
	unsigned int getHour() const
	{
		return hour;
	}
	unsigned int getMinute() const
	{
		return minute;
	}
	unsigned int getSecond() const
	{
		return second;
	}
	std::string getTimeRepresentation() const
	{
		return getNumberRepresentation(hour) + ":"
			 + getNumberRepresentation(minute) + ":"
		   	 + getNumberRepresentation(second);
	}
};

Time createTimeFromFormat(const std::string& str)
{
	std::istringstream in(str);
	unsigned int hour, minute, second;
	char dummy;
	in >> hour >> dummy >> minute >> dummy >> second;
	return Time(hour, minute, second);
}
std::pair<Time, Time> minmax(const Time& time1, const Time& time2)
{
	if (time1.toSeconds() < time2.toSeconds())
		return std::make_pair(time1, time2);
	return std::make_pair(time2, time1);
}

int main()
{
	std::vector<Time> v;

	int N;
	std::cin >> N;

	for (int i = 0; i < N; ++i)
	{
		std::string cur;
		std::cin >> cur;
		v.push_back(createTimeFromFormat(cur));
	}

	std::sort(v.begin(), v.end(), [](const Time& t1, const Time& t2) { return t1.distanceFromNoon() < t2.distanceFromNoon(); });
	std::string t1 = v[0].getTimeRepresentation(), t2 = v[1].getTimeRepresentation();
	if (t1 < t2)
		std::cout << t1 << '\n' << t2 << '\n';
	else
		std::cout << t2 << '\n' << t1 << '\n';
}