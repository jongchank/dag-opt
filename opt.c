#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>

/* support macros for finding maximum values */
#define MAX2(a, b)          ((a) > (b) ? (a) : (b))
#define MAX3(a, b, c)       (MAX2(MAX2((a), (b)), (c)))
#define MAX4(a, b, c, d)    (MAX2(MAX2(MAX2((a), (b)), (c)), (d)))
#define MAX5(a, b, c, d, e) (MAX2(MAX2(MAX2(MAX2((a), (b)), (c)), (d)), (e)))

/* max number of runnables */
#define N_MAX    10

/* default options */
#define TIMING   0      /* whether to show timing information */
#define UB       1.0    /* utilization bound */
#define ALPHA    0.01   /* alpha constant for control cost function */
#define BETA     0.01   /* beta constant for control cost function */
#define P_MIN    10.0   /* minimum period for exhaustive search */
#define P_MAX    1000.0 /* maximum period for exhaustive search */
#define P_STEP   10.0   /* step for exhaustive search */

struct options {
    int timing;
    double ub;
    double alpha;
    double beta;
    double p_min;
    double p_max;
    double p_step;
};

/* 
 * EXH: exhaustive method
 * OUR: our method
 */
enum cmd {EXH, OUR};

/* DAG type */
enum dag_type {A, B, C, D, E, F};

/* function prototypes */
static int parse_options(int argc, char *argv[], int *cmd, int *dag_type, double e[], struct options *o);
static int run_ours(int dag_type, double e[], struct options *o);
static int run_exhaustive(int dag_type, double e[], struct options *o);
static int get_n(int dag_type);
static double east(int dag_type, double e[]);
static double jconv(int dag_type, double p[], double e[]);
static double util(double p[], double e[], int n);
static void show_result(const char *method, double j, double u, double p[], int n, long secs, long usecs, int timing);
static double jmax(int n);
static void timediff(struct timeval *start, struct timeval *end, long *secs, long *usecs);
static void usage(const char *prog);

int main(int argc, char *argv[])
{
    double e[N_MAX + 1];
    int cmd, dag_type;
    int i, rc;
    struct options o;

    o.timing = 0;
    o.ub = UB;
    o.alpha = ALPHA;
    o.beta = BETA;
    o.p_min = P_MIN;
    o.p_max = P_MAX;
    o.p_step = P_STEP;

    rc = parse_options(argc, argv, &cmd, &dag_type, e, &o);
    if (rc < 0) {
        return EXIT_FAILURE;
    }
    if (cmd == EXH) {
        rc = run_exhaustive(dag_type, e, &o);
    }
    else if (cmd == OUR) {
        rc = run_ours(dag_type, e, &o);
    }
    else {
        fprintf(stderr, "Invalid command number: %d\n", cmd);
    }
    if (rc < 0) {
        return EXIT_FAILURE;
    }
    else {
        return EXIT_SUCCESS;
    }
    /* unreachable */
}

static int parse_options(int argc, char *argv[], int *cmd, int *dag_type, double e[], struct options *o)
{
    int i, n, opt;
	while ((opt = getopt(argc, argv, "tm:M:s:a:b:u:")) != -1) {
        switch (opt) {
            case 't':
                o->timing = 1;
                break;
            case 'm':
                o->p_min = atol(optarg);
                break;
            case 'M':
                o->p_max = atol(optarg);
                break;
            case 's':
                o->p_step = atol(optarg);
                break;
            case 'a':
                o->alpha = atol(optarg);
                break;
            case 'b':
                o->beta = atol(optarg);
                break;
            case 'u':
                o->ub = atol(optarg);
                break;
            default:
                usage(argv[0]);
                return -1;
        }
    }
    if (argc < optind + 2) {
        fprintf(stderr, "Failed to parse the command line option\n");
        usage(argv[0]);
        return -1;
    }
    if (strcmp(argv[optind], "exh") == 0 || strcmp(argv[optind], "EXH") == 0) {
        *cmd = EXH;
    }
    else if (strcmp(argv[optind], "our") == 0 || strcmp(argv[optind], "OUR") == 0) {
        *cmd = OUR;
    }
    else {
        fprintf(stderr, "Invalid command: %s\n", argv[optind]);
        usage(argv[0]);
        return -1;
    }
    optind++;
    if (strcmp(argv[optind], "a") == 0 || strcmp(argv[optind], "A") == 0) {
        *dag_type = A;
    }
    else if (strcmp(argv[optind], "b") == 0 || strcmp(argv[optind], "B") == 0) {
        *dag_type = B;
    }
    else if (strcmp(argv[optind], "c") == 0 || strcmp(argv[optind], "C") == 0) {
        *dag_type = C;
    }
    else if (strcmp(argv[optind], "d") == 0 || strcmp(argv[optind], "D") == 0) {
        *dag_type = D;
    }
    else {
        fprintf(stderr, "Invalid DAG type: %s\n", argv[optind]);
        usage(argv[0]);
        return -1;

    }
    optind++;
    n = get_n(*dag_type);
    if (n < 0) {
        fprintf(stderr, "Cannot identify n from the DAG type %s\n", argv[optind - 1]);
        usage(argv[0]);
        return -1;
    }
    if (n > argc - optind) {
        fprintf(stderr, "Requires %d execution times instead of %d\n", n, argc - optind);
        usage(argv[0]);
        return -1;
    }
    else if (n < argc - optind) {
        fprintf(stderr, "Requires %d execution times instead of %d\n", n, argc - optind);
        usage(argv[0]);
        return -1;
    }
    for (i = 1; i <= n; i++) {
        e[i] = atof(argv[optind++]);
    }
    return 0;
}

