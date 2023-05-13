//
//  wScrollView.h
//  PhotoKit
//
//  Created by Wael Youssef on 6/1/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#ifndef wScrollView_H
#define wScrollView_H


typedef struct _wScrollViewContext{
    wView*scrollView;
    wFunction didScrollFunction;
    
    wTimeStamp startTime;
    wTimeStamp currentTime;
    
    wSize contentSize;
    wPoint contentOffset;
    
    wPoint startPoint;
    wPoint currentPoint;
    
    wPoint startValue;
    wPoint midValue;
    wPoint endValue;
    
    float velocityY;
    float previousVelocityY;
    float velocityX;
    float previousVelocityX;
    
    float t;
    float tInc;
    
    bool shouldDeclarate;
    bool scrollsVertically;
    bool scrollsHorizontally;
}wScrollViewContext;

wView*wScrollViewNew();
wView*wScrollViewNewWithFrame(wRect frame);

void wScrollViewSetContentSize(wView*scrollView, wSize size);
void wScrollViewSetContentOffset(wView*scrollView, wPoint offset);
void wScrollViewSetScrollsVertically(wView*scrollView, bool scrollsVertically);
void wScrollViewSetScrollsHorizontally(wView*scrollView, bool scrollsHorizontally);
void wScrollViewSetDidScrollFunction(wView*scrollView, wFunction didScrollFunction);

wSize wScrollViewGetContentSize(wView*scrollView);
wPoint wScrollViewGetContentOffset(wView*scrollView);
bool wScrollViewGetScrollsVertically(wView*scrollView);
bool wScrollViewGetScrollsHorizontally(wView*scrollView);

#endif