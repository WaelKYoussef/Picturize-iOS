//
//  wViewTextures.c
//  PhotoKit
//
//  Created by Wael Youssef on 2/11/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#include "wViewAPI.h"

#pragma mark - wTexture.



#pragma mark - wTextureGeneral.
bool isPowerOfTwo(int number){return (number!=0)&&((number&(number-1))==0);}

wTexture*wTextureNewEmpty(){
    wTexture*Texture=malloc(sizeof(wTexture));
    memset(Texture, 0, sizeof(wTexture));
    return Texture;
}

wTexture*wTextureNew(const char*data, unsigned int width, unsigned int height, wTextureFormat format){
    wTexture*Texture=malloc(sizeof(wTexture));
    Texture[0].width=width,Texture[0].height=height;
    Texture[0].format=format;
    
    if ((width==0&&height==0) || !data) {
        Texture[0].texture=0;
        return Texture;
    }
    unsigned int Format;
    switch (format) {
    case wTextureFormatAlpha:{Format=GL_ALPHA;break;}
    case wTextureFormatRGB:{Format=GL_RGB;break;}
    case wTextureFormatRGBA:{Format=GL_RGBA;break;}
    default:{Format=GL_RGBA;break;}}
    
    
    bool powerOfTwo=isPowerOfTwo(width)&&isPowerOfTwo(height);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &Texture[0].texture);
    glBindTexture(GL_TEXTURE_2D, Texture[0].texture);
    glTexImage2D(GL_TEXTURE_2D, 0, Format, width, height, 0, Format, GL_UNSIGNED_BYTE, data);
    
    if (powerOfTwo) {glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);}
    else{glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);}
        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    return Texture;
}

void wTextureRelease(wTexture*texture){
    glDeleteTextures(1, &texture[0].texture);
    free(texture);
}
