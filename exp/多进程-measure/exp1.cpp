// 探究现代操作系统的多进程机制对不同测量工具测出来的测量结果的影响

#include <chrono>
#include <iostream>

using namespace std;

extern "C"
{
#include <openssl/bn.h>
#include <util/bn_avx2/get_bn_avx2.h>
#include <util/bn_convert/bn_convert.h>
    int mul_avx2(__m256i E_F[5], __m256i A_B[5], __m256i C_D[5]);
/* Montgomery mul: res = a*b*2^-256 mod P */
#define P256_LIMBS (256 / BN_BITS2)
    void ecp_nistz256_mul_mont(BN_ULONG res[P256_LIMBS],
                               const BN_ULONG a[P256_LIMBS],
                               const BN_ULONG b[P256_LIMBS]);

    __m256i A_B[5];
    __m256i C_D[5];
    __m256i E_F[5];

    BN_ULONG A[5] = {0};
    BN_ULONG B[5] = {0};
    BN_ULONG C[5] = {0};
    BN_ULONG D[5] = {0};
    BN_ULONG E[5] = {0};
    BN_ULONG F[5] = {0};

    uint64_t rdtsc_64(void (*process)(int), int a)
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

        process(a);

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
}

double time_sys(void (*process)(int), int a)
{
    double sum = 0;
    auto begin = chrono::high_resolution_clock::now();

    process(a);
    auto end = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    return elapsed.count() * 1e-9;
}

void base1(int iterations)
{
    for (int i = 0; i < iterations; i++)
    {
#ifdef MULAVX2
        mul_avx2(E_F, A_B, C_D);
#else
        ecp_nistz256_mul_mont(E, A, C);
        ecp_nistz256_mul_mont(E, A, C);
#endif
    }
}

void bench_mul_mod()
{
    __m256i A_B[5];
    __m256i C_D[5];
    __m256i E_F[5];

    BN_ULONG A[5] = {0};
    BN_ULONG B[5] = {0};
    BN_ULONG C[5] = {0};
    BN_ULONG D[5] = {0};
    BN_ULONG E[5] = {0};
    BN_ULONG F[5] = {0};

    unsigned int seed = (unsigned int)time(NULL);

    // Start measuring time

    int iterations = 10000 * 10000;

    auto sum = rdtsc_64(base1, iterations);

    // cout << "Time measured: " << sum / (iterations * iterations) << " cycles." << endl;
    cout << "Time measured(rdtsc): " << endl
         << sum / 2.6e+9 << " seconds." << endl;
}

void bench_mul_mod2()
{
    __m256i A_B[5];
    __m256i C_D[5];
    __m256i E_F[5];

    BN_ULONG A[5] = {0};
    BN_ULONG B[5] = {0};
    BN_ULONG C[5] = {0};
    BN_ULONG D[5] = {0};
    BN_ULONG E[5] = {0};
    BN_ULONG F[5] = {0};

    unsigned int seed = (unsigned int)time(NULL);

    // Start measuring time

    int iterations = 10000 * 10000;

    auto sum = time_sys(base1, iterations);

    // cout << "Time measured: " << sum / (iterations * iterations) * 2.6e+9 << " cycles." << endl;
    cout << "Time measured(cpp:high_resolution_clock): " << endl
         << sum << " seconds." << endl;
}

int main()
{
    bench_mul_mod();
    bench_mul_mod2();

    return 0;
}