#include <stdio.h>
#include <stdlib.h>
#include "ds_array.h"
#include "ds_memory.h"

void show_array(){

    int i;
    int c1;

    ds_init( "array.bin");
    
    for(i = 0; i < elements; ++i){
        ds_read( &c1, (i * sizeof(int)) + sizeof(elements), sizeof( c1 ));
        printf("%d  %d\n", i, c1);
    }

    printf("Elements: %ld\n", elements);
}

int ds_create_array(){

    int i = 0;
    i = ds_init("array.bin");

    if(i == 0){
        /* allocates memory for and store element tracker at start of file*/
        ds_malloc(sizeof(elements));
        ds_write(0, &elements, sizeof(long));
        /* allocates mememory for the array elements*/
        ds_malloc(sizeof(int) * MAX_ELEMENTS);
        ds_finish();

        return 0;
    }

    return -1;
}

int ds_init_array(){

    int i = 0;

    if(i == ds_init("array.bin")){
        /* store the value of elements into global variable*/
        ds_read(&elements, 0, sizeof(long));
        return 0;
    }

    return -1;
}

int ds_replace( int value, long index ){

    if(index > MAX_ELEMENTS || index < 0 || index > (elements-1)){
        return -1;
    }

    ds_write((index * sizeof(int)) + sizeof(elements), &value, sizeof(int));

    return 0;

}

int ds_insert( int value, long index ){

    int i, write, hold;

    /* make sure array is not full*/
    if(elements >= MAX_ELEMENTS ){
        return -1;
    }
    /* invalid index check*/
    if(index > MAX_ELEMENTS || index < 0 || index > elements){
        return -1;
    }
    /* if we are inserting to the end of the array*/
    if(elements == index || elements == 0){
        ds_write( (index * sizeof(int)) +sizeof(elements), &value, sizeof(int));
        elements++;
    }
    /* if we are inserting element not at the end of the array*/
    else{
        /* put value into memory location determined by index*/
        ds_read(&write, (index * sizeof(int)) + sizeof(elements), sizeof(int)); 
        ds_write((index * sizeof(int)) + sizeof(elements), &value, sizeof(int));
        elements++;

        /* shift all other elements of the array*/
        for(i = 1; i < elements; ++i){
            ds_read(&hold, ((index + i) * sizeof(int)) + sizeof(elements), sizeof(int));
            ds_write(((index + i) * sizeof(int)) + sizeof(elements), &write, sizeof(int));
            write = hold;

        }
    }
    return 0;
}

int ds_delete( long index ){

    int i;
    int next;

    /* invalid index check*/
    if(index > MAX_ELEMENTS || index < 0|| index > (elements-1)){
        return -1;
    }
    /* store value above index in temp to shift list to the left, remove item at index*/
    for( i = 0; i < elements; ++i){
        ds_read(&next, ((index + i + 1) * sizeof(int)) + sizeof(elements), sizeof(int));
        ds_write(((index + i) * sizeof(int)) + sizeof(elements), &next, sizeof(int));
    }
    elements--;

    return 0;
}

int ds_swap( long index1, long index2 ){

    int val1, val2;

    if(index1 == index2){
        return 0;
    }

    /* invalid index check*/
    if(index1 > MAX_ELEMENTS || index1 < 0|| index1 > (elements - 1)){
        return -1;
    }
    if(index2 > MAX_ELEMENTS || index2 < 0|| index2 > (elements - 1)){
        return -1;
    }

    /* hold previous values in temp variables then assign to eachother*/
    ds_read(&val1, (index1 * sizeof(int)) + sizeof(elements), sizeof(int));
    ds_read(&val2, (index2 * sizeof(int)) + sizeof(elements), sizeof(int));

    ds_write((index1 * sizeof(int)) + sizeof(elements), &val2, sizeof(int));
    ds_write((index2 * sizeof(int)) + sizeof(elements), &val1, sizeof(int));

    return 0;
}

long ds_find( int target ){

    int i, val;

    /* check each element until first instance of target is found*/
    for(i = 0; i < elements; ++i){
        ds_read(&val, (i * sizeof(int)) +sizeof(elements), sizeof(int));
        if( val == target){
            return i;
        }
    }
    return -1;
}
int ds_read_elements( char *filename ){

    FILE *fp;
    int i = 0;
    int val = 0;

    fp = fopen(filename, "r");

    if(fp == NULL){
        return -1;
    }

    /* read integer from every line until we hit an empty line*/
    while(fscanf(fp, "%d\n", &val) == 1){
        ds_insert(val, i);
        ++i;
    }

    return 0;
}

int ds_finish_array(){

    int i;

    /* write amount of elements back into file then close program*/
    ds_write(0, &elements, sizeof(long));
    i = ds_finish();

    if(i == 0){
        return -1;
    }

    return 0;
}
