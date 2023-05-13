//
//  PKGallery_Canvas.h
//  PhotoKit
//
//  Created by Wael on 8/26/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#ifndef PhotoKit_PKGallery_Canvas_h
#define PhotoKit_PKGallery_Canvas_h
#import "wViewAPI.h"

wView*GL_Canvas;
wView*GL_Canvas_Image;
wView*GL_Canvas_Image_Temp;
wView*GL_Canvas_LeftButton;
wView*GL_Canvas_RightButton;

void GL_Canvas_Create();
void GL_Canvas_LoadAssetAtIndex(unsigned int index);
void GL_Canvas_LoadNextPhoto();
void GL_Canvas_LoadPreviousPhoto();

unsigned int IndexOfLoadedAsset;
#endif
