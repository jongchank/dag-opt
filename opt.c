#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define ALPHA (0.001)
#define BETA  (0.001)

#define MIN_P (1)
#define MAX_P (100)

static double j(double t, double delta);
static double jconv(double p1, double p2, double p3, double p4);
static double util1(double e1, double p1);
static double util2(double e1, double p1,
                    double e2, double p2);
static double util3(double e1, double p1,
                    double e2, double p2,
                    double e3, double p3);
static double util4(double e1, double p1,
                    double e2, double p2,
                    double e3, double p3,
                    double e4, double p4);
int main(void)
{
    int n = 4;
//    double e1 = 10, e2 = 6, e3 = 9,  e4 = 4;  // 1 
//    double e1 = 5,  e2 = 7, e3 = 10, e4 = 9;  // 2
//    double e1 = 8,  e2 = 3, e3 = 5,  e4 = 10; // 3
    double e1 = 4,  e2 = 9, e3 = 12,  e4 = 5; // 3
//    double e1 = 5,  e2 = 7, e3 = 6,  e4 = 9;  // 4
//    double e1 = 4,  e2 = 5, e3 = 10, e4 = 8;  // 5
//    double e1 = 5,  e2 = 4, e3 = 4,  e4 = 7;  // 6
//    double e1 = 3,  e2 = 7, e3 = 5,  e4 = 4;  // 7
//    double e1 = 9,  e2 = 7, e3 = 6,  e4 = 4;  // 8
//    double e1 = 8,  e2 = 6, e3 = 6,  e4 = 6;  // 9
//    double e1 = 4,  e2 = 7, e3 = 5,  e4 = 5;  // 10
    double e_ast;
    double min_j = 100;

    double u, opt_u;
    int p1, p2, p3, p4;
    int opt_p1, opt_p2, opt_p3, opt_p4;
    double real_p1, real_p_ast, real_p2, real_p3, real_p4;
    double j;

    for (p1 = MIN_P; p1 <= MAX_P; p1++) {
        if (jconv(p1, 1, 1, 1)  > min_j) break;
#if 1
        if (util1(e1, p1) > 1) continue;
#endif
        for (p2 = MIN_P; p2 <= MAX_P; p2++) {
            if (jconv(p1, p2, 1, 1) > min_j) break;
#if 1
            if (util2(e1, p1, e2, p2) > 1) continue;
#endif
            for (p3 = MIN_P; p3 <= MAX_P; p3++) {
                if (jconv(p1, p2, p3, 1) > min_j) break;
#if 1
                if (util3(e1, p1, e2, p2, e3, p3) > 1) continue;
#endif
                for (p4 = MIN_P; p4 <= MAX_P; p4++) {
                    u = util4(e1, p1, e2, p2, e3, p3, e4, p4);
                    j = jconv(p1, p2, p3, p4);
                    if (u <= 1 && j < min_j) {
                        min_j = j;
                        opt_p1 = p1; opt_p2 = p2; opt_p3 = p3; opt_p4 = p4;
                    }
                }
            }
        }
    }

    printf("-EXHAUSTIVE------------------------------------------\n");
    printf("P1 = %d P2 = %d P3 = %d P4 = %d\n", opt_p1, opt_p2, opt_p3, opt_p4);
    printf("J = %g\n", jconv(opt_p1, opt_p2, opt_p3, opt_p4) / 2);
    printf("U = %g\n", util4(e1, opt_p1, e2, opt_p2, e3, opt_p3, e4, opt_p4));

    printf("-OURS (REAL)-----------------------------------------\n");
    e_ast = e2 + e3;
    real_p1 = e1 + sqrt((n - 2) * e1 * e_ast) + sqrt((ALPHA + BETA) * e1 * e4 / BETA);
    real_p_ast = real_p1 * sqrt((n - 2) * e_ast / e1);
    real_p2 = real_p_ast * e2 / e_ast;
    real_p3 = real_p_ast * e3 / e_ast;
    real_p4 = real_p1 * sqrt(BETA * e4 / ((ALPHA + BETA) * e1));

    printf("P1 = %g P2 = %g P3 = %g P4 = %g\n", real_p1, real_p2, real_p3, real_p4);
    printf("J = %g\n", jconv(real_p1, real_p2, real_p3, real_p4) / 2);
    printf("U = %g\n", util4(e1, real_p1, e2, real_p2, e3, real_p3, e4, real_p4));
    return 0;
}

static double j(double t, double delta)
{
    return (ALPHA * t + BETA + delta);
}

static double jconv(double p1, double p2, double p3, double p4)
{
    return (2 * ALPHA * p4 + 2 * BETA * (p1 + p2 + p3 + p4));
}

static double util1(double e1, double p1)
{
    return (e1/p1);
}

static double util2(double e1, double p1,
                    double e2, double p2)
{
    return (e1/p1 + e2/p2);
}

static double util3(double e1, double p1,
                    double e2, double p2,
                    double e3, double p3)
{
    return (e1/p1 + e2/p2 + e3/p3);
}

static double util4(double e1, double p1,
                    double e2, double p2,
                    double e3, double p3,
                    double e4, double p4)
{
    return (e1/p1 + e2/p2 + e3/p3 + e4/p4);
}
