//
//  wScrollView.c
//  PhotoKit
//
//  Created by Wael Youssef on 6/1/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#include "wViewAPI.h"

#pragma mark - Scrolling Declaration.
#define declarationDuration 0.5;
int declarationFrames=wFrameRate*declarationDuration;

float unsignFloat(float f){
    if (f<0.0) return -f;
    else return f;
}

float interpolateOffset(float start, float mid, float end, float t) {
    if (start==end) {
        return end;
    }
    else{
        float p1=start+(mid-start)*t;
        float p2=mid+(end-mid)*t;
        return p1+(p2-p1)*t;
    }
}

void wScrollViewBeginDeclaration(wScrollViewContext*SVC, wPoint from, wPoint mid, wPoint to){
    
    SVC[0].t=0.0;
    SVC[0].tInc=1.0/declarationFrames;
    
    SVC[0].startValue=from;
    SVC[0].midValue=mid;
    SVC[0].endValue=to;
    
    SVC[0].shouldDeclarate=true;
    wViewRequestFrames(declarationFrames);
}



void wScrollViewStopY(wScrollViewContext*SVC){
    SVC[0].velocityY=0.0;
    SVC[0].previousVelocityY=0.0;
    if (SVC[0].velocityX==0.0) {
        SVC[0].shouldDeclarate=false;
    }
}
void wScrollViewStopX(wScrollViewContext*SVC){
    SVC[0].velocityX=0.0;
    SVC[0].previousVelocityX=0.0;
    
    if (SVC[0].velocityY==0.0) {
        SVC[0].shouldDeclarate=false;
    }
}

void wScrollViewCheckBounds(wScrollViewContext*SVC){
    float maxY=SVC[0].contentSize.height-SVC[0].scrollView[0].frame.size.height;
    float maxX=SVC[0].contentSize.width-SVC[0].scrollView[0].frame.size.width;
    
    if (SVC[0].contentOffset.y>maxY) {
        SVC[0].contentOffset.y=maxY;wScrollViewStopY(SVC);}
    if (SVC[0].contentOffset.y<0) {
        SVC[0].contentOffset.y=0;wScrollViewStopY(SVC);}
    
    if (SVC[0].contentOffset.x>maxX) {
        SVC[0].contentOffset.x=maxX;wScrollViewStopX(SVC);}
    if (SVC[0].contentOffset.x<0) {
        SVC[0].contentOffset.x=0;wScrollViewStopX(SVC);}
}

void wScrollViewDeclaration(void*data){
    wScrollViewContext*SVC=data;
    if (SVC[0].shouldDeclarate) {
        SVC[0].t+=SVC[0].tInc;
        if (SVC[0].t>1.0) {SVC[0].t=1.0;}
        
        wPoint currentOffset=SVC[0].contentOffset;
        
        if (SVC[0].scrollsVertically) {
        currentOffset.y=interpolateOffset(SVC[0].startValue.y, SVC[0].midValue.y, SVC[0].endValue.y, SVC[0].t);}
        if (SVC[0].scrollsHorizontally) {
        currentOffset.x=interpolateOffset(SVC[0].startValue.x, SVC[0].midValue.x, SVC[0].endValue.x, SVC[0].t);}
        

        SVC[0].contentOffset=currentOffset;
        wScrollViewCheckBounds(SVC);
        
        if (SVC[0].t==1.0) {
            wScrollViewStopY(SVC);
            wScrollViewStopX(SVC);
        }
        if (SVC[0].didScrollFunction) {SVC[0].didScrollFunction(SVC[0].scrollView);}
    }
}


#pragma mark - Scrolling Gestures.
void wScrollViewTouchBegan(void*gesture){
    wScrollViewContext*SVC=wGestureGetView(gesture)[0].scrollContext;
    SVC[0].startPoint=wGestureGetLocationAtIndex(gesture, 0);
    SVC[0].startTime=wGestureGetTimeStamp(gesture);
    SVC[0].currentPoint=SVC[0].startPoint;
    SVC[0].currentTime=SVC[0].startTime;
    SVC[0].shouldDeclarate=false;
    SVC[0].velocityY=0.0;
    SVC[0].velocityX=0.0;
}

void wScrollViewDrag(void*gesture){
    wScrollViewContext*SVC=wGestureGetView(gesture)[0].scrollContext;
    
    wPoint newPoint=wGestureGetLocationAtIndex(gesture, 0);
    wTimeStamp newTime=wGestureGetTimeStamp(gesture);

    if (SVC[0].scrollsVertically) {
        float distance=(newPoint.y-SVC[0].currentPoint.y);
        SVC[0].velocityY=distance/wTimerStampDifference(SVC[0].currentTime, newTime);
        SVC[0].contentOffset.y-=distance;
    }
    if (SVC[0].scrollsHorizontally) {
        float distance=(newPoint.x-SVC[0].currentPoint.x);
        SVC[0].velocityX=distance/wTimerStampDifference(SVC[0].currentTime, newTime);
        SVC[0].contentOffset.x-=distance;
    }
    
    wScrollViewCheckBounds(SVC);
    SVC[0].currentPoint=newPoint;
    SVC[0].currentTime=newTime;
    wViewRequestFrames(1);
    if (SVC[0].didScrollFunction) {SVC[0].didScrollFunction(SVC[0].scrollView);}
}



