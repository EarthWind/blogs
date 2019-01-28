# Ceph简介
#### 1. 简介
###### 1.1 定义
>Ceph is a distributed object store and file system designed to provide excellent performance, reliability and scalability.

Ceph官网对Ceph给出了以上的定义，主要就是说：Ceph是用于提供高性能、高可用性、高扩展性的分布式对象存储和文件系统；从定义中，首先明确Ceph是一个分布式的存储系统，能够在同一集群中同时提供对象存储、文件存储和块存储，

- 对象存储：Ceph提供REST风格的接口称为RGW(RADOS GateWay),能够兼容S3和Swift的接口
- 文件存储：Ceph提供了兼容POSIX的网络文件系统CephFS，主要有高性能、大数据存储的特性
- 块存储：Ceph中的块设备称为RBD(RADOS Block Device), 常为虚拟化应用提供后端存储，比如OpenStack的cinder后端存储、Glance的镜像存储和虚拟机的数据存储等

###### 1.2 组件
在一个Ceph集群中，主要有以下几个组件组成：

- Ceph MON: (Monitor)主要用于管理集群图(cluster map)，即集群中的各个组件的信息和关系，在一个集群中必须有多个Monitor进行高可用；
- Ceph OSD: (Object Storage Device)存储数据并检查自身及其他OSD的状态并定时报告给Monitor；
- Ceph MGR: (Manager)主要用来提供给外部监控工具监控Ceph的接口；
- Ceph MDS: (Metadata Server)Ceph在提供CephFS文件系统功能使，需要使用该组件来进行文件的查询；
- Ceph Client: 用于和Ceph集群交互的客户端；

###### 1.3 集群图(*cluster map*)
在Ceph集群中，有以下五类map组成cluster map:

- The Monitor Map: 保存集群的fsid(集群唯一ID号)和集群中每个Monitor的名称、地址和端口号，Monitor Map的创建时间和最后修改时间以及当前版本号，可以使用```ceph mon dump```命令查看;
```
$ ceph mon dump
dumped monmap epoch 1
epoch 1
fsid eed882db-b60b-4fe0-af41-6ad7201cfc9d
last_changed 2018-11-05 14:07:18.357622
created 2018-11-05 14:07:18.357622
0: 192.168.4.51:6789/0 mon.mon
```

- The OSD Map: 包含集群的fsidh和集群中每个OSD的状态、数据分布时的权重以及地址端口等信息，另外还有OSD Map的创建时间，最近修改时间以及版本号，其他的则为一些属性信息，可以使用```ceph osd dump```命令查看；
```
$ ceph osd dump
epoch 85
fsid eed882db-b60b-4fe0-af41-6ad7201cfc9d
created 2018-11-05 14:07:19.544382
modified 2019-01-03 16:49:19.679614
flags sortbitwise,recovery_deletes,purged_snapdirs
crush_version 17
full_ratio 0.95
backfillfull_ratio 0.9
nearfull_ratio 0.85
require_min_compat_client jewel
min_compat_client jewel
require_osd_release mimic
max_osd 8
osd.0 up   in  weight 1 up_from 76 up_thru 0 down_at 70 last_clean_interval [62,69) 192.168.4.51:6800/38149 192.168.4.51:6801/38149 192.168.4.51:6802/38149 192.168.4.51:6803/38149 exists,up b450547d-827c-4715-a907-539aa253c57d
```

- The PG Map: 保存PG(Place Group)归置组的版本号，PG最近的操作和每个OSD上的主PG和PG的情况，可以使用```ceph pg dump```命令查看；
```
$ ceph pg dump
dumped all
version 601923
stamp 2019-01-17 15:23:36.263365
last_osdmap_epoch 0
last_pg_scan 0
PG_STAT OBJECTS MISSING_ON_PRIMARY DEGRADED MISPLACED UNFOUND BYTES LOG DISK_LOG STATE STATE_STAMP VERSION REPORTED UP UP_PRIMARY ACTING ACTING_PRIMARY LAST_SCRUB SCRUB_STAMP LAST_DEEP_SCRUB DEEP_SCRUB_STAMP SNAPTRIMQ_LEN
sum 0 0 0 0 0 0 0 0
OSD_STAT USED    AVAIL   TOTAL   HB_PEERS        PG_SUM PRIMARY_PG_SUM
7        1.0 GiB 223 GiB 224 GiB [0,1,2,3,4,5,6]      0              0
```

- The CRUSH Map: 用于保存所有的存储设备以及划分失败域以及数据保存中所使用的数据存储规则，使用```ceph osd getcrushmap -o filename```和```crushtool -d crushbinfile -o outputfilename```命令查看；
```
$ ceph osd getcrushmap -o crush.map.bin
$ crushtool -d crush.map.bin -o crush.map
$ cat crush.map
#begin crush map
tunable choose_local_tries 0
...
#devices
device 0 osd.0 class ssd
#types
type 0 osd
...
#buckets
root default {
        ...
}
#rules
rule replicated_rule {
        ...
}
# end crush map
```

- The MDS Map: 包括了MDS Map版本号、创建和修改时间以及保存元数据的池，元数据服务器列表和状态，使用```ceph fs dump```命令查看
```
$ ceph fs dump
dumped fsmap epoch 1
e1
enable_multiple, ever_enabled_multiple: 0,0
compat: compat={},rocompat={},incompat={1=base v0.20,2=client writeable ranges,3=default file layouts on dirs,4=dir inode in separate object,5=mds uses versioned encoding,6=dirfrag is stored in omap,8=no anchor table,9=file layout v2,10=snaprealm v2}
legacy client fscid: -1
No filesystems configured
```
