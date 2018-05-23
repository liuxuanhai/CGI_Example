#include <iostream>
#include <string>
#include <vector>
#include <array>
using namespace std;

int main(int argc, char ** argv)
{
	cout << "namespace" << endl;
	char new_len = 10;
	char *test_array = new char[new_len];
	delete [] test_array;

	/* vector 用于动态增长，使用new管理，堆内存，用于数组的话开销较大 */
	vector<int> v;
	v.push_back(3);
	v.push_back(4);
	cout << v.size() << endl;

	v.clear();
	cout << v.size() << endl;
	
	vector<char> v2(4);//这个是可以变量设置的
	cout << v2.size() << endl;
	v2.clear();
	cout << v2.size() << endl;

	/* 数组大小设置时必须固定 */
	array<char, 4> array1 = {1, 2, 3, 4};
	array<char, 4> array2 = array1;
	char buf[4] = {1, 2, 3, 4};

	for(char index : buf)
	{
		cout << index << endl;//打印不出来?
	}
	return 0;
}


