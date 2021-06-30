/* Autor: Marko Dragicevic Datum: 06.06.2021. */

#include "Output.h"
#include "SyntaxAnalysis.h"

using namespace std;

void createOutFile(string fileName, Instructions& instrs, Variables vars, Labels labels, string funcs)
{
	ofstream out;
	openFile(fileName, out);

	// ime funkcije kao .globl
	out << ".globl " << funcs << endl << endl;

	// .data ako ima MEM promenljivih
	Variables memVars;
	writeMemVars(vars, memVars, out);

	// .text
	out << endl << ".text" << endl;

	// labela kao ime funckije iz .globl
	out << funcs << ":" << endl;

	int instrCounter = 0;
	for (auto& instr : instrs)
	{
		string label = getLabelPointingOnThisInstr(instrCounter, labels);
		if (label.size() > 0)  out << label << ":" << endl;
		writeInstruction(instr, out);
		instrCounter++;
	}
}

void writeMemVars(Variables vars, Variables& memVars, std::ofstream& out)
{
	for (auto& var : vars)
	{
		if (var->getType() == Variable::MEM_VAR)
			memVars.push_back(var);
	}
	if (memVars.size() > 0)
	{
		out << ".data" << endl;
		for (auto& var : memVars)
		{
			out << var->getName() << ":\t.word " << var->getValue() << endl;
		}
	}
}

void writeInstruction(Instruction*& instr, std::ofstream& out)
{
	InstructionType iType = instr->getType();
	switch (iType)
	{
	case I_ADD:
	case I_SUB:
	case I_OR:
	case I_SEQ:
	{
		string dest = instr->getDst().front()->assignmentToReg();
		instr->getDst().pop_front();
		string src1 = instr->getSrc().front()->assignmentToReg();
		instr->getSrc().pop_front();
		string src2 = instr->getSrc().front()->assignmentToReg();
		instr->getSrc().pop_front();

		out << "\t" << instr->typeToCodeString() << "\t" << "$" << dest << ", $" << src1 << ", $" << src2 << endl;
		break;
	}
	case I_ADDI:
	{
		string dest = instr->getDst().front()->assignmentToReg();
		instr->getDst().pop_front();
		string src1 = instr->getSrc().front()->assignmentToReg();
		instr->getSrc().pop_front();
		instr->getUse().pop_front();
		int src2 = instr->getUse().front()->getValue();
		instr->getUse().pop_front();

		out << "\t" << instr->typeToCodeString() << "\t" << "$" << dest << ", $" << src1 << ", " << src2 << endl;
		break;
	}
	case I_LA:
	{
		string dest = instr->getDst().front()->assignmentToReg();
		instr->getDst().pop_front();
		string src1 = instr->getSrc().front()->getName();
		instr->getSrc().pop_front();

		out << "\t" << instr->typeToCodeString() << "\t" << "$" << dest << ", " << src1 << endl;
		break;
	}
	case I_LI:
	{
		string dest = instr->getDst().front()->assignmentToReg();
		instr->getDst().pop_front();
		int src1 = instr->getUse().front()->getValue();
		instr->getUse().pop_front();

		out << "\t" << instr->typeToCodeString() << "\t" << "$" << dest << ", " << src1 << endl;
		break;
	}
	case I_LW:
	{
		string dest = instr->getDst().front()->assignmentToReg();
		instr->getDst().pop_front();
		int src1 = instr->getUse().front()->getValue();
		instr->getUse().pop_front();
		string dst2 = instr->getDst().front()->assignmentToReg();
		instr->getDst().pop_front();
		out << "\t" << instr->typeToCodeString() << "\t" << "$" << dest << ", " << src1 << "($" << dst2 << ")" << endl;
		break;
	}
	case I_SW:
	{
		string src = instr->getSrc().front()->assignmentToReg();
		instr->getSrc().pop_front();
		instr->getUse().pop_front();

		int src1 = instr->getUse().front()->getValue();
		instr->getUse().pop_front();
		string dest2 = instr->getDst().front()->assignmentToReg();
		instr->getDst().pop_front();

		out << "\t" << instr->typeToCodeString() << "\t" << "$" << src << ", " << src1 << "($" << dest2 << ")" << endl;
		break;
	}
	case I_BLTZ:
	{
		string src = instr->getSrc().front()->assignmentToReg();
		instr->getSrc().pop_front();
		string label = instr->getLabelName();

		out << "\t" << instr->typeToCodeString() << "\t" << "$" << src << ", " << label << endl;
		break;
	}
	case I_B:
	{
		string label = instr->getLabelName();
		out << "\t" << instr->typeToCodeString() << "\t" << label << endl;
		break;
	}
	case I_ABS:
	{
		string dest = instr->getDst().front()->assignmentToReg();
		instr->getDst().pop_front();
		string src = instr->getSrc().front()->assignmentToReg();
		instr->getSrc().pop_front();

		out << "\t" << instr->typeToCodeString() << "\t" << "$" << dest << ", $" << src << endl;
		break;
	}
	case I_NOP:
	{
		out << "\t" << "nop" << endl;
		break;
	}
	default:
	{
		cout << "ERROR";
		break;
	}
	}
}

void openFile(std::string fileName, ofstream& out)
{
	out.open(fileName, ios::out);

	if (!out)
	{
		cout << "Greska pri otvaranju fajla: " << fileName << endl;
		exit(1);
	}
}