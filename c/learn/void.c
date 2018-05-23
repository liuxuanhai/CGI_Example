/******************************************    *******************************

      > File Name: void.c

      > Author: Ywl

      > Descripsion:

      > Created Time:     Tue 08 May 2018 07:08:47 PM PDT

      > Modify Time: 

 *********************************    ***************************************/
#include <stdio.h>
#include <string.h>
//void (*fun)()
void *fun(void)
{
    printf("none\n");
}

int main(int args, char **argv)
{
    void *point = NULL;
    int *ar = NULL;
    printf("void point size %lu\n", sizeof(void *));
    printf("ar point size %lu\n", sizeof(int *));
    printf("point var size %lu\n", sizeof(ar));
    //printf("void size %d\n", sizeof(void)); //error
    return 0;
}
