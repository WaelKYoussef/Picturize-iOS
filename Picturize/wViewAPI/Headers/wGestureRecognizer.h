//
//  wGestureRecognizer.h
//  PhotoKit
//
//  Created by Wael Youssef on 5/9/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#ifndef wGestureRecognizer_H
#define wGestureRecognizer_H

#define wGestureTouchDown 0
#define wGestureTouchUp 1
#define wGestureTap 2
#define wGestureDoubleTap 3
#define wGestureSwipe 4
#define wGestureDoubleSwipe 5

typedef struct _wGesture wGesture;
typedef struct _wGestureContext wGestureContext;

wGesture*wGestureNew(int gestureType);
void wGestureRelease(wGesture*gesture);
void wViewAddGesture(wView*view, wGesture*gesture);
void wViewRemoveGestureOfTypeAtIndex(wView*view, int gestureType, int index);
void wViewRemoveGesture(wView*view,wGesture*gesture);
void wViewRemoveAllGestures(wView*view);
unsigned char wViewGetGesturesCount(wView*view);
unsigned char wViewGetGesturesCountOfType(wView*view, unsigned char type);
wGesture*wViewGetGestureOfTypeAtIndex(wView*view, int gestureType, int index);

void wGestureSetFunction(wGesture*gesture, wFunction function);
wView*wGestureGetView(wGesture*gesture);
wTimeStamp wGestureGetTimeStamp(wGesture*gesture);
unsigned char wGestureGetType(wGesture*gesture);
wPoint wGestureGetLocationAtIndex(wGesture*gesture, int index);

void wGestureTouchDownProcess(wView*view, wPoint location);
void wGestureTouchUpProcess(wView*view, wPoint location);
void wGestureTapProcess(wView*view, wPoint location);
void wGestureDoubleTapProcess(wView*view, wPoint location);
void wGestureSwipeProcess(wView*view, wPoint location);
void wGestureDoubleSwipeProcess(wView*view, wPoint locations[2]);

#endif