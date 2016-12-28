#include <gtest/gtest.h>
#include "sbmlsim/SBMLSim.h"
#include "sbmlsim/internal/util/ASTNodeUtil.h"
#include "sbmlsim/internal/util/MathUtil.h"

namespace {

  class MathUtilTest : public ::testing::Test{};

  TEST_F(MathUtilTest, simplifyRuleOneTest1) {
    ASTNode* ast = SBML_parseFormula("((x + y) + 1) + 2");
    ASTNode* simp = MathUtil::simplifyRuleOne(ast);
    int num = simp->getNumChildren();
    EXPECT_EQ(4, num);
  }

  TEST_F(MathUtilTest, simplifyRuleOneTest2) {
    ASTNode* ast = SBML_parseFormula("((1 * 2) * 3) * 4");
    ASTNode* simp = MathUtil::simplifyRuleOne(ast);
    int num = simp->getNumChildren();
    EXPECT_EQ(4, num);
  }

  TEST_F(MathUtilTest, simplifyRuleOneTest3) {
    ASTNode* ast = SBML_parseFormula("((x + y) + 1) + 2");
    ASTNode* simp = MathUtil::simplifyRuleOne(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "x + y + 1 + 2");
  }

  TEST_F(MathUtilTest, simplifyRuleOneTest4) {
    ASTNode* ast = SBML_parseFormula("((1 * 2) * 3) * 4");
    ASTNode* simp = MathUtil::simplifyRuleOne(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "1 * 2 * 3 * 4");
  }

  TEST_F(MathUtilTest, simplifyRuleOneTest5) {
    ASTNode* ast = SBML_parseFormula("a - 5");
    ASTNode* simp = MathUtil::simplifyRuleOne(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "a + -5");
  }

  TEST_F(MathUtilTest, simplifyRuleOneTest6) {
    ASTNode* ast = SBML_parseFormula("1 / y");
    ASTNode* simp = MathUtil::simplifyRuleOne(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "1 * y^-1");
  }

  TEST_F(MathUtilTest, simplifyRuleOneTest7) {
    ASTNode* ast = SBML_parseFormula("x^2 / y^2");
    ASTNode* simp = MathUtil::simplifyRuleOne(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "x^2 * y^-2");
  }

