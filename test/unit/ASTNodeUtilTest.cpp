#include <gtest/gtest.h>
#include "sbmlsim/SBMLSim.h"
#include "sbmlsim/internal/util/ASTNodeUtil.h"
#include "sbmlsim/internal/util/MathUtil.h"

namespace {

  class ASTNodeUtilTest : public ::testing::Test{};

  TEST_F(ASTNodeUtilTest, isEqualTrueOperator) {
    ASTNode* ast1 = SBML_parseFormula("*");
    ASTNode* ast2 = SBML_parseFormula("*");
    EXPECT_TRUE(ASTNodeUtil::isEqual(ast1, ast2));
  }

  TEST_F(ASTNodeUtilTest, isEqualTrueFunc) {
    ASTNode* ast1 = SBML_parseFormula("sin(x)");
    ASTNode* ast2 = SBML_parseFormula("sin(y)");
    EXPECT_TRUE(ASTNodeUtil::isEqual(ast1, ast2));
  }

  TEST_F(ASTNodeUtilTest, isEqualTrueName) {
    ASTNode* ast1 = SBML_parseFormula("x");
    ASTNode* ast2 = SBML_parseFormula("x");
    EXPECT_TRUE(ASTNodeUtil::isEqual(ast1, ast2));
  }

  TEST_F(ASTNodeUtilTest, isEqualTrueInteger) {
    ASTNode* ast1 = SBML_parseFormula("7");
    ASTNode* ast2 = SBML_parseFormula("7");
    EXPECT_TRUE(ASTNodeUtil::isEqual(ast1, ast2));
  }

  TEST_F(ASTNodeUtilTest, isEqualTrueReal) {
    ASTNode* ast1 = SBML_parseFormula("2.71828");
    ASTNode* ast2 = SBML_parseFormula("2.71828");
    EXPECT_TRUE(ASTNodeUtil::isEqual(ast1, ast2));
  }

  TEST_F(ASTNodeUtilTest, isEqualTrueNullptr) {
    ASTNode* ast1 = nullptr;
    ASTNode* ast2 = nullptr;
    EXPECT_TRUE(ASTNodeUtil::isEqual(ast1, ast2));
  }

  TEST_F(ASTNodeUtilTest, isEquaFalseOperator) {
    ASTNode* ast1 = SBML_parseFormula("a + b");
    ASTNode* ast2 = SBML_parseFormula("a - b");
    EXPECT_FALSE(ASTNodeUtil::isEqual(ast1, ast2));
  }

  TEST_F(ASTNodeUtilTest, isEquaFalseFunc) {
    ASTNode* ast1 = SBML_parseFormula("sin(x)");
    ASTNode* ast2 = SBML_parseFormula("pow(y, 2)");
    EXPECT_FALSE(ASTNodeUtil::isEqual(ast1, ast2));
  }

  TEST_F(ASTNodeUtilTest, isEquaFalseName) {
    ASTNode* ast1 = SBML_parseFormula("x");
    ASTNode* ast2 = SBML_parseFormula("y");
    EXPECT_FALSE(ASTNodeUtil::isEqual(ast1, ast2));
  }

  TEST_F(ASTNodeUtilTest, isEqualFalseInteger) {
    ASTNode* ast1 = SBML_parseFormula("7");
    ASTNode* ast2 = SBML_parseFormula("5");
    EXPECT_FALSE(ASTNodeUtil::isEqual(ast1, ast2));
  }

  TEST_F(ASTNodeUtilTest, isEqualFalseReal) {
    ASTNode* ast1 = SBML_parseFormula("2.71828");
    ASTNode* ast2 = SBML_parseFormula("3.14159");
    EXPECT_FALSE(ASTNodeUtil::isEqual(ast1, ast2));
  }

  TEST_F(ASTNodeUtilTest, isEqualFalseNullptr1) {
    ASTNode* ast1 = SBML_parseFormula(nullptr);
    ASTNode* ast2 = SBML_parseFormula("5");
    EXPECT_FALSE(ASTNodeUtil::isEqual(ast1, ast2));
  }

  TEST_F(ASTNodeUtilTest, isEqualFalseNullptr2) {
    ASTNode* ast1 = SBML_parseFormula("5");
    ASTNode* ast2 = SBML_parseFormula(nullptr);
    EXPECT_FALSE(ASTNodeUtil::isEqual(ast1, ast2));
  }

  TEST_F(ASTNodeUtilTest, isEqualFalseDifferentDatatype1) {
    ASTNode* ast1 = SBML_parseFormula("2.0");
    ASTNode* ast2 = SBML_parseFormula("2");
    EXPECT_FALSE(ASTNodeUtil::isEqual(ast1, ast2));
  }

  TEST_F(ASTNodeUtilTest, isEqualFalseDifferentDatatype2) {
    ASTNode* ast1 = SBML_parseFormula("2.0");
    ASTNode* ast2 = SBML_parseFormula("sin(2.0)");
    EXPECT_FALSE(ASTNodeUtil::isEqual(ast1, ast2));
  }

  TEST_F(ASTNodeUtilTest, isEqualFalseDifferentDatatype3) {
    ASTNode* ast1 = SBML_parseFormula("x");
    ASTNode* ast2 = SBML_parseFormula("cos(x)");
    EXPECT_FALSE(ASTNodeUtil::isEqual(ast1, ast2));
  }

} // namespace
