#include <iostream>
#include <cmath>
#include <string>
#include <sstream>

int square(int x)
{
	return x * x;
}

class Complex
{
private:
	int mReal, mImaginary;

public:
	Complex() = default;
	Complex(int, int);
	Complex operator+(const Complex&) const;
	Complex operator-(const Complex&) const;
	Complex operator*(const Complex&) const;
	friend std::istream& operator>>(std::istream&, Complex&); 
	friend std::ostream& operator<<(std::ostream&, const Complex&);
};

Complex::Complex(int real, int imaginary) : mReal(real), mImaginary(imaginary)
{

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
	while(!expression.empty() && (expression.back() == 'i' || expression.back() == ' '))
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
	
	int coef;
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
	
	if (abs(complexNum.mImaginary) != 1)
		out << abs(complexNum.mImaginary);

	out << "i";

	return out;
}

int main()
{
	Complex a, b, c;
	std::cin >> a >> b;
	
	char oper;
	std::cin >> oper;

	if (oper == '+')
		c = a + b;
	else if (oper == '-')
		c = a - b;
	else 
		c = a * b;

	std::cout << c << '\n';
}