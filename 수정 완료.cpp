/*
calculator_buggy.cpp
*/

#include "std_lib_facilities.h"

struct Token {
    char kind;
    double value;
    string name;

    Token(char ch) : kind(ch), value(0) {}

    Token(char ch, double val) : kind(ch), value(val) {}

    Token(char ch, string name) : kind(ch), name(name) {} // 변수를 위한 생성자를 만듬
};

class Token_stream {
    bool full;
    Token buffer;
public:
    Token_stream() : full(0), buffer(0) {}

    Token get();

    void putback(Token t) {
        buffer = t;
        full = true;
    }

    void ignore(char);
};

class Calculator {
private:
    struct Variable {
        string name;
        double value;

        Variable(string n, double v) : name(n), value(v) {}
    };

    vector<Variable> names;

    Token_stream ts;

    const string prompt = "> "; // 클래스 안으로 이동
    const string result = "= ";

    double get_value(string);

    void set_value(string, double);

    bool is_declared(string);

    double expression();

    double exponential();

    double primary();

    double term();

    double declaration();

    double statement();

    void clean_up_mess();

public:
    static const char let = 'L'; // 클래스 안으로 이동
    static const char quit = 'Q';
    static const char print = ';';
    static const char number = '8';
    static const char name = 'a';

    Calculator();

    void calculate();
};

Token Token_stream::get() // 토큰 스트림의 멤버함수로 바꿈
{
    if (full) {
        full = false;
        return buffer;
    }
    char ch;
    cin >> ch;
    switch (ch) {
        case '(':
        case ')':
        case '+':
        case '-':
        case '*':
        case '/':
        case '^': // ^ 지원
        case '%':
        case ';':
        case '=':
            return Token(ch);
        case '.':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
            cin.unget();
            double val;
            cin >> val;
            return Token(Calculator::number, val);
        }
        default:
            if (isalpha(ch)) {
                string s;
                s += ch;
                while (cin.get(ch) && (isalpha(ch) || isdigit(ch))) s += ch;
                cin.unget();
                if (s == "let") return Token(Calculator::let);
                if (s == "quit") return Token(Calculator::quit);
                return Token(Calculator::name, s);
            }
            error("Bad token");
    }
}

void Token_stream::ignore(char c) {
    if (full && c == buffer.kind) {
        full = false;
        return;
    }
    full = false;

    char ch;
    while (cin >> ch)
        if (ch == c) return;
}

Calculator::Calculator() {
    names.push_back(Variable("pi", 3.14159));
    names.push_back(Variable("e", 2.71828));
}

double Calculator::get_value(string s) {
    for (int i = 0; i < names.size(); ++i)
        if (names[i].name == s) return names[i].value;
    error("get: undefined name ", s);
}

void Calculator::set_value(string s, double d) {
    for (int i = 0; i < names.size(); ++i) // size-1 까지 순회해야함
        if (names[i].name == s) {
            names[i].value = d;
            return;
        }
    error("set: undefined name ", s);
}

bool Calculator::is_declared(string s) {
    for (int i = 0; i < names.size(); ++i)
        if (names[i].name == s) return true;
    return false;
}

double Calculator::primary() {
    Token t = ts.get();
    switch (t.kind) {
        case '(': {
            double d = expression();
            t = ts.get();
            if (t.kind != ')') error("'(' expected");
            return d; // 값 리턴
        }
        case '+':
            return +primary(); // + 지원
        case '-':
            return -primary();
        case number:
            return t.value;
        case name:
            return get_value(t.name); // 변수의 경우 값을 반환
        default:
            error("primary expected");
    }
}

double Calculator::exponential() { // ^ 를 계산하는 함수
    double first = primary();
    Token t = ts.get();
    if(t.kind!='^'){
        ts.putback(t);
        return first;
    }
    return pow(first, exponential());
}

double Calculator::term() {
    double left = exponential();
    while (true) {
        Token t = ts.get();
        switch (t.kind) {
            case '*':
                left *= exponential();
                break;
            case '/': {
                double d = exponential();
                if (d == 0) error("divide by zero");
                left /= d;
                break;
            }
            default:
                ts.putback(t); // 잘못 꺼냈으면 다시 넣어줌
                return left;
        }
    }
}

double Calculator::expression() {
    double left = term();
    while (true) {
        Token t = ts.get();
        switch (t.kind) {
            case '+':
                left += term();
                break;
            case '-':
                left -= term();
                break;
            default:
                ts.putback(t); // 잘못 꺼냈으면 다시 넣어줌
                return left;
        }
    }
}

double Calculator::declaration() {
    Token t = ts.get();
    if (t.kind != name) error("name expected in declaration");
    string name = t.name;
    if (is_declared(name)) error(name, " declared twice");
    Token t2 = ts.get();
    if (t2.kind != '=') error("= missing in declaration of ", name);
    double d = expression();
    names.push_back(Variable(name, d));
    return d;
}

double Calculator::statement() {
    Token t = ts.get();
    switch (t.kind) {
        case let:
            return declaration();
        default:
            ts.putback(t);
            return expression();
    }
}

void Calculator::clean_up_mess() {
    ts.ignore(print);
}

void Calculator::calculate() {
    while (true)
        try {
            cout << prompt;
            Token t = ts.get();
            while (t.kind == print) t = ts.get();
            if (t.kind == quit) return;
            ts.putback(t);
            cout << result << statement() << endl;
        }
        catch (runtime_error &e) {
            cerr << e.what() << endl;
            clean_up_mess();
        }
}

int main() {
    try {
        Calculator c;
        c.calculate();
        return 0;
    }
    catch (exception &e) {
        cerr << "exception: " << e.what() << endl;
        keep_window_open();
        return 1;
    }
    catch (...) {
        cerr << "exception\n";
        keep_window_open();
        return 2;
    }
}

