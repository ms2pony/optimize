/**
 * @file rdtsc_short_test.c
 *
 * @brief 探究使用rdtsc测量较短执行时间的程序时出现的一些问题
 * 对象：执行时间较短的程序
 * 1. 同一个程序先后顺序对执行时间的影响
 * 2. prefetch和clflush指令是否可以消除1的影响
 * 3. 多次测量求平均值是否可以消除1的影响
 *
 * @version 0.1
 * @date 2022-06-16
 * @author pony (mspony@163.com)
 * @require
 */

#include <stdint.h>
#include <stdio.h>
#include <x86intrin.h>

void process()
{
    uint32_t __attribute__((aligned(64))) m1 = 1;
    uint32_t __attribute__((aligned(64))) m2 = 1;
    uint16_t __attribute__((aligned(64))) m3 = 1;
    uint16_t __attribute__((aligned(64))) m4 = 1;

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

//与process一样
void process2()
{
    uint32_t __attribute__((aligned(64))) m1 = 1;
    uint32_t __attribute__((aligned(64))) m2 = 1;
    uint16_t __attribute__((aligned(64))) m3 = 1;
    uint16_t __attribute__((aligned(64))) m4 = 1;

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

void process3()
{

    uint32_t __attribute__((aligned(64))) m1 = 1;
    uint32_t __attribute__((aligned(64))) m2 = 1;
    uint16_t __attribute__((aligned(64))) m3 = 1;
    uint16_t __attribute__((aligned(64))) m4 = 1;

    //使得m1-m4都在cache中
    {
        _mm_prefetch(&m1, _MM_HINT_T0);
        _mm_prefetch(&m2, _MM_HINT_T0);
        _mm_prefetch(&m3, _MM_HINT_T0);
        _mm_prefetch(&m4, _MM_HINT_T0);
    }

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

    printf("测量时间为：\n%u\n", time);
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
        "mfence\n"           //不用cpuid是因为延迟高，可能误差更大；顺带一提还会改变其他寄存器
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

uint64_t rdtsc_64_2(void (*process)())
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
        "mfence\n"           //不用cpuid是因为延迟高，可能误差更大；顺带一提还会改变其他寄存器
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

/* 同一个程序先后顺序对执行时间的影响
note:通过注释不同的block来进行探究 */
void rdtsc_short_test1()
{
    printf("rdtsc_short_test1开始\n");
    // case1，相同程序，先执行的执行时间较长
    /* {
        rdtsc_32(process);
        rdtsc_32(process);
    } */

    // case2，相同代码不同程序(地址不同)
    // 探究是否和case1的结果一样
    /* {
        rdtsc_32(process);
        rdtsc_32(process2);
    } */
    {
        rdtsc_32(process2);
        rdtsc_32(process);
    }

    printf("rdtsc_short_test1结束\n");
}

// prefetch和clflush指令是否可以消除1的影响
void rdtsc_short_test2()
{
    rdtsc_32(process3);
    rdtsc_32(process3);
    rdtsc_32(process3);
}

// 多次测量是否可以消除1的影响
void rdtsc_short_test3()
{
    uint64_t average;
    uint64_t sum = 0;

    for (int i = 0; i < 100; i++)
    {

        sum += rdtsc_64_2(process);
    }

    printf("平均执行时间为：%lld\n", sum / 100);

    sum = 0;

    for (int i = 0; i < 100; i++)
    {
        sum += rdtsc_64_2(process);
    }

    printf("平均执行时间为：%lld\n", sum / 100);
}

int main()
{
    // rdtsc_short_test1();
    rdtsc_short_test2();
    // rdtsc_short_test3();
}
