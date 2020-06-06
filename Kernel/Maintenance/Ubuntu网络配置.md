# 网络配置
## 静态IP配置
```SHELL
$ cat /etc/network/interfaces
auto enp0s8 # 开机自动挂载网卡
iface enp0s8 inet static # 静态分配IP，如果动态分配为dhcp
address 192.168.249.194
netmask 255.255.255.0
gateway 192.168.249.1
# dns-nameserver 119.29.29.29

$ sudo systemctl restart networking
```