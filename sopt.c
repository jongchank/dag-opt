#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define N           4       /* Number of runnables */
#define ALPHA       (0.001) /* ALPHA in J */
#define BETA        (0.001) /* BETA  in J */
#define J(t, delta) (ALPHA * (t) + BETA * (delta))  /* Control cost function */

static void print_p(int p[], int dim);
static int compare(int a[], int b[], int n);
static double util(double e[], int p[], int n);
static double jconv(int p[], int n); 

int main(void)
{
    int i;
    int *begins, *ends;
    int p[N], opt_p[N];
    double *e;
    double j, u, min_j;

    begins   = (int [N])    {1,    1,    1,    1};
    ends     = (int [N])    {100,  100,  100,  100};
    e        = (double [N]) {4.0,  9.0,  12.0, 5.0};

    for (i = 0; i < N; i++) {
        p[i] = begins[i];
    }

    min_j = jconv(ends, N);   /* maximum possible j value */

    for (;;) {
        u = util(e, p, N);
        j = jconv(p, N);

        /*
         * check skip hack 
         */
        if (p[0] != 1 && p[1] == 1 && p[2] == 1 && p[3] == 1 && j > min_j)
            break;

        if (p[0] != 1 && p[1] != 1 && p[2] == 1 && p[3] == 1 && j > min_j)
            p[0]++; p[1] = 1;

        if (u <= 1.0 && j < min_j) {
            min_j = j;
            for (i = 0; i < N; i++) {
                opt_p[i] = p[i];
            }
        }

        if (compare(p, ends, N) >= 0) {
            break;
        }
        for (i = N - 1; i >= 0; i--) {
            p[i]++; 
            if (p[i] > ends[i]) {
                p[i] = begins[i];
            }
            else {
                break;
            }
        }
    }

    printf("--------------------\n");
    print_p(opt_p, N); printf(" U = %g J = %g\n", util(e, opt_p, N), jconv(opt_p, N));

    return 0;
}

int compare(int a[], int b[], int n)
{
    int i;

    for (i = 0; i < n; i++) {
        if (a[i] < b[i]) {
            return -1;
        }
        else if (a[i] > b[i]) {
            return 1;
        }
        else {
            ;
        }
    }
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
    int i;
    double t, delta = 0.0;

    t = 2 * p[n - 1];
    for (i = 0; i < n; i++) {
        delta += 2 * p[i];
    }

    return J(t, delta);
}
