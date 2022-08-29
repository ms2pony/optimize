//参考：https://zhuanlan.zhihu.com/p/349949616
// 一些概念：墙上时钟时间 ，用户cpu时间 ，系统cpu时间
#include <stdio.h>
#include <chrono>

//方法1
void method1()
{
	//使用linux命令 time
}

/**
 * @brief 在
 *
 * @return int
 * @require
 */
int method2()
{
	double sum = 0;
	double add = 1;

	// Start measuring time
	auto begin = std::chrono::high_resolution_clock::now();

	int iterations = 1000 * 1000 * 1000;
	for (int i = 0; i < iterations; i++)
	{
		sum += add;
		add /= 2.0;
	}

	// Stop measuring time and calculate the elapsed time
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

	printf("Result: %.20f\n", sum);

	printf("Time measured: %.3f seconds.\n", elapsed.count() * 1e-9);

	return 0;
}