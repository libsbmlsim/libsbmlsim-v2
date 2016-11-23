#include <gtest/gtest.h>
#include <string>
#include "sbmlsim/SBMLSim.h"
#include "sbmlsim/internal/util/MathUtil.h"
#include "sbmlsim/internal/util/StringUtil.h"

namespace {

class MathUtilTest : public ::testing::Test{};

    TEST_F(MathUtilTest, containsTargetTrue) {
        std::string s = "z";
        ASTNode* ast = SBML_parseFormula("x^2 + 3 * y + sin(z)");
        EXPECT_TRUE(MathUtil::containsTarget(ast, s));
    }

    TEST_F(MathUtilTest, containsTargetFalse) {
        std::string s = "si";
        ASTNode* ast = SBML_parseFormula("x^2 + 3 * y + sin(z)");
        EXPECT_FALSE(MathUtil::containsTarget(ast, s));
    }

    TEST_F(MathUtilTest, containsTargetFalse2) {
        std::string s = "2";
        ASTNode* ast = SBML_parseFormula("x^2 + 3 * y + sin(z)");
        EXPECT_FALSE(MathUtil::containsTarget(ast, s));
    }

    TEST_F(MathUtilTest, simplifyTestTrue) {
        ASTNode* ast = SBML_parseFormula("((2 + 3 * 5) / 2) * x");
        ASTNode* simp = MathUtil::simplify(ast);
        std::string s = SBML_formulaToString(simp);
        EXPECT_EQ(s, "8.5 * x");
    }

    TEST_F(MathUtilTest, simplifyTestTrue2) {
        ASTNode* ast = SBML_parseFormula("2 * x + (3 * 5 / 2) * y");
        ASTNode* simp = MathUtil::simplify(ast);
        std::string s = SBML_formulaToString(simp);
        EXPECT_EQ(s, "2 * x + 7.5 * y");
    }

    TEST_F(MathUtilTest, simplifyTestTrue3) {
        ASTNode* ast = SBML_parseFormula("x ^ 2 * 2 + x * 3 + 1");
        ASTNode* simp = MathUtil::simplify(ast);
        std::string s = SBML_formulaToString(simp);
        EXPECT_EQ(s, "2 * x^2 + 3 * x + 1");
    }

    TEST_F(MathUtilTest, simplifyTestTrue4) {
        ASTNode* ast = SBML_parseFormula("2 + x");
        ASTNode* simp = MathUtil::simplify(ast);
        std::string s = SBML_formulaToString(simp);
        EXPECT_EQ(s, "x + 2");
    }

    TEST_F(MathUtilTest, simplifyTestPowerTrue) {
        ASTNode* ast = SBML_parseFormula("3 * x ^ 1 + 4");
        ASTNode* simp = MathUtil::simplify(ast);
        std::string s = SBML_formulaToString(simp);
        EXPECT_EQ(s, "3 * x + 4");
    }

    TEST_F(MathUtilTest, simplifyTestPowerTrue2) {
        ASTNode* ast = SBML_parseFormula("3 * x ^ 0 + 4");
        ASTNode* simp = MathUtil::simplify(ast);
        std::string s = SBML_formulaToString(simp);
        EXPECT_EQ(s, "7");
    }

    TEST_F(MathUtilTest, simplifyTestFalse) {
        ASTNode* ast = SBML_parseFormula("2 * x + (3 * 5 / 2) * y");
        ASTNode* simp = MathUtil::simplify(ast);
        std::string s = SBML_formulaToString(simp);
        EXPECT_NE(s, "2 * x + 7 * y");
    }

    TEST_F(MathUtilTest, differentiateTestAdd) {
        ASTNode* ast = SBML_parseFormula("2 * x + x ^ 2 + y ^ 2");
        ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
        std::string s = SBML_formulaToString(diff);
        EXPECT_EQ(s, "2 * x + 2");
    }

    TEST_F(MathUtilTest, differentiateTestSub) {
        ASTNode* ast = SBML_parseFormula("2 * x - x ^ 2 - y ^ 2");
        ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
        std::string s = SBML_formulaToString(diff);
        EXPECT_EQ(s, "2 - 2 * x");
    }

    TEST_F(MathUtilTest, differentiateTestConst) {
        ASTNode* ast = SBML_parseFormula("x * 13 + 11 * y");
        ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
        std::string s = SBML_formulaToString(diff);
        EXPECT_EQ(s, "13");
    }

    TEST_F(MathUtilTest, differentiateTestMul) {
        ASTNode* ast = SBML_parseFormula("x ^ 2 * x ^ 3");
        ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
        std::string s = SBML_formulaToString(diff);
        EXPECT_EQ(s, "2 * x * x^3 + x^2 * 3 * x^2");
    }

    TEST_F(MathUtilTest, differentiateTestDiv) {
        ASTNode* ast = SBML_parseFormula("2 * x - x ^ 2 - 1");
        ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
        std::string s = SBML_formulaToString(diff);
        EXPECT_EQ(s, "2 - 2 * x");
    }

} // namespace
