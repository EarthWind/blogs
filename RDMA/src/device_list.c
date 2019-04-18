#include <stdio.h>
#include <stdlib.h>
#include <endian.h>
#include <infiniband/verbs.h>


int main(int argc, char *argv[]){
        struct  ibv_device **dev_list;
        int     num_devices, i;

        dev_list = ibv_get_device_list(&num_devices);
        if(!dev_list){
                perror("Failed to get IB device list");
                exit(EXIT_FAILURE);
        }

        printf("        %-16s\tnode GUID\n", "device");
        printf("        %-16s\t-------------------\n", "--------");
 
        for(i = 0; i < num_devices; i++){
                printf("        %-16s\t%016llx\n",
                                ibv_get_device_name(dev_list[i]),
                                (unsigned long long)be64toh(ibv_get_device_guid(dev_list[i])));
                printf("        name\t\t\t%-16s\n", dev_list[i] -> name);
                printf("        dev_name\t\t%-16s\n", dev_list[i] -> dev_name);
                printf("        dev_path\t\t%-16s\n", dev_list[i] -> dev_path);
                printf("        ivdev_path\t\t%-16s\n", dev_list[i] -> ibdev_path);
        }

        ibv_free_device_list(dev_list);

        exit(EXIT_SUCCESS);
}
