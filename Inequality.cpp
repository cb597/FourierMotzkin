#include "Inequality.h"

std::vector<double> operator+(const std::vector<double> lhs, const std::vector<double> rhs)
{
	if (lhs.size() != rhs.size())
	{
		throw std::exception("invalid vector size for addition");
	}
	std::vector<double> result(lhs.size(), 0.);
	for (std::size_t i = 0; i < lhs.size(); ++i)
	{
		result[i] = lhs[i] + rhs[i];
	}
	return result;
}

std::vector<double> operator/(const std::vector<double> lhs, double rhs)
{
	std::vector<double> result;
	for (auto i : lhs)
	{
		result.push_back(i / rhs);
	}
	return result;
}

Inequality Inequality::operator+(const Inequality& arg) const  // addition
{
	Inequality result(*this);
	for (std::size_t i = 0; i < result.a.size(); ++i)
	{
		result.a[i] += arg.a[i];
	}
	result.b += arg.b;
	result.history = history + arg.history;
	return result;
}

void Inequality::normalize()
{
	if (a.size() < 1) return;
	double lambda = abs(a.front());
	if (lambda != 1 && lambda != 0)
	{
		for (auto &v : a)
		{
			v /= lambda;
		}
		b /= lambda;
		history = history / lambda;
	}
}