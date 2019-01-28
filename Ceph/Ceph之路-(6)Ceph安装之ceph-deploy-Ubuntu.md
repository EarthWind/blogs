# Ubuntu18.04 ceph-depoy安装Ceph
#### 1. 配置环境
Ubuntu和Centos上安装Ceph大同小异，主要是配置处不同：
###### 1.1 准备环境
可以使用Vmware或Virtualbox准备如下的环境，admin节点，monitor节点和3个osd节点，mgr和monitor安装在同一个节点上：


[![cluster-architecture](http://bananahub.blog/static/upload/blog/system-arch2019-01-26-10-21-14.png "cluster-architecture")](http://bananahub.blog/static/upload/blog/system-arch2019-01-26-10-21-14.png "cluster-architecture")
-----

| hostname | ip address | device |
| :------- | :--------- | :----- |
| admin    | 10.0.4.4   | any |
| mon01    | 10.0.4.5   | any |
| osd01    | 10.0.4.6   | /dev/sdb 50GB |
| osd02    | 10.0.4.7   | /dev/sdb 50GB |
| osd03    | 10.0.4.8   | /dev/sdb 50GB |

###### 1.2 安装ceph-deploy
admin节点:

```shell
#增加用于配置的用户，注意不要创建名为ceph的用户
# useradd -d /home/yang -m -s /bin/bash yang
# passwd yang

#新增用户无密钥sudo
# apt install sudo
# vim /etc/sudoers.d/yang
    yang ALL=(ALL) NOPASSWD:ALL
# chmod 0400 /etc/sudoers.d/yang

#安装pip
$ sudo apt install python python-pip
#或
$ curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
$ sudo python get-pip.py

# 安装ceph-deploy
$ sudo pip install ceph-deploy
```

###### 1.3 配置ceph个节点
monnitor和osd节点：
```shell
#增加安装ceph的用户并配置无密钥sudo
# useradd -d /home/yang -m -s /bin/bash yang
# passwd yang
# cat /etc/sudoers.d/yang
    yang ALL=(ALL) NOPASSWD:ALL
# chmod 0400 /etc/sudoers.d/yang

#安装ssh
$ sudo apt install openssh-server
$ sudo systemctl start sshd
$ sudo systemctl status sshd

#安装python
$ sudo apt install python2

#安装ntp
$ sudo apt install ntp

#防火墙配置
#关闭防火墙
$ sudo ufw status
$ sudo ufw disable
$ sudo systemctl stop ufw

#关闭selinux
$ sudo setenforce 0
```

###### 1.4 admin节点配置
admin节点：
```shell
#hosts解析配置
$ sudo cat /etc/hosts
	10.0.4.4        admin
	10.0.4.5        mon01
	10.0.4.6        osd01
	10.0.4.7        osd02
	10.0.4.8        osd03

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

接下来的配置请参考上一篇博客(从第二部分开始)： [Ceph之路-(5)Ceph安装之ceph-deploy-Centos](http://bananahub.blog/blog/69c36e54-2154-11e9-bd19-560001d681ef/ "Ceph之路-(5)Ceph安装之ceph-deploy-Centos")

#### 2. 附录
###### 2.1 参考
[1]. [Ceph document](http://docs.ceph.com/docs/master/start/ "Ceph document")
