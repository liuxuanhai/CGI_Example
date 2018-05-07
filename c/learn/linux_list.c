/******************************************    *******************************

      > File Name: linux_list.c

      > Author: Ywl

      > Descripsion:

      > Created Time:     Sun 06 May 2018 10:04:01 AM CST

      > Modify Time: 

 *********************************    ***************************************/
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "list.h"

struct student{
    int id;
    struct list_head list;
};

void print_student(struct student *stu)
{
    printf("printf--------------------------------\n");
    printf("student->id = %d\n", stu->id);
}

int main()
{
    struct student *stu1, *stu2, *stu3;
    struct student *stu;
    LIST_HEAD(stu_head);
    
    stu1 = (struct student *)malloc(sizeof(struct student));
    stu1->id = 1;
    INIT_LIST_HEAD(&stu1->list);

    stu2 = (struct student *)malloc(sizeof(struct student));
    stu2->id = 2;
    INIT_LIST_HEAD(&stu2->list);

    stu3 = (struct student *)malloc(sizeof(struct student));
    stu3->id = 3;
    INIT_LIST_HEAD(&stu3->list);

    list_add(&stu1->list, &stu_head);
    list_add(&stu2->list, &stu_head);
    list_add(&stu3->list, &stu_head);

    list_for_each_entry(stu, &stu_head, list){
        print_student(stu);
    }

    list_for_each_entry_reverse(stu, &stu_head, list){
        print_student(stu);
    }
/*
    list_del(&stu->list);
    list_for_each_entry(stu, &stu_head, list){
        print_student(stu);
    }
*/
    list_replace(&stu3->list, &stu2->list);
    list_for_each_entry(stu, &stu_head, list){
        print_student(stu);
    }

    return 0;
}
