#include "../../Potato/parser.h"
#include <assert.h>
#include <iostream>


std::u32string SBNF = UR"(
_IGNORE := '\s+'
Number := '[1-9][0-9]*'
%%%
$ := <Statement>
<Statement> := Number
<Statement> := <Statement> '+' <Statement> :[1]
<Statement> := <Statement> '*' <Statement> :[2]
<Statement> := <Statement> '/' <Statement> :[3]
<Statement> := <Statement> '-' <Statement> :[4]
<Statement> := '(' <Statement> ')'
%%%
('/' '*') ('+' '-')
)";


int to_int(std::u32string_view code)
{
	int data = 0;
	for (auto ite : code)
	{
		data = data * 10 + ite - U'0';
	}
	return data;
}

using namespace Potato::Parser;

int main()
{
	auto parser = sbnf::create(SBNF);
	auto Number = parser.find_symbol(U"Number");
	std::u32string Expression = U"1+2*4+(1+1)/2";
	sbnf_processer sp(parser);
	std::vector<int> Storage;
	sp.analyze(Expression, [&](sbnf_processer::travel tra) {
		if (tra.is_terminal())
		{
			if (tra.sym == *Number)
				Storage.push_back(to_int(tra.token_data));
		}
		else {
			switch (tra.noterminal.function_enum)
			{
			case 1: {
				Storage[Storage.size() - 2] += Storage[Storage.size() - 1];
				Storage.pop_back();
			}break;
			case 2: {
				Storage[Storage.size() - 2] *= Storage[Storage.size() - 1];
				Storage.pop_back();
			}break;
			case 3: {
				Storage[Storage.size() - 2] /= Storage[Storage.size() - 1];
				Storage.pop_back();
			}break;
			case 4: {
				Storage[Storage.size() - 2] -= Storage[Storage.size() - 1];
				Storage.pop_back();
			}break;
			default:
				break;
			}
		}
	});
	int result = Storage[0];
	std::cout << "1+2*4+(1+1)/2" << std::endl << "result :" << result << std::endl;
	system("pause");
	return 0;

}