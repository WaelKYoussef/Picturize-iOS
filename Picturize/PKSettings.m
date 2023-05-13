//
//  PKSettingsManager.m
//  PhotoKit
//
//  Created by Wael Youssef on 7/17/14.
//  Copyright (c) 2014 WaelKhaledYoussef. All rights reserved.
//

#import "PKCamera.h"
#import "PKSettings.h"
#import "PKGalleryLayer.h"
#import "PKExportManager.h"

#pragma mark -
#pragma mark - SettingsManager.
wArray*SL_ApplicationText=NULL;
NSString*PathForDataFolder;
NSString*PathForSettingsFile;


void SM_LoadSettings(){
    NSData*fileBuffer=[NSData dataWithContentsOfFile:PathForSettingsFile];
    const void*buffer=[fileBuffer bytes];
    memcpy(&SM_SettingsValues, buffer, sizeof(SM_SettingsValues));
}

void SM_SaveSettings(){
    NSData*fileBuffer=[[NSData alloc] initWithBytes:&SM_SettingsValues length:sizeof(SM_SettingsValues)];
    [fileBuffer writeToFile:PathForSettingsFile atomically:TRUE];
}

void SM_SaveCameraSettings(){
    if (SM_GetSelectionForSetting(PKSetting_RememberCameraOptions)==1) {
        SM_SaveSettings();
    }
}

void SM_LoadLanguage(PKLanguage language){
    NSString*fileName=NULL;
    switch (language) {
    case PKLanguage_English:fileName=@"En";break;
    case PKLanguage_Arabic:fileName=@"Ar";break;
    default:fileName=@"En";break;}
    
    NSString*LangFile=[[NSBundle mainBundle] pathForResource:fileName ofType:@"Lang"];
    NSData*fileBuffer=[NSData dataWithContentsOfFile:LangFile];
    const void*buffer=[fileBuffer bytes];
    
    
    if (SL_ApplicationText) {
    for (int i=wArrayGetCount(SL_ApplicationText)-1; i>=0; i--) {
    wStringRelease(wArrayGetItemAtIndex(SL_ApplicationText, i));}
    wArrayRelease(SL_ApplicationText),SL_ApplicationText=NULL;}
    
    SL_ApplicationText=wStringArrayNewFromData(buffer);
}

void SM_Setup(){
    NSArray*PathArray=NSSearchPathForDirectoriesInDomains
    (NSDocumentDirectory, NSUserDomainMask, TRUE);
    NSString*FileDirectoryPath=[PathArray objectAtIndex:0];
    
    PathForDataFolder=[FileDirectoryPath stringByAppendingPathComponent:@"/.DataFolder/"];
    PathForSettingsFile=[FileDirectoryPath stringByAppendingPathComponent:@".DataFolder/.SettingsFile.data"];
    
    NSFileManager*FileMan=[NSFileManager defaultManager];
    if (![FileMan fileExistsAtPath:PathForDataFolder]){
        isFirstTimeRun=true;
        [FileMan createDirectoryAtPath:PathForDataFolder withIntermediateDirectories:NO attributes:nil error:nil];}
    else{isFirstTimeRun=false;}
    
    if (![FileMan fileExistsAtPath: PathForSettingsFile]){
        memset(&SM_SettingsValues, 0, sizeof(SM_SettingsValues));
        SM_SaveSettings();}
    else{SM_LoadSettings();}
    
    NSURL*url=[NSURL fileURLWithPath:PathForDataFolder isDirectory:true];
    [url setResourceValue:[NSNumber numberWithBool:true] forKey:NSURLIsExcludedFromBackupKey error:nil];
    
    SM_LoadLanguage(0);
}



int SM_GetNumberOfSettings(){
    return 5;
}

wString*SM_GetTitleAtIndex(unsigned int index){
    wString*title=wArrayGetItemAtIndex(SL_ApplicationText, index);
    return title;
}

