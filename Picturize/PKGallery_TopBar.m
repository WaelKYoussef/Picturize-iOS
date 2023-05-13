//
//  PKGallery_TopBar.m
//  PhotoKit
//
//  Created by Wael on 8/25/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#import "PKGallery_TopBar.h"
#import "PKGalleryLayer.h"
#import "PKGalleryFileManager.h"
#import "PKCamera.h"
#import "PKGallery_Canvas.h"
#import "PKExportManager.h"

BOOL GL_TopBar_LV2_DeleteButton_IsEnabled;
BOOL GL_TopBar_LV2_ShareButton_IsEnabled;

void GL_TopBar_LV1_SelectModeButton_SetIsDisplayed(bool isDisplayed){
    
    if (isDisplayed) {wViewShow(GL_TopBar_LV1_SelectModeButton);
        wViewSetAlpha(GL_TopBar_LV1_SelectModeButton, 0.0);}
    else{wViewSetAlpha(GL_TopBar_LV1_SelectModeButton, 1.0);}
    
    wAnimationBeginContext();
    wAnimationSetDuration(0.3);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    if (isDisplayed) {
        wViewAnimateAlpha(GL_TopBar_LV1_SelectModeButton, 1.0);}
    else{wAnimationSetEndFunction(wViewHide, GL_TopBar_LV1_SelectModeButton);
        wViewAnimateAlpha(GL_TopBar_LV1_SelectModeButton, 0.0);}
    wAnimationApplyContext();
}

void GL_TopBar_LV1_SelectModeButton_Action(){
    if (!wViewGetIsHidden(GL_PhotoGrid)) {
        GL_PhotoGrid_EnterSelectMode();
        GL_LoadPhotos(wDeviceGetOrientation());
        GL_TopBar_DisplayLevel(2);
    }else if (!wViewGetIsHidden(GL_Canvas)){
        GL_TopBar_DisplayLevel(2);
    }
}

void GL_TopBar_LV2_DoneButton_Action(){
    if (!wViewGetIsHidden(GL_PhotoGrid)) {
        GL_PhotoGrid_EndSelectMode();
        GL_LoadPhotos(wDeviceGetOrientation());
        GL_TopBar_DisplayLevel(1);
    }else if (!wViewGetIsHidden(GL_Canvas)){
        GL_TopBar_DisplayLevel(1);
    }
}

BOOL GL_TopBar_LV2_ShareButton_Action(){
    if (!GL_TopBar_LV2_ShareButton_IsEnabled) {return false;}
    
    if (EM_IsDisplayed()) {
        EM_Hide();
    }
    else{
        EM_Display();
    }
    
    return true;
}

BOOL GL_TopBar_LV2_DeleteButton_Action_Process(NSMutableArray*itemsToDelete){
    NSError*Err=nil;
    [[PHPhotoLibrary sharedPhotoLibrary] performChangesAndWait:^{
        if (SelectedAlbumDeleteMode==AlbumDeleteModeRemove) {
            PHAssetCollection*Album=[Albums objectAtIndex:SelectedAlbumIndex];
            PHAssetCollectionChangeRequest*changeRequest=[PHAssetCollectionChangeRequest changeRequestForAssetCollection:Album];
            [changeRequest removeAssets:itemsToDelete];
        }
        else if (SelectedAlbumDeleteMode==AlbumDeleteModeDelete){
            [PHAssetChangeRequest deleteAssets:itemsToDelete];
        }
    } error:&Err];
    
    if (Err)return false;
    else return true;
}

