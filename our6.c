#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "common.h"

#define N 6

int main(void)
{
    int n = N;
    double e1 = 8,  e2 = 6, e3 = 7,  e4 = 4, e5 = 11, e6 = 8;
    double en, e_ast;
    double real_p1, real_p_ast, real_p2, real_p3, real_p4, real_p5, real_p6;

    e_ast = e2 + e5;
    en = e6;
    real_p1 = e1 + sqrt((n - 2) * e1 * e_ast) + sqrt((ALPHA + BETA) * e1 * en / BETA);
    real_p_ast = real_p1 * sqrt((n - 2) * e_ast / e1);
    real_p2 = real_p_ast * e2 / e_ast;
    real_p3 = real_p_ast * e3 / e_ast;
    real_p4 = real_p_ast * e4 / e_ast;
    real_p5 = real_p_ast * e5 / e_ast;
    real_p6 = real_p1 * sqrt(BETA * en / ((ALPHA + BETA) * e1));

    printf("%g %g %g %g %g %g %g %g\n", real_p1, real_p2, real_p3, real_p4, real_p5, real_p6, jconv6(real_p1, real_p2, real_p3, real_p4, real_p5, real_p6), util6(e1, real_p1, e2, real_p2, e3, real_p3, e4, real_p4, e5, real_p5, e6, real_p6));

    return 0;
}
