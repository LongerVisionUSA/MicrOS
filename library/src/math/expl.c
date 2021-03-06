#include "../math.h"

long double expl(long double x)
{
    feclearexcept(FE_OVERFLOW);

    long double result = powl(M_E, x);
    
    fexcept_t exceptions = __FPU_read_status_word();
    if(exceptions.overflow == 0)
    {
        return result;
    }
    // range error
    else
    {
        if(_math_errhandling == MATH_ERRNO)
        {
            errno = ERANGE;
            feclearexcept(FE_OVERFLOW);
        }
        // In other case overflow is already set so we don't need to cover _math_errhandling == MATH_ERREXCEPT
        return x > 0 ? HUGE_VALL : copysignl(HUGE_VALL, -1.0);
    }
}