//
// Created by Alex Shchelochkov on 18.12.2022.
//
#pragma once

#include <cstdint>
#include <iostream>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <vector>
#include <typeinfo>

namespace token {
    class Token;
    class NumberToken;
    class BraceToken;
    class OperationToken;
}

namespace visitor {
    class TokenVisitor {
    public:
        virtual void visit(token::NumberToken *token) = 0;
        virtual void visit(token::BraceToken *token) = 0;
        virtual void visit(token::OperationToken *token) = 0;
    };

    class ParserVisitor : public TokenVisitor {
    private:
        std::vector<token::Token*> postfix_tokens = {};
        std::vector<token::Token*> bin = {};
        void pop_while(const std::function<bool(token::Token*)>& pred);
    public:
        ParserVisitor();
        ~ParserVisitor();
        void visit(token::NumberToken *token) override;
        void visit(token::BraceToken *token) override;
        void visit(token::OperationToken *token) override;
        std::vector<token::Token*> result();
    };

    class PrintVisitor : public TokenVisitor {
    private:
        std::stringstream stream;
    public:
        PrintVisitor();
        ~PrintVisitor();
        void visit(token::NumberToken *token) override;
        void visit(token::BraceToken *token) override;
        void visit(token::OperationToken *token) override;
        std::ostream& operator<<(std::ostream& os);
    };

    class CalcVisitor : public TokenVisitor {
    private:
        std::stack<double> stack = {};
    public:
        CalcVisitor();
        ~CalcVisitor();
        void visit(token::NumberToken *token) override;
        void visit(token::BraceToken *token) override;
        void visit(token::OperationToken *token) override;
        double result();
    };
}
