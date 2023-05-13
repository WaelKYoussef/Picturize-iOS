//
//  PKCameraController.h
//  PhotoKit
//
//  Created by Wael Youssef on 6/18/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#import <AVFoundation/AVFoundation.h>
#import "iOSwOpenGLView.h"
#import "PKSettings.h"

#pragma mark - CameraManager.
@interface PKCameraController :
NSObject <AVCaptureVideoDataOutputSampleBufferDelegate >@end

typedef int PKCamera;
#define PKCamera_Back 0
#define PKCamera_Front 1

typedef int PKCameraResolution;
#define PKCameraResolution_High 0
#define PKCameraResolution_Medium 1
#define PKCameraResolution_Low 2

void CM_Setup(wTexture*targetTexture);
void CM_StartCamera();
void CM_StopCamera();
bool CM_CameraIsRunning();

void CM_SetCamera(PKCamera camera);
void CM_SetCameraResolution(PKCameraResolution cameraResolution);
void CM_SetCameraWithResolution(PKCamera camera, PKCameraResolution cameraResolution);

PKCamera CM_GetCurrentCamera();
bool CM_CameraIsAvailable(PKCamera camera);
bool CM_IsFlashAvailableForCamera(PKCamera camera);
bool CM_HasOnlyBackCamera();
bool CM_HasBothCameras();
bool CM_HasNoCamera();



#pragma mark - CameraLayer.
struct _CLMeasurements{
    int FiltersPerRow;
    float OutlineSize;
    float BarSize;
    float BarButtonSize;
    float ShutterSize;
    float GridBarSize;
    float StoreFontSize;
    float StoreIconSize;
    float restoreButtonSize;
};

struct _CLMeasurements CLM;

wView*CameraLayer;

void CL_Setup();
void CL_SetTimerButtonSeconds(unsigned char Seconds);
void CL_SetColorTheme(PKColorTheme colorTheme);
void CL_DisplayAndStartCamera();
void CL_Bar_RotateButtons(wDeviceOrientation orientation);
void CL_FilterGrid_Store_UpdatePackAtIndex(int index);
void CL_FilterGrid_Store_EnablePackAtIndex(int index);
BOOL CL_FilterGrid_Store_EnableFirstPack();
void CL_SetCameraQuality();
void disableTimer();

void CL_DisplayTutorial();