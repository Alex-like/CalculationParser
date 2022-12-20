//
// Created by Alex Shchelochkov on 19.12.2022.
//
#pragma once

#include "token.hpp"

namespace parser {
    class State {
    protected:
        std::string str;
        size_t pos;
    public:
        explicit State(std::string str, size_t pos) : str(std::move(str)), pos(pos) {}
        virtual ~State() = default;
        virtual State* next(std::vector<token::Token*> &tokens) = 0;
        char get() const {
            return str[pos];
        }
        State* space_or_end() const;
    };

    class Begin : public State {
    public:
        Begin(const std::string &str, size_t pos) : State(str, pos) {}
        ~Begin() override = default;
        State* next(std::vector<token::Token*> &tokens) override;
    };

    class Space : public State {
    public:
        Space(const std::string &str, size_t pos) : State(str, pos) {}
        ~Space() override = default;
        State* next(std::vector<token::Token*> &tokens) override;
    };

    class Number : public State {
    private:
        std::string number;
    public:
        Number(const std::string &str, size_t pos) : State(str, pos) {}
        ~Number() override = default;
        State* next(std::vector<token::Token*> &tokens) override;
    };

    class End : public State {
    public:
        End(const std::string &str, size_t pos) : State(str, pos) {}
        ~End() override = default;
        State* next(std::vector<token::Token*> &tokens) override {
            return this;
        }
    };

    State* State::space_or_end() const {
        if (pos >= str.size()) return new End(str, pos);
        if (std::isspace(get())) return new Space(str, pos);
        return nullptr;
    }

    State* Begin::next(std::vector<token::Token*> &tokens) {
        if (auto res = space_or_end()) return res;
        if (std::isdigit(get())) return new Number(str, pos);
        if (token::char_to_brace.contains(get())) {
            tokens.push_back(token::char_to_brace[get()]);
            pos++;
            return this;
        }
        if (token::char_to_operation.contains(get())) {
            tokens.push_back(token::char_to_operation[get()]);
            pos++;
            return this;
        }
        throw std::invalid_argument("Invalid expression: check " + std::to_string(pos) + " position.\n");
    }

    State* Space::next(std::vector<token::Token*> &tokens) {
        if (pos >= str.size()) return new End(str, pos);
        if (std::isspace(get())) {
            pos++;
            return this;
        }
        if (std::isdigit(get())) return new Number(str, pos) ;
        else return new Begin(str, pos);
    }

    State* Number::next(std::vector<token::Token*> &tokens) {
        if (auto res = space_or_end()) {
            tokens.push_back(new token::NumberToken(std::stod(number)));
            return res;
        }
        if (!std::isdigit(get())) {
            tokens.push_back(new token::NumberToken(std::stod(number)));
            return new Begin(str, pos);
        }
        number += get();
        pos++;
        return this;
    }
}
