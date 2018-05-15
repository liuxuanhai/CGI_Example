#include <iostream>
#include <string>
#include <vector>
#include <string.h>

#include <openssl/md5.h>

using namespace std;

int main(int argc, char **argv)
{
	unsigned char md[33] = {0};
	string pass = "password";
	MD5((const unsigned char *)pass.c_str(), pass.length(), md);

	char key[65];
	char tmp[3];
	for(int i = 0; i < 32; i++)
	{
		sprintf(tmp, "%02x", md[i]);
		strcat(key, tmp);
	}
	key[32] = '\0';
	cout << key << endl;
	return 0;
}
/* g++ use_openssl.cpp -lcrypto -lssl */
