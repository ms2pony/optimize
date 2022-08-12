## 常识

### 命令行加密

`openssl enc -aes-128-cbc -in plain.txt -out encrypt.txt -iv f123 -K 123 -nosalt -p`

`-nosalt` 不加盐

`-P` Print out the key and IV used then immediately exit: don't do any encryption or decryption.

iv cbc模式需要加的参数，叫做初始化向量

`openssl speed -elapsed -evp aes-256-cbc`

### AES基础

分割数据，一个块为128bit，固定的；密钥不固定，有128bit, 192bit, 256bit，被称为AES128，AES192，AES256

#### aes-gcm  aes-ccm

AES GCM模式涉及CTR、MAC(消息认证码)、GMAC(伽罗华消息认证码)；GCM是认证加密模式中的一种
参考：<https://juejin.cn/post/6844904122676690951> k1 GMAC k2 GCM

参考 <https://www.cnblogs.com/xzj8023tp/p/12970790.html>

#### 填充

这几种：NoPadding、PKCS5Padding、ISO10126Padding、PKCS7Padding

参考 <https://www.cnblogs.com/xzj8023tp/p/12970790.html> k NoPadding

#### 加密模式

1. ECB 模式
简单，有利于并行计算

2. CBC 模式
不利于并行计算；需要初始化向量IV

IV一般为16字节全0

cbc本质上和ecb差别不大，唯一区别是将前一次加密结果，与要加密的内容异或。因此，cbc的并行性较差，因为每次都要等待前一次的结果

除此之外还有：cfb(密文反馈)ofb(输出反馈)ctr(计数器)
参考：<https://juejin.cn/post/6844904122676690951> k 加密模式

## 实战

### 工具

### 实战1-正确性验证

#### 涉及文件

实战1

1. `/home/tam/Desktop/myopenssl/AES/main.c` AES程序；
2. `/home/tam/Desktop/myopenssl/learn-openssl/day06_aes/src/aes.c` AES程序，使用了AES_ecb_encrypt最简单的工作模式
3. `/home/tam/Desktop/myopenssl/learn-openssl/day06_aes/test/test1.c`
4. `/home/tam/Desktop/myopenssl/relic/src/bc/test1.c`

上面两个都使用了ecb模式，所有明文和密钥一致的话，密文肯定一样

#### 步骤

编译编译上面两个文件夹，使用命令`make`
调试2.程序，查看in(明文)，userKey(密钥)两个变量：
`x/4gx in`
`x/4gx userKey`

(gdb) x/4gx userKey
0x555555558015: 0x572c6f6c6c654861      0x626d2121646c726f
0x555555558025: 0xe68f87e98bb5e600      0xbab8e4b497e9b697
(gdb) x/4gx in
0x555555558004: 0x2073692073696854      0x2121206e69616c70
0x555555558014: 0x2c6f6c6c65486100      0x6d2121646c726f57

调试1.程序，`b 92` 即断点达到in和key变量赋值后

将上面查看到的两个变量赋值给1.程序中的对应变量in和key，只要上面每个变量输出的前16个byte：
`set {char*}key=0x572c6f6c6c654861`
`set {char*}(key+8)=0x626d2121646c726f`
同理in，赋值的时候要小心，程序2.输出的值，左边是低4字节，右边是高4字节

然后使用`gdb` 的命令`x/4gx 变量`的对应两个变量，相等表示程序1的明文和密钥与程序2的一样了。

然后两个程序继续调试，知道算出了out，使用gdb查看这两个程序的out(密文)，相等即说明两个AES加密程序正确

out的结果为：

(gdb) x/4gx out
0x555555561130: 0x9fe2104cbcd87036      0xa9bcbe0f1e2558af
0x555555561140: 0x0000000000000000      0x000000000001cec1

实验后发现是一致的，也就是说这两个程序正确

#### 结果

1. `/home/tam/Desktop/myopenssl/learn-openssl/day06_aes/test/test1.c`表明ecb模式性能要比cbc高，即使只加密一个块(128bit)，ecb加密一个块一般都在1200-2000时钟周期，而cbc在1800-2800个时钟周期。

2. `/home/tam/Desktop/myopenssl/learn-openssl/day06_aes/test/test1.c`与`/home/tam/Desktop/myopenssl/learn-openssl/day06_aes/src/aes.c`表明：
cbc与ecb相同明文和密钥，且cbc的iv为全0时，输出的密文是一致的

### 实战2-aesni_ecb_encrypt的问题

在明文的指针in传入aesni_ecb_encrypt时，必须对明文进行顺序调整，因为AES_set_encrypt_key函数进行密钥扩展后打乱的顺序，所以in也要重新调整顺序，这样才不会出错
参考 /home/tam/Desktop/myopenssl/openssl/crypto/aes/aes_core.c 从1433开始的4行

```asm
s0 = GETU32(in     ) ^ rk[0];
s1 = GETU32(in +  4) ^ rk[1];
s2 = GETU32(in +  8) ^ rk[2];
s3 = GETU32(in + 12) ^ rk[3];
```
