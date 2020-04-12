#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

int main(void)
{
    unsigned long n = 4;
    double e_ast;
    double e1 = 10, e2 = 6, e3 = 9,  e4 = 4;  // 1 
//    double e1 = 5,  e2 = 7, e3 = 10, e4 = 9;  // 2
//    double e1 = 8,  e2 = 3, e3 = 5,  e4 = 10; // 3
//    double e1 = 5,  e2 = 7, e3 = 6,  e4 = 9;  // 4
//    double e1 = 4,  e2 = 5, e3 = 10, e4 = 8;  // 5
//    double e1 = 5,  e2 = 4, e3 = 4,  e4 = 7;  // 6
//    double e1 = 3,  e2 = 7, e3 = 5,  e4 = 4;  // 7
//    double e1 = 9,  e2 = 7, e3 = 6,  e4 = 4;  // 8
//    double e1 = 8,  e2 = 6, e3 = 6,  e4 = 6;  // 9
//    double e1 = 4,  e2 = 7, e3 = 5,  e4 = 5;  // 10
    double bbeta = 0.001, aalpha = 0.001;
    double min_j1 = 100, min_j2 = 100, min_j3 = 100;

    double u, U;
    unsigned long p1, p2, p3, p4;
    unsigned long P1, P2, P3, P4;
    double real_p1, real_past, real_p2, real_p3, real_pn;

    unsigned long exh_p1, exh_p2, exh_p3, exh_p4;
    double exh_util, j1, j2, j3;

    for (p1 = 1; p1 <= 100; p1++) {
        for (p2 = 1; p2 <= 100; p2++) {
            for (p3 = 1; p3 <= 100; p3++) {
                for (p4 = 1; p4 <= 100; p4++) {
                    u = (e1 / p1) + (e2 / p2) + (e3 / p3) + (e4 / p4);
                    j1 = bbeta * (p4) + aalpha * (p1 +p2 + p4);
                    j2 = bbeta * (p4) + aalpha * (p1 +p3 + p4);
                    j3 = bbeta * (p4) + aalpha * (p1 +p2 + p3 + p4);
                
//                    if (0.99 <= u && u <= 1) {
                        if (j1 < min_j1 && j1 > j2 && j1 > j3) {
                            min_j1 = j1;
                            min_j2 = j2;
                            min_j3 = j3;
                            P1 = p1; P2 = p2; P3 = p3; P4 = p4;
                            U = u;
                        } 
                        if (j2 < min_j2 && j2 > j1 && j2 > j3) {
                            min_j1 = j1;
                            min_j2 = j2;
                            min_j3 = j3;
                            P1 = p1; P2 = p2; P3 = p3; P4 = p4;
                            U = u;
                        } 
                        if (j3 < min_j3 && j3 > j1 && j3 > j2) {
                            min_j1 = j1;
                            min_j2 = j2;
                            min_j3 = j3;
                            P1 = p1; P2 = p2; P3 = p3; P4 = p4;
                            U = u;
                       } 
 //                   }
                }
            }
        }
    }

    printf("P1 = %d P2 = %d P3 = %d P4 = %d\n", P1, P2, P3, P4);
    printf("J1 = %g J2 = %g J3 = %g\n", min_j1, min_j2, min_j3);
    printf("U = %g\n", U);
#if 0
    e_ast = (e2 + e3);

    real_p1 = e1 + sqrt((n - 2) * e1 * e_ast) + sqrt(2 * e1 * e4);
    real_past = real_p1 * sqrt((n - 2) * e_ast / e1);
    real_p2 = real_past * e2 / (e2 + e3);
    real_p3 = real_past * e3 / (e2 + e3);
    real_pn = real_p1 * sqrt(e4 / 2 / e1);

    real_j = 0.001*(real_p1+real_past) + 0.002*(real_pn)
    real_util = (e1/real_p1) + ( (e2+e3)/real_past) + (e4/real_pn)
#endif
    return 0;
}

#if 0 

disp('  ')




#endif
