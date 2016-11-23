#ifndef INCLUDE_SBMLSIM_INTERNAL_UTIL_MATHUTIL_H_
#define INCLUDE_SBMLSIM_INTERNAL_UTIL_MATHUTIL_H_

class MathUtil {
 public:
  static double factorial(unsigned long long n);
  static long long ceil(double f);
  static double pow(double x, double y);
 private:
  MathUtil() {}
  ~MathUtil() {}
};

#endif /* INCLUDE_SBMLSIM_INTERNAL_UTIL_MATHUTIL_H_ */
