### Appurupai

Lr1 Analyzer

Demo code in `Demo/Demo.sln(vs2019)`.

```cpp

#include "Appurupai/syntax.h"

// Non-Terminator
enum class Noterminal
{
	Exp = 0,
}; 

// Terminator
enum class Terminal
{
	Num = 1,
	Add,
	Multi
};

// define productions
Appurupai::LR1<Noterminal, Terminal> lr1{
		// start symbol
		Noterminal::Exp,
	{
		// production rules {non-terminator, {production} }
		{Noterminal::Exp, {Terminal::Num}},
		{Noterminal::Exp, {Noterminal::Exp, Terminal::Add, Noterminal::Exp}},
		{Noterminal::Exp, {Noterminal::Exp, Terminal::Multi, Noterminal::Exp}},
	},
	// priority
	{{Terminal::Multi}, {Terminal::Add}}
	};

// define input terminator stream
std::vector<Terminal> t = { Terminal::Num, Terminal::Add, Terminal::Num, Terminal::Multi, Terminal::Num };

// generate AST
auto re = Appurupai::generate_ast(lr1, t.begin(), t.end());

```