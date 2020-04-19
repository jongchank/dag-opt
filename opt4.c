#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "common.h"

#define N 4

int main(int argc, char *argv[])
{
    int n = N;
    //double e1 = 40,  e2 = 90, e3 = 120,  e4 = 50;
    double e1,  e2, e3,  e4;
    double min_j, min_j_u;

    double u, opt_u;
    int p1, p2, p3, p4;
    int opt_p1, opt_p2, opt_p3, opt_p4;
    double j;

    if (argc != 5) {
        return -1;
    }
    e1 = atof(argv[1]);
    e2 = atof(argv[2]);
    e3 = atof(argv[3]);
    e4 = atof(argv[4]);

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

    printf("%g %g %d %d %d %d\n", min_j, min_j_u, opt_p1, opt_p2, opt_p3, opt_p4);

    return 0;
}