wString*SM_GetTitleForSetting(PKSetting setting){
    wString*title=wArrayGetItemAtIndex(SL_ApplicationText, setting+2);
    return title;
}

unsigned int SM_GetOptionsCountForSetting(PKSetting setting){
    switch (setting) {
        case PKSetting_ColorTheme:return 2;break;
        case PKSetting_ImageQuality:return 3;break;
        default:return 0;break;
    }
}

wString*SM_GetOptionTitleForSetting(PKSetting setting, int Option){
    
    int selectionsIndex=0;
    switch (setting) {
    case PKSetting_ColorTheme:selectionsIndex=9;break;
    case PKSetting_ImageQuality:selectionsIndex=11;break;
    default:selectionsIndex=7; break;}
    
    wString*title=wArrayGetItemAtIndex(SL_ApplicationText, selectionsIndex+Option);
    return title;
}

PKSettingType SM_GetTypeForSetting(PKSetting setting){
    if (setting==2 || setting==3 || setting==4) {
        return PKSettingTypeBoolean;}
    else{return PKSettingTypeOptions;}
}

int SM_GetSelectionForSetting(PKSetting setting){
    return SM_SettingsValues[setting];
}

void SM_SetSelectionForSetting(PKSetting setting, int selection){
    SM_SettingsValues[setting]=selection;
}

void SM_SetApplicationColorTheme(PKColorTheme colorTheme){
    switch (colorTheme) {
        case PKColorTheme_Dark:{
            PKForeColor=wColorWhite();
            PKBackColor=wColorBlack();
            PKBarColor=wColorNewRGBAFloat(0.0, 0.0, 0.0, 0.6);
            break;}
            
        default:{
            PKForeColor=wColorBlack();
            PKBackColor=wColorWhite();
            PKBarColor=wColorNewRGBAFloat(1.0, 1.0, 1.0, 0.6);
            break;}
    }
    
    SL_SetColorTheme(colorTheme);
    CL_SetColorTheme(colorTheme);
    GL_SetColorTheme(colorTheme);
    EM_SetColorTheme();
}


#pragma mark -
#pragma mark - SettingsLayer.
wView*SL_SettingsList;
wView*SL_SettingsOptionList;

wView*SL_AboutBlock;

wView*SL_Bar;
wView*SL_BackButton;
wView*SL_Bar_Title;
wView*SL_TopBar_Border;

wTexture*SL_ArrowTexture;
wTexture*SL_SelectionMark;

wTexture*SL_BackTexture;
wTexture*SL_CloseTexture;

wFont SL_Title_Font;
wFont SL_Setting_Font;
PKSetting selectedSetting=0;

void SL_AdjustOrientation(wDeviceOrientation orientation){
    wTableViewSetOffsetFromBottom(SL_SettingsList, wViewGetFrame(SL_AboutBlock).size.height+(SLM.OptionSize));
    wTableViewSetOffsetFromBottom(SL_SettingsOptionList, 0);

    wRect frame=wViewGetFrame(SL_SettingsList);
    if (wDeviceOrientationIsPortrait(orientation)) {
        frame.size=wScreenGetPortraitFrame().size;
    }
    else{
        frame.size=wScreenGetLandscapeFrame().size;
    }
    
    wViewSetFrame(SL_SettingsList, frame);
    wTableViewLoad(SL_SettingsList);
    if (!wViewGetIsHidden(SL_SettingsOptionList)){
        wViewSetFrame(SL_SettingsOptionList, frame);
        wTableViewLoad(SL_SettingsOptionList);
    }
}

void SL_SetColorTheme(PKColorTheme colorTheme){
    wAnimationBeginContext();
    wAnimationSetDuration(0.3);
    wViewAnimateBackgroundColor(SL_BackButton, PKForeColor);
    wViewAnimateBackgroundColor(SL_Bar, PKBarColor);
    wViewAnimateBackgroundColor(SL_Bar_Title, PKForeColor);
    wAnimationApplyContext();
    
    
    wColor borderColor=PKBarColor;
    borderColor.a=1.0;
    wViewSetBackgroundColor(SL_TopBar_Border, borderColor);
    
    wViewSetBackgroundColor(SL_SettingsList, wColorNewGreyScaleFloat(0.99, 1.0));
    wViewSetBackgroundColor(SL_SettingsOptionList, wColorNewGreyScaleFloat(0.99, 1.0));
}

