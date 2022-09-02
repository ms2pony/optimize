#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include<stdio.h>

uint64_t time_now() 
{
	struct timeval tv;
	uint64_t ret;

	gettimeofday(&tv, NULL);
	ret = tv.tv_sec;
	ret *= 1000000;
	ret += tv.tv_usec;

	return ret;
}

int main(){
  int c=2391;
  int A[200],A1[200],B[200],B1[200],C[200],C1[200],D[200],A_1[200];

  uint64_t start, end;

  start=time_now();
  for(int i=0;i<10000;++i){
    for(int i=0;i<100;++i){
      A[i]=A[i]*B[i];
      B[i]=A[i]+c;
      A[i]=C[i]*c;
      C[i]=D[i]*A[i];
    }
  }
  end=time_now();
  printf("%lu us\n", (unsigned long) (end - start));

  start=time_now();
  for(int i=0;i<10000;++i){
    for(int i=0;i<100;++i){
      A[i]=A[i]*B[i];
      B1[i]=A[i]+c;
      A1[i]=C[i]*c;
      C1[i]=D[i]*A1[i];
    }
  }
  end=time_now();
  printf("%lu us\n", (unsigned long) (end - start));

}