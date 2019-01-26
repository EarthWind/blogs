# Centos ceph-depoy安装Ceph
#### 1. 配置环境
###### 1.1 准备环境
可以使用Vmware或Virtualbox准备如下的环境，admin节点，monitor节点和3个osd节点，mgr和monitor安装在同一个节点上：


[![cluster-architecture](http://bananahub.blog/static/upload/blog/system-arch2019-01-26-10-21-14.png "cluster-architecture")](http://bananahub.blog/static/upload/blog/system-arch2019-01-26-10-21-14.png "cluster-architecture")
-----


| hostname | ip address | device |
| :------- | :--------- | :----- |
| admin    | 10.0.2.13  | any |
| mon01    | 10.0.2.14  | any |
| osd01    | 10.0.2.15  | /dev/sdb 50GB |
| osd02    | 10.0.2.16  | /dev/sdb 50GB |
| osd03    | 10.0.2.17  | /dev/sdb 50GB |

###### 1.2 安装ceph-deploy
admin节点:

```shell
#增加用于配置的用户，注意不要创建名为ceph的用户
# useradd yang
# passwd yang

#新增用户无密钥sudo
# vim /etc/sudoers.d/yang
    *yang ALL=(ALL) NOPASSWD:ALL*
# chmod 0400 /etc/sudoers.d/yang

#安装pip
$ curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
$ sudo python get-pip.py

# 安装ceph-deploy
$ sudo pip install ceph-deploy
```

###### 1.3 配置ceph个节点
monnitor和osd节点：
```shell
#增加安装ceph的用户并配置无密钥sudo
# useradd yang
# passwd yang
# cat /etc/sudoers.d/yang
    *yang ALL=(ALL) NOPASSWD:ALL*
# chmod 0400 /etc/sudoers.d/yang

#安装ssh
$ sudo yum install openssh-server
$ sudo systemctl start sshd
$ sudo systemctl status sshd

#安装ntp
$ sudo yum install ntp ntpdate ntp-doc

#防火墙配置
#关闭防火墙
$ sudo systemctl stop firewalld
$ sudo systemctl disable firewalld
#或开放服务
#monitor节点
$ sudo firewall-cmd --zone=public --add-service=ceph-mon --permanent
#osd节点和mds节点
$ sudo firewall-cmd --zone=public --add-service=ceph --permanent
$ sudo firewall-cmd --reload

#关闭selinux
$ sudo setenforce 0
```

###### 1.4 admin节点配置
admin节点：
```shell
#hosts解析配置
$ sudo cat /etc/hosts
	10.0.2.13       admin
	10.0.2.14       mon01
	10.0.2.15       osd01
	10.0.2.16       osd02
	10.0.2.17       osd03

#ssh无密配置
$ ssh-keygen
$ ssh-copy-id mon01
$ ssh-copy-id osd01
$ ssh-copy-id osd02
$ ssh-copy-id osd03

#官方推荐配置
#admin节点ssh到ceph节点时，需要使用--username指定ceph节点安装所使用的用户名称，使用下面的配置后就不需要每次都指定用户了
$ cat ~/.ssh/config
	Host mon01
		Hostname mon01
		User yang
	Host osd01
		Hostname osd01
		User yang
	Host osd02
		Hostname osd02
		User yang
	Host osd03
		Hostname osd03
		User yang
```

###### 1.5 注意点
> 确保网卡为自动启动，文件夹/etc/sysconfig/network-scripts下ifcfg-{iface}文件中有OBOOT=yes配置

> 将hostname解析为IP地址而不是127.0.0.1

#### 2. 安装ceph
###### 2.1 初始化目录
```shell
#admin节点
$ mkdir ceph
$ mkdir ceph
```

###### 2.2 安装ceph
- 新建集群
```shell
$ ceph-deploy new mon01
```

- 网络配置
```shell
#公用网络配置
$ echo public_network = 10.0.2.0/24 >> ceph.conf
#如果存在集群网络，则进行添加
$ echo cluster_network = {cluster-network/netmask}
```

- ipv6配置
```shell
#如果是ipv6环境，则添加该配置项
$ echo ms bind ipv6 = true >> ceph.conf
```

- 安装ceph软件包
```shell
$ ceph-deploy install mon01 osd01 osd02 osd03
```

- 初始化monitor
```shell
$ ceph-deploy mon create-initial
```

- 将admin key分发到需要执行命令的节点
```shell
$ ceph-deploy admin mon01 osd01 osd02 osd03
```

> 注意有可能传输过去的ceph.client.admin.kering文件需要修改权限
> `sudo chmod 644 /etc/ceph/ceph.client.admin.keyring`

- 部署mgr
```shell
$ ceph-deploy mgr create mon01
```

- 部署osd，默认使用bluestore
```shell
$ ceph-deploy osd create osd01 --data /dev/sdb
$ ceph-deploy osd create osd02 --data /dev/sdb
$ ceph-deploy osd create osd03 --data /dev/sdb
```

#### 3. ceph状态查询
登陆mon01节点：
```shell
$ ceph -s
  cluster:
    id:     cc2fc0f8-b7eb-4bc1-98ea-422858c467aa
    health: HEALTH_OK

  services:
    mon: 1 daemons, quorum mon01
    mgr: mon01(active)
    osd: 3 osds: 3 up, 3 in

  data:
    pools:   0 pools, 0 pgs
    objects: 0  objects, 0 B
    usage:   3.0 GiB used, 147 GiB / 150 GiB avail
    pgs:

$ ceph health
HEALTH_OK
```

到此，Ceph集群部署成功；

#### 4. 附录
###### 4.1 参考
[1]. [Ceph document](http://docs.ceph.com/docs/master/start/ "Ceph document")
