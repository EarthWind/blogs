# nvme-core
#### 源码解析
1. 分配三个工作队列(workqueue)
- nvme-wq
- nvme-reset-wq
- nvme-delete-wq

2. 调用`alloc_chrdev_region`动态分配设备编号

3. 调用`class_create`创建设备类
- 创建类nvme:
- 创建类nvme-subsystem:

> note: 调用`class_create`和`class_register`可以将类注册到sysfs中，出现在文件/sys/class/目录下