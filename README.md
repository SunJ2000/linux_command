# my_linux_command
自己实现的常用的linux命令

### 1. ls
只支持 -l,-h,-t,-a,-r命令，不支持-R；

### 2. mkdir
支持 -p；

### 3. kill
支持-l -s ,只支持前32个信号，具体可以使用kill -l查看；

### 4. cat
支持 -s -b -n;

### 5. mv
支持 -i -f -n；使用rename函数完成，而不是link/unlink；

### 6. tree
支持-a -d -s -f
