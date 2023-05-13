//
//  wViewGeometry.c
//  PhotoKit
//
//  Created by Wael Youssef on 2/26/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//


#include "wViewAPI.h"

#pragma mark - wGeometryConstants.
const float D2R=M_PI/180.0;

const wPoint wPointZero={0.0f,0.0f};
const wSize wSizeZero={0.0f,0.0f};
const wRect wRectZero={0.0f,0.0f,0.0f,0.0f};

const wAnchor wAnchorTopLeft={0.0,0.0};
const wAnchor wAnchorCenterLeft={0.0,0.5};
const wAnchor wAnchorBottomLeft={0.0,1.0};
const wAnchor wAnchorMidTop={0.5,0.0};
const wAnchor wAnchorMidCenter={0.5,0.5};
const wAnchor wAnchorMidBottom={0.5,1.0};
const wAnchor wAnchorTopRight={1.0,0.0};
const wAnchor wAnchorCenterRight={1.0,0.5};
const wAnchor wAnchorBottomRight={1.0,1.0};

const wVec2 wVec2Zero={0.0f,0.0f};
const wVec3 wVec3Zero={0.0f,0.0f,0.0f};
const wVec4 wVec4Zero={0.0f,0.0f,0.0f,0.0f};
const wMatrix2 wMat2Identity={
    1.0f,0.0f,
    0.0f,1.0f};
const wMatrix3 wMat3Identity={
    1.0f,0.0f,0.0f,
    0.0f,1.0f,0.0f,
    0.0f,0.0f,1.0f};
const wMatrix4 wMat4Identity={
    1.0f,0.0f,0.0f,0.0f,
    0.0f,1.0f,0.0f,0.0f,
    0.0f,0.0f,1.0f,0.0f,
    0.0f,0.0f,0.0f,1.0f};

float interpolateFloat(float f0, float f1, float t) {
    return f0+(f1-f0)*t;
}

#pragma mark - wGeometryMisc.
wPoint wPointNew(float x, float y){
    wPoint point={x,y};return point;
}

bool wPointIsEqualToPoint(wPoint point1, wPoint point2){
    return (point1.x==point2.x && point1.y==point2.y);
}

float wPointDistanceFromPoint(wPoint point1, wPoint point2){
    float side1=point2.x-point1.x;
    float side2=point2.y-point1.y;
    return sqrt(side1*side1+side2*side2);
}

wPoint wPointAddPoints(wPoint p1, wPoint p2){
    return wPointNew(p1.x+p2.x, p1.y+p2.y);
}

wPoint interpolatePoint(wPoint p0, wPoint p1, float t){
    wPoint point;
    point.x=interpolateFloat(p0.x, p1.x, t);
    point.y=interpolateFloat(p0.y, p1.y, t);
    return point;
}

wAnchor wAnchorNew(float x, float y){
    wAnchor anchor={x,y};return anchor;
}

bool wAnchorIsEqualToAnchor(wAnchor anchor1, wAnchor anchor2){
    return (anchor1.x==anchor2.x && anchor1.y==anchor2.y);
}

wSize wSizeNew(float width, float height){
    wSize size={width,height};return size;
}

bool wSizeIsEqualToSize(wSize size1, wSize size2){
    return (size1.width==size2.width && size1.height==size2.height);
}

wRect wRectNew(float x, float y, float width, float height){
     wRect rect={wPointNew(x, y),wSizeNew(width, height)};return rect;
}

bool wRectIsEqualToRect(wRect rect1, wRect rect2){
    return (wPointIsEqualToPoint(rect1.origin, rect2.origin)&&
            wSizeIsEqualToSize(rect1.size, rect2.size));
}

bool wRectContainsPoint(wRect rect,wPoint point){
    if (point.x<rect.origin.x) {return false;}
    if (point.y<rect.origin.y) {return false;}
    if (point.x>rect.origin.x+rect.size.width) {return false;}
    if (point.y>rect.origin.y+rect.size.height) {return false;}
    return true;
}

