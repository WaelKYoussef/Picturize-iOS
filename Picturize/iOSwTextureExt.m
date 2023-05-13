//
//  iOSwTextureExt.m
//  PhotoKit
//
//  Created by Wael Youssef on 7/12/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#import "iOSwTextureExt.h"

@implementation iOSwTextureExt

+(wTexture*)wTextureFromImageWithName:(NSString*)name{
    UIImage*image=[UIImage imageNamed:name];
    return [iOSwTextureExt wTextureFromUIImage:image];
}

+(wTexture*)wTextureFromUIImage:(UIImage*)image{
    CGImageRef imageRef=[image CGImage];
    return [iOSwTextureExt wTextureFromCGImage:imageRef Size:wSizeZero OriginalSize:true];
}

+(wTexture*)wTextureFromCGImage:(CGImageRef)imageRef Size:(wSize)size OriginalSize:(BOOL)originalSize{
    
    size_t width, height;
    if (originalSize) {
        width=CGImageGetWidth(imageRef);
        height=CGImageGetHeight(imageRef);}
    else{width=size.width;
        height=size.height;}
        
    GLubyte* textureData = (GLubyte *)malloc(width * height * 4);
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    NSUInteger bytesPerPixel = 4;
    NSUInteger bytesPerRow = bytesPerPixel * width;
    NSUInteger bitsPerComponent = 8;
    CGContextRef context = CGBitmapContextCreate(textureData, width, height,
                                                 bitsPerComponent, bytesPerRow, colorSpace,
                                                 kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    
    CGColorSpaceRelease(colorSpace);
    CGContextClearRect(context, CGRectMake(0, 0, width, height));
    
    CGContextDrawImage(context, CGRectMake(0, 0, width, height), imageRef);
    CGContextRelease(context);
    
    wTexture*texture=wTextureNew((const char*)textureData, (int)width, (int)height, wTextureFormatRGBA);
    free(textureData);
        
    return texture;
}

+(wFont)wFontNewFromUIFont:(UIFont*)font{
    CTFontRef CFfont=CTFontCreateWithName((__bridge CFStringRef)font.fontName, font.pointSize, NULL);
    
    CFStringRef keys[] = { kCTFontAttributeName };
    CFTypeRef values[] = { CFfont };
    
    CFDictionaryRef attributes =
    CFDictionaryCreate(kCFAllocatorDefault, (const void**)&keys,
                       (const void**)&values, sizeof(keys) / sizeof(keys[0]),
                       &kCFTypeDictionaryKeyCallBacks,
                       &kCFTypeDictionaryValueCallBacks);
    
    return attributes;
}

+(void)wFontRelease:(wFont)font{
    CFRelease(font);
}

+(wTexture*)wTextureFromNSString:(NSString*)string Font:(wFont)font MaxWidth:(float)maxWidth{
    CFAttributedStringRef attrString=CFAttributedStringCreate(kCFAllocatorDefault, (__bridge CFStringRef)string, font);
    
    CTLineRef finalLine=CTLineCreateWithAttributedString(attrString);
    
    if (CTLineGetBoundsWithOptions(finalLine, 0).size.width>maxWidth) {
        CFAttributedStringRef attrTail =
        CFAttributedStringCreate(kCFAllocatorDefault, (__bridge CFStringRef)@"...", font);
        
        CTLineRef lineTail=CTLineCreateWithAttributedString(attrTail);
        CTLineRef line=CTLineCreateTruncatedLine(finalLine, maxWidth, kCTLineTruncationEnd, lineTail);
        CFRelease(finalLine);
        CFRelease(lineTail);
        finalLine=line;
        CFRelease(attrTail);
    }
    
    

    CTFontRef ctfont=CFDictionaryGetValue(font, kCTFontAttributeName);
    float a=CTFontGetAscent(ctfont);
    float d=CTFontGetDescent(ctfont);
    CGRect rect=CTLineGetBoundsWithOptions(finalLine, kCTLineBoundsUseGlyphPathBounds);
    size_t width=rect.size.width+10, height=a+d+10;
    
    GLubyte*textureData=(GLubyte*)malloc(width*height);
    CGContextRef context=CGBitmapContextCreate(textureData, width, height,
                                                   8, width, nil,
                                                   (CGBitmapInfo)kCGImageAlphaOnly);
    
    CGContextClearRect(context, CGRectMake(0, 0, width, height));
    CGContextSetTextPosition(context, 0.0, d+5);
    
    CTLineDraw(finalLine, context);
    
    CGContextRelease(context);
    CFRelease(finalLine);
    CFRelease(attrString);
    
    wTexture*texture=wTextureNew((const char*)textureData, (int)width, (int)height, wTextureFormatAlpha);
    free(textureData);
    return texture;
}

+(wTexture*)wTextureFromWString:(wString*)string Font:(wFont)font MaxWidth:(float)maxWidth{
    NSString*nsString=[NSString stringWithUTF8String:wStringGetCString(string)];
    return [iOSwTextureExt wTextureFromNSString:nsString Font:font MaxWidth:maxWidth];
}

@end