BOOL GL_TopBar_LV2_DeleteButton_Action_Confirm(){
    if (!GL_TopBar_LV2_DeleteButton_IsEnabled) {return false;}
    NSMutableArray*itemsToDelete=[NSMutableArray arrayWithCapacity:10];
    if (!wViewGetIsHidden(GL_PhotoGrid)) {
        
        unsigned int count=wTableViewGetItemCount(GL_PhotoGrid);
        
        if (SelectedAlbumDeleteMode==AlbumDeleteModeRemove) {
            for (unsigned int i=0; i<count; i++) {
                if (selectedFlags[i]) {
                    [itemsToDelete addObject:[SelectedAlbum objectAtIndex:i]];}
            }
        }
        else if (SelectedAlbumDeleteMode==AlbumDeleteModeDelete){
            for (unsigned int i=0; i<count; i++) {
                if (selectedFlags[i]) {
                    PHAsset*Asset=[SelectedAlbum objectAtIndex:i];
                    if ([Asset canPerformEditOperation:PHAssetEditOperationDelete]) {
                        [itemsToDelete addObject:Asset];}
                    else {selectedFlags[i]=false;}
                }
            }
        }
        
        if (GL_TopBar_LV2_DeleteButton_Action_Process(itemsToDelete)) {
            for (unsigned int i=count-1; i!=InvalidIndex; i--) {
                if (selectedFlags[i]) {
                    [SelectedAlbum removeObjectAtIndex:i];
                }
            }
            
            if (selectedFlags) {free(selectedFlags),selectedFlags=NULL;}
            unsigned int size=sizeof(bool)*wTableViewGetItemCount(GL_PhotoGrid);
            selectedFlags=malloc(size);
            memset(selectedFlags, false, size);
            
            wScrollViewSetContentOffset(GL_PhotoGrid, wPointZero);
            wTableViewSetItemCount(GL_PhotoGrid, SelectedAlbum.count);
            GL_LoadPhotos(wDeviceGetOrientation());
            wTableViewUpdateItemAtIndex(GL_AlbumGrid, SelectedAlbumIndex);
        }
        
        
    }else if (!wViewGetIsHidden(GL_Canvas)) {
        [itemsToDelete addObject:[SelectedAlbum objectAtIndex:IndexOfLoadedAsset]];
        if (GL_TopBar_LV2_DeleteButton_Action_Process(itemsToDelete)) {
            [SelectedAlbum removeObjectAtIndex:IndexOfLoadedAsset];
            
            GL_TopBar_ShouldUpdateGrids=true;
            if (SelectedAlbum.count>0) {GL_Canvas_LoadNextPhoto();}
            else{GL_TopBar_BackButton_Action();}
        }
    }
    
    return true;
}

BOOL GL_TopBar_LV2_DeleteButton_Action(){
    if (!GL_TopBar_LV2_DeleteButton_IsEnabled) {return false;}
    if (EM_IsDisplayed()) {
        EM_Hide();
    }
    if (SelectedAlbumDeleteMode==AlbumDeleteModeRemove) {
        [AM displayAlert:PKAlertType_Remove_Photos_Confirm];
    }
    else if (SelectedAlbumDeleteMode==AlbumDeleteModeDelete){
        [AM displayAlert:PKAlertType_Delete_Photos_Confirm];
    }
    return true;
}

void GL_TopBar_SetTitle(NSString*title){
    wRect BarFrame=wViewGetFrame(GL_TopBar);
    
    wTexture*Title=[iOSwTextureExt wTextureFromNSString:title Font:GL_TopBar_Font MaxWidth:BarFrame.size.width];
    wTexture*currentTitle=wViewGetBackgroundMask(GL_TopBar_LV1_Title);
    if (currentTitle) {wTextureRelease(currentTitle);}
    wViewSetBackgroundMask(GL_TopBar_LV1_Title, Title);

    wSize TitleSize=wSizeNew(Title[0].width, Title[0].height);
    wRect TitleFrame=wRectNew((BarFrame.size.width-TitleSize.width)/2.0, ((BarFrame.size.height-GLM.BarBorderSize)-TitleSize.height)/2.0, TitleSize.width, TitleSize.height);
    wViewSetFrame(GL_TopBar_LV1_Title, TitleFrame);
}


void GL_TopBar_GoToCamera(){
    GL_PhotoGrid_EndSelectMode();
    
    CM_StartCamera();
    wRect frame=wScreenGetFrame();
    frame.origin.x+=frame.size.width;
    
    wViewSetIsHidden(CameraLayer, false);
    wAnimationBeginContext();
    wAnimationSetDuration(0.5);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    wAnimationSetEndFunction(wViewHide, GalleryLayer);
    wViewAnimateFrame(GalleryLayer, frame);
    wAnimationApplyContext();
    
    wAnimationBeginContext();
    wAnimationSetDuration(0.3);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    wViewAnimateFrame(GL_AlbumGrid, frame);
    wViewAnimateFrame(GL_PhotoGrid, frame);
    wViewAnimateFrame(GL_Canvas, frame);
    wAnimationApplyContext();
    
    wAutoRotationSetAnimateResizing(false);
    wAutoRotationSetAnimateRotation(false);
    [iOSwOpenGLView overrideUserInputOrientation:true];
}