void SL_Bar_SetTitle(NSString*title){
    wRect BarFrame=wViewGetFrame(SL_Bar);
    
    wTexture*Title=[iOSwTextureExt wTextureFromNSString:title Font:SL_Title_Font MaxWidth:BarFrame.size.width];
    wTexture*currentTitle=wViewGetBackgroundMask(SL_Bar_Title);
    if (currentTitle) {wTextureRelease(currentTitle);}
    wViewSetBackgroundMask(SL_Bar_Title, Title);
    
    wSize TitleSize=wSizeNew(Title[0].width, Title[0].height);
    wRect TitleFrame=wRectNew((BarFrame.size.width-TitleSize.width)/2.0, ((BarFrame.size.height-SLM.BarBorderSize)-TitleSize.height)/2.0, TitleSize.width, TitleSize.height);
    wViewSetFrame(SL_Bar_Title, TitleFrame);
}

void SL_GoToCamera(){
    wRect screenFrame=wScreenGetFrame();
    screenFrame.origin.y+=screenFrame.size.height;
    
    CM_StartCamera();
    wViewShow(CameraLayer);
    
    wAnimationBeginContext();
    wAnimationSetDuration(0.5);
    wAnimationSetEndFunction(wViewHide, SettingsLayer);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    wViewAnimateFrame(SettingsLayer, screenFrame);
    wAnimationApplyContext();
    
    wAutoRotationSetAnimateRotation(false);
    [iOSwOpenGLView overrideUserInputOrientation:true];
}

void SL_BackButtonAction(void*gesture){
    if (!wViewGetIsHidden(SL_SettingsOptionList)) {
        wViewSetBackgroundMask(SL_BackButton, SL_CloseTexture);

        wRect screenFrame=wScreenGetFrame();
        screenFrame.origin.x+=screenFrame.size.width;
        
        SL_Bar_SetTitle([NSString stringWithUTF8String:wStringGetCString(SM_GetTitleAtIndex(1))]);
        
        wViewShow(SL_SettingsList);
        
        wAnimationBeginContext();
        wAnimationSetDuration(0.3);
        wAnimationSetEndFunction(wViewHide, SL_SettingsOptionList);
        wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
        wViewAnimateFrame(SL_SettingsOptionList, screenFrame);
        wAnimationApplyContext();
    }else if (!wViewGetIsHidden(SL_SettingsList)){
        SL_GoToCamera();
    }
}

void SL_Display_End(){
    wViewHide(CameraLayer);
    CM_StopCamera();
}

void SL_Display(){
    wAutoRotationSetAnimateRotation(true);
    [iOSwOpenGLView overrideUserInputOrientation:false];
    
    wViewSetBackgroundMask(SL_BackButton, SL_CloseTexture);
    
    wRect screenFrame=wScreenGetFrame();
    screenFrame.origin.y+=screenFrame.size.height;
    wViewSetFrame(SettingsLayer, screenFrame);
    screenFrame.origin.y-=screenFrame.size.height;
    
    SL_Bar_SetTitle([NSString stringWithUTF8String:wStringGetCString(SM_GetTitleAtIndex(1))]);
    
    wViewShow(SettingsLayer);
    wViewShow(SL_SettingsList);
    wViewHide(SL_SettingsOptionList);
    
    screenFrame.origin.x+=screenFrame.size.width;
    wViewSetFrame(SL_SettingsOptionList, screenFrame);
    screenFrame.origin.x-=screenFrame.size.width;
    
    wScrollViewSetContentOffset(SL_SettingsList, wPointZero);
    wTableViewSetItemCount(SL_SettingsList, SM_GetNumberOfSettings());
    SL_AdjustOrientation(wDeviceGetOrientation());
    
    wAnimationBeginContext();
    wAnimationSetDuration(0.5);
    wAnimationSetEndFunction(SL_Display_End, NULL);
    wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
    wViewAnimateFrame(SettingsLayer, screenFrame);
    wAnimationApplyContext();
}



