//
//  wAnimation.c
//  PhotoKit
//
//  Created by Wael Youssef on 3/28/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#include "wViewAPI.h"
#pragma mark - wAnimation.



#pragma mark - wAnimationTypes.
typedef struct _wRotationAnimationBlock{
    wMatrix4 originalMatrix;
    wMatrix4 (*rotationFunction)(wMatrix4 mat, float angle);
    float startValue;
    float endValue;
    wView*view;
}wRotationAnimationBlock;
typedef struct _wRotationAnimationBlockArray{
    int count,capacity;
    wRotationAnimationBlock*blocks;
}wRotationAnimationBlockArray;

typedef struct _wCenterAnimationBlock{
    wPoint startValue;
    wPoint endValue;
    wView*view;
}wCenterAnimationBlock;
typedef struct _wCenterAnimationBlockArray{
    int count,capacity;
    wCenterAnimationBlock*blocks;
}wCenterAnimationBlockArray;

typedef struct _wFrameAnimationBlock{
    wRect startValue;
    wRect endValue;
    wView*view;
}wFrameAnimationBlock;
typedef struct _wFrameAnimationBlockArray{
    int count,capacity;
    wFrameAnimationBlock*blocks;
}wFrameAnimationBlockArray;

typedef struct _wFloatAnimationBlock{
    float startValue;
    float endValue;
    float*floatPointer;
}wFloatAnimationBlock;
typedef struct _wFloatAnimationBlockArray{
    int count,capacity;
    wFloatAnimationBlock*blocks;
}wFloatAnimationBlockArray;

typedef struct _wAnimationContext{
    float duration;
    int animationCurve;
    int repeatCount;
    bool repeatIndefinitely;
    bool reverseRepeat;
    wFunction endFunction;
    void*endFunctionData;
    
    float t;
    float inc;
    float incChange;
    float incOriginal;
    bool isReversed;
    
    wRotationAnimationBlockArray rotationAnimationBlocks;
    wCenterAnimationBlockArray centerAnimationBlocks;
    wFrameAnimationBlockArray frameAnimationBlocks;
    wFloatAnimationBlockArray floatAnimationBlocks;
}wAnimationContext;

typedef struct _wAnimationContextArray{
    int count,capacity;
    wAnimationContext*animationContexts;
}wAnimationContextArray;



#pragma mark - wAnimationVariables.
bool isAnimating=false;
bool hasActiveContext=false;
wAnimationContext activeContext;
wAnimationContextArray submittedContexts={0,0,NULL};



#pragma mark - wAnimationContext.
void wAnimationBeginContext(){
    activeContext.duration=1.0;
    activeContext.animationCurve=wAnimationCurveNormal;
    activeContext.repeatCount=0;
    activeContext.repeatIndefinitely=false;
    activeContext.reverseRepeat=false;
    activeContext.endFunction=NULL;
    activeContext.endFunctionData=NULL;
    activeContext.t=0.0;
    activeContext.inc=0.0;
    activeContext.incChange=0.0;
    activeContext.incOriginal=0.0;
    activeContext.isReversed=false;
    
    activeContext.rotationAnimationBlocks.blocks=NULL;
    activeContext.rotationAnimationBlocks.count=0;
    activeContext.rotationAnimationBlocks.capacity=0;
    
    activeContext.centerAnimationBlocks.blocks=NULL;
    activeContext.centerAnimationBlocks.count=0;
    activeContext.centerAnimationBlocks.capacity=0;
    
    activeContext.frameAnimationBlocks.blocks=NULL;
    activeContext.frameAnimationBlocks.count=0;
    activeContext.frameAnimationBlocks.capacity=0;
    
    activeContext.floatAnimationBlocks.blocks=NULL;
    activeContext.floatAnimationBlocks.count=0;
    activeContext.floatAnimationBlocks.capacity=0;
    hasActiveContext=true;
}

bool wAnimationSubmitContext(){
    if (!submittedContexts.animationContexts) {
        submittedContexts.capacity=10;
        submittedContexts.animationContexts=malloc(sizeof(wAnimationContext)*10);
    }else{
        if (submittedContexts.count==submittedContexts.capacity) {
            submittedContexts.capacity=submittedContexts.count+10;
            submittedContexts.animationContexts=realloc(submittedContexts.animationContexts, sizeof(wAnimationContext)*submittedContexts.capacity);
        }
    }
    submittedContexts.animationContexts[submittedContexts.count]=activeContext;
    activeContext.rotationAnimationBlocks.blocks=NULL;
    activeContext.frameAnimationBlocks.blocks=NULL;
    activeContext.centerAnimationBlocks.blocks=NULL;
    activeContext.floatAnimationBlocks.blocks=NULL;
    submittedContexts.count++;
    isAnimating=true;
    
    return true;
}

