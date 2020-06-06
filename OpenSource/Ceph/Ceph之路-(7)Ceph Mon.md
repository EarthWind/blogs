# Ceph Mon
## 简介
> Ceph monitor主要用于维护管理集群cluster map的主副本,在集群中的osd的状态发生改变的时候，实时更新cluster map,各monitor之间使用改进的Paxos算法来提供一致性的元信息管理服务，它能为集群成员，配置和状态提供坚实的存储。客户端在和Monitor通信获取cluster map后，便能清楚的知道集群中Ceph Monitor,Ceph OSD进程或则Ceph元数据服务器的信息，从而可以和这些集群进程通信，另外，集群中的monitor还用来提供认证和日志服务；ceph集群中的map主要包括下面五类：
> - Monitor Map:主要包括了集群的fsid号，monitor服务器的地址和端口，以及map被创建的时间，最近修改时间和当前版本等信息,使用ceph mon dump命令可以查看Monitor Map;
> - OSD Map:主要包括了osd列表，版本信息，osd的基本参数和集群中Pool的信息，使用ceph osd dump可以查看OSD Map;
> - PG Map:主要包括pg列表，pg状态以及pg和osd的映射关系,使用ceph pg dump命令可以查看PG Map;
> - CRUSH Map:主要包括设备列表，bucket类别，bucket实例以及各类存储不同的存储规则;
> - MDS Map:主要包括map的版本号，创建时间和修改时间以及存储文件系统元数据的pool信息和其服务器的状态，可使用命令ceph fs dump查看该map;

## 原理
#### 架构
> Monitor服务会将所有的Cluster Map的变化都写到单个的Paxos实例中，并次持久化到后端kv数据库(rocksdb),其他Monitor在同步操作的时候可以查询到最新的Cluster map;注意，monitor和monitor之间不是通过配置文件ceph.conf来连接彼此，不像客服端或其他集群中的进程，它是通过monmap来连接的彼此的，这样设计的原因时避免了配置文件出现编写错误时导致集群失败，并且配置文件无法跟随集群的变化而实时更新以及保存更新之前的monmap，这些都是由分布式一致性算法paxos实现的；

#### monitor数据同步
> 在生产环境中一般存在多个monitor,每个monitor都会周期性的查看是否其他的monitor已经生成了新的cluster map.在cluster map发生变化的时候，总会存在有些monitor的cluster map的版本落后于其他的monitor,这时版本落后的这些monitor会被剔除quorum,这类monitor被称为requestor,它会重新向leader请求最新的cluster map,leader会将cluster map同步的任务分配给provider,该provider将提供给requester最新的cluster map,在monitor map中，如图2所示；会存在如下的三类monitor:
> - Leader:最先生产cluster map并被选举成功；
> - Provider:拥有最新的cluster map,但不是最新生成的；
> - Requester:该类monitor已被剔除了quorum,并向Leader请求最新的cluster map后重新加入quorum;
 
> Monitor map的数据更新严重依赖时间戳，monitor服务器之间的时间不同步会导致很多集群的很多异常问题，比如丢弃同步数据，同步超时等，须在每台服务器上安装ntp服务用于时间同步；

## 集群图
#### mon map
```SHELL
$ ceph mon dump
dumped monmap epoch 1
epoch 1 # mon map版本号
fsid 88e67289-315a-44ff-9617-fe898479064f # ceph集群唯一标识，可以使用ceph fsid命令打印
last_changed 2019-01-27 10:14:25.218559 # mon map的最后修改时间
created 2019-01-27 10:14:25.218559 # mon map的创建时间
0: 10.0.4.5:6789/0 mon.mon01 # mon map的monitor成员
```

#### osd map
```SHELL
$ ceph osd dump
epoch 26 # osd map版本号
fsid 88e67289-315a-44ff-9617-fe898479064f # ceph集群唯一标识
created 2019-01-27 10:14:26.130387 # osd map的创建时间
modified 2019-05-11 04:24:37.495505 # osd map的最后修改时间
flags sortbitwise,recovery_deletes,purged_snapdirs
crush_version 7 # crush map的版本号
full_ratio 0.95 # ceph集群的容量，这个比例是当前ceph所占的存储空间到达所有磁盘空间的95%时，为了避免数据的丢失，集群会组成客户端所有的读写操作；
backfillfull_ratio 0.9
nearfull_ratio 0.85 # Ceph集群容量接近full的警告值
require_min_compat_client jewel # 客户端版本要求
min_compat_client jewel # 客户端版本要求
require_osd_release mimic # osd的发布版本 要求
max_osd 3 # osd的数量
# ceph集群中存储池信息；1-存储池id,
pool 1 'test' replicated size 3 min_size 2 crush_rule 0 object_hash rjenkins pg_num 10 pgp_num 10 last_change 27 flags hashpspool stripe_width 0
# osd列表
osd.0 up   in  weight 1 up_from 23 up_thru 0 down_at 20 last_clean_interval [15,20) 10.0.4.6:6800/1529 10.0.4.6:6801/1529 10.0.4.6:6802/1529 10.0.4.6:6803/1529 exists,up c4847a97-621d-4c73-a015-f09ce46df51b
...
```

