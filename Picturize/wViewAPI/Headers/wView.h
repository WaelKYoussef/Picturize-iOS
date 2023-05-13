//
//  wView.h
//  PhotoKit
//
//  Created by Wael Youssef on 4/5/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#ifndef wView_H
#define wView_H


#pragma mark -
#pragma mark - wView.

#define wAutoLayoutStaticWidth 0
#define wAutoLayoutStaticHeight 0
#define wAutoLayoutProportionateToSuperView 1
#define wAutoLayoutStretchToSuperView 2
#define wAutoLayoutProportionateToHeight 3
#define wAutoLayoutProportionateToWidth 3


#define wPerspectiveOptionLocal 0
#define wPerspectiveOptionSuperView 1
#define wPerspectiveOptionDevice 2

#define wViewTypeNormal 0
#define wViewTypeScrollView 1
#define wViewTypeTableView 2
#define wViewTypeButton 3

typedef unsigned char wViewBackgroundImageDisplayMode;
#define wViewBackgroundImageDisplayModeStretch 0
#define wViewBackgroundImageDisplayModeCrop 1

#include "wScrollView.h"
#include "wTableView.h"


typedef struct _wVertex{ wVec4 location;wVec2 textureCoordinates; }wVertex;

typedef struct _wVertexArray{ wVertex*list;int count,capacity; }wVertexArray;

typedef struct _wAutoLayoutOptions{
    wAnchor anchor;
    int widthOption;
    int heightOption;
}wAutoLayoutOptions;



typedef struct _wViewRenderingContext{
    wView*clippingView;
    wVertexArray vertices;
    wMatrix4 matrix;
    wRect frame;
    wAnchor anchorPoint;
    wPoint centerPoint;
    float alpha;
    float texCoordinates[4];
    bool branchHasTransformation;
}wViewRenderingContext;

typedef struct _wView{
    wGestureContext*gestureContext;
    wScrollViewContext*scrollContext;
    wTableViewContext*tableContext;
    
    wView*superView;
    wArray*subviews;
    wTexture*backgroundImage;
    wTexture*backgroundMask;
    
    void*openGLData;
    wFunction renderOpenGLCode;
    
    wMatrix4 matrix;
    wRect frame;
    wColor backgroundColor;
    wAnchor anchorPoint;
    wPoint centerPoint;
    wAutoLayoutOptions autoLayoutOptions;
    
    
    
    float alpha;
    int tag;
    int  type;
    int perspectiveOption;
    wViewBackgroundImageDisplayMode backgroundImageDisplayMode;
    
    bool isOpaque;
    bool isHidden;
    bool clipsSubviews;
    bool perspectiveProjection;
    bool resizesSubviews;
    bool userInteractioEnabled;
    bool multiTouchEnabled;
    bool shouldDraw;
    
    wViewRenderingContext renderingContext;
}wView;





struct _wViewDefaults{
    wMatrix4 matrix;
    wColor backgroundColor;
    wAutoLayoutOptions autoLayoutOptions;
    float alpha;
    int perspectiveOption;
    wViewBackgroundImageDisplayMode backgroundImageDisplayMode;
    bool isOpaque;
    bool isHidden;
    bool clipsSubviews;
    bool perspectiveProjection;
    bool resizesSubviews;
    bool userInteractioEnabled;
    bool multiTouchEnabled;
    bool shouldDraw;
};







wView*const wRootView;
struct _wViewDefaults wViewDefaults;


#pragma mark - wViewGeneral.
wView*wViewNew();
wView*wViewNewWithFrame(wRect frame);
void wViewAddSubviewAtIndex(wView*view,wView*subview,int index);
void wViewAddSubview(wView*view,wView*subview);
void wViewRemoveSubviewAtIndex(wView*view,int index);
void wViewRemoveSubview(wView*view,wView*subview);
void wViewRemoveAllSubviews(wView*view);
void wViewRemoveFromSuperView(wView*view);
void wViewRotateOnXAxis(wView*view, float angle);
void wViewRotateOnYAxis(wView*view, float angle);
void wViewRotateOnZAxis(wView*view, float angle);
void wViewHide(wView*view);
void wViewShow(wView*view);
void wViewRelease(wView*view);

