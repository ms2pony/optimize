#include <stdint.h>
#include <stdio.h>
#include <x86intrin.h>

void process()
{
	int sum = 0;
	for (int i = 0; i < 1000; i++)
	{
		sum += 1;
	}
}

void rdtsc_64(void (*process)())
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

	printf("测量时间为：\n%u\n", time);
}

void rdtsc_long_test()
{
	rdtsc_64(process);
}

int main()
{
	rdtsc_long_test();
	rdtsc_long_test();
	rdtsc_long_test();
}