bool activeContextHasBlocks(){
    int blockCount=activeContext.rotationAnimationBlocks.count+
    activeContext.frameAnimationBlocks.count+
    activeContext.centerAnimationBlocks.count+
    activeContext.floatAnimationBlocks.count;
    if (blockCount>0) {return true;}
    else{return false;}
}
bool wAnimationApplyContext(){
    if (hasActiveContext) {
        if (!activeContextHasBlocks()) {return false;}
        
        float frameCount=floor(wFrameRate*activeContext.duration);
        if (frameCount==0.0) {return false;}
        activeContext.inc=1.0/frameCount;
        
        switch (activeContext.animationCurve) {
            case wAnimationCurveNormal:default:{
                activeContext.incChange=0.0;
                break;}
            case wAnimationCurveEaseIn:{
                activeContext.incChange=(activeContext.inc*2.0)/frameCount;
                activeContext.inc=0.0;
                break;}
            case wAnimationCurveEaseOut:{
                activeContext.incChange=-((activeContext.inc*2.0)/frameCount);
                activeContext.inc=activeContext.inc*2.0;
                break;}
        }
        activeContext.incOriginal=activeContext.inc;
        wAnimationSubmitContext();
        
        unsigned int framesRequired=activeContext.duration*wFrameRate;
        wViewRequestFrames(framesRequired+1);
    }
    hasActiveContext=false;
    
    return true;
}

bool wAnimationIsAnimating(){
    return isAnimating;
}

#pragma mark -
#pragma mark - wAnimationSetter
void wAnimationSetDuration(float duration){
    activeContext.duration=duration;}
void wAnimationSetAnimationCurve(int animationCurve){
    activeContext.animationCurve=animationCurve;}
void wAnimationSetRepeatCount(int repeatCount){
    activeContext.repeatCount=repeatCount;}
void wAnimationSetRepeatIndefinitly(bool repeatIndefinitly){
    activeContext.repeatIndefinitely=repeatIndefinitly;}
void wAnimationSetReverseRepeat(bool reverseRepeat){
    activeContext.reverseRepeat=reverseRepeat;}
void wAnimationSetEndFunction(wFunction function, void*data){
    activeContext.endFunction=function;
    activeContext.endFunctionData=data;}



#pragma mark -
#pragma mark - wAnimationGetter
float wAnimationGetDuration(){
    return activeContext.duration;}
int wAnimationGetAnimationCurve(){
    return activeContext.animationCurve;}
int wAnimationGetRepeatCount(){
    return activeContext.repeatCount;}
bool wAnimationGetRepeatIndefinitly(){
    return activeContext.repeatIndefinitely;}
bool wAnimationGetReverseRepeat(){
    return activeContext.reverseRepeat;}



#pragma mark -
#pragma mark - wViewAnimation
void wAnimationFloat(float*floatPointer,float newFloat){
    if (hasActiveContext && floatPointer[0]!=newFloat) {
        wFloatAnimationBlock block;
        block.startValue=floatPointer[0];
        block.endValue=newFloat;
        block.floatPointer=floatPointer;
        
        if (activeContext.floatAnimationBlocks.blocks==NULL) {
            activeContext.floatAnimationBlocks.blocks=malloc(0);}
        
        if (activeContext.floatAnimationBlocks.count==activeContext.floatAnimationBlocks.capacity) {
            activeContext.floatAnimationBlocks.capacity=activeContext.floatAnimationBlocks.count+10;
            activeContext.floatAnimationBlocks.blocks=realloc(activeContext.floatAnimationBlocks.blocks, sizeof(wFloatAnimationBlock)*activeContext.floatAnimationBlocks.capacity);
        }
        
        activeContext.floatAnimationBlocks.blocks[activeContext.floatAnimationBlocks.count]=block;
        activeContext.floatAnimationBlocks.count++;
    }
}

void wViewAnimateFrame(wView*view,wRect frame){
    bool condition=!wRectIsEqualToRect(view[0].frame, frame);
    if (hasActiveContext && condition) {
        wFrameAnimationBlock block;
        block.startValue=view[0].frame;
        block.endValue=frame;
        block.view=view;
        
        if (activeContext.frameAnimationBlocks.blocks==NULL) {
            activeContext.frameAnimationBlocks.blocks=malloc(0);}
        
        if (activeContext.frameAnimationBlocks.count==activeContext.frameAnimationBlocks.capacity) {
            activeContext.frameAnimationBlocks.capacity=activeContext.frameAnimationBlocks.count+10;
            activeContext.frameAnimationBlocks.blocks=realloc(activeContext.frameAnimationBlocks.blocks, sizeof(wFrameAnimationBlock)*activeContext.frameAnimationBlocks.capacity);
        }
        
        activeContext.frameAnimationBlocks.blocks[activeContext.frameAnimationBlocks.count]=block;
        activeContext.frameAnimationBlocks.count++;
    }
}

