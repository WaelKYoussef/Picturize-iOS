//
//  PKCameraController.m
//  PhotoKit
//
//  Created by Wael Youssef on 6/18/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//


#import "PKCamera.h"
#import "PKSettings.h"
#import "PKGalleryLayer.h"
#import "PKGalleryFileManager.h"
#include "PKFilterManager.h"
#import "iOSwTextureExt.h"


#pragma mark -
#pragma mark - CameraManager.
AVCaptureDevice*BackCameraDevice;
AVCaptureDevice*FrontCameraDevice;
AVCaptureVideoDataOutput*VideoOutput;
AVCaptureSession*RecordSession;
PKCameraController*CameraController;
PKCamera CurrentCamera;
PKCameraResolution CurrentCameraResolution;
wTexture*TextureTarget=NULL;
CVOpenGLESTextureCacheRef FrameTextureCash;
bool useCamera=false;

@implementation PKCameraController
-(BOOL)InitializeInputWithCamera:(PKCamera)camera{
    AVCaptureDevice*cameraDevice;
    if (camera==PKCamera_Back) {cameraDevice=BackCameraDevice;}
    else{cameraDevice=FrontCameraDevice;}
    
    AVCaptureDeviceInput*VideoInput=[AVCaptureDeviceInput deviceInputWithDevice:cameraDevice error:nil];
    if (VideoInput) {
        NSArray*Inputs=[RecordSession inputs];
        for (int i=0; i<Inputs.count; i++) {
            AVCaptureInput*Input=[Inputs objectAtIndex:i];
            [RecordSession removeInput:Input];
        }
        
        if ([RecordSession canAddInput:VideoInput]) {
            [RecordSession addInput:VideoInput];
            AVCaptureConnection*VideoConnection=[VideoOutput connectionWithMediaType:AVMediaTypeVideo];
            if ([VideoConnection isVideoOrientationSupported]){
                [VideoConnection setVideoOrientation:AVCaptureVideoOrientationPortrait];}
            if (camera==PKCamera_Front &&
                [VideoConnection isVideoMirroringSupported]) {
                [VideoConnection setVideoMirrored:YES];}
            
            CurrentCamera=camera;
            return YES;
        }
        else{
            NSLog(@"Err-2: Error Initalizing Video Camera!");
            return NO;
        }
    }
    else{
        NSLog(@"Err-1: Error Initalizing Video Camera!");return NO;
    }
    
}

-(BOOL)InitializeCameraResolution:(NSString*)Resolution{
    if ([RecordSession canSetSessionPreset:Resolution]) {
        [RecordSession setSessionPreset:Resolution];
        return YES;
    }
    else{NSLog(@"Err-1: Error Initializing Resolution !");return NO;}
}

// Most stable version yet.
-(void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection{
    
    if (TextureTarget&&useCamera) {
        CVImageBufferRef frameBuffer=CMSampleBufferGetImageBuffer(sampleBuffer);
        TextureTarget[0].width=(unsigned int)CVPixelBufferGetWidth(frameBuffer);
        TextureTarget[0].height=(unsigned int)CVPixelBufferGetHeight(frameBuffer);

        CVOpenGLESTextureRef frameTexture;
        CVReturn result=CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault, FrameTextureCash, frameBuffer, NULL, GL_TEXTURE_2D, GL_RGBA,TextureTarget[0].width, TextureTarget[0].height,GL_BGRA, GL_UNSIGNED_BYTE, 0, &frameTexture);
        
        if (result==kCVReturnSuccess) {
            TextureTarget[0].texture=CVOpenGLESTextureGetName(frameTexture);
            glBindTexture(GL_TEXTURE_2D, TextureTarget[0].texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
            wViewRequestFrames(1);
            CFRelease(frameTexture);
        }
    }
}
@end
/*
//Supports iOS 4.x & prior.
-(void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection{
    if (TextureTarget) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
        CVImageBufferRef frameBuffer=CMSampleBufferGetImageBuffer(sampleBuffer);
        TextureTarget[0].width=(unsigned int)CVPixelBufferGetWidth(frameBuffer);
        TextureTarget[0].height=(unsigned int)CVPixelBufferGetHeight(frameBuffer);
        CVPixelBufferLockBaseAddress(frameBuffer, 0);
        glDeleteTextures(1, &TextureTarget[0].texture);
        glGenTextures(1, &TextureTarget[0].texture);
        glBindTexture(GL_TEXTURE_2D, TextureTarget[0].texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CVPixelBufferGetBytesPerRow(frameBuffer)/4, TextureTarget[0].height, 0, GL_BGRA, GL_UNSIGNED_BYTE, CVPixelBufferGetBaseAddress(frameBuffer));
        
        CVPixelBufferUnlockBaseAddress(frameBuffer, 0);
        wViewRequestFrames(1);
    }
}
*/

/*
 //Uses a different thread & uses the frameQueue for cleanup.
struct _frameQueueItem{
    CVOpenGLESTextureRef texture;
    unsigned int counter;
};
struct _frameQueue{
    unsigned int count;
    unsigned int capacity;
    unsigned int itemsDeleted;
    struct _frameQueueItem queue[60];
};

struct _frameQueue frameQueue={0,60,0};

void addToFrameQueue(struct _frameQueueItem item){
    if (frameQueue.count<frameQueue.capacity) {
        frameQueue.queue[frameQueue.count]=item;
        frameQueue.count++;
    }
}
void cleanframeQueue(){
    for (int i=frameQueue.count-1; i>=0; i--) {
        frameQueue.queue[i].counter++;
        if (frameQueue.queue[i].counter>4) {
            
            CFRelease(frameQueue.queue[i].texture);
            memmove(&frameQueue.queue[i], &frameQueue.queue[i+1], sizeof(struct _frameQueueItem)*(frameQueue.count-i-1));
            frameQueue.count--;
            
            //  frameQueue.itemsDeleted++;
            //  if (frameQueue.itemsDeleted>5) {
            CVOpenGLESTextureCacheFlush(frameCash, 0);
            //      frameQueue.itemsDeleted=0;
            //  }
            
        }
    }
}

-(void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection{
    if (TextureTarget) {
        CVImageBufferRef frameBuffer=CMSampleBufferGetImageBuffer(sampleBuffer);
        unsigned int width=(unsigned int)CVPixelBufferGetWidth(frameBuffer);
        unsigned int height=(unsigned int)CVPixelBufferGetHeight(frameBuffer);
        
        CVOpenGLESTextureRef frameTexture;
        CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault, frameCash, frameBuffer, NULL, GL_TEXTURE_2D, GL_RGBA, width, height, GL_BGRA, GL_UNSIGNED_BYTE, 0, &frameTexture);
        
        TextureTarget[0].width=width;
        TextureTarget[0].height=height;
        TextureTarget[0].texture=CVOpenGLESTextureGetName(frameTexture);
        dispatch_async(dispatch_get_main_queue(), ^{
            glBindTexture(GL_TEXTURE_2D, TextureTarget[0].texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        });
        
        wViewRequestFrames(1);
        struct _frameQueueItem item={frameTexture, 0};
        addToFrameQueue(item);
        cleanframeQueue();
    }
}
*/

void CM_Setup(wTexture*targetTexture){
    TextureTarget=targetTexture;
    CameraController=[[PKCameraController alloc] init];
    
    CVReturn result=CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, [iOSwOpenGLView getOGLCOntext], NULL, &FrameTextureCash);
    if (result!=kCVReturnSuccess) {
        printf("Err: Failed to create camera texture cash.");}
    
    RecordSession=[[AVCaptureSession alloc] init];
    
    BackCameraDevice=NULL;FrontCameraDevice=NULL;
    NSArray*Cams=[AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    for (int i=0; i<Cams.count; i++) {
        AVCaptureDevice*Device=[Cams objectAtIndex:i];
        if ([Device position]==AVCaptureDevicePositionBack) {
            BackCameraDevice=Device;}
        else if ([Device position]==AVCaptureDevicePositionFront){
            FrontCameraDevice=Device;}}

    VideoOutput=[[AVCaptureVideoDataOutput alloc] init];
    [VideoOutput setSampleBufferDelegate:CameraController queue:dispatch_get_main_queue()];
    NSDictionary*OutputSettings =@{(NSString*)kCVPixelBufferPixelFormatTypeKey:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA]};
    [VideoOutput setVideoSettings:OutputSettings];
    [VideoOutput setAlwaysDiscardsLateVideoFrames:YES];
    [RecordSession addOutput:VideoOutput];
    
    AVCaptureConnection*VideoConnection=[VideoOutput connectionWithMediaType:AVMediaTypeVideo];
    if ([VideoConnection isVideoOrientationSupported]){
        [VideoConnection setVideoOrientation:AVCaptureVideoOrientationPortrait];}
    
    CurrentCameraResolution=PKCameraResolution_High;
    [RecordSession startRunning];
}

