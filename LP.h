#include <string>
#include <vector>

class LP
{
public:
	void read_file(std::string);
	LP();
	std::size_t m, n; //row and col count
	std::vector<double> c; //cost vector
	std::vector<double> b; //righthand side of constraints
	std::vector<std::vector<double> > A; //Matrix
};