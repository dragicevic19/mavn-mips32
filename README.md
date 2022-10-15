# mavn-mips32

This compiler translates higher level Mips assembly code to regular Mips 32bit assembly.

Compiler has implemented lexical analysis with state machine, syntax analysis with recursive descent algorithm, liveness analysis, interference matrix building and register allocation.

You can change number of real registers for allocation (REG_NUMBER) in Constants.h. Compiler also gives you error messages if there is an error with input file or some algorithm can't be executed.
