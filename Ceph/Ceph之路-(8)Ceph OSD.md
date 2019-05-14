# Ceph OSD
## 简介
> Ceph OSD主要用来存储数据，通常来说，一个OSD进程对应了单个的存储设备，比如传统的HDD，或则SSD，也有列外情况，比如在生产环境下，为了增加集群性能，一个OSD中使用HDD作为数据的存储，SSD作为元数据的存储，这样的设备搭配使用也是可以的。OSD的后端存储可以使用bluestore,filestore以及kvstore等；
>
> 为了保证OSD中各个副本对象的内容的一致性，OSD提供了数据清洗功能，轻度清洗主要对比副本的大小和属性是否一致，深度清洗会读取数据并使用校验和来验证数据的完整性；

## 基本操作
#### 新增OSD
```SHELL
# Filestore
sudo mkdir /var/lib/ceph/osd/${clusterid}-${id} # 创建osd的目录

sudo mkfs -t {fstype} /dev/{drive} # 格式化磁盘

sudo mount -o user_xattr /dev/{drive} /var/lib/ceph/osd/${cluster}-${id} # 挂在到osd目录

ceph-osd -i {osd-num} --mkfs –mkkey # 初始化osd 目录

ceph auth add osd.{osd-num} osd “allow *”  mon “allow rwx” -i /var/lib/ceph/osd/${cluster}-${id}/keyring # 创建cephx认证

ceph osd crush add {id-or-name} {weight} [{bucket-type}={bucket-name}] # 添加osd到crush map中

# Bluestore
ceph-volume lvm zap /dev/drive

ceph-volume lvm prepare --osd-id {id} --data /dev/{drive}

ceph-volume lvm activate {id} {fsid}
# 或
ceph-volume lvm create --osd-id {id} --data /dev/{drive}

sudo systemctl start ceph-osd@{osd_name}
```

#### 删除OSD
```SHELL
ceph osd out {osd-num}
# 如何在标记OSD为osd的过程中pg的状态一直卡在了active+remapped那么可用下面的方法；
ceph osd in {osd-num}
ceph osd crush reweight osd.{osd-num} 0

# 停止OSD进程
sudo systemcel stop ceph-osd@{osd-num}

# 移除集群中的OSD信息
ceph osd purge {id} --yes-i-really-really-mean-it
edit /etc/ceph.conf # 删除配置文件中的osd信息
```


