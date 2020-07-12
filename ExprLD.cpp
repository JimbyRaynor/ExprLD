// ExprLD.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <ctime>


using namespace std;

string digits = ".0123456789 ";
string Expr;
bool undefined;

struct valnode
{
	double  value;
	char op;
};

list<valnode> parselist;


void printlist()
{
	if (parselist.size() > 0)
		for (auto e = parselist.begin(); e != parselist.end(); e++)
			cout << "value = " << (*e).value << "  |  op = " << (*e).op << "\n";
}



void parse()
{
	int64_t p;
	int test;
	valnode token;
	string constructtoken = "";
	p = 0;
	if (Expr[0] == '-')
	{
		constructtoken = "-";
		p++;
	}
	while (p < Expr.length())
	{
		if (digits.find(Expr[p]) != string::npos)  // found digit
		{
			constructtoken = constructtoken + Expr[p]; // make number
		}
		else // found operator
		{
			if (constructtoken != "")
			{
				token.value = stod(constructtoken);
				token.op = '#';  // storing number, so not an operation	
				parselist.push_back(token);
				constructtoken = "";
			}
			token.value = 0;
			token.op = Expr[p];  // *,+,/,-,),(
			parselist.push_back(token);
			if (Expr[p] != ')')
			{
				if (Expr[p + 1] == '+') 
				{
					p++;
				}  // ignore extra +
				if (Expr[p + 1] == '-') // negative number
				{
					p++;
					token.value = 0;
					token.op = 'n'; // negative unary operator
					parselist.push_back(token); // could be '(' following so don't try to construct number!
				}
			}
		} 
		p++;
	}
	if (constructtoken != "")
	{
		token.value = stod(constructtoken); // last token has to be a number or ')';
		token.op = '#';
		parselist.push_back(token);
	}
}

double evallist(list<valnode>::iterator starte, list<valnode>::iterator ende)
// ende is    next(last node to be evaluated)   (just past ending of expression)
{
	undefined = false;
	valnode token1, token2, token3, newtoken;
	string test;
	newtoken.op = '#';
	list<valnode>::iterator e, etoken1, etoken2, etoken3;
	// do negative operator
	e = starte;
	newtoken.value = (*e).value;
	while (next(e) != ende)
	{
		if (e->op == 'n')
		{
			etoken2 = next(e);
			e->op = '#';
			e->value = -1 * etoken2->value;
			parselist.erase(etoken2);
		}
		if (next(e) != ende)
		{e++;}
	}
	// do divide
	e = starte;
	newtoken.value = (*e).value;
	while (next(e, 1) != ende)
	{
		etoken1 = e;
		token1 = *e; // number
		e++;
		etoken2 = e;
		token2 = *e; // operation
		e++;
		etoken3 = e;
		token3 = *e; // number
		if (token2.op == '/')
		{
			newtoken.op = '#';
			if (token3.value == 0) { undefined = true; return 0; }
			newtoken.value = token1.value / token3.value;
			(*etoken1).op = newtoken.op;
			(*etoken1).value = newtoken.value;
			parselist.erase(etoken2, next(etoken3)); // removes everything *before* next(etoken3)
			e = etoken1;
		}
	}
    // do times
	e = starte;
	newtoken.value = (*e).value;
	while (next(e, 1) != ende)
	{
		etoken1 = e;
		token1 = *e; // number
		e++;
		etoken2 = e;
		token2 = *e; // operation
		e++;
		etoken3 = e;
		token3 = *e; // number
		if (token2.op == '*')
		{
			newtoken.op = '#';
			newtoken.value = token1.value * token3.value;
			(*etoken1).op = newtoken.op;
			(*etoken1).value = newtoken.value; 
			parselist.erase(etoken2, next(etoken3)); // removes everything *before* next(etoken3)
			e = etoken1;
		}
	}
	// do minus
	e = starte;
	newtoken.value = (*e).value;
	while (next(e, 1) != ende)
	{
		etoken1 = e;
		token1 = *e; // number
		e++;
		etoken2 = e;
		token2 = *e; // operation
		e++;
		etoken3 = e;
		token3 = *e; // number
		if (token2.op == '-')
		{
			newtoken.op = '#';
			newtoken.value = token1.value - token3.value;
			(*etoken1).op = newtoken.op;   
			(*etoken1).value = newtoken.value;
			parselist.erase(etoken2, next(etoken3)); // removes everything *before* next(etoken3)
			e = etoken1;
		}
	}
	// do plus
	e = starte;
	newtoken.value = (*e).value;
	while (next(e,1) != ende)
	{
		etoken1 = e;
		token1 = *e; // number
		e++;
		etoken2 = e;
		token2 = *e; // operation
		e++;
		etoken3 = e;
		token3 = *e; // number
		if (token2.op == '+')
		{
			newtoken.op = '#';
			newtoken.value = token1.value + token3.value;
			(*etoken1).op = newtoken.op;
			(*etoken1).value = newtoken.value;
			parselist.erase(etoken2, next(etoken3)); // removes everything *before* next(etoken3)
			e = etoken1;
		}
	}
	return newtoken.value;
}

double evalbrackets()
{
	double myval=0;
	list<valnode>::iterator e,f, eleftb, erightb, ende, starte;
	e = parselist.begin();
	while (e != parselist.end())
	{
		if (e->op == ')')
		{
			f = e;
			while (f->op != '(' )
			{
				f--;
			}
			eleftb = f;
			erightb = e;
			starte = next(eleftb);
			ende = erightb;
			myval = evallist(starte, ende);
			parselist.erase(erightb);
			e=parselist.erase(eleftb);
		}
		e++;
	}
	starte = parselist.begin();
	ende = parselist.end();
	myval = evallist(starte, ende);
	return myval;
}

int main(int argc, char* argv[], char* envp[])
{
	double expval=0;
	list<valnode>::iterator ende;
	cout.precision(15);
	Expr = "7-((-9+1)/(7*12)+1)";
	//fstream myfile;
	//myfile.open(argv[1], ios_base::in);
	//if (myfile.is_open())
	//	getline(myfile, Expr);
	//myfile.close(); 
	cout << "exp = " << Expr << "\n";
	clock_t starttime;
	starttime = clock();
	parse();
	printlist();
	expval = evalbrackets();
	if (undefined)
	{
		cout << "Error: Division by zero \n";
	}
	else
	{
		cout << "exp = " << expval << "\n";
	}
	cout << "time = " << clock() - starttime << "\n";

}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
