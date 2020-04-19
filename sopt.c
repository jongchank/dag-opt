#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define BNB         1

#define UB          (1.0)
#define N           (5)     /* Number of runnables */
#define ALPHA       (0.001) /* ALPHA in J */
#define BETA        (0.001) /* BETA  in J */
#define J(t, delta) (ALPHA * (t) + BETA * (delta))  /* Control cost function */

static void print_p(int p[], int dim);
static double util(double e[], int p[], int n);
static double jconv(int p[], int n); 

int main(void)
{
    int i;
    int *mins, *maxes;
    int p[N], opt_p[N];
    double *e;
    double j, u, j_min, j_min_u;

    mins  = (int [N])    {1,   1,   1,   1,   1};
    maxes = (int [N])    {100, 100, 100, 100, 100};
    e     = (double [N]) {12.0, 8.0, 11.0, 6.0, 10.0};

    for (i = 0; i < N; i++) {
        p[i] = mins[i];
    }

    j_min = jconv(maxes, N);   /* maximum possible j value */

    for (;;) {
        int end_flag = 0;

        u = util(e, p, N);
        j = jconv(p, N);
        if (u <= UB && j < j_min) {
            j_min = j;
            j_min_u = u;
            for (i = 0; i < N; i++) {
                opt_p[i] = p[i];
            }
            print_p(opt_p, N); printf("%g %g\n", j_min, j_min_u);
        }
        for (i = N - 1; i >= -1; i--) {
            if (i == -1) {
                end_flag = 1;
                break;
            }
            p[i]++;
            if (p[i] > maxes[i]) {
                p[i] = mins[i];
            }
            else {
                break;
            }
        }
        if (end_flag) {
            break;
        }
    }

    printf("--------------------\n");
    print_p(opt_p, N); printf("%g %g\n", j_min, j_min_u);

    return 0;
}

void print_p(int p[], int dim)
{
    int i;
    for (i = 0; i < dim; i++) {
        printf("%d ", p[i]);
    }
}

static double util(double e[], int p[], int n)
{
    int i;
    double u = 0.0;

    for (i = 0; i < n; i++) {
        u += (e[i] / p[i]);
    }

    return u;
}

static double jconv(int p[], int n) 
{
    return (2 * ALPHA * p[4] + 2 * BETA * (p[0] + p[1] + p[2] + p[4]));
}
