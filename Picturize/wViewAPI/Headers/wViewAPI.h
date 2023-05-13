//
//  wViewAPI.h
//  PhotoKit
//
//  Created by Wael Youssef on 2/12/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#ifndef wViewAPI_H
#define wViewAPI_H

#define wFrameRate 60.0
typedef struct _wView wView;
typedef struct _wTimer wTimer;
typedef void (*wFunction)(void*);

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include "wArray.h"
#include "wString.h"
#include "wTimer.h"
#include "wColor.h"
#include "wGeometry.h"
#include "wGestureRecognizer.h"

#include "wAnimation.h"
#include "wAutoRotation.h"
#include "wTextures.h"
#include "wView.h"
#include "wScrollView.h"
#include "wTableView.h"
#include "wButton.h"

#endif