void CM_StartCamera(){
    useCamera=true;
}

void CM_StopCamera(){
    useCamera=false;
}

bool CM_CameraIsRunning(){
    return useCamera;
}

void CM_SetCamera(PKCamera camera){
    CM_SetCameraWithResolution(camera, CurrentCameraResolution);
}

void CM_SetCameraResolution(PKCameraResolution cameraResolution){
    CM_SetCameraWithResolution(CurrentCamera, cameraResolution);
}

void CM_SetCameraWithResolution(PKCamera camera, PKCameraResolution cameraResolution){
    
    NSString*CameraResolutionString;
    switch (cameraResolution) {
        case PKCameraResolution_High:
            CameraResolutionString=AVCaptureSessionPresetPhoto;break;
        case PKCameraResolution_Medium:
            CameraResolutionString=AVCaptureSessionPresetHigh;break;
        case PKCameraResolution_Low:
            CameraResolutionString=AVCaptureSessionPresetMedium;break;
        default:CameraResolutionString=AVCaptureSessionPresetPhoto;break;}
    
    BOOL CameraWasRunning=CM_CameraIsRunning();
    if (CameraWasRunning) {[RecordSession stopRunning];}
    [RecordSession beginConfiguration];
    [CameraController InitializeInputWithCamera:camera];
    [CameraController InitializeCameraResolution:CameraResolutionString];
    [RecordSession commitConfiguration];
    if (CameraWasRunning) {[RecordSession startRunning];}
    
}

PKCamera CM_GetCurrentCamera(){
    return CurrentCamera;
}

bool CM_CameraIsAvailable(PKCamera camera){
    if (camera==PKCamera_Front) {
        return [UIImagePickerController isCameraDeviceAvailable:UIImagePickerControllerCameraDeviceFront];}
    else{return [UIImagePickerController isCameraDeviceAvailable:UIImagePickerControllerCameraDeviceRear];}
}

bool CM_IsFlashAvailableForCamera(PKCamera camera){
    if (camera==PKCamera_Front) {
        return [UIImagePickerController isFlashAvailableForCameraDevice:UIImagePickerControllerCameraDeviceFront];}
    else{return [UIImagePickerController isFlashAvailableForCameraDevice:UIImagePickerControllerCameraDeviceRear];}
}

bool CM_HasOnlyBackCamera(){
    return
    !CM_CameraIsAvailable(PKCamera_Front) &&
    CM_CameraIsAvailable(PKCamera_Back);
}
bool CM_HasBothCameras(){
    return
    CM_CameraIsAvailable(PKCamera_Front) &&
    CM_CameraIsAvailable(PKCamera_Back);
};
bool CM_HasNoCamera(){
    return
    !CM_CameraIsAvailable(PKCamera_Front) &&
    !CM_CameraIsAvailable(PKCamera_Back);
}


#pragma mark -
#pragma mark - CameraLayer.
wView*CL_Bar;
wView*CL_ShutterButton;
wView*CL_Preview;
wView*CL_PreviewCover;
wView*CL_PreviewCamera;
wView*CL_PreviewGrid;
wView*CL_FilterGrid;

wArray*cameraButtons=NULL;
wRect CL_Preview_FullFrame;
wRect CL_Preview_SquareFrame;
wTexture*CL_Bar_TimerTextures[10];
wTexture*CL_Store_IconTexture;


SystemSoundID ShutterSound;
SystemSoundID TimerSound;

NSString*buttonNameSuffix=nil;



