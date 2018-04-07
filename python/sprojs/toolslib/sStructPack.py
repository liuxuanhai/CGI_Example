#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
      > File Name: sStructPack.py
      > Project Name: toolslib
      > Author: Ywl
      > Email: astralrovers@outlook.com
      > Descripsion:
      > Site:   
      > Software:   PyCharm Community Edition
      > Created Time:     2018/3/31 15:32 
      > Modify Time:
"""
import struct,ctypes
from enum import Enum, unique

@unique
class sStrcutProtocol(Enum):
    #二进制转换类型 CType_PYType_Size
    #C-Type Python-Type
    PadByte_NoVal = 'x'     #pad bute  to nu value
    Char_Str1 = 'c'         #char to str len=1
    SChar1 = 'b'            #char to int len=1
    UChar1 = 'B'            #unsigned char to int len=1
    Bool1 = '?'             #Bool to bool len=1
    Short2 = 'h'            #short to int len=2
    UShort2 = 'H'           #unsigned short to int len=2
    Int4 = 'i'              #int to int len=4
    UInt4 = 'I'             #unsigned int to int len=4
    LInt4 = 'l'             #long int to int len=4
    ULInt4 = 'L'            #unsigned long int to int len=4
    LLInt8 = 'q'            #long long int to int len=8
    ULLInt8 = 'Q'           #unsigned long long to int len=8
    Float4 = 'f'            #float to float len=4
    Double8 = 'd'           #double to float len=8
    sArrayChar_Str = 's'    #char[] to str
    pArrayChar_Str = 'p'    #char[] to str
    VoidPoint_Int = 'P'     #void * to int

    '''
    字节对齐
    '''
    Native4 = '@'			#4字节对齐
    Native = '='			#原字节对齐
    Little = '<'			#小端, 原字节数
    Big    = '>'			#大端, 原字节数
    Net    = '!'			#网络字节序, 原字节数

def sMakeDictData():
    pass

class _sTestStruct_():
    s = 5
    def __init__(self):
        self.name = {
        'type' : '11s',
        'cont' : 'yuwangliang'
    }
        self.age = {
        'type' : 'H',
        'cont' : 23
    }
        self.__attrlist = ['name', 'age']
    def protocol(self):
        return self.__attrlist
        pass

class sStructOperation():
    def __init__(self):
        self.buffer = ctypes.create_string_buffer(1024*4)
    #打包为网络数据
    def pack(self, data, sClass=None, protocol=None):
        if protocol == None:#参数协议为空
            if sClass != None:#数据类型
                protocol = self.makeProtocol(sClass)
                if protocol == None:
                    return 'err'
            else:
                return 'err'
        s = struct.Struct(protocol)
        s.pack_into(self.buffer, 0, *data)
        return self.buffer
    #解包为元组数据
    def unpack(self, data, sClass=None, protocol=None):
        if protocol == None:#参数协议为空
            if sClass != None:#数据类型
                protocol = self.makeProtocol(sClass)
                if protocol == None:
                    return 'err'
            else:
                return 'err'
        s = struct.Struct(protocol)
        s.unpack_from(self.buffer, 0)
        return self.buffer
    def makeProtocol(self, sClass):
        ret = ''
        if hasattr(sClass, 'protocol'):
            '''
            slist = sClass.__dict__
            for key, value in slist.items():
                if  str(sClass.__class__.__name__) not in str(key):
                    print(key)
                    namelist.append(str(key))
                    '''
            for index in sClass.protocol():
                ret = ret + getattr(sClass, index)['type']
        else:
            ret = None
        print(ret)
        return ret

if __name__ == '__main__':
    test1 = sStructOperation()
    test2 = _sTestStruct_()
    back = test1.pack((b'yuwangliang', 22), sClass=test2)
    print(test1.unpack(back, sClass=test2))
    #test1.makeProtocol(test2)
    pass