/* Autor: Marko Dragicevic Datum: 06.06.2021. */

#include "Simplification.h"

using namespace std;

bool endIt = false;
bool spill = false;

void copyVariables(Variables* vars, Variables& copyVars)
{
	for (auto& var : *vars)
		copyVars.push_back(var);

}

SimplificationStack& doSimplification(InterferenceGraph ig, int degree)
{
	endIt = false;
	spill = false;
	Variables vars;
	copyVariables(ig.getVariables(), vars); // da ne bih brisao promenljive iz grafa
	SimplificationStack* simpStack = new SimplificationStack;

	while (vars.size() > 0 && !endIt && !spill)
	{
		findMaxRang(ig, *simpStack, degree, vars);
	}

	return *simpStack;
}

void findMaxRang(InterferenceGraph& igCopy, SimplificationStack& simpStack, int degree, Variables& vars)
{
	int maxi = -1;
	int position = 0;

	InterferenceMatrix& matrix = igCopy.getMatrix();
	int matrixSize = matrix.size();

	for (int i = 0; i < matrixSize; i++)
	{
		int interfCounter = 0;
		int j = 0;
		
		for (; j < matrixSize; j++)
		{
			if (matrix[i][j] == __INTERFERENCE__) interfCounter++;
		}

		if (interfCounter > maxi)
		{
			if (interfCounter < degree)
			{
				maxi = interfCounter;
				position = i;
				spill = false;
			}
			else if(maxi == 0)
			{
				spill = true;
			}
		}
	}
	if (maxi == 0)
	{
		if (spill) emptyStack(simpStack);									//greska!
		else pushAllRemainingVarsToStack(vars, simpStack); // ok (niko vise nije u smetnji)

		endIt = true;														// ali svakako se ovde zavrsava simplification
		return;
	}
	removeVertexFromGraphAndPushToStack(position, matrix, vars, simpStack, matrixSize);
}

void pushAllRemainingVarsToStack(Variables& vars, SimplificationStack& simpStack)
{
	for (auto& var : vars)
	{
		simpStack.push(var);
	}
}

void removeVertexFromGraphAndPushToStack(int position, InterferenceMatrix& matrix, Variables& vars, SimplificationStack& simpStack, int matrixSize)
{
	bool deleteAllInterf = false;

	for (int i = 0; i < matrixSize; i++)
	{
		if (i == position) deleteAllInterf = true;

		for (int j = 0; j < matrixSize; j++)
		{
			if (deleteAllInterf) matrix[i][j] = __EMPTY__;
			else if (j == position) matrix[i][j] = __EMPTY__;
		}
		deleteAllInterf = false;
	}

	Variable* var = getVarByPosition(position, vars);
	simpStack.push(var);
	vars.remove(var);	// brisem iz liste promenljivih (vrv ne treba odavde da se brise)
}

Variable* getVarByPosition(int position, Variables& vars)
{
	Variable* retVar = nullptr;

	for (auto& var : vars)
	{
		if (var->getPos() == position)
		{
			retVar = var;
			break;
		}
	}
	return retVar;
}

void emptyStack(SimplificationStack& ss)
{
	while (!ss.empty())
	{
		ss.pop();
	}
}