#### pg map
```SHELL
~$ ceph pg dump
dumped all
version 5719 # pg map版本号
stamp 2019-05-11 07:21:18.562753
last_osdmap_epoch 0
last_pg_scan 0
PG_STAT OBJECTS MISSING_ON_PRIMARY DEGRADED MISPLACED UNFOUND BYTES LOG DISK_LOG STATE        STATE_STAMP                VERSION REPORTED UP      UP_PRIMARY ACTING  ACTING_PRIMARY LAST_SCRUB SCRUB_STAMP                LAST_DEEP_SCRUB DEEP_SCRUB_STAMP           SNAPTRIMQ_LEN
1.5           0                  0        0         0       0     0   0        0 active+clean 2019-05-11 05:27:01.868161     0'0    34:16 [2,0,1]          2 [2,0,1]              2        0'0 2019-05-11 05:26:59.945465             0'0 2019-05-11 05:26:59.945465             0

1 0 0 0 0 0 0 0 0

# osd 上的pg统计
sum 0 0 0 0 0 0 0 0
OSD_STAT USED    AVAIL   TOTAL   HB_PEERS PG_SUM PRIMARY_PG_SUM
1        1.0 GiB  49 GiB  50 GiB    [0,2]     10              5
0        1.0 GiB  49 GiB  50 GiB    [1,2]     10              1
2        1.0 GiB  49 GiB  50 GiB    [0,1]     10              4
sum      3.0 GiB 147 GiB 150 GiB
```

#### crush map
```SHELL
$ ceph osd crush dump
# 或
$ ceph osd getcrushmap -o crush.data
$ crushtool -d crush.data -o crush.map
$ cat crush.map
# begin crush map
tunable choose_local_tries 0
tunable choose_local_fallback_tries 0
tunable choose_total_tries 50
tunable chooseleaf_descend_once 1
tunable chooseleaf_vary_r 1
tunable chooseleaf_stable 1
tunable straw_calc_version 1
tunable allowed_bucket_algs 54

# devices
device 0 osd.0 class hdd
device 1 osd.1 class hdd
device 2 osd.2 class hdd

# types 设备类别
type 0 osd
type 1 host
type 2 chassis
type 3 rack
type 4 row
type 5 pdu
type 6 pod
type 7 room
type 8 datacenter
type 9 region
type 10 root

# buckets
host osd01 {
        id -3           # do not change unnecessarily
        id -4 class hdd # do not change unnecessarily
        # weight 0.049
        alg straw2      # 数据选择算法为straw2
        hash 0          # rjenkins1
        item osd.0 weight 0.049
}
...
root default {
        id -1           # do not change unnecessarily
        id -2 class hdd # do not change unnecessarily
        # weight 0.146
        alg straw2      # 数据选择算法为straw2
        hash 0          # rjenkins1
        item osd01 weight 0.049
        item osd02 weight 0.049
        item osd03 weight 0.049
}

# rules
rule replicated_rule {
        id 0
        type replicated
        min_size 1
        max_size 10
        step take default
        step chooseleaf firstn 0 type host
        step emit
}

# end crush map
```

#### fs map
```SHELL
$ ceph fs dump
dumped fsmap epoch 1
e1
enable_multiple, ever_enabled_multiple: 0,0
compat: compat={},rocompat={},incompat={1=base v0.20,2=client writeable ranges,3=default file layouts on dirs,4=dir inode in separate object,5=mds uses versioned encoding,6=dirfrag is stored in omap,8=no anchor table,9=file layout v2,10=snaprealm v2}
legacy client fscid: -1

No filesystems configured
```

## 操作
#### 初始化mon
```SHELL
ceph-authtool --create-keyring  /tmp/ceph.mon.keyring --gen-key -n mon. –cap mon ‘allow *’ #该用户是ceph集群的第一个用户，用于monitor之间的互联密钥以及创建后续的一些用户；

sudo ceph-authtool --create-keyring /etc/ceph/ceph.client.admin.keyring --gen-key -n client.amdin --set-uid=0 --cap mon ‘allow *’ --cap osd ‘sllow *’ --cap mds ‘allow *’  --cap mgr ‘allow *’   # 创建能够访问所有集群进程的用户

sudo ceph-authtool --create-keyring /var/lib/ceph/bootstrap-osd/ceph.keyring --gen-key -n client.bootstrap-osd --cap mon ‘profile bootstrap-osd’ #创建用于启动osd的用户

sudo ceph-authtool /tmp/ceph.mon.keyring –import-keyring /etc/ceph/ceph.client.admin.keyring

sudo ceph-authtool /tmp/ceph.mon.keyring –import-keyring /var/lib/ceph/bootstrap-osd/ceph.keyring #将keyring导入到ceph.mon.keyring

monmaptool --create --add {hostname} {ip-address} --fsid {uuid} /tmp/monmap #创建一个monmap，用于添加到quorum中
```
#### 增加mon
```SHELL
sudo mkdir /var/lib/ceph/mon/ceph-{mon-id} #创建monitor的数据目录

ceph auth get mon. -o /tmp/{keyring-file} #到处mon.用户的keyring

ceph mon getmap -o /tmp/{monmap-file} #获取mon map数据

sudo ceph-mon -i {mon-id} --mkfs --monmap /tmp/{monmap-file} --keyring /tmp/{keyring-file} #使用monmap和keyring初始化monitor 数据目录
```
#### 删除mon
```SHELL
service ceph –a stop mon.{mon_id} # 关闭ceph mon进程

ceph mon remove {mon-id} # 移除集群中的monitor
```