bool GL_TopBar_BackButton_Action(){
    wAutoRotationSetAnimateResizing(false);
    
    if (!wViewGetIsHidden(GL_Canvas)) {
        if (GL_TopBar_ShouldUpdateGrids) {
            wScrollViewSetContentOffset(GL_PhotoGrid, wPointZero);
            wTableViewSetItemCount(GL_PhotoGrid, SelectedAlbum.count);
            GL_LoadPhotos(wDeviceGetOrientation());
            wTableViewUpdateItemAtIndex(GL_AlbumGrid, SelectedAlbumIndex);
            GL_TopBar_ShouldUpdateGrids=false;
        }
        wRect frame=wScreenGetFrame();
        frame.origin.x+=frame.size.width;
        wViewShow(GL_PhotoGrid);
        
        wAnimationBeginContext();
        wAnimationSetDuration(0.3);
        wAnimationSetEndFunction(GL_SetDisplayedView, GL_PhotoGrid);
        wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
        wViewAnimateFrame(GL_Canvas, frame);
        wAnimationApplyContext();        
    }else if (!wViewGetIsHidden(GL_PhotoGrid)){
        NSString*title=[NSString stringWithUTF8String:wStringGetCString(SM_GetTitleAtIndex(0))];
        
        GL_TopBar_SetTitle(title);
        
        wRect frame=wScreenGetFrame();
        frame.origin.x+=frame.size.width;
        wViewShow(GL_AlbumGrid);
        
        wAnimationBeginContext();
        wAnimationSetDuration(0.3);
        wAnimationSetEndFunction(GL_SetDisplayedView, GL_AlbumGrid);
        wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
        wViewAnimateFrame(GL_PhotoGrid, frame);
        wAnimationApplyContext();
        GL_TopBar_LV1_SelectModeButton_SetIsDisplayed(false);
    }else if (!wViewGetIsHidden(GL_AlbumGrid)){
        GL_TopBar_GoToCamera();
    }
    return true;
}