void wViewAnimateCenter(wView*view,wPoint centerPoint){
    bool condition=!wPointIsEqualToPoint(view[0].centerPoint, centerPoint);
    if (hasActiveContext && condition) {
        wCenterAnimationBlock block;
        block.startValue=view[0].centerPoint;
        block.endValue=centerPoint;
        block.view=view;
        
        if (activeContext.centerAnimationBlocks.blocks==NULL) {
            activeContext.centerAnimationBlocks.blocks=malloc(0);}
        
        if (activeContext.centerAnimationBlocks.count==activeContext.centerAnimationBlocks.capacity) {
            activeContext.centerAnimationBlocks.capacity=activeContext.centerAnimationBlocks.count+10;
            activeContext.centerAnimationBlocks.blocks=realloc(activeContext.centerAnimationBlocks.blocks, sizeof(wCenterAnimationBlock)*activeContext.centerAnimationBlocks.capacity);
        }
        
        activeContext.centerAnimationBlocks.blocks[activeContext.centerAnimationBlocks.count]=block;
        activeContext.centerAnimationBlocks.count++;
    }
}

void wViewAnimateRotation(wView*view,float angle,wMatrix4 (*rotationFunction)(wMatrix4 mat, float angle)){
    if (hasActiveContext) {
        wRotationAnimationBlock block;
        block.originalMatrix=view[0].matrix;
        block.startValue=0.0;
        block.endValue=angle;
        block.view=view;
        block.rotationFunction=rotationFunction;
        
        if (activeContext.rotationAnimationBlocks.blocks==NULL) {
            activeContext.rotationAnimationBlocks.blocks=malloc(0);}
        
        if (activeContext.rotationAnimationBlocks.count==activeContext.rotationAnimationBlocks.capacity) {
            activeContext.rotationAnimationBlocks.capacity=activeContext.rotationAnimationBlocks.count+10;
            activeContext.rotationAnimationBlocks.blocks=realloc(activeContext.rotationAnimationBlocks.blocks, sizeof(wRotationAnimationBlock)*activeContext.rotationAnimationBlocks.capacity);
        }
        
        activeContext.rotationAnimationBlocks.blocks[activeContext.rotationAnimationBlocks.count]=block;
        activeContext.rotationAnimationBlocks.count++;
    }
}

void wViewAnimateRotationOnXAxis(wView*view,float angle){
    wViewAnimateRotation(view, angle, wMat4RotateOnXAxis);
}
void wViewAnimateRotationOnYAxis(wView*view,float angle){
    wViewAnimateRotation(view, angle, wMat4RotateOnYAxis);
}
void wViewAnimateRotationOnZAxis(wView*view,float angle){
    wViewAnimateRotation(view, angle, wMat4RotateOnZAxis);
}

void wViewAnimateBackgroundColor(wView*view,wColor backgroundColor){
    wColor*vBackgroundColor=&view[0].backgroundColor;
    wAnimationFloat(&vBackgroundColor[0].r, backgroundColor.r);
    wAnimationFloat(&vBackgroundColor[0].g, backgroundColor.g);
    wAnimationFloat(&vBackgroundColor[0].b, backgroundColor.b);
    wAnimationFloat(&vBackgroundColor[0].a, backgroundColor.a);
}

void wViewAnimateAnchorPoint(wView*view,wAnchor anchorPoint){
    wAnchor*vAnchorPoint=&view[0].anchorPoint;;
    wAnimationFloat(&vAnchorPoint[0].x, anchorPoint.x);
    wAnimationFloat(&vAnchorPoint[0].y, anchorPoint.y);
}

void wViewAnimateAlpha(wView*view,float alpha){
    float*vAlpha=&view[0].alpha;
    wAnimationFloat(&vAlpha[0], alpha);
}

void wViewAnimateMatrix(wView*view,wMatrix4 mat){
    wMatrix4*vMat=&view[0].matrix;
    wAnimationFloat(&vMat[0].m11, mat.m11);
    wAnimationFloat(&vMat[0].m12, mat.m12);
    wAnimationFloat(&vMat[0].m13, mat.m13);
    wAnimationFloat(&vMat[0].m14, mat.m14);
    
    wAnimationFloat(&vMat[0].m21, mat.m21);
    wAnimationFloat(&vMat[0].m22, mat.m22);
    wAnimationFloat(&vMat[0].m23, mat.m23);
    wAnimationFloat(&vMat[0].m24, mat.m24);
    
    wAnimationFloat(&vMat[0].m31, mat.m31);
    wAnimationFloat(&vMat[0].m32, mat.m32);
    wAnimationFloat(&vMat[0].m33, mat.m33);
    wAnimationFloat(&vMat[0].m34, mat.m34);
    
    wAnimationFloat(&vMat[0].m41, mat.m41);
    wAnimationFloat(&vMat[0].m42, mat.m42);
    wAnimationFloat(&vMat[0].m43, mat.m43);
    wAnimationFloat(&vMat[0].m44, mat.m44);
}

