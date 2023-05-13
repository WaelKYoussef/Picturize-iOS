//
//  wViewTextures.h
//  PhotoKit
//
//  Created by Wael Youssef on 4/5/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#ifndef wTexture_H
#define wTexture_H

#pragma mark -
#pragma mark - wTexture.

typedef unsigned char wTextureFormat;
#define wTextureFormatAlpha 1
#define wTextureFormatRGB 2
#define wTextureFormatRGBA 3

typedef struct _wTexture{
    GLuint texture;
    unsigned int width,height;
    wTextureFormat format;
}wTexture;

wTexture*wTextureNewEmpty();
wTexture*wTextureNew(const char*data, unsigned int width, unsigned int height, wTextureFormat format);
void wTextureRelease(wTexture*texture);
#endif