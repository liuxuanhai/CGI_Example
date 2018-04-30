#!/bin/bash
#下面是一个多行注释
:<<'EOF'
常用文件重定向命令
command > filename 把把标准输出重定向到一个新文件中
command >> filename 把把标准输出重定向到一个文件中(追加)
command 1 > fielname 把把标准输出重定向到一个文件中
command > filename 2>&1 把把标准输出和标准错误一起重定向到一个文件中
command 2 > filename 把把标准错误重定向到一个文件中
command 2 >> filename 把把标准输出重定向到一个文件中(追加)
command >> filename 2>&1 把把标准输出和标准错误一起重定向到一个文件中(追加)
command < filename >filename2 把c o m m a n d命令以f i l e n a m e文件作为标准输入，以f i l e n a m e 2文件
作为标准输出
command < filename 把c o m m a n d命令以f i l e n a m e文件作为标准输入
command << delimiter 把从标准输入中读入，直至遇到d e l i m i t e r分界符
command <&m 把把文件描述符m作为标准输入
command >&m 把把标准输出重定向到文件描述符m中
command <&- 把关闭标准输入
EOF


#'>>'重定向表示追加，'>'表示直接覆盖
#cat 'file' 会将文件内容全部显示出来，cat 'file' | more能够实现翻页等等
#tee命令,如果想将输出重定向到文件内，并且希望同时看到输出内容可以使用:ls | sort | tee test.txt
:<<'EOF'
e x e c命令可以用来替代当前s h e l l；换句话说，并没有启动子s h e l l。使用这一命令时任何现
有环境都将会被清除，并重新启动一个s h e l l。它的一般形式为：
exec command
其中的c o m m a n d通常是一个s h e l l脚本。
我所能够想像得出的描述e x e c命令最贴切的说法就是：它践踏了你当前的s h e l l。
当这个脚本结束时，相应的会话可能就结束了。e x e c命令的一个常见用法就是在用户
的. p r o f i l e最后执行时，用它来执行一些用于增强安全性的脚本。如果用户的输入无效，该
s h e l l将被关闭，然后重新回到登录提示符。e x e c还常常被用来通过文件描述符打开文件。
记住， e x e c在对文件描述符进行操作的时候（也只有在这时），它不会覆盖你当前的
s h e l l
EOF
