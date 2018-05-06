/******************************************    *******************************

      > File Name: usrlib.c

      > Author: Ywl

      > Descripsion:

      > Created Time:     Sat 05 May 2018 12:08:12 AM PDT

      > Modify Time: 

 *********************************    ***************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../module/module.h"
#include "usrlib.h"

char c_str_to_val(unsigned char *buf, unsigned int len, unsigned int type, char *str)
{   
    int sign;
    int tmp;

    memset(buf, 0, len);
    switch(type)
    {
        case TYPE_SINT1:
            if(len < 1)
            {
                return false;
            }
            *buf = strtol((char *)str, NULL, 10);
            break;
        case TYPE_SINT2:
            if(len < 2)
            {
                return false;
            }
            sign = strtol(str, NULL, 10);
            memcpy(buf, &sign, 2);
            break;
        case TYPE_SINT4:
            if(len < 4)
            {
                return false;
            }
            sign = strtol(str, NULL, 10);
            memcpy(buf, &sign, 4);
            break;
        case TYPE_BIT:
            if(len < 1)
            {
                return false;
            }
            *buf = strtoul(str, NULL, 10) & 0x01;
            break;
        case TYPE_UINT1:
            if(len < 1)
            {
                return false;
            }
            *buf = strtoul(str, NULL, 10);
            break;
        case TYPE_UINT2:
            if(len < 2)
            {
                return false;
            }
            tmp = strtoul(str, NULL, 10);
            memcpy(buf, &tmp, 2);
            break;
        case TYPE_UINT3:
            if(len < 3)
            {
                return false;
            }
            tmp = strtoul(str, NULL, 10);
            memcpy(buf, &tmp, 3);
            break;
        case TYPE_UINT4:
            if(len < 4)
            {
                return false;
            }
            tmp = strtoul(str, NULL, 10);
            memcpy(buf, &tmp, 4);
            break;
        case TYPE_STR:
            strncpy((char *)buf, str, len);
            break;
        case TYPE_DIG:
        case TYPE_NULL: 
            tmp = strlen(str);
            if(len * 2 < tmp)
            {
                tmp = len * 2;
            }
            m_ascii_to_hex(buf, str, tmp);
            break;
        default:
            return false;
    }
    return true;
}
char c_val_to_str(char *str, const unsigned char *buf, unsigned int len, unsigned int type)
{
    *str = 0;
    switch(type)
    {
        case TYPE_SINT1:
            if(len != 1)
            {
                return false;
            }
            sprintf((char *)str, "%d", (char)buf[0]);
            break;
        case TYPE_SINT2:
            if(len != 2)
            {
                return false;
            }
            sprintf((char *)str, "%d", (short)s_byte_to_int(buf, 2));
            break;
        case TYPE_SINT4:
            if(len != 4)
            {
                return false;
            }
            sprintf((char *)str, "%d", (int)s_byte_to_int(buf, 4));
            break;
        case TYPE_BIT:
            if(len != 1)
            {
                return false;
            }
            sprintf((char *)str, "%u", buf[0] & 0x01);
            break;
        case TYPE_UINT1:
            if(len != 1)
            {
                return false;
            }
            sprintf((char *)str, "%u", buf[0]);
            break;
        case TYPE_UINT2:
            if(len != 2)
            {
                return false;
            }
            sprintf((char *)str, "%u", s_byte_to_int(buf, 2));
            break;
        case TYPE_UINT3:
            if(len != 3)
            {
                return false;
            }
            sprintf((char *)str, "%u", s_byte_to_int(buf, 3));
            break;
        case TYPE_UINT4:
            if(len != 4)
            {
                return false;
            }
            sprintf((char *)str, "%u", s_byte_to_int(buf, 4));
            break;
        case TYPE_STR:
            strcpy((char *)str, (char *)buf);
            break;
        case TYPE_DIG:
        case TYPE_NULL:
            m_digit_to_ascii(str, buf, len);
            str[len << 1] = 0;
            break;
        default:
            return false;
    }
    return true;
}
