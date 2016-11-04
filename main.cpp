#include <iostream>
#include "FourierMotzkinEliminator.h"

int main(int argc, char* argv[])
{
	LP lp;
	lp.read_file("inst1.txt");
	FourierMotzkinEliminator fme(lp);
	BoolDVectorPair result = fme.run();

	if (!result.first)
	{
		std::cout << "empty ";
	}
	for (auto i : result.second)
	{
		std::cout << i << " ";
	}
}