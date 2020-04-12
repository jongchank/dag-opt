#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define ALPHA (0.001)
#define BETA  (0.001)

#define MIN_P (1)
#define MAX_P (100)

static double j(double t, double delta);
static double jconv5(double p1, double p2, double p3, double p4, double p5);
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
static double util5(double e1, double p1,
                    double e2, double p2,
                    double e3, double p3,
                    double e4, double p4,
                    double e5, double p5);
int main(void)
{
    int n = 5;
    double e1 = 12,  e2 = 8, e3 = 11,  e4 = 6, e5 = 10;
    double en, e_ast;
    double min_j = 100;

    double u, opt_u;
    int p1, p2, p3, p4, p5;
    int opt_p1, opt_p2, opt_p3, opt_p4, opt_p5;
    double real_p1, real_p_ast, real_p2, real_p3, real_p4, real_p5;
    double j;

    for (p1 = MIN_P; p1 <= MAX_P; p1++) {
        if (jconv5(p1, 1, 1, 1, 1)  > min_j)
            break;
        if (util1(e1, p1) > 1)
            continue;
        for (p2 = MIN_P; p2 <= MAX_P; p2++) {
            if (jconv5(p1, p2, 1, 1, 1) > min_j)
                break;
            if (util2(e1, p1, e2, p2) > 1)
                continue;
            for (p3 = MIN_P; p3 <= MAX_P; p3++) {
                if (jconv5(p1, p2, p3, 1, 1) > min_j)
                    break;
                if (util3(e1, p1, e2, p2, e3, p3) > 1)
                    continue;
                for (p4 = MIN_P; p4 <= MAX_P; p4++) {
                    if (jconv5(p1, p2, p3, p4, 1) > min_j)
                        break;
                    if (util4(e1, p1, e2, p2, e3, p3, e4, p4) > 1)
                        continue;
                    for (p5 = MIN_P; p5 <= MAX_P; p5++) {
                        u = util5(e1, p1, e2, p2, e3, p3, e4, p4, e5, p5);
                        j = jconv5(p1, p2, p3, p4, p5);
                        if (u <= 1 && j < min_j) {
                            min_j = j;
                            opt_p1 = p1; opt_p2 = p2; opt_p3 = p3; opt_p4 = p4; opt_p5 = p5;
                        }
                    }
                }
            }
        }
    }

    printf("-EXHAUSTIVE------------------------------------------\n");
    printf("P1 = %d P2 = %d P3 = %d P4 = %d P5 = %d\n", opt_p1, opt_p2, opt_p3, opt_p4, opt_p5);
    printf("J = %g\n", jconv5(opt_p1, opt_p2, opt_p3, opt_p4, opt_p5));
    printf("U = %g\n", util5(e1, opt_p1, e2, opt_p2, e3, opt_p3, e4, opt_p4, e5, opt_p5));

    printf("-OURS (REAL)-----------------------------------------\n");
    e_ast = e2 + e3;
    en = e5;
    real_p1 = e1 + sqrt((n - 2) * e1 * e_ast) + sqrt((ALPHA + BETA) * e1 * en / BETA);
    real_p_ast = real_p1 * sqrt((n - 2) * e_ast / e1);
    real_p2 = real_p_ast * e2 / e_ast;
    real_p3 = real_p_ast * e3 / e_ast;
    real_p4 = real_p_ast * e4 / e_ast;
    real_p5 = real_p1 * sqrt(BETA * en / ((ALPHA + BETA) * e1));

    printf("P1 = %g P2 = %g P3 = %g P4 = %g P5 = %g\n", real_p1, real_p2, real_p3, real_p4, real_p5);
    printf("J = %g\n", jconv5(real_p1, real_p2, real_p3, real_p4, real_p5));
    printf("U = %g\n", util5(e1, real_p1, e2, real_p2, e3, real_p3, e4, real_p4, e5, real_p5));
    return 0;
}

static double j(double t, double delta)
{
    return (ALPHA * t + BETA + delta);
}

static double jconv5(double p1, double p2, double p3, double p4, double p5)
{
    return (2 * ALPHA * p5 + 2 * BETA * (p1 + p2 + p3 + p5));
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

static double util5(double e1, double p1,
                    double e2, double p2,
                    double e3, double p3,
                    double e4, double p4,
                    double e5, double p5)
{
    return (e1/p1 + e2/p2 + e3/p3 + e4/p4 + e5/p5);
}
