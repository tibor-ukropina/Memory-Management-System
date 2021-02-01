#include <stdio.h>
#include <stdlib.h>
#include "ds_list.h"
#include "ds_memory.h"

void ds_create_list(){

    long val = -1;

    ds_init("list.bin");

    /* allocate and store the head pointer for the linked list*/
    ds_malloc(sizeof(long));
    ds_write(0, &val, sizeof(long));

    ds_finish();
    
}

int ds_init_list(){

    /* list.bin will store linked list*/
    ds_init("list.bin");
    return 0;
}

int ds_replace( int value, long index ){

    int i;
    struct ds_list_item_struct tester;
    long next_location = 0;
    long prev_location = 0;

    if(index < 0){
        return -1;
    }

    /* read in linked list head*/
    ds_read(&next_location, 0, sizeof(long));

    for(i = 0; i < index + 1; ++i){
        ds_read(&tester, next_location, sizeof(struct ds_list_item_struct));
        if(next_location == -1){
            return -1;
        }
        prev_location = next_location;
        next_location = tester.next;
    }
    tester.item = value;

    ds_write(prev_location, &tester, sizeof(struct ds_list_item_struct));

    return 0;
}

int ds_insert( int value, long index ){

    int i;
    long counter = 0;
    int old_head;
    long next_location = 0;
    long prev_location = 0;
    struct ds_list_item_struct new, prev, temp;

    if(index < 0){
        return -1;
    }

    /* read in linked list head*/
    ds_read(&next_location, 0, sizeof(long));

    /* if list is empty*/
    if(next_location == -1){

        ds_malloc(sizeof(struct ds_list_item_struct));
        new.next = -1;
        new.item = value;
        next_location = 8;
        
        ds_write(0, &next_location, sizeof(long));
        ds_write(next_location, &new, sizeof(struct ds_list_item_struct));
        return 0;
    }
    /* if insertion takes place at head*/
    if(index == 0){
        old_head = next_location;
        while(next_location != -1){
            ds_read(&prev, next_location, sizeof(struct ds_list_item_struct));
            next_location = prev.next;
            counter++;
        }
        /* write in new node*/
        ds_malloc(sizeof(struct ds_list_item_struct));
        new.next = old_head;
        new.item = value;
        ds_write((counter * sizeof(struct ds_list_item_struct)) + sizeof(long), &new, sizeof(struct ds_list_item_struct));
        
        counter = (counter * sizeof(struct ds_list_item_struct)) + sizeof(long);
        /* update initial integer*/
        ds_write(0, &counter, sizeof(long));

        return 0;
    }

    /* list traversal into element before index*/
    for(i = 0; i < index; ++i){
        ds_read(&prev, next_location, sizeof(struct ds_list_item_struct));
        if(next_location == -1){
            return -1;
        }
        prev_location = next_location;
        next_location = prev.next;
        counter++;
    }

    /* if element before index is last element*/
    if(next_location == -1){
        prev.next = (index * sizeof(struct ds_list_item_struct)) + sizeof(long);

        ds_write(prev_location, &prev, sizeof(struct ds_list_item_struct));
        ds_malloc(sizeof(struct ds_list_item_struct));

        new.item = value;
        new.next = -1;

        ds_write((index * sizeof(struct ds_list_item_struct)) + sizeof(long), &new, sizeof(struct ds_list_item_struct));
        return 0;
    }
    /* if we are inserting the element in the middle of the list*/
    else{
        temp = prev;

        new.next = prev.next;
        new.item = value;

        while(prev.next != -1){
            ds_read(&prev, next_location, sizeof(struct ds_list_item_struct));
            next_location = prev.next;
            counter++;
        }
        temp.next = (counter * sizeof(struct ds_list_item_struct)) + sizeof(long);
        ds_write(prev_location, &temp, sizeof(struct ds_list_item_struct));

        counter = (counter * sizeof(struct ds_list_item_struct)) + sizeof(long);

        ds_malloc(sizeof(struct ds_list_item_struct));
        ds_write(counter, &new, sizeof(struct ds_list_item_struct));
        
        return 0;
    }

}

int ds_delete( long index ){

    int i;
    struct ds_list_item_struct tester, tester2;
    long next_location = 0;
    long prev_location = 0;

    if(index < 0){
        return -1;
    }

    /* read in linked list head*/
    ds_read(&next_location, 0, sizeof(long));

    /* if deleting first element, head needs to be updated*/
    if(index == 0){
        ds_read(&tester, next_location, sizeof(struct ds_list_item_struct));
        ds_write(0, &(tester.next), sizeof(long));
        return 0;
    }
    /* if deleting any other element*/
    for(i = 0; i < index; ++i){
        ds_read(&tester, next_location, sizeof(struct ds_list_item_struct));
        prev_location = next_location;
        if(tester.next == -1){
            return -1;
        }
        next_location = tester.next;
    }
        
    ds_read(&tester2, next_location, sizeof(struct ds_list_item_struct));
    ds_free(next_location);

    tester.next = tester2.next;
    ds_write(prev_location, &tester, sizeof(struct ds_list_item_struct));

    return 0;
}

int ds_swap( long index1, long index2 ){

    int i, temp;
    long hi, lo;
    long lo_location = 0;
    long prev_location = 0;
    long next_location = 0;
    struct ds_list_item_struct lower, higher;

    hi = lo = 0;

    /* read in linked list head*/
    ds_read(&next_location, 0, sizeof(long));

    /* check for invalid index*/
    if(index1 < 0 || index2 < 0){
        return -1;
    }
    
    /* assign indexes by size */
    if(index1 == index2){
        return 0;
    }
    else if(index1 > index2){
        hi = index1;
        lo = index2;
    }
    else if(index2 > index1){
        hi = index2;
        lo = index1;
    }
    for(i = 0; i <= hi; ++i){

        ds_read(&higher, next_location, sizeof(struct ds_list_item_struct));
        if(next_location == -1){
            return -1;
        }
        prev_location = next_location;
        next_location = higher.next;

        if(i == lo){
            /* store lower index and adress into local variable*/
            ds_read(&lower, prev_location, sizeof(struct ds_list_item_struct));
            lo_location = prev_location;
        }
    }
    /* swap values between higher and lower index then store them in their original adresses*/
    temp = higher.item;
    higher.item = lower.item;
    lower.item = temp;

    ds_write(lo_location, &lower, sizeof(struct ds_list_item_struct));
    ds_write(prev_location, &higher, sizeof(struct ds_list_item_struct));

    return 0;
}

long ds_find( int target ){

    struct ds_list_item_struct tester;
    long next_location = 0;
    int counter = 0;

    /* read in linked list head*/
    ds_read(&next_location, 0, sizeof(long));

    /* traverse list until first instance of target is found, return amount of traversals*/
    while(next_location != -1){
        ds_read(&tester, next_location, sizeof(struct ds_list_item_struct));
        if(tester.item == target){
            return counter;
        }
        counter++;
        next_location = tester.next;
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

int ds_finish_list(){

    int i;

    i = ds_finish();

    /* check ds_finish was unsuccessful*/
    if(i == 0){
        return -1;
    }

    return 0;
}
