/* Autor: Marko Dragicevic Datum: 06.06.2021. */

#include "ResourceAllocation.h"
#include "InterferenceGraph.h"
#include "Constants.h"
#include <iostream>

using namespace std;


bool doResourceAllocation(SimplificationStack& ss, InterferenceGraph& ig)
{

	while (!ss.empty())
	{
		bool error = true;
		Variable* var = ss.top();
		ss.pop();
		bool regs[__REG_NUMBER__] = { true, true, true, true };

		for (Variable* graphVar : *(ig.getVariables()))
		{
			if (ig.getMatrix()[var->getPos()][graphVar->getPos()] == __INTERFERENCE__)
			{
				if (graphVar->getAssignment() != no_assign)
					regs[graphVar->getAssignment()-1] = false; // oznacava da registar dodeljen graphVar promenljivoj ne moze da se koristi
			}
		}

		for (int i = 0; i < __REG_NUMBER__; i++) // dodeljujem odgovarajuci registar promenljivoj sa steka
		{
			if (regs[i])
			{
				var->setAssignment((Regs)(i+1)); // i+1 zato sto je no_assign = 0
				error = false;
				break;
			}
		}

		if (error) return false;
	}
	return true;
}