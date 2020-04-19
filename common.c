#include "common.h"

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

double jconv4(double p1, double p2, double p3, double p4)
{
    return (2 * ALPHA * p4 + 2 * BETA * (p1 + p2 + p3 + p4));
}

double jconv5(double p1, double p2, double p3, double p4, double p5)
{
    return (2 * ALPHA * p5 + 2 * BETA * (p1 + p2 + p3 + p5));
}

double jconv6(double p1, double p2, double p3, double p4, double p5, double p6)
{
    return (2 * ALPHA * p6 + 2 * BETA * (p1 + p2 + p5 + p6));
}

double util4(double e1, double p1, double e2, double p2, double e3, double p3, double e4, double p4)
{
    return (e1/p1 + e2/p2 + e3/p3 + e4/p4);
}

double util5(double e1, double p1, double e2, double p2, double e3, double p3, double e4, double p4, double e5, double p5)
{
    return (e1/p1 + e2/p2 + e3/p3 + e4/p4 + e5/p5);
}

double util6(double e1, double p1, double e2, double p2, double e3, double p3, double e4, double p4, double e5, double p5, double e6, double p6)
{
    return (e1/p1 + e2/p2 + e3/p3 + e4/p4 + e5/p5 + e6/p6);
}
