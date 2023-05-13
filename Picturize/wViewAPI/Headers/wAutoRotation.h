//
//  wAutoRotation.h
//  PhotoKit
//
//  Created by Wael Youssef on 4/5/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#ifndef wAutoRotation_H
#define wAutoRotation_H

#pragma mark -
#pragma mark - wAutoRotation.

typedef int wDeviceOrientation;
#define wDeviceOrientationPortraitUp 0
#define wDeviceOrientationPortraitDown 1
#define wDeviceOrientationLandscapeLeft 2
#define wDeviceOrientationLandscapeRight 3

#pragma mark - wAutoRotationSetter.
void wAutoRotationSetDuration(float duration);
void wAutoRotationSetAnimateResizing(bool animateResizing);
void wAutoRotationSetAnimateRotation(bool animateRotation);
void wAutoRotationSetAllowsOrientation(wDeviceOrientation orientation, bool isAllowed);
void wAutoRotationSetBeginFunction(void (*beginFunction)(wDeviceOrientation));
void wAutoRotationSetEndFunction(void (*endFunction)(wDeviceOrientation));
void wAutoRotationSetBeginFunctionData(void*beginFunctionData);
void wAutoRotationSetEndFunctionData(void*endFunctionData);

#pragma mark - wAutoRotationGetter.
float wAutoRotationGetDuration();
bool wAutoRotationGetAnimateResizing();
bool wAutoRotationGetAnimateRotation();
bool wAutoRotationGetAllowsOrientation(wDeviceOrientation orientation);
bool wAutoRotationGetIsRotating();

#pragma mark - wDeviceOrientationSetter/Getter.
wDeviceOrientation wDeviceGetOrientation();
bool wDeviceSetOrientation(wDeviceOrientation orientation);
bool wDeviceOrientationIsPortrait(wDeviceOrientation orientation);
bool wDeviceOrientationIsLandscape(wDeviceOrientation orientation);

#pragma mark - SystemRserved.
wMatrix4 wAutoRotationMatrix;
void wAutoRotationAnimation();

#endif