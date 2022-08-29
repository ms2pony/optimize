/**
 * @file prefetch.c
 *
 * @brief 1.验证clflush和prefetch之间是否要加序列化指令才能保证他们的执行顺序
 * note: 相关数据的范围为全局(global)
 *
 * @version 0.1
 * @date 2022-06-16
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

/**
 * @brief 利用prefetch将数据提前放入cache
 * 然后测量执行时间
 * 这里保证了prefetch和clflush指令
 * 不会因为处理器的乱序执行策略而发生交替执行
 *
 * @require
 */
void in_cache_order_test()
{

    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t time = 0;

    _mm_clflush(&m1);
    _mm_clflush(&m2);
    _mm_clflush(&m3);
    _mm_clflush(&m4);

    asm volatile("cpuid"); // cpuid指令用于序列化
    // asm volatile("mfence"); //与上面一行代码作用一样

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

/**
 * @brief 利用prefetch将数据提前放入cache
 * 然后测量执行时间
 * 但此时若干个prefetch和clflush执行会发生交替执行
 *
 * @require
 */
void in_cache_no_order_test()
{

    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t time = 0;

    _mm_clflush(&m1);
    _mm_clflush(&m2);
    _mm_clflush(&m3);
    _mm_clflush(&m4);

    //无法保证前面4个_mm_clflush一定在后面4个_mm_prefetch

    _mm_prefetch(&m1, _MM_HINT_T0);
    _mm_prefetch(&m2, _MM_HINT_T0);
    _mm_prefetch(&m3, _MM_HINT_T0);
    _mm_prefetch(&m4, _MM_HINT_T0);

    //此时不能确定m1,m2,m3,m4是否在cache中

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

    printf("测量时间为：\n%lu\n", time);
}

int main()
{
    in_cache_order_test();    //使用序列化时，执行时间为80cycles左右
    in_cache_no_order_test(); //不使用序列化时，执行时间为300cycles左右
}