#pragma mark - wViewSetter.
void wViewSetBackgroundColor(wView*view, wColor backgroundColor);
void wViewSetFrame(wView*view, wRect frame);
void wViewSetCenterPoint(wView*view, wPoint centerPoint);
void wViewSetAnchorPoint(wView*view, wPoint anchorPoint);
void wViewSetIsHidden(wView*view, bool hidden);
void wViewSetIsOpaque(wView*view, bool isOpaque);
void wViewSetAlpha(wView*view, float alpha);
void wViewSetClipsSubviews(wView*view, bool clipsSubviews);
void wViewSetMatrix(wView*view, wMatrix4 mat);
void wViewSetPerspectiveProjection(wView*view, bool perspectiveProjection);
void wViewSetPerspectiveOption(wView*view, int perspectiveOption);
void wViewSetTag(wView*view, int tag);
void wViewSetResizesSubviews(wView*view, bool resizesSubviews);
void wViewSetAutoLayoutOptions(wView*view, wAutoLayoutOptions options);
void wViewSetRenderOpenGLFunction(wView*view, wFunction renderOpenGLFunction);
void wViewSetOpenGLData(wView*view, void*data);
void wViewSetUserInteractionEnabled(wView*view, bool userInteractioEnabled);
void wViewSetBackgroundImage(wView*view, wTexture*image);
void wViewSetBackgroundMask(wView*view, wTexture*mask);
void wViewSetMultiTouchEnabled(wView*view, bool multiTouchEnabled);
void wViewSetShouldDraw(wView*view, bool shouldDraw);
void wViewSetBackgroundImageDisplayMode(wView*view, wViewBackgroundImageDisplayMode mode);

#pragma mark - wViewGetter.
wColor wViewGetBackgroundColor(wView*view);
wRect wViewGetFrame(wView*view);
wRect wViewGetAbsoluteFrame(wView*view);
wPoint wViewGetCenterPoint(wView*view);
wPoint wViewGetAnchorPoint(wView*view);
bool wViewGetIsHidden(wView*view);
bool wViewGetIsOpaque(wView*view);
bool wViewGetAlpha(wView*view);
bool wViewGetClipsSubviews(wView*view);
wMatrix4 wViewGetMatrix(wView*view);
wMatrix4 wViewGetAbsoluteMatrix(wView*view);
bool wViewGetPerspectiveProjection(wView*view);
int wViewGetPerspectiveOption(wView*view);
int wViewGetType(wView*view);
int wViewGetTag(wView*view);
bool wViewGetResizesSubviews(wView*view);
wAutoLayoutOptions wViewGetAutoLayoutOptions(wView*view);
bool wViewGetUserInteractionEnabled(wView*view);
int wViewGetSubviewCount(wView*view);
wView*wViewGetSubviewAtIndex(wView*view, int index);
wTexture*wViewGetBackgroundImage(wView*view);
wTexture*wViewGetBackgroundMask(wView*view);
wArray*wViewGetSubviewArray(wView*view);
void*wViewGetOpenGLData(wView*view);
bool wViewGetMultiTouchEnabled(wView*view);
bool wViewGetShouldDraw(wView*view);
wViewBackgroundImageDisplayMode wViewGetBackgroundImageDisplayMode(wView*view);


#pragma mark - SystemReserved.
bool wViewRenderRoot();
void wViewSetupRenderingComponents(wSize screenSize);

void wViewRequestFrames(unsigned int frameCount);
void wViewSetRenderIndefinitly(bool renderIndefinitly);
bool wViewGetRenderIndefinitly();
bool wViewIsOnScreen(wView*view);

#pragma mark - wScreenDimentionsGetter.
float wScreenGetWidth();
float wScreenGetHeight();
wSize wScreenGetSize();
wPoint wScreenGetCenter();
wRect wScreenGetFrame();
wRect wScreenGetPortraitFrame();
wRect wScreenGetLandscapeFrame();


const wAutoLayoutOptions wAutoLayoutNone;
void printfmat4(wMatrix4 mat);

#endif
