/**
 * @file rdtsc_short_test.c
 *
 * @brief 探究对rdtsc_short_test.c中的rdtsc_short_test2函数
 * 进行适当修改后能否让执行时间固定(程序先后执行的执行时间)
 *
 * @version 0.1
 * @date 2022-06-16
 * @author pony (mspony@163.com)
 * @require
 */

#include <stdint.h>
#include <stdio.h>
#include <x86intrin.h>

uint32_t __attribute__((aligned(64))) m1 = 1;
uint32_t __attribute__((aligned(64))) m2 = 1;
uint16_t __attribute__((aligned(64))) m3 = 1;
uint16_t __attribute__((aligned(64))) m4 = 1;

void process3()
{

	asm volatile(
		// "mov $88, %%r8\n"
		"imul $6, %%eax\n"
		: "=a"(m2)
		: "a"(m1));

	asm volatile(
		"add $6, %%eax\n"
		: "=a"(m4)
		: "a"(m3));
}

void rdtsc_32(void (*process)())
{
	// int __attribute__((aligned(64))) a = 0; // main函数栈上的变量，64byte对齐

	uint64_t t1 = 0;
	uint64_t t2 = 0;

	uint64_t time = 0;

	// 32位时间测量
	asm volatile(
		"cpuid\n"
		"rdtsc\n"
		"mfence\n" //不用cpuid是因为延迟高，可能误差更大；顺带一提还会改变其他寄存器
		: "=a"(t1) //一定要存，因为接下来肯定要用到eax
	);
	/*开始处*/

	process();

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

	/*取消一个变量的优化*/
	// asm volatile("": :"r,m"(a): "memory");

	printf("测量时间为：\n%u\n", time);
}

void rdtsc_short_test2()
{
	//使得m1-m4都在cache中
	{
		_mm_prefetch(&m1, _MM_HINT_T0);
		_mm_prefetch(&m2, _MM_HINT_T0);
		_mm_prefetch(&m3, _MM_HINT_T0);
		_mm_prefetch(&m4, _MM_HINT_T0);
	}

	rdtsc_32(process3);
	rdtsc_32(process3);
	rdtsc_32(process3);
}

// prefetch和clflush指令是否可以消除1的影响
void rdtsc_short_test2_2()
{
	//使得m1-m4都在cache中
	{
		_mm_prefetch(&m1, _MM_HINT_T0);
		_mm_prefetch(&m2, _MM_HINT_T0);
		_mm_prefetch(&m3, _MM_HINT_T0);
		_mm_prefetch(&m4, _MM_HINT_T0);
	}

	rdtsc_32(process3);

	{
		_mm_prefetch(&m1, _MM_HINT_T0);
		_mm_prefetch(&m2, _MM_HINT_T0);
		_mm_prefetch(&m3, _MM_HINT_T0);
		_mm_prefetch(&m4, _MM_HINT_T0);
	}

	rdtsc_32(process3);

	{
		_mm_prefetch(&m1, _MM_HINT_T0);
		_mm_prefetch(&m2, _MM_HINT_T0);
		_mm_prefetch(&m3, _MM_HINT_T0);
		_mm_prefetch(&m4, _MM_HINT_T0);
	}

	rdtsc_32(process3);
}

void rdtsc_short_test2_3()
{
	//使得m1-m4都在cache中
	{
		_mm_clflush(&m1);
		_mm_clflush(&m2);
		_mm_clflush(&m3);
		_mm_clflush(&m4);

		asm volatile("cpuid"); // cpuid指令用于序列化
		_mm_prefetch(&m1, _MM_HINT_T0);
		_mm_prefetch(&m2, _MM_HINT_T0);
		_mm_prefetch(&m3, _MM_HINT_T0);
		_mm_prefetch(&m4, _MM_HINT_T0);
	}

	rdtsc_32(process3);
	rdtsc_32(process3);
	rdtsc_32(process3);
}

void rdtsc_short_test2_4()
{
	//使得m1-m4都在cache中
	{
		_mm_clflush(&m1);
		_mm_clflush(&m2);
		_mm_clflush(&m3);
		_mm_clflush(&m4);
	}

	rdtsc_32(process3);

	{
		_mm_clflush(&m1);
		_mm_clflush(&m2);
		_mm_clflush(&m3);
		_mm_clflush(&m4);
	}
	rdtsc_32(process3);

	{
		_mm_clflush(&m1);
		_mm_clflush(&m2);
		_mm_clflush(&m3);
		_mm_clflush(&m4);
	}
	rdtsc_32(process3);
}

int main()
{
	rdtsc_short_test2();
}
