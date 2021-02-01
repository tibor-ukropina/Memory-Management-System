struct ds_file_struct ds_file; /* Holds file pointer and the header */
struct ds_counts_struct ds_counts; /* Holds amount of read and write operations */

#include <stdio.h>
#include <stdlib.h>
#include "ds_memory.h"

int ds_create( char *filename, long size ){

    int i;
    char byte = '0';
    struct ds_file_struct header;

    header.fp = fopen(filename, "wb+");

    if(header.fp == NULL){
        return -1;
    }
    /* first entry of the file, length set equal to size to represent available memory*/
    header.block[0].start = 0;
    header.block[0].length = size;
    header.block[0].alloced = '0';
    /* initialize all other blocks*/
    for(i = 1; i < MAX_BLOCKS; ++i){
        header.block[i].start = 0;
        header.block[i].length = 0;
        header.block[i].alloced = '0';
    }
    /* write header block to the file*/
    fwrite(&header.block, sizeof(struct ds_blocks_struct) * MAX_BLOCKS, 1, header.fp );

    /* write in memory locations*/
    for(i = 0; i < size; ++i){
        fwrite(&byte, 1, 1, header.fp);
    }

    fclose(header.fp);

    return 0;
}

int ds_init( char *filename ){

    ds_counts.reads = 0;
    ds_counts.writes = 0;

    ds_file.fp = fopen(filename, "rb+");

    if(ds_file.fp == NULL){
        return -1;
    }
    /* Read header into global variable from file*/
    fread(&(ds_file.block), sizeof(struct ds_blocks_struct) * MAX_BLOCKS, 1, ds_file.fp); 

    /* file pointer gets moved after fread, needs to be reset*/
    rewind(ds_file.fp);

    return 0;
}

long ds_malloc( long amount ){
    
    int i = 0;
    int j = 0;
    struct ds_blocks_struct temp;

    /* look for a free block large enough to hold the amount*/
    for(i = 0; i < MAX_BLOCKS; ++i){
        if(ds_file.block[i].length >= amount && ds_file.block[i].alloced == '0'){
            
            temp = ds_file.block[i];

            /* allocate the new block */
            ds_file.block[i].length = amount;
            ds_file.block[i].alloced = '1';

            /* find the next free block*/
            for(j = 0; j < MAX_BLOCKS; ++j){
                if(ds_file.block[j].length == 0){
                    /* store left over memory in new block*/
                    ds_file.block[j].start =  temp.start + amount;
                    ds_file.block[j].length =  temp.length - amount;
                    ds_file.block[j].alloced =  '0';
                    break;
                }
            }
            return ds_file.block[i].start;
        }
    }
    return -1;
}

void ds_free( long start ){
    
    int i = 0;

    /* will free the block at index start */
    for(i = 0; i < MAX_BLOCKS; ++i){
        if(ds_file.block[i].start == start){
            ds_file.block[i].alloced = '0';
        }
    }
}

void *ds_read( void *ptr, long start, long bytes ){

    /* move file pointer past header */
    fseek(ds_file.fp, sizeof(struct ds_file_struct) + start, SEEK_SET); 

    if(fread(ptr, sizeof(char), bytes, ds_file.fp)){
        ds_counts.reads++;
        rewind(ds_file.fp); /* file pointer gets moved after fread, needs to be reset*/

        return ptr;
    }

    return NULL;
}

long ds_write( long start, void *ptr, long bytes ){

    /* move file pointer past header */
    fseek(ds_file.fp, sizeof(struct ds_file_struct) + start, SEEK_SET);

    if (fwrite(ptr, sizeof(char), bytes, ds_file.fp)){
        ds_counts.writes++;
        rewind(ds_file.fp); /* file pointer gets moved after fwrite, needs to be reset*/

        return start;
    }

    return -1;
}

int ds_finish(){

    /* write updated header back into file */
    if(fwrite(&(ds_file.block), sizeof(struct ds_blocks_struct) * MAX_BLOCKS, 1, ds_file.fp)){

        printf("reads: %d\n", ds_counts.reads);
        printf("writes: %d\n", ds_counts.writes);
        
        rewind(ds_file.fp); /* file pointer gets moved after fwrite, needs to be reset*/
        fclose(ds_file.fp);

        return 1;
    }

    return 0;    
}
