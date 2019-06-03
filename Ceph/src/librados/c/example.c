#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rados/librados.h>

int main (int argc, const char **argv) {
    rados_t cluster;
    char cluster_name[] = "ceph";
    char user_name[] = "client.admin";
    uint64_t flags = 0;

    int err;
    err = rados_create2(&cluster, cluster_name, user_name, flags);
    if (err < 0) {
        fprintf(stderr, "%s: couldn't create the cluster handle! %s\n", 
        argv[0], strerror(-err));
        exit(EXIT_FAILURE);
    } else {
        printf("\nCreated a cluster handle.\n");
    }

    err = rados_conf_read_file(cluster, "/etc/ceph/ceph.conf");
    if (err < 0) {
        fprintf(stderr, "%s: can't read config file: %s\n",
        argv[0], strerror(-err));
        exit(EXIT_FAILURE);
    } else {
        printf("\nRead the config file.\n");
    }

    err = rados_conf_parse_argv(cluster, argc, argv);
    if (err < 0){
        fprintf(stderr, "%s: can't parse command line arguments: %s\n",
        argv[0], strerror(-err));
        exit(EXIT_FAILURE);
    } else {
        printf("\nRead the command line arguments.\n");
    }

    err = rados_connect(cluster);
    if (err < 0){
        fprintf(stderr, "%s: con't connect to cluster: %s\n",
        argv[0], strerror(-err));
        exit(EXIT_FAILURE);
    } else {
        printf("\nConnected to the cluster.\n");
    }

    rados_shutdown(cluster);
    exit(EXIT_SUCCESS);
}

