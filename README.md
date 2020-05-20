# Appurupai

An Simple Lex/YACC Like Compiler Front End

Use simplified regular expressions to define your owned lexer, and use simplified BNF to define your owned parser.

Define Example:
```
std::u32string DefineCode = UR"(
// Define Lexer
_IGNORE := '\s+'
Number := '[1-9][0-9]*'
// Sperate Line
%%%
// Define Your bnf
$ := <Statement>
<Statement> := Number
<Statement> := <Statement> '+' <Statement> :[1]
<Statement> := <Statement> '*' <Statement> :[2]
<Statement> := <Statement> '/' <Statement> :[3]
<Statement> := <Statement> '-' <Statement> :[4]
<Statement> := '(' <Statement> ')'
// Sperate Line
%%%
// Define Operator Priority
('/' '*') ('+' '-')
)";
```

Use Example:
```

// utf-32 to int
int to_int(std::u32string_view code)
{
	int data = 0;
	for (auto ite : code)
	{
		data = data * 10 + ite - U'0';
	}
	return data;
}

// create sbnf
auto parser = sbnf::create(DefineCode);
// find symbol
auto Number = parser.find_symbol(U"Number");
// define processer
sbnf_processer sp(parser);
// define handle stack
std::vector<int> Storage;
// define input string
std::u32string Expression = U"1+2*4+(1+1)/2";
// handle
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

// print result
int result = Storage[0];
std::cout << "1+2*4+(1+1)/2" << std::endl << "result :" << result << std::endl;
```

Demo code in `Demo/Demo.sln(vs2019)`.