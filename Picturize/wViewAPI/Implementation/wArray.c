//
//  wArray.c
//  PhotoKit
//
//  Created by Wael Youssef on 5/15/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#include "wViewAPI.h"


typedef struct _wArray{ int count,capacity;wArrayItem*items; }wArray;

wArray*wArrayNewWithCapacity(int capacity){
    wArray*array=malloc(sizeof(wArray));
    array[0].count=0;
    array[0].capacity=capacity;
    if (array[0].capacity<0) {array[0].capacity=0;}
    array[0].items=malloc(sizeof(void*)*array[0].capacity);
    return array;
}

wArray*wArrayNew(){
    return wArrayNewWithCapacity(5);
}

wArrayItem wArrayGetItemAtIndex(wArray*array, int index){
    if (index<array[0].count) {
        return array[0].items[index];}
    else{return NULL;}
}

int wArrayGetCount(wArray*array){
    return array[0].count;
}

void wArrayAddItemAtIndex(wArray*array, wArrayItem item, int index){
    if (array[0].count==array[0].capacity) {
        array[0].capacity=array[0].count+5;
        array[0].items=realloc(array[0].items, sizeof(wArrayItem)*array[0].capacity);
    }
    
    int Index=index;
    if (Index<0) {Index=0;}
    else if (Index>array[0].count){Index=array[0].count;}
    
    memmove(&array[0].items[Index+1], &array[0].items[Index], sizeof(wArrayItem)*(array[0].count-Index));
    array[0].count++;
    array[0].items[Index]=item;
}

void wArrayAddItem(wArray*array, wArrayItem item){
    wArrayAddItemAtIndex(array, item, array[0].count);
}

void wArrayRemoveItemAtIndex(wArray*array, int index){
    if (index>=0 && index<array[0].count){
        memmove(&array[0].items[index], &array[0].items[index+1], sizeof(wArrayItem)*(array[0].count-index-1));
        array[0].count--;
        
        if (array[0].capacity>(array[0].count+10)) {
            array[0].capacity=array[0].count+5;
            array[0].items=realloc(array[0].items, sizeof(wArrayItem)*array[0].capacity);
        }
    }
}

void wArrayRemoveAllItems(wArray*array){
    if (array[0].count>0) {
        array[0].count=0;
        array[0].capacity=5;
        array[0].items=realloc(array[0].items, sizeof(wArrayItem)*array[0].capacity);
    }
}

void wArrayRelease(wArray*array){
    free(array[0].items);
    free(array);
    array=NULL;
}
