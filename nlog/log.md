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

### 术语-computer architecture和Microarchitecture

- Microarchitecture

   也被叫做computer organization(计算机组成)，缩写为µarch 或 uarch

   > In computer engineering, microarchitecture, also called computer organization and sometimes abbreviated as µarch or uarch, is the way a given instruction set architecture (ISA) is implemented in a particular processor.

   上面的引用来源：<https://en.wikipedia.org/wiki/Microarchitecture> wiki

   意思是说，微架构是特定指令集架构(ISA)在特定处理器中的实现方式

- computer architecture

   也被叫做计算机系统结构(wiki中文词条)，计算机体系结构(百度词条)

   > 计算机体系结构指软、硬件的系统结构，有两方面的含义：一是从程序设计者的角度所见的系统结构，它是研究计算机体系的概念性结构和功能特性，关系到软件设计的特性;二是从硬件设计者的角度所见的系统结构，实际上是计算机体系的组成或实现(参见计算机组织)，主要着眼于性能价格比的合理性。

   上面的引用来源百度百科：<https://baike.baidu.com/item/%E8%AE%A1%E7%AE%97%E6%9C%BA%E4%BD%93%E7%B3%BB%E7%BB%93%E6%9E%84/10547223>

   > 计算机系统结构指的是什么? 是一台计算机的外表? 还是是指一台计算机内部的一块块板卡安放结构? 都不是，那么它是什么? 计算机系统结构就是计算机的机器语言程序员或编译程序编写者所看到的外特性。所谓外特性，就是计算机的概念性结构和功能特性。
   >
   > 计算机系统结构（Computer Architecture）也称为计算机体系结构，它是由计算机结构外特性，内特性，微外特性组成的。经典的计算机系统结构的定义是指计算机系统多级层次结构中机器语言机器级的结构

   上面的引用来源百度百科：<https://baike.baidu.com/item/%E8%AE%A1%E7%AE%97%E6%9C%BA%E7%B3%BB%E7%BB%9F%E7%BB%93%E6%9E%84/10797569>

   上面两个引用说明计算机体系结构有两个方面的含义：在软件上，计算机体系结构是机器语言、汇编语言和高级语言程序员所看到的外特性，是指计算机系统多级层次结构中机器语言机器级的结构，它是软件和硬件/固件的主要交界面。
   在硬件上，是指计算机体系的组成或实现。

   另外，经典的计算机系统结构的定义是指计算机系统多级层次结构中机器语言机器级的结构，也就是程序员能看到的计算机的结构和功能特性。

   > Some definitions of architecture define it as describing the capabilities and programming model of a computer but not a particular implementation. In other definitions computer architecture involves instruction set architecture design, microarchitecture design, logic design, and implementation.

   上面的引用来源wiki中文：<https://en.wikipedia.org/wiki/Computer_architecture>

   意思是说Computer architecture有两种定义：第一种定义范围较小，指计算机的功能和编程模型。第二种定义范围较大，包括ISA，Microarchitecture，Other parts of a computer system: "Bus, memory, DMA, multiprocessing"

   **总结：computer architecture有两种定义：**

  - 第一种定义为经典定义，就是程序员能看到的计算机的结构和功能特性，是计算机系统多级层次结构中机器语言机器级的结构，这种定义是比较常见的
  - 第二种定义的computer architecture有三个内容：Instruction set architecture，Microarchitecture，Systems design，也就是包括了硬件接口(ISA)，硬件设计。

      其中Systems design，可以参考wiki中文：<https://en.wikipedia.org/wiki/Computer_architecture> k Systems design
