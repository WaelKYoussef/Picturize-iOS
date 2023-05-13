//
//  wGestureRecognizer.c
//  PhotoKit
//
//  Created by Wael Youssef on 5/9/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#include "wViewAPI.h"

#define wGCAvilableGestures 6
typedef struct _wGesture{
    unsigned char type;
    wFunction function;
    wView*view;
    wPoint locations[2];
    wTimeStamp time;
}wGesture;

typedef struct _wGestureContext{
    wArray*gestures[wGCAvilableGestures];
    unsigned char gestureCount;
    bool interruptTapGesture;
}wGestureContext;



#pragma mark - wGestureContext.

wGestureContext*wGestureContextNew(){
    wGestureContext*gestureContext=malloc(sizeof(wGestureContext));
    
    for (int i=0; i<wGCAvilableGestures; i++) {
         gestureContext[0].gestures[i]=NULL;}
    gestureContext[0].gestureCount=0;
    gestureContext[0].interruptTapGesture=false;
    return gestureContext;
}

void wGCRelease(wGestureContext*gestureContext){
    if (gestureContext) {
        for (int i=0; i<wGCAvilableGestures; i++) {
            gestureContext[0].gestures[i]=NULL;}
        free(gestureContext),gestureContext=NULL;
    }
}



#pragma mark - wGesture.

wGesture*wGestureNew(int gestureType){
    wGesture*gesture=malloc(sizeof(wGesture));
    
    gesture[0].type=gestureType;
    gesture[0].function=NULL;
    gesture[0].view=NULL;
    gesture[0].time=wTimeStampGet();
    return gesture;
}

void wGestureRelease(wGesture*gesture){
    free(gesture),gesture=NULL;
}

void wViewAddGesture(wView*view, wGesture*gesture){
    if (!view[0].gestureContext) {view[0].gestureContext=wGestureContextNew();}
    
    unsigned char type=gesture[0].type;
    if (!view[0].gestureContext[0].gestures[type]) {
       view[0].gestureContext[0].gestures[type]=wArrayNewWithCapacity(3);
    }
    
    if (wArrayGetCount(view[0].gestureContext[0].gestures[type])<10) {
        wArrayAddItem(view[0].gestureContext[0].gestures[type], gesture);
        
        if (gesture[0].view) {wViewRemoveGesture(gesture[0].view, gesture);}
        gesture[0].view=view;
        view[0].gestureContext[0].gestureCount++;
    }
}


void wViewRemoveGestureOfTypeAtIndex(wView*view, int gestureType, int index){
    if (view[0].gestureContext) {
        if (view[0].gestureContext[0].gestures[gestureType]) {
            wArray*gestures=view[0].gestureContext[0].gestures[gestureType];
            wGesture*gesture=wArrayGetItemAtIndex(gestures, index);
            gesture[0].view=NULL;
            wArrayRemoveItemAtIndex(gestures, index);
            view[0].gestureContext[0].gestureCount--;
            
            if (wArrayGetCount(gestures)<=0) {
                wArrayRelease(gestures);
                view[0].gestureContext[0].gestures[gestureType]=NULL;}
            if (view[0].gestureContext[0].gestureCount<=0) {
                wGCRelease(view[0].gestureContext);
                view[0].gestureContext=NULL;}
        }
    }
}

void wViewRemoveGesture(wView*view,wGesture*gesture){
    wArray*gestures=view[0].gestureContext[0].gestures[gesture[0].type];
    for (int i=wArrayGetCount(gestures)-1; i>=0; i--) {
        if (gesture==wArrayGetItemAtIndex(gestures, i)) {
            unsigned char type=gesture[0].type;
            gesture[0].view=NULL;
            wArrayRemoveItemAtIndex(gestures, i);
            view[0].gestureContext[0].gestureCount--;
            if (wArrayGetCount(gestures)<=0) {
                wArrayRelease(gestures);
                view[0].gestureContext[0].gestures[type]=NULL;}
            if (view[0].gestureContext[0].gestureCount<=0) {
                wGCRelease(view[0].gestureContext);
                view[0].gestureContext=NULL;}
            break;
        }
    }
}

unsigned char wViewGetGesturesCount(wView*view){
    if (!view[0].gestureContext) {return 0;}
    else{return view[0].gestureContext[0].gestureCount;}
}

unsigned char wViewGetGesturesCountOfType(wView*view, unsigned char type){
    if (!view[0].gestureContext) {return 0;}
    else if (!view[0].gestureContext[0].gestures[type]) {return 0;}
    else{return wArrayGetCount(view[0].gestureContext[0].gestures[type]);}
}

wGesture*wViewGetGestureOfTypeAtIndex(wView*view, int gestureType, int index){
    if (!view[0].gestureContext) {return NULL;}
    else if(!view[0].gestureContext[0].gestures[gestureType]){return NULL;}
    else{wArray*gestures=view[0].gestureContext[0].gestures[gestureType];
        return wArrayGetItemAtIndex(gestures, index);}
}

