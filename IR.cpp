/* Autor: Marko Dragicevic Datum: 06.06.2021. */

#include "IR.h"
#include "Types.h"

using namespace std;


Variable::Variable() : m_type(NO_TYPE), m_name(""), m_position(-1), m_assignment(no_assign) {}

Variable::Variable(std::string name, int pos) : m_type(NO_TYPE), m_name(name), m_position(pos), m_assignment(no_assign) {}

Variable::Variable(VariableType type, std::string name, int pos) : m_type(type), m_name(name), m_position(pos), m_assignment(no_assign) {}

Variable::Variable(VariableType type, string name, int value, int pos)
	: m_type(type), m_name(name), m_value(value), m_position(pos), m_assignment(no_assign) {}

std::string Variable::getName()
{
	return m_name;
}

Variable::VariableType Variable::getType()
{
	return m_type;
}

int Variable::getPos()
{
	return m_position;
}

Regs Variable::getAssignment()
{
	return m_assignment;
}

int Variable::getValue()
{
	return m_value;
}

string Variable::assignmentToReg()
{
	switch (m_assignment)
	{
	case no_assign:
		return "greska - no_assign";
	case 1:
		return "t0";
	case 2:
		return "t1";
	case 3:
		return "t2";
	case 4:
		return "t3";
	default:
		return "error";
	}
}

void Variable::setAssignment(Regs i)
{
	m_assignment = i;
}

/*********************************************************************************************************/


Instruction::Instruction() : m_position(0), m_type(I_NO_TYPE) {}

Instruction::Instruction(int pos, InstructionType type, Variables& dst, Variables& src) 
	: m_position(pos), m_type(type), m_dst(dst), m_src(src) {}

void Instruction::setPos(int pos)
{
	m_position = pos;
}

void Instruction::setType(InstructionType type)
{
	m_type = type;
}

void Instruction::setDst(Variables& dst)
{
	m_dst = dst;
}

void Instruction::setSrc(Variables& src)
{
	m_src = src;
}

void Instruction::setLabelName(string name)
{
	m_labelName = name;
}

string Instruction::getLabelName()
{
	return m_labelName;
}

Variables& Instruction::getDst()
{
	return m_dst;
}

Variables& Instruction::getSrc()
{
	return m_src;
}

Variables& Instruction::getDef()
{
	return m_def;
}

int Instruction::getPosition()
{
	return m_position;
}

InstructionType Instruction::getType()
{
	return m_type;
}

Variables& Instruction::getUse()
{
	return m_use;
}

Variables& Instruction::getIn()
{
	return m_in;
}

Variables& Instruction::getOut()
{
	return m_out;
}


std::list<Instruction*>& Instruction::getSucc()
{
	return m_succ;
}

std::list<Instruction*>& Instruction::getPred()
{
	return m_pred;
}


void Instruction::printInstruction()
{
	cout << m_position << endl;
	cout << "Type: " << typeToString() << endl << endl;

	cout << "Destination: \t";
	for (auto var : m_dst)
		cout << var->getName() << "\t";

	cout << endl << "Source: \t";
	for (auto var : m_src)
		cout << var->getName() << "\t";

	cout << endl << endl << "PRED:";
	for (auto instr : m_pred)
		cout << "\t" << instr->typeToString();

	cout << endl << endl << "SUCC:";
	for (auto instr : m_succ)
		cout << "\t" << instr->typeToString();

	cout << endl << endl << "USE: \t";
	for (auto var : m_use)
		cout << var->getName() << "\t";

	cout << endl << endl << "DEF: \t";
	for (auto var : m_def)
		cout << var->getName() << "\t";

	cout << endl << endl << "IN:";
	for (auto var : m_in)
		cout << "\t" << var->getName();

	cout << endl << endl << "OUT:";
	for (auto var : m_out)
		cout << "\t" << var->getName();

	cout << endl;
}

string Instruction::typeToString()
{
	switch (m_type)
	{
	case I_NO_TYPE:			return "[I_NO_TYPE]";
	case I_ADD:				return "[I_ADD]";
	case I_ADDI:			return "[I_ADDI]";
	case I_SUB:				return "[I_SUB]";
	case I_LA:				return "[I_LA]";
	case I_LI:				return "[I_LI]";
	case I_LW:				return "[I_LW]";
	case I_SW:				return "[I_SW]";
	case I_BLTZ:			return "[I_BLTZ]";
	case I_B:				return "[I_B]";
	case I_ABS:				return "[I_ABS]";
	case I_OR:				return "[I_OR]";
	case I_SEQ:				return "[I_SEQ]";
	case I_NOP:				return "[I_NOP]";
	}
}

string Instruction::typeToCodeString()
{
	switch (m_type)
	{
	case I_NO_TYPE:			return "ERROR - NO TYPE";
	case I_ADD:				return "add";
	case I_ADDI:			return "addi";
	case I_SUB:				return "sub";
	case I_LA:				return "la";
	case I_LI:				return "li";
	case I_LW:				return "lw";
	case I_SW:				return "sw";
	case I_BLTZ:			return "bltz";
	case I_B:				return "b";
	case I_ABS:				return "abs";
	case I_OR:				return "or";
	case I_SEQ:				return "seq";
	case I_NOP:				return "nop";
	}
}


Variables* InterferenceGraph::getVariables()
{
	return m_variables;
}

Variables InterferenceGraph::getCopyVariables()
{
	return *m_variables;
}

InterferenceMatrix& InterferenceGraph::getMatrix()
{
	return m_matrix;
}