bool barIsOpen=false;
void CL_DisplayTutorial_End(){
  //  NSLog(@"here");
    if (barIsOpen) {
        wRect frame=wViewGetFrame(CL_Bar);
        frame.origin.x=wScreenGetPortraitFrame().size.width;
        wAnimationBeginContext();
        wAnimationSetDuration(0.3);
        wAnimationSetEndFunction(wViewHide, CL_Bar);
        wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
        wViewAnimateFrame(CL_Bar, frame);
        wAnimationApplyContext();
        barIsOpen=false;
    }
}
wTimer*tutorialTimer;
void CL_DisplayTutorial(){
    if (!barIsOpen) {
        wViewShow(CL_Bar);
        wRect frame=wViewGetFrame(CL_Bar);
        frame.origin.x=wScreenGetPortraitFrame().size.width-frame.size.width;
        wAnimationBeginContext();
        wAnimationSetDuration(0.3);
        wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
        wViewAnimateFrame(CL_Bar, frame);
        wAnimationApplyContext();
        barIsOpen=true;
        
        tutorialTimer=wTimerNew(1.5, CL_DisplayTutorial_End, false, nil);
        wTimerStart(tutorialTimer);
    }
}

void CL_SetCameraQuality(){
    switch (SM_GetSelectionForSetting(PKSetting_ImageQuality)) {
        case 1:CM_SetCameraResolution(PKCameraResolution_Medium);break;
        case 2:CM_SetCameraResolution(PKCameraResolution_Low);break;
        default:CM_SetCameraResolution(PKCameraResolution_High);break;
    }
}

const wMatrix4 CL_Bar_ButtonMatrix_PortraitUp={
    1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,
    0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f};

const wMatrix4 CL_Bar_ButtonMatrix_PortraitDown={
    -1.0f,0.0f,0.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,
    0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f};

const wMatrix4 CL_Bar_ButtonMatrix_LandscapeLeft={
    0.0f,1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f,0.0f,
    0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f};

const wMatrix4 CL_Bar_ButtonMatrix_LandscapeRight={
    0.0f,-1.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,
    0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f};
wDeviceOrientation CL_Bar_Buttons_CurrentOrientation;

void CL_Bar_RotateButtons_End(){
    wMatrix4 mat;
    
    switch (wDeviceGetOrientation()) {
        case wDeviceOrientationPortraitUp:mat=CL_Bar_ButtonMatrix_PortraitUp;break;
        case wDeviceOrientationPortraitDown:mat=CL_Bar_ButtonMatrix_PortraitDown;break;
        case wDeviceOrientationLandscapeLeft:mat=CL_Bar_ButtonMatrix_LandscapeLeft;break;
        case wDeviceOrientationLandscapeRight:mat=CL_Bar_ButtonMatrix_LandscapeRight;break;
        default:break;
    }

    for (int i=0; i<wArrayGetCount(cameraButtons); i++) {
        wView*button=wArrayGetItemAtIndex(cameraButtons, i);
        wViewSetMatrix(button, mat);
    }
}

