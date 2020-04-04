#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

int priority(int op)
{
	switch (op)
	{
	case '(' : 
	case '{' :
	case '[' : return 0;

	case '+':
	case '-': return 1;

	case '*':
	case '/': return 2;


	}
}

 
bool preOp(char op1, char op2)
{
	return priority(op1) >= priority(op2);
}



void checkS(stack<char>* sc, vector<char>& v, char c)
{
	while (!sc->empty() && preOp(sc->top(), c))
	{
		v.push_back(sc->top()); sc->pop();
		v.push_back(' ');
	}
	sc->push(c);
}

void checkP1(stack<char>* sc, vector<char>& v)
{
	char c;

	for (c = sc->top(), sc->pop(); c != '{'; c = sc->top(), sc->pop())
	{
		v.push_back(' ');
		v.push_back(c);
	}
}
void checkP2(stack<char>* sc, vector<char>& v)
{
	char c;

	for (c = sc->top(), sc->pop(); c != '['; c = sc->top(), sc->pop())
	{
		v.push_back(' ');
		v.push_back(c);
	}
}
void checkP3(stack<char>* sc, vector<char>& v)
{
	char c;

	for (c = sc->top(), sc->pop(); c != '('; c = sc->top(), sc->pop())
	{
		v.push_back(' ');
		v.push_back(c);
	}
}



bool InfixToPostfix(vector<char>& Postfix, vector<char>& Infix)
{
	const char* op = "+-*/";

	stack<char> Stack;
	stack<char>* sc = &Stack;
	vector<char>* p = &Postfix;
	vector<char>::iterator n = Infix.begin();

	int  count = 1;
	int  parN1 = 0;
	int  parN2 = 0;
	int  parN3 = 0;

	while (n != Infix.end())
	{
		if (isdigit(*n) || *n == '.') {
			p->push_back(*n++); 
			count = 0;
			continue;
		}
		else if (strchr(op, *n)) { 
			if (++count == 2) { 
				if (*n == '+' || *n == '-') { 
					p->push_back(' ');
				}
				else return false;
			}
			else if (count == 3) return false;
			p->push_back(' ');
			checkS(sc, Postfix, *n);
		}
		switch (*n)
		{
		case '{':
			
			sc->push(*n);
			++parN1;
			break;

		case '}':
			checkP1(sc, Postfix);
			--parN1;
			break;
		
		case '[':

			sc->push(*n);
			++parN2;
			break;

		case ']':
			checkP2(sc, Postfix);
			--parN2;
			break;
		
		case '(':

			sc->push(*n);
			++parN3;
			break;

		case ')':
			checkP3(sc, Postfix);
			--parN3;
			break;
		}

		if (parN1 < 0) {
			cout << "Error!: unbalanced parentheses" << endl;
			return false;
		}
		if (parN2 < 0) {
			cout << "Error!: unbalanced parentheses" << endl;
			return false;
		}
		if (parN3 < 0) {
			cout << "Error!: unbalanced parentheses" << endl;
			return false;
		}
		n++;
	}
	

	while (!sc->empty())
	{
		Postfix.push_back(' ');
		Postfix.push_back(sc->top()); sc->pop();
	}

	return true;
}

double value(stack<double>* sd)
{
	double d = sd->top(); sd->pop();
	return d;
}



double cal(vector<char>& Postfix)
{
	const char* op = "+-*/";

	vector<char>::iterator p = Postfix.begin();
	stack<double>  Stack;
	stack<double>* sd = &Stack;

	double n1, n2;
	char   cNumber[32];
	char*  c = cNumber;

	while (p != Postfix.end())
	{
		if (isdigit(*p) || *p == '.') {
			*c++ = *p++;
			continue;
		}


		else if (isdigit(cNumber[0])) {
			*c = ' ';
			sd->push(atof(cNumber));
			c = cNumber;
			cNumber[0] = ' ';
		}
		else if (strchr(op, *p)) { 

			n1 = value(sd);
			n2 = value(sd);
			if (n1 == 0 && *p == '/') {
				cout << "Error!:divide by zero" << endl;
			}
			switch (*p)
			{
			case '+': sd->push(n2 + n1); break;
			case '-': sd->push(n2 - n1); break;
			case '*': sd->push(n2 * n1); break;
			case '/':sd->push(n2 / n1);break;
			}
		}
		p++; 
	}
	return value(sd);
}




bool funCheck(vector<char>& Infix)
{
	vector<char>::iterator p = Infix.begin();

	int n1 = 0;
	int n2 = 0;
	int n3 = 0;

	while (p != Infix.end())
	{
		if (*p == '{') {
			n1++;
		}
		else if (*p == '}') { 
			n1--;
		}
		else if (*p == '[') {
			n2++;
		}
		else if (*p == ']') {
			n2--;
		}
		else if (*p == '(') {
			n3++;
		}
		else if (*p == ')') { 
			n3--;
		}
		p++;
	}
	if (n1 != 0) {
		cout << "Error!: unbalanced parentheses" << endl;
		return false;
	}
	if (n2 != 0) {
		cout << "Error!: unbalanced parentheses" << endl;
		return false;
	}
	if (n3 != 0) {
		cout << "Error!: unbalanced parentheses" << endl;
		return false;
	}
	return true;
}

void result(vector<char>& Infix)
{
	vector<char> buffer;
	vector<char>::iterator it;

	if (funCheck(Infix)) {
		if (InfixToPostfix(buffer, Infix)) {
			cout << cal(buffer)<<endl;
		}
	}
	buffer.erase(buffer.begin(), buffer.end());
}

void getInfix(vector<char>& vc)
{
	int ENTER = 10;
	int BACK_SPACE = 8;

	for (int ch = getc(stdin); ch != ENTER; ch = getc(stdin))
	{
		vc.push_back(ch);
		if (ch == BACK_SPACE)
			vc.pop_back();
	}
}

int main()
{
	vector<char> vc;
	cout << fixed;
	cout.precision(3);

	while (cin.peek()!= 'E') 
	{
		getInfix(vc);
		result(vc);
		vc.erase(vc.begin(), vc.end());
	}
	return 0;
}

