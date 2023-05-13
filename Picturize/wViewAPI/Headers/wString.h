//
//  wString.h
//  PhotoKit
//
//  Created by Wael Youssef on 7/17/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#ifndef wString_h
#define wString_h

typedef void wString;

wString*wStringNewFromCString(const char*wString, unsigned int size);
unsigned int wStringGetLength(wString*string);
const char*wStringGetCString(wString*string);
void wStringRelease(wString*string);

wArray*wStringArrayNewFromData(const void*data);
const void*wStringArraySaveToData(wArray*array);

#endif
