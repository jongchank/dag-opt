#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "common.h"

#define N 5

int main(int argc, char *argv[])
{
    int n = N;
    // double e1 = 120,  e2 = 80, e3 = 110,  e4 = 60, e5 = 100;
    double e1, e2, e3, e4, e5;
    double min_j, min_j_u;

    double u, opt_u;
    int p1, p2, p3, p4, p5;
    int opt_p1, opt_p2, opt_p3, opt_p4, opt_p5;
    double j;

    if (argc != 6) {
        return -1;
    }
    e1 = atof(argv[1]);
    e2 = atof(argv[2]);
    e3 = atof(argv[3]);
    e4 = atof(argv[4]);
    e5 = atof(argv[5]);

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

    printf("%g %g %d %d %d %d %d\n", min_j, min_j_u, opt_p1, opt_p2, opt_p3, opt_p4, opt_p5);

    return 0;
}