void CL_Bar_RotateButtons(wDeviceOrientation orientation){
    float angle=90.0;
    switch (CL_Bar_Buttons_CurrentOrientation) {
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
    
    CL_Bar_Buttons_CurrentOrientation=orientation;
    
    wAnimationBeginContext();
    wAnimationSetEndFunction(CL_Bar_RotateButtons_End, NULL);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    wAnimationSetDuration(0.5);
    for (int i=0; i<wArrayGetCount(cameraButtons); i++) {
        wView*button=wArrayGetItemAtIndex(cameraButtons, i);
        wViewAnimateRotationOnZAxis(button, angle);
    }
    wAnimationApplyContext();

}

wArray*CL_FilterGrid_Store_Packs;
wFont CL_FilterGrid_Store_Font;

BOOL CL_FilterGrid_Store_EnableFirstPack(){
    return true;
}

void CL_FilterGrid_Store_EnablePackAtIndex_End(wView*pack){
    if (pack) {
        wView*Icon=wArrayGetItemAtIndex(wViewGetSubviewArray(pack), 0);
        wView*Title=wArrayGetItemAtIndex(wViewGetSubviewArray(pack), 1);
        wTexture*title=wViewGetBackgroundMask(Title);
        if (title) {wTextureRelease(title);}
        wViewRelease(Icon);
        wViewRelease(Title);
        wViewRelease(pack);
    }
}

void CL_SetColorTheme(PKColorTheme colorTheme){
    for (int i=0; i<wArrayGetCount(cameraButtons); i++) {
        wView*Button=wArrayGetItemAtIndex(cameraButtons, i);
        wViewSetBackgroundColor(Button, PKForeColor);
    }
    
    wViewSetBackgroundColor(CL_Bar, PKBarColor);
    
    wViewSetBackgroundColor(CL_ShutterButton, PKBarColor);
    wViewSetBackgroundColor(wArrayGetItemAtIndex(wViewGetSubviewArray(CL_ShutterButton), 0), PKForeColor);
}

void CL_FilterButtonAction(){
    wPoint offset=wScrollViewGetContentOffset(CL_FilterGrid);
    wRect frame=CL_Preview_FullFrame;
    frame.origin.x+=offset.x,frame.origin.y+=offset.y;
    
    wViewSetFrame(CL_Preview, frame);
    wViewAddSubview(CL_FilterGrid, CL_Preview);
    wViewSetIsHidden(CL_FilterGrid, false);
    
    
    float size=wTableViewGetItemSize(CL_FilterGrid).width;
    wPoint origin=wTableViewGetLocationOfItemAtIndex(CL_FilterGrid, wViewGetTag(CL_PreviewCamera));
    frame=wRectNew(origin.x, origin.y, size, size);
    
    wAnimationBeginContext();
    wAnimationSetEndFunction(wViewHide, CL_Preview);
    wAnimationSetDuration(0.3);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    wViewAnimateFrame(CL_Preview, frame);
    wViewAnimateFrame(CL_PreviewCamera, wRectNew(0, 0, size, size));
    wViewAnimateFrame(CL_PreviewGrid, wRectNew(0, 0, size, size));
    wViewAnimateAlpha(CL_Bar, 0.0);
    wViewAnimateAlpha(CL_ShutterButton, 0.0);
    wAnimationApplyContext();
    disableTimer();
}

void CL_GalleryButtonAction(){
    GL_Display();
    disableTimer();
}

wTimer*switchTimer=NULL;
void CL_SwitchButtonAction_End(){
    wAnimationBeginContext();
    wAnimationSetDuration(0.3);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    wViewAnimateAlpha(CL_PreviewCover, 0.0);
    wAnimationApplyContext();
}
void CL_SwitchButtonAction_Switch(){
    if (CM_GetCurrentCamera()==PKCamera_Front) {
        CM_SetCamera(PKCamera_Back);
        SM_SetSelectionForSetting(PKSetting_Camera, 0);}
    else {CM_SetCamera(PKCamera_Front);
        SM_SetSelectionForSetting(PKSetting_Camera, 1);}
    
    SM_SaveCameraSettings();
    
    wTimerStart(switchTimer);
}
void CL_SwitchButtonAction(){
    wViewSetBackgroundColor(CL_PreviewCover, wColorBlack());
    wAnimationBeginContext();
    wAnimationSetEndFunction(CL_SwitchButtonAction_Switch, NULL);
    wAnimationSetDuration(0.3);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    wViewAnimateAlpha(CL_PreviewCover, 1.0);
    wAnimationApplyContext();
    disableTimer();
}


void CL_SquareButtonAction(){
    if (SM_GetSelectionForSetting(PKSetting_CameraIsSquare)==1) {
        wAnimationBeginContext();
        wAnimationSetDuration(0.3);
        wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
        wViewAnimateFrame(CL_PreviewCamera, CL_Preview_FullFrame);
        wAnimationApplyContext();
        SM_SetSelectionForSetting(PKSetting_CameraIsSquare, 0);
    }
    else{
        wAnimationBeginContext();
        wAnimationSetDuration(0.3);
        wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
        wViewAnimateFrame(CL_PreviewCamera, CL_Preview_SquareFrame);
        wAnimationApplyContext();
        SM_SetSelectionForSetting(PKSetting_CameraIsSquare, 1);
    }
    
    SM_SaveCameraSettings();
}

void CL_GridButtonAction(){
    if (wViewGetAlpha(CL_PreviewGrid)==0.0) {
        wAnimationBeginContext();
        wAnimationSetDuration(0.3);
        wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
        wViewAnimateAlpha(CL_PreviewGrid, 1.0);
        wAnimationApplyContext();
        SM_SetSelectionForSetting(PKSetting_CameraGrid, 1);
    }
    else{
        wAnimationBeginContext();
        wAnimationSetDuration(0.3);
        wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
        wViewAnimateAlpha(CL_PreviewGrid, 0.0);
        wAnimationApplyContext();
        SM_SetSelectionForSetting(PKSetting_CameraGrid, 0);
    }
    
    SM_SaveCameraSettings();
}

void CL_SettingsButtonAction(){
    SL_Display();
    disableTimer();
}

void CL_SetTimerButtonSeconds(unsigned char Seconds){
    if (Seconds<10) {
        wView*TimerButton=wArrayGetItemAtIndex(cameraButtons, 1);
        wTexture*timerTexture=CL_Bar_TimerTextures[Seconds];
        wViewSetBackgroundMask(TimerButton, timerTexture);
    }
}

void CL_TimerButtonAction(){
    wView*TimerButton=wArrayGetItemAtIndex(cameraButtons, 1);
    
    int selection=SM_GetSelectionForSetting(PKSetting_CameraTimer);
    selection++;
    if (selection>3) {selection=0;}
    SM_SetSelectionForSetting(PKSetting_CameraTimer, selection);
    
    wTexture*timerTexture=NULL;
    switch (selection) {
        case 1:timerTexture=CL_Bar_TimerTextures[3];break;
        case 2:timerTexture=CL_Bar_TimerTextures[6];break;
        case 3:timerTexture=CL_Bar_TimerTextures[9];break;
        default:timerTexture=CL_Bar_TimerTextures[0];break;
    }
    wViewSetBackgroundMask(TimerButton, timerTexture);
    
    SM_SaveCameraSettings();
    disableTimer();
}

void CL_FlashButtonAction(){
    NSArray*Titles=[[NSArray alloc] initWithObjects:
                    @"CL_FlashOff",@"CL_FlashOn",@"CL_FlashAuto", nil];
    
    int selection=SM_GetSelectionForSetting(PKSetting_CameraFlash);
    
    selection++;
    if (selection>2) {selection=0;}
    
    
    
    wView*FlashButton=wArrayGetItemAtIndex(cameraButtons, 1);
    wTextureRelease(wViewGetBackgroundMask(FlashButton));
    wViewSetBackgroundImage(FlashButton, NULL);
    
    wTexture*flashTexture=[iOSwTextureExt wTextureFromImageWithName:[Titles objectAtIndex:selection]];
    
    SM_SetSelectionForSetting(PKSetting_CameraFlash,selection);
    
    wViewSetBackgroundMask(FlashButton, flashTexture);
    
    SM_SaveCameraSettings();
    disableTimer();
}

wView*CL_FilterGrid_ViewForFilter(){
    wView*BoxSlot=wViewNew();
    wViewSetShouldDraw(BoxSlot, false);
    wViewSetOpenGLData(BoxSlot, BoxSlot);
    wViewSetRenderOpenGLFunction(BoxSlot, PKFilterManagerRenderFunction);
    return BoxSlot;
}

BOOL CL_FilterGrid_SelectItemAction(int index){
    int packIndex=index/PKFilterManagerNumberOfFiltersPerPack();
    
    float size=wTableViewGetItemSize(CL_FilterGrid).width;
    wPoint offset=wScrollViewGetContentOffset(CL_FilterGrid);
    wPoint origin=wTableViewGetLocationOfItemAtIndex(CL_FilterGrid, index);
    origin.x-=offset.x,origin.y-=offset.y;
    
    wRect frame=wRectNew(origin.x, origin.y, size, size);
    wViewSetTag(CL_PreviewCamera, index);
    wViewSetFrame(CL_Preview, frame);
    wViewAddSubviewAtIndex(CameraLayer, CL_Preview, 1);
    wViewSetIsHidden(CL_Preview, false);
    
    wAnimationBeginContext();
    wAnimationSetDuration(0.3);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    wAnimationSetEndFunction(wViewHide, CL_FilterGrid);
    wViewAnimateFrame(CL_Preview, CL_Preview_FullFrame);
    wViewAnimateFrame(CL_PreviewGrid, CL_Preview_FullFrame);
    if (SM_GetSelectionForSetting(PKSetting_CameraIsSquare)==1) {
        wViewAnimateFrame(CL_PreviewCamera, CL_Preview_SquareFrame);}
    else{wViewAnimateFrame(CL_PreviewCamera, CL_Preview_FullFrame);}
    wViewAnimateAlpha(CL_Bar, 1.0);
    wViewAnimateAlpha(CL_ShutterButton, 1.0);
    wAnimationApplyContext();
    
    SM_SetSelectionForSetting(PKSetting_CameraFilter, index);
    SM_SaveCameraSettings();
    return true;
}

bool swipeBreak=false;
wPoint start;
void CL_Preview_TouchBeganAction(wGesture*gesture){
    start=wGestureGetLocationAtIndex(gesture, 0);
}
void CL_Preview_SwipeAction(wGesture*gesture){
    wPoint current=wGestureGetLocationAtIndex(gesture, 0);
    if (!swipeBreak) {
        float Distance=100.0;
        switch (deviceType) {
            case deviceType_iPhone:Distance=50.0;break;
            case deviceType_iPhoneRetina:Distance=100.0;break;
            case deviceType_iPad:Distance=100.0;break;
            case deviceType_iPadRetina:Distance=200.0;break;
            default:Distance=100.0;break;
        }
        
        if (start.x-current.x>Distance) {
            if (!barIsOpen) {
                wViewShow(CL_Bar);
                wRect frame=wViewGetFrame(CL_Bar);
                frame.origin.x=wScreenGetPortraitFrame().size.width-frame.size.width;
                wAnimationBeginContext();
                wAnimationSetDuration(0.3);
                wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
                wViewAnimateFrame(CL_Bar, frame);
                wAnimationApplyContext();
                barIsOpen=true;
            }
            swipeBreak=true;
        }else if (current.x-start.x>Distance){
            if (barIsOpen) {
                wRect frame=wViewGetFrame(CL_Bar);
                frame.origin.x=wScreenGetPortraitFrame().size.width;
                wAnimationBeginContext();
                wAnimationSetDuration(0.3);
                wAnimationSetEndFunction(wViewHide, CL_Bar);
                wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
                wViewAnimateFrame(CL_Bar, frame);
                wAnimationApplyContext();
                barIsOpen=false;
            }
            swipeBreak=true;
        }
    }
}
void CL_Preview_TouchEndedAction(wGesture*gesture){
    swipeBreak=false;
}

void CL_TakePicture(){
    AudioServicesPlaySystemSound(ShutterSound);
    
    GLuint width=TextureTarget[0].width,height=TextureTarget[0].height;
    if (SM_GetSelectionForSetting(PKSetting_CameraIsSquare)==PKSelection_Yes) {
        height=width;
    }
    
    BOOL mirrorX=true;
    if (CM_GetCurrentCamera()==PKCamera_Front) {
        if (SM_GetSelectionForSetting(PKSetting_MirrorSelfies)==PKSelection_Yes) {
            mirrorX=false;
        }
    }
    
    switch (wDeviceGetOrientation()) {
        case wDeviceOrientationPortraitUp:
            PKFilterManagerSetRenderTransformations(mirrorX, false, wDeviceOrientationPortraitDown);

            break;
        case wDeviceOrientationPortraitDown:
            PKFilterManagerSetRenderTransformations(mirrorX, false, wDeviceOrientationPortraitUp);

            break;
        case wDeviceOrientationLandscapeLeft:{
            PKFilterManagerSetRenderTransformations(mirrorX, false, wDeviceOrientationLandscapeRight);
            width=height;
            height=TextureTarget[0].width;
            break;}
        case wDeviceOrientationLandscapeRight:{
            PKFilterManagerSetRenderTransformations(mirrorX, false, wDeviceOrientationLandscapeLeft);
            width=height;
            height=TextureTarget[0].width;
            break;}
        default:
            break;
    }

    
    unsigned int filterTag=wViewGetTag(CL_PreviewCamera);
    wRect buff=wViewGetFrame(CL_PreviewCamera);
    buff.origin=wPointZero;
    buff.size=wSizeNew(width, height);
    glViewport(0, 0, width, height);
    PKFilterManagerSetDeviceDimentions(buff.size);
    
    
    GLubyte*finalData=malloc(width*height*4);
    
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,  width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    PKFilterManagerRenderToFrame(buff, filterTag);
    
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, finalData);
    glDeleteTextures(1, &texture);
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(finalData, width, height,
                                                 8, width*4, colorSpace,
                                                 kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(colorSpace);
    CGImageRef filteredImage=CGBitmapContextCreateImage(context);
    
    
    UIImage*OriginalImage=nil;
    UIImage*FilteredImage=[UIImage imageWithCGImage:filteredImage];
    if (SM_GetSelectionForSetting(PKSetting_SaveOriginalPhoto)==PKSelection_Yes) {
        if (filterTag!=0) {
            glClear(GL_COLOR_BUFFER_BIT);
            PKFilterManagerRenderToFrame(buff, 0);
            glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, finalData);
            CGImageRef original=CGBitmapContextCreateImage(context);
            OriginalImage=[UIImage imageWithCGImage:original];
        }
    }
    CGContextRelease(context);
    free(finalData);

   
    
    if (GM_AccessIsAllowedToUserPhotos()) {
        [[PHPhotoLibrary sharedPhotoLibrary] performChanges:^{
            if (FilteredImage) {
                [PHAssetChangeRequest creationRequestForAssetFromImage:FilteredImage];}
            if (OriginalImage) {
                [PHAssetChangeRequest creationRequestForAssetFromImage:OriginalImage];}
        } completionHandler:^(BOOL success, NSError *error) {
            if (success) {GM_ReloadAlbumAtIndex(0);}
            else{printf("Error Saving Pictures!\n");}
        }];
        
//        NSError*err;
//        [[PHPhotoLibrary sharedPhotoLibrary] performChangesAndWait:^{
//            if (FilteredImage) {
//                [PHAssetChangeRequest creationRequestForAssetFromImage:FilteredImage];}
//            if (OriginalImage) {
//                [PHAssetChangeRequest creationRequestForAssetFromImage:OriginalImage];}
//        } error:&err];
//        
//        if (!err) {GM_ReloadAlbumAtIndex(0);}
//        else{printf("Error Saving Pictures!\n");}
    }
    else{
        NSString*ext=@".jpg";
        NSFileManager*fileMan=[NSFileManager defaultManager];
        
        if (FilteredImage) {
            wDate date=wDateGet();
            NSString*Name=[NSString stringWithFormat:@"Photo%d%d%d%d%d%d", date.Year,date.Month,date.Day,date.Hour,date.Minute,date.Second];
            NSString*path=GM_PathForImageNameForITunes(Name);
            path=[path stringByAppendingString:ext];
            while ([fileMan fileExistsAtPath:path]) {
                Name=[Name stringByAppendingString:@"1"];
                path=GM_PathForImageNameForITunes(Name);
                path=[path stringByAppendingString:ext];
            }
            
            NSData*ImageData=UIImageJPEGRepresentation(FilteredImage, 1.0);
            [ImageData writeToFile:path atomically:true];
        }
        if (OriginalImage) {
            wDate date=wDateGet();
            NSString*Name=[NSString stringWithFormat:@"Photo1%d%d%d%d%d%d", date.Year,date.Month,date.Day,date.Hour,date.Minute,date.Second];
            NSString*path=GM_PathForImageNameForITunes(Name);
            path=[path stringByAppendingString:ext];
            while ([fileMan fileExistsAtPath:path]) {
                Name=[Name stringByAppendingString:@"1"];
                path=GM_PathForImageNameForITunes(Name);
                path=[path stringByAppendingString:ext];
            }
            
            NSData*ImageData=UIImageJPEGRepresentation(OriginalImage, 1.0);
            [ImageData writeToFile:path atomically:true];
        }
    }
    
    [iOSwOpenGLView rebindRenderBuffer];
    PKFilterManagerSetRenderTransformations(false, false, wDeviceOrientationPortraitUp);
    PKFilterManagerSetDeviceDimentions(wScreenGetPortraitFrame().size);
    
    
    wViewSetBackgroundColor(CL_PreviewCover, wColorWhite());
    wViewSetAlpha(CL_PreviewCover, 1.0);
    wAnimationBeginContext();
    wAnimationSetDuration(0.5);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    wViewAnimateAlpha(CL_PreviewCover, 0.0);
    wAnimationApplyContext();
}

