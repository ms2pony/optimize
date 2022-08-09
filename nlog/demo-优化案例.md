## 概述

==时间==：20220809

## 间接分支预测优化

[1]: https://blog.csdn.net/hanzefeng/article/details/82893317 "深入理解 CPU 的分支预测(Branch Prediction)模型"

优化一个数组累加的代码的案例，其循环代码：

```c
if (data[c] >= 128)
    sum += data[c];   
```

上面代码中`data[c] >= 128`的预测成功率只有50%，因为cpu无法预测随机的data[c]，
`data[c]`是int类型(4字节)有符号数, 则可以用符号位(第 31 bit)做为mask，以掩码的方式代替分支判断：

```c
// t=(data[c] - 128) >> 31, t为(data[c] - 128)的第31 bit的值
// data[c] >= 128，则t=0
// data[c] < 128，则t=1
int t = (data[c] - 128) >> 31;

sum += ~t & data[c];
```

上面代码可以免去移位：

```c
int t=-((data[c]>=128));
sum += t & data[c];
```

经过优化后改程序的性能明显提升

### 结论

具有频繁循环的代码应该避免数据强依赖的分支。
