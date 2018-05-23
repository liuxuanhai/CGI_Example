#include <iostream>

using namespace std;

/* 引用类型长做参数传递，代替指针，比指针安全，不存在NULL
 * 参数相当于传入变量的别名
 *
 * */
void fun(int &c)
{
	c = 90;
}

int main()
{
	int a = 9;
	int &b = a;		/* 引用类型只是对原变量指定一个别名，比如一个人有两个名字，但是都是代表的同一个人 
	
				类似指针，但是不会单独开辟空间，并且变为其他变量的引用，定义时就初始化	*/

	printf("a addr : %p\n", &a);		//从地址可以看出来，a, b的地址相同
	printf("b addr : %p\n", &b);

	int &c = b;
	printf("c addr : %p\n", &c);

	fun(a);
	cout << a << endl;

	//fun(4); //这里是错误的，既然是变量的别名，那传递的就应该是变量，而不字面量，这是一个常数

	return 0;
}
