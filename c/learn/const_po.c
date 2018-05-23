/******************************************    *******************************

      > File Name: const_po.c

      > Author: Ywl

      > Descripsion:

      > Created Time:     Sat 12 May 2018 01:37:26 PM CST

      > Modify Time: 

 *********************************    ***************************************/
#include <stdio.h>

int main()
{
    int test = 90;
    const int *p = &test;
    //*p = 8;
    test = 8;
    return 0;
}
