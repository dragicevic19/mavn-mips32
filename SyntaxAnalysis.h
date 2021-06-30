/* Autor: Marko Dragicevic Datum: 06.06.2021. */

#pragma once

#include "LexicalAnalysis.h"
#include "Token.h"
#include "IR.h"
#include <unordered_map>


typedef std::unordered_map<std::string, int> Labels;

class SyntaxAnalysis
{
public:
	/**
	* Constructor
	*/
	SyntaxAnalysis(LexicalAnalysis&, Instructions&, Variables&);
	

	/**
	* Method which performs lexical analysis
	*/
	bool Do();

	void makePredAndSucc();

	std::string getGlobal();

	Variables& getVariables();

	Labels getLabel();

private:
	/**
	* Prints the error message, and token that caused the syntax error
	*/
	void printSyntaxError(Token token);

	/**
	* Prints the token info
	*/
	void printTokenInfo(Token token);

	/**
	* Eats the current token if its type is "t"
	* otherwise reports syntax error
	*
	* param[in] - t - the expected token type
	*/
	void eat(TokenType t);

	/**
	* Returns the next token from the token list
	*/
	Token getNextToken();

	/**
	 * Nonterminal Q
	 */
	void Q();

	/**
	 * Nonterminal S
	 */
	void S();

	/**
	 * Nonterminal L
	 */
	void L();

	/**
	 * Nonterminal E
	 */
	void E();

	void makeInstruction(InstructionType instrType, std::vector<std::string>& destVars, std::vector<std::string>& srcVars, int instrPos);
	void addSrcAndUse(std::vector<std::string>& srcVars, Instruction* instr);
	void addDestAndDef(std::vector<std::string>& destVars, InstructionType instrType, Instruction* instr, bool& retflag);

	/**
	* Reference to lexical analysis module
	*/
	LexicalAnalysis& m_lexicalAnalysis;

	/**
	* Reference to instructions
	*/
	Instructions& m_instructions;
	int m_instrPosition;
	
	/**
	* Reference to instructions
	*/
	Variables& m_variables;
	int m_varPosition;

	Labels m_labelWithInstrPosition;

	std::string m_globalFunc;

	/**
	* Syntax error indicator
	*/
	bool m_errorFound;

	/**
	* Iterator to the token list which represents the output of the lexical analysis
	*/
	TokenList::iterator m_tokenIterator;

	/**
	* Current token that is being analyzed
	*/
	Token m_currentToken;
};


Variable* findVar(std::string varName, Variables& vars);

Instruction* getInstructionByPosition(int pos, Instructions& instrs);

std::string getLabelPointingOnThisInstr(int pos, Labels labels);
