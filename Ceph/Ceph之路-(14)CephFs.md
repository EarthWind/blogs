# CephFs
## 简介
> Ceph 文件系统（ Ceph Fs）是个 POSIX 兼容的文件系统，它使用 Ceph 存储集群来存储数据。主要有三种形式的客户端：
> - Kernel Object,内核态接口，使用mout命令来挂载CephFs文件系统
> - FUSE用户空间文件系统，使用ceph-fuse命令来挂载CephFs到操作系统指定的目录
> - 使用API编写的客户端

## 基本命令
#### 新增FS
```SHELL
# 新增文件系统元数据池和数据池
ceph osd pool create cephfs_fs <pg_num> <pgp_num>
ceph osd pool create cephfs_metada <pg_num> <pg_num>

# 创建fs
ceph fs new <fs-name> <metadata-pool> <fs-pool>

# 查看创建的fs
ceph fs ls
ceph fs dump
```

#### 挂载FS
```SHELL
# 查看模块是否加载
[cpu@mon ~]$ lsmod | grep rbd
rbd                   262144  0
libceph               524288  2 ceph,rbd
# 没有加载模块时应加载相应的模块
[cpu@mon ~]$  modprobe rbd
[cpu@mon ~]$  lsmod | grep rbd
# 使用admin挂载，从/etc/ceph/ceph.client.admin.keyring获取admin用户的keyring
AQARTkRbdUFhKhAAstGAl0UkUZu/pG8AyvD05Q==
# 挂载
sudo mkdir /mnt/mycephfs
sudo mount -t ceph  {mds-host-ip}:6789:/ /mnt/mycephfs -o name=admin,secret=AQARTkRbdUFhKhAAstGAl0UkUZu/pG8AyvD05Q==
# 查看挂载
df –Th
# 注意
由于客户端在挂载文件系统的时候没有指定需要挂载的文件系统，所以需要通过ceph fs set-default来设置默认使用的文件系统；
```

