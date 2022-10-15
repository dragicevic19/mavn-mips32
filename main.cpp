/* Autor: Marko Dragicevic Datum: 06.06.2021. */

#include <iostream>
#include <exception>

#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"
#include "IR.h"
#include "LivenessAnalysis.h"
#include "InterferenceGraph.h"
#include "Simplification.h"
#include "ResourceAllocation.h"
#include "Output.h"

using namespace std;

int main()
{
	try
	{
		std::string fileName = "examples/simple.mavn";
		std::string outFileName = "examples/simple.s";
		bool retVal = false;

		// leksicka analiza
		LexicalAnalysis lex;

		if (!lex.readInputFile(fileName))
			throw runtime_error("\nException! Failed to open input file!\n");

		lex.initialize();

		retVal = lex.Do();

		if (retVal)
		{
			cout << "Lexical analysis finished successfully!" << endl;
			//lex.printTokens();
		}
		else
		{
			lex.printLexError();
			throw runtime_error("\nException! Lexical analysis failed!\n");
		}

		// sintaksna analiza, spisak instrukcija i promenljivih
		Instructions instrs;
		Variables vars;
		SyntaxAnalysis syntax(lex, instrs, vars);

		if (syntax.Do())
		{
			cout << "Syntax analysis finished successfully!" << endl;
		}
		else
		{
			cout << "Syntax analysis failed!" << endl;
			exit(-1);
		}

		syntax.makePredAndSucc();

		//analiza zivotnog veka promenljivih
		livenessAnalysis(instrs);
		
		//for (auto& i : instrs)
		//{
		//	i->printInstruction();					// ispis instrukcija
		//	cout << endl << endl << endl;
		//}

		InterferenceGraph& ig = buildInterferenceGraph(instrs, vars);

/*		cout << endl;
		printInterferenceMatrix()*/;
		
		// 2) simplify
		SimplificationStack& ss = doSimplification(ig, __REG_NUMBER__);

		if (ss.empty())	// 3) spill
		{
			cout << "Spill detected!" << endl;
			cout << "==========================================" << endl << endl;
			return -2;
		}

		
		if (doResourceAllocation(ss, ig)) // 4) select
		{
			cout << endl << "==========================================" << endl;
			cout << "Allocation successful!" << endl;
			cout << "==========================================" << endl;
		}
		else
		{
			cout << endl << "==========================================" << endl;
			cout << "Allocation failed!" << endl;
			cout << "==========================================" << endl << endl;
			return -3;
		}

		createOutFile(outFileName, instrs, vars, syntax.getLabel(), syntax.getGlobal());
		cout << endl << "Output file: <" << outFileName << "> is created." << endl;

	}
	catch (runtime_error e)
	{
		cout << e.what() << endl;
		return 1;
	}

	return 0;
}
