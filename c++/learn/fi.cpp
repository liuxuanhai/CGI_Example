#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
	ofstream ofile;
	ofile.open("ofile.txt");

	ofile << "c++ learn" <<endl;

	ofile.close();
	return 0;
}
