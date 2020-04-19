#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "common.h"

#define N 6

int main(int argc, char *argv[])
{
    int n = N;
    //double e1 = 80,  e2 = 60, e3 = 70,  e4 = 40, e5 = 110, e6 = 80;
    double e1, e2, e3, e4, e5, e6;
    double min_j, min_j_u;

    double u, opt_u;
    int p1, p2, p3, p4, p5, p6;
    int opt_p1, opt_p2, opt_p3, opt_p4, opt_p5, opt_p6;
    double j;

    if (argc != 7) {
        return -1;
    }
    e1 = atof(argv[1]);
    e2 = atof(argv[2]);
    e3 = atof(argv[3]);
    e4 = atof(argv[4]);
    e5 = atof(argv[5]);
    e6 = atof(argv[6]);

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

    printf("%g %g %d %d %d %d %d %d\n", min_j, min_j_u, opt_p1, opt_p2, opt_p3, opt_p4, opt_p5, opt_p6);

    return 0;
}

