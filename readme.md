## 常识

### benchmark、experiment、test三者的区别

- benchmark

   >In computing, a benchmark is the act of running a computer program, a set of programs, or other operations, in order to assess the relative performance of an object, normally by running a number of standard tests and trials against it.

   来源：<https://en.wikipedia.org/wiki/Benchmark_(computing)>

   benchmark是指运行一个程序，程序集或其他操作(或叫做工作流，它包含很多步骤)的行为，而这个行为的目的是评估某个事物的相对性能

- experiment

   experiment是指运行一个程序，程序集或其他操作(或叫做工作流，它包含很多步骤)的行为，而这个行为的目的是验证某个文献的观点、结论、知识是否正确

- test

   test是指运行一个程序，程序集或其他操作(或叫做工作流，它包含很多步骤)的行为，而这个行为的目的是验证目标程序是否按照预期运行

### algorithm

算法中可以含有硬件属性，也可以涉及寄存器，只是通常的算法不涉及计算机体系结构中相关操作(如内存读写、压栈出栈等操作)

### 软件优化

不管是从微架构的角度，指令集(指令集架构)的角度来对一个程序进行性能优化，都属于软件优化，
也就是说，考虑硬件因素但不改变硬件本身的优化都属于软件优化。可以从很多很多角度对软件进行优化，下面列很多种软件优化方法

- 微架构优化
- 指令集优化
- 编程语言优化
- 操作系统优化

   从操作系统的角度来优化软件，举例：一个程序在某个操作系统下需要该操作系统的两个内核模块的功能，经过优化后，该程序对这两个内核模块的开销减少。这样的优化就可以叫做操作系统优化

- 编译器优化

   在优化过程中考虑的因素能够有助于编译器生成更高效的程序，这样的优化称为编译器优化

## preliminary

详见 [preliminary](nlog\preliminary.md)

## 文件说明

### docs

文件夹，与该项目相关或间接相关的比较系统的文档，这些文档比起nlog文件夹中的文档更加系统，正规，读者更能看懂。
