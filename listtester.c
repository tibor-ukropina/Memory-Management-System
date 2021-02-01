#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "ds_memory.h"
#include "ds_list.h"



int main(void){ 

    int i;

    ds_create("list.bin", 2048);

    ds_create_list();
    ds_init_list();

    ds_read_elements("elements.txt");

    i = ds_swap(18, 19);
    printf("The value of i is: %d\n", i);

    ds_finish_list();


    return 0;
}
