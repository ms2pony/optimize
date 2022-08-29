## 20220617

### todo

rdtsc_short_test2.c 对应的程序的各种情况的执行结果分析较为困难(特定情况的每次执行结果变化较大)，
有空去分析一下这个程序

### rdtsc_short_test.c

#### 探究内容

1. 同一个程序先后顺序对执行时间的影响，即第一次执行时间和第二次执行时间不同
2. prefetch 和 clflush 指令是否可以消除 1 的影响
3. 多次测量求平均值是否可以消除 1 的影响

#### 探究结果

- rdtsc_short_test1s

  case1,case2-代码块 1,,case2-代码块 2，3 个情况下的结果都一样，
  大部分时候的运行结果都是第一个执行的执行时间最长

- rdtsc_short_test2

  使用 prefetch 指令将数据提前放入 cache 中似乎也无法消除 1 的影响

  导致这种结果的可能原因：prefetch 不应该在 process3 中

- rdtsc_short_test3

  多次测量求平均值可以消除 1 的影响

### rdtsc_long_test.c

对于执行时间较短的程序而言，其执行的先后顺序确实对执行时间的影响，
这个结论是否对代码量较多的程序有影响

探究结果如下：

- rdtsc_long_test
  大部分时候的运行结果是三次的执行时间差不多

### rdtsc_short_test2.c

探究对 rdtsc_short_test.c 中的 rdtsc_short_test2 函数进行适当修改后，
能否让执行时间固定(程序先后执行的执行时间)