void GL_TopBar_Create(){
    wRect barFrame=wRectNew(0, 0, wScreenGetWidth(), GLM.BarSize);
    wAutoLayoutOptions barLOOpt=wAutoLayoutNone;
    barLOOpt.widthOption=wAutoLayoutStretchToSuperView;
    
    GL_TopBar=wViewNewWithFrame(barFrame);
    wViewAddSubview(GalleryLayer, GL_TopBar);
    wViewSetIsOpaque(GL_TopBar, false);
    wViewSetAutoLayoutOptions(GL_TopBar, barLOOpt);
    wViewSetClipsSubviews(GL_TopBar, true);
    
    GL_TopBar_LV1=wViewNewWithFrame(barFrame);
    wViewSetAutoLayoutOptions(GL_TopBar_LV1, barLOOpt);
    wViewSetBackgroundColor(GL_TopBar_LV1, wColorClear());
    wViewAddSubview(GL_TopBar, GL_TopBar_LV1);
    
    barFrame.origin.y=barFrame.size.height;
    GL_TopBar_LV2=wViewNewWithFrame(barFrame);
    wViewSetAutoLayoutOptions(GL_TopBar_LV2, barLOOpt);
    wViewSetBackgroundColor(GL_TopBar_LV2, wColorClear());
    wViewAddSubview(GL_TopBar, GL_TopBar_LV2);
    wViewHide(GL_TopBar_LV2);
    
    
    NSString*Suffix=nil;
    switch (deviceType) {
        case deviceType_iPhone:{Suffix=@"35.png";break;}
        case deviceType_iPhoneRetina:{Suffix=@"70.png";break;}
        case deviceType_iPad:{Suffix=@"45.png";break;}
        case deviceType_iPadRetina:{Suffix=@"90.png";break;}
        default:{Suffix=@"45.png";break;}
    }
    
    
    wRect buttonFrame=wRectNew(GLM.OutlineSize, GLM.OutlineSize, GLM.BarButtonSize, GLM.BarButtonSize);
    GL_TopBar_LV1_BackButton=wButtonNewWithFrame(buttonFrame);
    wViewSetBackgroundMask(GL_TopBar_LV1_BackButton, [iOSwTextureExt wTextureFromImageWithName:[@"GL_Back" stringByAppendingString:Suffix]]);
    wButtonSetFunction(GL_TopBar_LV1_BackButton, GL_TopBar_BackButton_Action);
    wViewAddSubview(GL_TopBar_LV1, GL_TopBar_LV1_BackButton);
    wGesture*doupleTap=wGestureNew(wGestureDoubleTap);
    wGestureSetFunction(doupleTap, GL_TopBar_GoToCamera);
    wViewAddGesture(GL_TopBar_LV1_BackButton, doupleTap);
    
    buttonFrame.origin.x=barFrame.size.width-GLM.BarButtonSize-GLM.OutlineSize;
    GL_TopBar_LV1_SelectModeButton=wButtonNewWithFrame(buttonFrame);
    wViewSetBackgroundMask(GL_TopBar_LV1_SelectModeButton, [iOSwTextureExt wTextureFromImageWithName:[@"GL_SelectMode"stringByAppendingString:Suffix]]);
    wButtonSetFunction(GL_TopBar_LV1_SelectModeButton,GL_TopBar_LV1_SelectModeButton_Action);
    wAutoLayoutOptions buttonLOOpt=wAutoLayoutNone;
    buttonLOOpt.anchor.x=1.0;
    wViewSetAutoLayoutOptions(GL_TopBar_LV1_SelectModeButton, buttonLOOpt);
    wViewAddSubview(GL_TopBar_LV1, GL_TopBar_LV1_SelectModeButton);
    
    GL_TopBar_LV1_Title=wViewNew();
    wViewSetIsOpaque(GL_TopBar_LV1_Title, false);
    wViewAddSubviewAtIndex(GL_TopBar, GL_TopBar_LV1_Title, 0);
    buttonLOOpt.anchor.x=0.5;
    wViewSetAutoLayoutOptions(GL_TopBar_LV1_Title, buttonLOOpt);
    wViewAddSubview(GL_TopBar_LV1, GL_TopBar_LV1_Title);
    

    
    
    GL_TopBar_Font=[iOSwTextureExt wFontNewFromUIFont:[UIFont fontWithName:@"HelveticaNeue-Light" size:GLM.TitleFontSize]];
    GL_TopBar_LV2_DeleteButton_RemoveIcon=[iOSwTextureExt wTextureFromImageWithName:[@"GL_Remove" stringByAppendingString:Suffix]];
    GL_TopBar_LV2_DeleteButton_DeleteIcon=[iOSwTextureExt wTextureFromImageWithName:[@"GL_Delete" stringByAppendingString:Suffix]];

    
    
    buttonFrame.origin.x=GLM.OutlineSize;
    GL_TopBar_LV2_DeleteButton=wButtonNewWithFrame(buttonFrame);
    wButtonSetFunction(GL_TopBar_LV2_DeleteButton, GL_TopBar_LV2_DeleteButton_Action);
    wViewAddSubview(GL_TopBar_LV2, GL_TopBar_LV2_DeleteButton);
    
    buttonFrame.origin.x=(barFrame.size.width-GLM.BarButtonSize)/2.0;
    GL_TopBar_LV2_ShareButton=wButtonNewWithFrame(buttonFrame);
    wViewSetBackgroundMask(GL_TopBar_LV2_ShareButton, [iOSwTextureExt wTextureFromImageWithName:[@"GL_Export" stringByAppendingString:Suffix]]);
    wButtonSetFunction(GL_TopBar_LV2_ShareButton, GL_TopBar_LV2_ShareButton_Action);
    buttonLOOpt.anchor.x=0.5;
    wViewSetAutoLayoutOptions(GL_TopBar_LV2_ShareButton, buttonLOOpt);
    wViewAddSubview(GL_TopBar_LV2, GL_TopBar_LV2_ShareButton);
    
    buttonFrame.origin.x=barFrame.size.width-GLM.BarButtonSize-GLM.OutlineSize;
    GL_TopBar_LV2_DoneButton=wButtonNewWithFrame(buttonFrame);
    wViewSetBackgroundMask(GL_TopBar_LV2_DoneButton, [iOSwTextureExt wTextureFromImageWithName:[@"GL_Done" stringByAppendingString:Suffix]]);
    wButtonSetFunction(GL_TopBar_LV2_DoneButton, GL_TopBar_LV2_DoneButton_Action);
    buttonLOOpt.anchor.x=1.0;
    wViewSetAutoLayoutOptions(GL_TopBar_LV2_DoneButton, buttonLOOpt);
    wViewAddSubview(GL_TopBar_LV2, GL_TopBar_LV2_DoneButton);
    
    GL_TopBar_Border=wViewNewWithFrame(wRectNew(0, barFrame.size.height-GLM.BarBorderSize, barFrame.size.width, GLM.BarBorderSize));
    wAutoLayoutOptions opt=wAutoLayoutNone;
    opt.widthOption=wAutoLayoutStretchToSuperView;
    wViewSetAutoLayoutOptions(GL_TopBar_Border, opt);
    wViewAddSubview(GL_TopBar, GL_TopBar_Border);
}

