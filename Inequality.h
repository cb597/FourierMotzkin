#include<vector>

class Inequality
{
public:
	Inequality(std::vector<double> a_, double b_, std::size_t m, std::size_t idx) : a(a_), b(b_), history(m, 0.0) { history[idx] = 1.; };
	std::vector<double> a;
	double b;
	Inequality Inequality::operator+(const Inequality& arg) const;
	Inequality pop_front() { Inequality p(*this); p.a.erase(p.a.begin()); return p; }
	void Inequality::normalize();
	std::vector<double> history;
};