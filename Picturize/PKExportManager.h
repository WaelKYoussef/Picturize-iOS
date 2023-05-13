//
//  PKExportManager.h
//  PhotoKit
//
//  Created by Wael on 9/3/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#ifndef PhotoKit_PKExportManager_h
#define PhotoKit_PKExportManager_h
#import "wViewAPI.h"
#import <Social/Social.h>
#import <MessageUI/MessageUI.h>
#import "PKAlertManager.h"


struct _ELMeasurements{
    int NumberOfRows;
    int NumberOfColumns;
    float OutlineSize;
    float ButtonSize;
};

struct _ELMeasurements ELM;


void EM_Display();
void EM_Hide();
BOOL EM_IsDisplayed();
void EM_Setup();
void EM_SetColorTheme();
void EM_iTunesButton_Action_Confirmed();
#endif
