//
//  wString.c
//  PhotoKit
//
//  Created by Wael Youssef on 7/17/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#include "wViewAPI.h"

wString*wStringNewFromCString(const char*cString, unsigned int size){
    
    unsigned int stringMemSize=(sizeof(unsigned char)*(size+1)) + sizeof(unsigned int);
    wString*string=malloc(stringMemSize);
    
    memcpy(string, &size, sizeof(unsigned int));
    memcpy(&string[sizeof(unsigned int)], cString, sizeof(unsigned char)*size);
    
    unsigned char nullPtr='\0';
    memcpy(&string[stringMemSize-sizeof(unsigned char)], &nullPtr, sizeof(unsigned char));
    return string;
}

unsigned int wStringGetLength(wString*string){
    unsigned int size;
    memcpy(&size, string, sizeof(unsigned int));
    return size;
}

const char*wStringGetCString(wString*string){
    const char*cString=&string[sizeof(unsigned int)];
    return cString;
}

void wStringRelease(wString*string){
    free(string);
}

wArray*wStringArrayNewFromData(const void*data){
    unsigned int Location=sizeof(unsigned int);
    unsigned int itemCount;
    memcpy(&itemCount, &data[Location], sizeof(unsigned int));
    Location+=sizeof(unsigned int);
    
    wArray*array=wArrayNewWithCapacity(itemCount);
    for (int i=0; i<itemCount; i++) {
        unsigned int stringLength;
        memcpy(&stringLength, &data[Location], sizeof(unsigned int));
        Location+=sizeof(unsigned int);
        wString*string=wStringNewFromCString(&data[Location], stringLength);
        wArrayAddItem(array, string);
        Location+=stringLength;
    }
    
    return array;
}

const void*wStringArraySaveToData(wArray*array){
    unsigned int Location=0;
    unsigned int dataSize=sizeof(unsigned int)*2;
    unsigned int arrayCount=wArrayGetCount(array);
    for (int i=0; i<arrayCount; i++) {
        wString*string=wArrayGetItemAtIndex(array, i);
        dataSize+=wStringGetLength(string);
        dataSize+=sizeof(unsigned int);
    }
    
    void*data=malloc(dataSize);
    
    memcpy(&data[Location], &dataSize, sizeof(unsigned int));
    Location+=sizeof(unsigned int);
    memcpy(&data[Location], &arrayCount, sizeof(unsigned int));
    Location+=sizeof(unsigned int);
    
    for (int i=0; i<arrayCount; i++) {
        wString*string=wArrayGetItemAtIndex(array, i);
        unsigned stringLength=wStringGetLength(string);
        memcpy(&data[Location], &stringLength, sizeof(unsigned int));
        Location+=sizeof(unsigned int);
        
        const char*cString=wStringGetCString(string);
        memcpy(&data[Location], cString, stringLength);
        Location+=stringLength;

    }
    
    return data;
}