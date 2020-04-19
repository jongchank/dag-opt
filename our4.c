#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "common.h"

#define N 4

int main(int argc, char *argv[])
{
    int n = N;
    //double e1 = 40,  e2 = 90, e3 = 120,  e4 = 50;
    double e1,  e2, e3,  e4;
    double en, e_ast;
    double min_j, j, u, min_j_u;
    double real_p1, real_p_ast, real_p2, real_p3, real_p4;
    int p1, p2, p3, p4;
    int p1d, p2d, p3d, p4d;
    int opt_p1, opt_p2, opt_p3, opt_p4;

    if (argc != 5) {
        return -1;
    }
    e1 = atof(argv[1]);
    e2 = atof(argv[2]);
    e3 = atof(argv[3]);
    e4 = atof(argv[4]);

    e_ast = e2 + e3;
    en = e4;
    real_p1 = e1 + sqrt((n - 2) * e1 * e_ast) + sqrt((ALPHA + BETA) * e1 * en / BETA);
    real_p_ast = real_p1 * sqrt((n - 2) * e_ast / e1);
    real_p2 = real_p_ast * e2 / e_ast;
    real_p3 = real_p_ast * e3 / e_ast;
    real_p4 = real_p1 * sqrt(BETA * en / ((ALPHA + BETA) * e1));

    printf("%g %g %g %g %g %g\n", jconv4(real_p1, real_p2, real_p3, real_p4), util4(e1, real_p1, e2, real_p2, e3, real_p3, e4, real_p4), real_p1, real_p2, real_p3, real_p4);

    opt_p1 = p1d = (int) real_p1 + 1;
    opt_p2 = p2d = (int) real_p2 + 1;
    opt_p3 = p3d = (int) real_p3 + 1;
    opt_p4 = p4d = (int) real_p4 + 1;

    min_j = jconv4(opt_p1, opt_p2, opt_p3, opt_p4);
    min_j_u = util4(e1, opt_p1, e2, opt_p2, e3, opt_p3, e4, opt_p4);

    for (p1 = p1d; p1 <= p1d + 1; p1++) {
        for (p2 = p2d; p2 <= p2d + 1; p2++) {
            for (p3 = p3d; p3 <= p3d + 1; p3++) {
                for (p4 = p4d; p4 <= p4d + 1; p4++) {
                    u = util4(e1, p1, e2, p2, e3, p3, e4, p4);
                    j = jconv4(p1, p2, p3, p4); 
                    if (u <= UB && j < min_j) {
                        min_j = j;
                        min_j_u = u;
                        opt_p1 = p1; opt_p2 = p2; opt_p3 = p3; opt_p4 = p4;
                    }
                }
            }
        }
    }

    printf("%d %d %d %d %g %g\n", opt_p1, opt_p2, opt_p3, opt_p4, min_j, min_j_u);
    return 0;
}
