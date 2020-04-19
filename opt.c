#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include "common.h"

static int exh4(int n, double e1, double e2, double e3, double e4);
static int our4(int n, double e1, double e2, double e3, double e4);
static int exh5(int n, double e1, double e2, double e3, double e4, double e5);
static int our5(int n, double e1, double e2, double e3, double e4, double e5);
static int exh6(int n, double e1, double e2, double e3, double e4, double e5, double e6);
static int our6(int n, double e1, double e2, double e3, double e4, double e5, double e6);

int main(int argc, char *argv[])
{
    double e1, e2, e3, e4, e5, e6;
    char *cmd;
    int n;

    if (argc < 3) {
        return -1;
    }

    cmd = argv[1];
    n = atoi(argv[2]);
   
    switch (n) {
    case 4:
        if (argc != 7) {
            return -1;
        }
        e1 = atof(argv[3]);
        e2 = atof(argv[4]);
        e3 = atof(argv[5]);
        e4 = atof(argv[6]);
        if (strncmp(cmd, "exh", 3) == 0) {
            exh4(n, e1, e2, e3, e4);
        } else if (strncmp(cmd, "our", 3) == 0) {
            our4(n, e1, e2, e3, e4);
        } else if (strncmp(cmd, "both", 4) == 0) {
            our4(n, e1, e2, e3, e4);
            exh4(n, e1, e2, e3, e4);
        } else {
            return -1;
        }
        break;
    case 5:
        if (argc != 8) {
            return -1;
        }
        e1 = atof(argv[3]);
        e2 = atof(argv[4]);
        e3 = atof(argv[5]);
        e4 = atof(argv[6]);
        e5 = atof(argv[7]);

        if (strncmp(cmd, "exh", 3) == 0) {
            exh5(n, e1, e2, e3, e4, e5);
        } else if (strncmp(cmd, "our", 3) == 0) {
            our5(n, e1, e2, e3, e4, e5);
        } else if (strncmp(cmd, "both", 4) == 0) {
            our5(n, e1, e2, e3, e4, e5);
            exh5(n, e1, e2, e3, e4, e5);
        } else {
            return -1;
        }
        break;
    case 6:
        if (argc != 9) {
            return -1;
        }
        e1 = atof(argv[3]);
        e2 = atof(argv[4]);
        e3 = atof(argv[5]);
        e4 = atof(argv[6]);
        e5 = atof(argv[7]);
        e6 = atof(argv[8]);

        if (strncmp(cmd, "exh", 3) == 0) {
            exh6(n, e1, e2, e3, e4, e5, e6);
        } else if (strncmp(cmd, "our", 3) == 0) {
            our6(n, e1, e2, e3, e4, e5, e6);
        } else if (strncmp(cmd, "both", 4) == 0) {
            our6(n, e1, e2, e3, e4, e5, e6);
            exh6(n, e1, e2, e3, e4, e5, e6);
        } else {
            return -1;
        }
        break;
    default:
        return -1;
    }

    return 0;
}