void SL_SettingsList_SelectSettingAction(int index){
    
    selectedSetting=index;
    
    PKSettingType type=SM_GetTypeForSetting(index);
    if (type==PKSettingTypeOptions) {
        wViewSetBackgroundMask(SL_BackButton, SL_BackTexture);

        wRect screenFrame=wScreenGetFrame();
        screenFrame.origin.x+=screenFrame.size.width;
        wViewSetFrame(SL_SettingsOptionList, screenFrame);
        screenFrame.origin.x-=screenFrame.size.width;
        
        SL_Bar_SetTitle([NSString stringWithUTF8String:wStringGetCString(SM_GetTitleForSetting(index))]);
        
        wScrollViewSetContentOffset(SL_SettingsOptionList, wPointZero);
        wTableViewSetItemCount(SL_SettingsOptionList, SM_GetOptionsCountForSetting(index));
        wTableViewSetOffsetFromBottom(SL_SettingsOptionList, 0);
        wTableViewLoad(SL_SettingsOptionList);
        wViewShow(SL_SettingsOptionList);
        wAnimationBeginContext();
        wAnimationSetDuration(0.3);
        wAnimationSetEndFunction(wViewHide, SL_SettingsList);
        wAnimationSetAnimationCurve(wAnimationCurveEaseOut);
        wViewAnimateFrame(SL_SettingsOptionList, screenFrame);
        wAnimationApplyContext();
        
    }
    else{
        if (SM_GetSelectionForSetting(index)==0) {
            SM_SetSelectionForSetting(index, 1);
        }
        else{
            SM_SetSelectionForSetting(index, 0);
        }
        wTableViewUpdateItemAtIndex(SL_SettingsList, index);
        SM_SaveSettings();
    }
}

void SL_SettingsList_CleanSetting(wView*settingSlot, int index){
    wArray*subviews=wViewGetSubviewArray(settingSlot);
    wView*titleView=wArrayGetItemAtIndex(subviews, 0);

    wTextureRelease(wViewGetBackgroundMask(titleView));
    wViewSetBackgroundMask(titleView, NULL);

    wView*subtitleView=wArrayGetItemAtIndex(subviews,1);
    wTextureRelease(wViewGetBackgroundMask(subtitleView));
    wViewSetBackgroundMask(subtitleView, NULL);
}

wView*SL_SettingsList_ViewForSetting(){
    wView*settingsSlot=wViewNew();
    wView*titleView=wViewNew();
    wView*subtitleView=wViewNew();
    wView*arrowView=wViewNew();
    
    wViewAddSubview(settingsSlot, titleView);
    wViewAddSubview(settingsSlot, subtitleView);
    wViewAddSubview(settingsSlot, arrowView);
    
    wViewSetIsOpaque(settingsSlot, false);
    wViewSetIsOpaque(titleView, false);
    wViewSetIsOpaque(subtitleView, false);
    wViewSetIsOpaque(arrowView, false);
        
    wViewSetBackgroundMask(arrowView, SL_ArrowTexture);
    wViewSetClipsSubviews(settingsSlot, true);
    
    wViewSetBackgroundColor(titleView, wColorBlack());
    wViewSetBackgroundColor(arrowView, wColorBlack());

    return settingsSlot;
}

