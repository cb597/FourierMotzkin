#include "LP.h"
#include "Inequality.h"

typedef std::vector<Inequality> InequalitySet;
typedef std::vector<std::vector<std::size_t> > Partition;
typedef std::pair<bool, std::vector<double> > BoolDVectorPair;

class FourierMotzkinEliminator
{
public:
	FourierMotzkinEliminator(LP lp_);
	BoolDVectorPair run();
private:
	std::size_t n;
	Partition partition_rows(InequalitySet const) const;
	std::vector<InequalitySet> inequalitySets;
	int run_elimination_step(std::size_t&);
	bool run_construct_solution_step(std::vector<double>&);
	BoolDVectorPair feasibility_check();
};