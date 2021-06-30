/* Autor: Marko Dragicevic Datum: 06.06.2021. */

#include "SyntaxAnalysis.h"

#include <iostream>
#include <iomanip>

using namespace std;

SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex, Instructions& instrs, Variables& vars)
	: m_lexicalAnalysis(lex), m_errorFound(false), m_tokenIterator(m_lexicalAnalysis.getTokenList().begin()), m_instructions(instrs), m_variables(vars)
{
	m_varPosition = 0;
	m_instrPosition = 0;
}


bool SyntaxAnalysis::Do()
{
	m_currentToken = getNextToken();

	Q();

	return !m_errorFound;
}


void SyntaxAnalysis::printSyntaxError(Token token)
{
	cout << "Syntax error! Token: " << token.getValue() << " unexpected" << endl;
}


void SyntaxAnalysis::printTokenInfo(Token token)
{
	cout << setw(20) << left << token.tokenTypeToString(token.getType());
	cout << setw(25) << right << token.getValue() << endl;
}


void SyntaxAnalysis::eat(TokenType t)
{
	if (m_errorFound == false)
	{
		if (m_currentToken.getType() == t)
		{
			if (m_currentToken.getType() != T_END_OF_FILE)
				m_currentToken = getNextToken();
		}
		else
		{
			printSyntaxError(m_currentToken);
			m_errorFound = true;
		}
	}
}


Token SyntaxAnalysis::getNextToken()
{
	if (m_tokenIterator == m_lexicalAnalysis.getTokenList().end())
		throw runtime_error("End of input file reached");
	return *m_tokenIterator++;
}


void SyntaxAnalysis::Q()
{
	if (m_errorFound) return;
	S();
	eat(T_SEMI_COL);
	L();
}


void SyntaxAnalysis::S()
{
	if (m_currentToken.getType() == T_MEM)
	{
		eat(T_MEM);
		string varName = m_currentToken.getValue();
		Variable* var = findVar(varName, m_variables);
		if (var)
		{
			m_errorFound = true;
			cout << "ERROR - Variable " << var->getName() << " already defined" << endl;
			return;
		}
		eat(T_M_ID);
		m_variables.push_back(new Variable(Variable::MEM_VAR, varName, stoi(m_currentToken.getValue()), -1));	// -1 pozicija jer mi ne treba MEM_VAR u grafu smetnji
		eat(T_NUM);
	}
	else if (m_currentToken.getType() == T_REG)
	{
		eat(T_REG);

		Variable* var = findVar(m_currentToken.getValue(), m_variables);
		if (var)
		{
			m_errorFound = true;
			cout << "ERROR - Variable " << var->getName() << " already defined" << endl;
			return;
		}
		m_variables.push_back(new Variable(Variable::REG_VAR, m_currentToken.getValue(), m_varPosition++));
		eat(T_R_ID);
	}
	else if (m_currentToken.getType() == T_FUNC)
	{
		eat(T_FUNC);
		m_globalFunc = m_currentToken.getValue();
		eat(T_ID);
	}
	else if (m_currentToken.getType() == T_ID)
	{
		m_labelWithInstrPosition[m_currentToken.getValue()] = m_instrPosition; // zapamti labelu i njenu prvu instrukciju
		eat(T_ID);
		eat(T_COL);
		E();
	}
	else
	{
		E();
	}
}


void SyntaxAnalysis::L()
{
	if (m_currentToken.getType() == T_END_OF_FILE)
		eat(T_END_OF_FILE);
	else
		Q();
}


