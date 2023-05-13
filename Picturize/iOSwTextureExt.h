//
//  iOSwTextureExt.h
//  PhotoKit
//
//  Created by Wael Youssef on 7/12/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreText/CoreText.h>
#include "wViewAPI.h"

typedef CFDictionaryRef wFont;

@interface iOSwTextureExt : NSObject
+(wTexture*)wTextureFromImageWithName:(NSString*)name;
+(wTexture*)wTextureFromUIImage:(UIImage*)image;
+(wTexture*)wTextureFromCGImage:(CGImageRef)imageRef Size:(wSize)size OriginalSize:(BOOL)originalSize;
+(wTexture*)wTextureFromWString:(wString*)string Font:(wFont)font MaxWidth:(float)maxWidth;
+(wTexture*)wTextureFromNSString:(NSString*)string Font:(wFont)font MaxWidth:(float)maxWidth;
+(wFont)wFontNewFromUIFont:(UIFont*)font;
+(void)wFontRelease:(wFont)font;
@end