void SL_SettingsList_PresentSetting(wView*settingsSlot, int index){
    wArray*subviews=wViewGetSubviewArray(settingsSlot);
    wView*titleView=wArrayGetItemAtIndex(subviews, 0);
    wView*subtitleView=wArrayGetItemAtIndex(subviews, 1);
    wView*arrowView=wArrayGetItemAtIndex(subviews, 2);
    
    wRect slotFrame=wViewGetFrame(settingsSlot);
    wString*string=SM_GetTitleForSetting(index);
    wTexture*title=[iOSwTextureExt wTextureFromWString:string Font:SL_Setting_Font MaxWidth:slotFrame.size.width];
    wViewSetBackgroundMask(titleView, title);
    
    
    wRect titleFrame=wRectNew(SLM.OutlineSize, 0, title[0].width, title[0].height);
    titleFrame.origin.y=(slotFrame.size.height-titleFrame.size.height)/2.0;
    wViewSetFrame(titleView, titleFrame);
    
    float subtitleOffset=slotFrame.size.width-SLM.OutlineSize;
    unsigned int selection=SM_GetSelectionForSetting(index);
    
    wString*SelectionWTitle=SM_GetOptionTitleForSetting(index, selection);
    NSString*SelectionTitle=[NSString stringWithUTF8String:wStringGetCString(SelectionWTitle)];
    if (SM_GetTypeForSetting(index)==PKSettingTypeOptions) {
        subtitleOffset-=(SLM.ArrowMarkSize+SLM.OutlineSize);
        wRect arrowFrame=wRectNew(slotFrame.size.width-SLM.ArrowMarkSize-SLM.OutlineSize, 0, SLM.ArrowMarkSize, SLM.ArrowMarkSize);
        arrowFrame.origin.y=(slotFrame.size.height-SLM.ArrowMarkSize)/2.0;
        wViewSetFrame(arrowView, arrowFrame);
        wViewShow(arrowView);
        
        wViewSetBackgroundColor(subtitleView, wColorDeepSkyBlue());
    }
    else{
        wViewHide(arrowView);
        if (SM_GetSelectionForSetting(index)==0) {
            wViewSetBackgroundColor(subtitleView, wColorRed());}
        else{wViewSetBackgroundColor(subtitleView, wColorGreen());}
    }
    
    wTexture*subtitle=[iOSwTextureExt wTextureFromNSString:SelectionTitle Font:SL_Setting_Font MaxWidth:slotFrame.size.width];
    wViewSetBackgroundMask(subtitleView, subtitle);
    
    
    wRect subtitleFrame=wRectNew(subtitleOffset-subtitle[0].width, 0, subtitle[0].width, subtitle[0].height);
    subtitleFrame.origin.y=(slotFrame.size.height-subtitleFrame.size.height)/2.0;
    wViewSetFrame(subtitleView, subtitleFrame);
}

void SL_SettingsOptionList_SelectOptionAction(int index){
    int previousSelection=SM_GetSelectionForSetting(selectedSetting);
    SM_SetSelectionForSetting(selectedSetting, index);
    
    switch (selectedSetting) {
        case PKSetting_ColorTheme:{
            SM_SetApplicationColorTheme(index);
            wTableViewUpdateItemAtIndex(SL_SettingsList, selectedSetting);
            
            break;}
            
        case PKSetting_ImageQuality:{
            wTableViewUpdateItemAtIndex(SL_SettingsList, selectedSetting);
            CL_SetCameraQuality();
            break;}
            
        default:
            break;
    }
    if (selectedSetting==PKSetting_ColorTheme) {
        wTableViewLoad(SL_SettingsOptionList);
    }else{
        wTableViewUpdateItemAtIndex(SL_SettingsOptionList, previousSelection);
        wTableViewUpdateItemAtIndex(SL_SettingsOptionList, index);
    }
    SM_SaveSettings();
}

void SL_SettingsOptionList_CleanOption(wView*optionSlot, int index){
    wArray*subviews=wViewGetSubviewArray(optionSlot);
    wView*titleView=wArrayGetItemAtIndex(subviews, 0);
    
    wTextureRelease(wViewGetBackgroundMask(titleView));
    wViewSetBackgroundMask(titleView, NULL);
}

