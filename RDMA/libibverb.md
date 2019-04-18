# LIBIBVERB
## 1. 获取设备信息
> - `struct ibv_device **ibv_get_device_list(int *num_devices);`
>
> 描述：获取系统上的infiniband设备列表，
>
> num_devices: 输出参数，返回设备的数量
>
> 返回：返回设备列表，程序结束时需要使用`ibv_free_device_list(struct ibv_device **list)`释放掉

> - `const char *ibv_get_device_name(struct ibv_device *device);`
>
> 描述：获取设备的设备名称，也可以通过`device -> name`获取设备名称
>
> device: 指定设备

> - `const char *ibv_node_type_str(enum ibv_node_type node_type);`
>
> 描述：将节点类型转换为字符串
>
> node_type: `device -> node_type`

> - `__be64 ibv_get_device_guid(struct ibv_device *device);`
>
> 描述：获取设备的guid
>
> device: 设备结构

> - `void ibv_free_device_list(struct ibv_device **list);`
>
> 描述：释放设备列表所占用的空间
>
> list: 指定设备列表，由`ibv_get_device_list(int *num_devices)`返回