wTimer*timer=NULL;
int count=0;
void disableTimer(){
    if (timer) {
        wTimerStop(timer);
        wTimerRelease(timer);
        timer=NULL;
        
        wView*TimerButton=wArrayGetItemAtIndex(cameraButtons, 1);
        wTexture*timerTexture=NULL;
        switch (SM_GetSelectionForSetting(PKSetting_CameraTimer)) {
            case 1:timerTexture=CL_Bar_TimerTextures[3];break;
            case 2:timerTexture=CL_Bar_TimerTextures[6];break;
            case 3:timerTexture=CL_Bar_TimerTextures[9];break;
            default:timerTexture=CL_Bar_TimerTextures[0];break;
        }
        wViewSetBackgroundMask(TimerButton, timerTexture);
    }
}

void decrement(){
    count--;
    if (count==0) {
        disableTimer();
        CL_TakePicture();
    }
    else{
        AudioServicesPlaySystemSound(TimerSound);
        CL_SetTimerButtonSeconds(count);
    }

}
void CL_ShutterButton_Action(){
    
    int selection=SM_GetSelectionForSetting(PKSetting_CameraTimer);
    if (selection!=0) {
        if (!timer) {
            switch (selection) {case 1:count=3;break;
                case 2:count=6;break;case 3:count=9;break;
                default:count=3;break;}
            AudioServicesPlaySystemSound(TimerSound);
            timer=wTimerNew(1.0, decrement, true, nil);
            wTimerStart(timer);
        }
        else{
            disableTimer();
        }
    }
    else{
        CL_TakePicture();
    }
    
}

