#include <stdint.h>
#include <stdio.h>
#include <x86intrin.h>

uint32_t rdtsc_32(void (*process)())
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

	return time;
}

uint64_t rdtsc_64(void (*process)())
{
	uint64_t t1 = 0;
	uint64_t t2 = 0;
	uint64_t t3 = 0;
	uint64_t t4 = 0;

	uint64_t time = 0;

	// 64位时间测量
	asm volatile(
		"cpuid\n"
		"rdtsc\n"
		"mfence\n"			 //不用cpuid是因为延迟高，可能误差更大；顺带一提还会改变其他寄存器
		: "=a"(t1), "=d"(t2) //一定要存，因为接下来肯定要用到eax;两次内存写操作，以后可以优化
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
		: "=a"(t3), "=d"(t4));
	// 对数据进行处理
	t2 = (t2 << 32) | t1;
	t4 = (t4 << 32) | t3;
	time = t4 - t2;

	return time;
}