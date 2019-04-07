# nvme-core
#### 源码解析
##### 1. 调用nvmf_host_default初始化nvmf_host

##### 2. 调用class_create
- 创建nvme-fabrics类

##### 3. 调用device_create创建设备

##### 4. 调用misc_register生成混杂设备
> nvmf_misc -> nvmf_dev_fops -> nvmf_dev_open,nvmf_dev_write,seq_read,nvmf_dev_release
- nvmf_dev_open

- nvmf_dev_write

- seq_read

- nvmf_dev_release

##### 5. 