void CL_Setup(){
    CL_FilterGrid_Store_Font=[iOSwTextureExt wFontNewFromUIFont:[UIFont fontWithName:@"HelveticaNeue-Light" size:CLM.StoreFontSize]];

    wRect frame=wScreenGetPortraitFrame();
    
    CameraLayer=wViewNewWithFrame(frame);
    wViewSetShouldDraw(CameraLayer, false);
    wViewAddSubviewAtIndex(wRootView, CameraLayer, 0);
    
    CL_FilterGrid=wTableViewNewWithFrame(frame);
    wViewAddSubview(CameraLayer,CL_FilterGrid);
    wViewSetBackgroundColor(CL_FilterGrid, wColorWhite());
    wTableViewSetOffsetFromBottom(CL_FilterGrid, CLM.OutlineSize+CLM.restoreButtonSize);

    wTableViewSetItemsOutlineSize(CL_FilterGrid, CLM.OutlineSize);
    wTableViewSetItemsPerRow(CL_FilterGrid, CLM.FiltersPerRow);
    wTableViewSetViewForItemAtIndexFunction(CL_FilterGrid, CL_FilterGrid_ViewForFilter);
    wTableViewSetSelectedItemAtIndexFunction(CL_FilterGrid, CL_FilterGrid_SelectItemAction);
    wTableViewSetItemCount(CL_FilterGrid, PKFilterManagerNumberOfFilters());
    wTableViewSetSquaredItems(CL_FilterGrid, true);
    wTableViewLoad(CL_FilterGrid);
    wViewHide(CL_FilterGrid);
    

    
    wSize contentSize=wScrollViewGetContentSize(CL_FilterGrid);
    wView*restoreButton=wButtonNewWithFrame(wRectNew(CLM.OutlineSize, contentSize.height-(CLM.OutlineSize+CLM.restoreButtonSize), wScreenGetPortraitFrame().size.width-(CLM.OutlineSize*2.0), CLM.restoreButtonSize));
    wViewAddSubview(CL_FilterGrid, restoreButton);
    wViewSetBackgroundColor(restoreButton, wColorBlack());

    wTexture*restoreButtonTitleText=[iOSwTextureExt wTextureFromNSString:@"Restore Packs" Font:CL_FilterGrid_Store_Font MaxWidth:wScreenGetPortraitFrame().size.width];
    wView*restoreButtonTitle=wViewNewWithFrame(wRectNew((wViewGetFrame(restoreButton).size.width-restoreButtonTitleText[0].width)/2.0, (CLM.restoreButtonSize-restoreButtonTitleText[0].height)/2.0, restoreButtonTitleText[0].width, restoreButtonTitleText[0].height));
    wViewSetBackgroundMask(restoreButtonTitle, restoreButtonTitleText);
    wViewSetIsOpaque(restoreButtonTitle, false);
    wViewSetBackgroundColor(restoreButtonTitle, wColorWhite());
    wViewAddSubview(restoreButton, restoreButtonTitle);
    
    CL_Preview=wViewNewWithFrame(wScreenGetPortraitFrame());
    wViewSetBackgroundColor(CL_Preview, wColorBlack());
    wViewSetUserInteractionEnabled(CL_Preview, true);
    wGesture*touchDown=wGestureNew(wGestureTouchDown);
    wGestureSetFunction(touchDown, CL_Preview_TouchBeganAction);
    wViewAddGesture(CL_Preview, touchDown);
    wGesture*swipe=wGestureNew(wGestureSwipe);
    wGestureSetFunction(swipe, CL_Preview_SwipeAction);
    wViewAddGesture(CL_Preview, swipe);
    wGesture*touchUp=wGestureNew(wGestureTouchUp);
    wGestureSetFunction(touchUp, CL_Preview_TouchEndedAction);
    wViewAddGesture(CL_Preview, touchUp);
    wViewAddSubview(CameraLayer, CL_Preview);
    
    CL_PreviewCamera=wViewNewWithFrame(wScreenGetPortraitFrame());
    wViewSetOpenGLData(CL_PreviewCamera, CL_PreviewCamera);
    wViewSetRenderOpenGLFunction(CL_PreviewCamera,PKFilterManagerRenderFunction);
    wViewSetShouldDraw(CL_PreviewCamera, false);
    wViewAddSubview(CL_Preview, CL_PreviewCamera);
    
    CL_PreviewGrid=wViewNewWithFrame(wScreenGetPortraitFrame());
    wViewAddSubview(CL_Preview, CL_PreviewGrid);
    wViewSetIsOpaque(CL_PreviewGrid, false);
    wViewSetClipsSubviews(CL_PreviewGrid, true);
    
    wRect barframe=wScreenGetPortraitFrame();
    
    float third=barframe.size.width/3.0;
    float start=(barframe.size.height-third)/2.0;
    for (int i=0; i<2; i++) {
        wView*bar=wViewNewWithFrame(wRectNew(0, start+(third*i), barframe.size.width, CLM.GridBarSize));
        wViewSetBackgroundColor(bar, wColorNewGreyScaleFloat(0.0, 0.5));
        wViewAddSubview(CL_PreviewGrid, bar);
        wViewSetIsOpaque(bar, false);
    }
    
    
    for (int i=1; i<=2; i++) {
        wView*bar=wViewNewWithFrame(wRectNew(third*i, 0, CLM.GridBarSize, barframe.size.height));
        wViewSetBackgroundColor(bar, wColorNewGreyScaleFloat(0.0, 0.5));
        wViewAddSubview(CL_PreviewGrid, bar);
        wViewSetIsOpaque(bar, false);
    }
    
    CL_Preview_FullFrame=wScreenGetPortraitFrame();
    CL_Preview_SquareFrame=wRectNew(0, (CL_Preview_FullFrame.size.height-CL_Preview_FullFrame.size.width)/2.0, CL_Preview_FullFrame.size.width, CL_Preview_FullFrame.size.width);
    
    CL_PreviewCover=wViewNewWithFrame(wScreenGetPortraitFrame());
    wViewSetBackgroundColor(CL_PreviewCover, wColorBlack());
    wViewAddSubview(CL_Preview, CL_PreviewCover);
    wViewSetIsOpaque(CL_PreviewCover, false);
    wViewSetAlpha(CL_PreviewCover, 0.0);
    
    
    switchTimer=wTimerNew(0.1, CL_SwitchButtonAction_End, false, NULL);
    
    CL_Bar=wViewNewWithFrame(wRectNew(wScreenGetPortraitFrame().size.width, 0, CLM.BarSize, wScreenGetHeight()));
    wViewAddSubview(CameraLayer, CL_Bar);
    wViewSetIsOpaque(CL_Bar, false);
    wViewSetBackgroundColor(CL_Bar, wColorNewRGBAFloat(1, 1, 1, 0.5));

    float containerHeight=(CLM.BarButtonSize*7)+(CLM.OutlineSize*6);
    wView*Cl_Bar_ButtonContainer=wViewNewWithFrame(wRectNew(CLM.OutlineSize, (wScreenGetHeight()-containerHeight)/2.0, CLM.BarButtonSize, containerHeight));
    wAutoLayoutOptions Cl_Bar_ButtonContaineropt=wAutoLayoutNone;
    Cl_Bar_ButtonContaineropt.anchor.y=0.5;
    wViewSetAutoLayoutOptions(Cl_Bar_ButtonContainer, Cl_Bar_ButtonContaineropt);
    wViewAddSubview(CL_Bar, Cl_Bar_ButtonContainer);
    wViewSetIsOpaque(Cl_Bar_ButtonContainer, false);
    
    cameraButtons=wArrayNewWithCapacity(8);
    wFunction functions[8]={
        CL_SwitchButtonAction,
        CL_TimerButtonAction,
        CL_FilterButtonAction,
        CL_GalleryButtonAction,
        CL_GridButtonAction,
        CL_SquareButtonAction,
        CL_SettingsButtonAction,
    };
    
    switch (deviceType) {
        case deviceType_iPhone:{buttonNameSuffix=@"60.png";break;}
        case deviceType_iPhoneRetina:{buttonNameSuffix=@"120.png";break;}
        case deviceType_iPad:{buttonNameSuffix=@"100.png";break;}
        case deviceType_iPadRetina:{buttonNameSuffix=@"200.png";break;}
        default:{buttonNameSuffix=@"100.png";break;}
    }
    
    NSArray*Titles=[[NSArray alloc] initWithObjects:
                    @"CL_Timer",@"CL_Timer_1",
                    @"CL_Timer_2",@"CL_Timer_3",
                    @"CL_Timer_4",@"CL_Timer_5",
                    @"CL_Timer_6",@"CL_Timer_7",
                    @"CL_Timer_8",@"CL_Timer_9",
                    nil];
    
    
    for (int i=0; i<10; i++) {
        CL_Bar_TimerTextures[i]=[iOSwTextureExt wTextureFromImageWithName:[[Titles objectAtIndex:i] stringByAppendingString:buttonNameSuffix]];
    }
    
    
    wTexture*timerTexture=NULL;
    switch (SM_GetSelectionForSetting(PKSetting_CameraTimer)) {
        case 1:timerTexture=CL_Bar_TimerTextures[3];break;
        case 2:timerTexture=CL_Bar_TimerTextures[6];break;
        case 3:timerTexture=CL_Bar_TimerTextures[9];break;
        default:timerTexture=CL_Bar_TimerTextures[0];break;
    }
    
    NSArray*Names=[NSArray arrayWithObjects:
                   [@"CL_Switch" stringByAppendingString:buttonNameSuffix],
                   [@"CL_Shift" stringByAppendingString:buttonNameSuffix],
                   [@"CL_Gallery" stringByAppendingString:buttonNameSuffix],
                   [@"CL_Grid" stringByAppendingString:buttonNameSuffix],
                   [@"CL_Square" stringByAppendingString:buttonNameSuffix],
                   [@"CL_Settings" stringByAppendingString:buttonNameSuffix],
                   nil];
    
    
    wTexture*textures[8]={
        [iOSwTextureExt wTextureFromImageWithName:[Names objectAtIndex:0]],
        timerTexture,
        [iOSwTextureExt wTextureFromImageWithName:[Names objectAtIndex:1]],
        [iOSwTextureExt wTextureFromImageWithName:[Names objectAtIndex:2]],
        [iOSwTextureExt wTextureFromImageWithName:[Names objectAtIndex:3]],
        [iOSwTextureExt wTextureFromImageWithName:[Names objectAtIndex:4]],
        [iOSwTextureExt wTextureFromImageWithName:[Names objectAtIndex:5]],
    };
    
    wRect buttonFrame=wRectNew(0.0, 0.0, CLM.BarButtonSize, CLM.BarButtonSize);
    for (int i=0; i<7; i++) {
        wView*button=wButtonNewWithFrame(buttonFrame);
        wViewSetAnchorPoint(button, wPointNew(0.5, 0.5));
        wViewSetBackgroundColor(button, wColorBlack());
        wViewSetBackgroundMask(button, textures[i]);
        wButtonSetFunction(button, functions[i]);
        wViewAddSubview(Cl_Bar_ButtonContainer, button);
        wArrayAddItem(cameraButtons, button);
        buttonFrame.origin.y+=CLM.BarButtonSize+CLM.OutlineSize;
    }
 
    NSString*suffix=nil;
    switch (deviceType) {
        case deviceType_iPhone:{suffix=@"75.png";break;}
        case deviceType_iPhoneRetina:{suffix=@"150.png";break;}
        case deviceType_iPad:{suffix=@"100.png";break;}
        case deviceType_iPadRetina:{suffix=@"200.png";break;}
        default:{suffix=@"100.png";break;}
    }
    
    
    CL_ShutterButton=wButtonNewWithFrame(wRectNew((wScreenGetWidth()-CLM.ShutterSize)/2.0, wScreenGetHeight()-CLM.ShutterSize-CLM.OutlineSize, CLM.ShutterSize, CLM.ShutterSize));
    wViewSetIsOpaque(CL_ShutterButton, false);
    wViewAddSubview(CameraLayer, CL_ShutterButton);
    wViewSetBackgroundMask(CL_ShutterButton, [iOSwTextureExt wTextureFromImageWithName:[@"CL_Shutter_Mask1" stringByAppendingString:suffix]]);
    wButtonSetFunction(CL_ShutterButton, CL_ShutterButton_Action);
    
    wView*shutterOverlay=wViewNewWithFrame(wRectNew(0,0,CLM.ShutterSize,CLM.ShutterSize));
    wViewSetIsOpaque(shutterOverlay, false);
    wViewSetBackgroundMask(shutterOverlay, [iOSwTextureExt wTextureFromImageWithName:[@"CL_Shutter_Mask2" stringByAppendingString:suffix]]);
    wViewAddSubview(CL_ShutterButton, shutterOverlay);
    
    if (SM_GetSelectionForSetting(PKSetting_RememberCameraOptions)==0) {
        CM_SetCamera(PKCamera_Back);
        wViewSetFrame(CL_PreviewCamera, CL_Preview_FullFrame);
        wViewSetTag(CL_Preview, 0);
        wViewSetAlpha(CL_PreviewGrid, 1.0);
        
        SM_SetSelectionForSetting(PKSetting_Camera, 0);
        SM_SetSelectionForSetting(PKSetting_CameraIsSquare, 0);
        SM_SetSelectionForSetting(PKSetting_CameraGrid, 1);
        SM_SetSelectionForSetting(PKSetting_CameraFilter, 0);
        SM_SetSelectionForSetting(PKSetting_CameraFlash, 0);
        SM_SetSelectionForSetting(PKSetting_CameraTimer,0);
        SM_SaveSettings();
    }
    else{
        PKCamera camera;
        PKCameraResolution resolution;
        
        if (SM_GetSelectionForSetting(PKSetting_Camera)==0) {
            camera=PKCamera_Back;}
        else{camera=PKCamera_Front;}
        
        switch (SM_GetSelectionForSetting(PKSetting_ImageQuality)) {
            case 1:resolution=PKCameraResolution_Medium;break;
            case 2:resolution=PKCameraResolution_Low;break;
            default:resolution=PKCameraResolution_High;break;
        }
        CM_SetCameraWithResolution(camera, resolution);
        
        if (SM_GetSelectionForSetting(PKSetting_CameraIsSquare)==PKSelection_Yes) {
            wViewSetFrame(CL_PreviewCamera, CL_Preview_SquareFrame);}
        else{wViewSetFrame(CL_PreviewCamera, CL_Preview_FullFrame);}
        
        if (SM_GetSelectionForSetting(PKSetting_CameraGrid)==0) {
            wViewSetAlpha(CL_PreviewGrid, 0.0);}
        else{wViewSetAlpha(CL_PreviewGrid, 1.0);}
        
        wViewSetTag(CL_Preview,
                    SM_GetSelectionForSetting(PKSetting_CameraFilter));
    }
    
    
    NSURL *shutterURL = [NSURL fileURLWithPath:[[NSBundle mainBundle] pathForResource:@"Shutter" ofType:@"aiff"]];
    AudioServicesCreateSystemSoundID((__bridge CFURLRef)shutterURL, &ShutterSound);
    
    NSURL *timerURL = [NSURL fileURLWithPath:[[NSBundle mainBundle] pathForResource:@"Timer" ofType:@"wav"]];
    AudioServicesCreateSystemSoundID((__bridge CFURLRef)timerURL, &TimerSound);
    
   // CL_FilterButtonAction();
    CL_Store_IconTexture=[iOSwTextureExt wTextureFromImageWithName:[@"CL_StoreIcon" stringByAppendingString:buttonNameSuffix]];
}