wRect interpolateRect(wRect r0, wRect r1, float t){
    wRect rect;
    rect.origin.x=interpolateFloat(r0.origin.x, r1.origin.x, t);
    rect.origin.y=interpolateFloat(r0.origin.y, r1.origin.y, t);
    rect.size.width=interpolateFloat(r0.size.width, r1.size.width, t);
    rect.size.height=interpolateFloat(r0.size.height, r1.size.height, t);
    return rect;
}


#pragma mark - wGeometryVecX.
wVec2 wVec2New(float x, float y){
    wVec2 vec={x,y};
    return vec;
}

wVec2 wVec2MultiplyByVec2(wVec2 vec1,wVec2 vec2){
    wVec2 Result={vec1.x*vec2.x,vec1.y*vec2.y};
    return Result;
}

wVec3 wVec3New(float x, float y, float z){
    wVec3 vec={x,y,z};return vec;
}

wVec3 wVec3MultiplyByVec3(wVec3 vec1,wVec3 vec2){
    wVec3 Result={vec1.x*vec2.x,vec1.y*vec2.y,vec1.z*vec2.z};
    return Result;
}

wVec4 wVec4New(float x, float y, float z, float w){
    wVec4 vec={x,y,z,w};
    return vec;
}

wVec4 wVec4MultiplyByVec4(wVec4 vec1,wVec4 vec2){
    wVec4 Result={vec1.x*vec2.x,vec1.y*vec2.y,vec1.z*vec2.z,vec1.w*vec2.w};
    return Result;
}


#pragma mark - wMatrix2.
wMatrix2 wMat2New(float m11,float m12,float m21,float m22){
    wMatrix2 mat={
        m11,m12,
        m21,m22};
    return mat;
}

wVec2 wMat2MultiplyByVec2(wMatrix2 mat,wVec2 vec){
    wVec2 Result;
    Result.x=(mat.m11*vec.x)+(mat.m12*vec.y);
    Result.y=(mat.m21*vec.x)+(mat.m22*vec.y);
    return Result;
}

wMatrix2 wMat2MultiplyByMat2(wMatrix2 mat1,wMatrix2 mat2){
    wMatrix2 Result;
    Result.m11=(mat1.m11*mat2.m11)+(mat1.m12*mat2.m21);
    Result.m12=(mat1.m11*mat2.m12)+(mat1.m12*mat2.m22);
    
    Result.m21=(mat1.m21*mat2.m11)+(mat1.m22*mat2.m21);
    Result.m22=(mat1.m21*mat2.m12)+(mat1.m22*mat2.m22);
    return Result;
}



#pragma mark - wMatrix3.
wMatrix3 wMat3New(float m11,float m12,float m13,float m21,float m22,float m23,float m31,float m32,float m33){
    wMatrix3 mat={
        m11,m12,m13,
        m21,m22,m23,
        m31,m32,m33};
    return mat;
}

wVec3 wMat3MultiplyByVec3(wMatrix3 mat,wVec3 vec){
    wVec3 Result;
    Result.x=(mat.m11*vec.x)+(mat.m12*vec.y)+(mat.m13*vec.z);
    Result.y=(mat.m21*vec.x)+(mat.m22*vec.y)+(mat.m23*vec.z);
    Result.z=(mat.m31*vec.x)+(mat.m32*vec.y)+(mat.m33*vec.z);
    return Result;
}

