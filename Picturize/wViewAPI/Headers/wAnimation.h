//
//  wAnimation.h
//  PhotoKit
//
//  Created by Wael Youssef on 4/5/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#ifndef wAnimation_H
#define wAnimation_H

#pragma mark -
#pragma mark - wAnimation.
typedef int wAnimationCurve;
#define wAnimationCurveNormal 0
#define wAnimationCurveEaseIn 1
#define wAnimationCurveEaseOut 2

#pragma mark - wAnimationGeneral.
void wAnimationBeginContext();
bool wAnimationApplyContext();
void wAnimationUpdate();
bool wAnimationIsAnimating();

#pragma mark - wAnimationSetter.
void wAnimationSetDuration(float duration);
void wAnimationSetAnimationCurve(wAnimationCurve animationCurve);
void wAnimationSetRepeatCount(int repeatCount);
void wAnimationSetRepeatIndefinitly(bool repeatIndefinitly);
void wAnimationSetReverseRepeat(bool reverseRepeat);
void wAnimationSetEndFunction(wFunction function, void*data);

#pragma mark - wAnimationGetter.
float wAnimationGetDuration();
wAnimationCurve wAnimationGetAnimationCurve();
int wAnimationGetRepeatCount();
bool wAnimationGetRepeatIndefinitly();
bool wAnimationGetReverseRepeat();
void wAnimationGetMatrix(wMatrix4*mat,wMatrix4 newMat);

#pragma mark - wViewAnimation.
void wViewAnimateFrame(wView*view,wRect frame);
void wViewAnimateCenter(wView*view,wPoint centerPoint);
void wViewAnimateBackgroundColor(wView*view,wColor backgroundColor);
void wViewAnimateAnchorPoint(wView*view,wAnchor anchorPoint);
void wViewAnimateAlpha(wView*view,float alpha);
void wViewAnimateMatrix(wView*view,wMatrix4 mat);
void wViewAnimateRotationOnXAxis(wView*view,float angle);
void wViewAnimateRotationOnYAxis(wView*view,float angle);
void wViewAnimateRotationOnZAxis(wView*view,float angle);

#endif