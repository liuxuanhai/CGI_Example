#!/usr/bin/python3

import os
from collections import Iterable

ls = []

def getPythonFile(path, ls):
    filelist = os.listdir(path)
    try:
        for temp in filelist:
            pathTemp = os.path.join(path, temp)         #merge documents
            if os.path.isdir(pathTemp) == True:
                getPythonFile(pathTemp, ls)
            elif pathTemp[pathTemp.rfind('.')+1:].upper() == 'PY':
                ls.append(pathTemp)
    except PermissionError:
        pass

if __name__ == "__main__":
    while True:
        path = input("input path:").strip()
        if True == os.path.isdir(path):
            break
    getPythonFile(path, ls)             #ls is address
    print(ls)
    #判断一个对象是不是可迭代类型
    print(isinstance(ls, Iterable))

#join的用法，将一个序列用前面的字符隔开组成一个字符串，split与其相反，将字符串使用指定字符分开为一个列表
    test = ['python', 'JavaScript']
    print(':'.join(test))
    print(':'.join(test).split(':'))

    #实现C语言下标遍历
    for i, value in enumerate(test):
        print(i, value)
#简洁的列表生成式
#生成器
    print([x * x for x in range(1,11)])
    print([x * x for x in range(1,11) if x%2 == 0])
    print([m + n for m in 'ABC' for n in 'XYZ'])

    #generator 对象，保存的是一个有规律的数列的算法
    g = (x * x for x in range(10))
    #实际上将上述[]换为(),此时保存的不是列表，而是一个算法，省去的很大内存
    for n in g:#可以用next(g)获取他的下一个值，基本不用
        print(n)
    def fbd(num):
        n, a, b = 0, 0, 1
        while n < num:
            yield b                 #函数里出现yeild，那么它将不是一个普通的函数，是generator对象,每次执行时(netx())遇到yield就会返回,下次调用时继续上次的地方执行
            a, b = b, a + b         #这句话比较难以理解，其实是先执行赋值操作，a等于b的原值, b等于a的原值加b的原值
            n = n + 1
        return 'None'               #这个返回值是捕捉不到的，除非用异常处理
    f = fbd(5)
    for n in fbd(12):
        print(n)