wMatrix3 wMat3MultiplyByMat3(wMatrix3 mat1,wMatrix3 mat2){
    wMatrix3 Result;
    Result.m11=(mat1.m11*mat2.m11)+(mat1.m12*mat2.m21)+(mat1.m13*mat2.m31);
    Result.m12=(mat1.m11*mat2.m12)+(mat1.m12*mat2.m22)+(mat1.m13*mat2.m32);
    Result.m13=(mat1.m11*mat2.m13)+(mat1.m12*mat2.m23)+(mat1.m13*mat2.m33);
    
    Result.m21=(mat1.m21*mat2.m11)+(mat1.m22*mat2.m21)+(mat1.m23*mat2.m31);
    Result.m22=(mat1.m21*mat2.m12)+(mat1.m22*mat2.m22)+(mat1.m23*mat2.m32);
    Result.m23=(mat1.m21*mat2.m13)+(mat1.m22*mat2.m23)+(mat1.m23*mat2.m33);
    
    Result.m31=(mat1.m31*mat2.m11)+(mat1.m32*mat2.m21)+(mat1.m33*mat2.m31);
    Result.m32=(mat1.m31*mat2.m12)+(mat1.m32*mat2.m22)+(mat1.m33*mat2.m32);
    Result.m33=(mat1.m31*mat2.m13)+(mat1.m32*mat2.m23)+(mat1.m33*mat2.m33);
    return Result;
}

wMatrix3 wMat3Rotate(wMatrix3 mat, float angle){
    float cosA=cos(angle*D2R);
    float sinA=sin(angle*D2R);
    wMatrix3 r={cosA,-sinA,0.0f,
        sinA, cosA,0.0f,
        0.0f, 0.0f,1.0f};
    
    return wMat3MultiplyByMat3(r, mat);
}


#pragma mark - wMatrix4.
wMatrix4 wMat4New(float m11,float m12,float m13,float m14,float m21,float m22,float m23,float m24,float m31,float m32,float m33,float m34,float m41,float m42,float m43,float m44){
    wMatrix4 mat={
        m11,m12,m13,m14,
        m21,m22,m23,m24,
        m31,m32,m33,m34,
        m41,m42,m43,m44};
    return mat;
}

bool wMat4IsIdentity(wMatrix4 mat){
    if (memcmp(&wMat4Identity, &mat, sizeof(wMatrix4))==0) {return true;}
    else return false;
}

bool wMat4IsEqualToMat4(wMatrix4 mat1,wMatrix4 mat2){
    if (memcmp(&mat1, &mat2, sizeof(wMatrix4))==0) {return true;}
    else return false;
}

wVec4 wMat4MultiplyByVec4(wMatrix4 mat,wVec4 vec){
    wVec4 Result;
    Result.x=(mat.m11*vec.x)+(mat.m12*vec.y)+(mat.m13*vec.z)+(mat.m14*vec.w);
    Result.y=(mat.m21*vec.x)+(mat.m22*vec.y)+(mat.m23*vec.z)+(mat.m24*vec.w);
    Result.z=(mat.m31*vec.x)+(mat.m32*vec.y)+(mat.m33*vec.z)+(mat.m34*vec.w);
    Result.w=(mat.m41*vec.x)+(mat.m42*vec.y)+(mat.m43*vec.z)+(mat.m44*vec.w);
    return Result;
}

