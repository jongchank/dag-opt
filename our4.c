#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "common.h"

#define N 4

int main(void)
{
    int n = N;
    double e1 = 4,  e2 = 9, e3 = 12,  e4 = 5;
    double en, e_ast;
    double real_p1, real_p_ast, real_p2, real_p3, real_p4;

    e_ast = e2 + e3;
    en = e4;
    real_p1 = e1 + sqrt((n - 2) * e1 * e_ast) + sqrt((ALPHA + BETA) * e1 * en / BETA);
    real_p_ast = real_p1 * sqrt((n - 2) * e_ast / e1);
    real_p2 = real_p_ast * e2 / e_ast;
    real_p3 = real_p_ast * e3 / e_ast;
    real_p4 = real_p1 * sqrt(BETA * en / ((ALPHA + BETA) * e1));

    printf("%g %g %g %g %g %g\n", real_p1, real_p2, real_p3, real_p4, jconv4(real_p1, real_p2, real_p3, real_p4), util4(e1, real_p1, e2, real_p2, e3, real_p3, e4, real_p4));

    return 0;
}
