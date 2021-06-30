/* Autor: Marko Dragicevic Datum: 06.06.2021. */

#pragma once
#include "IR.h"

/**
 * Builds the interference graph based on instruction list of the analyzed program.
 * @param - instructions
 *	List of instructions
 * @return
 *	Reference to the interference graph
 */
InterferenceGraph& buildInterferenceGraph(Instructions& instructions, Variables& vars);

void resizeMatrix(Variables&);

/**
 * Prints the interference graph
 */
void printInterferenceMatrix();

bool contains(Variables, Variable*);