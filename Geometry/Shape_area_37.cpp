#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <numeric>
#include <iomanip>
#include <algorithm>

class Shape
{
protected:
	std::vector<double> mSides;
public:
	virtual double get_area() = 0;
	friend std::istream& operator>>(std::istream&, Shape&);
};

std::istream& operator>>(std::istream& in, Shape& rhs)
{
	for (int i = 0; i < rhs.mSides.size(); ++i)
		in >> rhs.mSides[i];
	return in;
}

class Rectangle : public Shape
{
public:
	Rectangle()
	{
		mSides.resize(2);
	}
	virtual double get_area()
	{
		return mSides[0] * mSides[1];
	}
};

class Triangle : public Shape
{
public:
	Triangle()
	{
		mSides.resize(3);
	}
	virtual double get_area()
	{
		double p = std::accumulate(mSides.begin(), mSides.end(), 0.) / 2;
		return sqrt(p * (p - mSides[0]) * (p - mSides[1]) * (p - mSides[2]));
	}

};

class Square : public Shape
{
public:
	Square()
	{
		mSides.resize(1);
	}
	virtual double get_area()
	{
		return mSides[0] * mSides[0];
	}
};

int main()
{
	int N;
	std::cin >> N;

	std::cout << std::fixed << std::setprecision(5);

	std::vector<double> areas;

	for (int i = 0; i < N; ++i)
	{
		Shape* sPtr;
		std::string type;
		std::cin >> type;

		if (type[0] == 'R')
			sPtr = new Rectangle;
		else if (type[0] == 'S')
			sPtr = new Square;
		else
			sPtr = new Triangle;

		std::cin >> *sPtr;
		areas.push_back(sPtr->get_area());

		delete sPtr;
	}

	std::sort(areas.begin(), areas.end());

	for (const auto& area : areas)
		std::cout << area << '\n';
}