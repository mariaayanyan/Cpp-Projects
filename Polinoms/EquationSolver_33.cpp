#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

class Complex
{
private:
	double mReal, mImaginary;

public:
	Complex() = default;
	Complex(double, double);
	Complex operator+(const Complex&) const;
	Complex operator-(const Complex&) const;
	Complex operator*(const Complex&) const;
	bool operator<(const Complex&) const;
	friend std::istream& operator>>(std::istream&, Complex&);
	friend std::ostream& operator<<(std::ostream&, const Complex&);
};

double square(double x)
{
	return x * x;
}

Complex::Complex(double real, double imaginary = 0) : mReal(real), mImaginary(imaginary)
{

}
bool Complex::operator<(const Complex& rhs) const
{
	if (mReal != rhs.mReal)
		return mReal < rhs.mReal;
	return mImaginary < rhs.mImaginary;
}
Complex Complex::operator+(const Complex& rhs) const
{
	return Complex(mReal + rhs.mReal, mImaginary + rhs.mImaginary);
}
Complex Complex::operator-(const Complex& rhs) const
{
	return Complex(mReal - rhs.mReal, mImaginary - rhs.mImaginary);
}
Complex Complex::operator*(const Complex& rhs) const
{
	return Complex(mReal * rhs.mReal - mImaginary * rhs.mImaginary, mReal * rhs.mImaginary + mImaginary * rhs.mReal);
}
std::istream& operator>>(std::istream& in, Complex& complexNum)
{
	std::string expression;
	std::getline(in, expression);

	std::istringstream iss(expression);
	complexNum.mReal = complexNum.mImaginary = 0;

	// is a real number
	if (expression.back() != 'i')
	{
		iss >> complexNum.mReal;
		return in;
	}

	complexNum.mImaginary = 1;

	// get rid of i and possible whitespace at the end
	while (!expression.empty() && (expression.back() == 'i' || expression.back() == ' '))
		expression.pop_back();

	// is i
	if (expression.empty())
		return in;

	iss.str(expression);

	char sign;
	iss >> sign;

	// is -i
	if (sign == '-' && iss.rdbuf()->in_avail() == 0)
	{
		complexNum.mImaginary = -1;
		return in;
	}

	iss.putback(sign);

	double coef;
	iss >> coef;

	// imaginary part is not 0
	if (iss.rdbuf()->in_avail())
	{
		complexNum.mReal = coef;
		iss >> sign;
		if (sign == '-')
			complexNum.mImaginary = -1;
		if (iss.rdbuf()->in_avail())
			iss >> coef;
		else
			coef = 1;
	}

	complexNum.mImaginary *= coef;
	return in;
}
std::ostream& operator<<(std::ostream& out, const Complex& complexNum)
{
	out << std::fixed << std::setprecision(5);
	if (complexNum.mReal == 0 && complexNum.mImaginary == 0)
	{
		out << 0;
		return out;
	}

	if (complexNum.mReal)
		out << complexNum.mReal;

	if (complexNum.mImaginary == 0)
		return out;

	if (complexNum.mReal && complexNum.mImaginary > 0)
		out << " + ";
	if (complexNum.mImaginary < 0)
		out << (complexNum.mReal == 0 ? "-" : " - ");

	if (fabs(complexNum.mImaginary) != 1)
		out << fabs(complexNum.mImaginary);

	out << "i";

	return out;
}

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
	long long getCoeff(int degree) const
	{
		return mCoeff[degree];
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
		long long coeff = rhs.mCoeff[i];
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
				out << (coeff);

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

// Abstract base class for solving equations
class EquationSolver {
protected:
	Polinom mPolinom;
	std::vector<Complex> mRoots;

	// solve the equation and fill mRoots with roots of equation
	virtual void solve() = 0;
public:
	// create QuadradicEquationSolver from Polinom
	EquationSolver(const Polinom& polinom) : mPolinom(polinom) {}

	// return roots of equation (if not solved, solve the equation to find the roots) 
	std::vector<Complex> getRoots()
	{
		if (mRoots.empty())
			solve();
		return mRoots;
	}
};

// Concrete class for solving linear equations
class LinearEquationSolver : public EquationSolver {
protected:
	virtual void solve()
	{
		mRoots.emplace_back(-1.0 * mPolinom.getCoeff(0)/mPolinom.getCoeff(1));
	}
public:
	LinearEquationSolver(const Polinom& polinom) : EquationSolver(polinom) {}
};

// Concrete class for solving quadradic equations
class QuadradicEquationSolver : public EquationSolver {
protected:
	virtual void solve();
public:
	QuadradicEquationSolver(const Polinom& polinom) : EquationSolver(polinom) {}
};

void QuadradicEquationSolver::solve()
{
	double a = mPolinom.getCoeff(2);
	double b = mPolinom.getCoeff(1);
	double c = mPolinom.getCoeff(0);

	double D = b * b - 4 * a * c;
	if (D == 0)
	{
		mRoots.emplace_back(-b / (2 * a));
		return;
	}

	Complex Root;
	if (D > 0)
		Root = Complex(sqrt(D) / (2 * a));
	else
		Root = Complex(0, sqrt(-D) / (2 * a));
	
	mRoots.push_back(Complex(-b / (2 * a)) - Root);
	mRoots.push_back(Complex(-b / (2 * a)) + Root);
}

// Function for creating solver based on polinom degree
EquationSolver* solverFactory(const Polinom& polinom)
{
	if (polinom.getDegree() == 1)
		return new LinearEquationSolver(polinom);
	return new QuadradicEquationSolver(polinom);
}

int main()
{
	Polinom p;
	std::cin >> p;

	std::vector<Complex> Roots = solverFactory(p)->getRoots();
	std::sort(Roots.begin(), Roots.end());

	for (const auto& el : Roots)
		std::cout << el << '\n';
}	