void SyntaxAnalysis::E()
{
	vector<string> destVars;
	vector<string> srcVars; // u kreiranju instrukcije cu preko naziva ovih promenljivih da nadjem njihov objekat Variable

	if (m_currentToken.getType() == T_ADD)
	{
		eat(T_ADD);
		destVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		srcVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		srcVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		makeInstruction(I_ADD, destVars, srcVars, m_instrPosition++);
	}
	else if (m_currentToken.getType() == T_ADDI)
	{
		eat(T_ADDI);
		destVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		srcVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		srcVars.push_back(m_currentToken.getValue());
		m_variables.push_back(new Variable(Variable::CONST, m_currentToken.getValue(), stoi(m_currentToken.getValue()), -1));
		eat(T_NUM);
		makeInstruction(I_ADDI, destVars, srcVars, m_instrPosition++);
	}
	else if (m_currentToken.getType() == T_SUB)
	{
		eat(T_SUB);
		destVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		srcVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		srcVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		makeInstruction(I_SUB, destVars, srcVars, m_instrPosition++);
	}
	else if (m_currentToken.getType() == T_LA)
	{
		eat(T_LA);
		destVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		srcVars.push_back(m_currentToken.getValue());
		eat(T_M_ID);
		makeInstruction(I_LA, destVars, srcVars, m_instrPosition++);
	}
	else if (m_currentToken.getType() == T_LW)
	{
		eat(T_LW);
		destVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		srcVars.push_back(m_currentToken.getValue());
		m_variables.push_back(new Variable(Variable::CONST, m_currentToken.getValue(), stoi(m_currentToken.getValue()), -1));
		eat(T_NUM);
		eat(T_L_PARENT);	
		destVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		eat(T_R_PARENT);
		makeInstruction(I_LW, destVars, srcVars, m_instrPosition++);
	}
	else if (m_currentToken.getType() == T_LI)
	{
		eat(T_LI);
		destVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		srcVars.push_back(m_currentToken.getValue());
		m_variables.push_back(new Variable(Variable::CONST, m_currentToken.getValue(), stoi(m_currentToken.getValue()), -1));
		eat(T_NUM);
		makeInstruction(I_LI, destVars, srcVars, m_instrPosition++);
	}
	else if (m_currentToken.getType() == T_SW)
	{
		eat(T_SW);
		srcVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		srcVars.push_back(m_currentToken.getValue());
		m_variables.push_back(new Variable(Variable::CONST, m_currentToken.getValue(), stoi(m_currentToken.getValue()), -1));
		eat(T_NUM);
		eat(T_L_PARENT);
		destVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		eat(T_R_PARENT);
		makeInstruction(I_SW, destVars, srcVars, m_instrPosition++);
	}
	else if (m_currentToken.getType() == T_B)
	{
		eat(T_B);
		destVars.push_back(m_currentToken.getValue());
		eat(T_ID);
		makeInstruction(I_B, destVars, srcVars, m_instrPosition++);
	}
	else if (m_currentToken.getType() == T_BLTZ)
	{
		eat(T_BLTZ);
		srcVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		destVars.push_back(m_currentToken.getValue());
		eat(T_ID);
		makeInstruction(I_BLTZ, destVars, srcVars, m_instrPosition++);
	}

	// sledece tri instrukcije su dodate
	else if (m_currentToken.getType() == T_ABS)
	{
		eat(T_ABS);
		destVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		srcVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		makeInstruction(I_ABS, destVars, srcVars, m_instrPosition++);
	}
	else if (m_currentToken.getType() == T_OR)
	{
		eat(T_OR);
		destVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		srcVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		srcVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		makeInstruction(I_OR, destVars, srcVars, m_instrPosition++);
	}
	else if (m_currentToken.getType() == T_SEQ)
	{
		eat(T_SEQ);
		destVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		srcVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		srcVars.push_back(m_currentToken.getValue());
		eat(T_R_ID);
		makeInstruction(I_SEQ, destVars, srcVars, m_instrPosition++);
	}
	else
	{
		eat(T_NOP);
		makeInstruction(I_NOP, destVars, srcVars, m_instrPosition++);
	}
}

void SyntaxAnalysis::makeInstruction(InstructionType instrType, vector<string>& destVars, vector<string>& srcVars, int instrPos)
{
	if (m_errorFound) return;

	Instruction* instr = new Instruction();
	instr->setPos(instrPos);
	instr->setType(instrType);

	bool retflag;
	addDestAndDef(destVars, instrType, instr, retflag);
	if (retflag) return;
	addSrcAndUse(srcVars, instr);
	m_instructions.push_back(instr);
}

