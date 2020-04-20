#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include "common.h"

#define N_MAX 10
#define BNB   1
#define UB    1.0
#define ALPHA 0.01
#define BETA  0.01
#define P_STRIDE 10
#define P_MIN    P_STRIDE
#define P_MAX    1000

double east(double e[], int n);
static int exhaustive(double e[], int n);
static int ours(double e[], int n);
static int exhn(double e[], int n);
static void show_result(const char *method, double j, double u, int p[], int n, long secs, long usecs);
static void show_result_double(const char *method, double j, double u, double p[], int n, long secs, long usecs);
static void usage(const char *prog);
static void timediff(struct timeval *start, struct timeval *end, long *secs, long *usecs);
static double jmax(int n);
static double jconv(int p[], int n);
static double jconvf(double p[], int n);
static double util(int p[], double e[], int n);
static double utilf(double p[], double e[], int n);

int main(int argc, char *argv[])
{
    double e[N_MAX + 1];
    char *cmd;
    int i, n, rc;

    if (argc < 3) {
        usage(argv[0]);
        return 1;
    }
    cmd = argv[1];
    n = atoi(argv[2]);
    if (n + 3 != argc) {
        usage(argv[0]);
        return 1;
    }
    for (i = 0; i < n; i++) {
        e[i + 1] = atof(argv[i + 3]);
    }

    if (strncmp(cmd, "exh", 3) == 0) {
        printf("----------------------------------------------------------------------------------------\n");
        rc = exhaustive(e, n);
    }
    else if (strncmp(cmd, "our", 3) == 0) {
        printf("----------------------------------------------------------------------------------------\n");
        rc = ours(e, n);
    }
    else {
        rc = -1;
    }
    return rc;
}

static int ours(double e[], const int n)
{
    struct timeval start, end;
    long secs, usecs;
    double e_ast;
    double p[N_MAX + 1];
    double p_ast;
    int i;

    gettimeofday(&start, NULL);
    e_ast = east(e, n);

    p[1] = e[1] + sqrt((n -2) * e[1] * e_ast) + sqrt((ALPHA + BETA) * e[1] * e[n] / BETA); 
    p_ast = p[1] * sqrt((n - 2) * e_ast / e[1]);
    for (i = 2; i <= n-1; i++) {
        p[i] = p_ast * (e[i] / e_ast);
    }
    p[n] = p[1] * sqrt(BETA * e[n] / ((ALPHA + BETA) * e[1]));
    gettimeofday(&end, NULL);
    timediff(&start, &end, &secs, &usecs);

    show_result_double("OURS (REAL)", jconvf(p, n), utilf(p, e, n), p, n, secs, usecs);
    
    return 0;
}

static int exhaustive(double e[], const int n)
{
    struct timeval start, end;
    long secs, usecs;
    int i;
    int p[N_MAX + 1];
    int opt_p[N_MAX + 1];
    double min_j, min_j_u;
    double u, opt_u;
    double j;

    gettimeofday(&start, NULL);
    min_j = jmax(n);
    for (i = 1; i <= n; i++) {
        p[i] = P_MIN;
    }
    for (;;) {
        int end_flag = 0;
        /* loop body begins here */
        u = util(p, e, n);
        if (u > UB) {
            goto loop;
        }
        j = jconv(p, n);
        if (j < min_j) {
            min_j = j;
            min_j_u = u;
            memcpy(opt_p, p, sizeof(p));
        }
        /* loop body ends here */
loop:
        for (i = n; i >= 0; i--) {
            if (i == 0) {
                end_flag = 1;
                break;
            }
            p[i] += P_STRIDE;
            if (p[i] > P_MAX) {
                p[i] = P_MIN;
            }
            else {
                break;
            }
        }
        if (end_flag) {
            break;
        }
    }
    gettimeofday(&end, NULL);
    timediff(&start, &end, &secs, &usecs);
    show_result("EXHAUSTIVE", min_j, min_j_u, opt_p, n, secs, usecs); 

    return 0;
}

static void show_result(const char *method, double j, double u, int p[], int n, long secs, long usecs) 
{
    int i;

    printf("%s  %-6.2f %-6.2f (", method, j, u * 100);
    for (i = 1; i <= n; i++) {
        if (i != n) {
            printf("%-6d, ", p[i]);
        }
        else {
            printf("%-6d)  ", p[i]);
        }
    }
    printf("<%ld.%06ld>\n", secs, usecs);

    return;
}

