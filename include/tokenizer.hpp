//
// Created by Alex Shchelochkov on 19.12.2022.
//
#pragma once

#include "parser.hpp"

namespace tokenizer {
    class Tokenizer {
    public:
        virtual std::vector<token::Token*> tokenize() = 0;
    };

    class ExpressionTokenizer : public Tokenizer {
    private:
        parser::State* state;
    public:
        explicit ExpressionTokenizer(const std::string& input) {
            state = new parser::Begin(input, 0);
        }
        ~ExpressionTokenizer() {
            delete state;
        };
        std::vector<token::Token*> tokenize() override {
            std::vector<token::Token*> tokens = {};
            while (typeid(*state) != typeid(parser::End)) {
                state = state->next(tokens);
            }
            return tokens;
        }
    };
}
