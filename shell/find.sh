#!/bin/bash
:<<'EOF'
-name 按照文件名查找文件。
-perm 按照文件权限来查找文件。
-prune 使用这一选项可以使f i n d命令不在当前指定的目录中查找，如果同时使用了- d e p t h
选项，那么- p r u n e选项将被f i n d命令忽略。
-user 按照文件属主来查找文件。
-group 按照文件所属的组来查找文件。
-mtime -n +n 按照文件的更改时间来查找文件， - n表示文件更改时间距现在n天以内，+ n
表示文件更改时间距现在n天以前。F i n d命令还有- a t i m e和- c t i m e选项，但它们都和- m t i m e选项
相似，所以我们在这里只介绍- m t i m e选项。
-nogroup 查找无有效所属组的文件，即该文件所属的组在/ e t c / g r o u p s中不存在。
-nouser 查找无有效属主的文件，即该文件的属主在/ e t c / p a s s w d中不存在。
-newer file1 ! file2 查找更改时间比文件f i l e 1新但比文件f i l e 2旧的文件。
-type 查找某一类型的文件，诸如：
b - 块设备文件。
d - 目录。
c - 字符设备文件。
p - 管道文件。
l - 符号链接文件。
f - 普通文件。
-size n[c] 查找文件长度为n块的文件，带有c时表示文件长度以字节计。
-depth 在查找文件时，首先查找当前目录中的文件，然后再在其子目录中查找。
-fstype 查找位于某一类型文件系统中的文件，这些文件系统类型通常可以在配置文件
/ e t c / f s t a b中找到，该配置文件中包含了本系统中有关文件系统的信息。
-mount 在查找文件时不跨越文件系统m o u n t点。
-follow 如果f i n d命令遇到符号链接文件，就跟踪至链接所指向的文件。
-cpio 对匹配的文件使用c p i o命令，将这些文件备份到磁带设备中。

-name,按文件名查找，可以匹配模式:
	find . -name "*.txt" -print
-perm:
	find . -perm 755 -print		表示查找当前目录下文件属性为755的文件
	find . -perm -007 -print	表示查找所有用户的了
-prune,表示查找时忽略某个目录
find /apps -name "/apps/bin" -prune -o -print


使用exec或ok来执行shell命令
在查到到文件时可以使用需要的命令去操作：
e x e c选项后面跟随着所要执行的命令，然后是一对儿{ }，一个空格和一个\，最后是一个分号。
find logs -type f -mtime +5 -exec rm {} \;
EOF
