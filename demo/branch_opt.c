#include <unistd.h>
#include <fcntl.h>
#include <x86intrin.h>
#include <stdio.h>

int origin(int data[], int len)
{
    int sum = 0;
    for (int i = 0; i < len; i++)
    {
        if (data[i] >= 128)
            sum += data[i];
    }
    return sum;
}

// int opt(int data[], int len);

int opt(int data[], int len)
{
    int sum = 0;
    int t = 0;
    for (int i = 0; i < len; i++)
    {
        t = (data[i] - 128) >> 31;
        sum += ~t & data[i];
    }
    return sum;
}

int opt_no_shift(int data[], int len)
{
    int sum = 0;
    int t = 0;
    for (int i = 0; i < len; i++)
    {
        t = -((data[i] >= 128));
        sum += t & data[i];
    }
    return sum;
}

int main()
{
    int fd = open("/dev/urandom", O_RDONLY);
    int data[100] = {0};
    read(fd, data, 400);
    long t1, t2, time = 0;
    int flag = 0;

    for (int i = 0; i < 100; i++)
    {
        t1 = _rdtsc();
        flag = opt(data, 100);
        t2 = _rdtsc();
        time += t2 - t1;
    }

    printf("%lu\n%d\n", time, flag);

    time = 0;
    for (int i = 0; i < 100; i++)
    {
        t1 = _rdtsc();
        flag = opt_no_shift(data, 100);
        t2 = _rdtsc();
        time += t2 - t1;
    }
    printf("%lu\n%d\n", time, flag);
}