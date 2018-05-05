/******************************************    *******************************

      > File Name: usrlib.h

      > Author: Ywl

      > Descripsion:

      > Created Time:     Sat 05 May 2018 12:10:11 AM PDT

      > Modify Time: 

 *********************************    ***************************************/

#ifndef _USRLIB_H
#define _USRLIB_H

enum{
    TYPE_NULL,          //无数据类型
    TYPE_STR,           //字符串
    TYPE_DIG,           //数字串(传输时按照数字串自身的顺序，由左向右逐个字节传输。)
    TYPE_BIT,           //状态、bit类型
    TYPE_SINT1,     
    TYPE_UINT1,     
    TYPE_SINT2,     
    TYPE_UINT2,     
    TYPE_UINT3,     
    TYPE_SINT4,     
    TYPE_UINT4      
};

char c_val_to_str(char *str, const unsigned char *buf, unsigned int len, unsigned int type);
char c_str_to_val(unsigned char *buf, unsigned int len, unsigned int type, char *str);
#endif
