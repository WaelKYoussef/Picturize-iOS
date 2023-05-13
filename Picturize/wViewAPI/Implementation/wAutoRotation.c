//
//  wAutoRotation.c
//  PhotoKit
//
//  Created by Wael Youssef on 3/31/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//
#include "wViewAPI.h"

#pragma mark -
#pragma mark - wAutoRotation.
#pragma mark - wAutoRotationTypes.
typedef struct _wAutoRotationOptions{
    void (*beginFunction)(wDeviceOrientation);
    void (*endFunction)(wDeviceOrientation);
    void*beginFunctionData;
    void*endFunctionData;
    float duration;
    bool isRotating;
    bool animateResizing;
    bool animateRotation;
    bool allowsPortraitUp;
    bool allowsPortraitDown;
    bool allowsLandscapeLeft;
    bool allowsLandscapeRight;
}wAutoRotationOptions;

typedef struct _wAutoRotationAnimationVariables{
    float t;
    float tInc;
    float midValue;
    float endValue;
    wMatrix4 originalMatrix;
    wRect originalFrame;
    wRect targetFrame;
    bool animateResizing;
    bool animateRotation;
    bool shouldAnimate;
}wAutoRotationAnimationVariables;

#pragma mark - wAutoRotationVariables.
wDeviceOrientation currentDeviceOrientation;
wDeviceOrientation previousOrientation;
wAutoRotationAnimationVariables animeVal;
wAutoRotationOptions autoRotationOptions={NULL,NULL,NULL,NULL,0.3,false,false,true,true,true,true,true};

wMatrix4 wAutoRotationMatrix={
    1.0f,0.0f,0.0f,0.0f,
    0.0f,1.0f,0.0f,0.0f,
    0.0f,0.0f,1.0f,0.0f,
    0.0f,0.0f,0.0f,1.0f};

const wMatrix4 wAutoRotationMatrixPortraitUp={
    1.0f,0.0f,0.0f,0.0f,
    0.0f,1.0f,0.0f,0.0f,
    0.0f,0.0f,1.0f,0.0f,
    0.0f,0.0f,0.0f,1.0f};

const wMatrix4 wAutoRotationMatrixPortraitDown={
    -1.0f,0.0f,0.0f,0.0f,
    0.0f,-1.0f,0.0f,0.0f,
    0.0f,0.0f,1.0f,0.0f,
    0.0f,0.0f,0.0f,1.0f};

const wMatrix4 wAutoRotationMatrixLandscapeLeft={
    0.0f,1.0f,0.0f,0.0f,
    -1.0f,0.0f,0.0f,0.0f,
    0.0f,0.0f,1.0f,0.0f,
    0.0f,0.0f,0.0f,1.0f};

const wMatrix4 wAutoRotationMatrixLandscapeRight={
    0.0f,-1.0f,0.0f,0.0f,
    1.0f,0.0f,0.0f,0.0f,
    0.0f,0.0f,1.0f,0.0f,
    0.0f,0.0f,0.0f,1.0f};



#pragma mark -
#pragma mark - wAutoRotationSetter.
void wAutoRotationSetDuration(float duration){
    autoRotationOptions.duration=duration;}
void wAutoRotationSetAnimateRotation(bool animateRotation){
    autoRotationOptions.animateRotation=animateRotation;}
void wAutoRotationSetAnimateResizing(bool animateResizing){
    autoRotationOptions.animateResizing=animateResizing;}
void wAutoRotationSetAllowsOrientation(wDeviceOrientation orientation, bool isAllowed){
    switch (orientation) {
    case wDeviceOrientationPortraitUp:{
    autoRotationOptions.allowsPortraitUp=isAllowed;break;}
    case wDeviceOrientationPortraitDown:{
    autoRotationOptions.allowsPortraitDown=isAllowed;break;}
    case wDeviceOrientationLandscapeLeft:{
    autoRotationOptions.allowsLandscapeLeft=isAllowed;break;}
    case wDeviceOrientationLandscapeRight:{
    autoRotationOptions.allowsLandscapeRight=isAllowed;break;}}
}