wView*SL_SettingsOptionList_ViewForOption(){
    wView*optionSlot=wViewNew();
    wView*titleView=wViewNew();
    wView*selectionView=wViewNew();
    
    wViewAddSubview(optionSlot, titleView);
    wViewAddSubview(optionSlot, selectionView);
    
    wViewSetIsOpaque(optionSlot, false);
    wViewSetIsOpaque(titleView, false);
    wViewSetIsOpaque(selectionView, false);
    
    wViewSetBackgroundColor(selectionView, wColorBlue());
    
    wViewSetBackgroundMask(selectionView, SL_SelectionMark);
    wViewSetClipsSubviews(optionSlot, true);
    
    return optionSlot;
}

void SL_SettingsOptionList_PresentOption(wView*optionSlot, int index){
    wArray*subviews=wViewGetSubviewArray(optionSlot);
    wView*titleView=wArrayGetItemAtIndex(subviews, 0);
    wView*selectionView=wArrayGetItemAtIndex(subviews, 1);
    
    wRect slotFrame=wViewGetFrame(optionSlot);
    wString*string=SM_GetOptionTitleForSetting(selectedSetting, index);
    NSString*optionTitle=[NSString stringWithUTF8String:wStringGetCString(string)];
    wTexture*title=[iOSwTextureExt wTextureFromNSString:optionTitle Font:SL_Setting_Font MaxWidth:slotFrame.size.width];
    wViewSetBackgroundMask(titleView, title);
    
    
    wRect titleFrame=wRectNew(SLM.OutlineSize, 0, title[0].width, title[0].height);
    titleFrame.origin.y=(slotFrame.size.height-titleFrame.size.height)/2.0;
    wViewSetFrame(titleView, titleFrame);
    
    wViewSetBackgroundColor(titleView, wColorBlack());
    wViewSetBackgroundColor(selectionView, wColorBlack());
    
    unsigned int selection=SM_GetSelectionForSetting(selectedSetting);
    if (selection==index) {
        wRect selectionFrame=wRectNew(slotFrame.size.width-SLM.SelectionMarkSize-SLM.OutlineSize, 0, SLM.SelectionMarkSize, SLM.SelectionMarkSize);
        selectionFrame.origin.y=(slotFrame.size.height-SLM.SelectionMarkSize)/2.0;
        wViewSetFrame(selectionView, selectionFrame);
        wViewShow(selectionView);
    }
    else{
        wViewHide(selectionView);
    }
}




