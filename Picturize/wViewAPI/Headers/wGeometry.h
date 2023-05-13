//
//  wViewGeometry.h
//  PhotoKit
//
//  Created by Wael Youssef on 4/5/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#ifndef wGeometry_H
#define wGeometry_H

#pragma mark -
#pragma mark - wGeometry.
#pragma mark - wGeometryTypes.
typedef struct _wVec2{ float x,y; }wVec2;
typedef wVec2 wPoint; typedef wVec2 wAnchor;
typedef struct _wSize{ float width,height; }wSize;
typedef struct _wRect{ wPoint origin;wSize size; }wRect;

typedef struct _wVec3{ float x,y,z; }wVec3;
typedef struct _wVec4{ float x,y,z,w; }wVec4;
typedef struct _wMatrix2{ float m11,m12,m21,m22; }wMatrix2;
typedef struct _wMatrix3{ float m11,m12,m13,m21,m22,m23,m31,m32,m33; }wMatrix3;
typedef struct _wMatrix4{ float m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,m41,m42,m43,m44; }wMatrix4;

float interpolateFloat(float f0, float f1, float t) ;

#pragma mark - wGeometryConstants.
const wPoint wPointZero;
const wSize wSizeZero;
const wRect wRectZero;

const wAnchor wAnchorTopLeft;
const wAnchor wAnchorCenterLeft;
const wAnchor wAnchorBottomLeft;
const wAnchor wAnchorMidTop;
const wAnchor wAnchorMidCenter;
const wAnchor wAnchorMidBottom;
const wAnchor wAnchorTopRight;
const wAnchor wAnchorCenterRight;
const wAnchor wAnchorBottomRight;

const wVec2 wVec2Zero;
const wVec3 wVec3Zero;
const wVec4 wVec4Zero;
const wMatrix2 wMat2Identity;
const wMatrix3 wMat3Identity;
const wMatrix4 wMat4Identity;


#pragma mark - wGeometryFunctions.
wPoint wPointNew(float x, float y);
bool wPointIsEqualToPoint(wPoint point1, wPoint point2);
float wPointDistanceFromPoint(wPoint point1, wPoint point2);
wPoint wPointAddPoints(wPoint p1, wPoint p2);
wPoint interpolatePoint(wPoint p0, wPoint p1, float t);
wAnchor wAnchorNew(float x, float y);
bool wAnchorIsEqualToAnchor(wAnchor anchor1, wAnchor anchor2);
wSize wSizeNew(float width, float height);
bool wSizeIsEqualToSize(wSize size1, wSize size2);
wRect wRectNew(float x, float y, float width, float height);
bool wRectIsEqualToRect(wRect rect1, wRect rect2);
bool wRectContainsPoint(wRect rect,wPoint point);
wRect interpolateRect(wRect r0, wRect r1, float t);

wVec2 wVec2New(float x, float y);
wVec2 wVec2MultiplyByVec2(wVec2 vec1,wVec2 vec2);
wVec3 wVec3New(float x, float y, float z);
wVec3 wVec3MultiplyByVec3(wVec3 vec1,wVec3 vec2);
wVec4 wVec4New(float x, float y, float z, float w);
wVec4 wVec4MultiplyByVec4(wVec4 vec1,wVec4 vec2);

wMatrix2 wMat2New(float m11,float m12,
                  float m21,float m22);
wVec2 wMat2MultiplyByVec2(wMatrix2 mat,wVec2 vec);
wMatrix2 wMat2MultiplyByMat2(wMatrix2 mat1,wMatrix2 mat2);

wMatrix3 wMat3New(float m11,float m12,float m13,
                  float m21,float m22,float m23,
                  float m31,float m32,float m33);
wVec3 wMat3MultiplyByVec3(wMatrix3 mat,wVec3 vec);
wMatrix3 wMat3MultiplyByMat3(wMatrix3 mat1,wMatrix3 mat2);

wMatrix4 wMat4New(float m11,float m12,float m13,float m14,
                  float m21,float m22,float m23,float m24,
                  float m31,float m32,float m33,float m34,
                  float m41,float m42,float m43,float m44);
bool wMat4IsIdentity(wMatrix4 mat);
bool wMat4IsEqualToMat4(wMatrix4 mat1,wMatrix4 mat2);
wVec4 wMat4MultiplyByVec4(wMatrix4 mat,wVec4 vec);
wMatrix4 wMat4MultiplyByMat4(wMatrix4 mat1,wMatrix4 mat2);
wMatrix4 wMat4RotateOnXAxis(wMatrix4 mat, float angle);
wMatrix4 wMat4RotateOnYAxis(wMatrix4 mat, float angle);
wMatrix4 wMat4RotateOnZAxis(wMatrix4 mat, float angle);
wMatrix4 wMat4Translate(wMatrix4 mat, float x, float y, float z);

#endif
