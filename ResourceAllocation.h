/* Autor: Marko Dragicevic Datum: 06.06.2021. */

#pragma once
#ifndef __RESOURCE_ALLOCATION__
#define __RESOURCE_ALLOCATION__

#include "IR.h"
#include "InterferenceGraph.h"

#include <stack>


/**
 * Use this function to do resource allocation algorithm.
 * @param simplificationStack pointer to simplification stack
 * @param ig pointer to interference graph
 * @return true if no spill is detected during resource allocation algorithm, false actual spill detected
 */
bool doResourceAllocation(SimplificationStack& ss, InterferenceGraph& ig);

#endif
