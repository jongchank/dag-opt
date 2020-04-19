#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "common.h"

#define N 6

int main(int argc, char *argv[])
{
    int n = N;
    //double e1 = 80,  e2 = 60, e3 = 70,  e4 = 40, e5 = 110, e6 = 80;
    double e1, e2, e3, e4, e5, e6;
    double en, e_ast;
    double min_j, j, u, min_j_u;
    double real_p1, real_p_ast, real_p2, real_p3, real_p4, real_p5, real_p6;
    int p1, p2, p3, p4, p5, p6;
    int p1d, p2d, p3d, p4d, p5d, p6d;
    int opt_p1, opt_p2, opt_p3, opt_p4, opt_p5, opt_p6;

    if (argc != 7) {
        return -1;
    }
    e1 = atof(argv[1]);
    e2 = atof(argv[2]);
    e3 = atof(argv[3]);
    e4 = atof(argv[4]);
    e5 = atof(argv[5]);
    e6 = atof(argv[6]);

    e_ast = e2 + e5;
    en = e6;
    real_p1 = e1 + sqrt((n - 2) * e1 * e_ast) + sqrt((ALPHA + BETA) * e1 * en / BETA);
    real_p_ast = real_p1 * sqrt((n - 2) * e_ast / e1);
    real_p2 = real_p_ast * e2 / e_ast;
    real_p3 = real_p_ast * e3 / e_ast;
    real_p4 = real_p_ast * e4 / e_ast;
    real_p5 = real_p_ast * e5 / e_ast;
    real_p6 = real_p1 * sqrt(BETA * en / ((ALPHA + BETA) * e1));

    printf("%g %g %g %g %g %g %g %g\n", jconv6(real_p1, real_p2, real_p3, real_p4, real_p5, real_p6), util6(e1, real_p1, e2, real_p2, e3, real_p3, e4, real_p4, e5, real_p5, e6, real_p6), real_p1, real_p2, real_p3, real_p4, real_p5, real_p6);

    opt_p1 = p1d = (int) real_p1 + 1;
    opt_p2 = p2d = (int) real_p2 + 1;
    opt_p3 = p3d = (int) real_p3 + 1;
    opt_p4 = p4d = (int) real_p4 + 1;
    opt_p5 = p5d = (int) real_p5 + 1;
    opt_p6 = p6d = (int) real_p6 + 1;

    min_j = jconv6(opt_p1, opt_p2, opt_p3, opt_p4, opt_p5, opt_p6);
    min_j_u = util6(e1, opt_p1, e2, opt_p2, e3, opt_p3, e4, opt_p4, e5, opt_p5, e6, opt_p6);

    for (p1 = p1d; p1 <= p1d + 1; p1++) {
        for (p2 = p2d; p2 <= p2d + 1; p2++) {
            for (p3 = p3d; p3 <= p3d + 1; p3++) {
                for (p4 = p4d; p4 <= p4d + 1; p4++) {
                    for (p5 = p5d; p5 <= p5d + 1; p5++) {
                        for (p6 = p6d; p6 <= p5d + 1; p6++) {
                            u = util6(e1, p1, e2, p2, e3, p3, e4, p4, e5, p5, e6, p6);
                            j = jconv6(p1, p2, p3, p4, p5, p6); 
                            if (u <= UB && j < min_j) {
                                min_j = j;
                                min_j_u = u;
                                opt_p1 = p1; opt_p2 = p2; opt_p3 = p3; opt_p4 = p4, opt_p5 = p5; opt_p6 = p6;
                            }
                        }
                    }
                }
            }
        }
    }

    printf("%g %g %d %d %d %d %d %d\n", min_j, min_j_u, opt_p1, opt_p2, opt_p3, opt_p4, opt_p5, opt_p6);
    return 0;
}
