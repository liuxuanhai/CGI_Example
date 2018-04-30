#!/usr/bin/python
# coding=utf-8

"""
#########################################################################################

      > File Name: autoWrite.py

      > Author: Ywl

      > Descripsion:

      > Created Time:     Tue 20 Mar 2018 11:43:13 PM PDT

      > Modify Time: 

#########################################################################################
"""
import re
cCodeHead = '''
/******************************************************************************

                  版权所有 (C), 2001-2013, 三维通信股份有限公司

 ******************************************************************************
  文 件 名   : FilterCoef.c
  版 本 号   : 初稿
  作    者   : xyx
  生成日期   : 2014年8月7日
  最近修改   :
  功能描述   : 滤波器配置
  修改历史   :
  1.日    期   : 2014年8月7日
    作    者   : xyx
    修改内容   : 创建文件

   修改历史   :
  1.日    期   : 2015年1月26日
    作    者   : xyx
    修改内容   : 基于off-air滤波器系统设置,修改为符合HP/LP平台

******************************************************************************/

#include "../Src/Include.h"
#include "../Src/Main.h"
#include "FilterCoef.h"
#if HP_LP_COMP

/*fpga寄存器 滤波器系数*/
const INT16U REG_FILTER_COEF[][4] = {
   { REG1_FILTER_COEF_ADDR, REG1_FILTER_COEF_DATA, REG1_FILTER_COEF_EN, REG1_FILTER_COEF_DONE},
   { REG2_FILTER_COEF_ADDR, REG2_FILTER_COEF_DATA, REG2_FILTER_COEF_EN, REG2_FILTER_COEF_DONE},
   { REG3_FILTER_COEF_ADDR, REG3_FILTER_COEF_DATA, REG3_FILTER_COEF_EN, REG3_FILTER_COEF_DONE},
   { REG4_FILTER_COEF_ADDR, REG4_FILTER_COEF_DATA, REG4_FILTER_COEF_EN, REG4_FILTER_COEF_DONE},
};


//用UltraEdit打开txt文件,通过ALT+C进入列编辑模式,
//在每一行最后列加","号
'''
cCodeTail = '''
void filterCoefprint(void)
{
	INT16U i, j;
	INT16S *data;
	for(i = 0; i < ARRAY_SIZE(FILTER_COEF_Table); i++)
	{
		printf("第%i组\\n",i);
		data = (INT16S *)&FILTER_COEF_Table[i][0];
		for(j = 0; j<100; j++)
		{
			printf("data[%d]=%d\\n", j , *data);
			data++;
		}
	}
}


INT8U bwTocoefIndex(INT32U bw)
{
	INT8U confIndex = 0xff;
    if((bw > 0) && (bw <= 80*1000))
        confIndex = bw/1000 - 1;
	return confIndex;
}



/*****************************外部调用函数***********************************/
#define dbg 0
INT32S  FilterFpgaConfig(INT8U cha, INT32U bw)
{
	INT8U i, coefIndex;
	INT16S *data;
	//  printf("bw=%d\\n",bw);
	coefIndex = bwTocoefIndex(bw);  

	if((coefIndex >= ARRAY_SIZE(FILTER_COEF_Table)) || (cha >= 4))
	{
#if dbg
		printf("bandIndex is error\\n");
#endif
		return 1;
	} 
#if dbg
	printf("-----------------------\\n");
	printf("数组大小:%d,cha = %d\\n",ARRAY_SIZE(FILTER_COEF_Table),cha);
	printf("coefIndex = %d\\n",coefIndex);    
	printf("......................\\n");
	printf("\\r\\n");
#endif
	data = (INT16S *)&FILTER_COEF_Table[coefIndex][0];

	for(i = 0; i < 36; i++)
	{
#if dbg
		printf("data[%d]=%d\\n", i , *data);
#endif
		fpga_write(REG_FILTER_COEF[cha][0], &i, 1);             /*addr 1Byte*/
		fpga_write(REG_FILTER_COEF[cha][1], (INT8U *)data, 2);  /*data 2Bytes*/
		fpga_write(REG_FILTER_COEF[cha][2], (INT8U *)"\\x1", 1); /*en*/
		fpga_write(REG_FILTER_COEF[cha][2], (INT8U *)"\\x0", 1);
		data++;

	}
	fpga_write(REG_FILTER_COEF[cha][3], (INT8U *)"\\x1", 1); 
	fpga_write(REG_FILTER_COEF[cha][3], (INT8U *)"\\x0", 1);

	return 0;

}
#undef dbg
#endif
'''
#匹配36个逗号，有36个系数
def searchSpStrIndexNum(sStr, subStr, sNum):
    number = 0
    while sNum > 0:
        index = sStr.find(subStr)
        if index == -1:
            return -1
        sNum -= 1
        sStr = sStr[index+1:]
        number = number + index + 1
    return number

#需要匹配除了 数字 逗号 负号之外的其他字符
regStr = r'[^0-9,\-]'

def autoWrite():
    writeFile = []
    with open('1Mstep.txt', 'r') as opFile:
        fileBuf = opFile.read()
        for index in range(1, 81):
            spStr = str(index) + 'M:'
            #查找这一组
            sIndex = fileBuf.find(spStr)
            if sIndex == -1:
                print('not find ' + spStr)
                return
            searchBuf = fileBuf[sIndex+len(spStr)+1:]
            #查找这一组的结束位置,36个数据
            sIndex = searchSpStrIndexNum(searchBuf, ',', 36)
            if sIndex == -1:
                print(spStr + 'number error')
                return
            writeBuf = searchBuf[:sIndex+1]
            #匹配之前先去除回车、换行、空格
            swapList = str(writeBuf).split()        #保存去除其他不必要字符的列表
            writeBuf = ''.join(swapList)
            sSearch = re.search(regStr, writeBuf)   #查找非法字符 
            if sSearch != None:
                print(spStr + 'Data Error:' + writeBuf + ' ---- ' + sSearch.group())
                return
            for i in range(len(swapList)):
                swapList[i] = '\t' + swapList[i]    #添加tab
            writeBuf = '\n'.join(swapList) + '\n'

#            print(spStr + ' ' + writeBuf)
#            print('----------------------------------------------------------------------------------------')
            writeFile.append(writeBuf)
    #添加每组数据
    with open('filter.c', 'w', encoding='utf-8') as cSource:
        #添加头部
        cSource.write(cCodeHead + '\n')
        index = 1
        for sList in writeFile:
            filterGroup = 'const INT16S FILTER_COEF_' + str(index) + 'M[]={\n'  +  str(sList) + '};\n\n'
            cSource.write(filterGroup)
            index += 1
        tableStr = 'const INT16S *FILTER_COEF_Table[]={\n'
        tableIndexStr = ''
        for index in range(1, 81):
            tableIndexStr = tableIndexStr + '\tFILTER_COEF_' + str(index) + 'M,\n'
        tableStr = tableStr + tableIndexStr + '};\n'
        cSource.write(tableStr)
        #添加尾部
        cSource.write(cCodeTail + '\n')

if __name__ == "__main__":
    autoWrite()