void wScrollViewTouchEnd(void*gesture){
    wScrollViewContext*SVC=wGestureGetView(gesture)[0].scrollContext;
    
    
    SVC[0].currentPoint=wGestureGetLocationAtIndex(gesture, 0);
    SVC[0].currentTime=wGestureGetTimeStamp(gesture);
    float time=wTimerStampDifference(SVC[0].startTime, SVC[0].currentTime);
    
    
    bool compositeVelocityY=false;
    bool compositeVelocityX=false;
    if (time<0.35){
        if ((SVC[0].velocityY>0.0) == (SVC[0].previousVelocityY>0.0)) {
            float Distance=(SVC[0].currentPoint.y-SVC[0].startPoint.y);
            if (unsignFloat(Distance/time)>2000.0) {
            SVC[0].velocityY+=(SVC[0].previousVelocityY);
            compositeVelocityY=true;}
        }
        
        if ((SVC[0].velocityX>0.0) == (SVC[0].previousVelocityX>0.0)) {
            float Distance=(SVC[0].currentPoint.x-SVC[0].startPoint.x);
            if (unsignFloat(Distance/time)>2000.0) {
                SVC[0].velocityX+=(SVC[0].previousVelocityX);
                compositeVelocityX=true;}
        }
        
    }
    if (!compositeVelocityY) {SVC[0].previousVelocityY=0.0;}
    if (!compositeVelocityX) {SVC[0].previousVelocityX=0.0;}
    
    
    float maxVelocity=50000;
    if (SVC[0].scrollsVertically) {
        if (SVC[0].velocityY>0.0) {
            if (SVC[0].velocityY>maxVelocity){SVC[0].velocityY=maxVelocity;}}
        else{if (SVC[0].velocityY<-maxVelocity){SVC[0].velocityY=-maxVelocity;}}}
    if (SVC[0].scrollsHorizontally) {
        if (SVC[0].velocityX>0.0) {
            if (SVC[0].velocityX>maxVelocity){SVC[0].velocityX=maxVelocity;}}
        else{if (SVC[0].velocityX<-maxVelocity){SVC[0].velocityX=-maxVelocity;}}}
    
    
    SVC[0].previousVelocityY=SVC[0].velocityY;
    SVC[0].previousVelocityX=SVC[0].velocityX;
    float duration=declarationDuration;
    wPoint d=wPointNew(SVC[0].velocityX*duration,SVC[0].velocityY*duration);
    wPoint start=SVC[0].contentOffset;
    wPoint mid=wPointNew(start.x-(d.x*0.9), start.y-(d.y*0.9));
    wPoint end=wPointNew(start.x-d.x, start.y-d.y);
    
    wScrollViewBeginDeclaration(SVC, start,mid,end);
    if (SVC[0].didScrollFunction) {SVC[0].didScrollFunction(SVC[0].scrollView);}
}



#pragma mark - wScrollView General.
void wViewSetScrollVariables(wView*view){
    view[0].scrollContext=malloc(sizeof(wScrollViewContext));
    view[0].type=wViewTypeScrollView;
    view[0].multiTouchEnabled=false;
    view[0].scrollContext[0].didScrollFunction=NULL;
    view[0].scrollContext[0].contentOffset=wPointZero;
    view[0].scrollContext[0].contentSize=wSizeZero;
    view[0].scrollContext[0].scrollView=view;
    view[0].scrollContext[0].scrollsVertically=true;
    view[0].scrollContext[0].scrollsHorizontally=true;
    view[0].scrollContext[0].shouldDeclarate=false;
    
    wViewSetOpenGLData(view, view[0].scrollContext);
    wViewSetRenderOpenGLFunction(view, wScrollViewDeclaration);
    
    wGesture*touchdown=wGestureNew(wGestureTouchDown);
    wGestureSetFunction(touchdown, wScrollViewTouchBegan);
    wViewAddGesture(view, touchdown);
    wGesture*scroll=wGestureNew(wGestureSwipe);
    wGestureSetFunction(scroll, wScrollViewDrag);
    wViewAddGesture(view, scroll);
    wGesture*touchup=wGestureNew(wGestureTouchUp);
    wGestureSetFunction(touchup, wScrollViewTouchEnd);
    wViewAddGesture(view, touchup);
    wViewSetUserInteractionEnabled(view, true);
}

wView* wScrollViewNew(){
    wView*view=wViewNew();
    wViewSetScrollVariables(view);
    return view;
}

wView*wScrollViewNewWithFrame(wRect frame){
    wView*view=wViewNewWithFrame(frame);
    wViewSetScrollVariables(view);
    return view;
}



#pragma mark - wScrollView Getters/Setters.
void wScrollViewSetContentOffset(wView*scrollView, wPoint offset){
    scrollView[0].scrollContext[0].contentOffset=offset;
    if (wViewIsOnScreen(scrollView)) {wViewRequestFrames(1);}}
void wScrollViewSetContentSize(wView*scrollView, wSize size){
    scrollView[0].scrollContext[0].contentSize=size;}
void wScrollViewSetScrollsVertically(wView*scrollView, bool scrollsVertically){
    scrollView[0].scrollContext[0].scrollsVertically=scrollsVertically;}
void wScrollViewSetScrollsHorizontally(wView*scrollView, bool scrollsHorizontally){
    scrollView[0].scrollContext[0].scrollsHorizontally=scrollsHorizontally;}
void wScrollViewSetDidScrollFunction(wView*scrollView, wFunction didScrollFunction){
    scrollView[0].scrollContext[0].didScrollFunction=didScrollFunction;}

wPoint wScrollViewGetContentOffset(wView*scrollView){
    return scrollView[0].scrollContext[0].contentOffset;}
wSize wScrollViewGetContentSize(wView*scrollView){
    return scrollView[0].scrollContext[0].contentSize;}
bool wScrollViewGetScrollsVertically(wView*scrollView){
    return scrollView[0].scrollContext[0].scrollsVertically;}
bool wScrollViewGetScrollsHorizontally(wView*scrollView){
    return scrollView[0].scrollContext[0].scrollsHorizontally;}
