# LIBRADOS(C)
`rados_create()`->`rados_conf_read_file()`|`rados_conf_parse_argv()`|`rados_conf_parse_env()`->`rados_connect()`->`rados_ioctx_create()`->`rados_write_full()`->`rados_ioctx_destroy()`->`rados_shutdown()`

## 步骤
1. 创建一个用于和集群交互的handle,保存连接信息于rados_t的结构体中；
> - `int rados_create(rados_t * cluster, const char *const id)`
> - `int rados_create2(rados_t * pcluster, const char *const clustername, const char *const name, uint64_t flags)`
> - `int rados_create_with_context(rados_t * cluster, rados_config_t cct)`
```C
int err;
rados_t cluster;

err = rados_create(&cluster, NULL);
if (err < 0) {
        fprintf(stderr, "%s: cannot create a cluster handle: %s\n", argv[0], strerror(-err));
        exit(1);
}
```

2. 读取配置文件、命令参数或环境变量配置handle，用于连接集群
> - `int rados_conf_set(rados_t cluster, const char * option, const char * value)`
> - `int rados_conf_get(rados_t cluster, const char * option, char * buf, size_t len)`
> - `int rados_conf_read_file(rados_t cluster, const char * path)`
> - `int rados_conf_parse_argv(rados_t cluster, int argc, const char ** argv)`
> - `int rados_conf_parse_argv_remainder(rados_t cluster, int argc, const char ** argv, const char ** remargv)`
> - `int rados_conf_parse_env(rados_t cluster, const char * var)`
```C
err = rados_conf_read_file(cluster, "/path/to/myceph.conf");
if (err < 0) {
        fprintf(stderr, "%s: cannot read config file: %s\n", argv[0], strerror(-err));
        exit(1);
}
```

3. 一旦handle配置成功后就可以连接到集群中
> - `int rados_connect(rados_t cluster)`
```C
err = rados_connect(cluster);
if (err < 0) {
        fprintf(stderr, "%s: cannot connect to cluster: %s\n", argv[0], strerror(-err));
        exit(1);
}
```

4. 创建IO上下文，用于对ceph集群进行IO操作
> - `int rados_ioctx_create(rados_t cluster, const char * pool_name, rados_ioctx_t * ioctx)`
> - `int rados_ioctx_create2(rados_t cluster, int64_t pool_id, rados_ioctx_t * ioctx)`
```C
rados_ioctx_t io;
char *poolname = "mypool";

err = rados_ioctx_create(cluster, poolname, &io);
if (err < 0) {
        fprintf(stderr, "%s: cannot open rados pool %s: %s\n", argv[0], poolname, strerror(-err));
        rados_shutdown(cluster);
        exit(1);
}
```

5. 对集群进行读写操作
> - `int rados_write_full(rados_ioctx_t io, const char * oid, const char * buf, size_t len)`
> - `int rados_write(rados_ioctx_t io, const char * oid, const char * buf, size_t len, uint64_t off)`
> - `int rados_writesame(rados_ioctx_t io, const char * oid, const char * buf, size_t data_len, size_t write_len, uint64_t off)`
> - `CEPH_RADOS_API int rados_append(rados_ioctx_t io, const char * oid, const char * buf, size_t len)`
> - `int rados_read(rados_ioctx_t io, const char * oid, char * buf, size_t len, uint64_t off)`
```C
err = rados_write_full(io, "greeting", "hello", 5);
if (err < 0) {
        fprintf(stderr, "%s: cannot write pool %s: %s\n", argv[0], poolname, strerror(-err));
        rados_ioctx_destroy(io);
        rados_shutdown(cluster);
        exit(1);
}
```

6. 关闭IO上下文并关闭客户端和集群的连接
```C
rados_ioctx_destroy(io);
rados_shutdown(cluster);
```

## 异步IO
> - `int rados_aio_write(rados_ioctx_t io, const char * oid, rados_completion_t completion, const char * buf, size_t len, uint64_t off)`
> - `int rados_aio_append(rados_ioctx_t io, const char * oid, rados_completion_t completion, const char * buf, size_t len)`
> - `int rados_aio_write_full(rados_ioctx_t io, const char * oid, rados_completion_t completion, const char * buf, size_t len)`
> - `int rados_aio_writesame(rados_ioctx_t io, const char * oid, rados_completion_t completion, const char * buf, size_t data_len, size_t write_len, uint64_t off)`
> - `int rados_aio_remove(rados_ioctx_t io, const char * oid, rados_completion_t completion)`
> - `int rados_aio_read(rados_ioctx_t io, const char * oid, rados_completion_t completion, char * buf, size_t len, uint64_t off)`
> - `int rados_aio_flush(rados_ioctx_t io)`
> - `int rados_aio_flush_async(rados_ioctx_t io, rados_completion_t completion)`
> - `int rados_aio_stat(rados_ioctx_t io, const char * o, rados_completion_t completion, uint64_t * psize, time_t * pmtime)`
> - `int rados_aio_cmpext(rados_ioctx_t io, const char * o, rados_completion_t completion, const char * cmp_buf, size_t cmp_len, uint64_t off)`
> - `int rados_aio_cancel(rados_ioctx_t io, rados_completion_t completion)`
> - `int rados_aio_exec(rados_ioctx_t io, const char * o, rados_completion_t completion, const char * cls, const char * method, const char * in_buf, size_t in_len, char * buf, size_t out_len)`