void wAnimationMatrix(wMatrix4*mat,wMatrix4 newMat){
    wAnimationFloat(&mat[0].m11, newMat.m11);
    wAnimationFloat(&mat[0].m12, newMat.m12);
    wAnimationFloat(&mat[0].m13, newMat.m13);
    wAnimationFloat(&mat[0].m14, newMat.m14);
    
    wAnimationFloat(&mat[0].m21, newMat.m21);
    wAnimationFloat(&mat[0].m22, newMat.m22);
    wAnimationFloat(&mat[0].m23, newMat.m23);
    wAnimationFloat(&mat[0].m24, newMat.m24);
    
    wAnimationFloat(&mat[0].m31, newMat.m31);
    wAnimationFloat(&mat[0].m32, newMat.m32);
    wAnimationFloat(&mat[0].m33, newMat.m33);
    wAnimationFloat(&mat[0].m34, newMat.m34);
    
    wAnimationFloat(&mat[0].m41, newMat.m41);
    wAnimationFloat(&mat[0].m42, newMat.m42);
    wAnimationFloat(&mat[0].m43, newMat.m43);
    wAnimationFloat(&mat[0].m44, newMat.m44);
}

#pragma mark -
#pragma mark - wAnimationUpdate.
void wAnimationTerminateContext(wAnimationContext*context, int index){
    
    if (context[0].endFunction) {context[0].endFunction(context[0].endFunctionData);}
    
    if (context[0].repeatCount<=0 && !context[0].repeatIndefinitely) {
        if (context[0].frameAnimationBlocks.blocks) {
            free(context[0].frameAnimationBlocks.blocks);}
        if (context[0].centerAnimationBlocks.blocks) {
            free(context[0].centerAnimationBlocks.blocks);}
        if (context[0].floatAnimationBlocks.blocks) {
            free(context[0].floatAnimationBlocks.blocks);}
        
        memmove(&submittedContexts.animationContexts[index], &submittedContexts.animationContexts[index+1], sizeof(wAnimationContext)*(submittedContexts.count-index-1));
        
        submittedContexts.count--;
        
        if (submittedContexts.count<=0) {isAnimating=false;}
    }else{
        context[0].repeatCount--;
        if (context[0].reverseRepeat) {
            context[0].inc=-context[0].inc;
            context[0].isReversed=!context[0].isReversed;
        }else{
            context[0].t=0.0;
            context[0].inc=context[0].incOriginal;
        }
    }
}

void wAnimationUpdate(){
    for (int i=submittedContexts.count-1; i>=0; i--) {
        wAnimationContext*context=&submittedContexts.animationContexts[i];
        
        context[0].t+=context[0].inc;
        if (context[0].t>1.0) {context[0].t=1.0;}
        if (context[0].t<0.0) {context[0].t=0.0;}
        
        for (int y=0; y<context[0].frameAnimationBlocks.count; y++) {
            wFrameAnimationBlock*frameBlock=&context[0].frameAnimationBlocks.blocks[y];
            wViewSetFrame(frameBlock[0].view, interpolateRect(frameBlock[0].startValue, frameBlock[0].endValue, context[0].t));
        }
        
        for (int y=0; y<context[0].centerAnimationBlocks.count; y++) {
            wCenterAnimationBlock*centerBlock=&context[0].centerAnimationBlocks.blocks[y];
            wViewSetCenterPoint(centerBlock[0].view, interpolatePoint(centerBlock[0].startValue, centerBlock[0].endValue, context[0].t));
        }
        
        for (int y=0; y<context[0].floatAnimationBlocks.count; y++) {
            wFloatAnimationBlock*floatBlock=&context[0].floatAnimationBlocks.blocks[y];
            floatBlock[0].floatPointer[0]=interpolateFloat(floatBlock[0].startValue, floatBlock[0].endValue, context[0].t);
        }
        
        for (int y=0; y<context[0].rotationAnimationBlocks.count; y++) {
            wRotationAnimationBlock*rotationBlock=&context[0].rotationAnimationBlocks.blocks[y];
            float angle=interpolateFloat(rotationBlock[0].startValue, rotationBlock[0].endValue, context[0].t);
            rotationBlock[0].view[0].matrix=(*rotationBlock[0].rotationFunction)(rotationBlock[0].originalMatrix, angle);
        }
        
        float target=1.0;
        if (context[0].isReversed) {target=0.0;}
        if (context[0].t==target) {
            wAnimationTerminateContext(context,i);}
        else{context[0].inc+=context[0].incChange;}
    }
}


