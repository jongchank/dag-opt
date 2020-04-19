#ifndef _COMMON_H_
#define _COMMON_H_

#define BNB   1
#define UB    1.0
#define ALPHA 0.01
#define BETA  0.01
#define P_STRIDE 10
#define P_MIN    P_STRIDE
#define P_MAX    1000

double jconv4(double p1, double p2, double p3, double p4);
double jconv5(double p1, double p2, double p3, double p4, double p5);
double jconv6(double p1, double p2, double p3, double p4, double p5, double p6);
double util4(double e1, double p1, double e2, double p2, double e3, double p3, double e4, double p4);
double util5(double e1, double p1, double e2, double p2, double e3, double p3, double e4, double p4, double e5, double p5);
double util6(double e1, double p1, double e2, double p2, double e3, double p3, double e4, double p4, double e5, double p5, double e6, double p6);

#endif
