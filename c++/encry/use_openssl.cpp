#include <iostream>
#include <string>
#include <vector>

#include <stdlib.h>
#include <string.h>

#include "encryption.h"

using namespace std;


int main(int argc, char **argv)
{
	unsigned char md[33] = {0};
	string pass = "password";

	md5((const unsigned char *)pass.c_str(), pass.length(), md);

	printf("md5: %s\n", md);

	memset(md, 0, sizeof(md));
	sha256((const unsigned char *)pass.c_str(), pass.length(), md);

	printf("sha256: %s\n", md);
	return 0;
}
/* g++ use_openssl.cpp -lcrypto -lssl */
