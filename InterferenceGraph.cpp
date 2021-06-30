/* Autor: Marko Dragicevic Datum: 06.06.2021. */

#include "InterferenceGraph.h"

using namespace std;

static InterferenceGraph ig;

void resizeMatrix(Variables& vars)	// da bih dodavao smetnje u vektor moram prvo da ga resize()
{
	int varsCounter = 0;

	for (auto& var : vars)
	{
		if (var->getType() == Variable::REG_VAR)
		{
			varsCounter++;
			ig.getVariables()->push_back(var);
		}
	}

	ig.getMatrix().resize(varsCounter);

	for (auto& row : ig.getMatrix())
	{
		row.resize(varsCounter);
	}
}

InterferenceGraph& buildInterferenceGraph(Instructions& instructions, Variables& vars)
{
	resizeMatrix(vars);

	for (Instruction* instr : instructions)
	{
		for (Variable* defVar : instr->getDef())
		{
			for (Variable* var : instr->getOut())
			{
				if (var != defVar)
				{
					ig.getMatrix()[defVar->getPos()][var->getPos()] = __INTERFERENCE__;
					ig.getMatrix()[var->getPos()][defVar->getPos()] = __INTERFERENCE__;
				}
			}
		}
	}

	ig.getVariables()->sort([](Variable* v1, Variable* v2) {return v1->getPos() < v2->getPos(); });
	return ig;
}

bool contains(Variables variables, Variable* var)
{
	for (Variable* v : variables)
		if (v == var) return true;
	return false;
}

void printInterferenceMatrix()
{
	if (ig.getMatrix().size() == 0)
	{
		cout << "There is nothing to print!" << endl;
		return;
	}

	cout << "==========================================" << endl;
	cout << "Interference matrix:" << endl;
	cout << "==========================================" << endl;

	// print existing variables in order to mark rows and columns
	for (auto varIt = ig.getVariables()->begin(); varIt != ig.getVariables()->end(); varIt++)
	{
		cout << "\t" << (*varIt)->getName();
	}
	cout << endl;

	auto varIt = ig.getVariables()->begin();
	for (auto it1 = ig.getMatrix().begin(); it1 != ig.getMatrix().end(); it1++)
	{
		cout << (*varIt++)->getName();
		for (auto it2 = (*it1).begin(); it2 != (*it1).end(); it2++)
		{
			cout << "\t" << *it2;
		}
		cout << endl;
	}
	cout << "==========================================" << endl;
}