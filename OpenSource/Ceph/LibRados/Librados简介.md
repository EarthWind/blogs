# librados
## 简介
> librados API可以和Ceph集群中的两类后台进程通信：
> - Ceph Monitor
> - Ceph OSD Daemon

## 安装
#### librados for C/C++
```SHELL
# for debian
sudo apt install librados-dev
# for centos
sudo yum install librados2-devel
```

#### librados for python
```SHELL
# for debin
sudo apt install python-rados
# for centos
sudo yum install python-rados
```

## 配置集群HANDLE
> Ceph客户端通过librados直接和OSD交互进行数据的存储和检索，客户端首先使用librados和Ceph Monitors连接，一旦连接成功后，librados会从Monitors中获取Cluster Map(集群图)，当客户端进行读写操作时，可以创建一个绑定在pool(池)上IO上下文进行；

![集群交互图](http://docs.ceph.com/docs/master/_images/ditaa-403756b622f733fc4f8f8e6e30624c7f5255af58.png)

> 客户端配置:
> - rados_create()或rados_create2()使用的user ID
> - cephx的认证密钥
> - monitor的ID和IP地址
> - 日志等级
> - 调试等级

> 客户端创建集群handle之后，可以进行如下的操作:
> - 获取集群统计信息
> - 使用池操作
> - 获取和设置配置

## 配置IO上下文
> 一旦handle连接集群成功后，就可以创建一个IO上下文(I/O Context)来开始对集群进行读写操作，通常一个IO上下文会绑定到一个指定的池上，所以handle配置的用户需要有对存储池操作的权限，IO上下文的功能包括：
> - 读写数据和扩展属性
> - 查询和遍历对象和扩展其属性
> - 对存储池进行快照，查询快照等
