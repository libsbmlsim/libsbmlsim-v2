#ifndef INCLUDE_SBMLSIM_INTERNAL_UTIL_MATHUTIL_H_
#define INCLUDE_SBMLSIM_INTERNAL_UTIL_MATHUTIL_H_

#include <sbml/SBMLTypes.h>
#include <string>

class MathUtil {
 public:
  static double factorial(unsigned long long n);
  static long long ceil(double f);
  static bool containsTarget(const ASTNode *ast, std::string target);
  static ASTNode* simplify(ASTNode *ast);
  static ASTNode* differentiate(const ASTNode *ast, std::string target);
 private:
  MathUtil() {}
  ~MathUtil() {}
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_UTIL_MATHUTIL_H_ */
