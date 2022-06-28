%处理data1y的数据
%输出为M，M为后面拟合曲线的向量y
data1x=data17x;
data1y=data17y;

ave=mean(data1y,2);
b=data1y-ave;

%标准差，参数2：除n还是除n-1；参数3，对列进行计算还是对行进行计算
s=std(data1y,1,2);

m=size(data1y,1);%行
n=size(data1y,2);%列

for j=1:m
    for i=1:n
        if (abs(data1y(j,i)-ave(j))-3*s(j))>0
            data1y(j,i)=0;
        else
            continue;
        end
    end
end

clear j i

%求不为0的平均值，~~data1y只有0，1,用于计算不为0的元素个数
M=sum(data1y,2)./sum(~~data1y,2);