#include<stdio.h>
#include<stdint.h>
#include<x86intrin.h>

int t1(long *xp, long *yp);

int t2(long *xp, long *yp);


uint64_t rdtsc1() {
    uint64_t a, d;
    asm volatile ("mfence");
    asm volatile ("rdtsc" : "=a" (a), "=d" (d));
    a = (d<<32) | a;
    asm volatile ("mfence");
    return a;
    }


int main(){
    // int i,j=0;
    // unsigned long t1_1,t1_2;
    // unsigned long t2_1,t2_2;
    unsigned long time1,time2;
    long x1=1;
    long x2=2;

    time1=t1(&x1,&x2);
    time2=t2(&x1,&x2);
    
    printf("time1:%lu\n",time1);
    printf("time2:%lu\n",time2);
}
