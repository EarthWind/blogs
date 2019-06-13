# MDS状态
> 在使用CephFS中MDS会存在多种状态
## 状态描述
#### 一般状态
```SHELL
up:active
```
> 这是MDS的基本状态，表示MDS当前是正常可用的，可以为客户端提供正常的文件系统服务；
```SHELL
up:standby
```
> MDS处于备份模式，在主OSD出现问题的时候，为变为主OSD；一般在Monitor会将恢复正常的OSD设置为该状态；这种模式下不会同步主OSD的数据，所以在集群出现问题时恢复的时间会比较长；
```SHELL
up:standby_replay
```
> 备MDS会同步主MDS的日志，这样在主OSD出现问题时备份MDS可以更快的重放日志，然后称为主OSD；缺点是该OSD只能取代跟随的MDS，当其他的MDS出现问题时，它不能进行取代；

#### 其他状态
```SHELL
up:boot
```
> 在启动期间该状态会广播给Monitor，该状态不会出现在MDS集群中，因为会立即赋予MDS一个rank或将MDS设置为备份模式；
```SHELL
up:creating
```
> 创建新的rank让后加入MDS集群
```SHELL
up:starting
```
> MDS在启动一个停止的rank
```SHELL
up:stopping
```
> 当一个rank停止的时候，Monitor会命令MDS进入该状态；MDS不会接收新的客户端连接，将文件系统的子树合并都其他的rank，将元素据日志刷入磁盘，如果rank为0，则会关闭所有客户端的连接；
```SHELL
up:replay
```
> 该状态代表一个备MDS正在取代主MDS并且正在恢复日志；
```SHELL
up:resolve
```

```SHELL
up:reconnect
```
> MDS可能从replay或resolve状态进入该状态，此状态是请求客户端重新连接，任何和MDS存在session的客户端都需要在mds_reconnect_timeout时间内重新连接；
```SHELL
up:rejoin
```
```SHELL
up:clientreplay
```

#### 失败状态
```SHELL
down:failed
```

