#include <iostream>
#include "tokenizer.hpp"

using namespace std;

int main() {
    string input;
    getline(cin, input);
    auto tokens = tokenizer::ExpressionTokenizer(input).tokenize();
    auto parserVisitor = new visitor::ParserVisitor();
    token::acceptAll(parserVisitor, tokens);
    tokens = parserVisitor->result();
    auto printVisitor = new visitor::PrintVisitor();
    token::acceptAll(printVisitor, tokens);
    printVisitor->operator<<(std::cout);
    auto calcVisitor = new visitor::CalcVisitor();
    token::acceptAll(calcVisitor, tokens);
    cout << calcVisitor->result() << '\n';

    delete calcVisitor;
    delete printVisitor;
    delete parserVisitor;

    return 0;
}
