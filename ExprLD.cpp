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
	valnode token;
	string constructtoken;
	constructtoken = Expr[0];  // could be -
	p = 0;
	while (p < Expr.length() - 1)
	{
		p++;
		if (digits.find(Expr[p]) != string::npos)  // found digit
		{
			constructtoken = constructtoken + Expr[p];
		}
		else
		{
			token.value = stod(constructtoken);
			token.op = '#';  // storing number, so not an operation
			parselist.push_back(token);
			constructtoken = "";
			token.value = 0;
			token.op = Expr[p];
			parselist.push_back(token);
			p++;
			constructtoken = constructtoken + Expr[p]; // next number could start with '-'
		}
	}
	token.value = stoi(constructtoken);
	token.op = '#';
	parselist.push_back(token);
}

double evallist()
{
	undefined = false;
	double num1, num2;
	valnode token1, token2, token3, newtoken;
	string test;
	newtoken.op = '#';
	list<valnode>::iterator e, etoken1, etoken2, etoken3;
	e = parselist.begin();
	newtoken.value = (*e).value;
	while (next(e, 1) != parselist.end())
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
			e = parselist.erase(etoken2); // erase returns new position of item following etoken2
			parselist.erase(e++); // remove token 3 as well
			e = etoken1;
		}
	}
	e = parselist.begin();
	newtoken.value = (*e).value;
	while (next(e, 1) != parselist.end())
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
			e = parselist.erase(etoken2); // erase returns new position of item following etoken2
			parselist.erase(e++); // remove token 3 as well
			e = etoken1;
		}
	}
	e = parselist.begin();
	newtoken.value = (*e).value;
	while (next(e, 1) != parselist.end())
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
			e = parselist.erase(etoken2); // erase returns new position of item following etoken2
			parselist.erase(e++); // remove token 3 as well
			e = etoken1;
		}
	}
	return newtoken.value;
}

int main(int argc, char* argv[], char* envp[])
{
	cout.precision(15);
	double mydouble = 1.3467;
	long double mydoubledouble = 1.3467;
	cout << "size of double = " << sizeof(mydouble) << " bytes \n";
	cout << "size of long double = " << sizeof(mydoubledouble) << " bytes \n";
	//Expr = "-2.0234*6.03734--8--5--1-6+-9*2--8*4+6";
	fstream myfile;
	myfile.open(argv[1], ios_base::in);
	if (myfile.is_open())
		getline(myfile, Expr);
	myfile.close(); 
	cout << "exp = " << Expr << "\n";
	clock_t starttime;
	starttime = clock();
	parse();
	//printlist();
	cout << "exp = " << evallist() << "\n";
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
