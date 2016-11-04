#include "FourierMotzkinEliminator.h"
#include <math.h>
#include <limits>
#include <algorithm>
#include <iostream>


double operator*(const std::vector<double> lhs, const std::vector<double> rhs)  // scalar product
{
	double scalarproduct = 0.;
	if (lhs.size() != rhs.size())
	{
		throw std::exception("scalar multiplication failed due to incompatible vector sizes");
	}
	for (std::size_t i = 0; i < lhs.size(); ++i)
	{
		scalarproduct += lhs[i] * rhs[i];
	}
	return scalarproduct;
}



FourierMotzkinEliminator::FourierMotzkinEliminator(LP lp_)
{
	inequalitySets.push_back(InequalitySet());
	for (std::size_t i = 0; i < lp_.m; ++i)
	{
		inequalitySets.front().push_back(Inequality(lp_.A[i], lp_.b[i], lp_.m, i));
	}
	n = lp_.n;
}

BoolDVectorPair FourierMotzkinEliminator::run()
{
	std::vector<double> x(1, 0.);
	std::size_t stepsleft = inequalitySets.back()[0].a.size();
	while (run_elimination_step(stepsleft) > 0);
	BoolDVectorPair result = feasibility_check();
	if (result.first)
	{
		while (run_construct_solution_step(x));
		x.erase(x.begin());
		return BoolDVectorPair(true, x);
	}
	else
	{
		return result;
	}
}

Partition FourierMotzkinEliminator::partition_rows(const InequalitySet A) const // L|U|N
{
	std::vector<std::vector<std::size_t> > part(3);
	for (std::size_t i = 0; i < A.size(); ++i)
	{
		int leading_coefficient = (int) round(A.at(i).a.front());
		switch (leading_coefficient)
		{
		case -1:
			part[0].push_back(i);
			break;
		case 1:
			part[1].push_back(i);
			break;
		case 0:
			part[2].push_back(i);
			break;
		default:
			throw std::exception("wtf alter lass den scheiss");
			break;
		}
	}
	return part;
}

int FourierMotzkinEliminator::run_elimination_step(std::size_t& stepsleft)
{
	InequalitySet& current = inequalitySets.back();
	for (auto &ineq : current)
	{
		ineq.normalize();
	}

	Partition partition = partition_rows(current);

	InequalitySet next;
	for (auto p : partition[0])
	{
		for (auto q : partition[1])
		{
			Inequality n = current[p] + current[q];
			next.push_back(n.pop_front());
		}
	}
	for (auto n : partition[2])
	{
		next.push_back(current[n].pop_front());
	}

	inequalitySets.push_back(next);
	return --stepsleft;
}

bool FourierMotzkinEliminator::run_construct_solution_step(std::vector<double>& x)
{
	InequalitySet& current = inequalitySets.back();
	for (auto &ineq : current)
	{
		ineq.normalize();
	}
	Partition partition = partition_rows(current);
	double left = std::numeric_limits<double>::lowest();
	double right = std::numeric_limits<double>::max();

	for (auto l : partition[0])
	{
		auto val = current[l].a * x - current[l].b;
		left = std::max(left, val);
	}
	for (auto r : partition[1])
	{
		auto val = current[r].b - current[r].a * x;
		right = std::min(right, val);
	}

	if (left > right)
	{
		std::cout << "empty";
	}

	double solution = left / 2 + right / 2;
	x.insert(x.begin() + 1, solution);

	inequalitySets.pop_back();
	return x.size() <= n;
}

BoolDVectorPair FourierMotzkinEliminator::feasibility_check()
{
	for (auto i : inequalitySets.back())
	{
		if (i.a.size() != 0)
		{
			throw std::exception("feasability check only works for variableless inequalities");
		}
		if (i.b < 0)
		{
			return BoolDVectorPair(false, i.history);
		}
	}
	inequalitySets.pop_back();
	return BoolDVectorPair(true, std::vector<double>());
}
