//
//  PKOpenGLView.h
//  PhotoKit
//
//  Created by Wael Youssef on 1/8/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#ifndef iOSwOpenGLView_H
#define iOSwOpenGLView_H

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <CoreMotion/CoreMotion.h>
#include "wViewAPI.h"
@interface iOSwOpenGLView:UIView
+(EAGLContext*)getOGLCOntext;
+(void)overrideUserInputOrientation:(BOOL)overrideUserInputOrientation;
+(void)rebindRenderBuffer;
@end

#endif