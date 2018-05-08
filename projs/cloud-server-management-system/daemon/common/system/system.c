/******************************************    *******************************

      > File Name: system.c

      > Author: Ywl

      > Descripsion:

      > Created Time:     Tue 08 May 2018 02:29:22 AM PDT

      > Modify Time: 

 *********************************    ***************************************/
#include <stdio.h>

#include "../../module/module.h"
#include "../common.h"

void system_start(void)
{
    network_start();
    while(1)
    {
        m_ostime_delay(OS_SEC(10));
        dbg_print("heart beat");
    }
}