void SL_Setup(){
    
    SL_Title_Font=[iOSwTextureExt wFontNewFromUIFont:[UIFont fontWithName:@"HelveticaNeue-Light" size:SLM.TitleFontSize]];
    SL_Setting_Font=[iOSwTextureExt wFontNewFromUIFont:[UIFont fontWithName:@"HelveticaNeue-Light" size:SLM.SettingFontSize]];
    
    wRect frame=wScreenGetFrame();
    frame.origin.y+=frame.size.height;
    
    SettingsLayer=wViewNewWithFrame(frame);
    wViewSetShouldDraw(SettingsLayer, false);
    wViewAddSubview(wRootView, SettingsLayer);
    wViewHide(SettingsLayer);
    wViewSetClipsSubviews(SettingsLayer, true);
    
    frame=wScreenGetFrame();
    SL_SettingsList=wTableViewNewWithFrame(frame);
    wViewAddSubview(SettingsLayer,SL_SettingsList);
    wTableViewSetItemsOutlineSize(SL_SettingsList, SLM.OutlineSize);
    wTableViewSetItemsPerRow(SL_SettingsList, 1);
    wTableViewSetViewForItemAtIndexFunction(SL_SettingsList, SL_SettingsList_ViewForSetting);
    wTableViewSetPresentItemAtIndexFunction(SL_SettingsList, SL_SettingsList_PresentSetting);
    wTableViewSetCleanItemAtIndexFunction(SL_SettingsList, SL_SettingsList_CleanSetting);
    wTableViewSetSelectedItemAtIndexFunction(SL_SettingsList, SL_SettingsList_SelectSettingAction);
    wTableViewSetOffsetFromTop(SL_SettingsList, SLM.BarSize);
    wTableViewSetSquaredItems(SL_SettingsList, false);
    wTableViewSetItemHeight(SL_SettingsList, SLM.OptionSize);
    
    SL_AboutBlock=createAboutBlock();

    frame.origin.x+=frame.size.width;
    SL_SettingsOptionList=wTableViewNewWithFrame(frame);
    wViewAddSubview(SettingsLayer,SL_SettingsOptionList);
    wTableViewSetItemsOutlineSize(SL_SettingsOptionList, SLM.OutlineSize);
    wTableViewSetItemsPerRow(SL_SettingsOptionList, 1);
    wTableViewSetViewForItemAtIndexFunction(SL_SettingsOptionList, SL_SettingsOptionList_ViewForOption);
    wTableViewSetPresentItemAtIndexFunction(SL_SettingsOptionList, SL_SettingsOptionList_PresentOption);
    wTableViewSetCleanItemAtIndexFunction(SL_SettingsOptionList, SL_SettingsOptionList_CleanOption);
    wTableViewSetSelectedItemAtIndexFunction(SL_SettingsOptionList, SL_SettingsOptionList_SelectOptionAction);
    wTableViewSetOffsetFromTop(SL_SettingsOptionList, SLM.BarSize);
    wViewHide(SL_SettingsOptionList);
    wTableViewSetSquaredItems(SL_SettingsOptionList, false);
    wTableViewSetItemHeight(SL_SettingsOptionList, SLM.OptionSize);
    
    wRect barFrame=wRectNew(0, 0, wScreenGetWidth(), SLM.BarSize);
    SL_Bar=wViewNewWithFrame(barFrame);
    wViewAddSubview(SettingsLayer, SL_Bar);
    wViewSetIsOpaque(SL_Bar, false);
    
    NSString*backName=nil;
    NSString*closeName=nil;
    NSString*selectedName=nil;
    NSString*nextPageName=nil;
    
    switch (deviceType) {
        case deviceType_iPhone:{
            backName=@"SL_Back35.png";
            closeName=@"SL_Close35.png";
            selectedName=@"SL_Selected20.png";
            nextPageName=@"SL_NextPage15.png";
            break;}
        case deviceType_iPhoneRetina:{
            backName=@"SL_Back70.png";
            closeName=@"SL_Close70.png";
            selectedName=@"SL_Selected40.png";
            nextPageName=@"SL_NextPage30.png";
            break;}
        case deviceType_iPad:{
            backName=@"SL_Back45.png";
            closeName=@"SL_Close45.png";
            selectedName=@"SL_Selected25.png";
            nextPageName=@"SL_NextPage20.png";
            break;}
        case deviceType_iPadRetina:{
            backName=@"SL_Back90.png";
            closeName=@"SL_Close90.png";
            selectedName=@"SL_Selected50.png";
            nextPageName=@"SL_NextPage40.png";
            break;}
        default:{
            backName=@"SL_Back70.png";
            closeName=@"SL_Close70.png";
            selectedName=@"SL_Selected40.png";
            nextPageName=@"SL_NextPage30.png";
            break;}
    }
    
    SL_BackTexture=[iOSwTextureExt wTextureFromImageWithName:backName];
    SL_CloseTexture=[iOSwTextureExt wTextureFromImageWithName:closeName];
    
    
    SL_BackButton=wButtonNewWithFrame(wRectNew(SLM.OutlineSize, SLM.OutlineSize, SLM.BarButtonSize, SLM.BarButtonSize));
    wButtonSetFunction(SL_BackButton, SL_BackButtonAction);
    wViewAddSubview(SL_Bar, SL_BackButton);
    wGesture*doupleTap=wGestureNew(wGestureDoubleTap);
    wGestureSetFunction(doupleTap, SL_GoToCamera);
    wViewAddGesture(SL_BackButton, doupleTap);

    
    
    SL_Bar_Title=wViewNew();
    wViewSetIsOpaque(SL_Bar_Title, false);
    wViewAddSubviewAtIndex(SL_Bar, SL_Bar_Title, 0);
   
    SL_ArrowTexture=[iOSwTextureExt wTextureFromImageWithName:nextPageName];
    
    SL_SelectionMark=[iOSwTextureExt wTextureFromImageWithName:selectedName];
    
    wAutoLayoutOptions layoutOpt=wAutoLayoutNone;
    layoutOpt.widthOption=wAutoLayoutStretchToSuperView;
    layoutOpt.heightOption=wAutoLayoutStretchToSuperView;
    wViewSetAutoLayoutOptions(SettingsLayer, layoutOpt);
    layoutOpt.heightOption=wAutoLayoutStaticHeight;
    wViewSetAutoLayoutOptions(SL_Bar, layoutOpt);
    
    layoutOpt.widthOption=wAutoLayoutStaticWidth;
    layoutOpt.anchor=wPointNew(0.5, 0.0);
    wViewSetAutoLayoutOptions(SL_Bar_Title, layoutOpt);
    
    SL_TopBar_Border=wViewNewWithFrame(wRectNew(0, barFrame.size.height-GLM.BarBorderSize, barFrame.size.width, GLM.BarBorderSize));
    wAutoLayoutOptions opt=wAutoLayoutNone;
    opt.widthOption=wAutoLayoutStretchToSuperView;
    wViewSetAutoLayoutOptions(SL_TopBar_Border, opt);
    wViewAddSubview(SL_Bar, SL_TopBar_Border);
}

