#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

class Polinom
{
private:
	std::vector<long long> mCoeff;
public:
	Polinom operator+(const Polinom& rhs) const;
	Polinom operator-(const Polinom& rhs) const;
	Polinom operator*(const Polinom& rhs) const;
	friend std::istream& operator>>(std::istream& in, Polinom& rhs);
	friend std::ostream& operator<<(std::ostream& out, const Polinom& rhs);
	friend void derive(Polinom& p, int degree);
	int getDegree() const
	{
		return mCoeff.size() - 1;
	}
};

long long prod(int start, int end)
{
	long long ans = 1;
	for (int i = start; i <= end; ++i)
		ans *= i;

	return ans;
}

void derive(Polinom& p, int degree = 1)
{
	if (degree == 0)
		return;

	for (int i = 0; i < p.mCoeff.size(); ++i)
	{
		if (i - degree < 0)
		{
			p.mCoeff[i] = 0; 
			continue;
		}

		p.mCoeff[i - degree] = p.mCoeff[i] * prod(i - degree + 1, i);
		p.mCoeff[i] = 0;
	}

	while (p.mCoeff.size() > 1 && p.mCoeff.back() == 0)
		p.mCoeff.pop_back();
}

Polinom Polinom::operator+(const Polinom& rhs) const
{
	Polinom res;
	int maxDegree = std::max(getDegree(), rhs.getDegree());
	int minDegree = std::min(getDegree(), rhs.getDegree());
	res.mCoeff.resize(maxDegree + 1);

	for (int i = 0; i < minDegree; ++i)
		res.mCoeff[i] = mCoeff[i] + rhs.mCoeff[i];

	for (int i = minDegree; i < mCoeff.size(); ++i)
		res.mCoeff[i] += mCoeff[i];

	for (int i = minDegree; i < rhs.mCoeff.size(); ++i)
		res.mCoeff[i] += rhs.mCoeff[i];

	while (res.mCoeff.back() == 0 && res.mCoeff.size() > 1)
	{
		res.mCoeff.pop_back();
	}

	return res;
}
Polinom Polinom::operator-(const Polinom& rhs) const
{
	Polinom res;
	int maxDegree = std::max(getDegree(), rhs.getDegree());
	int minDegree = std::min(getDegree(), rhs.getDegree());
	res.mCoeff.resize(maxDegree + 1);

	for (int i = 0; i < minDegree; ++i)
		res.mCoeff[i] = mCoeff[i] - rhs.mCoeff[i];

	for (int i = minDegree; i < mCoeff.size(); ++i)
		res.mCoeff[i] += mCoeff[i];

	for (int i = minDegree; i < rhs.mCoeff.size(); ++i)
		res.mCoeff[i] -= rhs.mCoeff[i];

	while (res.mCoeff.back() == 0 && res.mCoeff.size() > 1)
	{
		res.mCoeff.pop_back();
	}

	return res;
}
Polinom Polinom::operator*(const Polinom& rhs) const
{
	Polinom res;
	int resDegree = getDegree() + rhs.getDegree();
	res.mCoeff.resize(resDegree + 1);

	for (int i = 0; i <= getDegree(); ++i)
	{
		for (int j = 0; j <= rhs.getDegree(); ++j)
		{
			res.mCoeff[i + j] += mCoeff[i] * rhs.mCoeff[j];
		}
	}

	while (res.mCoeff.back() == 0 && res.mCoeff.size() > 1)
	{
		res.mCoeff.pop_back();
	}

	return res;
}
std::istream& operator>>(std::istream& in, Polinom& rhs)
{
	std::string expression;
	std::getline(in, expression);
	std::istringstream iss(expression);

	int coeff, sign = 1;
	char dummy;
	while (iss >> dummy)
	{
		if (dummy == 'x')
		{
			coeff = 1;
			iss.putback(dummy);
		}
		else if (dummy == '-')
		{
			sign = -1;

			char x;
			iss >> x;

			if (x == 'x')
			{
				coeff = 1;
				iss.putback(x);
			}
			else
			{
				iss.putback(x);
				iss >> coeff;
			}
		}
		else
		{
			iss.putback(dummy);
			iss >> coeff;
		}

		// 0 power
		if (iss.rdbuf()->in_avail() == 0)
		{
			if (rhs.mCoeff.empty())
			{
				rhs.mCoeff.resize(1);
			}

			rhs.mCoeff[0] = sign * coeff;
			break;
		}

		// read 'x'
		iss >> dummy;

		int power;
		if (iss.rdbuf()->in_avail() == 0)
			power = 1;
		else
		{
			// read '^' or something else
			iss >> dummy;

			if (dummy == '^')
				iss >> power;
			else
			{
				iss.putback(dummy);
				power = 1;
			}
		}

		// first member
		if (rhs.mCoeff.empty())
		{
			rhs.mCoeff.resize(power + 1);
		}
		rhs.mCoeff[power] = sign * coeff;

		// not the last, so there's a sign after
		if (iss.rdbuf()->in_avail())
		{
			iss >> dummy;
			if (dummy == '-')
				sign = -1;
			else
				sign = 1;
		}
	}

	return in;
}
std::ostream& operator<<(std::ostream& out, const Polinom& rhs)
{
	bool is0 = true;
	for (int i = rhs.getDegree(); i > 0; --i)
	{
		int coeff = rhs.mCoeff[i];
		if (coeff)
		{
			is0 = false;

			if (i == rhs.getDegree())
			{
				if (coeff < 0)
					out << "-";
			}
			else
			{
				if (coeff < 0)
					out << " - ";
				else
					out << " + ";
			}

			if (abs(coeff) != 1)
				out << abs(coeff);

			out << "x";

			if (i != 1)
				out << "^" << i;
		}
	}

	if (is0)
		out << rhs.mCoeff[0];

	else if (rhs.mCoeff[0])
	{
		if (rhs.mCoeff[0] < 0)
			out << " - ";
		else
			out << " + ";
		out << abs(rhs.mCoeff[0]);
	}

	return out;
}

int main()
{
	Polinom p;
	std::cin >> p;

	int degree;
	std::cin >> degree;

	derive(p, degree);
	std::cout << p << '\n';
}