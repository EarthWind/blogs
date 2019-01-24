# Shadowsocks服务器搭建
#### 1.0 注册VPS
精力有限啊，对购买和使用VPS经验较少，不敢过多置评那家好，国内国外优缺点相信用过就会有体会了，在这里对推荐[VULTR](https://www.vultr.com/?ref=7599093 "VULTR")，全球16个机房，亚洲有新加坡和日本可选，最赞的地方是支持使用支付宝，另外新用户还赠送10美元进行体验，本次分享也是使用VULTR的VPS，配置为1核、512MB内存、20GB ssd盘和500GB带宽，如果有兴趣欢迎使用上面的链接进行注册；这是相应的部分价格表：

#### 2.0 部署VPS
登陆成功进入管理界面，
1. 点击新增服务器。
2. 选择机房，这里我们选在新加坡。
3. 选择操作系统。
4. 选择套餐，如果没有其他需求，推荐3.5美元的套餐，含有独立IP。
5. 选择Deploy后等待部署完成。
6. 服务器部署完成后可以查看服务器IP地址以及用户名密码。
7. 登入系统，推荐使用[PuTTY](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html "PuTTY")进行进行登陆；

#### 3.0 部署shadowsocks服务器
使用root登陆系统后，可以选择创建新用户进行服务器搭建，但是也需要sudo权限，所以可以直接使用root用户进行安装:
```
wget https://raw.githubusercontent.com/EarthWind/shadowsocks/master/shadowsocks.sh
chmod u+x shadowsocks.sh
./shadowsocks.sh
```
执行脚本后会有以下的以下交互：
- 配置密码

- 配置shadowsocks端口

- 选择加密方式，可以默认

- 等待依赖包下载安装完成后显示enjoy it.密码端口等信息保持在/etc/shadowsocks.json文件中，忘记的时候可以查看该文件；

启动shadowsocks:
```
systemctl start shadowsocks
systemctl status shadowsocks
	
```

#### 4.0 配置服务器
服务器的配置主要是打开shadowsocks的防火墙端口：
```
systemctl status firewalld
● firewalld.service - firewalld - dynamic firewall daemon
   Loaded: loaded (/usr/lib/systemd/system/firewalld.service; enabled; vendor preset: enabled)
   Active: active (running) since Thu 2019-01-17 15:20:13 UTC; 29min ago
     Docs: man:firewalld(1)
 Main PID: 1725 (firewalld)
   CGroup: /system.slice/firewalld.service
           └─1725 /usr/bin/python -Es /usr/sbin/firewalld --nofork --nopid

Jan 17 15:20:11 guest systemd[1]: Starting firewalld - dynamic firewall daemon...
Jan 17 15:20:13 guest systemd[1]: Started firewalld - dynamic firewall daemon.

#打开端口
firewall-cmd --zone=public --add-port=shadowsocks-port/tcp --permanent
firewall-cmd --reload
#查询
 firewall-cmd --zone=public --query-port=shadowsocks/tcp
```

#### 5.0 windos客户端配置
从[Github](https://github.com/shadowsocks/shadowsocks-windows/releases "Shadowsocks Win Release")下载Shadowsocks的windows客户端；下载完成后解压缩zip包直接双击Shadowsocks.exe运行，如下图，填写相应的信息后点击OK：

**[GOOGLE](https://www.google.com/ "GOOGLE")**测试


#### 6.0 android客户端配置
从[Github](https://github.com/shadowsocks/shadowsocks-android/releases "Shadowsoks Andorid Release")下载APP，配置和windows客户端基本一致；

#### 7.0 Linux客户端
- Centos
请参考[CentOS 7 安装 shadowsocks 客户端](https://brickyang.github.io/2017/01/14/CentOS-7-%E5%AE%89%E8%A3%85-Shadowsocks-%E5%AE%A2%E6%88%B7%E7%AB%AF/ "CentOS 7 安装 shadowsocks 客户端")

- Ubuntu
请参考[各种系统下Shadowsocks客户端的安装与配置](http://www.jeyzhang.com/how-to-install-and-setup-shadowsocks-client-in-different-os.html "各种系统下Shadowsocks客户端的安装与配置")