void wViewRemoveAllGestures(wView*view){
    wGesture*gesture=NULL;
    wGestureContext*GC=view[0].gestureContext;
    if (GC) {
    wArray*gestures=NULL;
    for (int x=wGCAvilableGestures-1; x>=0; x--) {
        gestures=view[0].gestureContext[0].gestures[x];
        for (int i=wArrayGetCount(gestures)-1; i>=0; i--) {
        gesture=wArrayGetItemAtIndex(gestures, i);
        gesture[0].view=NULL;}
        wArrayRemoveAllItems(gestures);
        wArrayRelease(gestures);
        view[0].gestureContext[0].gestures[x]=NULL;
    }}
    wGCRelease(view[0].gestureContext);
    view[0].gestureContext=NULL;
}



void wGestureSetFunction(wGesture*gesture, wFunction function){
    gesture[0].function=function;
}
wView*wGestureGetView(wGesture*gesture){
    return gesture[0].view;
}
wTimeStamp wGestureGetTimeStamp(wGesture*gesture){
    return gesture[0].time;
}
unsigned char wGestureGetType(wGesture*gesture){
    return gesture[0].type;
}
wPoint wGestureGetLocationAtIndex(wGesture*gesture, int index){
    if (index==0||index==1) {return gesture[0].locations[index];}
    else{return wPointZero;}
}



#pragma mark - wGestureProcesses.
void wGestureTouchDownProcess(wView*view, wPoint location){
    int count=wViewGetGesturesCountOfType(view, wGestureTouchDown);
    for (int i=count-1; i>=0; i--) {
        wGesture*gesture=wViewGetGestureOfTypeAtIndex(view, wGestureTouchDown, i);
        gesture[0].locations[0]=location;
        gesture[0].time=wTimeStampGet();
        if (gesture[0].function) {gesture[0].function(gesture);}
    }
}

void wGestureTouchUpProcess(wView*view, wPoint location){
    int count=wViewGetGesturesCountOfType(view, wGestureTouchUp);
    for (int i=count-1; i>=0; i--) {
        wGesture*gesture=wViewGetGestureOfTypeAtIndex(view, wGestureTouchUp, i);
        gesture[0].locations[0]=location;
        gesture[0].time=wTimeStampGet();
        if (gesture[0].function) {gesture[0].function(gesture);}
    }
}

void wGestureTapProcessDelay(void*data){
    wGesture*gesture=data;
    if (!gesture[0].view[0].gestureContext[0].interruptTapGesture) {
        if (gesture[0].function) {gesture[0].function(gesture);}
    }
}

void wGestureTapProcess(wView*view, wPoint location){
    int count=wViewGetGesturesCountOfType(view, wGestureTap);
    bool delay=wViewGetGesturesCountOfType(view, wGestureDoubleTap)>0;
    
    for (int i=count-1; i>=0; i--) {
        wGesture*gesture=wViewGetGestureOfTypeAtIndex(view, wGestureTap, i);
        gesture[0].locations[0]=location;
        if (delay) {wTimer*timer=wTimerNew(0.2, wGestureTapProcessDelay, false, gesture);
            wTimerStart(timer);}
        else{gesture[0].time=wTimeStampGet();
            if (gesture[0].function) {gesture[0].function(gesture);}}
    }
}

void wGestureDoubleTapProcessDelay(void*data){
    wView*view=data;
    view[0].gestureContext[0].interruptTapGesture=false;
}


void wGestureDoubleTapProcess(wView*view, wPoint location){
    int count=wViewGetGesturesCountOfType(view, wGestureDoubleTap);
    bool hasGesture=false;
    for (int i=count-1; i>=0; i--) {
        wGesture*gesture=wViewGetGestureOfTypeAtIndex(view, wGestureDoubleTap, i);
        gesture[0].locations[0]=location;
        gesture[0].time=wTimeStampGet();
        if (gesture[0].function) {gesture[0].function(gesture);}
        hasGesture=true;
    }
    if (hasGesture) {
        view[0].gestureContext[0].interruptTapGesture=true;
        wTimer*timer=wTimerNew(0.2, wGestureDoubleTapProcessDelay, false, view);
        wTimerStart(timer);
    }
}

void wGestureSwipeProcess(wView*view, wPoint location){
    int count=wViewGetGesturesCountOfType(view, wGestureSwipe);
    for (int i=count-1; i>=0; i--) {
        wGesture*gesture=wViewGetGestureOfTypeAtIndex(view, wGestureSwipe, i);
        gesture[0].locations[0]=location;
        gesture[0].time=wTimeStampGet();
        if (gesture[0].function) {gesture[0].function(gesture);}
    }
}

void wGestureDoubleSwipeProcess(wView*view, wPoint locations[2]){
    int count=wViewGetGesturesCountOfType(view, wGestureDoubleSwipe);
    for (int i=count-1; i>=0; i--) {
        wGesture*gesture=wViewGetGestureOfTypeAtIndex(view,wGestureDoubleSwipe, i);
        gesture[0].locations[0]=locations[0];
        gesture[0].locations[1]=locations[1];
        gesture[0].time=wTimeStampGet();
        if (gesture[0].function) {gesture[0].function(gesture);}
    }
}