void SyntaxAnalysis::addSrcAndUse(std::vector<std::string>& srcVars, Instruction* instr)
{
	for (auto& srcVar : srcVars)
	{
		Variable* var = findVar(srcVar, m_variables);
		if (!var)
		{
			m_errorFound = true;
			cout << "ERROR - variable " << srcVar << " was not declared" << endl;
			return;
		}
		else
		{
			instr->getUse().push_back(var);
			if (var->getType() != Variable::CONST) // const samo smestam u USE
				instr->getSrc().push_back(var);
		}
	}
}

void SyntaxAnalysis::addDestAndDef(std::vector<std::string>& destVars, InstructionType instrType, Instruction* instr, bool& retflag)
{
	retflag = true;
	for (auto& destVar : destVars)
	{
		Variable* var = findVar(destVar, m_variables);
		if (!var)
		{
			if (instrType == I_BLTZ || instrType == I_B) instr->setLabelName(destVar); // ne mogu odmah da dodajem succ jer mozda jos uvek nije napravljena instrukcija posle te labele
			else
			{
				m_errorFound = true;
				cout << "ERROR - variable " << destVar << " was not declared" << endl;
				return;
			}
		}
		else
		{
			instr->getDst().push_back(var);
			instr->getDef().push_back(var);
		}
	}
	retflag = false;
}


string SyntaxAnalysis::getGlobal()
{
	return m_globalFunc;
}

Variables& SyntaxAnalysis::getVariables()
{
	return m_variables;
}

Labels SyntaxAnalysis::getLabel()
{
	return m_labelWithInstrPosition;
}

void SyntaxAnalysis::makePredAndSucc()
{

	for (auto& instr : m_instructions)
	{
		int currentPosition = instr->getPosition();
		Instruction* previousInstr = NULL;
		Instruction* nextInstr = NULL;

		if (currentPosition > 0) previousInstr = getInstructionByPosition(currentPosition - 1, m_instructions);
		if (currentPosition + 1 < m_instructions.size()) nextInstr = getInstructionByPosition(currentPosition + 1, m_instructions);

		if (previousInstr && previousInstr->getType() != I_B)
		{
			instr->getPred().push_back(previousInstr);
		}

		string labelKey = getLabelPointingOnThisInstr(currentPosition, m_labelWithInstrPosition);
		if (labelKey.size() > 0)
		{
			for (auto& instruct : m_instructions)
				if (instruct->getLabelName() == labelKey) instr->getPred().push_back(instruct);
		}

		if (instr->getType() == I_B)
		{
			instr->getSucc().push_back(getInstructionByPosition(m_labelWithInstrPosition[instr->getLabelName()], m_instructions));
			continue;
		}

		else if (instr->getType() == I_BLTZ)
		{
			instr->getSucc().push_back(getInstructionByPosition(m_labelWithInstrPosition[instr->getLabelName()], m_instructions));
		}

		if (nextInstr)
		{
			instr->getSucc().push_back(nextInstr);
		}
	}
}


Variable* findVar(string varName, Variables& vars)
{
	Variable* retVar = nullptr;

	for (auto& var : vars)
	{
		if (var->getName() == varName)
		{
			retVar = var;
			break;
		}
	}
	return retVar;
}


Instruction* getInstructionByPosition(int pos, Instructions& instrs)
{
	Instruction* retVal = nullptr;

	for (auto instr : instrs)
	{
		if (instr->getPosition() == pos)
		{
			retVal = instr;
			break;
		}
	}
	return retVal;
}


string getLabelPointingOnThisInstr(int pos, Labels labels)
{
	string retString = "";

	for (auto& i : labels)
	{
		if (i.second == pos)
		{
			retString = i.first;
			break;
		}
	}
	return retString;
}
