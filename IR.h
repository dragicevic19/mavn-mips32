/* Autor: Marko Dragicevic Datum: 06.06.2021. */

#ifndef __IR__
#define __IR__

#include "Types.h"


/**
 * This class represents one variable from program code.
 */
class Variable
{
public:
	enum VariableType
	{
		MEM_VAR,
		REG_VAR,
		CONST,
		NO_TYPE
	};

	Variable(); 
	Variable(std::string name, int pos);
	Variable(VariableType type, std::string name, int pos);
	Variable(VariableType type, std::string name, int value, int pos);

	std::string getName();
	VariableType getType();
	int getPos();
	Regs getAssignment();
	int getValue();
	std::string assignmentToReg();

	void setAssignment(Regs i);

private:
	VariableType m_type;
	std::string m_name;
	int m_value;
	int m_position;
	Regs m_assignment;
};

/**
 * This type represents list of variables from program code.
 */
typedef std::list<Variable*> Variables;


/**
 * This class represents one instruction in program code.
 */
class Instruction
{
public:
	Instruction();
	Instruction(int pos, InstructionType type, Variables& dst, Variables& src);

	void setPos(int pos);
	void setType(InstructionType type);
	void setDst(Variables& dst);
	void setSrc(Variables& src);
	void setLabelName(std::string name);

	int getPosition();
	InstructionType getType();
	Variables& getDst();
	Variables& getSrc();
	Variables& getUse();
	Variables& getDef();
	Variables& getIn();
	Variables& getOut();
	std::list<Instruction*>& getSucc();
	std::list<Instruction*>& getPred();
	std::string getLabelName();


	void printInstruction();
	std::string typeToString();
	std::string typeToCodeString();

private:
	int m_position;
	InstructionType m_type;
	
	Variables m_dst;
	Variables m_src;

	Variables m_use;
	Variables m_def;
	Variables m_in;
	Variables m_out;
	std::list<Instruction*> m_succ;
	//vector<int> m_succNumbers; // u ovaj vektor ubacujem pozicije instrukcija posle bltz ili b instrukcije jer se mozda skok desava pre same instrukcije

	std::list<Instruction*> m_pred;
	std::string m_labelName = "";	// smestam naziv labele na koju bltz ili b instrukcije skacu
};

/**
 * This type represents list of instructions from program code.
 */
typedef std::list<Instruction*> Instructions;


/**
 * This type represents matrix of integers.
 */
typedef std::vector<std::vector<int>> InterferenceMatrix;


/**
 * This type represents stack of pointers to variables.
 */
typedef std::stack<Variable*> SimplificationStack;

/**
 * This type represents interference graph.
 */
class InterferenceGraph
{
private:
	InterferenceMatrix m_matrix;	// interference matrix
	Variables* m_variables;		// all variables from instructions
public:
	/**
	 * Default construction.
	 */
	InterferenceGraph()
	{
		m_variables = new Variables;
	}

	Variables* getVariables();
	Variables getCopyVariables();
	InterferenceMatrix& getMatrix();
};
#endif
