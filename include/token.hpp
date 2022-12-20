//
// Created by Alex Shchelochkov on 18.12.2022.
//
#pragma once

#include <functional>
#include <map>
#include <utility>
#include "visitor.hpp"

namespace token {
    class Token {
    public:
        virtual void accept(visitor::TokenVisitor *visitor) = 0;
    };

    class NumberToken : public Token {
    private:
        double val;
    public:
        explicit NumberToken(double val) : val(val) {}
        ~NumberToken() = default;
        void accept(visitor::TokenVisitor *visitor) override {
            visitor->visit(this);
        }
        double value() const {
            return val;
        }
        std::string to_string() const {
            return std::to_string(val);
        }
    };

    class BraceToken : public Token {
    private:
        std::string val;
    public:
        explicit BraceToken(std::string val) : val(std::move(val)) {}
        ~BraceToken() = default;
        std::string to_string() const {
            return val;
        }
        void accept(visitor::TokenVisitor *visitor) override {
            visitor->visit(this);
        }
    };

    static bool equal(const BraceToken& lhs, const BraceToken& rhs) {
        return lhs.to_string() == rhs.to_string();
    }

    static BraceToken Left("(");
    static BraceToken Right(")");
    static std::map<char, BraceToken*> char_to_brace = {
            {'(', &Left},
            {')', &Right}
    };

    class OperationToken : public Token {
    private:
        std::string val;
        size_t priority;
        std::function<double(double, double)> op;
    public:
        OperationToken(std::string val, size_t prior, std::function<double(double, double)> op) :
        val(std::move(val)),
        priority(prior),
        op(std::move(op))
        {}
        double operator()(double lhs, double rhs) {
            return op(lhs, rhs);
        }
        std::string to_string() const {
            return val;
        }
        size_t prior() const {
            return priority;
        }
        void accept(visitor::TokenVisitor *visitor) override {
            visitor->visit(this);
        }
    };

    static bool cmp_by_prior(OperationToken* a, OperationToken* b) {
        return a->prior() >= b->prior();
    }

    static OperationToken Add("+", 1, [](double x, double y){ return x + y; });
    static OperationToken Subtract("-", 1, [](double x, double y){ return x - y; });
    static OperationToken Multiply("*", 2, [](double x, double y){ return x * y; });
    static OperationToken Divide("/", 2, [](double x, double y){ return x / y; });
    static std::map<char, OperationToken*> char_to_operation = {
            {'+', &Add},
            {'-', &Subtract},
            {'*', &Multiply},
            {'/', &Divide}
    };

    static void acceptAll(visitor::TokenVisitor *visitor, std::vector<Token*> &tokens) {
        for (auto &token : tokens)
            token->accept(visitor);
    }
}