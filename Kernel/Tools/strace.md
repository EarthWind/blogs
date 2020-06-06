# STRACE
> strace命令会在程序退出时运行结束，它会拦截和记录程序在运行期间所产生的系统调用和程序收到的信号，默认的记录信息包括了系统调用名称、系统调用的参数和系统调用的返回值，当系统调用出错时，会附加打印errno并给出文本注释，通常可以通过`-o`参数将这些信息保存到一个指定的文件中；

#### 基本参数
> \-i&emsp;&emsp;打印系统调用的指令指针
>
> \-k&emsp;&emsp;答应每个系统调用和的执行栈
>
> \-o&emsp;&emsp;指定将追踪内容输出到文件
>
> \-t&emsp;&emsp;在每行的开始加上时间信息，秒级别(时：分：秒)
>
> \-tt&emsp;&emsp;在每行的开始加上时间信息，微秒级别
>
> \-ttt&emsp;&emsp;微秒级别输出时间
>
> \-T&emsp;&emsp;在系统调用的最后添加系统调用所消耗的时间
>
> \-f&emsp;&emsp;追踪fork调用所产生的子进程
>
> \-ff&emsp;&emsp;如果有-o参数，则将每个进程的最总情况单独输出，文件名为filename.pid
>
> \-F&emsp;&emsp;追踪vfork调用所产生的子进程
> \-c&emsp;&emsp;统计一个程序运行期间系统调用出现次数、出错次数、时间消耗等信息；
>
> \-v&emsp;&emsp;默认情况下系统调用的参数最多只会答应strsize(默认32)字节的数据，多余的会用...表示，该参数会将所有的信息都输出
>
> -e expr&emsp;指定一个表达式来控制如何跟踪

#### e参数表达式
> [qualifier=][!][?]value1[,[?]value2]...

#### 参考列表
- [strace - trace system calls and signals](http://man7.org/linux/man-pages/man1/strace.1.html)
- [手把手教你用Strace诊断问题](https://huoding.com/2015/10/16/474)
