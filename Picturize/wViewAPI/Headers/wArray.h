//
//  wArray.h
//  PhotoKit
//
//  Created by Wael Youssef on 5/15/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#ifndef wArray_H
#define wArray_H

typedef struct _wArray wArray;
typedef  void* wArrayItem;

wArray*wArrayNew();
wArray*wArrayNewWithCapacity(int capacity);
wArrayItem wArrayGetItemAtIndex(wArray*array, int index);
int wArrayGetCount(wArray*array);
void wArrayAddItemAtIndex(wArray*array, wArrayItem item, int index);
void wArrayAddItem(wArray*array, wArrayItem item);
void wArrayRemoveItemAtIndex(wArray*array, int index);
void wArrayRemoveAllItems(wArray*array);
void wArrayRelease(wArray*array);

#endif