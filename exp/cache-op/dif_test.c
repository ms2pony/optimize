/**
 * @file dif_test.c
 *
 * @brief 探究程序执行前是否在cache中对执行实验的影响
 * 用到的intrinsic 函数，_mm_prefetch和_mm_clflush
 *
 * @version 0.1
 * @date 2022-06-17
 * @author pony (mspony@163.com)
 * @require
 */
#include <stdint.h>
#include <stdio.h>
#include <x86intrin.h>

// 64字节对齐的变量(内存)，内存段， 64字节对齐
uint32_t __attribute__((aligned(64))) m1 = 1;
uint32_t __attribute__((aligned(64))) m2 = 1;
uint16_t __attribute__((aligned(64))) m3 = 1;
uint16_t __attribute__((aligned(64))) m4 = 1;

void in_cache_test()
{

	uint64_t t1 = 0;
	uint64_t t2 = 0;
	uint64_t time = 0;

	_mm_prefetch(&m1, _MM_HINT_T0);
	_mm_prefetch(&m2, _MM_HINT_T0);
	_mm_prefetch(&m3, _MM_HINT_T0);
	_mm_prefetch(&m4, _MM_HINT_T0);

	//此时可以确定m1,m2,m3,m4都在cache中

	// 32位时间测量
	asm volatile(
		"cpuid\n"
		"rdtsc\n"
		"mfence\n" //不用cpuid是因为延迟高，可能误差更大；顺带一提还会改变其他寄存器
		: "=a"(t1) //一定要存，因为接下来肯定要用到eax
	);
	/*开始处*/

	asm volatile(
		// "mov $88, %%r8\n"
		"imul $6, %%eax\n"
		: "=a"(m2)
		: "a"(m1));

	asm volatile(
		"add $6, %%eax\n"
		: "=a"(m4)
		: "a"(m3));

	/*结束处*/
	asm volatile(
		"mfence\n"
		"rdtsc\n"
		"mfence\n" //极致地减小误差
	);

	asm volatile(
		""
		: "=a"(t2));
	time = t2 - t1;

	// printf("%d,%d,%d,%d\n", m1, m2, m3, m4);
	printf("测量时间为：\n%lu\n", time);
}

void out_cache_test()
{

	uint64_t t1 = 0;
	uint64_t t2 = 0;
	uint64_t time = 0;

	_mm_clflush(&m1);
	_mm_clflush(&m2);
	_mm_clflush(&m3);
	_mm_clflush(&m4);

	//此时可以确定m1,m2,m3,m4都在cache中

	// 32位时间测量
	asm volatile(
		"cpuid\n"
		"rdtsc\n"
		"mfence\n" //不用cpuid是因为延迟高，可能误差更大；顺带一提还会改变其他寄存器
		: "=a"(t1) //一定要存，因为接下来肯定要用到eax
	);
	/*开始处*/

	asm volatile(
		// "mov $88, %%r8\n"
		"imul $6, %%eax\n"
		: "=a"(m2)
		: "a"(m1));

	asm volatile(
		"add $6, %%eax\n"
		: "=a"(m4)
		: "a"(m3));

	/*结束处*/
	asm volatile(
		"mfence\n"
		"rdtsc\n"
		"mfence\n" //极致地减小误差
	);

	asm volatile(
		""
		: "=a"(t2));
	time = t2 - t1;

	// printf("%d,%d,%d,%d\n", m1, m2, m3, m4);
	printf("测量时间为：\n%lu\n", time);
}

int main()
{
	// out_cache_test();
	// in_cache_test();

	in_cache_test();
	out_cache_test();
}