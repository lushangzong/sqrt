#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/timeb.h>

//timeb用大写来表示
#define TIMEB timeb

//全局的函数，直接获取x的绝对值
#define ABS(x) ((x)>0?(x):-(x))

//误差上限
#define eps 1e-10

//牛顿法
double sqrt1(double xi)
{
	//返回平方根
 
	double g =  xi;

	//迭代结果在控制精度之内
	while (ABS(g*g - x)>eps)
	{
		//迭代公式
		g = (g + x / g) / 2;
	}
	return g;
}

//计算[0,2)范围内数的平方根
double Tsqrt(double x)
{
	//通过泰勒公式求解
	double sum, coffe, factorial, xpower, term;
	int i;
	sum = 0;  //求和
	coffe = 1;
	factorial = 1;
	xpower = 1;

	//每一次计算的多项式的值，第一次为1
	term = 1; 
	i = 0;

	//当多项式的结果小于精度就退出
	while (ABS(term)>eps)
	{
		sum += term;
		coffe *= (0.5 - i);//系数
		factorial *= (i + 1); //阶乘		
		xpower *= (x - 1); //多项式
		term = coffe * xpower / factorial;
		i++;
	}
	return sum;
}

//级数逼近法
double sqrt2(double x)
{
	//大于2的数要转化为[0,2)区间上去
	double correction = 1;

	//将数字变换到小于2
	while (x >= 2)
	{
		x /= 4;
		correction *= 2;
	}
	//用小于2的结果乘以变换系数
	return Tsqrt(x)*correction;
}

//平方根倒数速算法（卡马克反转）
float sqrt3(float xi) 
{
	
	//储存长整形
	long i;

	//变为float
	float number = (float)xi;
	float x, y;
	const float f = 1.5F;
	x = number * 0.5F;
	y = number;
	i = *(long *)&y;//将浮点数表示的内存转变为整数
	i = 0x5f3759df - (i >> 1); //注意这一行 
	y = *(float*)&i;

	//牛顿迭代
	y = y * (f - (x * y * y));
	y = y * (f - (x * y * y));

	//返回平方根
	return number * y;
}

//二分法
double sqrt4(double x)
{
	//前后中三个值
	double low, up, mid;
	low = 0, up = (x >= 1 ? x : 1);
	mid = (low + up) / 2;
	do
	{
		//通过中间值的平方的结果来选择将中间值赋值给小数还是大数
		if (mid*mid>x)
			up = mid;
		else
			low = mid;
		mid = (up + low) / 2;

		//控制精度
	} while (ABS(mid - low) > eps);

	return mid;
}

//主函数，比较效率
int main() 
{
	//因为一次的计算时间太短，所以增加计算次数
	int number = 100000;

	//计算结果
	double y, y1, y2, y3,y4;

	//计算时间
	double t, t1, t2, t3,t4;

	//计算20个数值
	for (int i = 0; i<20; i++)
	{
		//计算一万以内的数值的平方根
		double x = rand() % 10000;

		//开始时间和截至时间
		clock_t start, finish;

		//内置的函数
		start = clock();
		for (int j = 0; j < number; j++)
		{
			y = sqrt(x);
		}
		finish = clock();
		t = finish-start;

		//牛顿法
		start = clock();
		for (int j = 0; j < number; j++)
		{
			y1 = sqrt1(x);
		}
		finish = clock();
		t1 = finish - start;
		
		//级数逼近法
		start = clock();
		for (int j = 0; j < number; j++)
		{
			y2 = sqrt2(x);
		}
		finish = clock();
		t2 = finish - start;

		//平方根倒数法
		start = clock();
		float x1 = (float)x;
		for (int j = 0; j < number; j++)
		{
			y3 = sqrt3(x1);
		}
		finish = clock();
		t3 = finish - start;

		//二分法
		start = clock();
		for (int j = 0; j < number; j++)
		{
			y4 = sqrt4(x);
		}
		finish = clock();
		t4 = finish - start;

		printf("x=%7.2f y=%16.11f <%6.2f>,y1=%16.11f <%6.2f>,y2=%16.11f <%6.2f>,y3=%16.11f <%6.2f>,y4=%16.11f <%6.2f>\n", x, y, t, y1, t1, y2, t2, y3, t3,y4,t4);
	}
	
	system("pause");
	return 0;

}