void wAutoRotationSetBeginFunction(void (*beginFunction)(wDeviceOrientation)){
    autoRotationOptions.beginFunction=beginFunction;}
void wAutoRotationSetEndFunction(void (*endFunction)(wDeviceOrientation)){
    autoRotationOptions.endFunction=endFunction;}
void wAutoRotationSetBeginFunctionData(void*beginFunctionData){
    autoRotationOptions.beginFunctionData=beginFunctionData;}
void wAutoRotationSetEndFunctionData(void*endFunctionData){
    autoRotationOptions.endFunctionData=endFunctionData;}



#pragma mark -
#pragma mark - wAutoRotationGetter.
float wAutoRotationGetDuration(){
    return autoRotationOptions.duration;}
bool wAutoRotationGetAnimateResizing(){
    return autoRotationOptions.animateResizing;}
bool wAutoRotationGetAnimateRotation(){
    return autoRotationOptions.animateRotation;}
bool wAutoRotationGetAllowsOrientation(wDeviceOrientation orientation){
    switch (orientation) {
    case wDeviceOrientationPortraitUp:{
    return autoRotationOptions.allowsPortraitUp;break;}
    case wDeviceOrientationPortraitDown:{
    return autoRotationOptions.allowsPortraitDown;break;}
    case wDeviceOrientationLandscapeLeft:{
    return autoRotationOptions.allowsLandscapeLeft;break;}
    case wDeviceOrientationLandscapeRight:{
    return autoRotationOptions.allowsLandscapeRight;break;}
    default:{return false;break;}}}
bool wAutoRotationGetIsRotating(){
    return autoRotationOptions.isRotating;}



#pragma mark -
#pragma mark - wAutoRotationAnimation.
void wAutoRotationSetRotationMatrix(){
    switch (wDeviceGetOrientation()) {
    case wDeviceOrientationPortraitUp:{
    wAutoRotationMatrix=wAutoRotationMatrixPortraitUp;break;}
    case wDeviceOrientationPortraitDown:{
    wAutoRotationMatrix=wAutoRotationMatrixPortraitDown;break;}
    case wDeviceOrientationLandscapeLeft:{
    wAutoRotationMatrix=wAutoRotationMatrixLandscapeLeft;break;}
    case wDeviceOrientationLandscapeRight:{
    wAutoRotationMatrix=wAutoRotationMatrixLandscapeRight;break;}
    default:{wAutoRotationMatrix=wAutoRotationMatrixPortraitUp;break;}}
}

void wAutoRotationEndRotation(){
    wAutoRotationSetRotationMatrix();
    
    animeVal.shouldAnimate=false;
    autoRotationOptions.isRotating=false;
    if (autoRotationOptions.endFunction) {
        autoRotationOptions.endFunction(previousOrientation);}
    wViewRequestFrames(1);
}

void wAutoRotationAnimationSetAngles(wMatrix4 originalMatrix, float mid, float to){
    animeVal.originalMatrix=originalMatrix;
    animeVal.midValue=mid;
    animeVal.endValue=to;
}

void wAutoRotationAnimationSetFrames(wRect originalFrame, wRect targetFrame){
    animeVal.originalFrame=originalFrame;
    animeVal.targetFrame=targetFrame;
}


void wAutoRotationAnimationBegin(float duration){
    animeVal.t=0.0;
    animeVal.tInc=1.0/(duration*wFrameRate);
    
    animeVal.shouldAnimate=true;
    wViewRequestFrames((duration*wFrameRate));
}

void wAutoRotationAnimation(){
    if (animeVal.shouldAnimate) {
        animeVal.t+=animeVal.tInc;
        if (animeVal.t>1.0) {animeVal.t=1.0;}
        
        if (animeVal.animateResizing) {
            wRect newFrame=interpolateRect(animeVal.originalFrame, animeVal.targetFrame, animeVal.t);
            wViewSetFrame(wRootView, newFrame);
        }
        if (animeVal.animateRotation) {
            float p1=animeVal.midValue*animeVal.t;
            float p2=animeVal.midValue+(animeVal.endValue-animeVal.midValue)*animeVal.t;
            float angle=p1+(p2-p1)*animeVal.t;
            
            wAutoRotationMatrix=wMat4RotateOnZAxis(animeVal.originalMatrix, floorf(angle));
        }
        
        if (animeVal.t==1.0) {
            wAutoRotationEndRotation();
        }
    }
}