wMatrix4 wMat4MultiplyByMat4(wMatrix4 mat1,wMatrix4 mat2){
    wMatrix4 Result;
    Result.m11=(mat1.m11*mat2.m11)+(mat1.m12*mat2.m21)+(mat1.m13*mat2.m31)+(mat1.m14*mat2.m41);
    Result.m12=(mat1.m11*mat2.m12)+(mat1.m12*mat2.m22)+(mat1.m13*mat2.m32)+(mat1.m14*mat2.m42);
    Result.m13=(mat1.m11*mat2.m13)+(mat1.m12*mat2.m23)+(mat1.m13*mat2.m33)+(mat1.m14*mat2.m43);
    Result.m14=(mat1.m11*mat2.m14)+(mat1.m12*mat2.m24)+(mat1.m13*mat2.m34)+(mat1.m14*mat2.m44);
    
    Result.m21=(mat1.m21*mat2.m11)+(mat1.m22*mat2.m21)+(mat1.m23*mat2.m31)+(mat1.m24*mat2.m41);
    Result.m22=(mat1.m21*mat2.m12)+(mat1.m22*mat2.m22)+(mat1.m23*mat2.m32)+(mat1.m24*mat2.m42);
    Result.m23=(mat1.m21*mat2.m13)+(mat1.m22*mat2.m23)+(mat1.m23*mat2.m33)+(mat1.m24*mat2.m43);
    Result.m24=(mat1.m21*mat2.m14)+(mat1.m22*mat2.m24)+(mat1.m23*mat2.m34)+(mat1.m24*mat2.m44);
    
    Result.m31=(mat1.m31*mat2.m11)+(mat1.m32*mat2.m21)+(mat1.m33*mat2.m31)+(mat1.m34*mat2.m41);
    Result.m32=(mat1.m31*mat2.m12)+(mat1.m32*mat2.m22)+(mat1.m33*mat2.m32)+(mat1.m34*mat2.m42);
    Result.m33=(mat1.m31*mat2.m13)+(mat1.m32*mat2.m23)+(mat1.m33*mat2.m33)+(mat1.m34*mat2.m43);
    Result.m34=(mat1.m31*mat2.m14)+(mat1.m32*mat2.m24)+(mat1.m33*mat2.m34)+(mat1.m34*mat2.m44);
    
    Result.m41=(mat1.m41*mat2.m11)+(mat1.m42*mat2.m21)+(mat1.m43*mat2.m31)+(mat1.m44*mat2.m41);
    Result.m42=(mat1.m41*mat2.m12)+(mat1.m42*mat2.m22)+(mat1.m43*mat2.m32)+(mat1.m44*mat2.m42);
    Result.m43=(mat1.m41*mat2.m13)+(mat1.m42*mat2.m23)+(mat1.m43*mat2.m33)+(mat1.m44*mat2.m43);
    Result.m44=(mat1.m41*mat2.m14)+(mat1.m42*mat2.m24)+(mat1.m43*mat2.m34)+(mat1.m44*mat2.m44);
    return Result;
}

wMatrix4 wMat4RotateOnXAxis(wMatrix4 mat, float angle){
    float cosA=cos(angle*D2R);
    float sinA=sin(angle*D2R);
    wMatrix4 r={1.0f,0.0f, 0.0f,0.0f,
        0.0f,cosA,-sinA,0.0f,
        0.0f,sinA, cosA,0.0f,
        0.0f,0.0f,0.0f,1.0f};
    return wMat4MultiplyByMat4(r, mat);
}

wMatrix4 wMat4RotateOnYAxis(wMatrix4 mat, float angle){
    float cosA=cos(angle*D2R);
    float sinA=sin(angle*D2R);
    wMatrix4 r={cosA,0.0f,sinA,0.0f,
        0.0f,1.0f,0.0f,0.0f,
        -sinA,0.0f,cosA,0.0f,
        0.0f,0.0f,0.0f,1.0f};
    
    return wMat4MultiplyByMat4(r, mat);
}

wMatrix4 wMat4RotateOnZAxis(wMatrix4 mat, float angle){
    float cosA=cos(angle*D2R);
    float sinA=sin(angle*D2R);
    wMatrix4 r={cosA,-sinA,0.0f,0.0f,
        sinA, cosA,0.0f,0.0f,
        0.0f, 0.0f,1.0f,0.0f,
        0.0f, 0.0f,0.0f,1.0f};
    
    return wMat4MultiplyByMat4(r, mat);
}

wMatrix4 wMat4Translate(wMatrix4 mat, float x, float y, float z){
    wMatrix4 m={1.0f,0.0f,0.0f, x,
                0.0f,1.0f,0.0f, y,
                0.0f,0.0f,1.0f, z,
                0.0f,0.0f,0.0f,1.0f};

    return wMat4MultiplyByMat4(m, mat);
    
//    wMatrix4 m=mat;
//    m.m14+=x;
//    m.m24+=y;
//    m.m34+=z;
    return m;
}