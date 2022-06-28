/**
 * @file rdtsc2.c
 *
 * @brief 出自仓库：https://github.com/IAIK/flush_flush.git
 * 中的cacheutils.h
 * 该文件中的rdtsc_begin和rdtsc_end函数
 * 与`./rdtsc.c`中的rdtsc_64很像
 *
 * @version 0.1
 * @date 2022-06-17
 * @author pony (mspony@163.com)
 * @require
 */
#include <stdint.h>

uint64_t rdtsc_nofence()
{
	uint64_t a, d;
	asm volatile("rdtsc"
				 : "=a"(a), "=d"(d));
	a = (d << 32) | a;
	return a;
}

uint64_t rdtsc()
{
	uint64_t a, d;
	asm volatile("mfence");
	asm volatile("rdtsc"
				 : "=a"(a), "=d"(d));
	a = (d << 32) | a;
	asm volatile("mfence");
	return a;
}

uint64_t rdtsc_begin()
{
	uint64_t a, d;
	asm volatile("mfence\n\t"
				 "CPUID\n\t"
				 "RDTSCP\n\t"
				 "mov %%rdx, %0\n\t"
				 "mov %%rax, %1\n\t"
				 "mfence\n\t"
				 : "=r"(d), "=r"(a)
				 :
				 : "%rax", "%rbx", "%rcx", "%rdx");
	a = (d << 32) | a;
	return a;
}

uint64_t rdtsc_end()
{
	uint64_t a, d;
	asm volatile("mfence\n\t"
				 "RDTSCP\n\t"
				 "mov %%rdx, %0\n\t"
				 "mov %%rax, %1\n\t"
				 "CPUID\n\t"
				 "mfence\n\t"
				 : "=r"(d), "=r"(a)
				 :
				 : "%rax", "%rbx", "%rcx", "%rdx");
	a = (d << 32) | a;
	return a;
}