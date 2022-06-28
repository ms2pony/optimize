## 20220628

### 术语-优化编译器

Optimizing compiler (优化编译器)

参考wiki：<https://en.wikipedia.org/wiki/Optimizing_compiler>

> In computing, an optimizing compiler is a compiler that tries to minimize or maximize some attributes of an executable computer program

上面的引用出自wiki：<https://en.wikipedia.org/wiki/Optimizing_compiler>

也就是说优化编译器并不是指一个编译器的优化功能，也不是指对一个编译器进行优化，而是指一个具有优化功能的编译器

### 程序性能优化步骤

参考：csapp，代号："csapp1"，中文名为深入理解计算机系统，第5章开始到5.1节前的内容，本地位置："F:\iii\ii\i\reference\深入理解计算机系统（第三版）.pdf"

程序性能优化，可以先考虑改进原有的算法和数据结构，也可以不改进，我认为一般的程序性能优化不会考虑改进原有的算法和数据结构。下面关于程序性能优化的内容就是指一般的程序性能优化。

程序性能优化也叫代码性能优化，软件性能优化，程序优化的第一步就是要消除不必要的工作，让代码尽可能有效地执行所期望的任务。这包括消除不必要的函数调用、条件测试和内存引用。
<!-- 上面一段部分参考了csapp1 -->

在进行第一步时需要考虑，是优化源代码还是优化汇编代码。如果是优化源代码，则还要考虑优化编译器的能力和局限，因为源代码需要经过编译器编译成机器代码才能在机器上运行。而汇编代码就直接等价于机器代码，不需要借助编译器。对程序优化的第一步可以由如下两种分类：

- 源代码优化

   在使用各种提高代码性能的技术来优化源代码的同时，还要注意编写出容易被编译器优化的代码

- 汇编代码优化

   只需要关注优化后的代码是否能够更高效的完成所期望的任务。这些优化可以是：减少或消除过程调用(函数调用)，减少条件测试，减少内存引用。

程序性能优化的第二部，可以利用处理器提供的指令级并行能力来优化程序，这需要了解运行该程序的机器是如何运作的，需要知道该机器的一些硬件信息(如指令的延迟，吞吐量)。
<!-- 上面的内容参考了csapp1 -->