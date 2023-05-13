//
//  wButton.c
//  PhotoKit
//
//  Created by Wael Youssef on 7/19/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#include "wViewAPI.h"


wView*wButtonNew(){
    wView*button=wViewNew();
    button[0].type=wViewTypeButton;
    wViewSetIsOpaque(button, false);
    wGesture*gesture=wGestureNew(wGestureTap);
    wViewAddGesture(button, gesture);
    wViewSetUserInteractionEnabled(button, true);
    return button;
}

wView*wButtonNewWithFrame(wRect frame){
    wView*button=wViewNewWithFrame(frame);
    button[0].type=wViewTypeButton;
    wViewSetIsOpaque(button, false);
    wGesture*gesture=wGestureNew(wGestureTap);
    wViewAddGesture(button, gesture);
    wViewSetUserInteractionEnabled(button, true);
    return button;
}

void wButtonSetFunction(wView*button, wFunction function){
    wGesture*gesture=wViewGetGestureOfTypeAtIndex(button, wGestureTap, 0);
    wGestureSetFunction(gesture, function);
}