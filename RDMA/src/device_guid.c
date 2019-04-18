#include <stdio.h>
#include <stdlib.h>
#include <endian.h>
#include <infiniband/verbs.h>

int main(int argc, char *argv[]){
        struct ibv_device **devices;
        int i, num_devices;

        devices = ibv_get_device_list(&num_devices);
        if(!devices){
                perror("failed to get IB device list");
                exit(EXIT_FAILURE);
        }
        for(i = 0; i< num_devices; i++){
                printf("%s node guid\n", ibv_get_device_name(devices[i]));
                printf("0x%llx\n", (unsigned long long)be64toh(ibv_get_device_guid(devices[i])));
        }

        ibv_free_device_list(devices);
        exit(EXIT_SUCCESS);
}
