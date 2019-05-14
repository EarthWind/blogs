# RBD
## 简介
> RBD(RADOS Block Device).即Ceph集群提供的块设备功能；Ceph块设备是轻量级的，可调大小的并且将数据条带化存储到多个OSD上，利用了RADOS提供的快照功能，多副本功能和一致性功能使得Ceph块设备具有高性能，高可用性等特性，它主要使用以下两种方式和Ceph集群进行交互：
> - Kernel rbd:建立好Ceph块设备后，将其映射到操作系统内核中，可像其他物理块设备一样进行格式化并挂载使用，它的设备文件格式为/dev/rbd*;
> - librbd:提供给依赖libvirt和QEMU的虚拟化软件如，OpenStack,CloudStack的后端块设备；

## 原理
#### 架构

#### 快照
> 快照是特定时间点跨设备的只读复制副本，Ceph还支持快照分层，这允许使用rbd命令或许多高层级的应用诸如QEMU,libvirt,OpenCloud和CloudStack快速地导出块设备；注意在对块设备打快照的时候，需要先关闭块设备的IO，以免打出的快照中缺少数据；如果块设备上有文件系统，可以使用fsfreeze命令关闭设备上的IO；
>
> Ceph支持COW(Copy-on-wright写拷贝)特性，在对块设备创建快照后，该快照是只读的，基于该快照，可以创建多个可写的克隆块设备，由于其是基于Parent快照的，所以Parent快照是不能删除，这会导致克隆的Child块设备数据丢失，所以需要对只读的Parent快照进行保护操作；克隆的镜像可以读取，写入，再克隆或重新设置大小，并且可以将一个池中的镜像克隆到另一个池中

#### 镜像
> RBD的Image可以在两个Ceph集群之间进行镜像同步，该功能可以在Jewel版本之后使用，镜像需要在每个集群中的池上进行配置，配置时可以指定需要镜像的Image或镜像该池中的所有Image,集群中的rbd-mirror进程负责将Image的变化发送到远程Image并将其进行恢复；
镜像有两种复制模式：
> - One-way Replication(单边复制)：数据仅从主集群同步到次集群，rbd-mirror进程仅在次集群上运行；
> - Two-way Replication(双边复制)：当数据从一个集群上的主Image同步到另一个集群上的次Image(反之亦可)，此时，双边都需要rbd-mirror进程的运行；

> 镜像有两种配置模式：
> - Pool(池镜像)：镜像池内的所有Image
> - Image(Image模式)：仅镜像指定的Image，该模式仅需要启动一个rbd-mirror进程，依赖与Image的日志特性journaling，需要将该特性进行使能；并且该特性依赖于exclusive-lock特性的使能

#### iSCSI网关
> iSCSI网关集成了Ceph存储和iSCSI标准用于提供高可用的iSCSI Target，它将RBD镜像导出为SCSI磁盘，iSCSI客户端可以通过TCP/IP网络发送命令到RBD镜像。
>
> 每一个iSCSI网关运行Linux IO(LIO)子系统来提供对iSCSI协议的支持，LIO利用用户空间直通(TCMU)与Ceph的librbd库进行交互，并将RBD镜像暴露给iSCSI客户端;

#### QEMU
> RBD最常用的场景时为虚拟机提供块设备，如下图是Ceph和QEMU相应的架构关系：
>
> QEMU默认使用的配置文件为/etc/ceph/$cluster.conf，默认使用的用户为client.admin。

#### libvirt
> RBD支持QEMU/KVM，所以Ceph可以作为和libvirt交互的虚拟管理组件的后端存储，libvirt常用的场景是为云解决方案如OpenStack或CloudStack提供Ceph块设备，它们使用libvirt和QEMU/KVM进行交互，QEME/KVM通过librbd和Ceph块设备交互，

#### RBD缓存
> 使用内核驱动驱动的RBD能够使用缓存页来提高性能，而用户空间实现(librbd)的RBD则不能使用缓存页的快速优势，所以RBD实现了自己的缓存机制---“RBD caching”，它和磁盘缓存的效果是一样的，当操作系统发送更新请求时，缓存中的所有脏数据都会写回到OSDs中，它使用LRU(最近最少使用)算法，并且在回写模式下它可以合并连续的请求以获取更高的吞吐量；

#### RBD REPLAY
> 是一系列捕获RBD负载的工具集合，如果需要使用则需要安装lttng-tools。并且librbd的版本在v0.87以上。