#pragma mark -
#pragma mark - wDeviceOrientationSetter/Getter.
bool wDeviceSetOrientation(wDeviceOrientation orientation){
    if (currentDeviceOrientation!=orientation && !autoRotationOptions.isRotating) {
        if (!wAutoRotationGetAllowsOrientation(orientation)) {return false;}
        
        if (autoRotationOptions.beginFunction) {
        autoRotationOptions.beginFunction(orientation);}
        previousOrientation=orientation;
        
        float angle=90.0;
        switch (currentDeviceOrientation) {
        case wDeviceOrientationPortraitUp:{
        if (orientation==wDeviceOrientationPortraitDown) {angle=180.0;}
        else if(orientation==wDeviceOrientationLandscapeLeft){angle=-90.0;}break;}
        case wDeviceOrientationPortraitDown:{
        if (orientation==wDeviceOrientationPortraitUp) {angle=180.0;}
        else if(orientation==wDeviceOrientationLandscapeRight){angle=-90.0;}break;}
        case wDeviceOrientationLandscapeLeft:{
        if (orientation==wDeviceOrientationLandscapeRight) {angle=180.0;}
        else if (orientation==wDeviceOrientationPortraitDown){angle=-90.0;}break;}
        case wDeviceOrientationLandscapeRight:{
        if (orientation==wDeviceOrientationLandscapeLeft){angle=180.0;}
        else if (orientation==wDeviceOrientationPortraitUp){angle=-90.0;}break;}}
        
        autoRotationOptions.isRotating=true;
        bool fromPortrait=wDeviceOrientationIsPortrait(currentDeviceOrientation);
        bool toPortrait=wDeviceOrientationIsPortrait(orientation);
        
        wRect newFrame;
        if (toPortrait) {newFrame=wScreenGetPortraitFrame();}
        else{newFrame=wScreenGetLandscapeFrame();
            newFrame.origin.y=(newFrame.size.width-newFrame.size.height)/2.0;
            newFrame.origin.x=-newFrame.origin.y;}
        currentDeviceOrientation=orientation;

        animeVal.animateResizing=false;
        animeVal.animateRotation=false;
        if (autoRotationOptions.animateResizing) {
            if (fromPortrait!=toPortrait) {
                animeVal.animateResizing=true;
                wAutoRotationAnimationSetFrames(wRootView[0].frame, newFrame);}
        }else{if (fromPortrait!=toPortrait) {wViewSetFrame(wRootView, newFrame);}}
        
        if (autoRotationOptions.animateRotation) {
            animeVal.animateRotation=true;
            wAutoRotationAnimationSetAngles(wAutoRotationMatrix, angle*0.8, angle);
        }else {wAutoRotationSetRotationMatrix();}
        
        if (animeVal.animateResizing ||  animeVal.animateRotation) {
            float duration=autoRotationOptions.duration;
            if (fromPortrait==toPortrait) {duration*=2.0;}
            wAutoRotationAnimationBegin(duration);}
        else{wAutoRotationEndRotation();}
    }
    return true;
}

wDeviceOrientation wDeviceGetOrientation(){
    return currentDeviceOrientation;
}
 
bool wDeviceOrientationIsPortrait(wDeviceOrientation orientation){
    if (orientation==wDeviceOrientationPortraitDown ||
        orientation==wDeviceOrientationPortraitUp) {
        return true;}
    else return false;
}
bool wDeviceOrientationIsLandscape(wDeviceOrientation orientation){
    if (orientation==wDeviceOrientationPortraitDown ||
        orientation==wDeviceOrientationPortraitUp) {
        return false;}
    else return true;
}
