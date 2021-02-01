/* 
This file allows testing of the array API
*/
#include <stdio.h>
#include <stdlib.h>
#include "ds_memory.h"
#include "ds_array.h"


int main(void){

    int i;

    /* we chose to use array.bin to store our array*/
    i = ds_create("array.bin", 2048);
    i = ds_create_array();
    i = ds_init_array();

    /* we will read numbers from the file elements.txt*/
    ds_read_elements("elements.txt");

    /* sample swap, here we are swapping elements at positions 2 and 5 */
    i = ds_swap(2, 5);

    /* sample delete, here we delete element from 6th position*/
    i = ds_delete(6);

    i = ds_finish_array();

    show_array();
}
