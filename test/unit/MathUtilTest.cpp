#include <gtest/gtest.h>
#include <string>
#include "sbmlsim/SBMLSim.h"
#include "sbmlsim/internal/util/MathUtil.h"

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

    TEST_F(MathUtilTest, simplifyTestFalse) {
        ASTNode* ast = SBML_parseFormula("2 * x + (3 * 5 / 2) * y");
        ASTNode* simp = MathUtil::simplify(ast);
        std::string s = SBML_formulaToString(simp);
        EXPECT_NE(s, "2 * x + 7 * y");
    }
} // namespace