## 命令
#### 创建RBD
```SHELL
ceph osd pool create {pool-name} {pg-num} {pgp-num} # 如果在创建时没有指定pool-name,则默认会将rbd的数据存储在rbd池中；
rbd pool init {pool-name}
rbd create --size {megabytes} {pool-name}/{image-name} # 创建块设备，首先需要创建Pool, --size <M/G/T>，默认单位为M(1024)

# 默认情况下会使用admin用户来进行集群认证，admin用户拥有集群中的所有权限，所以可以创建新的用户用于rbd的操作
ceph auth get-or-create client.{ID} mon 'profile rbd' osd 'profile {profile name} [pool={pool-name}][, profile ...]'
```

#### 查询RBD
```SHELL
rbd ls {pool-name} 
rbd trash ls {pool-name}
rbd info {pool-name}/{image-name}
```

#### 删除RBD
```SHELL
rbd rm {pool-name}/{image-name}
rbd trash mv {pool-name}/{image-name}
rbd trash rm {pool-name}/{image-name}
```

#### 恢复RBD
```SHELL
rbd trash restore {image-id} # 恢复rbd pool的块设备
rbd trash resotre {pool-name}/{image-id } # 恢复其他数据池的块设备
rbd trash restore {pool-name}/{image-id }  --image {image-new-name} # 恢复镜像时可以重命名
```

#### 修改RBD的大小
```SHELL
rbd resize --size {megabytes} {pool-name}/{image-name} # 增加rbd的大小
rbd resize --size {megabytes} {pool-name}/{image-name} --allow-shrink # 减少rbd的大小
```

#### RBD内核映射
```SHELL
rbd list # 查看块设备
sudo rbd device map {pool-name}/{image-name} --id {user-name} --keyring {keyring-file} # 映射块设备到内核空间，映射时默认使用admin用户；
rbd device list # 查看设备的映射情况
sudo rbd device unmap /dev/rbd/{pool-name}/{image-name}
sudo rbd unmap {pool-name}/{image-name}
```

#### RBD快照
```SHELL
rbd snap create {pool-nam}/{image-name}@{snap-name}
rbd snap ls {pool-name}/{image-name}
rbd snap roolback {pool-name}/{image-name}@{snap-name} # 官方推荐使用克隆而不是恢复的方式来获得一个之前的块设备；因为它更为节省时间；
rbd snap  protect {pool-name}/{image-name}@{snap-name}
rbd snap rm {pool-name}/{image-name}@{snap-name}
rbd snap purge {pool-name}{image-name}
```

#### RBD克隆
```SHELL
rbd snap protect {pool-name}/{image-name}@{snap-name}
rbd clone {pool-name}/{image-name}@{snap-name} {pool-name}/{child-image-name}
rbd snap unprotect {pool-name}/{image-name}@{snap-name}
rbd children {pool-name}/{image-name}@{snapshot-name}
rbd flatten {pool-name}/{image-name}
```

#### RBD镜像
```SHELL
# 池模式
rbd mirror pool enable {pool-name} {mode} # 使能进行，mode为镜像模式，包括pool和image两种
rbd mirror pool disable {pool-name} 
rbd mirror pool peer add {pool-name} {client-name}@{cluster-name} # /etc/ceph/{cluster-name}.conf该配置文件查找远程的用户名，/etc/ceph/{cluster-name}.{client-name}.keyring该配置文件查找知道用用户cliet-name的kering;
rbd mirror pool peer remove {pool-name} {peer-uuid}

# Image模式
rbd feature enable {pool-name}/{image-name} {feature-name} # 需要使能journaling特性；
rbd mirror image enable {pool-name}/{image-name}
rbd mirror image disable {pool-name}/{image-name}

# 主备切换
rbd mirror image demote {pool-name}/{image-name} # 降级Image
rbd mirror pool demote {pool-name} # 降级池
rbd mirror image promote [--force] {pool-name}/{image-name} # 提升池
rbd mirror pool promote [--force] {pool-name}

# 强制同步
rbd mirror image resync {pool-name}/{image-name} # 当rbd-mirror进程检测到脑裂事件时，关联的镜像不会在进行同步，需要将主Image进行降级后然后在强制同步；
rbd mirror image status {pool-name}/{image-name} # 查看镜像的状态；
rbd mirror pool status {pool-name}

# rbd-mirror进程
ceph auth get-or-create client.rbd-mirror.{unique id} mon 'profile rbd-mirror' osd 'profile rbd'
systemctl enable ceph-rbd-mirror@rbd-mirror.{unique id}
rbd-mirror -f --log-file={log_path} # 指定前台运行
```