static void show_result_double(const char *method, double j, double u, double p[], int n, long secs, long usecs) 
{
    int i;

    printf("%s  %-6.2f %-6.2f (", method, j, u * 100);
    for (i = 1; i <= n; i++) {
        if (i != n) {
            printf("%-3.2f, ", p[i]);
        }
        else {
            printf("%-3.2f)  ", p[i]);
        }
    }
    printf("<%ld.%06ld>\n", secs, usecs);

    return;
}

static void usage(const char *prog)
{
    fprintf(stderr, "usage: %s cmd n e1 e2 ... en\n", prog);
    fprintf(stderr, "  cmd: [exh|our|both]\n");
    fprintf(stderr, "  n: number of runnables. n should be >= 4\n");
    fprintf(stderr, "  e1 e2 ... en: execution times for each runnable\n");
    return;
}

void timediff(struct timeval *start, struct timeval *end, long *secs, long *usecs)
{
    *secs = (end->tv_sec - start->tv_sec); //avoid overflow by subtracting first
    *usecs = (end->tv_usec - start->tv_usec);
    if (*usecs < 0) {
        *secs -= 1;
        *usecs = 1000000 + *usecs;
    }
    return; 
}


double east(double e[], int n)
{
    switch (n) {
    case 4:
        return e[2] + e[3];
    case 5:
        return e[2] + e[3];
    case 6:
        return e[2] + e[5];
    default:
        return 0;
    }
}

double jmax(int n)
{
    return (2 * ALPHA * P_MAX + 2 * BETA * n * P_MAX);
}

double jconv(int p[], int n)
{
    switch (n) {
    case 4:
        return (2 * ALPHA * p[4] + 2 * BETA * (p[1] + p[2] + p[3] + p[4]));
    case 5:
        return (2 * ALPHA * p[5] + 2 * BETA * (p[1] + p[2] + p[3] + p[5]));
    case 6:
        return (2 * ALPHA * p[6] + 2 * BETA * (p[1] + p[2] + p[5] + p[6]));
    default:
        return 0;
    }
}

double jconvf(double p[], int n)
{
    switch (n) {
    case 4:
        return (2 * ALPHA * p[4] + 2 * BETA * (p[1] + p[2] + p[3] + p[4]));
    case 5:
        return (2 * ALPHA * p[5] + 2 * BETA * (p[1] + p[2] + p[3] + p[5]));
    case 6:
        return (2 * ALPHA * p[6] + 2 * BETA * (p[1] + p[2] + p[5] + p[6]));
    default:
        return 0;
    }
}

double utilf(double p[], double e[], int n)
{
    int i;
    double u = 0.0;

    /* loop unrolling for performance optimization */
    switch (n) {
    case 4:
        return (e[1]/p[1] + e[2]/p[2] + e[3]/p[3] + e[4]/p[4]);
    case 5:
        return (e[1]/p[1] + e[2]/p[2] + e[3]/p[3] + e[4]/p[4] + e[5]/p[5]);
    case 6:
        return (e[1]/p[1] + e[2]/p[2] + e[3]/p[3] + e[4]/p[4] + e[5]/p[5] + e[6]/p[6]);
    case 7:
        return (e[1]/p[1] + e[2]/p[2] + e[3]/p[3] + e[4]/p[4] + e[5]/p[5] + e[6]/p[6] + e[7]/p[7]);
    case 8:
        return (e[1]/p[1] + e[2]/p[2] + e[3]/p[3] + e[4]/p[4] + e[5]/p[5] + e[6]/p[6] + e[7]/p[7] + e[8]/p[8]);
    default:
        for (i = 1; i <= n; i++) {
            u += (e[i] / p[i]);
        }
        return u;
    }
    /* unreachable */
}

double util(int p[], double e[], int n)
{
    int i;
    double u = 0.0;

    /* loop unrolling for performance optimization */
    switch (n) {
    case 4:
        return (e[1]/p[1] + e[2]/p[2] + e[3]/p[3] + e[4]/p[4]);
    case 5:
        return (e[1]/p[1] + e[2]/p[2] + e[3]/p[3] + e[4]/p[4] + e[5]/p[5]);
    case 6:
        return (e[1]/p[1] + e[2]/p[2] + e[3]/p[3] + e[4]/p[4] + e[5]/p[5] + e[6]/p[6]);
    case 7:
        return (e[1]/p[1] + e[2]/p[2] + e[3]/p[3] + e[4]/p[4] + e[5]/p[5] + e[6]/p[6] + e[7]/p[7]);
    case 8:
        return (e[1]/p[1] + e[2]/p[2] + e[3]/p[3] + e[4]/p[4] + e[5]/p[5] + e[6]/p[6] + e[7]/p[7] + e[8]/p[8]);
    default:
        for (i = 1; i <= n; i++) {
            u += (e[i] / p[i]);
        }
        return u;
    }
    /* unreachable */
}
