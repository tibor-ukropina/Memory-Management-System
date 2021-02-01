#include <stdio.h>
#include <stdlib.h>
#include "ds_memory.h"
#include "ds_array.h"


int main(void){

    int i;

    i = ds_create("array.bin", 2048);
    i = ds_create_array();
    i = ds_init_array();

    ds_read_elements("elements.txt");

    
    
    printf("i: %d\n", i);

    

    i = ds_finish_array();

    show_array();
}
