# 性能优化相关

==原始创建时间==：20220626

程序性能优化相关的资料收集

## 测量工具

测量工具可以是软件，代码块，指令

### 性能分析工具

- perf，linux下，可以测量硬件事件
- Intel VTune Profiler，跨平台，可以测量硬件事件

### microbenchmark的相关工具

可以得到短小代码块相关性能信息的工具如下：

- 静态性能分析工具，模拟程序运行情况并较为准确的预测其性能的工具
  - uiCA
- 动态性能分析工具，相比于一般的性能分析工具，开销小，更精准
  - nanoBench

## benchmark的设计

- 椭圆曲线密码算法整体性能测试的benchmark设计
- 椭圆曲线密码算法各个部件性能测试的benchmark设计

## 测量时需要注意的点

### 减小误差的测量方法

对于microbenchmark，测量性能参数的过程中应该尽可能减小误差

```c++ {.line-numbers}
saveState()
disablePreemptionAndInterrupts()
serializingInstruction
start <= readPerfCtrs()
serializingInstruction
AsmCode
serializingInstruction
start <= readPerfCtrs()
serializingInstruction
enablePreemptionAndInterrupts()
restoreState()
return end-start
```

上述代码用于测量line6 AsmCode的性能，其中AsmCode是待测代码的n份拷贝，采用了如下手段减小误差：

- 使用序列化指令包裹readPerfCtrs指令，防止乱序执行
- 相减取平均值，第一次测量n取110，第二次取10，然后相减除100
- 相减取平均值的这个操作重复100次，并且是在预热后进行(保证程序进入缓存)
- n的取值要适当，既要足够小能让代码全部进入指令cache，又要足够大一遍能够得到准确值

### 程序运行时cpu频率的变化和超线程

一般情况下，benchmark的程序都能以接近最大睿频的频率运行，且频率都比较稳定。

如果程序每次运行的平均频率抖动得厉害，使用以下方法解决：

- 关闭睿频，程序性能会下降，不推荐
- 将cpu工作模式切换到performance，这样cpu就会一直保持最大频率运行，linux系统下才有这个设置

linux下详细的cpu睿频以及频率设置，参考：<https://blog.csdn.net/Adam_996/article/details/104050066>

有些时候超线程是否开启也会对程序产生影响，开关超线程的方法为：
linux下，运行命令`echo off > /sys/devices/system/cpu/smt/control`

相关问题：

以下问题详细参考：

- windows的关闭超线程功能有bug
- 我的笔记本电脑没有超线程开关
- wsl下的linux系统没有调节睿频的相关设备

### reference clock cycles 和 core clock cycles

参考：<https://zhuanlan.zhihu.com/p/474082387>

- reference clock cycles
  - rdtsc指令得到的就是这种时钟周期
  - 它以CPU的基准频率进行计数，不受动态频率与CPU状态的影响
  - 但是对于同一个程序所需的reference clock cycles，还是会随着cpu频率的变化而变化，cpu频率越高，reference clock cycles越低。
  - 可以通过它得到程序执行的时间，因为基准频率已知
- core clock cycles
  - 高于基准频率时，core clock cycles的值大于reference clock cycles；反之则小于

### 进程切换对时间测量的影响

一般来说，可以获得非常短(小于大约10ms)或者非常长(大于大约1s)的时间段的准确测量值，即使是在负载很重的机器上。对于非常短的时间段，上下文来不及切换；对于非常长的时间段，操作系统会维护每个进程的执行时间信息。参考：`F:\iii\ii\i\workStudation\Studation\optimize\nlog\20220701-测量程序执行时间.md`

除此以外，对于较长时间段的测量，一些已有的性能分析工具还会提供其他性能参数(如时钟周期)。这些工具的原理和操作系统测量时间的原理类似(例如perf的原理：每隔一个固定的时间，就在CPU上（每个核上都有）产生一个中断，在中断上看看，当前是哪个进程的哪个函数，然后给对应的进程的函数加一个统计值，可以手动设置这个频率，参考：<https://zhuanlan.zhihu.com/p/22194920> k 每隔一个固定的时间)。

总结：microbench中的程序不用担心进程切换的问题，插入简单的计时代码就可以测量出程序的性能。对那些时间段中等(10ms~1s)的时间段，可以使用运行多个实例的方法。长时间段程序的其他性能参数可以使用性能分析工具得到。

### 为何rdtsc等指令也能准确测量长时间段

对长时间段的程序而言，由于进程切换的关系，，使用rdtsc进行测量应该是得不到正确结果的，测量的结果应该是会大于实际的结果，但是我经过实验发现，rdtsc指令和系统测量时间的api这两种方法得到的时间是差不多的。该实验是测量了一个执行时间大概为1s的程序，然后两种方法得到的结果误差小于0.1s，结果如下：

```bash
Time measured: 2.31532 seconds.
Time measured: 2.35052 seconds.

Time measured: 2.29715 seconds.
Time measured: 2.30612 seconds.

Time measured: 2.33997 seconds.
Time measured: 2.32541 seconds.
```

该实验参考：

## 如何分析基本块的吞吐量

一个能够预测代码基本块运行情况的网站，地址：<https://uica.uops.info/>

该软件实现的相关论文：uiCA: Accurate Throughput Prediction of Basic Blocks on Recent Intel Microarchitectures，本地位置："F:\iii\ii\i\ref\性能优化\2022UATP.pdf"
