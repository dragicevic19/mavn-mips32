/* Autor: Marko Dragicevic Datum: 06.06.2021. */

#pragma once
#include "IR.h"
#include <fstream>
#include "SyntaxAnalysis.h"

void createOutFile(std::string fileName, Instructions& instrs, Variables vars, Labels labels, std::string globalFunc);

void openFile(std::string fileName, std::ofstream& out);

void writeInstruction(Instruction*& instr, std::ofstream& out);

void writeMemVars(Variables vars, Variables& memVars, std::ofstream& out);
