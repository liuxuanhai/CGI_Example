/******************************************    *******************************

      > File Name: switch.c

      > Author: Ywl

      > Descripsion:

      > Created Time:     Wed 02 May 2018 06:16:02 PM PDT

      > Modify Time: 

 *********************************    ***************************************/
#include <stdio.h>

int main()
{
    int a = 2;
    switch(a){
        case 1: printf("red\n");
        case 2: printf("yellow\n");
        case 3: printf("blue\n");
    }
    char *p = NULL;
    printf("%d\n", p);
    return 0;
}
