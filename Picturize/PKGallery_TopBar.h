//
//  PKGallery_TopBar.h
//  PhotoKit
//
//  Created by Wael on 8/25/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#ifndef PhotoKit_PKGallery_TopBar_h
#define PhotoKit_PKGallery_TopBar_h

#include "wViewAPI.h"
#import "iOSwTextureExt.h"

#define InvalidIndex 4294967295

wView*GL_TopBar;
wView*GL_TopBar_LV1;
wView*GL_TopBar_LV2;
wView*GL_TopBar_LV1_Title;
wView*GL_TopBar_LV1_BackButton;
wView*GL_TopBar_LV1_SelectModeButton;
wView*GL_TopBar_LV2_DoneButton;
wView*GL_TopBar_LV2_DeleteButton;
wView*GL_TopBar_LV2_ShareButton;
wView*GL_TopBar_Border;
wFont GL_TopBar_Font;

wArray*GL_TopBar_Buttons;
wTexture*GL_TopBar_LV2_DeleteButton_RemoveIcon;
wTexture*GL_TopBar_LV2_DeleteButton_DeleteIcon;
BOOL GL_TopBar_ShouldUpdateGrids;

void GL_TopBar_Create();
void GL_TopBar_Show();
void GL_TopBar_Hide();
void GL_TopBar_SetTitle(NSString*title);
bool GL_TopBar_BackButton_Action();
void GL_TopBar_DisplayLevel(unsigned char level);
void GL_TopBar_LV1_SelectModeButton_SetIsDisplayed(bool isDisplayed);
void GL_TopBar_LV2_DeleteButton_SetIsEnabled(bool enabled);
void GL_TopBar_LV2_ShareButton_SetIsEnabled(bool enabled);
BOOL GL_TopBar_LV2_DeleteButton_Action_Confirm();
#endif
