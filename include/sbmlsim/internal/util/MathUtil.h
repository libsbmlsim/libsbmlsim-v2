#ifndef INCLUDE_SBMLSIM_INTERNAL_UTIL_MATHUTIL_H_
#define INCLUDE_SBMLSIM_INTERNAL_UTIL_MATHUTIL_H_

class MathUtil {
 public:
  static double factorial(unsigned long long n);
  static long long ceil(double f);
 private:
  MathUtil() {}
  ~MathUtil() {}
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_UTIL_MATHUTIL_H_ */