void GL_TopBar_DisplayLevel(unsigned char level){
    wAnimationBeginContext();
    wAnimationSetDuration(0.3);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    
    wRect LV1Frame,LV2Frame;
    wViewShow(GL_TopBar_LV1),wViewShow(GL_TopBar_LV2);
    if (level==1) {
        LV1Frame=wRectNew(0, -GLM.BarSize, wScreenGetWidth(), GLM.BarSize);
        LV2Frame=wRectNew(0, 0, wScreenGetWidth(), GLM.BarSize);
        wViewSetFrame(GL_TopBar_LV1, LV1Frame);
        wViewSetFrame(GL_TopBar_LV2, LV2Frame);
        LV1Frame.origin.y+=GLM.BarSize;
        LV2Frame.origin.y+=GLM.BarSize;
        wAnimationSetEndFunction(wViewHide, GL_TopBar_LV2);
    }
    else{
        LV1Frame=wRectNew(0, 0, wScreenGetWidth(), GLM.BarSize);
        LV2Frame=wRectNew(0, GLM.BarSize, wScreenGetWidth(), GLM.BarSize);
        wViewSetFrame(GL_TopBar_LV1, LV1Frame);
        wViewSetFrame(GL_TopBar_LV2, LV2Frame);
        LV1Frame.origin.y-=GLM.BarSize;
        LV2Frame.origin.y-=GLM.BarSize;
        wAnimationSetEndFunction(wViewHide, GL_TopBar_LV1);
        
        switch (SelectedAlbumDeleteMode) {
            case AlbumDeleteModeNone:{wViewHide(GL_TopBar_LV2_DeleteButton);break;}
            case AlbumDeleteModeRemove:{wViewShow(GL_TopBar_LV2_DeleteButton);
                wViewSetBackgroundMask(GL_TopBar_LV2_DeleteButton, GL_TopBar_LV2_DeleteButton_RemoveIcon);break;}
            case AlbumDeleteModeDelete:{wViewShow(GL_TopBar_LV2_DeleteButton);
                wViewSetBackgroundMask(GL_TopBar_LV2_DeleteButton, GL_TopBar_LV2_DeleteButton_DeleteIcon);break;}
            default:{wViewHide(GL_TopBar_LV2_DeleteButton);break;}
        }
    }
    
    wViewAnimateFrame(GL_TopBar_LV1, LV1Frame);
    wViewAnimateFrame(GL_TopBar_LV2, LV2Frame);
    wAnimationApplyContext();
    
    if (level==1 && EM_IsDisplayed()) {EM_Hide();}
}


void GL_TopBar_LV2_DeleteButton_SetIsEnabled(bool enabled){
    GL_TopBar_LV2_DeleteButton_IsEnabled=enabled;
    if (enabled) {wViewSetBackgroundColor(GL_TopBar_LV2_DeleteButton, wColorCrimson());}
    else{wViewSetBackgroundColor(GL_TopBar_LV2_DeleteButton, wColorDarkGray());}
}
void GL_TopBar_LV2_ShareButton_SetIsEnabled(bool enabled){
    GL_TopBar_LV2_ShareButton_IsEnabled=enabled;
    if (enabled) {wViewSetBackgroundColor(GL_TopBar_LV2_ShareButton, PKForeColor);}
    else{if (EM_IsDisplayed()) {EM_Hide();}
    wViewSetBackgroundColor(GL_TopBar_LV2_ShareButton, wColorDarkGray());}
}

void GL_TopBar_Show(){
    wRect topBarFrame=wRectNew(0, -GLM.BarSize, wScreenGetWidth(), GLM.BarSize);
    wViewSetFrame(GL_TopBar, topBarFrame);
    wViewShow(GL_TopBar);
    topBarFrame.origin.y+=GLM.BarSize;
    
    wAnimationBeginContext();
    wAnimationSetDuration(0.3);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    wViewAnimateFrame(GL_TopBar, topBarFrame);
    wAnimationApplyContext();
}

void GL_TopBar_Hide(){
    wRect topBarFrame=wRectNew(0, 0, wScreenGetWidth(), GLM.BarSize);
    wViewSetFrame(GL_TopBar, topBarFrame);
    topBarFrame.origin.y-=GLM.BarSize;
    
    wAnimationBeginContext();
    wAnimationSetDuration(0.3);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    wAnimationSetEndFunction(wViewHide, GL_TopBar);
    wViewAnimateFrame(GL_TopBar, topBarFrame);
    wAnimationApplyContext();
}