wView*createAboutBlock(){
    wFont aboutBlockFont=[iOSwTextureExt wFontNewFromUIFont:[UIFont fontWithName:@"AmericanTypewriter-CondensedBold" size:SLM.SettingFontSize]];
    
    wAutoLayoutOptions CALOO=wAutoLayoutNone;
    CALOO.anchor=wAnchorNew(0.5, 0.0);
    float aboutBlockWidth=wScreenGetPortraitFrame().size.width;
    float aboutBlockHeight=0.0;
    wView*aboutBlock=wViewNewWithFrame(wRectNew((wScreenGetFrame().size.width-aboutBlockWidth)/2.0, 0, aboutBlockWidth, 1000.0));
    wViewSetAutoLayoutOptions(aboutBlock, CALOO);
    wViewAddSubview(SL_SettingsList, aboutBlock);

    wViewDefaults.isOpaque=false;
    wViewDefaults.backgroundColor=wColorBlack();
    float y=0.0;
    wTexture*MadeByLineTex=[iOSwTextureExt wTextureFromNSString:@"Made by Wael K. Youssef" Font:aboutBlockFont MaxWidth:wScreenGetPortraitFrame().size.width];
    wView*MadeByLine=wViewNewWithFrame(wRectNew((aboutBlockWidth-MadeByLineTex[0].width)/2.0, y, MadeByLineTex[0].width, MadeByLineTex[0].height));
    wViewSetBackgroundMask(MadeByLine, MadeByLineTex);
    wViewAddSubview(aboutBlock, MadeByLine);
    y+=MadeByLineTex[0].height;
    
    aboutBlockHeight=y;
    wViewSetFrame(aboutBlock, wRectNew((wScreenGetFrame().size.width-aboutBlockWidth)/2.0, ((SLM.OutlineSize+SLM.OptionSize)*SM_GetNumberOfSettings())+(SLM.BarSize*1.5), aboutBlockWidth, aboutBlockHeight));
    wViewDefaults.isOpaque=true;
    wViewDefaults.backgroundColor=wColorClear();
    
    return aboutBlock;
}
