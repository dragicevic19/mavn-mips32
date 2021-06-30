/* Autor: Marko Dragicevic Datum: 06.06.2021. */
#pragma once

#include "IR.h"

/**
 * Performs simplification of the interference graph by removing variable nodes from it.
 * @param - ig
 *	Reference of the interference graph
 * @param - degree
 *	Number of available registers
 * @return
 *	Reference to the simplification stack
 */
SimplificationStack& doSimplification(InterferenceGraph ig, int degree);

void findMaxRang(InterferenceGraph& ig, SimplificationStack& simpStack, int degree, Variables&);

void removeVertexFromGraphAndPushToStack(int position, InterferenceMatrix& matrix, Variables& vars, SimplificationStack& simpStack, int matrixSize);

Variable* getVarByPosition(int position, Variables& vars);

void pushAllRemainingVarsToStack(Variables&, SimplificationStack&);

void emptyStack(SimplificationStack&);

void copyVariables(Variables*, Variables&);