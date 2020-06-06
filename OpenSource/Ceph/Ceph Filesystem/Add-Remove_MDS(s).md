# 添加和删除元数据服务器
> 当在Ceph集群中时文件系统CephFS服务时，至少需要部署一个元数据服务器(MDS);

## 添加元数据服务器
1. 创建mds的数据挂载点/var/lib/ceph/mds/ceph-{$id}.

2. 编辑ceph.conf的[MDS]段
```SHELL
[mds.{$id}]
host = {hostname}
```

3. 如果使用CephX，创建认证密钥
```SHELL
sudo ceph auth get-or-create mds.{$id} mon 'profile mds' mgr 'profile mds' mds 'allow *' osd 'allow *' > /var/lib/ceph/mds/ceph-{$id}/keyring 
```

4. 启动服务
```SHELL
sudo service ceph start mds.{$id}
```

5. 使用`ceph -s`命令时显示包含如下
```SHELL
mds: cephfs_a-1/1/1 up  {0=c=up:active}, 3 up:standby
```

## 删除元数据服务器
> 如果你想移除集群中的元数据服务器，可以使用下面的方法，
1. 使用上面的方法建立一个新的元数据服务器；

2. 停止旧的元数据服务器然后使用新元数据服务器
```SHELL
ceph mds fail <mds name>
```

3. 删除旧元数据的数据挂载点目录/var/lib/ceph/mds/ceph-{$id}

## 参考
- [ADD/REMOVE METADATA SERVER](http://docs.ceph.com/docs/master/cephfs/add-remove-mds/)

