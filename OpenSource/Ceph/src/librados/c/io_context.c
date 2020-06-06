#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rados/librados.h>

int main(int argc, const char **argv){
    rados_t cluster;
    char cluster_name[] = "ceph";
    char user_name[] = "client.admin";
    uint64_t flags = 0;
    int err;
    rados_ioctx_t io;
    char poolname[] = "test";
    char xattr[] = "en_US";

    /*
     * 创建一个hanldle
     */
    err = rados_create2(&cluster, cluster_name, user_name, flags);
    if (err < 0) {
        fprintf(stderr, "%s: couldn't create the cluster handle! %s",
        argv[0], strerror(-err));
        exit(EXIT_FAILURE);
    } else {
        printf("Created a cluster handle!\n");
    }

    /*
     * 读取配置文件，至少要包含monitor的地址和密钥路径
     */
    err = rados_conf_read_file(cluster, "/etc/ceph/ceph.conf");
    if (err < 0) {
        fprintf(stderr, "%s: can't read configuration file: %s\n",
        argv[0], strerror(-err));
        exit(EXIT_FAILURE);
    } else {
        printf("Read the configuration file.\n");
    }

    /*
     * 通过命令行参数来获取客户端的配置信息
     */
    err = rados_conf_parse_argv(cluster, argc, argv);
    if (err < 0) {
        fprintf(stderr, "%s: can't parse command line arguments: %s\n",
        argv[0], strerror(-err));
        exit(EXIT_FAILURE);
    } else {
        printf("Read the command line arguments.\n");
    }

    /*
     * 连接到集群
     */
    err = rados_connect(cluster);
    if (err < 0) {
        fprintf(stderr, "%s: can't commnect to cluster: %s\n",
        argv[0], strerror(-err));
        exit(EXIT_FAILURE);
    } else {
        printf("Connected to the cluster.\n");
    }

    /*
     * 创建IO上下文
     */
    err = rados_ioctx_create(cluster, poolname, &io);
    if (err < 0) {
        fprintf(stderr, "%s: can't open rados pool %s : %s\n",
        argv[0], poolname, strerror(-err));
        exit(EXIT_FAILURE);
    } else {
        printf("Created I/O context.\n");   
    }

    /*
     * 通过IO上下文，写入对象到集群中
     */
    err = rados_write(io, "hw", "Hello World!", 12, 0);
    if (err < 0) {
        fprintf(stderr, "%s: cann't write object \"hw\" to pool %s\n",
        argv[0], poolname);
        rados_ioctx_destroy(io);
        rados_shutdown(cluster);
        exit(EXIT_FAILURE);
    } else {
        printf("Wrote \"Hello World\" to object \"hw\".\n");
    }

    /*
     * 设置对象的属性
     */
    err = rados_setxattr(io, "hw", "lang", xattr, 5);
    if (err < 0) {
        fprintf(stderr, "%s: cann't write xattr to pool %s: %s\n",
        argv[0], poolname, strerror(-err));
        rados_ioctx_destroy(io);
        rados_shutdown(cluster);
        exit(EXIT_FAILURE);
    } else {
        printf("Wrote \"en_US\" to xattr \"lang\" for object \"hw\".\n");
    }

    /*
     * 创建异步回调
     */
    rados_completion_t comp;
    err = rados_aio_create_completion(NULL, NULL, NULL, &comp);
    if (err < 0) {
        fprintf(stderr, "%s: Couldn't create aio completion: %s\n", 
        argv[0], strerror(-err));
        rados_ioctx_destroy(io);
        rados_shutdown(cluster);
        exit(EXIT_FAILURE);
    } else {
        printf("Created AIO completion.\n");
    }

    /*
     * 异步读取对象
     */
    char read_res[100];
    err = rados_aio_read(io, "hw", comp, read_res, 12, 0);
    if (err < 0) {
        fprintf(stderr, "%s: cann't read object.%s %s\n",
        argv[0], poolname, strerror(-err));
        rados_ioctx_destroy(io);
        rados_shutdown(cluster);
        exit(EXIT_FAILURE);
    }
    rados_aio_wait_for_complete(comp); // 阻塞等待读取结束
    rados_aio_release(comp);           // 释放回调
    // 注意在回调成功之后read_res中才会有内容
    printf("Read object \"hw\".The contents are:\n %s \n", read_res);

    /*
     * 获取对象属性
     */
    char xattr_res[100];
    err = rados_getxattr(io, "hw", "lang", xattr_res, 5);
    if (err < 0) {
        fprintf(stderr, "%s: cann't read xattr. %s %s\n",
        argv[0], poolname, strerror(-err));
        rados_ioctx_destroy(io);
        rados_shutdown(cluster);
        exit(EXIT_FAILURE);
    } else {
        printf("Read xattr \"lang\" for object \"hw\".The contents are: \n %s \n",
        xattr_res);
    }

    /*
     * 移除对象属性
     */
    err = rados_rmxattr(io, "hw", "lang");
    if (err < 0) {
        fprintf(stderr, "%s: cann't remove xattr. %s %s\n", 
        argv[0], poolname, strerror(-err));
        rados_ioctx_destroy(io);
        rados_shutdown(cluster);
        exit(EXIT_FAILURE);
    } else {
        printf("Removed xattr \"lang\" for object \"hw\".\n");
    }

    /*
     * 从集群中移除对象
     */
    err = rados_remove(io, "hw");
    if (err < 0) {
        fprintf(stderr, "%s: can't remove object. %s %s\n",
        argv[0], poolname, strerror(-err));
        rados_ioctx_destroy(io);
        rados_shutdown(cluster);
        exit(EXIT_FAILURE);
    } else {
        printf("Removed object\"hw\".\n");
    }

    exit(EXIT_SUCCESS);
}

