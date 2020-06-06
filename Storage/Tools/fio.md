# FIO
## 编译安装
```SHELL
$ wget https://github.com/axboe/fio/archive/fio-3.13.tar.gz
$ tar -zxvf fio-3.13.tar.gz
$ cd fio-fio-3.13/
$ ./configure
$ make
$ sudo make install
```

## job file参数
> *fio [options] [jobfile] ...*
#### 基本格式
> - ;和#表示注释
```SHELL
; -- start job file --
[global]
rw=randread
size=128m

[job1]

[job2]

; -- end job file --
```
或
```SHELL
; -- start job file including.fio --
[global]
filename=/tmp/test
filesize=1m
include glob-include.fio

[test]
rw=randread
bs=4k
time_based=1
runtime=10
include test-include.fio
; -- end job file including.fio --
```

#### 环境变量
> 可以在命令行中添加变量`var=value`，在脚本中`$var`进行使用；如`$ SIZE=64m NUMJOBS=4 fio jobfile.fio`
>
> 除了自定义的环境变量，fio还保留了关键字
>> - $pagesize:系统页大小
>> - $mb_memory:系统内存的大小，单位为mb
>> - $ncpus:系统CPU数量

#### 参数
###### 参数类型
> - str: 字符串
> - time: d-天，h-小时，m-分钟，s-秒，ms-毫秒，us-微秒，不指定的时候，默认为秒
> - int: 整形
> - bool: 布尔类型，0或1
> - irange: 范围值；1024-4096，也可以使用 : 进行分隔，如1k:4k，多个范围用 / 进行分割

###### 单位
> - kb_base=int: kb的基数，默认为1024，可以更换为1000
> - unit_base=int: 0-指定检测，8-字节为单位，1-以位为单位

###### 常用
```
filename=/dev/emcpowerb　支持文件系统或者裸设备，-filename=/dev/sda2或-filename=/dev/sdb
direct=1                 测试过程绕过机器自带的buffer，使测试结果更真实
rw=randwread             测试随机读的I/O
rw=randwrite             测试随机写的I/O
rw=randrw                测试随机混合写和读的I/O
rw=read                  测试顺序读的I/O
rw=write                 测试顺序写的I/O
rw=rw                    测试顺序混合写和读的I/O
bs=4k                    单次io的块文件大小为4k
bsrange=512-2048         同上，提定数据块的大小范围
size=5g                  本次的测试文件大小为5g，以每次4k的io进行测试
numjobs=30               将该job克隆30份到线程或进程中，每个线程或进程相互独立，运行完全相同的测试，
runtime=1000             测试时间为1000秒，如果不写则一直将5g文件分4k每次写完为止
ioengine=psync           io引擎使用pync方式，如果要使用libaio引擎，需要yum install libaio-devel包
rwmixwrite=30            在混合读写的模式下，写占30%
group_reporting          关于显示结果的，汇总每个进程的信息此外，显示进程组的统计信息而不是每个单独进程或线程的信息，一个进程组包括每个job中所有的numjobs参数的进程或线程；
stonewall,wait_for_previous
                        在启动此作业之前，请等待作业文件中的先前作业退出。 可用于在作业文件中插入序列化点。 石墙也意味着要建立一个新的报告组
lockmem=1g               只使用1g内存进行测试
zero_buffers             用0初始化系统buffer
nrfiles=8                每个进程生成文件的数量
thread                   如果该参数被设置，则会使用pthread_create来替换fork创建线程而不是进程
cpus_allowed             CPU掩码，可以为0,4,5或3,4-8
```

## 参考列表
- [IO测试工具之fio详解](https://www.cnblogs.com/raykuan/p/6914748.html)
- [Fio Output Explained](https://tobert.github.io/post/2014-04-17-fio-output-explained.html)
- [fio document](https://buildmedia.readthedocs.org/media/pdf/fio/latest/fio.pdf)
- [Welcome to FIO’s documentation!](https://fio.readthedocs.io/en/latest/)

