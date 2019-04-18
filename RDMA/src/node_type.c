#include <stdio.h>
#include <stdlib.h>
#include <infiniband/verbs.h>

int main(int argc, char *argv[]){
        struct  ibv_device **devices, dev;
        int     num_devices;
        int     i;

        devices = ibv_get_device_list(&num_devices);
        if(!devices){
                perror("failed to get IB device list");
                exit(EXIT_FAILURE);
        }
        for(i = 0; i < num_devices; i++){
                printf("%s node type\n", ibv_get_device_name(devices[i]));
                printf("%s\n", ibv_node_type_str(devices[i] -> node_type));
        }

        ibv_free_device_list(devices);
        exit(EXIT_SUCCESS);
}