  TEST_F(MathUtilTest, simplifyRuleTwoTest1) {
    ASTNode* ast = SBML_parseFormula("(1 + 2) + 3");
    ASTNode* simp = MathUtil::simplifyRuleTwo(MathUtil::simplifyRuleOne(ast));
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "6");
  }

  TEST_F(MathUtilTest, simplifyRuleTwoTest2) {
    ASTNode* ast = SBML_parseFormula("1 + x + 2 + y");
    ASTNode* simp = MathUtil::simplifyRuleTwo(MathUtil::simplifyRuleOne(ast));
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "x + y + 3");
  }

  TEST_F(MathUtilTest, simplifyRuleTwoTest3) {
    ASTNode* ast = SBML_parseFormula("1 * 2 * 3");
    ASTNode* simp = MathUtil::simplifyRuleTwo(MathUtil::simplifyRuleOne(ast));
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "6");
  }

  TEST_F(MathUtilTest, simplifyRuleTwoTest4) {
    ASTNode* ast = SBML_parseFormula("2 * x * 3 * y * 1");
    ASTNode* simp = MathUtil::simplifyRuleTwo(MathUtil::simplifyRuleOne(ast));
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "6 * x * y");
  }

  TEST_F(MathUtilTest, simplifyRuleTwoTest5) {
    ASTNode* ast = SBML_parseFormula("0 ^ 3");
    ASTNode* simp = MathUtil::simplifyRuleTwo(MathUtil::simplifyRuleOne(ast));
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "0");
  }

  TEST_F(MathUtilTest, simplifyRuleTwoTest6) {
    ASTNode* ast = SBML_parseFormula("1 ^ (-3)");
    ASTNode* simp = MathUtil::simplifyRuleTwo(MathUtil::simplifyRuleOne(ast));
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "1");
  }

  TEST_F(MathUtilTest, simplifyRuleTwoTest7) {
    ASTNode* ast = SBML_parseFormula("(x^2 + y^2)^0");
    ASTNode* simp = MathUtil::simplifyRuleTwo(MathUtil::simplifyRuleOne(ast));
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "1");
  }

  TEST_F(MathUtilTest, simplifyRuleTwoTest8) {
    ASTNode* ast = SBML_parseFormula("(2 * x * 3 * y * 1)^1");
    ASTNode* simp = MathUtil::simplifyRuleTwo(MathUtil::simplifyRuleOne(ast));
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "6 * x * y");
  }

  TEST_F(MathUtilTest, simplifyRuleTwoTest9) {
    ASTNode* ast = SBML_parseFormula("pow(2, 3)");
    ASTNode* simp = MathUtil::simplifyRuleTwo(MathUtil::simplifyRuleOne(ast));
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "8");
  }

  TEST_F(MathUtilTest, simplifyRuleTwoTest10) {
    ASTNode* ast = SBML_parseFormula("pow(pow(x, 2), 3)");
    ASTNode* simp = MathUtil::simplifyRuleTwo(MathUtil::simplifyRuleOne(ast));
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "x^6");
  }

  TEST_F(MathUtilTest, simplifyRuleTwoTest11) {
    ASTNode* ast = SBML_parseFormula("pow(pow(2, x), 3)");
    ASTNode* simp = MathUtil::simplifyRuleTwo(MathUtil::simplifyRuleOne(ast));
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "8^x");
  }

  TEST_F(MathUtilTest, simplifyRuleTwoTest12) {
    ASTNode* ast = SBML_parseFormula("3 * x^p + 2 * x^p");
    ASTNode* simp = MathUtil::simplifyRuleTwo(MathUtil::simplifyRuleOne(ast));
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "5 * x^p");
  }

  TEST_F(MathUtilTest, simplifyRuleTwoTest13) {
    ASTNode* ast = SBML_parseFormula("3 * x^p - 2 * x^p");
    ASTNode* simp = MathUtil::simplifyRuleTwo(MathUtil::simplifyRuleOne(ast));
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "x^p");
  }

  TEST_F(MathUtilTest, simplifyRuleTwoTest14) {
    ASTNode* ast = SBML_parseFormula("2 * x^p - x^p");
    ASTNode* simp = MathUtil::simplifyRuleTwo(MathUtil::simplifyRuleOne(ast));
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "x^p");
  }

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

  TEST_F(MathUtilTest, simplifyTest) {
    ASTNode* ast = SBML_parseFormula("((3 + 3 * 5) / 2) * x");
    ASTNode* simp = MathUtil::simplify(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "9 * x");
  }

  TEST_F(MathUtilTest, simplifyTest2) {
    ASTNode* ast = SBML_parseFormula("2 * x + (3 * 5 / 2) * y");
    ASTNode* simp = MathUtil::simplify(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "2 * x + 15 / 2 * y");
  }

  TEST_F(MathUtilTest, simplifyTest3) {
    ASTNode* ast = SBML_parseFormula("x ^ 2 * 2 + x * 3 + 1");
    ASTNode* simp = MathUtil::simplify(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "2 * x^2 + 3 * x + 1");
  }

  TEST_F(MathUtilTest, simplifyTest4) {
    ASTNode* ast = SBML_parseFormula("2 + x");
    ASTNode* simp = MathUtil::simplify(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "x + 2");
  }

  TEST_F(MathUtilTest, simplifyTest5) {
    ASTNode* ast = SBML_parseFormula("2 * x * 3");
    ASTNode* simp = MathUtil::simplify(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "6 * x");
  }

  TEST_F(MathUtilTest, simplifyTest6) {
    ASTNode* ast = SBML_parseFormula("2 * 4 * 5 * x * 3");
    ASTNode* simp = MathUtil::simplify(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "120 * x");
  }

  TEST_F(MathUtilTest, simplifyTest7) {
    ASTNode* ast = SBML_parseFormula("2 + x + 3");
    ASTNode* simp = MathUtil::simplify(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "x + 5");
  }

  TEST_F(MathUtilTest, simplifyTest8) {
    ASTNode* ast = SBML_parseFormula("2 + 4 + x + 3");
    ASTNode* simp = MathUtil::simplify(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "x + 9");
  }

  TEST_F(MathUtilTest, simplifyTest9) {
    ASTNode* ast = SBML_parseFormula("2 * sin(x) * 3 + y + 7 + 4 + z");
    ASTNode* simp = MathUtil::simplify(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "6 * sin(x) + y + 11 + z");
  }

  TEST_F(MathUtilTest, simplifyTest10) {
    ASTNode* ast = SBML_parseFormula("sin(2 * x) * 3");
    ASTNode* simp = MathUtil::simplify(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "3 * sin(2 * x)");
  }

  TEST_F(MathUtilTest, simplifyTestPower) {
    ASTNode* ast = SBML_parseFormula("3 * x ^ 1 + 4");
    ASTNode* simp = MathUtil::simplify(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "3 * x + 4");
  }

  TEST_F(MathUtilTest, simplifyTestPower2) {
    ASTNode* ast = SBML_parseFormula("3 * x ^ 0 + 4");
    ASTNode* simp = MathUtil::simplify(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "7");
  }

  TEST_F(MathUtilTest, simplifyTestPower3) {
    ASTNode* ast = SBML_parseFormula("(2 * 1 * pow(x, 2 - 1) + 0 * x + 3 * 1 + 0)");
    ASTNode* simp = MathUtil::simplify(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "2 * x + 3");
  }

  TEST_F(MathUtilTest, simplifyTestFalse) {
    ASTNode* ast = SBML_parseFormula("2 * x + (3 * 5 / 2) * y");
    ASTNode* simp = MathUtil::simplify(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_NE(s, "2 * x + 7 * y");
  }

  TEST_F(MathUtilTest, simplifyTestOneArg) {
    ASTNode* ast = SBML_parseFormula("sin(2 * x * 3)");
    ASTNode* simp = MathUtil::simplify(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_NE(s, "sin(6 * x)");
  }

  TEST_F(MathUtilTest, differentiateTest) {
    ASTNode* ast = SBML_parseFormula("x");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "1");
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
    ASTNode* ast = SBML_parseFormula("1 / x");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "-1 / x^2");
  }

  TEST_F(MathUtilTest, differentiateTestDiv2) {
    ASTNode* ast = SBML_parseFormula("3 * x^2 / (x - 5)^2");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "(6 * x * (x - 5)^2 - 3 * x^2 * 2 * (x - 5)) / (x - 5)^4");
  }

  TEST_F(MathUtilTest, differentiateTestPow) {
    ASTNode* ast = SBML_parseFormula("x^3 + 4");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "3 * x^2");
  }

  TEST_F(MathUtilTest, differentiateTestPow2) {
    ASTNode* ast = SBML_parseFormula("2^(x^3)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "2^x^3 * log(2) * 3 * x^2");
  }

  TEST_F(MathUtilTest, differentiateTestPow3) {
    ASTNode* ast = SBML_parseFormula("x^(2*x)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "2 * x * x^(2 * x - 1) + x^(2 * x) * log(x) * 2");
  }

  TEST_F(MathUtilTest, differentiateTestPow4) {
    ASTNode* ast = SBML_parseFormula("(2 * x)^3.5");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "7 * (2 * x)^2.5");
  }

  TEST_F(MathUtilTest, differentiateTestPowE) {
    // d{e^u}/dx = e^u*du/dx
    ASTNode* ast = SBML_parseL3Formula("exponentiale^(3 * x^2)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToL3String(diff);
    EXPECT_EQ(s, "exponentiale^(3 * x^2) * (6 * x)");
  }

  TEST_F(MathUtilTest, differentiateTestSQRT) {
    ASTNode* ast = SBML_parseFormula("sqrt(x)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "1 / 2 * x^(1 / 2 - 1)");
  }

  TEST_F(MathUtilTest, differentiateTestSin) {
    ASTNode* ast = SBML_parseFormula("sin(x^2+3*x+4)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "(2 * x + 3) * cos(x^2 + 3 * x + 4)");
  }

  TEST_F(MathUtilTest, differentiateTestCos) {
    ASTNode* ast = SBML_parseFormula("cos(x^2+3*x+4)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "-1 * (2 * x + 3) * sin(x^2 + 3 * x + 4)");
  }

  TEST_F(MathUtilTest, differentiateTestTan) {
    ASTNode* ast = SBML_parseFormula("tan(x^2+3*x+4)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "(2 * x + 3) * sec(x^2 + 3 * x + 4)^2");
  }

  TEST_F(MathUtilTest, differentiateTestSec) {
    ASTNode* ast = SBML_parseFormula("sec(x^2+3*x+4)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "(2 * x + 3) * sec(x^2 + 3 * x + 4) * tan(x^2 + 3 * x + 4)");
  }

  TEST_F(MathUtilTest, differentiateTestCot) {
    ASTNode* ast = SBML_parseFormula("sec(x^2+3*x)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "(2 * x + 3) * sec(x^2 + 3 * x) * tan(x^2 + 3 * x)");
  }

  TEST_F(MathUtilTest, differentiateTestSinh) {
    ASTNode* ast = SBML_parseFormula("sinh(x^2 + 3 * x)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "(2 * x + 3) * cosh(x^2 + 3 * x)");
  }

  TEST_F(MathUtilTest, differentiateTestCosh) {
    ASTNode* ast = SBML_parseFormula("cosh(x^2 + 3 * x)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "(2 * x + 3) * sinh(x^2 + 3 * x)");
  }

  TEST_F(MathUtilTest, differentiateTestTanh) {
    ASTNode* ast = SBML_parseFormula("tanh(x^2 + 3 * x)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "(2 * x + 3) * sech(x^2 + 3 * x)^2");
  }

  TEST_F(MathUtilTest, differentiateTestSech) {
    ASTNode* ast = SBML_parseFormula("sech(x^2+3*x+4)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "-1 * (2 * x + 3) * sech(x^2 + 3 * x + 4) * tanh(x^2 + 3 * x + 4)");
  }

  TEST_F(MathUtilTest, differentiateTestCoth) {
    ASTNode* ast = SBML_parseFormula("coth(x^2+3*x+4)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "(2 * x + 3) * (-1 / sinh(x^2 + 3 * x + 4)^2)");
  }

  TEST_F(MathUtilTest, differentiateTestAsin) {
    ASTNode* ast = SBML_parseFormula("asin(x^2+3*x+4)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "(2 * x + 3) / sqrt(1 - (x^2 + 3 * x + 4)^2)");
  }

  TEST_F(MathUtilTest, differentiateTestAcos) {
    ASTNode* ast = SBML_parseFormula("acos(x^2+3*x+4)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "-1 * (2 * x + 3) / sqrt(1 - (x^2 + 3 * x + 4)^2)");
  }

  TEST_F(MathUtilTest, differentiateTestAtan) {
    ASTNode* ast = SBML_parseFormula("atan(x^2+3*x+4)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "(2 * x + 3) / ((x^2 + 3 * x + 4)^2 + 1)");
  }

  TEST_F(MathUtilTest, differentiateTestArcsec) {
    ASTNode* ast = SBML_parseFormula("arcsec(2*x)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "2 / (abs(2 * x) * sqrt((2 * x)^2 - 1))");
  }

  TEST_F(MathUtilTest, differentiateTestArccsc) {
    ASTNode* ast = SBML_parseFormula("arccsc(2*x)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "-1 * (2 / (abs(2 * x) * sqrt((2 * x)^2 - 1)))");
  }

  TEST_F(MathUtilTest, differentiateTestArccot) {
    ASTNode* ast = SBML_parseFormula("arccot(x^2+3*x+4)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "-1 * ((2 * x + 3) / ((x^2 + 3 * x + 4)^2 + 1))");
  }

  TEST_F(MathUtilTest, differentiateTestArcsinh) {
    ASTNode* ast = SBML_parseFormula("arcsinh(x^2+3*x+4)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "(2 * x + 3) / sqrt(1 + (x^2 + 3 * x + 4)^2)");
  }

  TEST_F(MathUtilTest, differentiateTestArccosh) {
    ASTNode* ast = SBML_parseFormula("arccosh(x^2+3*x+4)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "(2 * x + 3) / sqrt((x^2 + 3 * x + 4)^2 - 1)");
  }

  TEST_F(MathUtilTest, differentiateTestArctanh) {
    ASTNode* ast = SBML_parseFormula("arctanh(x^2+3*x+4)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "(2 * x + 3) / (1 - (x^2 + 3 * x + 4)^2)");
  }

  TEST_F(MathUtilTest, differentiateTestArcsech) {
    ASTNode* ast = SBML_parseFormula("arcsech(2*x)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "-1 * (2 / (2 * x * sqrt(1 - (2 * x)^2)))");
  }

  TEST_F(MathUtilTest, differentiateTestArccsch) {
    ASTNode* ast = SBML_parseFormula("arccsch(3*x)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "-1 * (3 / (3 * x * sqrt((3 * x)^2 + 1)))");
  }

  TEST_F(MathUtilTest, differentiateTestArccoth) {
    ASTNode* ast = SBML_parseFormula("arccoth(x^2+3*x+4)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "(2 * x + 3) / (1 - (x^2 + 3 * x + 4)^2)");
  }

  TEST_F(MathUtilTest, differentiateTestAbs) {
    ASTNode* ast = SBML_parseFormula("abs(x^2+3*x+4)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "(2 * x + 3) * ((x^2 + 3 * x + 4) / abs(x^2 + 3 * x + 4))");
  }

  TEST_F(MathUtilTest, differentiateTestExp) {
    ASTNode* ast = SBML_parseFormula("exp(x^2+3*x+4)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "(2 * x + 3) * exp(x^2 + 3 * x + 4)");
  }

  TEST_F(MathUtilTest, differentiateTestLn) {
    ASTNode* ast = SBML_parseFormula("ln(x^2+3*x+4)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "(2 * x + 3) / (x^2 + 3 * x + 4)");
  }

  TEST_F(MathUtilTest, differentiateTestLog) {
    ASTNode* ast = SBML_parseFormula("log(10, x^2+3*x+4)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "(2 * x + 3) / ((x^2 + 3 * x + 4) * log(10))");
  }

  TEST_F(MathUtilTest, differentiateTestLog2) {
    ASTNode* ast = SBML_parseFormula("log(2, x^2+3*x+4)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "(2 * x + 3) / ((x^2 + 3 * x + 4) * log(2))");
  }

  TEST_F(MathUtilTest, differentiateTestCeil) {
    ASTNode* ast = SBML_parseFormula("ceil(x^2+3*x+4)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "0");
  }

  TEST_F(MathUtilTest, differentiateTestFloor) {
    ASTNode* ast = SBML_parseFormula("floor(x^2+3*x+4)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "0");
  }

  TEST_F(MathUtilTest, differentiateTestPiecewise1) {
    ASTNode* ast = SBML_parseFormula("piecewise(x, true, x^3)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "piecewise(1, true, 3 * x^2)");
  }

  TEST_F(MathUtilTest, differentiateTestPiecewise2) {
    ASTNode* ast = SBML_parseFormula("piecewise(x^2, false, sin(2*x))");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "piecewise(2 * x, false, 2 * cos(2 * x))");
  }

  TEST_F(MathUtilTest, differentiateTestPiecewise3) {
    ASTNode* ast = SBML_parseFormula("piecewise(x, false, x^3, true, sin(x^2))");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToString(diff);
    EXPECT_EQ(s, "piecewise(1, false, 3 * x^2, true, 2 * x * cos(x^2))");
  }

  TEST_F(MathUtilTest, differentiateTestFactorial) {
    ASTNode* ast = SBML_parseFormula("factorial(x)");
    ASTNode* diff = MathUtil::simplify(MathUtil::differentiate(ast, "x"));
    std::string s = SBML_formulaToL3String(diff);
    EXPECT_EQ(s, "(1 / 2) * (2 * pi * x)^(1 / 2 - 1) * (2 * pi) * (x / exponentiale)^x + sqrt(2 * pi * x) * (x * (x / exponentiale)^(x - 1) * (1 / exponentiale) + (x / exponentiale)^x * ln(x / exponentiale))");
  }

  /*
  TEST_F(MathUtilTest, taylorSeriesTest1) {
    ASTNode* ast = SBML_parseFormula("sin(x)");
    ASTNode* taylorSeries = MathUtil::simplify(MathUtil::taylorSeries(ast, "x", 0, 5));
    std::string s = SBML_formulaToString(taylorSeries);
    EXPECT_EQ(s, "x + -1 / 6 * x^3 + 1 / 120 * x^5");
  }

  TEST_F(MathUtilTest, taylorSeriesTest2) {
    ASTNode* ast = SBML_parseL3Formula("exponentiale^x");
    ASTNode* taylorSeries = MathUtil::simplify(MathUtil::taylorSeries(ast, "x", 0, 4));
    std::string s = SBML_formulaToString(taylorSeries);
    EXPECT_EQ(s, "x + 1 + 1 / 2 * x^2 + 1 / 6 * x^3 + 1 / 24 * x^4");
  }

  TEST_F(MathUtilTest, taylorSeriesTest3) {
    ASTNode* ast = SBML_parseL3Formula("1/(1-x)");
    ASTNode* taylorSeries = MathUtil::simplify(MathUtil::taylorSeries(ast, "x", 0, 5));
    std::string s = SBML_formulaToString(taylorSeries);
    EXPECT_EQ(s, "x + 1 + x^2 + x^3 + x^4 + x^5");
  }
   */

  TEST_F(MathUtilTest, isEqualTreeTrue1) {
    ASTNode* ast1 = SBML_parseFormula("a*b");
    ASTNode* ast2 = SBML_parseFormula("a*b");
    EXPECT_TRUE(MathUtil::isEqualTree(ast1, ast2));
  }

  TEST_F(MathUtilTest, isEqualTreeTrue2) {
    ASTNode* ast1 = SBML_parseFormula("a*b");
    ASTNode* ast2 = SBML_parseFormula("b*a");
    EXPECT_TRUE(MathUtil::isEqualTree(ast1, ast2));
  }

  TEST_F(MathUtilTest, isEqualTreeTrue3) {
    ASTNode* ast1 = SBML_parseFormula("sin(2*x)");
    ASTNode* ast2 = SBML_parseFormula("sin(x*2)");
    EXPECT_TRUE(MathUtil::isEqualTree(ast1, ast2));
  }

  TEST_F(MathUtilTest, isEqualTreeTrue4) {
    ASTNode* ast1 = SBML_parseFormula("x^p");
    ASTNode* ast2 = SBML_parseFormula("x^p");
    EXPECT_TRUE(MathUtil::isEqualTree(ast1, ast2));
  }

  TEST_F(MathUtilTest, isEqualTreeTrue5) {
    ASTNode* ast1 = SBML_parseFormula("2^p");
    ASTNode* ast2 = SBML_parseFormula("2^p");
    EXPECT_TRUE(MathUtil::isEqualTree(ast1, ast2));
  }

  TEST_F(MathUtilTest, isEqualTreeTrue6) {
    ASTNode* ast1 = SBML_parseFormula("x^3");
    ASTNode* ast2 = SBML_parseFormula("x^3");
    EXPECT_TRUE(MathUtil::isEqualTree(ast1, ast2));
  }

  TEST_F(MathUtilTest, isEqualTreeTrue7) {
    ASTNode* ast1 = SBML_parseFormula("2^3");
    ASTNode* ast2 = SBML_parseFormula("2^3");
    EXPECT_TRUE(MathUtil::isEqualTree(ast1, ast2));
  }

  TEST_F(MathUtilTest, isEqualTreeFalse1) {
    ASTNode* ast1 = SBML_parseFormula("a*b");
    ASTNode* ast2 = SBML_parseFormula("a*c");
    EXPECT_FALSE(MathUtil::isEqualTree(ast1, ast2));
  }

  TEST_F(MathUtilTest, isEqualTreeFalse2) {
    ASTNode* ast1 = SBML_parseFormula("sin(a*b)");
    ASTNode* ast2 = SBML_parseFormula("sin(a*c)");
    EXPECT_FALSE(MathUtil::isEqualTree(ast1, ast2));
  }

  TEST_F(MathUtilTest, isEqualTreeFalse3) {
    ASTNode* ast1 = SBML_parseFormula("sin(a*b)");
    ASTNode* ast2 = SBML_parseFormula("cos(a*b)");
    EXPECT_FALSE(MathUtil::isEqualTree(ast1, ast2));
  }

  /*
  TEST_F(MathUtilTest, simplifyNewTest1) {
    ASTNode* ast = SBML_parseFormula("((3 + 3 * 5) / 2) * x");
    ASTNode* simp = MathUtil::simplifyNew(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "9 * x");
  }

  TEST_F(MathUtilTest, simplifyNewTest2) {
    ASTNode* ast = SBML_parseFormula("2 * x + (3 * 5 / 2) * y");
    ASTNode* simp = MathUtil::simplifyNew(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "2 * x + 15 / 2 * y");
  }
   */

  TEST_F(MathUtilTest, simplifyNewTest3) {
    ASTNode* ast = SBML_parseFormula("x ^ 2 * 2 + x * 3 + 1");
    ASTNode* simp = MathUtil::simplifyNew(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "2 * x^2 + 3 * x + 1");
  }

  TEST_F(MathUtilTest, simplifyNewTest4) {
    ASTNode* ast = SBML_parseFormula("2 + x");
    ASTNode* simp = MathUtil::simplifyNew(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "x + 2");
  }

  TEST_F(MathUtilTest, simplifyNewTest5) {
    ASTNode* ast = SBML_parseFormula("2 * x * 3");
    ASTNode* simp = MathUtil::simplifyNew(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "6 * x");
  }

  TEST_F(MathUtilTest, simplifyNewTest6) {
    ASTNode* ast = SBML_parseFormula("2 * 4 * 5 * x * 3");
    ASTNode* simp = MathUtil::simplifyNew(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "120 * x");
  }

  TEST_F(MathUtilTest, simplifyNewTest7) {
    ASTNode* ast = SBML_parseFormula("2 + x + 3");
    ASTNode* simp = MathUtil::simplifyNew(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "x + 5");
  }

  TEST_F(MathUtilTest, simplifyNewTest8) {
    ASTNode* ast = SBML_parseFormula("2 + 4 + x + 3");
    ASTNode* simp = MathUtil::simplifyNew(ast);
    std::string s = SBML_formulaToString(simp);
    EXPECT_EQ(s, "x + 9");
  }

} // namespace
