#ifndef INCLUDE_SBMLSIM_INTERNAL_THIRDPARTY_LSODA_H_
#define INCLUDE_SBMLSIM_INTERNAL_THIRDPARTY_LSODA_H_

extern "C" {

typedef void    (*_lsoda_f) (double, double *, double *, void *);

void lsoda(_lsoda_f f, int neq, double *y, double *t, double tout, int itol, double *rtol, double *atol,
       int itask, int *istate, int iopt, int jt,
       int iwork1, int iwork2, int iwork5, int iwork6, int iwork7, int iwork8, int iwork9,
       double rwork1, double rwork5, double rwork6, double rwork7, void *_data);

} /* extern "C" */

#endif /* INCLUDE_SBMLSIM_INTERNAL_THIRDPARTY_LSODA_H_ */
