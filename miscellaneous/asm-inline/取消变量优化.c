int main()
{
	int __attribute__((aligned(64))) a = 0; // main函数栈上的变量，64byte对齐

	/*取消一个变量的优化*/
	asm volatile(""
				 :
				 : "r,m"(a)
				 : "memory");
}