//
// Created by Alex Shchelochkov on 20.12.2022.
//
#include "token.hpp"
#include "visitor.hpp"

visitor::ParserVisitor::ParserVisitor() = default;

visitor::ParserVisitor::~ParserVisitor() {
    postfix_tokens.clear();
}

void visitor::ParserVisitor::pop_while(const std::function<bool(token::Token*)>& pred) {
    if (bin.empty()) return;
    while (!bin.empty() && pred(bin.back())) {
        postfix_tokens.push_back(bin.back());
        bin.pop_back();
    }
}
void visitor::ParserVisitor::visit(token::NumberToken* token) {
    postfix_tokens.push_back(token);
}
void visitor::ParserVisitor::visit(token::BraceToken *token) {
    if (token::equal(*token, token::Left)) {
        bin.push_back(token);
        return;
    }
    pop_while([](token::Token* t) -> bool { return typeid(*t) != typeid(token::BraceToken); });
    if (bin.empty())
        throw std::invalid_argument("Invalid brace position.\n");
    bin.pop_back();
}
void visitor::ParserVisitor::visit(token::OperationToken *token) {
    pop_while([&](token::Token *t) -> bool {
        if (typeid(*t) != typeid(token::OperationToken))
            return false;
        return token::cmp_by_prior(dynamic_cast<token::OperationToken *>(t), token);
    });
    bin.push_back(token);
}
std::vector<token::Token*> visitor::ParserVisitor::result() {
    while (!bin.empty()) {
        postfix_tokens.push_back(bin.back());
        bin.pop_back();
    }
    return postfix_tokens;
}

visitor::PrintVisitor::PrintVisitor() = default;

visitor::PrintVisitor::~PrintVisitor() {
    stream.clear();
}

void visitor::PrintVisitor::visit(token::NumberToken *token) {
    stream << token->to_string() << ' ';
}
void visitor::PrintVisitor::visit(token::BraceToken *token) {
    stream << token->to_string() << ' ';
}
void visitor::PrintVisitor::visit(token::OperationToken *token) {
    stream << token->to_string() << ' ';
}
std::ostream& visitor::PrintVisitor::operator<<(std::ostream& os) {
    os << stream.str() << '\n';
    return os;
}

visitor::CalcVisitor::CalcVisitor() = default;

visitor::CalcVisitor::~CalcVisitor() {
    while (!stack.empty())
        stack.pop();
}

void visitor::CalcVisitor::visit(token::NumberToken *token) {
    stack.push(token->value());
}
void visitor::CalcVisitor::visit(token::BraceToken *token) {
    throw std::invalid_argument("Invalid brace position.\n");
}
void visitor::CalcVisitor::visit(token::OperationToken *token) {
    if (stack.size() < 2)
        throw std::invalid_argument("Invalid operation position.\n");
    double rhs = stack.top();
    stack.pop();
    double lhs = stack.top();
    stack.pop();
    stack.push(token->operator()(lhs, rhs));
}
double visitor::CalcVisitor::result() {
    if (stack.size() != 1)
        throw std::invalid_argument("Invalid expression.\n");
    return stack.top();
}