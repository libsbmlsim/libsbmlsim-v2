#ifndef INCLUDE_SBMLSIM_INTERNAL_UTIL_MATHUTIL_H_
#define INCLUDE_SBMLSIM_INTERNAL_UTIL_MATHUTIL_H_

#include <sbml/SBMLTypes.h>
#include <string>

class MathUtil {
 public:
  static double factorial(unsigned long long n);
  static long long ceil(double f);
  static long long floor(double f);
  static double pow(double x, double y);
  static double exp(double x);
  static double fabs(double x);
  static bool isLong(double f);
  static bool isRationalForm(const ASTNode *ast);
  static bool containsTarget(const ASTNode *ast, std::string target);
  static ASTNode* simplify(const ASTNode *ast);
  static ASTNode* simplifyNew(const ASTNode *ast);
  static ASTNode* simplifyTwoPath(const ASTNode *ast);
  static ASTNode* simplifyRuleOne(const ASTNode *ast);
  static ASTNode* simplifyRuleTwo(const ASTNode *ast);
  static ASTNode* differentiate(const ASTNode *ast, std::string target);
  static ASTNode* reduceFraction(const ASTNode *ast);
  static ASTNode* taylorSeries(const ASTNode *ast, std::string target, double point, int order);
  static bool isEqualTree(const ASTNode *root1, const ASTNode *root2);
 private:
  MathUtil() {}
  ~MathUtil() {}
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_UTIL_MATHUTIL_H_ */