static int run_ours(int dag_type, double e[], struct options *o)
{
    struct timeval start, end;
    long secs, usecs;
    int i, n;
    double e_ast;
    double p[N_MAX + 1];
    double p_ast;

    double alpha = o->alpha;
    double beta = o->beta;

    gettimeofday(&start, NULL);
    n = get_n(dag_type);
    e_ast = east(dag_type, e);

    p[1] = e[1] + sqrt((n -2) * e[1] * e_ast) + sqrt((alpha + beta) * e[1] * e[n] / beta); 
    p_ast = p[1] * sqrt((n - 2) * e_ast / e[1]);
    for (i = 2; i <= n - 1; i++) {
        p[i] = p_ast * (e[i] / e_ast);
    }
    p[n] = p[1] * sqrt(beta * e[n] / ((alpha + beta) * e[1]));
    gettimeofday(&end, NULL);
    timediff(&start, &end, &secs, &usecs);

    show_result("OURS", jconv(dag_type, p, e), util(p, e, n), p, n, secs, usecs, o->timing);
    
    return 0;
}

static int run_exhaustive(int dag_type, double e[], struct options *o)
{
    struct timeval start, end;
    long secs, usecs;
    int i, n;
    double p[N_MAX + 1];
    double opt_p[N_MAX + 1];
    double min_j, min_j_u;
    double u, opt_u;
    double j;

    double ub = o->ub;
    double alpha = o->alpha;
    double beta = o->beta;
    double p_min = o->p_min;
    double p_max = o->p_max;
    double p_step = o->p_step;

    gettimeofday(&start, NULL);
    n = get_n(dag_type);
    min_j = jmax(n);
    for (i = 1; i <= n; i++) {
        p[i] = p_min;
    }
    for (;;) {
        int end_flag = 0;
        /* loop body begins here */
        u = util(p, e, n);
        if (u > ub) {
            goto loop;
        }
        j = jconv(dag_type, p, e);
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
            p[i] += p_step;
            if (p[i] > p_max) {
                p[i] = p_min;
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
    show_result("EXHAUSTIVE", min_j, min_j_u, opt_p, n, secs, usecs, o->timing); 

    return 0;
}

static int get_n(int dag_type)
{
    switch (dag_type) {
    case A:
        return 4;
    case B:
        return 5;
    case C:
        return 5;
    case D:
        return 6;
    default:
        return -1;
    }
}

static double east(int dag_type, double e[])
{
    switch (dag_type) {
    case A:
        return e[2] + e[3];
    case B:
        return MAX2(e[2] + e[3], e[2] + e[4]);
    case C:
        return MAX2(e[4], e[2] + e[3]);
    case D:
        return MAX3(e[2] + e[3], e[2] + e[5], e[4] + e[5]);
    default:
        return 0;
    }
}

static double jconv(int dag_type, double p[], double e[])
{
    switch (dag_type) {
    case A:
        return (2 * ALPHA * p[4] + 2 * BETA * (p[1] + p[2] + p[3] + p[4]));
    case B:
        return MAX2(2 * ALPHA * p[5] + 2 * BETA * (p[1] + p[2] + p[3] + p[5]),
                    2 * ALPHA * p[5] + 2 * BETA * (p[1] + p[2] + p[4] + p[5]));
    case C:
        return MAX2(2 * ALPHA * p[5] + 2 * BETA * (p[1] + p[4] + p[5]),
                    2 * ALPHA * p[5] + 2 * BETA * (p[1] + + p[2] + p[3] + p[5]));
    case D:
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

static void show_result(const char *method, double j, double u, double p[], int n, long secs, long usecs, int timing) 
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
    if (timing) {
        printf(" <%ld.%06ld>\n", secs, usecs);
    }
    else {
        printf("\n");
    }
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