static int exh4(int n, double e1, double e2, double e3, double e4)
{
    struct timeval start, end;
    long secs, usecs;
    double min_j, min_j_u;

    double u, opt_u;
    int p1, p2, p3, p4;
    int opt_p1, opt_p2, opt_p3, opt_p4;
    double j;

    gettimeofday(&start, NULL);
    min_j = jconv4(P_MAX, P_MAX, P_MAX, P_MAX);
    for (p1 = P_MIN; p1 <= P_MAX; p1 += P_STRIDE) {
#if BNB == 1
        if (jconv4(p1, P_MIN, P_MIN, P_MIN)  > min_j)
            break;
        if (util4(e1, p1, e2, P_MAX, e3, P_MAX, e4, P_MAX) > UB)
            continue;
#endif
        for (p2 = P_MIN; p2 <= P_MAX; p2 += P_STRIDE) {
#if BNB == 1
            if (jconv4(p1, p2, P_MIN, P_MIN) > min_j)
                break;
            if (util4(e1, p1, e2, p2, e3, P_MAX, e4, P_MAX) > UB)
                continue;
#endif
            for (p3 = P_MIN; p3 <= P_MAX; p3 += P_STRIDE) {
#if BNB == 1
                if (jconv4(p1, p2, p3, P_MIN) > min_j)
                    break;
                if (util4(e1, p1, e2, p2, e3, p3, e4, P_MAX) > UB)
                    continue;
#endif
                for (p4 = P_MIN; p4 <= P_MAX; p4 += P_STRIDE) {
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
    gettimeofday(&end, NULL);
    timediff(&start, &end, &secs, &usecs);

    printf("%g %g %d %d %d %d (%ld.%06ld)\n", min_j, min_j_u, opt_p1, opt_p2, opt_p3, opt_p4, secs, usecs);

    return 0;
}

static int our4(int n, double e1, double e2, double e3, double e4)
{
    struct timeval start, end;
    long secs, usecs;
    double en, e_ast;
    double min_j, j, u, min_j_u;
    double real_p1, real_p_ast, real_p2, real_p3, real_p4;
    int p1, p2, p3, p4;
    int p1d, p2d, p3d, p4d;
    int opt_p1, opt_p2, opt_p3, opt_p4;

    gettimeofday(&start, NULL);

    e_ast = e2 + e3;
    en = e4;
    real_p1 = e1 + sqrt((n - 2) * e1 * e_ast) + sqrt((ALPHA + BETA) * e1 * en / BETA);
    real_p_ast = real_p1 * sqrt((n - 2) * e_ast / e1);
    real_p2 = real_p_ast * e2 / e_ast;
    real_p3 = real_p_ast * e3 / e_ast;
    real_p4 = real_p1 * sqrt(BETA * en / ((ALPHA + BETA) * e1));

    gettimeofday(&end, NULL);
    secs = (end.tv_sec - start.tv_sec); //avoid overflow by subtracting first
    usecs = (end.tv_usec - start.tv_usec);
    if (usecs < 0) {
        secs -= 1;
        usecs = 1000000 + usecs;
    }

    printf("%g %g %g %g %g %g (%ld.%06ld)\n", jconv4(real_p1, real_p2, real_p3, real_p4), util4(e1, real_p1, e2, real_p2, e3, real_p3, e4, real_p4), real_p1, real_p2, real_p3, real_p4, secs, usecs);

    gettimeofday(&start, NULL);
    /*************************/
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
    /*************************/
    gettimeofday(&end, NULL);
    timediff(&start, &end, &secs, &usecs);

    printf("%g %g %d %d %d %d (%ld.%06ld)\n", min_j, min_j_u, opt_p1, opt_p2, opt_p3, opt_p4, secs, usecs);
    return 0;
}

static int exh5(int n, double e1, double e2, double e3, double e4, double e5)
{
    struct timeval start, end;
    long secs, usecs;
    double min_j, min_j_u;

    double u, opt_u;
    int p1, p2, p3, p4, p5;
    int opt_p1, opt_p2, opt_p3, opt_p4, opt_p5;
    double j;

    gettimeofday(&start, NULL);

    min_j = jconv5(P_MAX, P_MAX, P_MAX, P_MAX, P_MAX);
    for (p1 = P_MIN; p1 <= P_MAX; p1 += P_STRIDE) {
#if BNB == 1
        if (jconv5(p1, P_MIN, P_MIN, P_MIN, P_MIN)  > min_j)
            break;
        if (util5(e1, p1, e2, P_MAX, e3, P_MAX, e4, P_MAX, e5, P_MAX) > UB)
            continue;
#endif
        for (p2 = P_MIN; p2 <= P_MAX; p2 += P_STRIDE) {
#if BNB == 1
            if (jconv5(p1, p2, P_MIN, P_MIN, P_MIN) > min_j)
                break;
            if (util5(e1, p1, e2, p2, e3, P_MAX, e4, P_MAX, e5, P_MAX) > UB)
                continue;
#endif
            for (p3 = P_MIN; p3 <= P_MAX; p3 += P_STRIDE) {
#if BNB == 1
                if (jconv5(p1, p2, p3, P_MIN, P_MIN) > min_j)
                    break;
                if (util5(e1, p1, e2, p2, e3, p3, e4, P_MAX, e5, P_MAX) > UB)
                    continue;
#endif
                for (p4 = P_MIN; p4 <= P_MAX; p4 += P_STRIDE) {
#if BNB == 1
                    if (jconv5(p1, p2, p3, p4, P_MIN) > min_j)
                        break;
                    if (util5(e1, p1, e2, p2, e3, p3, e4, p4, e5, P_MAX) > UB)
                        continue;
#endif
                    for (p5 = P_MIN; p5 <= P_MAX; p5 += P_STRIDE) {
                        u = util5(e1, p1, e2, p2, e3, p3, e4, p4, e5, p5);
                        j = jconv5(p1, p2, p3, p4, p5);
                        if (u <= UB && j < min_j) {
                            min_j = j;
                            min_j_u = u;
                            opt_p1 = p1; opt_p2 = p2; opt_p3 = p3; opt_p4 = p4; opt_p5 = p5;
                        }
                    }
                }
            }
        }
    }

    gettimeofday(&end, NULL);
    timediff(&start, &end, &secs, &usecs);

    printf("%g %g %d %d %d %d %d (%ld.%06ld)\n", min_j, min_j_u, opt_p1, opt_p2, opt_p3, opt_p4, opt_p5, secs, usecs);

    return 0;
}

static int our5(int n, double e1, double e2, double e3, double e4, double e5)
{
    struct timeval start, end;
    long secs, usecs;
    double en, e_ast;
    double min_j, j, u, min_j_u;
    double real_p1, real_p_ast, real_p2, real_p3, real_p4, real_p5;
    int p1, p2, p3, p4, p5;
    int p1d, p2d, p3d, p4d, p5d;
    int opt_p1, opt_p2, opt_p3, opt_p4, opt_p5;

    gettimeofday(&start, NULL);

    e_ast = e2 + e3;
    en = e5;
    real_p1 = e1 + sqrt((n - 2) * e1 * e_ast) + sqrt((ALPHA + BETA) * e1 * en / BETA);
    real_p_ast = real_p1 * sqrt((n - 2) * e_ast / e1);
    real_p2 = real_p_ast * e2 / e_ast;
    real_p3 = real_p_ast * e3 / e_ast;
    real_p4 = real_p_ast * e4 / e_ast;
    real_p5 = real_p1 * sqrt(BETA * en / ((ALPHA + BETA) * e1));

    gettimeofday(&end, NULL);
    timediff(&start, &end, &secs, &usecs);

    printf("%g %g %g %g %g %g %g (%ld.%06ld)\n", jconv5(real_p1, real_p2, real_p3, real_p4, real_p5), util5(e1, real_p1, e2, real_p2, e3, real_p3, e4, real_p4, e5, real_p5), real_p1, real_p2, real_p3, real_p4, real_p5, secs, usecs);

    gettimeofday(&start, NULL);
   
    opt_p1 = p1d = (int) real_p1 + 1;
    opt_p2 = p2d = (int) real_p2 + 1;
    opt_p3 = p3d = (int) real_p3 + 1;
    opt_p4 = p4d = (int) real_p4 + 1;
    opt_p5 = p5d = (int) real_p5 + 1;

    min_j = jconv5(opt_p1, opt_p2, opt_p3, opt_p4, opt_p5);
    min_j_u = util5(e1, opt_p1, e2, opt_p2, e3, opt_p3, e4, opt_p4, e5, opt_p5);

    for (p1 = p1d; p1 <= p1d + 1; p1++) {
        for (p2 = p2d; p2 <= p2d + 1; p2++) {
            for (p3 = p3d; p3 <= p3d + 1; p3++) {
                for (p4 = p4d; p4 <= p4d + 1; p4++) {
                    for (p5 = p5d; p5 <= p5d + 1; p5++) {
                        u = util5(e1, p1, e2, p2, e3, p3, e4, p4, e5, p5);
                        j = jconv5(p1, p2, p3, p4, p5); 
                        if (u <= UB && j < min_j) {
                            min_j = j;
                            min_j_u = u;
                            opt_p1 = p1; opt_p2 = p2; opt_p3 = p3; opt_p4 = p4, opt_p5 = p5;
                        }
                    }
                }
            }
        }
    }

    gettimeofday(&end, NULL);
    timediff(&start, &end, &secs, &usecs);

    printf("%g %g %d %d %d %d %d (%ld.%06ld)\n", min_j, min_j_u, opt_p1, opt_p2, opt_p3, opt_p4, opt_p5, secs, usecs);
    return 0;
}

static int exh6(int n, double e1, double e2, double e3, double e4, double e5, double e6)
{
    struct timeval start, end;
    long secs, usecs;
    double min_j, min_j_u;

    double u, opt_u;
    int p1, p2, p3, p4, p5, p6;
    int opt_p1, opt_p2, opt_p3, opt_p4, opt_p5, opt_p6;
    double j;

    gettimeofday(&start, NULL);

    min_j = jconv6(P_MAX, P_MAX, P_MAX, P_MAX, P_MAX, P_MAX);
    for (p1 = P_MIN; p1 <= P_MAX; p1 += P_STRIDE) {
#if BNB == 1
        if (jconv6(p1, P_MIN, P_MIN, P_MIN, P_MIN, P_MIN)  > min_j)
            break;
        if (util6(e1, p1, e2, P_MAX, e3, P_MAX, e4, P_MAX, e5, P_MAX, e6, P_MAX) > UB)
            continue;
#endif
        for (p2 = P_MIN; p2 <= P_MAX; p2 += P_STRIDE) {
#if BNB == 1
            if (jconv6(p1, p2, P_MIN, P_MIN, P_MIN, P_MIN) > min_j)
                break;
            if (util6(e1, p1, e2, p2, e3, P_MAX, e4, P_MAX, e5, P_MAX, e6, P_MAX) > UB)
                continue;
#endif
            for (p3 = P_MIN; p3 <= P_MAX; p3 += P_STRIDE) {
#if BNB == 1
                if (jconv6(p1, p2, p3, P_MIN, P_MIN, P_MIN) > min_j)
                    break;
                if (util6(e1, p1, e2, p2, e3, p3, e4, P_MAX, e5, P_MAX, e6, P_MAX) > UB)
                    continue;
#endif
                for (p4 = P_MIN; p4 <= P_MAX; p4 += P_STRIDE) {
#if BNB == 1
                    if (jconv6(p1, p2, p3, p4, P_MIN, P_MIN) > min_j)
                        break;
                    if (util6(e1, p1, e2, p2, e3, p3, e4, p4, e5, P_MAX, e6, P_MAX) > UB)
                        continue;
#endif
                    for (p5 = P_MIN; p5 <= P_MAX; p5 += P_STRIDE) {
#if BNB == 1
                        if (jconv6(p1, p2, p3, p4, p5, P_MIN) > min_j)
                            break;
                        if (util6(e1, p1, e2, p2, e3, p3, e4, p4, e5, p5, e6, P_MAX) > UB)
                            continue;
#endif
                        for (p6 = P_MIN; p6 <= P_MAX; p6 += P_STRIDE) {
                            u = util6(e1, p1, e2, p2, e3, p3, e4, p4, e5, p5, e6, p6);
                            j = jconv6(p1, p2, p3, p4, p5, p6);
                            if (u <= UB && j < min_j) {
                                min_j = j;
                                min_j_u = u;
                                opt_p1 = p1; opt_p2 = p2; opt_p3 = p3; opt_p4 = p4; opt_p5 = p5; opt_p6 = p6;
                            }
                        }
                    }
                }
            }
        }
    }

    gettimeofday(&end, NULL);
    timediff(&start, &end, &secs, &usecs);

    printf("%g %g %d %d %d %d %d %d (%ld.%06ld)\n", min_j, min_j_u, opt_p1, opt_p2, opt_p3, opt_p4, opt_p5, opt_p6, secs, usecs);

    return 0;
}

static int our6(int n, double e1, double e2, double e3, double e4, double e5, double e6)
{
    struct timeval start, end;
    long secs, usecs;
    double en, e_ast;
    double min_j, j, u, min_j_u;
    double real_p1, real_p_ast, real_p2, real_p3, real_p4, real_p5, real_p6;
    int p1, p2, p3, p4, p5, p6;
    int p1d, p2d, p3d, p4d, p5d, p6d;
    int opt_p1, opt_p2, opt_p3, opt_p4, opt_p5, opt_p6;

    gettimeofday(&start, NULL);

    e_ast = e2 + e5;
    en = e6;
    real_p1 = e1 + sqrt((n - 2) * e1 * e_ast) + sqrt((ALPHA + BETA) * e1 * en / BETA);
    real_p_ast = real_p1 * sqrt((n - 2) * e_ast / e1);
    real_p2 = real_p_ast * e2 / e_ast;
    real_p3 = real_p_ast * e3 / e_ast;
    real_p4 = real_p_ast * e4 / e_ast;
    real_p5 = real_p_ast * e5 / e_ast;
    real_p6 = real_p1 * sqrt(BETA * en / ((ALPHA + BETA) * e1));

    gettimeofday(&end, NULL);
    timediff(&start, &end, &secs, &usecs);

    printf("%g %g %g %g %g %g %g %g (%ld.%06ld)\n", jconv6(real_p1, real_p2, real_p3, real_p4, real_p5, real_p6), util6(e1, real_p1, e2, real_p2, e3, real_p3, e4, real_p4, e5, real_p5, e6, real_p6), real_p1, real_p2, real_p3, real_p4, real_p5, real_p6, secs, usecs);

    gettimeofday(&start, NULL);

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

    gettimeofday(&end, NULL);
    timediff(&start, &end, &secs, &usecs);

    printf("%g %g %d %d %d %d %d %d (%ld.%06ld)\n", min_j, min_j_u, opt_p1, opt_p2, opt_p3, opt_p4, opt_p5, opt_p6, secs, usecs);
    return 0;
}

