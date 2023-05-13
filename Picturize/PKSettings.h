//
//  PKSettingsManager.h
//  PhotoKit
//
//  Created by Wael Youssef on 7/17/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "wViewAPI.h"
#import "iOSwTextureExt.h"


BOOL isFirstTimeRun;

#pragma mark - SettingsManager.
typedef int PKSettingType;
#define PKSettingTypeOptions 0
#define PKSettingTypeBoolean 1

typedef int PKSetting;
#define PKSetting_ColorTheme 0
#define PKSetting_ImageQuality 1
#define PKSetting_SaveOriginalPhoto 2
#define PKSetting_MirrorSelfies 3
#define PKSetting_RememberCameraOptions 4

#define PKSetting_Camera 50
#define PKSetting_CameraGrid 51
#define PKSetting_CameraIsSquare 52
#define PKSetting_CameraFlash 53
#define PKSetting_CameraFilter 54
#define PKSetting_CameraTimer 55

typedef int PKSelection;
#define PKSelection_No 0
#define PKSelection_Yes 1

typedef int PKColorTheme;
#define PKColorTheme_Light 0
#define PKColorTheme_Dark 1

typedef int PKLanguage;
#define PKLanguage_English 0
#define PKLanguage_French 1
#define PKLanguage_Russian 2
#define PKLanguage_German 3
#define PKLanguage_Italian 4
#define PKLanguage_Dutch 5
#define PKLanguage_Japanese 6
#define PKLanguage_Chinese 7
#define PKLanguage_Arabic 8
#define PKLanguage_Spanish 9
#define PKLanguage_Portuguese 10

int SM_SettingsValues[100];
wColor PKForeColor;
wColor PKBackColor;
wColor PKBarColor;

void SM_Setup();
wString*SM_GetTitleAtIndex(unsigned int index);
int SM_GetSelectionForSetting(PKSetting setting);
void SM_SetSelectionForSetting(PKSetting setting, int selection);
void SM_SaveSettings();
void SM_SaveCameraSettings();
void SM_SetApplicationColorTheme(PKColorTheme colorTheme);

unsigned char deviceType;
#define deviceType_iPhone 0
#define deviceType_iPhoneRetina 1
#define deviceType_iPad 2
#define deviceType_iPadRetina 3

#pragma mark - SettingsLayer.
struct _SLMeasurements{
    float OptionSize;
    float OutlineSize;
    float BarSize;
    float BarBorderSize;
    float BarButtonSize;
    float TitleFontSize;
    float SettingFontSize;
    float SelectionMarkSize;
    float ArrowMarkSize;
};

struct _SLMeasurements SLM;

wView*SettingsLayer;
void SL_Setup();
void SL_Display();
void SL_SetColorTheme(PKColorTheme colorTheme);
void SL_AdjustOrientation(wDeviceOrientation orientation);


wView*createAboutBlock();
