/* Autor: Marko Dragicevic Datum: 06.06.2021. */

#include "LivenessAnalysis.h"

using namespace std;

void livenessAnalysis(Instructions& instructions)
{
	Instructions::reverse_iterator it;
	bool change = true;
	for (Instruction* instr : instructions)
	{
		instr->getIn().clear();
		instr->getOut().clear();
	}

	bool ponovi = true;

	while (ponovi)
	{
		ponovi = false;

		for (it = instructions.rbegin(); it != instructions.rend(); it++)
		{
			Instruction* instr = *it;

			Variables in = instr->getIn();
			Variables out = instr->getOut();

			for (Instruction* instruct : instr->getSucc())
				for (Variable* inVar : instruct->getIn())
					if (inVar->getType() == Variable::REG_VAR)
						instr->getOut().push_back(inVar);

			for (Variable* useVar : instr->getUse())
				if (useVar->getType() == Variable::REG_VAR)
					instr->getIn().push_back(useVar);

			for (Variable* outVar : instr->getOut())
			{
				bool found = find(instr->getDef().begin(), instr->getDef().end(), outVar) != instr->getDef().end();
				if (!found)
				{
					if (outVar->getType() == Variable::REG_VAR)
						instr->getIn().push_back(outVar);
				}
			}

			instr->getIn().sort();
			instr->getIn().unique();

			instr->getOut().sort();
			instr->getOut().unique();

			if (instr->getIn() != in || instr->getOut() != out) ponovi = true;
		}
	}
}
