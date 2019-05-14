# Ceph MGR
## 简介
Ceph Manager守护程序（ceph-mgr）与monitor守护程序一起运行，为外部监视和管理系统提供额外的监视和接口；在Ceph 12.x(luminous)的发布版本以上，ceph-mgr守护进程时必须的，没有该守护进程的集群会出现HEALTH_WARN警告，在11.x(kraken)及之前的版本中该组件时可选的

## 基本操作
#### 安装MGR
```SHELL
mkdir /var/lib/ceph/mgr/ceph-{ id } 

ceph auth get-or-create mgr.{ id } mon ‘allow profile mgr’ osd ‘allow *’ mds ‘allow *’ > /var/lib/ceph/mgr/ceph-{ id }/keyring

sudo ceph-mgr -i { id } --setuser ceph --setgroup ceph

sudo systemctl start ceph-mgr@{ id }
```
