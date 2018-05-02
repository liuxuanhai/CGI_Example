/******************************************    *******************************
      > File Name: main.c
      > Author: Ywl
      > Descripsion:
      > Created Time:     Thu 30 Nov 2017 11:13:28 AM CST
      > Modify Time: 
 *********************************    ***************************************/

#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>

#include "../../common/systemtask/systask.h"

int main(int argc, char **argv)
{
	char os_name[125];
	gethostname(os_name, sizeof(os_name));
	printf("#########################Welcome to %s##########################################\n", os_name);
	printf("##########################Start Server Init, please waiting#####################\n");
	printf("Loading.........................................................................\n");
    c_system_initialize();
	return 0;
}
