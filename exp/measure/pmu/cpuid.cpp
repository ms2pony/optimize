#include<stdint.h>
#include<stdio.h>

#include <sched.h>
int main(){
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);
    // asm volatile(
    //     "movl $0xa, %eax\n"
    //     "cpuid\n"
    // );
    uint32_t time=0;
    asm volatile(
        "cpuid\n"
        "movl	$1073741826, %ecx\n"
        "rdpmc\n"
        "movl %eax, %r8d\n"
        "cpuid\n"
        "movl	$1073741826, %ecx\n"
        "rdpmc\n"
        "movl %eax, %r9d\n"
        "cpuid\n"
    );
    asm volatile(
        "sub %%r8d, %%r9d\n"
        "mov %%r9d, %%eax\n"
        :"=a"(time)
    );
    printf("%u\n",time);
}