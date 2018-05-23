/******************************************    *******************************

      > File Name: null.c

      > Author: Ywl

      > Descripsion:

      > Created Time:     Tue 08 May 2018 07:04:01 PM PDT

      > Modify Time: 

 *********************************    ***************************************/
#include <stdio.h>

int main()
{
    char *p = NULL;
    if(p)
    {
        printf("addr is %p\n", p);
    }
    return 0;
}
