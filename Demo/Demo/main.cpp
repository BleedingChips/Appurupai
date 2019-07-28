#include "../..//Appurupai/syntax.h"
#include <iostream>

enum class Noterminal
{
	Exp = 0,
}; 

enum class Terminal
{
	Num = 1,
	Add,
	Multi
};

void print_symbol(Noterminal sym)
{
	switch (sym)
	{
	case Noterminal::Exp:
		std::cout << "Exp";
		break;
	default:
		assert(false);
	}
}

void print_symbol(Terminal sym)
{
	switch (sym)
	{
	case Terminal::Num:
		std::cout << "Num"; break;
	case Terminal::Add:
		std::cout << "Add"; break;
	case Terminal::Multi:
		std::cout << "Multi"; break;
	default:
		assert(false);
	}
}

void print_symbol_imp(Appurupai::ast_node<Noterminal, Terminal>& a, size_t layout = 0)
{
	for (size_t i = 0; i < layout; ++i)
		std::cout << "-";
	print_symbol(a.symbol());
	std::cout << " : ";
	for (size_t i = 0; i < a.size(); ++i)
	{
		auto& re = a[i];
		if (std::holds_alternative<Appurupai::ast_node<Noterminal, Terminal>>(re))
		{
			auto& p = std::get<Appurupai::ast_node<Noterminal, Terminal>>(re);
			std::cout << " ";
			print_symbol(p.symbol());
		}
		else if (std::holds_alternative<Appurupai::ast_node_terminal<Terminal>>(re))
		{
			auto& p = std::get<Appurupai::ast_node_terminal<Terminal>>(re);
			std::cout << " ";
			print_symbol(p.symbol);
		}
	}
	std::cout << std::endl;
	for (size_t i = 0; i < a.size(); ++i)
	{
		auto& re = a[i];
		if (std::holds_alternative<Appurupai::ast_node<Noterminal, Terminal>>(re))
		{
			auto& p = std::get<Appurupai::ast_node<Noterminal, Terminal>>(re);
			print_symbol_imp(p, layout + 1);
		}
	}
}

int main()
{
	Appurupai::LR1<Noterminal, Terminal> lr1{
		Noterminal::Exp,
	{
		{Noterminal::Exp, {Terminal::Num}},
		{Noterminal::Exp, {Noterminal::Exp, Terminal::Add, Noterminal::Exp}},
		{Noterminal::Exp, {Noterminal::Exp, Terminal::Multi, Noterminal::Exp}},
	},
	{{Terminal::Multi}, {Terminal::Add}}
	};
	
	std::vector<Terminal> t = { Terminal::Num, Terminal::Add, Terminal::Num, Terminal::Multi, Terminal::Num };

	try {
		auto re = Appurupai::generate_ast(lr1, t.begin(), t.end());
		print_symbol_imp(re);
	}
	catch (const Appurupai::Error::generate_ast_unacceptable_error<Noterminal, Terminal>& e)
	{
		assert(false);
	}
	return 0;

}