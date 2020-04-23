#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>

#define N_MAX    10
#define UB       1.0
#define ALPHA    0.01
#define BETA     0.01
#define P_STRIDE 10.0
#define P_MIN    P_STRIDE
#define P_MAX    1000.0

#define SHOW_TIMING   0

#define MAX2(a, b)          ((a) > (b) ? (a) : (b))
#define MAX3(a, b, c)       (MAX2(MAX2((a), (b)), (c)))
#define MAX4(a, b, c, d)    (MAX2(MAX2(MAX2((a), (b)), (c)), (d)))
#define MAX5(a, b, c, d, e) (MAX2(MAX2(MAX2(MAX2((a), (b)), (c)), (d)), (e)))

static int run_ours(double e[], char t, int n);
static int run_exhaustive(double e[], char t, int n);
static int get_n(char t);
static double east(double e[], char t, int n);
static double jconv(double p[], double e[], char t, int n);
static double util(double p[], double e[], int n);
static void show_result(const char *method, double j, double u, double p[], int n, long secs, long usecs);
static double jmax(int n);
static void timediff(struct timeval *start, struct timeval *end, long *secs, long *usecs);
static void usage(const char *prog);

int main(int argc, char *argv[])
{
    double e[N_MAX + 1];
    char *cmd;
    char t;
    int i, n, rc;

    if (argc < 3) {
        fprintf(stderr, "Algorithm and DAG type are mandatory\n");
        usage(argv[0]);
        return 1;
    }
    cmd = argv[1];
    if (strlen(argv[2]) != 1) {
        fprintf(stderr, "Invalid DAG type: %s\n", argv[1]);
        usage(argv[0]);
        return 1;
    }
    t = *(char*)argv[2];
    n = get_n(t);
    if (n < 0) {
        fprintf(stderr, "Invalid DAG type: %c\n", t);
        usage(argv[0]);
        return 1;
    }
    if (n + 3 != argc) {
        fprintf(stderr, "Not enough arguments\n");
        usage(argv[0]);
        return 1;
    }
    for (i = 1; i <= n; i++) {
        e[i] = atof(argv[i + 2]);
    }

    if (strncmp(cmd, "exh", 3) == 0) {
        rc = run_exhaustive(e, t, n);
    }
    else if (strncmp(cmd, "our", 3) == 0) {
        rc = run_ours(e, t, n);
    }
    else {
        fprintf(stderr, "Invalid command: %s\n", cmd);
    }
    return rc;
}

static int run_ours(double e[], char t, const int n)
{
    struct timeval start, end;
    long secs, usecs;
    double e_ast;
    double p[N_MAX + 1];
    double p_ast;
    int i;

    gettimeofday(&start, NULL);
    e_ast = east(e, t, n);

    p[1] = e[1] + sqrt((n -2) * e[1] * e_ast) + sqrt((ALPHA + BETA) * e[1] * e[n] / BETA); 
    p_ast = p[1] * sqrt((n - 2) * e_ast / e[1]);
    for (i = 2; i <= n - 1; i++) {
        p[i] = p_ast * (e[i] / e_ast);
    }
    p[n] = p[1] * sqrt(BETA * e[n] / ((ALPHA + BETA) * e[1]));
    gettimeofday(&end, NULL);
    timediff(&start, &end, &secs, &usecs);

    show_result("OURS", jconv(p, e, t, n), util(p, e, n), p, n, secs, usecs);
    
    return 0;
}

static int run_exhaustive(double e[], char t, const int n)
{
    struct timeval start, end;
    long secs, usecs;
    int i;
    double p[N_MAX + 1];
    double opt_p[N_MAX + 1];
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
        j = jconv(p, e, t, n);
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

static int get_n(char t)
{
    switch (t) {
    case 'a':
        return 4;
    case 'b':
        return 5;
    case 'c':
        return 5;
    case 'd':
        return 6;
    default:
        return -1;
    }
}

static double east(double e[], char t, int n)
{
    switch (t) {
    case 'a':
        return e[2] + e[3];
    case 'b':
        return MAX2(e[2] + e[3], e[2] + e[4]);
    case 'c':
        return MAX2(e[4], e[2] + e[3]);
    case 'd':
        return MAX3(e[2] + e[3], e[2] + e[5], e[4] + e[5]);
    case 'e':
        return MAX2(e[2], e[3]);
    case 'f':
        return MAX2(e[2] + e[3], e[4]);
    case 'g':
        return MAX3(e[2], e[3], e[4]);
    case 'h':
        return MAX2(e[2] + e[3], e[4] + e[5]);
    default:
        return 0;
    }
}

static double jconv(double p[], double e[], char t, int n)
{
    switch (t) {
    case 'a':
        return (2 * ALPHA * p[4] + 2 * BETA * (p[1] + p[2] + p[3] + p[4]));
    case 'b':
        return MAX2(2 * ALPHA * p[5] + 2 * BETA * (p[1] + p[2] + p[3] + p[5]),
                    2 * ALPHA * p[5] + 2 * BETA * (p[1] + p[2] + p[4] + p[5]));
    case 'c':
        return MAX2(2 * ALPHA * p[5] + 2 * BETA * (p[1] + p[4] + p[5]),
                    2 * ALPHA * p[5] + 2 * BETA * (p[1] + + p[2] + p[3] + p[5]));
    case 'd':
        return MAX3(2 * ALPHA * p[6] + 2 * BETA * (p[1] + p[2] + p[3] + p[6]),
                    2 * ALPHA * p[6] + 2 * BETA * (p[1] + p[2] + p[5] + p[6]),
                    2 * ALPHA * p[6] + 2 * BETA * (p[1] + p[4] + p[5] + p[6]));
    default:
        return 0;
    }
}

static double util(double p[], double e[], int n)
{
    int i;
    double u = 0.0;

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

static void show_result(const char *method, double j, double u, double p[], int n, long secs, long usecs) 
{
    int i;

    printf("%-10s | %-6.2f %-6.2f | (", method, j, u * 100);
    for (i = 1; i <= n; i++) {
        if (i != n) {
            printf("%-3.2f, ", p[i]);
        }
        else {
            printf("%-3.2f)", p[i]);
        }
    }
#if SHOW_TIMING == 1 
    printf(" <%ld.%06ld>\n", secs, usecs);
#else
    printf("\n");
#endif
    return;
}

static double jmax(int n)
{
    return (2 * ALPHA * P_MAX + 2 * BETA * n * P_MAX);
}

static void timediff(struct timeval *start, struct timeval *end, long *secs, long *usecs)
{
    *secs = (end->tv_sec - start->tv_sec); //avoid overflow by subtracting first
    *usecs = (end->tv_usec - start->tv_usec);
    if (*usecs < 0) {
        *secs -= 1;
        *usecs = 1000000 + *usecs;
    }
    return; 
}

static void usage(const char *prog)
{
    fprintf(stderr, "usage: %s algorithm dag e1 e2 ... en\n", prog);
    fprintf(stderr, "  algorithm: [exh|our]\n");
    fprintf(stderr, "  dag: DAG type [a|b|c|d]\n");
    fprintf(stderr, "  e1 e2 ... en: execution times for each runnable\n");
    return;
}
