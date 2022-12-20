//
// Created by Alex Shchelochkov on 20.12.2022.
//
#include <gtest/gtest.h>
#include "tokenizer.hpp"

using namespace std;

GTEST_API_ int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

double run(const string& input) {
    auto tokens = tokenizer::ExpressionTokenizer(input).tokenize();
    auto parserVisitor = new visitor::ParserVisitor();
    token::acceptAll(parserVisitor, tokens);
    tokens = parserVisitor->result();
    auto calcVisitor = new visitor::CalcVisitor();
    token::acceptAll(calcVisitor, tokens);
    double res = calcVisitor->result();

    delete calcVisitor;
    delete parserVisitor;

    return res;
}

static vector<pair<string, double>> test_expected = {
        {"2", 2},
        {"2 + 1", 3},
        {"(30 + 2) / 8", 4.},
        {"(23 + 10) * 5 - 3 * (32 + 5) * (10 - 4 * 5) + 8 / 2", 1279.}
};

TEST(SimpleTest, CalculationTests) {
    for (auto &p : test_expected)
        EXPECT_NEAR(p.second, run(p.first), 1e-6) << "Error in exp: " << p.first << '\n';
}

TEST(ThrowingTest, empty_test) {
    EXPECT_THROW(run(""), invalid_argument);
}

TEST(ThrowingTest, number_seq_test) {
    EXPECT_THROW(run("2 2"), invalid_argument);
}

TEST(ThrowingTest, missed_argument_test) {
    EXPECT_THROW(run("(2 + ) - 1"), invalid_argument);
}

